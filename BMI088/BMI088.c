#include "BMI088.h"

uint8_t pTxData;
uint8_t pRxData;
float INS_angle[3] = { 0.0f, 0.0f, 0.0f };

void BMI_Init()
{
	//���ַ0x7E��д��0xB6ֵ�����ٶȼ������λ��ʹ���ٶȼƸ����Ĵ����ָ�ΪĬ��ֵ
	BMI088_ACC_Enable;
	pTxData = (0x7E & 0x7F);    //Bit #0��Bit #1-7��Bit #0��0����ʾд
    HAL_SPI_Transmit(&hspi1, &pTxData, 1, 1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);    //�ȴ�SPI�������
    pTxData = 0xB6;    //Bit #8-15
    HAL_SPI_Transmit(&hspi1, &pTxData, 1, 1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);    //�ȴ�SPI�������
    HAL_Delay(1);    //��ʱ1ms
	BMI088_ACC_Disable;
	
	//���ٶȼƸ�λ��Ĭ������ͣģʽ�����ַ0x7D��д��0x04ֵ��ʹ���ٶȼƽ�������ģʽ
	BMI088_ACC_Enable;
	pTxData = (0x7D & 0x7F);    //Bit #0��Bit #1-7��Bit #0��0����ʾд
    HAL_SPI_Transmit(&hspi1, &pTxData, 1, 1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);    //�ȴ�SPI�������
    pTxData = 0x04;    //Bit #8-15
    HAL_SPI_Transmit(&hspi1, &pTxData, 1, 1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);    //�ȴ�SPI�������
    HAL_Delay(1);    //��ʱ1ms
	BMI088_ACC_Disable;
	
	//���ַ0x14��д��0xB6ֵ�������������λ��ʹ�����Ǹ����Ĵ����ָ�ΪĬ��ֵ
    BMI088_GYRO_Enable;    //PB0��0��Ƭѡ������
    pTxData = (0x14 & 0x7F);    //Bit #0��Bit #1-7��Bit #0��0����ʾд
    HAL_SPI_Transmit(&hspi1, &pTxData, 1, 1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);    //�ȴ�SPI�������
    pTxData = 0xB6;    //Bit #8-15
    HAL_SPI_Transmit(&hspi1, &pTxData, 1, 1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);    //�ȴ�SPI�������
    HAL_Delay(30);    //��ʱ30ms
    BMI088_GYRO_Disable;    //PB0��1��ȡ��Ƭѡ������
}

void BMI_Read(float *acc,float *gyro)
{
	uint8_t i=0;
	uint8_t buf[8];
	BMI088_ACC_Enable;    //PA4��0��Ƭѡ���ٶȼ�
    pTxData = (0x12 | 0x80);    //Bit #0��Bit #1-7��Bit #0��1����ʾ��
    HAL_SPI_Transmit(&hspi1, &pTxData, 1, 1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);    //�ȴ�SPI�������
    HAL_SPI_Receive(&hspi1, &pRxData, 1, 1000);    //Bit #8-15����Чֵ
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_RX);    //�ȴ�SPI�������
    i = 0;
    while (i < 6)
    {
    	HAL_SPI_Receive(&hspi1, &pRxData, 1, 1000);    //Bit #16-23���Ĵ���0x12��ֵ��Ȼ���ǼĴ���0x13��0x14��0x15��0x16��0x17��ֵ
    	while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_RX);    //�ȴ�SPI�������
    	buf[i] = pRxData;
        i++;
    }
    BMI088_ACC_Disable;    //PA4��1��ȡ��Ƭѡ���ٶȼ�
    acc[0] = ((int16_t)((buf[1]) << 8) | buf[0]) * BMI088_ACCEL_3G_SEN;
    acc[1] = ((int16_t)((buf[3]) << 8) | buf[2]) * BMI088_ACCEL_3G_SEN;
    acc[2] = ((int16_t)((buf[5]) << 8) | buf[4]) * BMI088_ACCEL_3G_SEN;
    	
	BMI088_GYRO_Enable;    //PB0��0��Ƭѡ������
    pTxData = (0x00 | 0x80);    //Bit #0��Bit #1-7��Bit #0��1����ʾ��
    HAL_SPI_Transmit(&hspi1, &pTxData, 1, 1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX);    //�ȴ�SPI�������
	i = 0;
    while (i < 8)
    {
    	HAL_SPI_Receive(&hspi1, &pRxData, 1, 1000);    //Bit #8-15���Ĵ���0x00��ֵ��Ȼ���ǼĴ���0x01��0x02��0x03��0x04��0x05��0x06��0x07��ֵ
    	while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_RX);    //�ȴ�SPI�������
    	buf[i] = pRxData;
    	i++;
    }
    BMI088_GYRO_Disable;    //PB0��1��ȡ��Ƭѡ������
    if(buf[0] == 0x0F)	//buf[0]����GYRO_CHIP_ID��Ӧ��Ϊ0x0F���ж����Ƕ�ȡ�����ǲ��������ǵ�ֵ��
    {
    	gyro[0] = ((int16_t)((buf[3]) << 8) | buf[2]) * BMI088_GYRO_2000_SEN;
    	gyro[1] = ((int16_t)((buf[5]) << 8) | buf[4]) * BMI088_GYRO_2000_SEN;
    	gyro[2] = ((int16_t)((buf[7]) << 8) | buf[6]) * BMI088_GYRO_2000_SEN;
    }
}
