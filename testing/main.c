/*
 ============================================================================
 Name        : test GPIO and GPS unit
 Author      : Ali Mehrabi
 Version     :
 Copyright   :
 Description : NEO-M8M I2C registers start @ 0xfd, if content is not 0xff new data is available.
               Maximum read length 256 bytes.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <sys/mman.h>
#include <stdint.h>
#include <linux/i2c-dev.h>

#define USER_IO_DIR     (0x01000000)  // gpio53 = input  gpio54= output  bit(53-29)
#define LED_ON          (0x01000000)  // turn gpio54 on
#define LED_OFF         (0x00000000)  // turn gpio54 off
#define REG_BASE_GPIO    0xFF709000   // ARM GPIO1 Base Address
#define REG_GPIO_DDR     0xFF709004   // ARM GPIO1 Data Direction Register Address
#define REG_GPIO_DR      0xFF709004   // ARM GPIO0 Data Register Address
#define REG_SPAN_GPIOS 	 0x00001000   // ARM GPIO1 Address span
#define GPS_I2C_ADDR     0x42
#define GPIO_DEV        "/dev/mem"     // GPIO DEV Memory Map
#define I2C_DEV         "/dev/i2c-1"   // I2C-1 DEV

void *virtual_base;
void *base_gpios;
void*   led_addr;
void*   sw_addr;
int     gpio_fd;
int     i2c_fd;

uint32_t  *gpio_data;
uint32_t  *gpios;
uint32_t  *gpio_ddr;
//uint8_t    i2c_slave_addr = 0x42;
uint8_t   *data;

uint8_t open_gpio()
{
	if(	(gpio_fd = open(GPIO_DEV, O_RDWR | O_SYNC))<0)
	  {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return 0xff;
	  }
	base_gpios = mmap(NULL, REG_SPAN_GPIOS, PROT_READ | PROT_WRITE, MAP_SHARED, gpio_fd, REG_BASE_GPIO);
	if( base_gpios == MAP_FAILED )
	  {
		printf( "ERROR: mmap() failed...\n" );
		close( gpio_fd );
		return 0xfe;
	  }
	return 0x00;
}

uint8_t open_i2c(void)
{
	if ((i2c_fd = open(I2C_DEV, O_RDWR)) < 0)
	  {
		printf("Failed to open the i2c bus\n");
		return 0xfb;
	  }

	if(ioctl(i2c_fd, I2C_SLAVE, GPS_I2C_ADDR) < 0)
	{
		printf("ERROR setting I2C slave address\n");
		close(i2c_fd);
		return 0xfa;
	}
	return 0x00;
}

uint8_t *read_i2c_message_len(int file, uint8_t reg)
  {
    static uint8_t data[2];
    write(file, &reg, 1);
    read(file, data, 2);
	return data;
  }

int main(int argc, char *argv[])
  {
	uint16_t i;
	//uint8_t base_reg = 0xff;
	uint8_t  i2c_len_reg = 0xfd;
	uint8_t *len;
	uint8_t  data_block[254];
    uint8_t  gps_packet[256];
    open_gpio();
    open_i2c();


	 gpio_ddr= (uint32_t*) (base_gpios + 0x4);
	*gpio_ddr = USER_IO_DIR ; // configuring GPIO54 as output
	 gpios = (uint32_t*) base_gpios;

	for(i = 0; i < 3; i++)
	{
	   usleep(100000);
	   *gpios = LED_ON;
       usleep(100000);
       *gpios = LED_OFF;
    }

	for( i = 0; i< 7; i++)
	{
	   usleep(50000);
	   *gpios = LED_ON;
       usleep(50000);
       *gpios = LED_OFF;
    }

// starting i2c packet read loop
	for(uint16_t k = 0; k<50; k++)
	{
	  len = read_i2c_message_len(i2c_fd, i2c_len_reg);
	  if(*len != 0xff && *(len+1) != 0xff)
    	{
		 // write(i2c_fd, &base_reg,1);
	      read(i2c_fd, data_block, 254);
	      *(gps_packet) = *len;
	      *(gps_packet+1) = *(len+1);
          memcpy(gps_packet+2, data_block, 254*sizeof(uint8_t));
	 //print gps message:
	     for(i=0; i < 256; i++)
		  // printf("%02X  ", gps_packet[i]);
	       printf("%C", gps_packet[i]);

         printf("\n");
         printf("\n");
    	}
      usleep(200000);
	}
    /*printf("\033[A\33[2K\r"); // delete line back home*/
return 0;
}


