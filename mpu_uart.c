
#ifndef __MPU_UART_H_
#define __MPU_UART_H_

#include <msp430.h>
#include <stdio.h>
#include "mpu_uart.h"

void setup_uart(void) {
	P3SEL = BIT3 + BIT4;
	UCA0CTL1 |= UCSWRST;
	UCA0CTL1 |= UCSSEL_1;
	UCA0BR0 = 0x03;
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRS_3 + UCBRF_0;
	UCA0CTL1 &= ~UCSWRST;
}

void uart_tx(char *tx_data)
{
	PTxUart = tx_data;
	UCA0IE |= UCTXIE;
}
void char_send(char *data){

	uart_tx(data);
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {
	switch (__even_in_range(UCA0IV, 4)) {
	case 0:
		break;
	case 2:
		break;
	case 4:
	{
		int i = 0;
		for (i = 0;i<12;i++) {
			while ((UCA0STAT & UCBUSY));
			UCA0TXBUF = *PTxUart++;
		}
		UCA0IFG &= ~UCTXIFG;
		UCA0IE &= ~UCTXIE;
		}
		break;
	default:
		break;
	}
}

#endif /* MPU_UART_H_ */
