#include "i2cLib.h"
#include "hiddata.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hexdump(const char* intro, uint8_t *buffer, int len){
    int     i;
    FILE    *fp = stdout;
    if( intro!=NULL ) fprintf(fp, intro,NULL);
    for(i = 0; i < len; i++) {
        fprintf(fp, ((i%16)==0) ? ((i==0)?"":"\n"):" ");
        fprintf(fp, "0x%02x", buffer[i]);
    }
    fprintf(fp, "\n");
}

int linkm_command(Adaptator* ad, int cmd, 
                  int num_send, int num_recv,
                  uint8_t* buf_send, uint8_t* buf_recv){
    uint8_t buf[200]; // FIXME: correct size?  // FIXME: was static
    int len,err;
    if( ad==NULL ) {
        return LINKM_ERR_NOTOPEN;
    }
    memset( buf, 0, sizeof(buf));  // debug: zero everything (while testing)
    buf[0] = 1;            // byte 0 : report id, required by usb functions
    buf[1] = START_BYTE;   // byte 1 : start byte
    buf[2] = cmd;          // byte 2 : command
    buf[3] = num_send;     // byte 3 : num bytes to send (starting at byte 1+4) 
    buf[4] = num_recv;     // byte 4 : num bytes to recv 
    if( buf_send != NULL ) {
        memcpy( buf+5, buf_send, sizeof(buf)-5 );
    }
    // send out the command part of the transaction
    if((err = usbhidSetReport(ad, (char*)buf, REPORT1_SIZE)) != 0) {
        fprintf(stderr, "error writing data: %s\n", linkm_error_msg(err));
        return err;
    }

    // FIXME FIXME FIXME
    // FIXME: shouldn't we always get a response back,
    // so we can view error codes ?
    // also, whats with the buf+3 down there, shouldn't it be buf+2?
    // (don't tell me about report id in byte0, i don't believe it

    // if we should return a response from the command, do it
    if( num_recv !=0 ) {
        // FIXME: maybe put delay in here?
        //usleep( millisleep * 1000); // sleep millisecs waiting for response
        memset(buf, 0, sizeof(buf));  // clear out so to more easily see data
        len = sizeof(buf);
        if((err = usbhidGetReport(ad, 1, (char*)buf, &len)) != 0) {
            fprintf(stderr, "error reading data: %s\n", linkm_error_msg(err));
            return err;
        }
        else {  // it was good
            // byte 0 is report id
            //// byte 0 is transaction counter ( we can ignore)
            // byte 1 is error code   // FIXME: return this?
            // byte 2 is resp_byte 0
            // byte 3 is resp_byte 1
            // ...
            memcpy( buf_recv, buf+2, num_recv );  // byte 0 report id
            return buf[1];                        // byte 1 is error code
        }
    }
    return 0;
}

int i2c_open(Adaptator** ad){
    if( *ad != NULL ) {
       //linkm_close(*ad);
    }
    return usbhidOpenDevice(ad, 
                            IDENT_VENDOR_NUM,  IDENT_VENDOR_STRING,
                            IDENT_PRODUCT_NUM, IDENT_PRODUCT_STRING,
                            1);  // NOTE: '0' means "not using report IDs"
}

void i2c_close(Adaptator* ad){
	 usbhidCloseDevice(ad);
}

int i2c_init(Adaptator* ad){
	int err = linkm_command(ad, LINKM_CMD_I2CINIT, 0,0,  NULL,NULL);
    if( err ) {
		return err;
    }
}

int i2c_write(Adaptator* ad, unsigned char slaveAddr, unsigned char msg[]){
	printf("addr %d: sending :%s\n",slaveAddr,msg);
	unsigned char write_buffer[200];
    unsigned char * write_bufferp = write_buffer;
	int err;
	write_buffer[0]=slaveAddr;
    while(*(msg)!='\0') {
        *(++write_bufferp) = *(msg++);
    }
	err = linkm_command(ad, LINKM_CMD_I2CTRANS, sizeof(msg)+1,0, write_buffer, NULL );
	if(err){
		return err;
	}
	return 0;
}

void i2c_read(Adaptator* ad,char* read_buffer){
	int len,err;
	memset(read_buffer, 0, sizeof(read_buffer));
    len = sizeof(read_buffer);
	usbhidGetReport(ad, 0, (char*)read_buffer, &len);
	printf("%s",read_buffer);
}

char* linkm_error_msg(int errCode)
{
    static char buffer[80];

    switch(errCode){
    case USBOPEN_ERR_ACCESS:      return "Access to device denied";
    case USBOPEN_ERR_NOTFOUND:    return "LinkM, not found";
    case USBOPEN_ERR_IO:          return "Communication error with device";
    case LINKM_ERR_BADSTART:      return "LinkM, bad start byte";
    case LINKM_ERR_BADARGS:       return "LinkM, improper args for command";
    case LINKM_ERR_I2C:           return "LinkM, I2C error";
    case LINKM_ERR_I2CREAD:       return "LinkM, I2C read error";
    case LINKM_ERR_NOTOPEN:       return "LinkM not opened";
    default:
        sprintf(buffer, "Unknown USB error %d", errCode);
        return buffer;
    }
    return NULL;    // not reached 
}

int main(){
	Adaptator* ad = NULL;
	unsigned char msg[] = {'p', '\0'};
	int err;
	if( (err = i2c_open( &ad )) != 0 ) {
        fprintf(stderr, "Error opening the adapter: %s\n", linkm_error_msg(err));
        exit(1);
    }
    if( (err = i2c_init( ad )) != 0 ) {
        fprintf(stderr, "Error initializing i2c: %s\n", linkm_error_msg(err));
        exit(1);
    }
	if( (err= i2c_write(ad,4,msg)) != 0){
		fprintf(stderr, "Error writing to the adapter: %s\n", linkm_error_msg(err));
		exit(1);
	}
	return 0;
}
