/*
 * Uart Configuration Header File (POSIX Compliant)
 *
 */

#ifndef UARTCONFIGURATION
#define UARTCONFIGURATION

//Standard Header Files
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <termios.h>
#include <inttypes.h>
#include <sys/stat.h>

/*
 * Macro & Constant Definitions
 */
	//Status Flags
#define Status_Error -1
#define Status_Ok     0

/*
 * Function Declarations
 */
extern int Initialize_Uart(const char* Directory_Uart0);

extern uint8_t Request_WeightMeasurementData(int FileDescriptor, uint8_t Gain);
extern uint8_t Receive_Uart(int FileDescriptor, unsigned char* ReadBuffer, size_t Size);
extern uint8_t Transmit_Uart(int FileDescriptor, const unsigned char* WriteBuffer, size_t Size);

#endif /*End of uartconfiguration_H*/
