/*
 * mpu9150.c
 *
 *  Created on: 2019Äê1ÔÂ9ÈÕ
 *      Author: dell
 */
#include <stdio.h>
#include<string.h>
#include "mpu9150.h"
#include "msp430_i2c.h"
#include "msp430_clock.h"

int MPU9150_REST(void){
	unsigned char rest = 0x80;
	msp430_i2c_write(I2C_ADDR, MPU9150_PWR_MGMT_1, 1,  &rest);
	return 0;
}
int MPU9150_INIT(void)
{
	unsigned char pwr1= 0x00;
	unsigned char pwr2= 0x00;
	unsigned char INT_PIN= 0x22;
	unsigned char config= 0x03;
	unsigned char smp_div= 0x04;
	unsigned char INT_EN= 0x01;

	msp430_i2c_write(I2C_ADDR, MPU9150_PWR_MGMT_1, 1,  &pwr1);
	msp430_delay_ms(100);

	msp430_i2c_write(I2C_ADDR, MPU9150_PWR_MGMT_1, 1, &pwr2);
	msp430_delay_ms(200);

	printf("mpu9150_ok\n");
	msp430_i2c_write(I2C_ADDR, MPU9150_CONFIG, 1, &config);
	msp430_i2c_write(I2C_ADDR, MPU9150_SMPLRT_DIV, 1, &smp_div);

	unsigned char g_temp;
	msp430_i2c_read(I2C_ADDR, MPU9150_GYRO_CONFIG, 1, &g_temp);
	unsigned char g_config1 = g_temp & ~0xE0;
	unsigned char g_config2 = g_temp & ~0x18;
	unsigned char g_config3 = g_temp | 3 <<3;
	msp430_i2c_write(I2C_ADDR, MPU9150_GYRO_CONFIG, 1, &g_config1);
	msp430_i2c_write(I2C_ADDR, MPU9150_GYRO_CONFIG, 1, &g_config2);
	msp430_i2c_write(I2C_ADDR, MPU9150_GYRO_CONFIG, 1, &g_config3);

	unsigned char a_temp;
	msp430_i2c_read(I2C_ADDR, MPU9150_ACCEL_CONFIG, 1, &a_temp);
	unsigned char a_config1 = a_temp & ~ 0xE0;
	unsigned char a_config2 = a_temp & ~ 0x18;
	unsigned char a_config3 = a_temp | 3 <<3;
	msp430_i2c_write(I2C_ADDR, MPU9150_ACCEL_CONFIG, 1, &a_config1);
	msp430_i2c_write(I2C_ADDR, MPU9150_ACCEL_CONFIG, 1, &a_config2);
	msp430_i2c_write(I2C_ADDR, MPU9150_ACCEL_CONFIG, 1, &a_config3);


	msp430_i2c_write(I2C_ADDR, MPU9150_INT_PIN_CFG, 1, &INT_PIN);

	msp430_i2c_write(I2C_ADDR, MPU9150_INT_ENABLE, 1, &INT_EN);

	return 0;
}

int MPU9150_Read_Accel_Values(unsigned char *data)
{
	msp430_i2c_read(I2C_ADDR, MPU9150_ACCEL_XOUT_H, 6,  data);
    return 0;
}

int MPU9150_Read_Gyro_Values(unsigned char *temp)
{
	msp430_i2c_read(I2C_ADDR, MPU9150_GYRO_XOUT_H, 6, temp);
	return 0;
}
