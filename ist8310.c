#include "ist8310.h"

//the first column:the registers of IST8310. ��һ��:IST8310�ļĴ���
//the second column: the value to be writed to the registers.�ڶ���:��Ҫд��ļĴ���ֵ
//the third column: return error value.������:���صĴ�����
static const uint8_t ist8310_write_reg_data_error[4][3] ={
        {0x0B, 0x08, 0x01},     //enalbe interrupt  and low pin polarity.�����жϣ��������õ͵�ƽ
        {0x41, 0x09, 0x02},     //average 2 times.ƽ����������
        {0x42, 0xC0, 0x03},     //must be 0xC0. ������0xC0
        {0x0A, 0x0B, 0x04}};    //200Hz output rate.200Hz���Ƶ��
uint8_t ist8310_Init()
{
	uint8_t res = 0;
	uint8_t writeNum = 0;
	//IST8310 �� RESET���͵�ƽ���� IST8310
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_Delay(150);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); 
	HAL_Delay(50);
	//��IST8310�ļĴ���д���������ݲ�������֤
	HAL_I2C_Mem_Read(&hi2c3, IST8310_IIC_ADDRESS <<1, 0x00,I2C_MEMADD_SIZE_8BIT,&res,1,10);
	if (res != 0x10)
    {
        return 0x40;
    }
	for (writeNum = 0; writeNum < 4; writeNum++)
    {   
		uint8_t data = ist8310_write_reg_data_error[writeNum][1];
        HAL_I2C_Mem_Write(&hi2c3, IST8310_IIC_ADDRESS <<1, ist8310_write_reg_data_error[writeNum][0],I2C_MEMADD_SIZE_8BIT,&data,1,10);
        HAL_Delay(150);
        HAL_I2C_Mem_Read(&hi2c3, IST8310_IIC_ADDRESS <<1, (ist8310_write_reg_data_error[writeNum][0]),I2C_MEMADD_SIZE_8BIT,&res,1,10);;
        HAL_Delay(150);
        if (res != ist8310_write_reg_data_error[writeNum][1])
        {
            return ist8310_write_reg_data_error[writeNum][2];
        }
    }
    return 0x00;
}
void ist8310_Read(float mag[3])
{
	uint8_t buf[6];
	
	//read the "DATAXL" register (0x03)
	HAL_I2C_Mem_Read(&hi2c3, IST8310_IIC_ADDRESS <<1, 0x03,I2C_MEMADD_SIZE_8BIT,buf,6,10);
	
    mag[0] = MAG_SEN * (int16_t)((buf[1] << 8) | buf[0]);
    mag[1] = MAG_SEN * (int16_t)((buf[3] << 8) | buf[2]);
    mag[2] = MAG_SEN * (int16_t)((buf[5] << 8) | buf[4]);
	
}