#define _AFXDLL
#include "stdafx.h"
#define FTD3XX_STATIC
#include "usb3.h"
#include<iostream>
#include "FTD3XX.h"
#include <initguid.h> 
#include <string>
#include <cassert>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <thread>
#include <fstream> // for test file 
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define  BUFFER_SIZE   4096
#define  NCOL          640
#define  NROW          480
#define  FRAME_SIZE    NCOL*NROW 
#define  ONPIXEL       0xFF
#define  OFFPIXEL      0x3F
#define  ONCOLOR       {255,205,153} //{BGR} lb
#define  OFFCOLOR      {0,0,255}     // Red
#define  DARK          {0,0,0}      // Black
#define  INTENSITY     37

using namespace cv;
using namespace std;

uint16_t  xaddr;
uint16_t  yaddr;
uint32_t  loc;
uint8_t   dpol;
uint8_t* fr = new uint8_t[FRAME_SIZE];
/*-------------------------------------------------------------------------------*/

void conv_buffer_to_pixels()
{
	for (uint32_t i = 0; i < BUFFER_SIZE; i = i + 4)
	{
		uint32_t loc;
		uint16_t xaddr;
		uint16_t yaddr;
		uint8_t  dpol;
		yaddr = ((uint16_t(0x03 & acReadBuf[i + 1])) << 8) + uint16_t(acReadBuf[i]);
		xaddr = ((uint16_t(0x07 & acReadBuf[i + 2])) << 6) + uint16_t((0xFC & acReadBuf[i + 1]) >> 2);
		if ((yaddr > NCOL - 1) || (xaddr > NROW - 1))
		{
			yaddr = 0;
			xaddr = 0;
		}
		dpol = ((0x08 & acReadBuf[i + 2]) >> 3);
		loc = NCOL * uint32_t(xaddr) + uint32_t(yaddr);
		fr[loc] = (dpol != 0) ? ONPIXEL : OFFPIXEL;
	}
}

void clear_frame(void) {
  for (uint32_t i = 0; i < FRAME_SIZE; i++)
	*(fr + i) = 0;
}

Mat setup_pixels(void) {
  Mat img = Mat::zeros(NROW, NCOL, CV_8UC3);
  for (uint16_t j = 0; j < NCOL; j++)
	{
	 for (uint16_t i = 0; i < NROW; i++)
	   {
		if (fr[NCOL * i + j] == ONPIXEL)
		   img.at<Vec3b>(i, j) = ONCOLOR; 
		else if (fr[NCOL * i + j] == OFFPIXEL)
		   img.at<Vec3b>(i, j) = OFFCOLOR;
		else 
			img.at<Vec3b>(i, j) = DARK; 
		}
	}
  return img;
}
//key actions

bool escape_keypressed(void)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		return true;
	else
		return false;
}

bool space_keypressed(void)
{
	if (GetAsyncKeyState(VK_SPACE))
		return true;
	else
		return false;
}

void USB_buff_read_thread(void)
{
	while (true)
	{
		USB_sync_read(0x82);
		if (escape_keypressed())
		{
			std::cout << " Exit on ESCAPE key pressed" << std::endl;
			exit(3);
		}
	}
}

void make_frame_thread(void)
{
	while (true) 
	{
	  conv_buffer_to_pixels();
	  if (escape_keypressed())
	    {
		std::cout << " Exit on ESCAPE key pressed" << std::endl;
		exit(3);
	    }
	}
}

void show_img_thread(void)
{
	
	while (true)
	{
		imshow("Display Window", setup_pixels());
		clear_frame();
		waitKey(1);
		if (escape_keypressed())
		{
			std::cout << " Exit on ESCAPE key pressed" << std::endl;
			exit(3);
		}
	}
}


/*-------------------------------------------------------------------------------*/

int main()
{
	FT_STATUS rd_status;
	bool escape = false;

	if (!USB_device_open())  exit(3);

	getDeviceDetails();

	thread usb_read(USB_buff_read_thread);
	thread frame(make_frame_thread);
	thread show(show_img_thread);
	usb_read.join();
	frame.join();
	show.join();

	USB_device_close();
	cout << " done!" << endl;
	delete[] fr;
	return 0;
}
