#pragma once
#include <stdint.h>
// #include "stm32l4xx_hal_i2c.h"
#include "stm32f1xx_hal.h"


extern float    capacity;
extern float    capacity_last;
extern float    capacity_process[ 100 ];
extern uint16_t IC_version;

#define I2C_DEFAULT_ADDRESS 0x36  // 从机地址
#define REGISTER_VCELL      0x02  // 电压VCELL 寄存器保存测量到的电池电压。通过读取该寄存器的值，可获得电池的当前电压。
#define REGISTER_SOC        0x04  // 电池状态 (SOC)：SOC 寄存器提供芯片算法计算出的电池充电状态。该值表示电池电量与其容量的相对值。
#define REGISTER_MODE       0x06  // 休眠状态：MODE 寄存器可以指示芯片是否处于休眠模式。此寄存器中有一个特定位用于指示此状态。
#define REGISTER_VERSION    0x08
#define VALRT_MIN           0x14
#define VRESET              0x18


#define REG_V_CELL  0x02
#define REG_SOC     0x04
#define REG_MODE    0x06
#define REG_VERSION 0x08
#define REG_HIBRT   0x0A
#define REG_CONFIG  0x0C
#define REG_VALRT   0x14
#define REG_CRATE   0x16
#define REG_V_RESET 0x18
#define REG_STATUS  0x1A

/**< REG RANGE:0x40 to 0x70*/
#define REG_TABLE     0x40
#define REG_TABLE_END 0x70

#define REG_CMD 0xFE

float read_voltage( I2C_HandleTypeDef* hi2c );

float read_current( I2C_HandleTypeDef* hi2c, float resistance );

float read_soc( I2C_HandleTypeDef* hi2c );

uint16_t read_ic_version( I2C_HandleTypeDef* hi2c );

void write_valrt_min( I2C_HandleTypeDef* hi2c, uint8_t valrt_min_value );

uint8_t read_valrt_min( I2C_HandleTypeDef* hi2c );

HAL_StatusTypeDef read_reset( I2C_HandleTypeDef* hi2c, uint8_t* reset_value );
