#ifndef __BMI088_H__
#define __BMI088_H__

#include "main.h"
#include "spi.h"
#include "MahonyAHRS.h"

#define BMI088_ACCEL_3G_SEN 0.0008974358974f    //这个数字我终于知道哪来的了
#define BMI088_GYRO_2000_SEN 0.00106526443603169529841533860381f
/*通过 拉低or拉高 对应引脚电平 片选or取消片选 加速度计or陀螺仪*/
#define BMI088_ACC_Enable { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); }
#define BMI088_ACC_Disable { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); }
#define BMI088_GYRO_Enable { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); }
#define BMI088_GYRO_Disable { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); }



void BMI_Init();
void BMI_Read();
#endif 