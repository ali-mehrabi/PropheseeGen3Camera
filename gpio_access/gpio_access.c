/*
This program demonstrate how to use hps communicate with FPGA i2c, spi, and gpio
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"
#include "gpio.h"
#include "prophesee.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define SPI_CLK   0x01  // LED0 PIN_V10
#define SPI_MOSI  0x02  // LED1 PIV15
#define SPI_CCS_N 0x01  // LED2 PIW15  clk = 1 , cs_n = 0
#define SPI_IDLE  0x05  // clk = 1 , cs_n = 1
#define SET       0xFF
#define RESET     0x00
#define SPI_PERIOD  10 // 1 = 10 microseconds

void *h2p_lw_led_addr;

/* Pin mapping
0 0 0 0   0 SPI_CCS  SPI_MOSI   SPI_CLK */

void nop(int i)
{
	usleep(10*i);
}

char get_mosi_bit(char d)
{
	if(d & 0x80)
		return SPI_MOSI;
	else
		return RESET;
}

char set_clk(char d)
{
   return d | SPI_CLK;
}

char chip_select(void)
{
	return SPI_CCS_N;
}
char chip_deselect(void)
{
	return SPI_IDLE; // clk = 1, css = 1
}


void spi_write(uint32_t address, uint32_t data)
{
	uint8_t c, d;
	uint8_t i,j;
	uint32_t shd;
	shd = address;
	int f = SPI_PERIOD/2;
	*(uint32_t *)h2p_lw_led_addr = chip_select();
	nop(4*f);
	for(j = 0 ;j < 4; j++)
	{
	  c = (shd & 0xff000000) >> 24;
	  for(i = 0 ; i < 8; i++)
	   {
		d = get_mosi_bit(c);
		*(uint32_t *)h2p_lw_led_addr = d;
		nop(f);
		*(uint32_t *)h2p_lw_led_addr = set_clk(d);
		nop(f);
		 c = c << 1;
	   }
	 shd =  shd << 8;
    }

	shd = data;
	for(j = 0 ;j < 4; j++)
	{
	  c = (shd & 0xff000000)>> 24;
	  for(i = 0 ; i < 8; i++)
	   {
		d = get_mosi_bit(c);
		*(uint32_t *)h2p_lw_led_addr = d;
		nop(f);
		*(uint32_t *)h2p_lw_led_addr = set_clk(d);
		 nop(f);
		 c = c <<1;
	   }
	 shd  = shd << 8;
    }
  nop(2*f);
}

void config_camera(void)
{
	spi_write(CLK_CONTROL,0x00000001);
	spi_write(SPARE_CONTROL,0x00000100);
	spi_write(0x00000000,0x00000010);
	spi_write(0x00000000,0x00000018);
	spi_write(0x00000000,0x0000001a);
	spi_write(0x00000040,0x00140501);
	spi_write(0x00000100,0x4903e1b0);
	spi_write(0x00000104,0x4903e1b0);
	spi_write(ROI_X00,0xffffffff);
	spi_write(ROI_X01,0xffffffff);
	spi_write(ROI_X02,0xffffffff);
	spi_write(ROI_X03,0xffffffff);
	spi_write(ROI_X04,0xffffffff);
	spi_write(ROI_X05,0xffffffff);
	spi_write(ROI_X06,0xffffffff);
	spi_write(ROI_X07,0xffffffff);
	spi_write(ROI_X08,0xffffffff);
	spi_write(ROI_X09,0xffffffff);
	spi_write(ROI_X10,0xffffffff);
	spi_write(ROI_X11,0xffffffff);
	spi_write(ROI_X12,0xffffffff);
	spi_write(ROI_X13,0xffffffff);
	spi_write(ROI_X14,0xffffffff);
	spi_write(ROI_X15,0xffffffff);
	spi_write(ROI_X16,0xffffffff);
	spi_write(ROI_X17,0xffffffff);
	spi_write(ROI_X18,0xffffffff);
	spi_write(ROI_X19,0xffffffff);
	spi_write(0x00000250,0xffffffff);
	spi_write(ROI_Y00,0xffffffff);
	spi_write(ROI_Y01,0xffffffff);
	spi_write(ROI_Y02,0xffffffff);
	spi_write(ROI_Y03,0xffffffff);
	spi_write(ROI_Y04,0xffffffff);
	spi_write(ROI_Y05,0xffffffff);
	spi_write(ROI_Y06,0xffffffff);
	spi_write(ROI_Y07,0xffffffff);
	spi_write(ROI_Y08,0xffffffff);
	spi_write(ROI_Y09,0xffffffff);
	spi_write(ROI_Y10,0xffffffff);
	spi_write(ROI_Y11,0xffffffff);
	spi_write(ROI_Y12,0xffffffff);
	spi_write(ROI_Y13,0xffffffff);
	spi_write(ROI_Y14,0xffffffff);
	spi_write(0x0000033c,0xffffffff);
	spi_write(0x00000250,0x00000000);
	spi_write(0x0000033c,0x00000000);
	spi_write(ROI_CONTROL,0x0000002b);
	spi_write(0x00000008,0x400203ff);
	spi_write(0x00000044,0x400203ff);
	spi_write(SPARE_CONTROL,0x10000101);
	spi_write(0x0000011c,0x49014dc0);
	spi_write(BIAS_DIFF_OFF_CONTROL,0x7900571f);
	spi_write(BIAS_DIFF_ON_CONTROL,0x79125c34);
	spi_write(0x00000154,0x7902ce2a);
	spi_write(BIAS_DIFF_ON_CONTROL,0x79125c33);
	spi_write(BIAS_DIFF_CONTROL,0x7902ce29);
	spi_write(0x00000020,0x00000001);
	spi_write(0x00000168,0x61000000);
}

