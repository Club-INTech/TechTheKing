/*
  Le code est largement et honteusement repompé des libs Arduino
  Elimination du c++, includes, types inutiles
  Commentaires passés auf franzosisch au fil de ma lecture
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "i2c.h"

// Variables pour les fonctions bas niveau

static volatile uint8_t i2c_state;
static uint8_t i2c_slarw;

static void (*i2c_onSlaveTransmit)(void);
static void (*i2c_onSlaveReceive)(uint8_t*, int);

static uint8_t* i2c_masterBuffer;
static volatile uint8_t i2c_masterBufferIndex;
static uint8_t i2c_masterBufferLength;

static uint8_t* i2c_txBuffer;
static volatile uint8_t i2c_txBufferIndex;
static volatile uint8_t i2c_txBufferLength;

static uint8_t* i2c_rxBuffer;
static volatile uint8_t i2c_rxBufferIndex;

static volatile uint8_t i2c_error;

// Variables pour le haut niveau

uint8_t* rxBuffer = 0;
uint8_t rxBufferIndex = 0;
uint8_t rxBufferLength = 0;

uint8_t txAddress = 0;
uint8_t* txBuffer = 0;
uint8_t txBufferIndex = 0;
uint8_t txBufferLength = 0;

uint8_t transmitting = 0;

// Pointeur vers les fonctions de requête et réception

void (*user_onRequest) (void);
void (*user_onReceive) (int);

// Fonctions haut niveau

void i2c_beginMaster(void)
{
  // initialisation du buffer pour la réception
  rxBuffer = (uint8_t*) calloc(I2C_BUFFER_LENGTH, sizeof(uint8_t));
  rxBufferIndex = 0;
  rxBufferLength = 0;

  // initialisation du buffer pour la transmission
  txBuffer = (uint8_t*) calloc(I2C_BUFFER_LENGTH, sizeof(uint8_t));
  txBufferIndex = 0;
  txBufferLength = 0;

  i2c_init();
}

void i2c_beginSlave(uint8_t address)
{
  i2c_setAddress(address);
  i2c_attachSlaveTxEvent(i2c_onRequestService);
  i2c_attachSlaveRxEvent(i2c_onReceiveService);
  i2c_beginMaster();
}

uint8_t i2c_requestFrom(uint8_t address, uint8_t quantity)
{
  // buffer plein
  if(quantity > I2C_BUFFER_LENGTH)
  {
    quantity = I2C_BUFFER_LENGTH;
  }
  
  // lecture du buffer
  uint8_t read = i2c_readFrom(address, rxBuffer, quantity);
  
  // reset réception
  rxBufferIndex = 0;
  rxBufferLength = read;

  return read;
}

void i2c_beginTransmission(uint8_t address)
{
  transmitting = 1;
  
  // adresse du slave cible
  txAddress = address;
  
  // reset transmission
  txBufferIndex = 0;
  txBufferLength = 0;
}

uint8_t i2c_endTransmission(void)
{
  // on vide le buffer
  int8_t ret = i2c_writeTo(txAddress, txBuffer, txBufferLength, 1);
  
  // reset et fin transmission
  txBufferIndex = 0;
  txBufferLength = 0;
  transmitting = 0;
  return ret;
}

void i2c_sendArray(uint8_t* data, uint8_t quantity)
{
  if(transmitting) // on est master
  {
  uint8_t i;
    for(i = 0; i < quantity; ++i)
	{
      i2c_send(data[i]);
    }
  }
  else // on est slave
  {
    i2c_transmit(data, quantity);
  }
}

void i2c_send(uint8_t data)
{
  if(transmitting) // on est master
  {
    // buffer plein
    if(txBufferLength >= I2C_BUFFER_LENGTH)
	{
      return;
	}
	
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    txBufferLength = txBufferIndex;
  }
  else // on est slave
  {
    i2c_transmit(&data, 1);
  }
}

uint8_t i2c_available(void)
{
  return rxBufferLength - rxBufferIndex;
}

uint8_t i2c_receive(void)
{
  // si jamais on nous envoie un string
  uint8_t value = '\0';

  if(rxBufferIndex < rxBufferLength)
  {
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

void i2c_onReceiveService(uint8_t* inBytes, int numBytes)
{
  // pas pour nous
  if(!user_onReceive)
  {
    return;
  }

  // on a pas lu tout le buffer!
  if(rxBufferIndex < rxBufferLength)
  {
    return;
  }

  // on récupère le contenu du buffer i2c
  uint8_t i;
  
  for(i = 0; i < numBytes; ++i)
  {
    rxBuffer[i] = inBytes[i];
  }
  
  // Reset réception
  rxBufferIndex = 0;
  rxBufferLength = numBytes;
  
  user_onReceive(numBytes);
}

void i2c_onRequestService(void)
{
  // pas pour nous
  if(!user_onRequest)
  {
    return;
  }
  
  // Reset transmission
  txBufferIndex = 0;
  txBufferLength = 0;
  
  user_onRequest();
}

void i2c_onReceive( void (*function)(int) )
{
  user_onReceive = function;
}

void i2c_onRequest( void (*function)(void) )
{
  user_onRequest = function;
}

// Fonctions bas niveau

void i2c_init(void)
{
  i2c_state = I2C_READY;

  // activer pull-ups internes
  sbi(PORTC, 4);
  sbi(PORTC, 5);

  // vitesse de communication
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  TWBR = ((CPU_FREQ / I2C_FREQ) - 16) / 2;

  // activer l'i2c
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);

  // buffers
  i2c_masterBuffer = (uint8_t*) calloc(I2C_BUFFER_LENGTH, sizeof(uint8_t));
  i2c_txBuffer = (uint8_t*) calloc(I2C_BUFFER_LENGTH, sizeof(uint8_t));
  i2c_rxBuffer = (uint8_t*) calloc(I2C_BUFFER_LENGTH, sizeof(uint8_t));
}

void i2c_setAddress(uint8_t address)
{
  TWAR = address << 1;
}

uint8_t i2c_readFrom(uint8_t address, uint8_t* data, uint8_t length)
{
  uint8_t i;

  // on vérifie que ça rentre
  if(I2C_BUFFER_LENGTH < length)
  {
    return 0;
  }

  // on attend d'être prêts
  while(I2C_READY != i2c_state)
  {
    continue;
  }
  
  i2c_state = I2C_MRX;
  i2c_error = 0xFF;

  i2c_masterBufferIndex = 0;
  i2c_masterBufferLength = length-1;

  // on se met en lecture
  i2c_slarw = TW_READ;
  i2c_slarw |= address << 1;

  // on démarre la com
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);

  // on attend d'avoir fini
  while(I2C_MRX == i2c_state)
  {
    continue;
  }

  // si c'est trop long, on coupe
  if (i2c_masterBufferIndex < length)
  {
    length = i2c_masterBufferIndex;
  }

  // on récupère le buffer
  for(i = 0; i < length; ++i)
  {
    data[i] = i2c_masterBuffer[i];
  }
	
  return length;
}

uint8_t i2c_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait)
{
  uint8_t i;

  // si on a les yeux plus gros que le ventre
  if(I2C_BUFFER_LENGTH < length)
  {
    return 1;
  }

  // on attend d'êtres prêts
  while(I2C_READY != i2c_state)
  {
    continue;
  }
  
  i2c_state = I2C_MTX;
  i2c_error = 0xFF;

  i2c_masterBufferIndex = 0;
  i2c_masterBufferLength = length;
  
  // on balance le message dans le buffer
  for(i = 0; i < length; ++i)
  {
    i2c_masterBuffer[i] = data[i];
  }
  
  // on se met en écriture
  i2c_slarw = TW_WRITE;
  i2c_slarw |= address << 1;
  
  // on démarre la com
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);

  // on attend d'avoir fini
  while(wait && (I2C_MTX == i2c_state))
  {
    continue;
  }
  
  if (i2c_error == 0xFF)
    return 0;	// success
  else if (i2c_error == TW_MT_SLA_NACK)
    return 2;	// le slave n'accuse pas réception de son adresse
  else if (i2c_error == TW_MT_DATA_NACK)
    return 3;	// le slave n'accuse pas réception des données
  else
    return 4;	// fails divers
}

uint8_t i2c_transmit(uint8_t* data, uint8_t length)
{
  uint8_t i;

  // ça rentre?
  if(I2C_BUFFER_LENGTH < length)
  {
    return 1;
  }
  
  // en slave, on vérifie qu'on a le droit de l'ouvrir avant de parler
  if(I2C_STX != i2c_state)
  {
    return 2;
  }
  
  // on écrit dans le buffer de transmission
  i2c_txBufferLength = length;
  for(i = 0; i < length; ++i)
  {
    i2c_txBuffer[i] = data[i];
  }
  
  return 0;
}

void i2c_attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
  i2c_onSlaveReceive = function;
}

void i2c_attachSlaveTxEvent( void (*function)(void) )
{
  i2c_onSlaveTransmit = function;
}

void i2c_reply(uint8_t ack)
{
  if(ack)
  {
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
  }
  else
  {
	  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
  }
}

void i2c_stop(void)
{
  // on coupe tout
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);

  // on attend l'envoi du bit de stop
  while(TWCR & _BV(TWSTO))
  {
    continue;
  }

  i2c_state = I2C_READY;
}

void i2c_releaseBus(void)
{
  // on arrête la transmission
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);

  i2c_state = I2C_READY;
}

ISR (TWI_vect)
{
  // Achtung! C'est LE gros morceau
  
  switch(TW_STATUS)
  {
    // Master
    case TW_START:     // start envoyé
    case TW_REP_START: // repeated start envoyé
      // on copy l'adresse du reçue et on envoie ack
      TWDR = i2c_slarw;
      i2c_reply(1);
      break;

    // Transmission en master
    case TW_MT_SLA_ACK:  // le slave envoie ack pour une adresse
    case TW_MT_DATA_ACK: // le slave envoie ack pour des données
      // on envoi les données à envoyer si besoin
      if(i2c_masterBufferIndex < i2c_masterBufferLength)
	  {
        // on balance les données dans le buffer et on envoie ack
        TWDR = i2c_masterBuffer[i2c_masterBufferIndex++];
        i2c_reply(1);
      }
      else
	  {
        i2c_stop();
      }
      break;
    case TW_MT_SLA_NACK:  // on reçoit nack pour une adresse
      i2c_error = TW_MT_SLA_NACK;
      i2c_stop();
      break;
    case TW_MT_DATA_NACK: // on reçoit nack pour des données
      i2c_error = TW_MT_DATA_NACK;
      i2c_stop();
      break;
    case TW_MT_ARB_LOST: // on perd le contrôle du bus
      i2c_error = TW_MT_ARB_LOST;
      i2c_releaseBus();
      break;

    // Réception en master
    case TW_MR_DATA_ACK: // on a reçu les données, on envoie ack
      // on les met dans le buffer
      i2c_masterBuffer[i2c_masterBufferIndex++] = TWDR;
    case TW_MR_SLA_ACK:  // ack après envoi de l'adresse
      // si on attend d'autres octets, on accuse réception
      if(i2c_masterBufferIndex < i2c_masterBufferLength)
	  {
        i2c_reply(1);
      }
      else
	  {
        i2c_reply(0);
      }
      break;
    case TW_MR_DATA_NACK: // pas d'ack après envoi de données
      i2c_masterBuffer[i2c_masterBufferIndex++] = TWDR;
    case TW_MR_SLA_NACK: // pas d'ack après envoi de données
      i2c_stop();
      break;

	// Slave
	
    // Réception en slave
    case TW_SR_SLA_ACK:   // adresse reçue, ack renvoyé
    case TW_SR_GCALL_ACK: // broadcast, ack renvoyé
	case TW_SR_ARB_LOST_SLA_ACK:   // on perd le contrôle du bus, ack renvoyé
    case TW_SR_ARB_LOST_GCALL_ACK: // on perd le contrôle du bus, ack renvoyé
      // on passe en mode réception
      i2c_state = I2C_SRX;
      // on accepte l'écriture sur le buffer en on envoie ack
      i2c_rxBufferIndex = 0;
      i2c_reply(1);
      break;
    case TW_SR_DATA_ACK:       // données reçues, ack renvoyé
    case TW_SR_GCALL_DATA_ACK: // données reçues en broadcast, ack renvoyé
      // le buffer de réception n'est pas vide
      if(i2c_rxBufferIndex < I2C_BUFFER_LENGTH)
	  {
        // écriture dans le buffer et on renvoie ack
        i2c_rxBuffer[i2c_rxBufferIndex++] = TWDR;
        i2c_reply(1);
      }
      else
	  {
        // on renvoie nack
        i2c_reply(0);
      }
      break;
    case TW_SR_STOP: // réception d'un stop our repeated start
      if(i2c_rxBufferIndex < I2C_BUFFER_LENGTH)
	  {
        i2c_rxBuffer[i2c_rxBufferIndex] = '\0';
      }
      i2c_onSlaveReceive(i2c_rxBuffer, i2c_rxBufferIndex);
      i2c_reply(1);
      i2c_state = I2C_READY;
      break;
    case TW_SR_DATA_NACK:       // réception de données, on retourne nack
    case TW_SR_GCALL_DATA_NACK: // réception de données en broadcast, on retourne nack
      i2c_reply(0);
      break;
    
    // Transmission en slave
    case TW_ST_SLA_ACK:          // adresse reçue, ack renvoyé
    case TW_ST_ARB_LOST_SLA_ACK: // on perd le contrôle du bus, ack renvoyé
      // le slave passe en transmission
      i2c_state = I2C_STX;
      // ready the tx buffer index for iteration
      i2c_txBufferIndex = 0;
      // set tx buffer length to be zero, to verify if user changes it
      i2c_txBufferLength = 0;
      // request for txBuffer to be filled and length to be set
      // note: user must call i2c_transmit(bytes, length) to do this
      i2c_onSlaveTransmit();
      // if they didn't change buffer & length, initialize it
      if(0 == i2c_txBufferLength)
	  {
        i2c_txBufferLength = 1;
        i2c_txBuffer[0] = 0x00;
      }
      // transmit first byte from buffer, fall
    case TW_ST_DATA_ACK: // byte sent, ack returned
      // copy data to output register
      TWDR = i2c_txBuffer[i2c_txBufferIndex++];
      // if there is more to send, ack, otherwise nack
      if(i2c_txBufferIndex < i2c_txBufferLength)
	  {
        i2c_reply(1);
      }
      else
	  {
        i2c_reply(0);
      }
      break;
    case TW_ST_DATA_NACK: // on a reçu nack, comm terminée
    case TW_ST_LAST_DATA: // on a reçu ack, mais on a terminé
      i2c_reply(1);
      i2c_state = I2C_READY;
      break;

    // Autres
    case TW_NO_INFO:   // pas d'information particulière
      break;
    case TW_BUS_ERROR: // erreur
      i2c_error = TW_BUS_ERROR;
      i2c_stop();
      break;
  }
}

