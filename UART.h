#ifndef __UART_H__
#define __UART_H__

extern void calc_UART_baud_rate(void);
extern void SerialPort0_ISR(void);
extern void Send_UINT8_To_UART0(UINT8 buf);
extern void UART_Hal_Init76E003(uint16_t    u16Baudrate);

#endif