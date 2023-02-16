/*
 * gpio.h
 *
 *  Created on: 27 Dec. 2022
 *      Author: 30060006
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
