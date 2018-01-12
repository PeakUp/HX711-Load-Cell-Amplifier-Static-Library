# HX711-Load-Cell-Amplifier-Static-Library

Static library written in C to get measurements from HX711 (24 bit ADC Load Cell Amplifier) via serial communication [Protocol -> UART]

//HX711 yük hücresi(ağırlık sensörü) kuvvetlendiricisi için statik C kütüphanesi 

Application Source File -> Loadcell.c (Driver)
UART configurations & Tx_Rx Function Definitions -> uartconfiguration.c
Function Definitions & Macros -> uartconfiguration.h

Gain parameter could be set one of the following: 128 - 64 - 32

Note: For now it only gets raw data from RX, when interfacing with HX711, this value corresponds to two's complement, so you need to convert the received value after the receiver block, but this block will be added within 1-2 days !  
