#pragma once
#define _AFXDLL
#include "stdafx.h"
#define FTD3XX_STATIC

#include<iostream>
#include "FTD3XX.h"
#include <initguid.h> 
#include <string>
#include <cassert>

#define  BUFFER_SIZE   4096
#define  NCOL          640
#define  NROW          480
#define  FRAME_SIZE    NCOL*NROW 

using namespace std;

FT_HANDLE ftHandle = NULL;
DWORD Flags = 0;
DWORD Type = 0;
DWORD ID = 0;
DWORD numDevs = 0;
UCHAR acReadBuf[BUFFER_SIZE] = {};
UCHAR acWriteBuf[BUFFER_SIZE] = {};
char  SerialNumber[16] = { 0 };
char  Description[32] = { 0 };
DEFINE_GUID(GUID_DEVINTERFACE_FOR_D3XX, 0xd1e8fe6a, 0xab75, 0x4d9e, 0x97, 0xd2, 0x6, 0xfa, 0x22, 0xc7, 0x73, 0x6c);
bool ChipConfiguration()
{
	FT_STATUS status = FT_OK;
	//FT_HANDLE ftHandle;
	GUID DeviceGUID[2] = { 0 };
	// Open a device handle by GUID
	memcpy(&DeviceGUID[0], &GUID_DEVINTERFACE_FOR_D3XX, sizeof(GUID));
	status = FT_Create(&DeviceGUID[0], FT_OPEN_BY_GUID, &ftHandle);
	if (FT_FAILED(status))
	{
		FT_Close(ftHandle);
		cout << "configuration faild" << endl;
		return false;
	}
	// Get chip configuration
	FT_60XCONFIGURATION oConfigurationData = { 0 };
	status = FT_GetChipConfiguration(ftHandle, &oConfigurationData);
	if (FT_FAILED(status))
	{
		FT_Close(ftHandle);
		return false;
	}
	// Set chip configuration
	oConfigurationData.FIFOMode = CONFIGURATION_FIFO_MODE_600;
	oConfigurationData.ChannelConfig = CONFIGURATION_CHANNEL_CONFIG_4;
	oConfigurationData.OptionalFeatureSupport = CONFIGURATION_OPTIONAL_FEATURE_DISABLEALL;
	status = FT_SetChipConfiguration(ftHandle, &oConfigurationData);
	if (FT_FAILED(status))
	{
		FT_Close(ftHandle);
		return false;
	}
	// Close device handle
	FT_Close(ftHandle);
	return true;
}
/*-------------------------------------------------------------------------------*/

void getDeviceDetails()
{
	FT_STATUS status;
	status = FT_CreateDeviceInfoList(&numDevs);
	if (!FT_FAILED(status) && numDevs > 0)
	{
		printf("List of Connected Devices!\n\n");
		for (DWORD i = 0; i < numDevs; i++)
		{
			status = FT_GetDeviceInfoDetail(i, &Flags, &Type, &ID, NULL,
				SerialNumber, Description, &ftHandle);
			if (!FT_FAILED(status))
			{
				printf("Device[%d]\n", i);
				printf("\tFlags: 0x%x %s | Type: %d | ID: 0x%08X | ftHandle=0x%x \n", Flags,
					Flags & FT_FLAGS_SUPERSPEED ? "[USB 3]" :
					Flags & FT_FLAGS_HISPEED ? "[USB 2]" :
					Flags & FT_FLAGS_OPENED ? "[OPENED]" : "",
					Type,
					ID,
					ftHandle);
				printf("\tSerialNumber=%s\n", SerialNumber);
				printf("\tDescription=%s\n", Description);
			}
		}
	}
}
/*-------------------------------------------------------------------------------*/
bool USB_device_open()
{
	FT_STATUS status = FT_OK;
	//FT_HANDLE ftHandle;
	GUID DeviceGUID[2] = { 0 };
	memcpy(&DeviceGUID[0], &GUID_DEVINTERFACE_FOR_D3XX, sizeof(GUID));
	status = FT_Create(&DeviceGUID[0], FT_OPEN_BY_GUID, &ftHandle);
	if (FT_FAILED(status))
	{
		cout << "device open failed" << endl;
		return false;
	}
	else
	{
		cout << "Device " << ftHandle << " successfully opend." << endl;
		return true;
	}
}

/*-------------------------------------------------------------------------------*/
bool USB_sync_write(int pipe_no)
{
	FT_STATUS status = FT_OK;
	//UCHAR acWriteBuf[BUFFER_SIZE] = { 0xAB };
	ULONG ulBytesWritten = 0;
	status = FT_WritePipe(ftHandle, pipe_no, acWriteBuf, sizeof(acWriteBuf), &ulBytesWritten, NULL);
	if FT_FAILED(status)
	{
		FT_Close(ftHandle);
		return false;
	}
	else
		return true;
}
/*-------------------------------------------------------------------------------*/

bool USB_sync_read(int pipe_no)
{
	//fstream my_file;
	//my_file.open("my_file.txt", ios::out);
	FT_STATUS status = FT_OK;
	//UCHAR acReadBuf[BUFFER_SIZE] = { 0xFF };
	ULONG ulBytesRead = 0;
	status = FT_ReadPipe(ftHandle, pipe_no, acReadBuf, sizeof(acReadBuf), &ulBytesRead, NULL);
	if (FT_FAILED(status))
	{
		FT_Close(ftHandle);
		printf("USB read failed\n");
		return false;
	}
	else
	{
		//for (int i = 0; i < BUFFER_SIZE; i + 4) {
		//	my_file <<  i << "  " << to_string(acReadBuf[i]) << "  " << to_string(acReadBuf[i + 1]) << "  " << to_string(acReadBuf[i + 2]) << endl;
		//}
		//my_file.close();
		return true;
	}
}
/*-------------------------------------------------------------------------------*/
void USB_device_close()
{
	FT_Close(ftHandle);
	cout << "USB3 closed" << endl;
}
