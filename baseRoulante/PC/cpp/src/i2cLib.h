#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "hiddata.h"

// NOTE: these values much be the same as those in 
//       "linkm/firmware/usbconfig.h"           and 
//       "linkm/bootloadHID/firmware/usbconfig.h"
//#define IDENT_VENDOR_NUM        0x16C0
//#define IDENT_PRODUCT_NUM       0x05DF

#define USBOPEN_SUCCESS         0   /* no error */
#define USBOPEN_ERR_ACCESS      1   /* not enough permissions to open device */
#define USBOPEN_ERR_IO          2   /* I/O error */
#define USBOPEN_ERR_NOTFOUND    3   /* device not found */
#define IDENT_VENDOR_NUM        0x20A0
#define IDENT_PRODUCT_NUM       0x4110
#define IDENT_VENDOR_STRING     "ThingM"
#define IDENT_PRODUCT_STRING    "LinkM"

// gotta be one more than the size, for the reportid
#define REPORT1_SIZE        17
//#define REPORT1_SIZE        16

#define START_BYTE          0xDA

// Command byte values for linkm_command()
enum { 
    LINKM_CMD_NONE     = 0,      // no command, do not use
    // I2C commands
    LINKM_CMD_I2CTRANS = 1,      // i2c read & write (N args: addr + other)
    LINKM_CMD_I2CWRITE = 2,      // i2c write to dev (N args: addr + other)
    LINKM_CMD_I2CREAD  = 3,      // i2c read         (1 args: addr)
    LINKM_CMD_I2CSCAN  = 4,      // i2c bus scan     (2 args: start,end)
    LINKM_CMD_I2CCONN  = 5,      // i2c connect/disc (1 args: 1/0)
    LINKM_CMD_I2CINIT  = 6,      // i2c init         (0 args: )
};

// Return values for linkm_command()
enum { 
    LINKM_ERR_NONE     =   0,
    LINKM_ERR_BADSTART = 101,
    LINKM_ERR_BADARGS,
    LINKM_ERR_I2C,
    LINKM_ERR_I2CREAD,
    LINKM_ERR_NOTOPEN = 199,
};

typedef struct usbDevice Adaptator;

char* linkm_error_msg(int errCode);
void hexdump(const char* intro, uint8_t *buffer, int len);


/*********************************************************/

#define ADRESSE_ACTIONNEURS     0X10
#define ADRESSE_CAPTEURS        0

#define HAUTEUR_GAUCHE          0X41
#define HAUTEUR_DROITE          0X42
#define HAUTEUR_DEUX            0X4B
#define ANGLE_GAUCHE            0X11
#define ANGLE_DROITE            0X12
#define AIMANT_GAUCHE_HAUT      0X21
#define AIMANT_DROIT_HAUT       0X22
#define AIMANT_GAUCHE_BAS       0X31
#define AIMANT_DROIT_BAS        0X32

int i2c_open(Adaptator** ad);
void i2c_close(Adaptator* ad);
int i2c_order(Adaptator* ad, unsigned char slave_addr, unsigned char msg_array[], unsigned int msg_size);
int i2c_request(Adaptator* ad, unsigned char slave_address, unsigned char request, char* read_buffer, unsigned int read_size);

/*********************************************************/


#endif