void cam_roi_setup(void)
{
	spi_write(ROI_X00,0x00000000);
	spi_write(ROI_X01,0x00000000);
	spi_write(ROI_X02,0xffffffff);
	spi_write(ROI_X03,0xffffffff);
	spi_write(ROI_X04,0xffffffff);
	spi_write(ROI_X05,0xffffffff);
	spi_write(ROI_X06,0xffffffff);
	spi_write(ROI_X07,0xffffffff);
	spi_write(ROI_X08,0xffffffff);
	spi_write(ROI_X09,0xffffffff);
	spi_write(ROI_X10,0xffffffff);
	spi_write(ROI_X11,0xffffffff);
	spi_write(ROI_X12,0xffffffff);
	spi_write(ROI_X13,0xffffffff);
	spi_write(ROI_X14,0xffffffff);
	spi_write(ROI_X15,0xffffffff);
	spi_write(ROI_X16,0xffffffff);
	spi_write(ROI_X17,0xffffffff);
	spi_write(ROI_X18,0x00000000);
	spi_write(ROI_X19,0x00000000);
	spi_write(ROI_Y00,0x00000000);
	spi_write(ROI_Y01,0x00000000);
	spi_write(ROI_Y02,0xffffffff);
	spi_write(ROI_Y03,0xffffffff);
	spi_write(ROI_Y04,0xffffffff);
	spi_write(ROI_Y05,0xffffffff);
	spi_write(ROI_Y06,0xffffffff);
	spi_write(ROI_Y07,0xffffffff);
	spi_write(ROI_Y08,0xffffffff);
	spi_write(ROI_Y09,0xffffffff);
	spi_write(ROI_Y10,0xffffffff);
	spi_write(ROI_Y11,0xffffffff);
	spi_write(ROI_Y12,0xffffffff);
	spi_write(ROI_Y13,0x00000000);
	spi_write(ROI_Y14,0x00000000);
	spi_write(ROI_CONTROL, 0x0000002b);
	spi_write(CLK_CONTROL, 0x00000003);
}

void cam_pixel_setup(void)
{
	spi_write(EVENT_FILTER_CONTROL,0x00000079); //60Mhz clk
	spi_write(THRESHOLD_CONTROL,0x00190080); // 100evt/100us
	spi_write(BIAS_PR_CONTROL,0x510BB8B1); //IDAC=3000
	spi_write(BIAS_DIFF_CONTROL,0x7902ce29);
	spi_write(BIAS_FO_CONTROL, 0x610040D6);
	spi_write(BIAS_DIFF_ON_CONTROL, 0x79125C34);
    spi_write(BIAS_DIFF_OFF_CONTROL, 0x7900571E);
    spi_write(BIAS_HPF, 0x51002FD4);
}

int main() {

//	uint8_t  i2c_len_reg = 0xfd;
//	uint8_t *len;
//	uint8_t  data_block[254];
//  uint8_t  gps_packet[256];
     open_gpio();
     open_i2c();
	 gpio_ddr= (uint32_t*) (base_gpios + 0x4);
	*gpio_ddr = USER_IO_DIR ; // configuring GPIO54 as output
	 gpios = (uint32_t*) base_gpios;

	void *virtual_base;
	int fd;
	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span

	if( (fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );
	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}
	

	h2p_lw_led_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + LED_PIO_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	*(uint32_t *)h2p_lw_led_addr = chip_deselect();


	/* test
		while(1){
		//	*(uint32_t *)h2p_lw_led_addr = chip_select();
			//spi_write(0xaaaaaaaa,0xcccc3333);
			*(uint32_t *)h2p_lw_led_addr = chip_select();
			nop(50);
			spi_write(0xaaaaffff, 0x00555aaa);
			nop(50);
			*(uint32_t *)h2p_lw_led_addr = chip_deselect();
			nop(150);
		}
	*/

	*(uint32_t *)h2p_lw_led_addr = chip_select();
	nop(10);
	config_camera();
	nop(100);
	//cam_roi_setup();
	//nop(100);
	//cam_pixel_setup();
	nop(10);
    *(uint32_t *)h2p_lw_led_addr = chip_deselect();
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close(fd);
		return 1;
	}


	/// just an indicator to show config is done
		for(uint8_t i = 0; i < 5; i++)
		{
		   usleep(100000);
		   *gpios = LED_ON;
	       usleep(100000);
	       *gpios = LED_OFF;
	    }

	close(fd);

	return 0;
}
