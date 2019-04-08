#include <msp430.h> 
#include<stdio.h>
#include "msp430_clock.h"
#include "F5xx_F6xx_Core_Lib/HAL_UCS.h"
#include "F5xx_F6xx_Core_Lib/HAL_PMM.h"
#include "msp430_clock.h"
#include "msp430_i2c.h"
#include "mpu9150.h"
#include "HAL_FLASH.h"
#include <msp430f5529.h>
#include "mpu_uart.h"

unsigned char i;
int m;
int n;
unsigned int count = 8;
unsigned char a[6];
unsigned char g[6];
unsigned char flg = 0;
char content = 0;
#define LOOPCOUNT 10

void write2Flash() {
	char *total_buffer = malloc(sizeof(char) * 128);
	char *flash_ptr;
	    int i =0;
	    flash_ptr = (char *) 0x1880;
	    Flash_SegmentErase(flash_ptr);
			   for(i= 0;i<LOOPCOUNT;i++) {
				   msp430_i2c_read(I2C_ADDR, MPU9150_INT_STATUS, 1, &flg);
				   while(flg == 1){
			       		   MPU9150_Read_Accel_Values(&a);
			       		   MPU9150_Read_Gyro_Values(&g);
			       		   for(n=0;n<6;n++){
			       			   printf("%x\n",a[n]);
			       			   total_buffer[n] = a[n];
			       		   }
			       		   for(m=0;m<6;m++){
			       			   printf("%x\n",g[m]);
			       			   total_buffer[6 + m] = g[m];
			       		   }
			       		   FlashWrite_8(total_buffer, (flash_ptr + 12 * i), 12);
			       		   flg = 0;
			       		   msp430_delay_ms(100);

				   	}
			   }
}
int main(void) {

    WDTCTL = WDTPW | WDTHOLD;
    char *read_buffer = malloc(sizeof(char) * 12);
    char *flash_ptr;
    int i =0;
    flash_ptr = (char *) 0x1880;
    msp430_clock_init(20000000L,2);
    msp430_clock_enable();
    msp430_i2c_enable();
    msp430_delay_ms(100);
    msp430_i2c_read(I2C_ADDR, MPU9150_WHO_AM_I, 1, &i);
       	if(i == 0x68){
       		printf("%x\n",i);
       		printf("mpu9150 detection\n");
       		MPU9150_REST();
       		MPU9150_INIT();
       		}
      	else{
      		printf("error\n");
      	}
       	flash_ptr = (char *) 0x1880;
       	write2Flash();
		   for (i =  0;i<10;i++){
			   FlashRead_8(read_buffer,flash_ptr + 12 * i,12);
			   msp430_delay_ms(100);
		   }
		   for (i = 0;i<LOOPCOUNT;i++) {
			   FlashRead_8(read_buffer + 12 * i,flash_ptr + 12 * i,12);
			   msp430_delay_ms(100);
		   }
		   setup_uart();
		   for (i = 0;i<LOOPCOUNT;i++){
			   int j = 0;
			   char_send(read_buffer + 12 * i);
			   msp430_delay_ms(5000);
			   for (j = 0;j<12;j++) {
				   printf("%x\n",read_buffer[j + i * 12]);
			   }
		   }
       msp430_i2c_disable();
       msp430_clock_disable();
	return 0;
}
