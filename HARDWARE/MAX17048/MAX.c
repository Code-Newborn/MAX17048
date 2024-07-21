#include "MAX.h"
#include "i2c.h"

// #include <math.h>

float    capacity      = 0;
float    capacity_last = 0;
float    capacity_process[ 100 ];
uint16_t IC_version = 0x0000;

void write_valrt_min( I2C_HandleTypeDef* hi2c, uint8_t valrt_min_value ) {
    uint8_t           data[ 1 ];
    HAL_StatusTypeDef status;

    // 设置VALRT寄存器的值。 VALRT_MIN 地址是 0x14
    data[ 0 ] = valrt_min_value;

    // 将值发送到 VALRT 寄存器
    status = HAL_I2C_Mem_Write( hi2c, I2C_DEFAULT_ADDRESS << 1, VALRT_MIN, I2C_MEMADD_SIZE_8BIT, data, sizeof( data ), HAL_MAX_DELAY );

    // 检查操作是否成功
    if ( status != HAL_OK ) {
    }
}

void write_reset( I2C_HandleTypeDef* hi2c, uint8_t reset_value ) {
    uint8_t           data[ 1 ];
    HAL_StatusTypeDef status;

    data[ 0 ] = reset_value;

    // 向 VRESET 寄存器发送数值
    status = HAL_I2C_Mem_Write( hi2c, I2C_DEFAULT_ADDRESS << 1, VRESET, I2C_MEMADD_SIZE_8BIT, data, sizeof( data ), HAL_MAX_DELAY );

    // 检查操作是否成功
    if ( status != HAL_OK ) {
        // 错误处理
    }
}

HAL_StatusTypeDef read_reset( I2C_HandleTypeDef* hi2c, uint8_t* reset_value ) {
    HAL_StatusTypeDef status;

    // 为来自 VRESET 寄存器的值准备接收缓冲区
    uint8_t data[ 1 ] = { 0 };

    // 从 VRESET 寄存器读取值
    status = HAL_I2C_Mem_Read( hi2c, I2C_DEFAULT_ADDRESS << 1, VRESET, I2C_MEMADD_SIZE_8BIT, data, sizeof( data ), HAL_MAX_DELAY );

    // 检查操作是否成功
    if ( status == HAL_OK ) {
        // 如果读取成功，则为传递的指针赋值
        *reset_value = data[ 0 ];
    }
    else {
        // 如果需要的话进行错误处理
    }

    // 返回操作的状态
    return status;
}

uint8_t read_valrt_min( I2C_HandleTypeDef* hi2c ) {
    uint8_t           valrt_min_value;
    uint8_t           data[ 1 ];
    HAL_StatusTypeDef status;

    // 从 VALRT 寄存器读取值。 VALRT_MIN地址是 0x14
    status = HAL_I2C_Mem_Read( hi2c, ( I2C_DEFAULT_ADDRESS << 1 ) | 0x01, VALRT_MIN, I2C_MEMADD_SIZE_8BIT, data, sizeof( data ), HAL_MAX_DELAY );

    if ( status != HAL_OK ) {
        // 错误处理，返回例如 0xFF，这可能表示错误
        return 0xFF;
    }

    // 将读取值赋值给变量
    valrt_min_value = data[ 0 ];

    // 返回读取的值
    return valrt_min_value;
}

float read_voltage( I2C_HandleTypeDef* hi2c ) {
    uint8_t  data[ 2 ];
    uint16_t raw_voltage;
    float    voltage;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read( hi2c, I2C_DEFAULT_ADDRESS << 1, REGISTER_VCELL, I2C_MEMADD_SIZE_8BIT, data, sizeof( data ), HAL_MAX_DELAY );

    if ( status != HAL_OK ) {
        return -1.0f;  // Indicate error
    }

    raw_voltage = ( data[ 0 ] << 8 ) | data[ 1 ];

    voltage = raw_voltage * 78.125e-6;  // 78.125 µV per LSB

    return voltage;
}

float read_current( I2C_HandleTypeDef* hi2c, float resistance ) {
    uint8_t  data[ 2 ];
    uint16_t raw_voltage;
    float    voltage, current;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read( hi2c, I2C_DEFAULT_ADDRESS << 1, REGISTER_VCELL, I2C_MEMADD_SIZE_8BIT, data, sizeof( data ), HAL_MAX_DELAY );

    if ( status != HAL_OK ) {
        return -1.0f;  // Indicate error
    }

    raw_voltage = ( data[ 0 ] << 8 ) | data[ 1 ];
    voltage     = raw_voltage * 78.125e-6;  // 78.125 µV per LSB

    if ( resistance > 0 ) {
        current = voltage / resistance;
        return current;
    }
    else {
        return -1.0f;  // 返回 -1 表示电阻值错误
    }
}

float read_soc( I2C_HandleTypeDef* hi2c ) {
    uint8_t data[ 2 ];
    float   soc;

    // 从 SOC 寄存器读取 2 个字节
    if ( HAL_I2C_Mem_Read( hi2c, I2C_DEFAULT_ADDRESS << 1, REGISTER_SOC, I2C_MEMADD_SIZE_8BIT, data, sizeof( data ), HAL_MAX_DELAY ) == HAL_OK ) {
        // SoC 值以 16 位数字形式存储，其中每一位相当于 1/256%
        soc = ( ( data[ 0 ] << 8 ) | data[ 1 ] ) / 256.0f;
    }
    else {
        // 如果出现错误，返回 -1
        soc = -1.0f;
    }
    return soc;
}

uint16_t read_ic_version( I2C_HandleTypeDef* hi2c ) {
    uint8_t version[ 2 ];
    uint8_t reg = REGISTER_VERSION;  // VERSION 寄存器的地址是 0x08

    uint16_t _version;

    // 从 VERSION 寄存器读取 1 个字节
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read( hi2c, I2C_DEFAULT_ADDRESS << 1, reg, I2C_MEMADD_SIZE_8BIT, version, sizeof( version ), HAL_MAX_DELAY );

    if ( status != HAL_OK ) {
        return 0xFF;  // 出错时返回 0xFF
    }
    _version = ( version[ 0 ] << 8 ) | version[ 1 ];  // 芯片版本号 0x001_

    return _version;
}
