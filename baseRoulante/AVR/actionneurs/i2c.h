/*
  Librairie I2C
  Largement repompée sur celle d'Arduino
  Couche C++ éliminée
*/

#ifndef i2c_h
#define i2c_h

  #include <stdint.h>

  //#define ATMEGA8

  #define CPU_FREQ 16000000L
  #define I2C_FREQ 100000L  
  #define I2C_BUFFER_LENGTH 32

  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

  #define I2C_READY 0
  #define I2C_MRX   1
  #define I2C_MTX   2
  #define I2C_SRX   3
  #define I2C_STX   4

  // Fonctions bas niveau
  // Ne pas utiliser
  
  void i2c_init(void);
  void i2c_setAddress(uint8_t);
  uint8_t i2c_readFrom(uint8_t, uint8_t*, uint8_t);
  uint8_t i2c_writeTo(uint8_t, uint8_t*, uint8_t, uint8_t);
  uint8_t i2c_transmit(uint8_t*, uint8_t);
  void i2c_attachSlaveRxEvent( void (*)(uint8_t*, int) );
  void i2c_attachSlaveTxEvent( void (*)(void) );
  void i2c_reply(uint8_t);
  void i2c_stop(void);
  void i2c_releaseBus(void);

  // Fonctions haut niveau
  // S'utilise comme les équivalents Arduino
  // Regarder les exemple pour le fonctionnement

  void i2c_beginMaster(void);
  void i2c_beginSlave(uint8_t address);
  uint8_t i2c_requestFrom(uint8_t address, uint8_t quantity);
  void i2c_beginTransmission(uint8_t address);
  uint8_t i2c_endTransmission(void);
  void i2c_sendArray(uint8_t* data, uint8_t quantity);
  void i2c_send(uint8_t data);
  uint8_t i2c_available(void);
  uint8_t i2c_receive(void);
  void i2c_onReceiveService(uint8_t* inBytes, int numBytes);
  void i2c_onRequestService(void);
  void i2c_onReceive( void (*function)(int) );
  void i2c_onRequest( void (*function)(void) );
  
#endif