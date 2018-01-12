/*
 * Uart Configuration Source File (POSIX Compliant)
 *
 * Brief:
 *		Configure the UART0 on Rpi 3 to communicate with HX711
 *
 * Interface:
 *		Serial:
 *			HX711 -> RPi 3 (UART 0)
 *				Tx: Pin 8
 *				Rx: Pin 10
 *
 * Build:
 *		gcc
 */

//Custom Header Files
#include "uartconfiguration.h"


/*
 * Function Definitions
 */

/*
 * Brief: Configures & initializes the UART to communicate with HX711
 *
 *
 * Parameters:
 *		Directory_Uart0: Directory to UART0 file
 *
 * Return Value:
 *		UART0 File Descriptor: (int)
 *				>0  -> No error
 *				<0 -> Error
 *
 * Algorithm:
 *		-Open UART0 file for configuration
 *		-Write parameters to communicate with HX711
 *
 * Notes:
 *		Do not forget to close UART file descriptor after UART is no longer in need
 */
extern int Initialize_Uart(const char* Directory_Uart0)
{
	//Local variables
	int FileDescriptor_Uart0 = 0;
	int StatusReturn_AttributeSet = 0;
	struct termios Structure_UartMode;

	//Variable initialization
	FileDescriptor_Uart0 = 0;

	//Write to UART
	FileDescriptor_Uart0 = open(Directory_Uart0, O_RDWR | O_NOCTTY | O_NONBLOCK);
		//Function Error
	if (0 > FileDescriptor_Uart0)
	{

#if DEBUG
printf("\nCan not open file...\nError:\t%d\nErrno:\t%d", FileDescriptor_Uart0, errno);
#endif

		return Status_Error;
	}

	//Configure Input/Output Modes
	Structure_UartMode.c_oflag = 0x00;
        Structure_UartMode.c_lflag = 0x00;
		//Ignore Parity Bits
	Structure_UartMode.c_iflag = IGNPAR;
		//Enable receiver, Bit logic 1 width = 26.04 us (38400 Baud)
	Structure_UartMode.c_cflag = CREAD | CLOCAL | CS8 | B38400;

	//Flush all input data before any configuration change
	StatusReturn_AttributeSet = tcflush(FileDescriptor_Uart0, TCIFLUSH);
		//Function Error
	if (0 != StatusReturn_AttributeSet)
	{

#if DEBUG
printf("Can not flush to file...\n");
#endif

                return Status_Error;
	}

	//Apply changes for UART immediately
	StatusReturn_AttributeSet = tcsetattr(FileDescriptor_Uart0, TCSANOW, &Structure_UartMode);
		//Function Error
	if (0 != StatusReturn_AttributeSet)
	{

#if DEBUG
printf("Can not apply changes to file...\n");
#endif

		return Status_Error;
	}

	//Setup Successfully Completed
	return FileDescriptor_Uart0;

}//End of Initialize_Uart()


/*
 * Brief: Transmits given buffer with given size to serial port via UART file descriptor
 *
 *
 * Parameters:
 *		FileDescriptor: (int) Valid file descriptor
 *		WriteBuffer:    (const unsigned char*) Pointer to unsigned char buffer
 *		Size:  		(size_t) Transmit block size
 *
 *
 * Return Value:
 *		Status Return: (uint8_t)
 *				Status_Ok    -> Normal Operation
 *				Status_Error -> Error Detected
 *
 * Algorithm:
 *
 *
 * Notes:
 *
 */
extern uint8_t Transmit_Uart(int FileDescriptor, const unsigned char* WriteBuffer, size_t Size)
{
	//Unspecified behaviour
	if ((0 >= Size) || (SSIZE_MAX < Size))
	{

#if DEBUG
printf("\nUart transmit failed...\nSize exceeds limits {0 >= Size > SSIZE_MAX}");
#endif

		return Status_Error;
	}

	//No valid file descriptor
	if (0 > FileDescriptor)
	{

#if DEBUG
printf("\nFile descriptor is not valid...\n");
#endif

		return Status_Error;
	}

	//Local variables
	ssize_t StatusReturn_Size;

	//Transmit given characters
	StatusReturn_Size = write(FileDescriptor, WriteBuffer, Size);
		//Transmit error
	if ((0 >= StatusReturn_Size) || (Size < StatusReturn_Size))
	{

#if DEBUG
printf("\nUart transmit failed...\nTransmit size exceeds limits {0 >= StatusReturn_Size > Size}");
#endif

		return Status_Error;
	}

//Print the sent character count
#if DEBUG
printf("\nSent character byte count:\t%d\n", StatusReturn_Size);
for(uint8_t index = 0; index < Size; index++)
printf("\nSent bytes:\t%u\n", WriteBuffer[index]);
#endif

	//Successful return
	return Status_Ok;

}//End of Transmit_Uart()


