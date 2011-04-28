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

    // linkm board commands
    LINKM_CMD_VERSIONGET = 100,  // return linkm version
    LINKM_CMD_STATLEDSET = 101,  // status LED set   (1 args: 1/0)
    LINKM_CMD_STATLEDGET = 102,  // status LED get   (0 args)
    LINKM_CMD_PLAYSET    = 103,  // set params of player state machine
    LINKM_CMD_PLAYGET    = 104,  // get params of  player state machine
    LINKM_CMD_EESAVE     = 105,  // save linkm state to EEPROM
    LINKM_CMD_EELOAD     = 106,  // load linkm state from EEPROM
    LINKM_CMD_GOBOOTLOAD = 107,  // trigger USB bootload
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


int i2c_open(Adaptator** ad);
void i2c_close(Adaptator* ad);
int i2c_write(Adaptator* ad,unsigned char slaveAddr,unsigned char msg[]);
void i2c_read(Adaptator* ad,char* read_buffer);
char* linkm_error_msg(int errCode);
void hexdump(const char* intro, uint8_t *buffer, int len);

#endif
