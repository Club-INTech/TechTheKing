#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "twi_slave.h"
#include "compteur.h"
 
static uint8_t TWI_buf[TWI_BUFFER_SIZE];
static uint8_t TWI_msgSize = 0;
static uint8_t TWI_state = TWI_NO_STATE;
static uint8_t TWI_busy = 0;


union TWI_statusReg_t TWI_statusReg = {0};


void TWI_Init ()
{
    TWI_slaveAddress = 0x10;

    TWI_Slave_Initialise( (uint8_t) (TWI_slaveAddress<<TWI_ADR_BITS) );

    TWI_Start_Transceiver();
}

uint8_t TWI_Loop ()
{
    // On attend la fin de l'operation en cours.
    if ( ! TWI_Transceiver_Busy() ) {
        
        // On verifie que l'operation a reussi
        if ( TWI_statusReg.lastTransOK ) {
            
            // On verifie qu'on a bien recu quelque chose
            if ( TWI_statusReg.RxDataInBuf ) {
                
                TWI_Get_Data_From_Transceiver(messageBuf, 2);
                
                // Reset les compteurs
                if (messageBuf[0] == TWI_CMD_MASTER_RESET)
                    roue1 = 0;
                    roue2 = 0;
                    
                // Envoi des donnees angle
                if (messageBuf[0] == TWI_CMD_MASTER_DISTANCE) {
                    charger_distance();
                    TWI_Start_Transceiver_With_Data( messageBuf, 4 );
                }

                // Envoi des donnees angle
                if (messageBuf[0] == TWI_CMD_MASTER_ANGLE) {
                    charger_angle();
                    TWI_Start_Transceiver_With_Data( messageBuf, 4 );
                }
                
            }
            // L'operation a foire
            else
                return -1;

            // Demarrer la transmission
            if ( ! TWI_Transceiver_Busy() )
                TWI_Start_Transceiver();
        }

        // La liaison est occupée
        else
            return 1;
        
    }

    return 0;
}

void TWI_Slave_Initialise( unsigned char TWI_ownAddress )
{
  TWAR = TWI_ownAddress;
  TWCR = (1<<TWEN);
  TWI_busy = 0;
}    


uint8_t TWI_Transceiver_Busy( void )
{
  return TWI_busy;
}


uint8_t TWI_Get_State_Info( void )
{
    while ( TWI_Transceiver_Busy() ) {}
    
    return ( TWI_state );
}


void TWI_Start_Transceiver_With_Data( uint8_t *msg, uint8_t msgSize )
{
    uint8_t temp;

    while ( TWI_Transceiver_Busy() ) {}

    TWI_msgSize = msgSize;

    for ( temp = 0; temp < msgSize; temp++ ) {
        TWI_buf[ temp ] = msg[ temp ];
    }

    TWI_statusReg.all = 0;
    TWI_state = TWI_NO_STATE ;
    TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
    TWI_busy = 1;
}


void TWI_Start_Transceiver( void )
{
    while ( TWI_Transceiver_Busy() ) {}

    TWI_statusReg.all = 0;
    TWI_state = TWI_NO_STATE ;
    TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
    TWI_busy = 0;
}


uint8_t TWI_Get_Data_From_Transceiver( uint8_t *msg, uint8_t msgSize )
{
    uint8_t i;

    while ( TWI_Transceiver_Busy() ) {}

    if( TWI_statusReg.lastTransOK ) {

        for ( i=0; i<msgSize; i++ )
            msg[ i ] = TWI_buf[ i ];

        TWI_statusReg.RxDataInBuf = 0;
    }

    return (TWI_statusReg.lastTransOK);
}


ISR (TWI_vect)
{
  static uint8_t TWI_bufPtr;
  
  switch (TWSR) {
      
    case TWI_STX_ADR_ACK:
        TWI_bufPtr   = 0;

    case TWI_STX_DATA_ACK:
        TWDR = TWI_buf[TWI_bufPtr++];
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
        TWI_busy = 1;
        break;
        
    case TWI_STX_DATA_NACK:
        if (TWI_bufPtr == TWI_msgSize)
            TWI_statusReg.lastTransOK = 1;
        else
            TWI_state = TWSR;
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
        TWI_busy = 0;
        break;
      
    case TWI_SRX_GEN_ACK:
        TWI_statusReg.genAddressCall = 1;
        
    case TWI_SRX_ADR_ACK:
        TWI_statusReg.RxDataInBuf = 1;
        TWI_bufPtr = 0;
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
        TWI_busy = 1;
        break;
        
    case TWI_SRX_ADR_DATA_ACK:
    case TWI_SRX_GEN_DATA_ACK:
        TWI_buf[TWI_bufPtr++] = TWDR;
        TWI_statusReg.lastTransOK = 1;
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA); 
        TWI_busy = 1;
        break;
        
    case TWI_SRX_STOP_RESTART:
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
        TWI_busy = 0;
        break;
        
    case TWI_SRX_ADR_DATA_NACK:
    case TWI_SRX_GEN_DATA_NACK:
    case TWI_STX_DATA_ACK_LAST_BYTE:
    case TWI_BUS_ERROR:
        TWI_state = TWSR;
        TWCR =   (1<<TWSTO)|(1<<TWINT);
        break;

    default:     
        TWI_state = TWSR;     
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
        TWI_busy = 0;
    }
}
