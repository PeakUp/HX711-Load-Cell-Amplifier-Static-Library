/*
 * Loadcell Main Application Source File
 *
 * Brief:
 *		Application Code
 *
 * Interface:
 *              Serial:
 *                      HX711 -> RPi 3 (UART 0)
 *                              Tx: Pin 8
 *                              Rx: Pin 10
 *
 * Build:
 *              gcc -Wall -O3 uartconfiguration.c Loadcell.c -o Loadcell
 * Debug:
 *		gcc -Wall -O3 -DDEBUG uartconfiguration.c Loadcell.c -o Loadcell
 */

/*
 * Header Files
 */
	//Standard Header Files
#include <stdlib.h>
	//Custom Header Files
#include "uartconfiguration.h"

int main(int argc, char* argv[])
{
	//Local variables
	int FileDescriptor_Uart0 = 0;
	int StatusReturn_FileClose = 0;

	uint8_t Gain = 128;

	//UART0 File Location
	const char Directory_Uart0_Local[] = "/dev/serial0";

	//Initialize UART0 on Rpi 3
	FileDescriptor_Uart0 = Initialize_Uart(&Directory_Uart0_Local[0]);
		//Could not initialize UART0
	if (0 >= FileDescriptor_Uart0)
	{

#if DEBUG
printf("\nUART initialization error...\n");
#endif

		exit(EXIT_FAILURE);
	}

	//Request data from HX711
	uint8_t StatusReturn_DataRequest = 0;

	StatusReturn_DataRequest = Request_WeightMeasurementData(FileDescriptor_Uart0, Gain);
		//Data is not ready or function error
	if (Status_Ok != StatusReturn_DataRequest)
	{

#if DEBUG
printf("\nData is not ready, or function error...\n");
#endif

	}

	//Receive return values
        uint8_t StatusReturn_UartReceive = 0;
        const uint8_t BufferSize_UartRead = 3;
        unsigned char Buffer_UartRead[BufferSize_UartRead];

	//Read from UART file descriptor into the receive buffer
	StatusReturn_UartReceive = Receive_Uart(FileDescriptor_Uart0, Buffer_UartRead, BufferSize_UartRead);
		//Read error
	if (Status_Ok != StatusReturn_UartReceive)
	{

#if DEBUG
printf("\nUART receive error...\n");
#endif

	}

//Printf received result
#if DEBUG
for(uint8_t index = 0; index < 3; index++)
printf("\nReceived bytes:\t%u\n", Buffer_UartRead[index]);
#endif


	//Close UART file descriptor, free resources
	StatusReturn_FileClose = close(FileDescriptor_Uart0);
		//Could not close file descriptor
	if (0 != StatusReturn_FileClose)
	{

#if DEBUG
printf("\nUART file closing error...\n");
#endif

		exit(EXIT_FAILURE);
	}

	//Successfull termination
	printf("\nSuccessful Termination...\n\n");
	exit(EXIT_SUCCESS);

}//End of main()