/*
 * Brief: Receive the data from UART file buffer into the given buffer with given size
 *
 *
 * Parameters:
 *              FileDescriptor: (int) Valid file descriptor
 *              ReadBuffer:     (unsigned char*) Pointer to unsigned char buffer
 *              Size:           (size_t) Receive block size
 *
 *
 * Return Value:
 *              Status Return: (uint8_t)
 *                              Status_Ok    -> Normal Operation
 *                              Status_Error -> Error Detected
 *
 * Algorithm:
 *
 *
 * Notes:
 *		Size must be lower than the sizeof(ReadBuffer), else this function will overwrite to some undefined memory address
 *		Do not forget to add NULL to the last element of the given buffer array if it is a character array, when using it as a string
 */
extern uint8_t Receive_Uart(int FileDescriptor, unsigned char* ReadBuffer, size_t Size)
{
        //Unspecified behaviour
        if ((0 >= Size) || (SSIZE_MAX < Size))
        {

#if DEBUG
printf("\nUart transmit failed...\nSize exceeds limits {0 >= Size > SSIZE_MAX}");
#endif

                return Status_Error;
        }

        //No valid file descriptor
        if (0 > FileDescriptor)
        {

#if DEBUG
printf("\nFile descriptor is not valid...\n");
#endif

                return Status_Error;
        }

	//Local Variable
	ssize_t StatusReturn_ReadCount = 0;

	//Read into the given buffer
	StatusReturn_ReadCount = read(FileDescriptor, ReadBuffer, Size);
		//Read error
	if (0 > StatusReturn_ReadCount)
	{

#if DEBUG
printf("\nUART receive error...\nErrno:\t%d\n", errno);
#endif

		return Status_Error;
	}
		//End of File indicator is set, attemting to read from empty file
	else if (0 == StatusReturn_ReadCount)
	{

#if DEBUG
printf("\nUART receive error, there is no data to read...\n");
#endif

		return Status_Error;
	}
		//Data successfully read into the given buffer
	else
	{

#if DEBUG
printf("\nUART receive success...\n");
#endif

	}

	int StatusReturn_FlushError = 0;

	//Flush all pending data (Input & output)
	StatusReturn_FlushError = tcflush(FileDescriptor, TCIOFLUSH);
		//Data not flushed
	if (StatusReturn_FlushError)
	{

#if DEBUG
printf("\nFlush on UART failed...\n");
#endif

		return Status_Error;
	}

	//Successful return
	return Status_Ok;

}//End of Receive_Uart()


/*
 * Brief: Requests weight measurement data from HX711
 *
 *
 * Parameters:
 *              FileDescriptor: (int) Valid file descriptor
 *		Gain: (uint8_t) Gain parameter, 128, 64 or 32
 *
 * Return Value:
 *              Status Return:  (uint8_t)
 *                              Status_Ok    -> Normal Operation
 *                              Status_Error -> Error Detected
 *
 * Algorithm:
 *		If DOUT pin is high, return Status_Error
 *
 * Notes:
 *
 */
extern uint8_t Request_WeightMeasurementData(int FileDescriptor, uint8_t Gain)
{
	//Only if DOUT is low, request could be done


        //Transmit some sample values
        uint8_t StatusReturn_UartTransmit  = 0;
        const uint8_t BufferSize_UartWrite = 4;
        unsigned char Buffer_UartWrite[BufferSize_UartWrite];

	//First 3 bytes to send, all high
        Buffer_UartWrite[0] = 0xFF;
        Buffer_UartWrite[1] = 0xFF;
        Buffer_UartWrite[2] = 0xFF;

	//Gain: 128
	if (128 == Gain)
	{
	        Buffer_UartWrite[3] = 0x80;
	}
        //Gain: 64
	else if (64 == Gain)
	{
	        Buffer_UartWrite[3] = 0xE0;
	}
        //Gain: 32
	else if (32 == Gain)
	{
	        Buffer_UartWrite[3] = 0xC0;
	}
	//Gain parameter is not valid
	else
	{

#if DEBUG
printf("\nGain parameter is not valid...\n");
#endif

		return Status_Error;
	}

        StatusReturn_UartTransmit = Transmit_Uart(FileDescriptor, &Buffer_UartWrite[0], BufferSize_UartWrite);
                //Transmit Error
        if (Status_Ok != StatusReturn_UartTransmit)
        {

#if DEBUG
printf("UART transmit error...\n");
#endif

        }

	//Successful return
	return Status_Ok;

}//End of Request_WeightMeasurementData()
