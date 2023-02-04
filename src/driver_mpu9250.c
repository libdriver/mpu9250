/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_mpu9250.c
 * @brief     driver mpu9250 source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-08-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/08/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_mpu9250.h"
#include "driver_mpu9250_code.h"
#include <math.h>

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "TDK MPU9250"        /**< chip name */
#define MANUFACTURER_NAME         "TDK"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.4f                 /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                 /**< chip max supply voltage */
#define MAX_CURRENT               3.7f                 /**< chip max current */
#define TEMPERATURE_MIN           -40.0f               /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                /**< chip max operating temperature */
#define DRIVER_VERSION            1000                 /**< driver version */

/**
 * @brief chip ak8963 iic address definition
 */
#define AK8963_IIC_ADDRESS              0x0C        /**< ak8963 iic address */

/**
 * @brief chip register definition
 */
#define MPU9250_REG_SELF_TEST_X_GYRO    0x00        /**< gyro self test x register */
#define MPU9250_REG_SELF_TEST_Y_GYRO    0x01        /**< gyro self test y register */
#define MPU9250_REG_SELF_TEST_Z_GYRO    0x02        /**< gyro self test z register */
#define MPU9250_REG_SELF_TEST_X_ACCEL   0x0D        /**< accel self test x register */
#define MPU9250_REG_SELF_TEST_Y_ACCEL   0x0E        /**< accel self test y register */
#define MPU9250_REG_SELF_TEST_Z_ACCEL   0x0F        /**< accel self test z register */
#define MPU9250_REG_XG_OFFSET_H         0x13        /**< gyro offset x high register */
#define MPU9250_REG_XG_OFFSET_L         0x14        /**< gyro offset x low register */
#define MPU9250_REG_YG_OFFSET_H         0x15        /**< gyro offset y high register */
#define MPU9250_REG_YG_OFFSET_L         0x16        /**< gyro offset y low register */
#define MPU9250_REG_ZG_OFFSET_H         0x17        /**< gyro offset z high register */
#define MPU9250_REG_ZG_OFFSET_L         0x18        /**< gyro offset z low register */
#define MPU9250_REG_SMPRT_DIV           0x19        /**< smprt div register */
#define MPU9250_REG_CONFIG              0x1A        /**< configure register */
#define MPU9250_REG_GYRO_CONFIG         0x1B        /**< gyro configure register */
#define MPU9250_REG_ACCEL_CONFIG        0x1C        /**< accel configure register */
#define MPU9250_REG_ACCEL_CONFIG2       0x1D        /**< accel configure 2 register */
#define MPU9250_REG_LP_ACCEL_ODR        0x1E        /**< low power accel odr register */
#define MPU9250_REG_WOM_THR             0x1F        /**< wom threshold register */
#define MPU9250_REG_MOTION_DURATION     0x20        /**< motion duration register */
#define MPU9250_REG_FIFO_EN             0x23        /**< fifo enable register */
#define MPU9250_REG_I2C_MST_CTRL        0x24        /**< i2c master ctrl register */
#define MPU9250_REG_I2C_MST_STATUS      0x36        /**< i2c master status register */
#define MPU9250_REG_I2C_MST_DELAY_CTRL  0x67        /**< i2c master delay ctrl register */
#define MPU9250_REG_I2C_SLV0_ADDR       0x25        /**< iic slave0 address register */
#define MPU9250_REG_I2C_SLV0_REG        0x26        /**< iic slave0 reg register */
#define MPU9250_REG_I2C_SLV0_CTRL       0x27        /**< iic slave0 ctrl register */
#define MPU9250_REG_I2C_SLV0_DO         0x63        /**< iic slave0 do register */
#define MPU9250_REG_I2C_SLV1_ADDR       0x28        /**< iic slave1 address register */
#define MPU9250_REG_I2C_SLV1_REG        0x29        /**< iic slave1 reg register */
#define MPU9250_REG_I2C_SLV1_CTRL       0x2A        /**< iic slave1 ctrl register */
#define MPU9250_REG_I2C_SLV1_DO         0x64        /**< iic slave1 do register */
#define MPU9250_REG_I2C_SLV2_ADDR       0x2B        /**< iic slave2 address register */
#define MPU9250_REG_I2C_SLV2_REG        0x2C        /**< iic slave2 reg register */
#define MPU9250_REG_I2C_SLV2_CTRL       0x2D        /**< iic slave2 ctrl register */
#define MPU9250_REG_I2C_SLV2_DO         0x65        /**< iic slave2 do register */
#define MPU9250_REG_I2C_SLV3_ADDR       0x2E        /**< iic slave3 address register */
#define MPU9250_REG_I2C_SLV3_REG        0x2F        /**< iic slave3 reg register */
#define MPU9250_REG_I2C_SLV3_CTRL       0x30        /**< iic slave3 ctrl register */
#define MPU9250_REG_I2C_SLV3_DO         0x66        /**< iic slave3 do register */
#define MPU9250_REG_I2C_SLV4_ADDR       0x31        /**< iic slave4 address register */
#define MPU9250_REG_I2C_SLV4_REG        0x32        /**< iic slave4 reg register */
#define MPU9250_REG_I2C_SLV4_CTRL       0x34        /**< iic slave4 ctrl register */
#define MPU9250_REG_I2C_SLV4_DO         0x33        /**< iic slave4 do register */
#define MPU9250_REG_I2C_SLV4_DI         0x35        /**< iic slave4 di register */
#define MPU9250_REG_EXT_SENS_DATA_00    0x49        /**< extern sensor data 00 register */
#define MPU9250_REG_INT_PIN_CFG         0x37        /**< interrupt pin configure register */
#define MPU9250_REG_INT_ENABLE          0x38        /**< interrupt enable register */
#define MPU9250_REG_INT_STATUS          0x3A        /**< interrupt status register */
#define MPU9250_REG_ACCEL_XOUT_H        0x3B        /**< accel xout high register */
#define MPU9250_REG_ACCEL_XOUT_L        0x3C        /**< accel xout low register */
#define MPU9250_REG_ACCEL_YOUT_H        0x3D        /**< accel yout high register */
#define MPU9250_REG_ACCEL_YOUT_L        0x3E        /**< accel yout low register */
#define MPU9250_REG_ACCEL_ZOUT_H        0x3F        /**< accel zout high register */
#define MPU9250_REG_ACCEL_ZOUT_L        0x40        /**< accel zout low register */
#define MPU9250_REG_TEMP_OUT_H          0x41        /**< temp high register */
#define MPU9250_REG_TEMP_OUT_L          0x42        /**< temp low register */
#define MPU9250_REG_GYRO_XOUT_H         0x43        /**< gyro xout high register */
#define MPU9250_REG_GYRO_XOUT_L         0x44        /**< gyro xout low register */
#define MPU9250_REG_GYRO_YOUT_H         0x45        /**< gyro yout high register */
#define MPU9250_REG_GYRO_YOUT_L         0x46        /**< gyro yout low register */
#define MPU9250_REG_GYRO_ZOUT_H         0x47        /**< gyro zout high register */
#define MPU9250_REG_GYRO_ZOUT_L         0x48        /**< gyro zout low register */
#define MPU9250_REG_SIGNAL_PATH_RESET   0x68        /**< signal path reset register */
#define MPU9250_REG_MOT_DETECT_CTRL     0x69        /**< motion detect ctrl register */
#define MPU9250_REG_USER_CTRL           0x6A        /**< user ctrl register */
#define MPU9250_REG_PWR_MGMT_1          0x6B        /**< power management 1 register */
#define MPU9250_REG_PWR_MGMT_2          0x6C        /**< power management 2 register */
#define MPU9250_REG_BANK_SEL            0x6D        /**< bank sel register */
#define MPU9250_REG_MEM                 0x6F        /**< memory register */
#define MPU9250_REG_PROGRAM_START       0x70        /**< program start register */
#define MPU9250_REG_FIFO_COUNTH         0x72        /**< fifo count high threshold register */
#define MPU9250_REG_FIFO_COUNTL         0x73        /**< fifo count low threshold register */
#define MPU9250_REG_R_W                 0x74        /**< fifo read write data register */
#define MPU9250_REG_WHO_AM_I            0x75        /**< who am I register */
#define MPU9250_REG_XA_OFFSET_H         0x77        /**< accel offset x high register */
#define MPU9250_REG_XA_OFFSET_L         0x78        /**< accel offset x low register */
#define MPU9250_REG_YA_OFFSET_H         0x7A        /**< accel offset y high register */
#define MPU9250_REG_YA_OFFSET_L         0x7B        /**< accel offset y low register */
#define MPU9250_REG_ZA_OFFSET_H         0x7D        /**< accel offset z high register */
#define MPU9250_REG_ZA_OFFSET_L         0x7E        /**< accel offset z low register */
#define AK8963_REG_WIA                  0x00        /**< device id register */
#define AK8963_REG_INFO                 0x01        /**< information register */
#define AK8963_REG_ST1                  0x02        /**< status 1 register */
#define AK8963_REG_HXL                  0x03        /**< x axis data high register */
#define AK8963_REG_HXH                  0x04        /**< x axis data low register */
#define AK8963_REG_HYL                  0x05        /**< y axis data high register */
#define AK8963_REG_HYH                  0x06        /**< y axis data low register */
#define AK8963_REG_HZL                  0x07        /**< z axis data high register */
#define AK8963_REG_HZH                  0x08        /**< z axis data low register */
#define AK8963_REG_ST2                  0x09        /**< status 2 register */
#define AK8963_REG_CNTL1                0x0A        /**< control 1 register */
#define AK8963_REG_CNTL2                0x0B        /**< control 2 register */
#define AK8963_REG_ASTC                 0x0C        /**< self test register */
#define AK8963_REG_TS1                  0x0D        /**< test 1 register */
#define AK8963_REG_TS2                  0x0E        /**< test 2 register */
#define AK8963_REG_I2CDIS               0x0F        /**< iic disable register */
#define AK8963_REG_ASAX                 0x10        /**< x axis sensitivity adjustment value register */
#define AK8963_REG_ASAY                 0x11        /**< y axis sensitivity adjustment value register */
#define AK8963_REG_ASAZ                 0x12        /**< z axis sensitivity adjustment value register */

/**
 * @brief mpu9250 dmp code definition
 */
#define MPU9250_DMP_SAMPLE_RATE               200                                                 /**< sample rate */
#define MPU9250_DMP_GYRO_SF                   (46850825LL * 200 / MPU9250_DMP_SAMPLE_RATE)        /**< gyro sf */
#define MPU9250_DMP_D_PEDSTD_TIMECTR          964                                                 /**< walk time mem register */
#define MPU9250_DMP_D_PEDSTD_STEPCTR          (768 + 0x60)                                        /**< step counter mem register */
#define MPU9250_DMP_D_1_36                    (256 + 36)                                          /**< 36 register */
#define MPU9250_DMP_D_1_40                    (256 + 40)                                          /**< 40 register */
#define MPU9250_DMP_D_1_44                    (256 + 44)                                          /**< 44 register */
#define MPU9250_DMP_D_1_72                    (256 + 72)                                          /**< 72 register */
#define MPU9250_DMP_D_1_79                    (256 + 79)                                          /**< 79 register */
#define MPU9250_DMP_D_1_88                    (256 + 88)                                          /**< 88 register */
#define MPU9250_DMP_D_1_90                    (256 + 90)                                          /**< 90 register */
#define MPU9250_DMP_D_1_92                    (256 + 92)                                          /**< 92 register */
#define MPU9250_DMP_D_1_218                   (256 + 218)                                         /**< 218 register */
#define MPU9250_DMP_D_0_22                    (512 + 22)                                          /**< 22 register */
#define MPU9250_DMP_D_0_104                   104                                                 /**< 104 register */
#define MPU9250_DMP_TAPW_MIN                  478                                                 /**< tap time min register */
#define MPU9250_DMP_TAP_THX                   468                                                 /**< tap threshold x register */
#define MPU9250_DMP_TAP_THY                   472                                                 /**< tap threshold y register */
#define MPU9250_DMP_TAP_THZ                   476                                                 /**< tap threshold z register */
#define MPU9250_DMP_CFG_6                     2753                                                /**< cfg 6 register */
#define MPU9250_DMP_CFG_8                     2718                                                /**< cfg 8 register */
#define MPU9250_DMP_CFG_MOTION_BIAS           1208                                                /**< motion bias register */
#define MPU9250_DMP_CFG_LP_QUAT               2712                                                /**< lp quat register */
#define MPU9250_DMP_CFG_FIFO_ON_EVENT         2690                                                /**< fifo on event register */
#define MPU9250_DMP_FCFG_1                    1062                                                /**< fcfg 1 register */
#define MPU9250_DMP_FCFG_2                    1066                                                /**< fcfg 2 register */
#define MPU9250_DMP_FCFG_3                    1088                                                /**< fcfg 3 register */
#define MPU9250_DMP_FCFG_7                    1073                                                /**< fcfg 7 register */
#define MPU9250_DMP_D_EXT_GYRO_BIAS_X         (61 * 16)                                           /**< gyro bias x register */
#define MPU9250_DMP_D_EXT_GYRO_BIAS_Y         (61 * 16 + 4)                                       /**< gyro bias y register */
#define MPU9250_DMP_D_EXT_GYRO_BIAS_Z         (61 * 16 + 8)                                       /**< gyro bias z register */
#define MPU9250_DMP_D_ACCEL_BIAS              660                                                 /**< accel bias register */
#define MPU9250_DMP_FEATURE_SEND_ANY_GYRO     (MPU9250_DMP_FEATURE_SEND_RAW_GYRO | \
                                               MPU9250_DMP_FEATURE_SEND_CAL_GYRO)                 /**< send any gyro register */
#define MPU9250_DMP_CFG_15                    2727                                                /**< cfg 15 register */
#define MPU9250_DMP_CFG_27                    2742                                                /**< cfg 27 register */
#define MPU9250_DMP_CFG_GYRO_RAW_DATA         2722                                                /**< cfg gyro raw data register */
#define MPU9250_DMP_CFG_20                    2224                                                /**< cfg 20 register */
#define MPU9250_DMP_CFG_ORIENT_INT            1853                                                /**< cfg orient int register */
#define MPU9250_DMP_QUAT_ERROR_THRESH         (1L << 24)                                          /**< quat error thresh */
#define MPU9250_DMP_QUAT_MAG_SQ_NORMALIZED    (1L << 28)                                          /**< quat mag sq normalized */
#define MPU9250_DMP_QUAT_MAG_SQ_MIN           (MPU9250_DMP_QUAT_MAG_SQ_NORMALIZED - \
                                               MPU9250_DMP_QUAT_ERROR_THRESH)                     /**< quat mag sq min */
#define MPU9250_DMP_QUAT_MAG_SQ_MAX           (MPU9250_DMP_QUAT_MAG_SQ_NORMALIZED + \
                                               MPU9250_DMP_QUAT_ERROR_THRESH)                     /**< quat mag sq max */
#define MPU9250_DMP_INT_SRC_TAP               0x01                                                /**< int src tap */
#define MPU9250_DMP_INT_SRC_ORIENT            0x08                                                /**< int src orient */
#define MPU9250_DMP_TAP_THRESH                250                                                 /**< 250 mg/ms */
#define MPU9250_DMP_TAP_MIN_TAP_COUNT         1                                                   /**< 1 */
#define MPU9250_DMP_TAP_TIME                  100                                                 /**< 100 ms */
#define MPU9250_DMP_TAP_TIME_MULTI            200                                                 /**< 200 ms */
#define MPU9250_DMP_SHAKE_REJECT_THRESH       200                                                 /**< 200 ms */
#define MPU9250_DMP_SHAKE_REJECT_TIME         40                                                  /**< 40 ms */
#define MPU9250_DMP_SHAKE_REJECT_TIMEOUT      10                                                  /**< 10 ms */

/**
 * @brief inner function definition
 */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))        /**< min function */

/**
 * @brief test st table definition
 */
static uint16_t gs_st_tb[256] = 
{
    2620, 2646, 2672, 2699, 2726, 2753, 2781, 2808,
    2837, 2865, 2894, 2923, 2952, 2981, 3011, 3041,
    3072, 3102, 3133, 3165, 3196, 3228, 3261, 3293,
    3326, 3359, 3393, 3427, 3461, 3496, 3531, 3566,
    3602, 3638, 3674, 3711, 3748, 3786, 3823, 3862,
    3900, 3939, 3979, 4019, 4059, 4099, 4140, 4182,
    4224, 4266, 4308, 4352, 4395, 4439, 4483, 4528,
    4574, 4619, 4665, 4712, 4759, 4807, 4855, 4903,
    4953, 5002, 5052, 5103, 5154, 5205, 5257, 5310,
    5363, 5417, 5471, 5525, 5581, 5636, 5693, 5750,
    5807, 5865, 5924, 5983, 6043, 6104, 6165, 6226,
    6289, 6351, 6415, 6479, 6544, 6609, 6675, 6742,
    6810, 6878, 6946, 7016, 7086, 7157, 7229, 7301,
    7374, 7448, 7522, 7597, 7673, 7750, 7828, 7906,
    7985, 8065, 8145, 8227, 8309, 8392, 8476, 8561,
    8647, 8733, 8820, 8909, 8998, 9088, 9178, 9270,
    9363, 9457, 9551, 9647, 9743, 9841, 9939, 10038,
    10139, 10240, 10343, 10446, 10550, 10656, 10763, 10870,
    10979, 11089, 11200, 11312, 11425, 11539, 11654, 11771,
    11889, 12008, 12128, 12249, 12371, 12495, 12620, 12746,
    12874, 13002, 13132, 13264, 13396, 13530, 13666, 13802,
    13940, 14080, 14221, 14363, 14506, 14652, 14798, 14946,
    15096, 15247, 15399, 15553, 15709, 15866, 16024, 16184,
    16346, 16510, 16675, 16842, 17010, 17180, 17352, 17526,
    17701, 17878, 18057, 18237, 18420, 18604, 18790, 18978,
    19167, 19359, 19553, 19748, 19946, 20145, 20347, 20550,
    20756, 20963, 21173, 21385, 21598, 21814, 22033, 22253,
    22475, 22700, 22927, 23156, 23388, 23622, 23858, 24097,
    24338, 24581, 24827, 25075, 25326, 25579, 25835, 26093,
    26354, 26618, 26884, 27153, 27424, 27699, 27976, 28255,
    28538, 28823, 29112, 29403, 29697, 29994, 30294, 30597,
    30903, 31212, 31524, 31839, 32157, 32479, 32804, 33132,
};

/**
 * @brief      read bytes
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_mpu9250_read(mpu9250_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == MPU9250_INTERFACE_IIC)                                     /* if iic interface */
    {
        if (handle->iic_read(handle->iic_addr, reg, (uint8_t *)buf, len) != 0)        /* read data */
        {
            return 1;                                                                 /* return error */
        }
        else
        {
            return 0;                                                                 /* success return 0 */
        }
    }
    else                                                                              /* spi interface */
    {
        if (handle->spi_read(reg | 0x80, (uint8_t *)buf, len) != 0)                   /* read data */
        {
            return 1;                                                                 /* return error */
        }
        else
        {
            return 0;                                                                 /* success return 0 */
        }
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_mpu9250_write(mpu9250_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == MPU9250_INTERFACE_IIC)                                      /* if iic interface */
    {
        if (handle->iic_write(handle->iic_addr, reg, (uint8_t *)buf, len) != 0)        /* write data */
        {
            return 1;                                                                  /* return error */
        }
        else
        {
            return 0;                                                                  /* success return 0 */
        }
    }
    else                                                                               /* spi interface */
    {
        if (handle->spi_write(reg & (~0x80), (uint8_t *)buf, len) != 0)                /* write data */
        {
            return 1;                                                                  /* return error */
        }
        else
        {
            return 0;                                                                  /* success return 0 */
        }
    }
}

/**
 * @brief     magnetometer write bytes
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_mpu9250_mag_write(mpu9250_handle_t *handle, uint8_t reg, uint8_t *data)
{
    if (handle->iic_spi == MPU9250_INTERFACE_IIC)                                            /* if iic interface */
    {
        if (handle->iic_write(AK8963_IIC_ADDRESS << 1, reg, (uint8_t *)data, 1) != 0)        /* write data */
        {
            return 1;                                                                        /* return error */
        }
        else
        {
            return 0;                                                                        /* success return 0 */
        }
    }
    else                                                                                     /* spi interface */
    {
        return 1;
    }
}

/**
 * @brief      magnetometer read bytes
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_mpu9250_mag_read(mpu9250_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == MPU9250_INTERFACE_IIC)                                            /* if iic interface */
    {
        if (handle->iic_read(AK8963_IIC_ADDRESS << 1, reg, (uint8_t *)buf, len) != 0)        /* read data */
        {
            return 1;                                                                        /* return error */
        }
        else
        {
            return 0;                                                                        /* success return 0 */
        }
    }
    else                                                                                     /* spi interface */
    {
        return 1;
    }
}

/**
 * @brief     write memory bytes
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] addr is the memory address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 len is invalid
 * @note      none
 */
static uint8_t a_mpu9250_write_mem(mpu9250_handle_t *handle, uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t tmp[2];
    
    tmp[0] = (addr >> 8) & 0xFF;                                                                  /* set the addr high */
    tmp[1] = (addr >> 0) & 0xFF;                                                                  /* set the addr low */
    
    if (tmp[1] + len > 256)                                                                       /* check the range */
    {
        return 2;                                                                                 /* return error */
    }
    
    if (handle->iic_spi == MPU9250_INTERFACE_IIC)                                                 /* if iic interface */
    {
        if (handle->iic_write(handle->iic_addr, MPU9250_REG_BANK_SEL, (uint8_t *)tmp, 2) != 0)    /* write data */
        {
            return 1;                                                                             /* return error */
        }
        if (handle->iic_write(handle->iic_addr, MPU9250_REG_MEM, (uint8_t *)buf, len) != 0)       /* write data */
        {
            return 1;                                                                             /* return error */
        }
    }
    else                                                                                          /* if spi interface */
    {
        if (handle->spi_write(MPU9250_REG_BANK_SEL & (~0x80), (uint8_t *)tmp, 2) != 0)            /* write data */
        {
            return 1;                                                                             /* return error */
        }
        if (handle->spi_write(MPU9250_REG_MEM & (~0x80), (uint8_t *)buf, len) != 0)               /* write data */
        {
            return 1;                                                                             /* return error */
        }
    }
    
    return 0;                                                                                     /* success return 0 */
}

/**
 * @brief      read memory bytes
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  addr is the memory address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 len is invalid
 * @note       none
 */
static uint8_t a_mpu9250_read_mem(mpu9250_handle_t *handle, uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t tmp[2];
    
    tmp[0] = (addr >> 8) & 0xFF;                                                                  /* set the addr high */
    tmp[1] = (addr >> 0) & 0xFF;                                                                  /* set the addr low */
    
    if (tmp[1] + len > 256)                                                                       /* check the range */
    {
        return 2;                                                                                 /* return error */
    }
    
    if (handle->iic_spi == MPU9250_INTERFACE_IIC)                                                 /* if iic interface */
    {
        if (handle->iic_write(handle->iic_addr, MPU9250_REG_BANK_SEL, (uint8_t *)tmp, 2) != 0)    /* write data */
        {
            return 1;                                                                             /* return error */
        }
        if (handle->iic_read(handle->iic_addr, MPU9250_REG_MEM, (uint8_t *)buf, len) != 0)        /* read data */
        {
            return 1;                                                                             /* return error */
        }
    }
    else                                                                                          /* if spi interface */
    {
        if (handle->spi_write(MPU9250_REG_BANK_SEL & (~0x80), (uint8_t *)tmp, 2) != 0)            /* write data */
        {
            return 1;                                                                             /* return error */
        }
        if (handle->spi_read(MPU9250_REG_MEM | 0x80, (uint8_t *)buf, len) != 0)                   /* read data */
        {
            return 1;                                                                             /* return error */
        }
    }
    
    return 0;                                                                                     /* success return 0 */
}

/**
 * @brief     reset the fifo
 * @param[in] *handle points to an mpu9250 handle structure
 *            - 0 success
 *            - 1 reset fifo failed
 * @note       none
 */
static uint8_t a_mpu9250_reset_fifo(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    uint8_t int_enable;
    uint8_t fifo_enable;
    uint8_t user_ctrl;
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_ENABLE, &int_enable, 1);            /* read the int enable */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mpu9250: read int enable register failed.\n");          /* read int enable register failed */
        
        return 1;                                                                    /* return error */
    }
    res = a_mpu9250_read(handle, MPU9250_REG_FIFO_EN, &fifo_enable, 1);              /* read the fifo enable */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mpu9250: read fifo enable register failed.\n");         /* read fifo enable register failed */
        
        return 1;                                                                    /* return error */
    }
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, &user_ctrl, 1);              /* read the user ctrl */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mpu9250: read user ctrl register failed.\n");           /* read user ctrl register failed */
        
        return 1;                                                                    /* return error */
    }
    
    prev = 0;                                                                        /* set 0 */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_ENABLE, &prev, 1);                 /* disable all interrupt */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mpu9250: write int enable register failed.\n");         /* write int enable register failed */
        
        return 1;                                                                    /* return error */
    }
    prev = 0;                                                                        /* set 0 */
    res = a_mpu9250_write(handle, MPU9250_REG_FIFO_EN, &prev, 1);                    /* disable all fifo */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mpu9250: write fifo enable register failed.\n");        /* write fifo enable register failed */
        
        return 1;                                                                    /* return error */
    }
    user_ctrl &= ~(1 << 6);                                                          /* disable the fifo */
    user_ctrl &= ~(1 << 7);                                                          /* disable the dmp */
    if (handle->dmp_inited == 1)                                                     /* if we use dmp */
    {
        user_ctrl |= (1 << 2) | (1 << 3);                                            /* reset the fifo and dmp */
    }
    else
    {
        user_ctrl |= 1 << 2;                                                         /* reset the fifo */
    }
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, &user_ctrl, 1);             /* write the user ctrl */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mpu9250: write user ctrl register failed.\n");          /* write user ctrl register failed */
        
        return 1;                                                                    /* return error */
    }
    handle->delay_ms(50);                                                            /* delay 50 ms */
    if (handle->dmp_inited == 1)                                                     /* if we use dmp */
    {
        user_ctrl |= (1 << 6) | (1 << 7);                                            /* enable fifo and dmp */
    }
    else
    {
        user_ctrl |= 1 << 6;                                                         /* enable fifo */
    }
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, &user_ctrl, 1);             /* write the user ctrl */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mpu9250: write user ctrl register failed.\n");          /* write user ctrl register failed */
        
        return 1;                                                                    /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_INT_ENABLE, &int_enable, 1);           /* restore the int enable */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mpu9250: write int enable register failed.\n");         /* write int enable register failed */
        
        return 1;                                                                    /* return error */
    }
    res = a_mpu9250_write(handle, MPU9250_REG_FIFO_EN, &fifo_enable, 1);             /* restore the fifo enable */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mpu9250: write fifo enable register failed.\n");        /* write fifo enable register failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     inv row to scale
 * @param[in] *row points to a row buffer
 * @return    scale
 * @note      none
 */
static uint16_t a_mpu9250_inv_row_2_scale(int8_t *row)
{
    uint16_t b;

    if (row[0] > 0)             /* check row 0 */
    {
        b = 0;                  /* set 0 */
    }
    else if (row[0] < 0)        /* check row 0 */
    {
        b = 4;                  /* set 4 */
    }
    else if (row[1] > 0)        /* check row 1 */
    {
        b = 1;                  /* set 1 */
    }
    else if (row[1] < 0)        /* check row 1 */
    {
        b = 5;                  /* set 5 */
    }
    else if (row[2] > 0)        /* check row 2 */
    {
        b = 2;                  /* set 2 */
    }
    else if (row[2] < 0)        /* check row 2 */
    {
        b = 6;                  /* set 6 */
    }
    else
    {
        b = 7;                  /* set 7 */
    }
    
    return b;                   /* return scale */
}

/**
 * @brief     inv orientation matrix to scalar
 * @param[in] *mtx points to a matrix buffer
 * @return    scale
 * @note      none
 */
static uint16_t a_mpu9250_inv_orientation_matrix_to_scalar(int8_t *mtx)
{
    uint16_t scalar; 
    
    scalar = a_mpu9250_inv_row_2_scale(mtx);                  /* convert the part 0 */
    scalar |= a_mpu9250_inv_row_2_scale(mtx + 3) << 3;        /* convert the part 1 */
    scalar |= a_mpu9250_inv_row_2_scale(mtx + 6) << 6;        /* convert the part 2 */
    
    return scalar;                                            /* return the scalar */
}

/**
 * @brief     dmp decode the gesture
 * @param[in] *gesture points to a gesture buffer
 * @note      none
 */
static void a_mpu9250_dmp_decode_gesture(mpu9250_handle_t *handle, uint8_t gesture[4])
{
    uint8_t tap;
    uint8_t orient;
    
    orient = gesture[3] & 0xC0;                                /* set the orient */
    tap = 0x3F & gesture[3];                                   /* set the tap */
    if ((gesture[1] & MPU9250_DMP_INT_SRC_TAP) != 0)           /* check the tap output */
    {
        uint8_t direction, count;
        
        direction = tap >> 3;                                  /* get the direction */
        count = (tap % 8) + 1;                                 /* get the count */
        if (handle->dmp_tap_callback != NULL)                  /* check the dmp tap callback */
        {
            handle->dmp_tap_callback(direction, count);        /* run the dmp tap callback */
        }
    }
    if ((gesture[1] & MPU9250_DMP_INT_SRC_ORIENT) != 0)        /* check the orient output */
    {
        if (handle->dmp_orient_callback != NULL)               /* check the dmp orient callback */
        {
            handle->dmp_orient_callback(orient >> 6);          /* run the dmp orient callback */
        }
    }
}

/**
 * @brief      run the accel self test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *bias_regular points to a bias regular buffer
 * @param[out] *bias_st points to a bias st buffer
 * @return     status code
 *             - 0 success
 *             - others test failed
 * @note       none
 */
static uint8_t a_mpu9250_accel_self_test(mpu9250_handle_t *handle, int32_t *bias_regular, int32_t *bias_st)
{
    uint8_t res;
    uint8_t i;
    uint8_t otp_value_zero;
    uint8_t regs[3];
    float accel_st_al_min;
    float accel_st_al_max;
    float accel_offset_max;
    float st_shift_cust[3];
    float st_shift_ratio[3];
    float ct_shift_prod[3];
    
    res = a_mpu9250_read(handle, MPU9250_REG_SELF_TEST_X_ACCEL, regs, 3);         /* read the tmp */
    if (res != 0)                                                                 /* check the result */
    {
        return 1;                                                                 /* return error */
    }
    
    otp_value_zero = 0;
    for (i = 0; i < 3; i++)                                                       /* 3 times */
    {
        if (regs[i] != 0)                                                         /* check the regs */
        {
            ct_shift_prod[i] = (float)gs_st_tb[regs[i] - 1];                      /* get the table */
            ct_shift_prod[i] *= 65536.f;                                          /* x65535.0f */
            ct_shift_prod[i] /= 16384.0f;                                         /* 2g */
        }
        else
        {
            ct_shift_prod[i] = 0.0f;                                              /* init 0.0f */
            otp_value_zero = 1;                                                   /* check otp value zero */
        }
    }
    if (otp_value_zero == 0)                                                      /* check otp value zero */
    {
        for (i = 0; i < 3; i++)                                                   /* 3 times */
        {
            st_shift_cust[i] = (float)(bias_st[i] - bias_regular[i]);             /* set the shift cust */
            st_shift_ratio[i] = st_shift_cust[i] / ct_shift_prod[i] - 1.f;        /* set the shift ratio */
            if (fabsf(st_shift_ratio[i]) > 0.5f)                                  /* check the shift ratio */
            {
                return 1;                                                         /* return error */
            }
        }
    }
    else
    {
        accel_st_al_min = 0.225f * 65536.f;                                       /* set the st al min */
        accel_st_al_max = 0.675f * 65536.f;                                       /* set the st al max */
        for (i = 0; i < 3; i++)                                                   /* 3 times */
        {
            st_shift_cust[i] = (float)(bias_st[i] - bias_regular[i]);             /* shift cust */
            if (st_shift_cust[i] < accel_st_al_min 
                || st_shift_cust[i] > accel_st_al_max)                            /* check the result */
            {
                return 1;                                                         /* return error */
            }
        }
    }
    
    accel_offset_max = 0.5f * 65536.f;                                            /* set the offset max */
    for (i = 0; i < 3; i++)                                                       /* 3 times */
    {
        if (fabsf((float)bias_regular[i]) > accel_offset_max)                     /* check offset */
        {
            return 1;                                                             /* return error */
        }
    }

    return 0;                                                                     /* success return 0 */
}

/**
 * @brief      run the gyro self test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *bias_regular points to a bias regular buffer
 * @param[out] *bias_st points to a bias st buffer
 * @return     status code
 *             - 0 success
 *             - others test failed
 * @note       none
 */
static uint8_t a_mpu9250_gyro_self_test(mpu9250_handle_t *handle, int32_t *bias_regular, int32_t *bias_st)
{
    uint8_t res;
    uint8_t i;
    uint8_t otp_value_zero;
    float gyro_st_al_max;
    float gyro_offset_max;
    float st_shift_cust[3];
    float st_shift_ratio[3];
    float ct_shift_prod[3];
    uint8_t regs[3];

    res = a_mpu9250_read(handle, MPU9250_REG_SELF_TEST_X_GYRO, regs, 3);        /* read the tmp */
    if (res != 0)                                                               /* check the result */
    {
        return 1;                                                               /* return error */
    }

    otp_value_zero = 0;                                                         /* init otp value zero 0.0f */
    for (i = 0; i < 3; i++)                                                     /* 3 times */
    {
        if (regs[i] != 0)                                                       /* check regs */
        {
            ct_shift_prod[i] = (float)gs_st_tb[regs[i] - 1];                    /* set shift prod */
            ct_shift_prod[i] *= 65536.f;                                        /* x65536.0f */
            ct_shift_prod[i] /= 131.0f;                                         /* 250dps */
        }
        else
        {
            ct_shift_prod[i] = 0;                                               /* init 0.0f */
            otp_value_zero = 1;                                                 /* set otp value zero 1 */
        }
    }

    if (otp_value_zero == 0)                                                    /* if otp value zero == 0 */
    {
        for (i = 0; i < 3; i++)                                                 /* 3 times */
        {
            st_shift_cust[i] = (float)(bias_st[i] - bias_regular[i]);           /* shift cust */
            st_shift_ratio[i] = st_shift_cust[i] / ct_shift_prod[i];            /* shift ratio */
            if (fabsf(st_shift_ratio[i]) < 0.5f)                                /* check ratio */
            {
                return 1;                                                       /* return error */
            }
        }
    }
    else
    {
        gyro_st_al_max = 60.f * 65536.f;                                        /* set st al max */
        for (i = 0; i < 3; i++)                                                 /* 3 times */
        {
            st_shift_cust[i] = (float)(bias_st[i] - bias_regular[i]);           /* set cust */
            if (st_shift_cust[i] < gyro_st_al_max)                              /* check st al max */
            {
                return 1;                                                       /* return error */
            }
        }
    }
    
    gyro_offset_max = 20.f * 65536.f;                                           /* set gyro offset max */
    for (i = 0; i < 3; i++)                                                     /* 3 times */
    {
        if( fabsf((float)bias_regular[i]) > gyro_offset_max)                    /* check gyro offset max */
        {
            return 1;                                                           /* return error */
        }
    }
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief      get the st biases
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *gyro_offset points to a gyro offset buffer
 * @param[out] *accel_offset points to an accel offset buffer
 * @param[in]  hw_test_enable is the test bool value
 * @return     status code
 *             - 0 success
 *             - 1 others failed
 * @note       none
 */
static uint8_t a_mpu9250_get_st_biases(mpu9250_handle_t *handle,
                                       int32_t gyro_offset[3], int32_t accel_offset[3],
                                       uint8_t hw_test_enable)
{
    uint8_t res;
    uint16_t cnt;
    uint16_t pack_cnt;
    uint16_t i;
    uint8_t data[12];
    
    data[0] = 0x01;                                                                                 /* set 0x01 */
    data[1] = 0x00;                                                                                 /* set 0x00 */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, data, 2);                                 /* write pwr mgmt1 */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    handle->delay_ms(200);                                                                          /* delay 200ms */
    data[0] = 0;                                                                                    /* set 0 */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_ENABLE, data, 1);                                 /* write int enable */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    res = a_mpu9250_write(handle, MPU9250_REG_FIFO_EN, data, 1);                                    /* write the fifo enable */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, data, 1);                                 /* write the pwr mgmt 1 */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_MST_CTRL, data, 1);                               /* write the i2c mst ctrl */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, data, 1);                                  /* write the user ctrl */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    
    data[0] = 1 << 3 | 1 << 2;                                                                      /* set fifo and dmp reset */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, data, 1);                                  /* write user ctrl */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    handle->delay_ms(15);                                                                           /* delay 15 ms */
    data[0] = 0x01;                                                                                 /* set 0x01 */
    res = a_mpu9250_write(handle, MPU9250_REG_CONFIG, data, 1);                                     /* write config */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    data[0] = 0x00;                                                                                 /* set 0 */
    res = a_mpu9250_write(handle, MPU9250_REG_SMPRT_DIV, data, 1);                                  /* write div */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    if (hw_test_enable != 0)                                                                        /* if enable */
    {
        data[0] = 0x00 | 0xE0;                                                                      /* set 250dps and test */
    }
    else
    {
        data[0] = 0x00;                                                                             /* set 250dps */
    }
    res = a_mpu9250_write(handle, MPU9250_REG_GYRO_CONFIG, data, 1);                                /* read the gyro config */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    if (hw_test_enable != 0)                                                                        /* if enable */
    {
        data[0] = 0x18 | 0xE0;                                                                      /* enable 2g and test */
    }
    else
    {
        data[0] = 0x18;                                                                             /* enable 2g */
    }
    res = a_mpu9250_write(handle, MPU9250_REG_ACCEL_CONFIG, data, 1);                               /* read the accel config */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    if (hw_test_enable != 0)                                                                        /* if enable */
    {
        handle->delay_ms(200);                                                                      /* delay 200ms */
    }
    
    data[0] = 1 << 6;                                                                               /* enable fifo */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, data, 1);                                  /* write user ctrl */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    data[0] = 0x78;                                                                                 /* enable xyz */
    res = a_mpu9250_write(handle, MPU9250_REG_FIFO_EN, data, 1);                                    /* write fifo enable */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    handle->delay_ms(50);                                                                           /* delay 50 ms */
    data[0] = 0x00;                                                                                 /* set disable */
    res = a_mpu9250_write(handle, MPU9250_REG_FIFO_EN, data, 1);                                    /* write fifo enable */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_FIFO_COUNTH, data, 2);                                 /* read fifo counter */
    if (res != 0)                                                                                   /* check the result */
    {
        return 1;                                                                                   /* return error */
    }
    cnt = ((uint16_t)data[0] << 8) | data[1];                                                       /* set the counter */
    pack_cnt = cnt / 12;                                                                            /* set the packet counter */
    
    gyro_offset[0] = 0;                                                                             /* gyro offset 0 */
    gyro_offset[1] = 0;                                                                             /* gyro offset 1 */
    gyro_offset[2] = 0;                                                                             /* gyro offset 2 */
    accel_offset[0] = 0;                                                                            /* accel offset 0 */
    accel_offset[1] = 0;                                                                            /* accel offset 1 */
    accel_offset[2] = 0;                                                                            /* accel offset 2 */
    for (i = 0; i < pack_cnt; i++)                                                                  /* packet counter */
    {
        int16_t accel_cur[3];
        int16_t gyro_cur[3];
        
        res = a_mpu9250_read(handle, MPU9250_REG_R_W, data, 12);                                    /* read data */
        if (res != 0)                                                                               /* check the result */
        {
            return 1;                                                                               /* return error */
        }
        
        accel_cur[0] = ((int16_t)data[0] << 8) | data[1];                                           /* accel cur 0 */
        accel_cur[1] = ((int16_t)data[2] << 8) | data[3];                                           /* accel cur 1 */
        accel_cur[2] = ((int16_t)data[4] << 8) | data[5];                                           /* accel cur 2 */
        accel_offset[0] += (int32_t)accel_cur[0];                                                   /* accel offset 0 */
        accel_offset[1] += (int32_t)accel_cur[1];                                                   /* accel offset 1 */
        accel_offset[2] += (int32_t)accel_cur[2];                                                   /* accel offset 2 */
        gyro_cur[0] = (((int16_t)data[6] << 8) | data[7]);                                          /* gyro cur 0 */
        gyro_cur[1] = (((int16_t)data[8] << 8) | data[9]);                                          /* gyro cur 1 */
        gyro_cur[2] = (((int16_t)data[10] << 8) | data[11]);                                        /* gyro cur 2 */
        gyro_offset[0] += (int32_t)gyro_cur[0];                                                     /* gyro offset 0 */
        gyro_offset[1] += (int32_t)gyro_cur[1];                                                     /* gyro offset 1 */
        gyro_offset[2] += (int32_t)gyro_cur[2];                                                     /* gyro offset 2 */
    }
    
    gyro_offset[0] = (int32_t)(((int64_t)gyro_offset[0] << 16) / (32768 / 250) / pack_cnt);         /* set the gyro offset 0 */
    gyro_offset[1] = (int32_t)(((int64_t)gyro_offset[1] << 16) / (32768 / 250) / pack_cnt);         /* set the gyro offset 1 */
    gyro_offset[2] = (int32_t)(((int64_t)gyro_offset[2] << 16) / (32768 / 250) / pack_cnt);         /* set the gyro offset 2 */
    accel_offset[0] = (int32_t)(((int64_t)accel_offset[0] << 16) / (32768 / 16) / pack_cnt);        /* set the accel offset 0 */
    accel_offset[1] = (int32_t)(((int64_t)accel_offset[1] << 16) / (32768 / 16) / pack_cnt);        /* set the accel offset 1 */
    accel_offset[2] = (int32_t)(((int64_t)accel_offset[2] << 16) / (32768 / 16) / pack_cnt);        /* set the accel offset 2 */
    if (accel_offset[2] > 0L)                                                                       /* check the accel offset */
    {
        accel_offset[2] -= 65536L;                                                                  /* -65536 */
    }
    else
    {
        accel_offset[2] += 65536L;                                                                  /* +65536 */
    }
    
    return 0;                                                                                       /* success return 0 */
}

/**
 * @brief     bus deinit
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 deinit failed
 * @note      none
 */
static uint8_t a_mpu9250_deinit(mpu9250_handle_t *handle)
{
    if (handle->iic_spi == MPU9250_INTERFACE_IIC)        /* if iic interface */
    {
        if (handle->iic_deinit() != 0)                   /* iic deinit */
        {
            return 1;                                    /* return error */
        }
        else
        {
            return 0;                                    /* success return 0 */
        }
    }
    else
    {
        if (handle->spi_deinit() != 0)                   /* if spi interface */
        {
            return 1;                                    /* return error */
        }
        else
        {
            return 0;                                    /* success return 0 */
        }
    }
}

/**
 * @brief     load the dmp firmware
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 load firmware failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is running
 *            - 5 code compare error
 *            - 6 set program start failed
 * @note      none
 */
uint8_t mpu9250_dmp_load_firmware(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint16_t i;
    uint16_t size;
    uint16_t this_write;
    uint8_t tmp[2];
    uint8_t cur[16];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (handle->dmp_inited != 0)                                                         /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is running.\n");                               /* dmp is running */
        
        return 4;                                                                        /* return error */
    }
    
    size = MPU9250_DMP_CODE_SIZE;                                                        /* set the code size */
    for (i = 0; i < size; i += this_write)                                               /* code size times */
    {
        this_write = MIN(16, size - i);                                                  /* get the written size */
        
        res = a_mpu9250_write_mem(handle, i, (uint8_t *)(gs_mpu9250_dmp_code + i), 
                                  this_write);                                           /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        res = a_mpu9250_read_mem(handle, i, cur, this_write);                            /* read data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: read mem failed.\n");                          /* read mem failed */
           
            return 1;                                                                    /* return error */
        }
        if (memcmp(gs_mpu9250_dmp_code + i, cur, this_write) != 0)                       /* check the code */
        {
            handle->debug_print("mpu9250: code compare error.\n");                       /* code compare error */
           
            return 5;                                                                    /* return error */
        }
    }
    tmp[0] = (0x0400 >> 8) & 0xFF;                                                       /* set the addr high */
    tmp[1] = (0x0400 >> 0) & 0xFF;                                                       /* set the addr low */
    
    if (handle->iic_write(handle->iic_addr, MPU9250_REG_PROGRAM_START,
                         (uint8_t *)tmp, 2) != 0)                                        /* write data */
    {
        handle->debug_print("mpu9250: set program start failed.\n");                     /* set program start failed */
        
        return 6;                                                                        /* return error */
    }
    handle->dmp_inited = 1;                                                              /* flag the dmp inited bit */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     dmp set the pedometer walk time
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] ms is the walk time
 * @return    status code
 *            - 0 success
 *            - 1 dmp set pedometer walk time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_pedometer_walk_time(mpu9250_handle_t *handle, uint32_t ms)
{
    uint8_t res;
    uint8_t tmp[4];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (handle->dmp_inited != 1)                                                         /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                            /* dmp is not inited */
        
        return 4;                                                                        /* return error */
    }
    
    ms /= 20;                                                                            /* div 20 */
    tmp[0] = (uint8_t)((ms >> 24) & 0xFF);                                               /* set part 0 */
    tmp[1] = (uint8_t)((ms >> 16) & 0xFF);                                               /* set part 1 */
    tmp[2] = (uint8_t)((ms >> 8) & 0xFF);                                                /* set part 2 */
    tmp[3] = (uint8_t)(ms & 0xFF);                                                       /* set part 3 */
    
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_PEDSTD_TIMECTR, tmp, 4);             /* write data */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                             /* write mem failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      dmp get the pedometer walk time
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *ms points to a walk time buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get pedometer walk time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_pedometer_walk_time(mpu9250_handle_t *handle, uint32_t *ms)
{
    uint8_t res;
    uint8_t tmp[4];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (handle->dmp_inited != 1)                                                   /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                      /* dmp is not inited */
        
        return 4;                                                                  /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_PEDSTD_TIMECTR, tmp, 4);        /* read data */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");                        /* read mem failed */
       
        return 1;                                                                  /* return error */
    }
    *ms = (((uint32_t)tmp[0] << 24) | ((uint32_t)tmp[1] << 16) |
           ((uint32_t)tmp[2] << 8) | tmp[3]) * 20;                                 /* get the ms */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     dmp set the pedometer step count
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] count is the step count
 * @return    status code
 *            - 0 success
 *            - 1 dmp set pedometer step count failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_pedometer_step_count(mpu9250_handle_t *handle, uint32_t count)
{
    uint8_t res;
    uint8_t tmp[4];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (handle->dmp_inited != 1)                                                         /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                            /* dmp is not inited */
        
        return 4;                                                                        /* return error */
    }
    
    tmp[0] = (uint8_t)((count >> 24) & 0xFF);                                            /* set part 0 */
    tmp[1] = (uint8_t)((count >> 16) & 0xFF);                                            /* set part 1 */
    tmp[2] = (uint8_t)((count >> 8) & 0xFF);                                             /* set part 2 */
    tmp[3] = (uint8_t)(count & 0xFF);                                                    /* set part 3 */
    
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_PEDSTD_STEPCTR, tmp, 4);             /* write data */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                             /* write mem failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      dmp get the pedometer step count
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *count points to a step count buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get pedometer step count failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_pedometer_step_count(mpu9250_handle_t *handle, uint32_t *count)
{
    uint8_t res;
    uint8_t tmp[4];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (handle->dmp_inited != 1)                                                   /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                      /* dmp is not inited */
        
        return 4;                                                                  /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_PEDSTD_STEPCTR, tmp, 4);        /* read data */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");                        /* read mem failed */
       
        return 1;                                                                  /* return error */
    }
    *count = (((uint32_t)tmp[0] << 24) | ((uint32_t)tmp[1] << 16) |
             ((uint32_t)tmp[2] << 8) | tmp[3]);                                    /* get the ms */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     dmp set the shake reject timeout
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] ms is the reject timeout
 * @return    status code
 *            - 0 success
 *            - 1 dmp set shake reject timeout failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_shake_reject_timeout(mpu9250_handle_t *handle, uint16_t ms)
{
    uint8_t res;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    ms /= (1000 / MPU9250_DMP_SAMPLE_RATE);                                    /* convert time */
    tmp[0] = (ms >> 8) & 0xFF;                                                 /* set part 0 */
    tmp[1] = (ms >> 0) & 0xFF;                                                 /* set part 1 */
    
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_88, tmp, 2);             /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                   /* write mem failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief      dmp get the shake reject timeout
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *ms points to a reject timeout buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get shake reject timeout failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_shake_reject_timeout(mpu9250_handle_t *handle, uint16_t *ms)
{
    uint8_t res;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_1_88, tmp, 2);              /* read data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");                    /* read mem failed */
       
        return 1;                                                              /* return error */
    }
    *ms = (uint16_t)((uint16_t)tmp[0] << 8) | tmp[1];                          /* get the raw time */
    *ms *= (1000 / MPU9250_DMP_SAMPLE_RATE);                                   /* convert time */
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     dmp set the shake reject time
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] ms is the shake reject time
 * @return    status code
 *            - 0 success
 *            - 1 dmp set shake reject time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_shake_reject_time(mpu9250_handle_t *handle, uint16_t ms)
{
    uint8_t res;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    ms /= (1000 / MPU9250_DMP_SAMPLE_RATE);                                    /* convert time */
    tmp[0] = (ms >> 8) & 0xFF;                                                 /* set part 0 */
    tmp[1] = (ms >> 0) & 0xFF;                                                 /* set part 1 */
    
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_90, tmp, 2);             /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                   /* write mem failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief      dmp get the shake reject time
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *ms points to a shake reject time buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get shake reject time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_shake_reject_time(mpu9250_handle_t *handle, uint16_t *ms)
{
    uint8_t res;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_1_90, tmp, 2);              /* read data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");                    /* read mem failed */
       
        return 1;                                                              /* return error */
    }
    *ms = (uint16_t)((uint16_t)tmp[0] << 8) | tmp[1];                          /* get the raw time */
    *ms *= (1000 / MPU9250_DMP_SAMPLE_RATE);                                   /* convert time */
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     dmp set the shake reject thresh
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] dps is the shake reject thresh
 * @return    status code
 *            - 0 success
 *            - 1 dmp set shake reject thresh failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_shake_reject_thresh(mpu9250_handle_t *handle, uint16_t dps)
{
    uint8_t res;
    uint8_t tmp[4];
    uint32_t thresh_scaled;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    thresh_scaled = MPU9250_DMP_GYRO_SF / 1000 * dps;                          /* convert to thresh scaled */
    tmp[0] = (uint8_t)(((uint32_t)thresh_scaled >> 24) & 0xFF);                /* set the part 3 */
    tmp[1] = (uint8_t)(((uint32_t)thresh_scaled >> 16) & 0xFF);                /* set the part 2 */
    tmp[2] = (uint8_t)(((uint32_t)thresh_scaled >> 8) & 0xFF);                 /* set the part 1 */
    tmp[3] = (uint8_t)((uint32_t)thresh_scaled & 0xFF);                        /* set the part 0 */
    
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_92, tmp, 4);             /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                   /* write mem failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief      dmp get the shake reject thresh
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *dps points to a shake reject thresh dps buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get shake reject thresh failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_shake_reject_thresh(mpu9250_handle_t *handle, uint16_t *dps)
{
    uint8_t res;
    uint8_t tmp[4];
    uint32_t thresh_scaled;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_1_92, tmp, 4);              /* read data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");                    /* read mem failed */
       
        return 1;                                                              /* return error */
    }
    thresh_scaled = (((uint32_t)tmp[0] << 24) | ((uint32_t)tmp[1] << 16) |
                    ((uint32_t)tmp[2] << 8) | tmp[3]);                         /* get the thresh scaled */
    *dps = (uint16_t)((float)(thresh_scaled) /
                     ((float)(MPU9250_DMP_GYRO_SF) / 1000.0f));                /* convert the thresh scaled */
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     dmp set max time between taps to register as a multi tap
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] ms is the delay time
 * @return    status code
 *            - 0 success
 *            - 1 dmp set tap time multi failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_tap_time_multi(mpu9250_handle_t *handle, uint16_t ms)
{
    uint8_t res;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    ms /= (1000 / MPU9250_DMP_SAMPLE_RATE);                                    /* convert time */
    tmp[0] = (ms >> 8) & 0xFF;                                                 /* set part 0 */
    tmp[1] = (ms >> 0) & 0xFF;                                                 /* set part 1 */
    
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_218, tmp, 2);            /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                   /* write mem failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief      dmp get max time between taps to register as a multi tap
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *ms points to a delay time buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get tap time multi failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_tap_time_multi(mpu9250_handle_t *handle, uint16_t *ms)
{
    uint8_t res;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_1_218, tmp, 2);             /* read data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");                    /* read mem failed */
       
        return 1;                                                              /* return error */
    }
    *ms = (uint16_t)((uint16_t)tmp[0] << 8) | tmp[1];                          /* get the raw time */
    *ms *= (1000 / MPU9250_DMP_SAMPLE_RATE);                                   /* convert time */
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     dmp set the tap time
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] ms is the tap time
 * @return    status code
 *            - 0 success
 *            - 1 dmp set tap time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_tap_time(mpu9250_handle_t *handle, uint16_t ms)
{
    uint8_t res;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    ms /= (1000 / MPU9250_DMP_SAMPLE_RATE);                                    /* convert time */
    tmp[0] = (ms >> 8) & 0xFF;                                                 /* set part 0 */
    tmp[1] = (ms >> 0) & 0xFF;                                                 /* set part 1 */
    
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_TAPW_MIN, tmp, 2);           /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                   /* write mem failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief      dmp get the tap time
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *ms points to a tap time buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get tap time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_tap_time(mpu9250_handle_t *handle, uint16_t *ms)
{
    uint8_t res;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_TAPW_MIN, tmp, 2);            /* read data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");                    /* read mem failed */
       
        return 1;                                                              /* return error */
    }
    *ms = (uint16_t)((uint16_t)tmp[0] << 8) | tmp[1];                          /* get the raw time */
    *ms *= (1000 / MPU9250_DMP_SAMPLE_RATE);                                   /* convert time */
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     dmp set the min tap count
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] cnt is the tap counter
 * @return    status code
 *            - 0 success
 *            - 1 dmp set min tap count failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 *            - 5 cnt must be between 1 and 4
 * @note      1 <= cnt <= 4
 */
uint8_t mpu9250_dmp_set_min_tap_count(mpu9250_handle_t *handle, uint8_t cnt)
{
    uint8_t res;
    uint8_t tmp;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    if ((cnt < 1) || (cnt > 4))                                                /* check cnt */
    {
        handle->debug_print("mpu9250: cnt must be between 1 and 4.\n");        /* cnt must be between 1 and 4 */
        
        return 5;                                                              /* return error */
    }
    
    tmp = cnt - 1;                                                             /* set the counter */
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_79, &tmp, 1);            /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                   /* write mem failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief      dmp get the min tap count
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *cnt points to a tap counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get min tap count failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_min_tap_count(mpu9250_handle_t *handle, uint8_t *cnt)
{
    uint8_t res;
    uint8_t tmp;
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    if (handle->dmp_inited != 1)                                          /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");             /* dmp is not inited */
        
        return 4;                                                         /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_1_79, &tmp, 1);        /* read data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");               /* read mem failed */
       
        return 1;                                                         /* return error */
    }
    *cnt = tmp + 1;                                                       /* set the counter */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief     dmp enable or disable gyro calibrate
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 dmp set gyro calibrate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_gyro_calibrate(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (handle->dmp_inited != 1)                                                        /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                           /* dmp is not inited */
        
        return 4;                                                                       /* return error */
    }
    
    if (enable != 0)                                                                    /* enable */
    {
        uint8_t regs[9] = {0xb8, 0xaa, 0xb3, 0x8d, 0xb4, 0x98, 0x0d, 0x35, 0x5d};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_MOTION_BIAS, regs, 9);        /* write data */
        if (res != 0)                                                                   /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                        /* write mem failed */
           
            return 1;                                                                   /* return error */
        }
        handle->mask |= MPU9250_DMP_FEATURE_GYRO_CAL;                                   /* set the mask */
        
        return 0;                                                                       /* success return 0 */
    }
    else                                                                                /* disable */
    {
        uint8_t regs[9] = {0xb8, 0xaa, 0xaa, 0xaa, 0xb0, 0x88, 0xc3, 0xc5, 0xc7};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_MOTION_BIAS, regs, 9);        /* write data */
        if (res != 0)                                                                   /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                        /* write mem failed */
           
            return 1;                                                                   /* return error */
        }
        handle->mask &= ~MPU9250_DMP_FEATURE_GYRO_CAL;                                  /* set the mask */
        
        return 0;                                                                       /* success return 0 */
    }
}

/**
 * @brief     dmp enable or disable generate 3 axis quaternions from dmp
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 dmp set 3x quaternion failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_3x_quaternion(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    if (handle->dmp_inited != 1)                                              /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                 /* dmp is not inited */
        
        return 4;                                                             /* return error */
    }
    
    if (enable != 0)                                                          /* enable */
    {
        uint8_t regs[4] = {0xC0, 0xC2, 0xC4, 0xC6};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_LP_QUAT, regs, 4);  /* write data */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
           
            return 1;                                                         /* return error */
        }
        res = a_mpu9250_reset_fifo(handle);                                   /* reset the fifo */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: reset fifo failed.\n");             /* reset fifo failed */
           
            return 1;                                                         /* return error */
        }
        handle->mask |= MPU9250_DMP_FEATURE_3X_QUAT;                          /* set the mask */
        
        return 0;                                                             /* success return 0 */
    }
    else                                                                      /* disable */
    {
        uint8_t regs[4] = {0x8B, 0x8B, 0x8B, 0x8B};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_LP_QUAT, regs, 4);  /* write data */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
           
            return 1;                                                         /* return error */
        }
        res = a_mpu9250_reset_fifo(handle);                                   /* reset the fifo */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: reset fifo failed.\n");             /* reset fifo failed */
           
            return 1;                                                         /* return error */
        }
        handle->mask &= ~MPU9250_DMP_FEATURE_3X_QUAT;                         /* set the mask */
        
        return 0;                                                             /* success return 0 */
    }
}

/**
 * @brief     dmp enable or disable generate 6 axis quaternions from dmp
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 dmp set 6x quaternion failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_6x_quaternion(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    if (handle->dmp_inited != 1)                                              /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                 /* dmp is not inited */
        
        return 4;                                                             /* return error */
    }
    
    if (enable != 0)                                                          /* enable */
    {
        uint8_t regs[4] = {0x20, 0x28, 0x30, 0x38};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_8, regs, 4);        /* write data */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
           
            return 1;                                                         /* return error */
        }
        res = a_mpu9250_reset_fifo(handle);                                   /* reset the fifo */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: reset fifo failed.\n");             /* reset fifo failed */
           
            return 1;                                                         /* return error */
        }
        handle->mask |= MPU9250_DMP_FEATURE_6X_QUAT;                          /* set the mask */
        
        return 0;                                                             /* success return 0 */
    }
    else                                                                      /* disable */
    {
        uint8_t regs[4] = {0xA3, 0xA3, 0xA3, 0xA3};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_8, regs, 4);        /* write data */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
           
            return 1;                                                         /* return error */
        }
        res = a_mpu9250_reset_fifo(handle);                                   /* reset the fifo */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: reset fifo failed.\n");             /* reset fifo failed */
           
            return 1;                                                         /* return error */
        }
        handle->mask &= ~MPU9250_DMP_FEATURE_6X_QUAT;                         /* set the mask */
        
        return 0;                                                             /* success return 0 */
    }
}

/**
 * @brief     dmp set the interrupt mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] mode is the dmp interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 dmp set interrupt mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_interrupt_mode(mpu9250_handle_t *handle, mpu9250_dmp_interrupt_mode_t mode)
{
    uint8_t res;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    if (handle->dmp_inited != 1)                                              /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                 /* dmp is not inited */
        
        return 4;                                                             /* return error */
    }
    
    if (mode == MPU9250_DMP_INTERRUPT_MODE_CONTINUOUS)                        /* continuous */
    {
        uint8_t regs_continuous[11] = {0xd8, 0xb1, 0xb9, 
                                       0xf3, 0x8b, 0xa3, 
                                       0x91, 0xb6, 0x09, 
                                       0xb4, 0xd9};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_FIFO_ON_EVENT, 
                                  (uint8_t *)regs_continuous, 11);            /* write data */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
           
            return 1;                                                         /* return error */
        }
        
        return 0;                                                             /* success return 0 */
    }
    else                                                                      /* disable */
    {
        uint8_t regs_gesture[11] = {0xda, 0xb1, 0xb9, 
                                    0xf3, 0x8b, 0xa3, 
                                    0x91, 0xb6, 0xda, 
                                    0xb4, 0xda};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_FIFO_ON_EVENT, 
                                  (uint8_t *)regs_gesture, 11);               /* write data */
        if (res != 0)                                                         /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
           
            return 1;                                                         /* return error */
        }
        
        return 0;                                                             /* success return 0 */
    }
}

/**
 * @brief     dmp set the gyro bias
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] *bias points to a bias buffer
 * @return    status code
 *            - 0 success
 *            - 1 dmp set gyro bias failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_gyro_bias(mpu9250_handle_t *handle, int32_t bias[3])
{
    uint8_t res;
    uint8_t regs[4];
    int32_t gyro_bias_body[3];
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    if (handle->dmp_inited != 1)                                                                    /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                                       /* dmp is not inited */
        
        return 4;                                                                                   /* return error */
    }
    
    gyro_bias_body[0] = bias[handle->orient & 3];                                                   /* set the body 0 */
    if ((handle->orient & 4) != 0)                                                                  /* check bit 3 */
    {
        gyro_bias_body[0] *= -1;                                                                    /* *(-1) */
    }
    gyro_bias_body[1] = bias[(handle->orient >> 3) & 3];                                            /* set the body 1 */
    if ((handle->orient & 0x20) != 0)                                                               /* check bit 6 */
    {
        gyro_bias_body[1] *= -1;                                                                    /* *(-1) */
    }
    gyro_bias_body[2] = bias[(handle->orient >> 6) & 3];                                            /* set the body 2 */
    if ((handle->orient & 0x100) != 0)                                                              /* check bit 9 */
    {
        gyro_bias_body[2] *= -1;                                                                    /* *(-1) */
    }
    
    gyro_bias_body[0] = (int32_t)(((int64_t)gyro_bias_body[0] * MPU9250_DMP_GYRO_SF) >> 30);        /* set body 0 */
    gyro_bias_body[1] = (int32_t)(((int64_t)gyro_bias_body[1] * MPU9250_DMP_GYRO_SF) >> 30);        /* set body 1 */
    gyro_bias_body[2] = (int32_t)(((int64_t)gyro_bias_body[2] * MPU9250_DMP_GYRO_SF) >> 30);        /* set body 2 */
    
    regs[0] = (uint8_t)((gyro_bias_body[0] >> 24) & 0xFF);                                          /* set part 0 */
    regs[1] = (uint8_t)((gyro_bias_body[0] >> 16) & 0xFF);                                          /* set part 1 */
    regs[2] = (uint8_t)((gyro_bias_body[0] >> 8) & 0xFF);                                           /* set part 2 */
    regs[3] = (uint8_t)(gyro_bias_body[0] & 0xFF);                                                  /* set part 3 */
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_EXT_GYRO_BIAS_X, (uint8_t *)regs, 4);           /* write data */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                                        /* write mem failed */
       
        return 1;                                                                                   /* return error */
    }
    regs[0] = (uint8_t)((gyro_bias_body[1] >> 24) & 0xFF);                                          /* set part 0 */
    regs[1] = (uint8_t)((gyro_bias_body[1] >> 16) & 0xFF);                                          /* set part 1 */
    regs[2] = (uint8_t)((gyro_bias_body[1] >> 8) & 0xFF);                                           /* set part 2 */
    regs[3] = (uint8_t)(gyro_bias_body[1] & 0xFF);                                                  /* set part 3 */
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_EXT_GYRO_BIAS_Y, (uint8_t *)regs, 4);           /* write data */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                                        /* write mem failed */
       
        return 1;                                                                                   /* return error */
    }
    regs[0] = (uint8_t)((gyro_bias_body[2] >> 24) & 0xFF);                                          /* set part 0 */
    regs[1] = (uint8_t)((gyro_bias_body[2] >> 16) & 0xFF);                                          /* set part 1 */
    regs[2] = (uint8_t)((gyro_bias_body[2] >> 8) & 0xFF);                                           /* set part 2 */
    regs[3] = (uint8_t)(gyro_bias_body[2] & 0xFF);                                                  /* set part 3 */
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_EXT_GYRO_BIAS_Z, (uint8_t *)regs, 4);           /* write data */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                                        /* write mem failed */
       
        return 1;                                                                                   /* return error */
    }
    
    return 0;                                                                                       /* success return 0 */
}

/**
 * @brief     dmp set the accel bias
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] *bias points to a bias buffer
 * @return    status code
 *            - 0 success
 *            - 1 dmp set accel bias failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_accel_bias(mpu9250_handle_t *handle, int32_t bias[3])
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    int32_t accel_bias_body[3];
    uint8_t regs[12];
    int64_t accel_sf;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    if (handle->dmp_inited != 1)                                                            /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                               /* dmp is not inited */
        
        return 4;                                                                           /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&prev, 1);            /* read accelerometer config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer config failed.\n");                /* read accelerometer config failed */
       
        return 1;                                                                           /* return error */
    }
    range = ((prev >> 3) & 0x3);                                                            /* get the range */
    if (range == 0)                                                                         /* if 2g */
    {
        accel_sf = (int64_t)16384 << 15;                                                    /* set the accel sf */
    }
    else if (range == 1)                                                                    /* if 4g */
    {
        accel_sf = (int64_t)8092 << 15;                                                     /* set the accel sf */
    }
    else if (range == 2)                                                                    /* if 8g */
    {
        accel_sf = (int64_t)4096 << 15;                                                     /* set the accel sf */
    }
    else                                                                                    /* if 16g */
    {
        accel_sf = (int64_t)2048 << 15;                                                     /* set the accel sf */
    }
    
    accel_bias_body[0] = bias[handle->orient & 3];                                          /* set the bias body 0 */
    if ((handle->orient & 4) != 0)                                                          /* check the orient */
    {
        accel_bias_body[0] *= -1;                                                           /* *(-1) */
    }
    accel_bias_body[1] = bias[(handle->orient >> 3) & 3];                                   /* set the bias body 1 */
    if ((handle->orient & 0x20) != 0)                                                       /* check the orient */
    {
        accel_bias_body[1] *= -1;                                                           /* *(-1) */
    }
    accel_bias_body[2] = bias[(handle->orient >> 6) & 3];                                   /* set the bias body 2 */
    if ((handle->orient & 0x100) != 0)                                                      /* check the orient */
    {
        accel_bias_body[2] *= -1;                                                           /* *(-1) */
    }
    
    accel_bias_body[0] = (int32_t)(((int64_t)accel_bias_body[0] * accel_sf) >> 30);         /* set the bias body 0 */
    accel_bias_body[1] = (int32_t)(((int64_t)accel_bias_body[1] * accel_sf) >> 30);         /* set the bias body 1 */
    accel_bias_body[2] = (int32_t)(((int64_t)accel_bias_body[2] * accel_sf) >> 30);         /* set the bias body 2 */
    regs[0] = (uint8_t)((accel_bias_body[0] >> 24) & 0xFF);                                 /* set reg 0 */
    regs[1] = (uint8_t)((accel_bias_body[0] >> 16) & 0xFF);                                 /* set reg 1 */
    regs[2] = (uint8_t)((accel_bias_body[0] >> 8) & 0xFF);                                  /* set reg 2 */
    regs[3] = (uint8_t)(accel_bias_body[0] & 0xFF);                                         /* set reg 3 */
    regs[4] = (uint8_t)((accel_bias_body[1] >> 24) & 0xFF);                                 /* set reg 4 */
    regs[5] = (uint8_t)((accel_bias_body[1] >> 16) & 0xFF);                                 /* set reg 5 */
    regs[6] = (uint8_t)((accel_bias_body[1] >> 8) & 0xFF);                                  /* set reg 6 */
    regs[7] = (uint8_t)(accel_bias_body[1] & 0xFF);                                         /* set reg 7 */
    regs[8] = (uint8_t)((accel_bias_body[2] >> 24) & 0xFF);                                 /* set reg 8 */
    regs[9] = (uint8_t)((accel_bias_body[2] >> 16) & 0xFF);                                 /* set reg 9 */
    regs[10] = (uint8_t)((accel_bias_body[2] >> 8) & 0xFF);                                 /* set reg 10 */
    regs[11] = (uint8_t)(accel_bias_body[2] & 0xFF);                                        /* set reg 11 */
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_ACCEL_BIAS, (uint8_t *)regs, 12);       /* write data */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                                /* write mem failed */
       
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     dmp set the orientation
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] *mat points to an orientation matrix buffer
 * @return    status code
 *            - 0 success
 *            - 1 dmp set orientation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_orientation(mpu9250_handle_t *handle, int8_t mat[9])
{
    uint8_t gyro_axes[4] = {0x4C, 0xCD, 0x6C, 0x00};
    uint8_t accel_axes[4] = {0x0C, 0xC9, 0x2C, 0x00 };
    uint8_t gyro_sign[4] = {0x36, 0x56, 0x76, 0x00 };
    uint8_t accel_sign[4] = {0x26, 0x46, 0x66, 0x00 };
    uint8_t res;
    uint8_t gyro_regs[3];
    uint8_t accel_regs[3];
    uint16_t orient;
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    if (handle->dmp_inited != 1)                                          /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");             /* dmp is not inited */
        
        return 4;                                                         /* return error */
    }
    
    orient = a_mpu9250_inv_orientation_matrix_to_scalar(mat);             /* inv orientation matrix to scalar */
    gyro_regs[0] = gyro_axes[orient & 3];                                 /* set the gyro regs 0 */
    gyro_regs[1] = gyro_axes[(orient >> 3) & 3];                          /* set the gyro regs 1 */
    gyro_regs[2] = gyro_axes[(orient >> 6) & 3];                          /* set the gyro regs 2 */
    accel_regs[0] = accel_axes[orient & 3];                               /* set the accel regs 0 */
    accel_regs[1] = accel_axes[(orient >> 3) & 3];                        /* set the accel regs 1 */
    accel_regs[2] = accel_axes[(orient >> 6) & 3];                        /* set the accel regs 2 */
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_FCFG_1, 
                              (uint8_t *)gyro_regs, 3);                   /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
       
        return 1;                                                         /* return error */
    }
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_FCFG_2, 
                              (uint8_t *)accel_regs, 3);                  /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
       
        return 1;                                                         /* return error */
    }
    
    memcpy(gyro_regs, gyro_sign, 3);                                      /* copy the gyro regs */
    memcpy(accel_regs, accel_sign, 3);                                    /* copy the accel regs */
    if ((orient & 4) != 0)                                                /* bit 3 */
    {
        gyro_regs[0] |= 1;                                                /* set 1 */
        accel_regs[0] |= 1;                                               /* set 1 */
    }
    if ((orient & 0x20) != 0)                                             /* bit 6 */
    {
        gyro_regs[1] |= 1;                                                /* set 1 */
        accel_regs[1] |= 1;                                               /* set 1 */
    }
    if ((orient & 0x100) != 0)                                            /* bit 9 */
    {
        gyro_regs[2] |= 1;                                                /* set 1 */
        accel_regs[2] |= 1;                                               /* set 1 */
    }
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_FCFG_3, 
                              (uint8_t *)gyro_regs, 3);                   /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
       
        return 1;                                                         /* return error */
    }
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_FCFG_7, 
                              (uint8_t *)accel_regs, 3);                  /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");              /* write mem failed */
       
        return 1;                                                         /* return error */
    }
    handle->orient = orient;                                              /* set the orient */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief     dmp enable or disable the dmp feature
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] mask is the set mask
 * @return    status code
 *            - 0 success
 *            - 1 dmp set feature failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      mask can be MPU9250_DMP_FEATURE_TAP, MPU9250_DMP_FEATURE_ORIENT
 *            MPU9250_DMP_FEATURE_3X_QUAT, MPU9250_DMP_FEATURE_PEDOMETER
 *            MPU9250_DMP_FEATURE_6X_QUAT, MPU9250_DMP_FEATURE_GYRO_CAL
 *            MPU9250_DMP_FEATURE_SEND_RAW_ACCEL, MPU9250_DMP_FEATURE_SEND_RAW_GYRO
 *            MPU9250_DMP_FEATURE_SEND_CAL_GYRO or combination
 */
uint8_t mpu9250_dmp_set_feature(mpu9250_handle_t *handle, uint16_t mask)
{
    uint8_t res;
    uint8_t tmp[10];
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (handle->dmp_inited != 1)                                                         /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                            /* dmp is not inited */
        
        return 4;                                                                        /* return error */
    }
    
    tmp[0] = (uint8_t)((MPU9250_DMP_GYRO_SF >> 24) & 0xFF);                              /* set the param 0 */
    tmp[1] = (uint8_t)((MPU9250_DMP_GYRO_SF >> 16) & 0xFF);                              /* set the param 1 */
    tmp[2] = (uint8_t)((MPU9250_DMP_GYRO_SF >> 8) & 0xFF);                               /* set the param 2 */
    tmp[3] = (uint8_t)(MPU9250_DMP_GYRO_SF & 0xFF);                                      /* set the param 3 */
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_0_104, tmp, 4);                      /* write data */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                             /* write mem failed */
       
        return 1;                                                                        /* return error */
    }
    
    tmp[0] = 0xA3;
    if ((mask & MPU9250_DMP_FEATURE_SEND_RAW_ACCEL) != 0)                                /* set the raw accel */
    {
        tmp[1] = 0xC0;                                                                   /* set the param 1 */
        tmp[2] = 0xC8;                                                                   /* set the param 2 */
        tmp[3] = 0xC2;                                                                   /* set the param 3 */
    }
    else
    {
        tmp[1] = 0xA3;                                                                   /* set the param 1 */
        tmp[2] = 0xA3;                                                                   /* set the param 2 */
        tmp[3] = 0xA3;                                                                   /* set the param 3 */
    }
    if ((mask & MPU9250_DMP_FEATURE_SEND_ANY_GYRO) != 0)                                 /* set any gyro */
    {
        tmp[4] = 0xC4;                                                                   /* set the param 4 */
        tmp[5] = 0xCC;                                                                   /* set the param 5 */
        tmp[6] = 0xC6;                                                                   /* set the param 6 */
    }
    else
    {
        tmp[4] = 0xA3;                                                                   /* set the param 4 */
        tmp[5] = 0xA3;                                                                   /* set the param 5 */
        tmp[6] = 0xA3;                                                                   /* set the param 6 */
    }
    tmp[7] = 0xA3;                                                                       /* set the param 7 */
    tmp[8] = 0xA3;                                                                       /* set the param 8 */
    tmp[9] = 0xA3;                                                                       /* set the param 9 */
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_15, tmp, 10);                      /* write data */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                             /* write mem failed */
       
        return 1;                                                                        /* return error */
    }
    
    if ((mask & (MPU9250_DMP_FEATURE_TAP | MPU9250_DMP_FEATURE_ORIENT)) != 0)            /* set the cfg */
    {
        tmp[0] = 0x20;                                                                   /* set the param */
    }
    else
    {
        tmp[0] = 0xD8;                                                                   /* set the param */
    }
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_27, tmp, 1);                       /* write data */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                             /* write mem failed */
       
        return 1;                                                                        /* return error */
    }
    
    if ((mask & MPU9250_DMP_FEATURE_GYRO_CAL) != 0)                                      /* if true */
    {
        uint8_t regs[9] = {0xb8, 0xaa, 0xb3, 0x8d, 0xb4, 0x98, 0x0d, 0x35, 0x5d};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_MOTION_BIAS, regs, 9);         /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
    }
    else
    {
        uint8_t regs[9] = {0xb8, 0xaa, 0xaa, 0xaa, 0xb0, 0x88, 0xc3, 0xc5, 0xc7};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_MOTION_BIAS, regs, 9);         /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
    }
    
    if ((mask & MPU9250_DMP_FEATURE_SEND_ANY_GYRO) != 0)                                 /* check the gyro */
    {
        if ((mask & MPU9250_DMP_FEATURE_SEND_CAL_GYRO) != 0)                             /* set the cal gyro */
        {
            tmp[0] = 0xB2;                                                               /* set the param 0 */
            tmp[1] = 0x8B;                                                               /* set the param 1 */
            tmp[2] = 0xB6;                                                               /* set the param 2 */
            tmp[3] = 0x9B;                                                               /* set the param 3 */
        }
        else
        {
            tmp[0] = 0xC0;                                                               /* set the param 0 */
            tmp[1] = 0x80;                                                               /* set the param 1 */
            tmp[2] = 0xC2;                                                               /* set the param 2 */
            tmp[3] = 0x90;                                                               /* set the param 3 */
        }
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_GYRO_RAW_DATA, tmp, 4);        /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
    }
    
    if ((mask & MPU9250_DMP_FEATURE_TAP) != 0)                                           /* check the tap */
    {
        uint8_t prev;
        uint16_t ms;
        uint16_t dps;
        uint32_t thresh_scaled;
        uint8_t range;
        uint16_t dmp_thresh;
        uint16_t dmp_thresh_2;
        float scaled_thresh;
        
        tmp[0] = 0xF8;                                                                   /* set the param */
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_20, tmp, 1);                   /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        scaled_thresh = (float)MPU9250_DMP_TAP_THRESH / MPU9250_DMP_SAMPLE_RATE;         /* get the scaled thresh */
        res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&prev, 1);     /* read accelerometer config */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: read accelerometer config failed.\n");         /* read accelerometer config failed */
           
            return 1;                                                                    /* return error */
        }
        range = ((prev >> 3) & 0x3);                                                     /* get the range */
        if (range == 0)                                                                  /* if 2g */
        {
            dmp_thresh = (uint16_t)(scaled_thresh * 16384);                              /* set dmp thresh */
            dmp_thresh_2 = (uint16_t)(scaled_thresh * 12288);                            /* set dmp thresh2 */
        }
        else if (range == 1)                                                             /* if 4g */
        {
            dmp_thresh = (uint16_t)(scaled_thresh * 8192);                               /* set dmp thresh */
            dmp_thresh_2 = (uint16_t)(scaled_thresh * 6144);                             /* set dmp thresh2 */
        }
        else if (range == 2)                                                             /* if 8g */
        {
            dmp_thresh = (uint16_t)(scaled_thresh * 4096);                               /* set dmp thresh */
            dmp_thresh_2 = (uint16_t)(scaled_thresh * 3072);                             /* set dmp thresh2 */
        }
        else                                                                             /* if 16g */
        {
            dmp_thresh = (uint16_t)(scaled_thresh * 2048);                               /* set dmp thresh */
            dmp_thresh_2 = (uint16_t)(scaled_thresh * 1536);                             /* set dmp thresh2 */
        }
        tmp[0] = (uint8_t)(dmp_thresh >> 8);                                             /* set part 0 */
        tmp[1] = (uint8_t)(dmp_thresh & 0xFF);                                           /* set part 1 */
        tmp[2] = (uint8_t)(dmp_thresh_2 >> 8);                                           /* set part 2 */
        tmp[3] = (uint8_t)(dmp_thresh_2 & 0xFF);                                         /* set part 3 */
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_TAP_THX, tmp, 2);                  /* write tap threshold x */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_36, tmp + 2, 2);               /* write register 36 */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_TAP_THY, tmp, 2);                  /* write tap threshold y */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_40, tmp + 2, 2);               /* write register 40 */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_TAP_THZ, tmp, 2);                  /* write tap threshold z */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_44, tmp + 2, 2);               /* write register 44 */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        tmp[0] = 0x3F;                                                                   /* enable all tap axis */
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_72, tmp, 1);                   /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        tmp[0] = MPU9250_DMP_TAP_MIN_TAP_COUNT - 1;                                      /* set the counter */
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_79, tmp, 1);                   /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        ms = MPU9250_DMP_TAP_TIME;                                                       /* set tap time */
        ms /= (1000 / MPU9250_DMP_SAMPLE_RATE);                                          /* convert time */
        tmp[0] = (ms >> 8) & 0xFF;                                                       /* set part 0 */
        tmp[1] = (ms >> 0) & 0xFF;                                                       /* set part 1 */
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_TAPW_MIN, tmp, 2);                 /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        ms = MPU9250_DMP_TAP_TIME_MULTI;                                                 /* set tap time multi */
        ms /= (1000 / MPU9250_DMP_SAMPLE_RATE);                                          /* convert time */
        tmp[0] = (ms >> 8) & 0xFF;                                                       /* set part 0 */
        tmp[1] = (ms >> 0) & 0xFF;                                                       /* set part 1 */
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_218, tmp, 2);                  /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        dps = MPU9250_DMP_SHAKE_REJECT_THRESH;                                           /* set the shake reject thresh */
        thresh_scaled = MPU9250_DMP_GYRO_SF / 1000 * dps;                                /* convert to thresh scaled */
        tmp[0] = (uint8_t)(((uint32_t)thresh_scaled >> 24) & 0xFF);                      /* set the part 3 */
        tmp[1] = (uint8_t)(((uint32_t)thresh_scaled >> 16) & 0xFF);                      /* set the part 2 */
        tmp[2] = (uint8_t)(((uint32_t)thresh_scaled >> 8) & 0xFF);                       /* set the part 1 */
        tmp[3] = (uint8_t)((uint32_t)thresh_scaled & 0xFF);                              /* set the part 0 */
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_92, tmp, 4);                   /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        ms = MPU9250_DMP_SHAKE_REJECT_TIME;                                              /* set the reject time */
        ms /= (1000 / MPU9250_DMP_SAMPLE_RATE);                                          /* convert time */
        tmp[0] = (ms >> 8) & 0xFF;                                                       /* set part 0 */
        tmp[1] = (ms >> 0) & 0xFF;                                                       /* set part 1 */
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_90, tmp, 2);                   /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        
        ms = MPU9250_DMP_SHAKE_REJECT_TIMEOUT;                                           /* set the reject timeout */
        ms /= (1000 / MPU9250_DMP_SAMPLE_RATE);                                          /* convert time */
        tmp[0] = (ms >> 8) & 0xFF;                                                       /* set part 0 */
        tmp[1] = (ms >> 0) & 0xFF;                                                       /* set part 1 */
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_88, tmp, 2);                   /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
    }
    else
    {
        tmp[0] = 0xD8;                                                                   /* set the param */
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_20, tmp, 1);                   /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
    }
    
    if ((mask & MPU9250_DMP_FEATURE_ORIENT) != 0)                                        /* set the orient */
    {
        tmp[0] = 0xD9;                                                                   /* set the param */
    }
    else
    {
        tmp[0] = 0xD8;                                                                   /* set the param */
    }
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_ORIENT_INT, tmp, 1);               /* write data */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                             /* write mem failed */
       
        return 1;                                                                        /* return error */
    }
    
    if ((mask & MPU9250_DMP_FEATURE_3X_QUAT) != 0)                                       /* true */
    {
        uint8_t regs[4] = {0xC0, 0xC2, 0xC4, 0xC6};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_LP_QUAT, regs, 4);             /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        res = a_mpu9250_reset_fifo(handle);                                              /* reset the fifo */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: reset fifo failed.\n");                        /* reset fifo failed */
           
            return 1;                                                                    /* return error */
        }
    }
    else
    {
        uint8_t regs[4] = {0x8B, 0x8B, 0x8B, 0x8B};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_LP_QUAT, regs, 4);             /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        res = a_mpu9250_reset_fifo(handle);                                              /* reset the fifo */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: reset fifo failed.\n");                        /* reset fifo failed */
           
            return 1;                                                                    /* return error */
        }
    }
    
    if ((mask & MPU9250_DMP_FEATURE_6X_QUAT) != 0)                                       /* enable */
    {
        uint8_t regs[4] = {0x20, 0x28, 0x30, 0x38};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_8, regs, 4);                   /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        res = a_mpu9250_reset_fifo(handle);                                              /* reset the fifo */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: reset fifo failed.\n");                        /* reset fifo failed */
           
            return 1;                                                                    /* return error */
        }
    }
    else
    {
        uint8_t regs[4] = {0xA3, 0xA3, 0xA3, 0xA3};
        
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_8, regs, 4);                   /* write data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");                         /* write mem failed */
           
            return 1;                                                                    /* return error */
        }
        res = a_mpu9250_reset_fifo(handle);                                              /* reset the fifo */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("mpu9250: reset fifo failed.\n");                        /* reset fifo failed */
           
            return 1;                                                                    /* return error */
        }
    }
    
    handle->mask = mask | MPU9250_DMP_FEATURE_PEDOMETER;                                 /* set the mask */
    
    return a_mpu9250_reset_fifo(handle);                                                 /* reset the fifo */
}

/**
 * @brief     dmp set the fifo rate
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] rate is the set rate
 * @return    status code
 *            - 0 success
 *            - 1 dmp set fifo rate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 *            - 5 rate > 200
 * @note      none
 */
uint8_t mpu9250_dmp_set_fifo_rate(mpu9250_handle_t *handle, uint16_t rate)
{
    uint8_t regs_end[12] = {0xFE, 0xF2, 0xAB,
                            0xC4, 0xAA, 0xF1,
                            0xDF, 0xDF, 0xBB,
                            0xAF, 0xDF, 0xDF};
    uint8_t res;
    uint16_t d;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    if (rate > MPU9250_DMP_SAMPLE_RATE)                                        /* check rate */
    {
        handle->debug_print("mpu9250: rate > 200.\n");                         /* rate > 200 */
        
        return 5;                                                              /* return error */
    }
    
    d = MPU9250_DMP_SAMPLE_RATE / rate - 1;                                    /* set div */
    tmp[0] = (uint8_t)((d >> 8) & 0xFF);                                       /* set tmp part0 */
    tmp[1] = (uint8_t)(d & 0xFF);                                              /* set tmp part1 */
    
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_0_22, tmp, 2);             /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                   /* write mem failed */
       
        return 1;                                                              /* return error */
    }
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_CFG_6,
                             (uint8_t *)regs_end, 12);                         /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                   /* write mem failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief      dmp get the fifo rate
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *rate points to a rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get fifo rate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_fifo_rate(mpu9250_handle_t *handle, uint16_t *rate)
{
    uint8_t res;
    uint16_t d;
    uint8_t tmp[2];
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    if (handle->dmp_inited != 1)                                         /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");            /* dmp is not inited */
        
        return 4;                                                        /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_0_22, tmp, 2);        /* read data */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");              /* read mem failed */
       
        return 1;                                                        /* return error */
    }
    d = (uint16_t)tmp[0] << 8 | tmp[1];                                  /* get the div */
    *rate = MPU9250_DMP_SAMPLE_RATE / (d + 1);                           /* set the rate */
    
    return 0;                                                            /* success return 0 */
}

/**
 * @brief     dmp enable or disable the tap axes
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] axis is the set axis
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 dmp set tap axes failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_tap_axes(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t tmp;
    uint8_t pos;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_1_72, &tmp, 1);             /* read data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");                    /* read mem failed */
       
        return 1;                                                              /* return error */
    }
    pos = (uint8_t)((axis - 5) * 2);                                           /* get the pos */
    if (enable == MPU9250_BOOL_TRUE)                                           /* if enable */
    {
        tmp |= (3 << pos);                                                     /* enable */
    }
    else
    {
        tmp &= ~(3 << pos);                                                    /* disable */
    }
    res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_72, &tmp, 1);            /* write data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: write mem failed.\n");                   /* write mem failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief      dmp get the tap axes status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  axis is the set axis
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get tap axes failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_get_tap_axes(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t tmp;
    uint8_t pos;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    res = a_mpu9250_read_mem(handle, MPU9250_DMP_D_1_72, &tmp, 1);             /* read data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: read mem failed.\n");                    /* read mem failed */
       
        return 1;                                                              /* return error */
    }
    pos = (uint8_t)((axis - 5) * 2);                                           /* get the pos */
    if (((tmp >> pos) & 0x3) == 0x3)                                           /* if enable */
    {
        *enable = MPU9250_BOOL_TRUE;                                           /* set enable */
    }
    else
    {
        *enable = MPU9250_BOOL_FALSE;                                          /* set disable */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     dmp set the tap thresh
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] axis is the set axis
 * @param[in] mg_ms is the set thresh
 * @return    status code
 *            - 0 success
 *            - 1 dmp set tap thresh failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 *            - 5 mg/ms > 1600
 *            - 6 invalid axis
 * @note      none
 */
uint8_t mpu9250_dmp_set_tap_thresh(mpu9250_handle_t *handle, mpu9250_axis_t axis, uint16_t mg_ms)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    uint8_t tmp[4];
    uint16_t dmp_thresh;
    uint16_t dmp_thresh_2;
    float scaled_thresh;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    if (mg_ms > 1600)                                                          /* check the mg/ms */
    {
        handle->debug_print("mpu9250: mg/ms > 1600.\n");                       /* mg/ms > 1600 */
        
        return 5;                                                              /* return error */
    }
    
    scaled_thresh = (float)mg_ms / MPU9250_DMP_SAMPLE_RATE;                    /* get the scaled thresh */
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG,
                        (uint8_t *)&prev, 1);                                  /* read accelerometer config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer config failed.\n");   /* read accelerometer config failed */
       
        return 1;                                                              /* return error */
    }
    range = ((prev >> 3) & 0x3);                                               /* get the range */
    if (range == 0)                                                            /* if 2g */
    {
        dmp_thresh = (uint16_t)(scaled_thresh * 16384);                        /* set dmp thresh */
        dmp_thresh_2 = (uint16_t)(scaled_thresh * 12288);                      /* set dmp thresh2 */
    }
    else if (range == 1)                                                       /* if 4g */
    {
        dmp_thresh = (uint16_t)(scaled_thresh * 8192);                         /* set dmp thresh */
        dmp_thresh_2 = (uint16_t)(scaled_thresh * 6144);                       /* set dmp thresh2 */
    }
    else if (range == 2)                                                       /* if 8g */
    {
        dmp_thresh = (uint16_t)(scaled_thresh * 4096);                         /* set dmp thresh */
        dmp_thresh_2 = (uint16_t)(scaled_thresh * 3072);                       /* set dmp thresh2 */
    }
    else                                                                       /* if 16g */
    {
        dmp_thresh = (uint16_t)(scaled_thresh * 2048);                         /* set dmp thresh */
        dmp_thresh_2 = (uint16_t)(scaled_thresh * 1536);                       /* set dmp thresh2 */
    }
    tmp[0] = (uint8_t)(dmp_thresh >> 8);                                       /* set part 0 */
    tmp[1] = (uint8_t)(dmp_thresh & 0xFF);                                     /* set part 1 */
    tmp[2] = (uint8_t)(dmp_thresh_2 >> 8);                                     /* set part 2 */
    tmp[3] = (uint8_t)(dmp_thresh_2 & 0xFF);                                   /* set part 3 */
    
    if (axis == MPU9250_AXIS_X)                                                /* if axis x */
    {
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_TAP_THX, tmp, 2);        /* write tap threshold x */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");               /* write mem failed */
           
            return 1;                                                          /* return error */
        }
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_36, tmp + 2, 2);     /* write register 36 */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");               /* write mem failed */
           
            return 1;                                                          /* return error */
        }
        
        return 0;                                                              /* success return 0 */
    }
    else if (axis == MPU9250_AXIS_Y)                                           /* if axis y */
    {
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_TAP_THY, tmp, 2);        /* write tap threshold y */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");               /* write mem failed */
           
            return 1;                                                          /* return error */
        }
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_40, tmp + 2, 2);     /* write register 40 */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");               /* write mem failed */
           
            return 1;                                                          /* return error */
        }
        
        return 0;                                                              /* success return 0 */
    }
    else if (axis == MPU9250_AXIS_Z)                                           /* if axis z */
    {
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_TAP_THZ, tmp, 2);        /* write tap threshold z */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");               /* write mem failed */
           
            return 1;                                                          /* return error */
        }
        res = a_mpu9250_write_mem(handle, MPU9250_DMP_D_1_44, tmp + 2, 2);     /* write register 44 */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("mpu9250: write mem failed.\n");               /* write mem failed */
           
            return 1;                                                          /* return error */
        }
        
        return 0;                                                              /* success return 0 */
    }
    else
    {
        handle->debug_print("mpu9250: invalid axis.\n");                       /* invalid axis */
        
        return 6;                                                              /* return error */
    }
}

/**
 * @brief      dmp get the tap thresh
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  axis is the set axis
 * @param[out] *mg_ms points to an mg/ms thresh buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp get tap thresh failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 *             - 5 invalid axis
 * @note       none
 */
uint8_t mpu9250_dmp_get_tap_thresh(mpu9250_handle_t *handle, mpu9250_axis_t axis, uint16_t *mg_ms)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    uint8_t tmp[2];
    uint16_t dmp_thresh;
    float scaled_thresh;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (handle->dmp_inited != 1)                                               /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                  /* dmp is not inited */
        
        return 4;                                                              /* return error */
    }
    
    if (axis == MPU9250_AXIS_X)                                                /* if axis x */
    {
        res = a_mpu9250_read_mem(handle, MPU9250_DMP_TAP_THX, tmp, 2);         /* read tap threshold x */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("mpu9250: read mem failed.\n");                /* read mem failed */
           
            return 1;                                                          /* return error */
        }
    }
    else if (axis == MPU9250_AXIS_Y)                                           /* if axis y */
    {
        res = a_mpu9250_read_mem(handle, MPU9250_DMP_TAP_THY, tmp, 2);         /* read tap threshold y */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("mpu9250: read mem failed.\n");                /* read mem failed */
           
            return 1;                                                          /* return error */
        }
    }
    else if (axis == MPU9250_AXIS_Z)                                           /* if axis z */
    {
        res = a_mpu9250_read_mem(handle, MPU9250_DMP_TAP_THZ, tmp, 2);         /* read tap threshold z */
        if (res != 0)                                                          /* check result */
        {
            handle->debug_print("mpu9250: read mem failed.\n");                /* read mem failed */
           
            return 1;                                                          /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid axis.\n");                       /* invalid axis */
        
        return 5;                                                              /* return error */
    }
    dmp_thresh = (uint16_t)tmp[0] << 8 | tmp[1];                               /* set the dmp thresh */
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG,
                        (uint8_t *)&prev, 1);                                  /* read accelerometer config */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer config failed.\n");   /* read accelerometer config failed */
       
        return 1;                                                              /* return error */
    }
    range = ((prev >> 3) & 0x3);                                               /* get the range */
    if (range == 0)                                                            /* if 2g */
    {
        scaled_thresh = dmp_thresh / 16384.0f;                                 /* set dmp thresh */
    }
    else if (range == 1)                                                       /* if 4g */
    {
        scaled_thresh = dmp_thresh / 8192.0f;                                  /* set dmp thresh */
    }
    else if (range == 2)                                                       /* if 8g */
    {
        scaled_thresh = dmp_thresh / 4096.0f;                                  /* set dmp thresh */
    }
    else                                                                       /* if 16g */
    {
        scaled_thresh = dmp_thresh / 2048.0f;                                  /* set dmp thresh */
    }
    *mg_ms = (uint16_t)(scaled_thresh * MPU9250_DMP_SAMPLE_RATE);              /* set the mg/ms */
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief         dmp read the data
 * @param[in]     *handle points to an mpu9250 handle structure
 * @param[out]    *accel_raw points to an accel raw buffer
 * @param[out]    *accel_g points to an accel g buffer
 * @param[out]    *gyro_raw points to a gyro raw buffer
 * @param[out]    *gyro_dps points to a gyro dps buffer
 * @param[out]    *quat points to a quat buffer
 * @param[out]    *pitch points to a pitch buffer
 * @param[out]    *roll points to a roll buffer
 * @param[out]    *yaw points to a yaw buffer
 * @param[in,out] *l points to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 dmp get fifo rate failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 dmp is not inited
 *                - 5 quat check error
 *                - 6 fifo overflow
 *                - 7 fifo data is too little
 *                - 8 no data
 * @note          none
 */
uint8_t mpu9250_dmp_read(mpu9250_handle_t *handle,
                         int16_t (*accel_raw)[3], float (*accel_g)[3],
                         int16_t (*gyro_raw)[3], float (*gyro_dps)[3],
                         int32_t (*quat)[4],
                         float *pitch, float *roll, float *yaw,
                         uint16_t *l
                        )
{
    uint8_t res;
    uint8_t i = 0;
    uint16_t len;
    uint8_t buf[2];
    uint8_t prev;
    uint16_t count;
    uint16_t j;
    
    if (handle == NULL)                                                                                                   /* check handle */
    {
        return 2;                                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                                              /* check handle initialization */
    {
        return 3;                                                                                                         /* return error */
    }
    if (handle->dmp_inited != 1)                                                                                          /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                                                             /* dmp is not inited */
        
        return 4;                                                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_STATUS, (uint8_t *)&prev, 1);                                            /* read config */
    if (res != 0)                                                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read int status failed.\n");                                                        /* read int status failed */
       
        return 1;                                                                                                         /* return error */
    }
    if ((prev & (1 << MPU9250_INTERRUPT_FIFO_OVERFLOW)) != 0)                                                             /* if fifo overflow */
    {
        handle->debug_print("mpu9250: fifo overflow.\n");                                                                 /* fifo overflow */
        (void)a_mpu9250_reset_fifo(handle);                                                                               /* reset the fifo */
        
        return 6;                                                                                                         /* return error */
    }
    
    len = 0;                                                                                                              /* set len 0 */
    if ((handle->mask & MPU9250_DMP_FEATURE_SEND_RAW_ACCEL) != 0)                                                         /* check the accel */
    {
        len += 6;                                                                                                         /* size += 6 */
    }
    if ((handle->mask & MPU9250_DMP_FEATURE_SEND_ANY_GYRO) != 0)                                                          /* check the gyro */
    {
        len += 6;                                                                                                         /* size += 6 */
    }
    if ((handle->mask & (MPU9250_DMP_FEATURE_3X_QUAT | MPU9250_DMP_FEATURE_6X_QUAT)) != 0)                                /* check the quat */
    {
        len += 16;                                                                                                        /* size += 16 */
    }
    if ((handle->mask & (MPU9250_DMP_FEATURE_TAP | MPU9250_DMP_FEATURE_ORIENT)) != 0)                                     /* check the tap and orient */
    {
        len += 4;                                                                                                         /* size += 4 */
    }
    if (len == 0)                                                                                                         /* check the len */
    {
        handle->debug_print("mpu9250: no data.\n");                                                                       /* no data */
       
        return 8;                                                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_FIFO_COUNTH, (uint8_t *)buf, 2);                                             /* read fifo count */
    if (res != 0)                                                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read fifo count failed.\n");                                                        /* read fifo count failed */
       
        return 1;                                                                                                         /* return error */
    }
    count = (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                                                 /* set count */
    count = (count < 1024) ? count : 1024;                                                                                /* just the counter */
    count = (count < (*l) * len) ? count : ((*l) *len);                                                                   /* just outer buffer size */
    count = (count / len) * len;                                                                                          /* len times */
    *l = count / len;                                                                                                     /* set the output length */
    res = a_mpu9250_read(handle, MPU9250_REG_R_W, handle->buf, count);                                                    /* read data */
    if (res != 0)                                                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read failed.\n");                                                                   /* read failed */
       
        return 1;                                                                                                         /* return error */
    }
    if (count < len)                                                                                                      /* check the count */
    {
        handle->debug_print("mpu9250: fifo data is too little.\n");                                                       /* fifo data is too little */
        
        return 7;                                                                                                         /* return error */
    }
    
    for (j = 0; j < (*l); j++)                                                                                            /* (*l) times */
    {
        if ((handle->mask & (MPU9250_DMP_FEATURE_3X_QUAT | MPU9250_DMP_FEATURE_6X_QUAT)) != 0)                            /* check the quat */
        {
            int32_t quat_q14[4];
            int32_t quat_mag_sq;
            float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
            
            i = 0;                                                                                                        /* set 0 */
            quat[j][0] = ((int32_t)handle->buf[0 + len * j] << 24) | ((int32_t)handle->buf[1 + len * j] << 16) |
                         ((int32_t)handle->buf[2 + len * j] << 8) | handle->buf[3 + len * j];                             /* set the quat 0 */
            quat[j][1] = ((int32_t)handle->buf[4 + len * j] << 24) | ((int32_t)handle->buf[5 + len * j] << 16) |
                         ((int32_t)handle->buf[6 + len * j] << 8) | handle->buf[7 + len * j];                             /* set the quat 1 */
            quat[j][2] = ((int32_t)handle->buf[8 + len * j] << 24) | ((int32_t)handle->buf[9 + len * j] << 16) |
                         ((int32_t)handle->buf[10 + len * j] << 8) | handle->buf[11 + len * j];                           /* set the quat 2 */
            quat[j][3] = ((int32_t)handle->buf[12 + len * j] << 24) | ((int32_t)handle->buf[13 + len * j] << 16) |
                         ((int32_t)handle->buf[14 + len * j] << 8) | handle->buf[15 + len * j];                           /* set the quat 3 */
            i += 16;                                                                                                      /* size += 16 */
            
            quat_q14[0] = quat[j][0] >> 16;                                                                               /* set the quat q14[0] */
            quat_q14[1] = quat[j][1] >> 16;                                                                               /* set the quat q14[1] */
            quat_q14[2] = quat[j][2] >> 16;                                                                               /* set the quat q14[2] */
            quat_q14[3] = quat[j][3] >> 16;                                                                               /* set the quat q14[3] */
            quat_mag_sq = quat_q14[0] * quat_q14[0] + quat_q14[1] * quat_q14[1] +
                          quat_q14[2] * quat_q14[2] + quat_q14[3] * quat_q14[3];                                          /* get the quat mag sq */
            if ((quat_mag_sq < MPU9250_DMP_QUAT_MAG_SQ_MIN) ||
                (quat_mag_sq > MPU9250_DMP_QUAT_MAG_SQ_MAX))                                                              /* check the size */
            {
                handle->debug_print("mpu9250: quat check error.\n");                                                      /* quat check error */
                (void)a_mpu9250_reset_fifo(handle);                                                                       /* reset the fifo */
                
                return 5;                                                                                                 /* return error */
            }
            q0 = quat[j][0] / 1073741824.0f;                                                                              /* set q0 */
            q1 = quat[j][1] / 1073741824.0f;                                                                              /* set q1 */
            q2 = quat[j][2] / 1073741824.0f;                                                                              /* set q2 */
            q3 = quat[j][3] / 1073741824.0f;                                                                              /* set q3 */
            pitch[j] = asinf(-2 * q1 * q3 + 2 * q0* q2)* 57.3f;                                                           /* set pitch */
            roll[j] = atan2f(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1)* 57.3f;                           /* set roll */
            yaw[j] = atan2f(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3f;                      /* set yaw */
        }
        else
        {
            quat[j][0] = 0;                                                                                               /* set 0 */
            quat[j][1] = 0;                                                                                               /* set 0 */
            quat[j][2] = 0;                                                                                               /* set 0 */
            quat[j][3] = 0;                                                                                               /* set 0 */
            pitch[j] = 0.0f;                                                                                              /* set 0.0f */
            roll[j] = 0.0f;                                                                                               /* set 0.0f */
            yaw[j] = 0.0f;                                                                                                /* set 0.0f */
        }
        if ((handle->mask & MPU9250_DMP_FEATURE_SEND_RAW_ACCEL) != 0)                                                     /* check the accel */
        {
            uint8_t accel_conf;
            
            accel_raw[j][0] = ((int16_t)handle->buf[i + 0 + len * j] << 8) | handle->buf[i + 1 + len * j];                /* set the accel x raw data */
            accel_raw[j][1] = ((int16_t)handle->buf[i + 2 + len * j] << 8) | handle->buf[i + 3 + len * j];                /* set the accel y raw data */
            accel_raw[j][2] = ((int16_t)handle->buf[i + 4 + len * j] << 8) | handle->buf[i + 5 + len * j];                /* set the accel z raw data */
            i += 6;                                                                                                       /* size += 6 */
            
            res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&accel_conf, 1);                            /* read accel config */
            if (res != 0)                                                                                                 /* check result */
            {
                handle->debug_print("mpu9250: read accel config failed.\n");                                              /* read accel config failed */
               
                return 1;                                                                                                 /* return error */
            }
            accel_conf = (accel_conf >> 3) & 0x3;                                                                         /* get the accel conf */
            if (accel_conf == 0)                                                                                          /* ±2g */
            {
                accel_g[j][0] = (float)(accel_raw[j][0]) / 16384.0f;                                                      /* set accel x */
                accel_g[j][1] = (float)(accel_raw[j][1]) / 16384.0f;                                                      /* set accel y */
                accel_g[j][2] = (float)(accel_raw[j][2]) / 16384.0f;                                                      /* set accel z */
            }
            else if (accel_conf == 1)                                                                                     /* ±4g */
            {
                accel_g[j][0] = (float)(accel_raw[j][0]) / 8192.0f;                                                       /* set accel x */
                accel_g[j][1] = (float)(accel_raw[j][1]) / 8192.0f;                                                       /* set accel y */
                accel_g[j][2] = (float)(accel_raw[j][2]) / 8192.0f;                                                       /* set accel z */
            }
            else if (accel_conf == 2)                                                                                     /* ±8g */
            {
                accel_g[j][0] = (float)(accel_raw[j][0]) / 4096.0f;                                                       /* set accel x */
                accel_g[j][1] = (float)(accel_raw[j][1]) / 4096.0f;                                                       /* set accel y */
                accel_g[j][2] = (float)(accel_raw[j][2]) / 4096.0f;                                                       /* set accel z */
            }
            else                                                                                                          /* ±16g */
            {
                accel_g[j][0] = (float)(accel_raw[j][0]) / 2048.0f;                                                       /* set accel x */
                accel_g[j][1] = (float)(accel_raw[j][1]) / 2048.0f;                                                       /* set accel y */
                accel_g[j][2] = (float)(accel_raw[j][2]) / 2048.0f;                                                       /* set accel z */
            }
        }
        else
        {
            accel_raw[j][0] = 0;                                                                                          /* set 0 */
            accel_raw[j][1] = 0;                                                                                          /* set 0 */
            accel_raw[j][2] = 0;                                                                                          /* set 0 */
            accel_g[j][0] = 0.0f;                                                                                         /* set 0.0f */
            accel_g[j][1] = 0.0f;                                                                                         /* set 0.0f */
            accel_g[j][2] = 0.0f;                                                                                         /* set 0.0f */
        }
        if ((handle->mask & MPU9250_DMP_FEATURE_SEND_ANY_GYRO) != 0)                                                      /* check the gyro */
        {
            uint8_t gyro_conf;
            
            gyro_raw[j][0] = ((int16_t)handle->buf[i + 0 + len * j] << 8) | handle->buf[i + 1 + len * j];                 /* set the gyro x raw data */
            gyro_raw[j][1] = ((int16_t)handle->buf[i + 2 + len * j] << 8) | handle->buf[i + 3 + len * j];                 /* set the gyro y raw data */
            gyro_raw[j][2] = ((int16_t)handle->buf[i + 4 + len * j] << 8) | handle->buf[i + 5 + len * j];                 /* set the gyro z raw data */
            i += 6;                                                                                                       /* size += 6 */
            
            res = a_mpu9250_read(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&gyro_conf, 1);                              /* read gyro config */
            if (res != 0)                                                                                                 /* check result */
            {
                handle->debug_print("mpu9250: read gyro config failed.\n");                                               /* read gyro config failed */
               
                return 1;                                                                                                 /* return error */
            }
            gyro_conf = (gyro_conf >> 3) & 0x3;                                                                           /* get the gyro conf */
            if (gyro_conf == 0)                                                                                           /* ±250dps */
            {
                gyro_dps[j][0] = (float)(gyro_raw[j][0]) / 131.0f;                                                        /* set gyro x */
                gyro_dps[j][1] = (float)(gyro_raw[j][1]) / 131.0f;                                                        /* set gyro y */
                gyro_dps[j][2] = (float)(gyro_raw[j][2]) / 131.0f;                                                        /* set gyro z */
            }
            else if (gyro_conf == 1)                                                                                      /* ±500dps */
            {
                gyro_dps[j][0] = (float)(gyro_raw[j][0]) / 65.5f;                                                         /* set gyro x */
                gyro_dps[j][1] = (float)(gyro_raw[j][1]) / 65.5f;                                                         /* set gyro y */
                gyro_dps[j][2] = (float)(gyro_raw[j][2]) / 65.5f;                                                         /* set gyro z */
            }
            else if (gyro_conf == 2)                                                                                      /* ±1000dps */
            {
                gyro_dps[j][0] = (float)(gyro_raw[j][0]) / 32.8f;                                                         /* set gyro x */
                gyro_dps[j][1] = (float)(gyro_raw[j][1]) / 32.8f;                                                         /* set gyro y */
                gyro_dps[j][2] = (float)(gyro_raw[j][2]) / 32.8f;                                                         /* set gyro z */
            }
            else                                                                                                          /* ±2000dps */
            {
                gyro_dps[j][0] = (float)(gyro_raw[j][0]) / 16.4f;                                                         /* set gyro x */
                gyro_dps[j][1] = (float)(gyro_raw[j][1]) / 16.4f;                                                         /* set gyro y */
                gyro_dps[j][2] = (float)(gyro_raw[j][2]) / 16.4f;                                                         /* set gyro z */
            }
        }
        else
        {
            gyro_raw[j][0] = 0;                                                                                           /* set 0 */
            gyro_raw[j][1] = 0;                                                                                           /* set 0 */
            gyro_raw[j][2] = 0;                                                                                           /* set 0 */
            gyro_dps[j][0] = 0.0f;                                                                                        /* set 0.0f */
            gyro_dps[j][1] = 0.0f;                                                                                        /* set 0.0f */
            gyro_dps[j][2] = 0.0f;                                                                                        /* set 0.0f */
        }
        if ((handle->mask & (MPU9250_DMP_FEATURE_TAP | MPU9250_DMP_FEATURE_ORIENT)) != 0)                                 /* check the tap and orient */
        {
            a_mpu9250_dmp_decode_gesture(handle, handle->buf + i + len * j);                                              /* run the decode gesture */
        }
    }
    
    return 0;                                                                                                             /* success return 0 */
}

/**
 * @brief     dmp set the tap callback
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] *callback points to a callback function address
 * @return    status code
 *            - 0 success
 *            - 1 dmp set tap callback failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_tap_callback(mpu9250_handle_t *handle, void (*callback)(uint8_t count, uint8_t direction))
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    if (handle->dmp_inited != 1)                                     /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");        /* dmp is not inited */
        
        return 4;                                                    /* return error */
    }
    
    handle->dmp_tap_callback = callback;                             /* set the callback */
    
    return 0;                                                        /* success return 0 */
}

/**
 * @brief     dmp set the orient callback
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] *callback points to a callback function address
 * @return    status code
 *            - 0 success
 *            - 1 dmp set orient callback failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_orient_callback(mpu9250_handle_t *handle, void (*callback)(uint8_t orientation))
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    if (handle->dmp_inited != 1)                                     /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");        /* dmp is not inited */
        
        return 4;                                                    /* return error */
    }
    
    handle->dmp_orient_callback = callback;                          /* set the callback */
    
    return 0;                                                        /* success return 0 */
}

/**
 * @brief     enable or disable the dmp
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 dmp set enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 dmp is not inited
 * @note      none
 */
uint8_t mpu9250_dmp_set_enable(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (handle->dmp_inited != 1)                                                         /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                            /* dmp is not inited */
        
        return 4;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);            /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                        /* read user ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 7);                                                                   /* clear config */
    prev |= enable << 7;                                                                 /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);           /* write config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write user ctrl failed.\n");                       /* write user ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      dmp gyro accel raw offset convert
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  *gyro_offset_raw points to a gyro offset raw buffer
 * @param[in]  *accel_offset_raw points to an accel offset raw buffer
 * @param[out] *gyro_offset points to a gyro offset buffer
 * @param[out] *accel_offset points to an accel offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 dmp set enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 dmp is not inited
 * @note       none
 */
uint8_t mpu9250_dmp_gyro_accel_raw_offset_convert(mpu9250_handle_t *handle,
                                                  int32_t gyro_offset_raw[3], int32_t accel_offset_raw[3], 
                                                  int32_t gyro_offset[3], int32_t accel_offset[3])
{
    uint8_t res;
    uint8_t accel_conf;
    uint8_t gyro_conf;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    if (handle->dmp_inited != 1)                                                                  /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is not inited.\n");                                     /* dmp is not inited */
        
        return 4;                                                                                 /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&accel_conf, 1);            /* read accel config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mpu9250: read accel config failed.\n");                              /* read accel config failed */
       
        return 1;                                                                                 /* return error */
    }
    res = a_mpu9250_read(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&gyro_conf, 1);              /* read gyro config */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("mpu9250: read gyro config failed.\n");                               /* read gyro config failed */
       
        return 1;                                                                                 /* return error */
    }
    accel_conf = (accel_conf >> 3) & 0x3;                                                         /* get the accel conf */
    gyro_conf = (gyro_conf >> 3) & 0x3;                                                           /* get the gyro conf */
    if (accel_conf == 0)                                                                          /* ±2g */
    {
        accel_offset[0] = (int32_t)(accel_offset_raw[0] * 16384.0f);                              /* set accel offset 0 */
        accel_offset[1] = (int32_t)(accel_offset_raw[1] * 16384.0f);                              /* set accel offset 1 */
        accel_offset[2] = (int32_t)(accel_offset_raw[2] * 16384.0f);                              /* set accel offset 2 */
    }
    else if (accel_conf == 1)                                                                     /* ±4g */
    {
        accel_offset[0] = (int32_t)(accel_offset_raw[0] * 8092.0f);                               /* set accel offset 0 */
        accel_offset[1] = (int32_t)(accel_offset_raw[1] * 8092.0f);                               /* set accel offset 1 */
        accel_offset[2] = (int32_t)(accel_offset_raw[2] * 8092.0f);                               /* set accel offset 2 */
    }
    else if (accel_conf == 2)                                                                     /* ±8g */
    {
        accel_offset[0] = (int32_t)(accel_offset_raw[0] * 4096.0f);                               /* set accel offset 0 */
        accel_offset[1] = (int32_t)(accel_offset_raw[1] * 4096.0f);                               /* set accel offset 1 */
        accel_offset[2] = (int32_t)(accel_offset_raw[2] * 4096.0f);                               /* set accel offset 2 */
    }
    else                                                                                          /* ±16g */
    {
        accel_offset[0] = (int32_t)(accel_offset_raw[0] * 2048.0f);                               /* set accel offset 0 */
        accel_offset[1] = (int32_t)(accel_offset_raw[1] * 2048.0f);                               /* set accel offset 1 */
        accel_offset[2] = (int32_t)(accel_offset_raw[2] * 2048.0f);                               /* set accel offset 2 */
    }
    if (gyro_conf == 0)                                                                           /* ±250dps */
    {
        gyro_offset[0] = (int32_t)(gyro_offset_raw[0] * 131.f);                                   /* set gyro offset 0 */
        gyro_offset[1] = (int32_t)(gyro_offset_raw[1] * 131.f);                                   /* set gyro offset 1 */
        gyro_offset[2] = (int32_t)(gyro_offset_raw[2] * 131.f);                                   /* set gyro offset 2 */
    }
    else if (gyro_conf == 1)                                                                      /* ±500dps */
    {
        gyro_offset[0] = (int32_t)(gyro_offset_raw[0] * 65.5f);                                   /* set gyro offset 0 */
        gyro_offset[1] = (int32_t)(gyro_offset_raw[1] * 65.5f);                                   /* set gyro offset 1 */
        gyro_offset[2] = (int32_t)(gyro_offset_raw[2] * 65.5f);                                   /* set gyro offset 2 */
    }
    else if (gyro_conf == 2)                                                                      /* ±1000dps */
    {
        gyro_offset[0] = (int32_t)(gyro_offset_raw[0] * 32.8f);                                   /* set gyro offset 0 */
        gyro_offset[1] = (int32_t)(gyro_offset_raw[1] * 32.8f);                                   /* set gyro offset 1 */
        gyro_offset[2] = (int32_t)(gyro_offset_raw[2] * 32.8f);                                   /* set gyro offset 2 */
    }
    else                                                                                          /* ±2000dps */
    {
        gyro_offset[0] = (int32_t)(gyro_offset_raw[0] * 16.4f);                                   /* set gyro offset 0 */
        gyro_offset[1] = (int32_t)(gyro_offset_raw[1] * 16.4f);                                   /* set gyro offset 1 */
        gyro_offset[2] = (int32_t)(gyro_offset_raw[2] * 16.4f);                                   /* set gyro offset 2 */
    }
    
    return 0;                                                                                     /* success return 0 */
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t mpu9250_set_interface(mpu9250_handle_t *handle, mpu9250_interface_t interface)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_spi = (uint8_t)interface;        /* set interface */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mpu9250_get_interface(mpu9250_handle_t *handle, mpu9250_interface_t *interface)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    
    *interface = (mpu9250_interface_t)(handle->iic_spi);        /* get interface */
    
    return 0;                                                   /* success return 0 */
}

/**
 * @brief     set the chip address pin
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] addr_pin is the chip address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t mpu9250_set_addr_pin(mpu9250_handle_t *handle, mpu9250_address_t addr_pin)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }

    handle->iic_addr = (uint8_t)addr_pin;        /* set iic addr */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip address pin
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *addr_pin points to a chip address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mpu9250_get_addr_pin(mpu9250_handle_t *handle, mpu9250_address_t *addr_pin)
{
    if (handle == NULL)                                      /* check handle */
    {
        return 2;                                            /* return error */
    }

    *addr_pin = (mpu9250_address_t)(handle->iic_addr);       /* get iic addr */
    
    return 0;                                                /* success return 0 */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 *            - 5 id is invalid
 * @note      none
 */
uint8_t mpu9250_init(mpu9250_handle_t *handle)
{
    uint8_t res, prev;
    uint32_t timeout;
  
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->debug_print == NULL)                                                /* check debug_print */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->iic_init == NULL)                                                   /* check iic_init */
    {
        handle->debug_print("mpu9250: iic_init is null.\n");                        /* iic_init is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_deinit == NULL)                                                 /* check iic_deinit */
    {
        handle->debug_print("mpu9250: iic_deinit is null.\n");                      /* iic_deinit is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_read == NULL)                                                   /* check iic_read */
    {
        handle->debug_print("mpu9250: iic_read is null.\n");                        /* iic_read is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->iic_write == NULL)                                                  /* check iic_write */
    {
        handle->debug_print("mpu9250: iic_write is null.\n");                       /* iic_write is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_init == NULL)                                                   /* check spi_init */
    {
        handle->debug_print("mpu9250: spi_init is null.\n");                        /* spi_init is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_deinit == NULL)                                                 /* check spi_deinit */
    {
        handle->debug_print("mpu9250: spi_deinit is null.\n");                      /* spi_deinit is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_read == NULL)                                                   /* check spi_read */
    {
        handle->debug_print("mpu9250: spi_read is null.\n");                        /* spi_read is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->spi_write == NULL)                                                  /* check spi_write */
    {
        handle->debug_print("mpu9250: spi_write is null.\n");                       /* spi_write is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->delay_ms == NULL)                                                   /* check delay_ms */
    {
        handle->debug_print("mpu9250: delay_ms is null.\n");                        /* delay_ms is null */
       
        return 3;                                                                   /* return error */
    }
    if (handle->receive_callback == NULL)                                           /* check receive_callback */
    {
        handle->debug_print("mpu9250: receive_callback is null.\n");                /* receive_callback is null */
       
        return 3;                                                                   /* return error */
    }
    
    if (handle->iic_spi == MPU9250_INTERFACE_IIC)                                   /* if iic interface */
    {
        res = handle->iic_init();                                                   /* iic init */
        if (res != 0)                                                               /* check the result */
        {
            handle->debug_print("mpu9250: iic init failed.\n");                     /* iic init failed */
           
            return 1;                                                               /* return error */
        }
    }
    else                                                                            /* if spi interface */
    {
        res = handle->spi_init();                                                   /* spi init */
        if (res != 0)                                                               /* check the result */
        {
            handle->debug_print("mpu9250: spi init failed.\n");                     /* spi init failed */
           
            return 1;                                                               /* return error */
        }
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_WHO_AM_I, &prev, 1);                   /* read who am I */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: read who am i failed.\n");                    /* read who am I failed */
        (void)a_mpu9250_deinit(handle);                                             /* iic or spi deinit */
        
        return 5;                                                                   /* return error */
    }
    if (prev != 0x71)                                                               /* check the id */
    {
        handle->debug_print("mpu9250: id is invalid.\n");                           /* id is invalid */
        (void)a_mpu9250_deinit(handle);                                             /* iic or spi deinit */
        
        return 5;                                                                   /* return error */
    }
    
    prev = 1 << 7;                                                                  /* reset the device */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, &prev, 1);                /* write pwr mgmt 1 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: write pwr mgmt 1 failed.\n");                 /* write pwr mgmt 1 failed */
        (void)a_mpu9250_deinit(handle);                                             /* iic or spi deinit */
        
        return 4;                                                                   /* return error */
    }
    handle->delay_ms(10);                                                           /* delay 10 ms */
    timeout = 100;                                                                  /* set the timeout 1000 ms */
    while (timeout != 0)                                                            /* check the timeout */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, &prev, 1);             /* read pwr mgmt 1 */
        if (res != 0)                                                               /* check the result */
        {
            handle->debug_print("mpu9250: read pwr mgmt 1 failed.\n");              /* read pwr mgmt 1 failed */
            (void)a_mpu9250_deinit(handle);                                         /* iic or spi deinit */
            
            return 4;                                                               /* return error */
        }
        if ((prev & (1 << 7)) == 0)                                                 /* check the result */
        {
            handle->inited = 1;                                                     /* flag the inited bit */
            handle->dmp_inited = 0;                                                 /* flag closed */
            
            return 0;                                                               /* success return 0 */
        }
        handle->delay_ms(10);                                                       /* delay 10 ms */
        timeout--;                                                                  /* timeout-- */
    }
    
    handle->debug_print("mpu9250: reset failed.\n");                                /* reset failed */
   
    return 4;                                                                       /* return error */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 enter sleep mode failed
 * @note      none
 */
uint8_t mpu9250_deinit(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    prev = (1 << 6) | (1 << 3) | (7 << 0);                                      /* enter sleep mode */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, &prev, 1);            /* write pwr mgmt 1 */
    if (res != 0)                                                               /* check the result */
    {
        handle->debug_print("mpu9250: write pwr mgmt 1 failed.\n");             /* write pwr mgmt 1 failed */
       
        return 4;                                                               /* return error */
    }
    res = a_mpu9250_deinit(handle);                                             /* deinit */
    if (res != 0)                                                               /* check the result */
    {
        handle->debug_print("mpu9250: deinit failed.\n");                       /* deinit failed */
       
        return 1;                                                               /* return error */
    }
    handle->inited = 0;                                                         /* flag closed */
    handle->dmp_inited = 0;                                                     /* flag closed */
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief         read the data
 * @param[in]     *handle points to an mpu9250 handle structure
 * @param[out]    **accel_raw points to an accel raw data buffer
 * @param[out]    **accel_g points to a converted accel data buffer
 * @param[out]    **gyro_raw points to a gyro raw data buffer
 * @param[out]    **gyro_dps points to a converted gyro data buffer
 * @param[out]    **mag_raw points to a mag raw data buffer
 * @param[out]    **mag_ut points to a converted mag data buffer
 * @param[in,out] *len points to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 length is zero
 *                - 5 dmp is running
 *                - 6 fifo conf is error
 * @note          spi can't read mag data
 */
uint8_t mpu9250_read(mpu9250_handle_t *handle,
                     int16_t (*accel_raw)[3], float (*accel_g)[3],
                     int16_t (*gyro_raw)[3], float (*gyro_dps)[3],
                     int16_t (*mag_raw)[3], float (*mag_ut)[3],
                     uint16_t *len
                    )
{
    uint8_t res;
    uint8_t prev;
    uint8_t accel_conf;
    uint8_t gyro_conf;
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    if ((*len) == 0)                                                                               /* check length */
    {
        handle->debug_print("mpu9250: length is zero.\n");                                         /* length is zero */
                                                                                                  
        return 4;                                                                                  /* return error */
    }
    if (handle->dmp_inited != 0)                                                                   /* check dmp initialization */
    {
        handle->debug_print("mpu9250: dmp is running.\n");                                         /* dmp is running */
        
        return 5;                                                                                  /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);                      /* read config */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                                  /* read user ctrl failed */
       
        return 1;                                                                                  /* return error */
    }
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&accel_conf, 1);             /* read accel config */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read accel config failed.\n");                               /* read accel config failed */
       
        return 1;                                                                                  /* return error */
    }
    res = a_mpu9250_read(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&gyro_conf, 1);               /* read gyro config */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read gyro config failed.\n");                                /* read gyro config failed */
       
        return 1;                                                                                  /* return error */
    }
    accel_conf = (accel_conf >> 3) & 0x3;                                                          /* get the accel conf */
    gyro_conf = (gyro_conf >> 3) & 0x3;                                                            /* get the gyro conf */
    if ((prev & (1 << 6)) != 0)                                                                    /* if fifo mode */
    {
        uint8_t conf;
        uint8_t buf[2];
        uint16_t count;
        uint16_t i;
        
        res = a_mpu9250_read(handle, MPU9250_REG_FIFO_EN, (uint8_t *)&conf, 1);                    /* read fifo enable */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("mpu9250: read fifo enable failed.\n");                            /* read fifo enable failed */
           
            return 1;                                                                              /* return error */
        }
        if ((conf != 0x78) && (conf != 0x79))                                                      /* check the conf */
        {
            handle->debug_print("mpu9250: fifo conf is error.\n");                                 /* fifo conf is error */
                                                                                                      
            return 6;                                                                              /* return error */
        }
        
        res = a_mpu9250_read(handle, MPU9250_REG_FIFO_COUNTH, (uint8_t *)buf, 2);                  /* read fifo count */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("mpu9250: read fifo count failed.\n");                             /* read fifo count failed */
           
            return 1;                                                                              /* return error */
        }
        
        if (conf == 0x78)                                                                          /* only acc and gyro */
        {
            count = (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                  /* set count */
            count = (count < 1024) ? count : 1024;                                                 /* just the counter */
            count = (count < ((*len) * 12)) ? count : ((*len) * 12);                               /* just outer buffer size */
            count = (count / 12) * 12;                                                             /* 12 times */
            *len = count / 12;                                                                     /* set the output length */
            res = a_mpu9250_read(handle, MPU9250_REG_R_W, handle->buf, count);                     /* read data */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("mpu9250: read failed.\n");                                    /* read failed */
               
                return 1;                                                                          /* return error */
            }
            for (i = 0; i < (*len); i++)                                                           /* *len times */
            {
                accel_raw[i][0] = (int16_t)((uint16_t)handle->buf[i * 12 + 0] << 8) | 
                                             handle->buf[i * 12 + 1];                              /* set raw accel x */
                accel_raw[i][1] = (int16_t)((uint16_t)handle->buf[i * 12 + 2] << 8) | 
                                             handle->buf[i * 12 + 3];                              /* set raw accel y */
                accel_raw[i][2] = (int16_t)((uint16_t)handle->buf[i * 12 + 4] << 8) | 
                                             handle->buf[i * 12 + 5];                              /* set raw accel z */
                gyro_raw[i][0] = (int16_t)((uint16_t)handle->buf[i * 12 + 6] << 8) | 
                                            handle->buf[i * 12 + 7];                               /* set raw gyro x */
                gyro_raw[i][1] = (int16_t)((uint16_t)handle->buf[i * 12 + 8] << 8) | 
                                            handle->buf[i * 12 + 9];                               /* set raw gyro y */
                gyro_raw[i][2] = (int16_t)((uint16_t)handle->buf[i * 12 + 10] << 8) | 
                                            handle->buf[i * 12 + 11];                              /* set raw gyro z */
                mag_raw[i][0] = 0;                                                                 /* set 0 */
                mag_raw[i][1] = 0;                                                                 /* set 0 */
                mag_raw[i][2] = 0;                                                                 /* set 0 */
                
                if (accel_conf == 0)                                                               /* ±2g */
                {
                    accel_g[i][0] = (float)(accel_raw[i][0]) / 16384.0f;                           /* set accel x */
                    accel_g[i][1] = (float)(accel_raw[i][1]) / 16384.0f;                           /* set accel y */
                    accel_g[i][2] = (float)(accel_raw[i][2]) / 16384.0f;                           /* set accel z */
                }
                else if (accel_conf == 1)                                                          /* ±4g */
                {
                    accel_g[i][0] = (float)(accel_raw[i][0]) / 8192.0f;                            /* set accel x */
                    accel_g[i][1] = (float)(accel_raw[i][1]) / 8192.0f;                            /* set accel y */
                    accel_g[i][2] = (float)(accel_raw[i][2]) / 8192.0f;                            /* set accel z */
                }
                else if (accel_conf == 2)                                                          /* ±8g */
                {
                    accel_g[i][0] = (float)(accel_raw[i][0]) / 4096.0f;                            /* set accel x */
                    accel_g[i][1] = (float)(accel_raw[i][1]) / 4096.0f;                            /* set accel y */
                    accel_g[i][2] = (float)(accel_raw[i][2]) / 4096.0f;                            /* set accel z */
                }
                else                                                                               /* ±16g */
                {
                    accel_g[i][0] = (float)(accel_raw[i][0]) / 2048.0f;                            /* set accel x */
                    accel_g[i][1] = (float)(accel_raw[i][1]) / 2048.0f;                            /* set accel y */
                    accel_g[i][2] = (float)(accel_raw[i][2]) / 2048.0f;                            /* set accel z */
                }
                
                if (gyro_conf == 0)                                                                /* ±250dps */
                {
                    gyro_dps[i][0] = (float)(gyro_raw[i][0]) / 131.0f;                             /* set gyro x */
                    gyro_dps[i][1] = (float)(gyro_raw[i][1]) / 131.0f;                             /* set gyro y */
                    gyro_dps[i][2] = (float)(gyro_raw[i][2]) / 131.0f;                             /* set gyro z */
                }
                else if (gyro_conf == 1)                                                           /* ±500dps */
                {
                    gyro_dps[i][0] = (float)(gyro_raw[i][0]) / 65.5f;                              /* set gyro x */
                    gyro_dps[i][1] = (float)(gyro_raw[i][1]) / 65.5f;                              /* set gyro y */
                    gyro_dps[i][2] = (float)(gyro_raw[i][2]) / 65.5f;                              /* set gyro z */
                }
                else if (gyro_conf == 2)                                                           /* ±1000dps */
                {
                    gyro_dps[i][0] = (float)(gyro_raw[i][0]) / 32.8f;                              /* set gyro x */
                    gyro_dps[i][1] = (float)(gyro_raw[i][1]) / 32.8f;                              /* set gyro y */
                    gyro_dps[i][2] = (float)(gyro_raw[i][2]) / 32.8f;                              /* set gyro z */
                }
                else                                                                               /* ±2000dps */
                {
                    gyro_dps[i][0] = (float)(gyro_raw[i][0]) / 16.4f;                              /* set gyro x */
                    gyro_dps[i][1] = (float)(gyro_raw[i][1]) / 16.4f;                              /* set gyro y */
                    gyro_dps[i][2] = (float)(gyro_raw[i][2]) / 16.4f;                              /* set gyro z */
                }
                mag_ut[i][0] = 0.0f;                                                               /* set 0.0f */
                mag_ut[i][1] = 0.0f;                                                               /* set 0.0f */
                mag_ut[i][2] = 0.0f;                                                               /* set 0.0f */
            }
            
            return 0;                                                                              /* success return 0 */
        }
        else                                                                                       /* acc, gyro and mag */
        {
            float mag_k_x;
            float mag_k_y;
            float mag_k_z;
            
            count = (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                  /* set count */
            count = (count < 1024) ? count : 1024;                                                 /* just the counter */
            count = (count < ((*len) * 20)) ? count : ((*len) * 20);                               /* just outer buffer size */
            count = (count / 20) * 20;                                                             /* 20 times */
            *len = count / 20;                                                                     /* set the output length */
            res = a_mpu9250_read(handle, MPU9250_REG_R_W, handle->buf, count);                     /* read data */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("mpu9250: read failed.\n");                                    /* read failed */
               
                return 1;                                                                          /* return error */
            }
            
            mag_k_x = ((float)handle->mag_asa[0] - 128.0f) * 0.5f / 128.0f + 1.0f;                 /* get the x sensitivity adjustment */
            mag_k_y = ((float)handle->mag_asa[1] - 128.0f) * 0.5f / 128.0f + 1.0f;                 /* get the y sensitivity adjustment */
            mag_k_z = ((float)handle->mag_asa[2] - 128.0f) * 0.5f / 128.0f + 1.0f;                 /* get the z sensitivity adjustment */
            
            for (i = 0; i < (*len); i++)                                                           /* *len times */
            {
                accel_raw[i][0] = (int16_t)((uint16_t)handle->buf[i * 20 + 0] << 8) | 
                                             handle->buf[i * 20 + 1];                              /* set raw accel x */
                accel_raw[i][1] = (int16_t)((uint16_t)handle->buf[i * 20 + 2] << 8) | 
                                             handle->buf[i * 20 + 3];                              /* set raw accel y */
                accel_raw[i][2] = (int16_t)((uint16_t)handle->buf[i * 20 + 4] << 8) | 
                                             handle->buf[i * 20 + 5];                              /* set raw accel z */
                gyro_raw[i][0] = (int16_t)((uint16_t)handle->buf[i * 20 + 6] << 8) | 
                                            handle->buf[i * 20 + 7];                               /* set raw gyro x */
                gyro_raw[i][1] = (int16_t)((uint16_t)handle->buf[i * 20 + 8] << 8) | 
                                            handle->buf[i * 20 + 9];                               /* set raw gyro y */
                gyro_raw[i][2] = (int16_t)((uint16_t)handle->buf[i * 20 + 10] << 8) | 
                                            handle->buf[i * 20 + 11];                              /* set raw gyro z */
                mag_raw[i][0] = (int16_t)((uint16_t)handle->buf[i * 20 + 14] << 8) | 
                                           handle->buf[i * 20 + 13];                               /* set raw mag x */
                mag_raw[i][1] = (int16_t)((uint16_t)handle->buf[i * 20 + 16] << 8) | 
                                           handle->buf[i * 20 + 15];                               /* set raw mag y */
                mag_raw[i][2] = (int16_t)((uint16_t)handle->buf[i * 20 + 18] << 8) | 
                                           handle->buf[i * 20 + 17];                               /* set raw mag z */
                
                if (accel_conf == 0)                                                               /* ±2g */
                {
                    accel_g[i][0] = (float)(accel_raw[i][0]) / 16384.0f;                           /* set accel x */
                    accel_g[i][1] = (float)(accel_raw[i][1]) / 16384.0f;                           /* set accel y */
                    accel_g[i][2] = (float)(accel_raw[i][2]) / 16384.0f;                           /* set accel z */
                }
                else if (accel_conf == 1)                                                          /* ±4g */
                {
                    accel_g[i][0] = (float)(accel_raw[i][0]) / 8192.0f;                            /* set accel x */
                    accel_g[i][1] = (float)(accel_raw[i][1]) / 8192.0f;                            /* set accel y */
                    accel_g[i][2] = (float)(accel_raw[i][2]) / 8192.0f;                            /* set accel z */
                }
                else if (accel_conf == 2)                                                          /* ±8g */
                {
                    accel_g[i][0] = (float)(accel_raw[i][0]) / 4096.0f;                            /* set accel x */
                    accel_g[i][1] = (float)(accel_raw[i][1]) / 4096.0f;                            /* set accel y */
                    accel_g[i][2] = (float)(accel_raw[i][2]) / 4096.0f;                            /* set accel z */
                }
                else                                                                               /* ±16g */
                {
                    accel_g[i][0] = (float)(accel_raw[i][0]) / 2048.0f;                            /* set accel x */
                    accel_g[i][1] = (float)(accel_raw[i][1]) / 2048.0f;                            /* set accel y */
                    accel_g[i][2] = (float)(accel_raw[i][2]) / 2048.0f;                            /* set accel z */
                }
                
                if (gyro_conf == 0)                                                                /* ±250dps */
                {
                    gyro_dps[i][0] = (float)(gyro_raw[i][0]) / 131.0f;                             /* set gyro x */
                    gyro_dps[i][1] = (float)(gyro_raw[i][1]) / 131.0f;                             /* set gyro y */
                    gyro_dps[i][2] = (float)(gyro_raw[i][2]) / 131.0f;                             /* set gyro z */
                }
                else if (gyro_conf == 1)                                                           /* ±500dps */
                {
                    gyro_dps[i][0] = (float)(gyro_raw[i][0]) / 65.5f;                              /* set gyro x */
                    gyro_dps[i][1] = (float)(gyro_raw[i][1]) / 65.5f;                              /* set gyro y */
                    gyro_dps[i][2] = (float)(gyro_raw[i][2]) / 65.5f;                              /* set gyro z */
                }
                else if (gyro_conf == 2)                                                           /* ±1000dps */
                {
                    gyro_dps[i][0] = (float)(gyro_raw[i][0]) / 32.8f;                              /* set gyro x */
                    gyro_dps[i][1] = (float)(gyro_raw[i][1]) / 32.8f;                              /* set gyro y */
                    gyro_dps[i][2] = (float)(gyro_raw[i][2]) / 32.8f;                              /* set gyro z */
                }
                else                                                                               /* ±2000dps */
                {
                    gyro_dps[i][0] = (float)(gyro_raw[i][0]) / 16.4f;                              /* set gyro x */
                    gyro_dps[i][1] = (float)(gyro_raw[i][1]) / 16.4f;                              /* set gyro y */
                    gyro_dps[i][2] = (float)(gyro_raw[i][2]) / 16.4f;                              /* set gyro z */
                }
                if ((handle->buf[i * 20 + 19] & (1 << 4)) != 0)                                    /* if 16 bits */
                {
                    mag_ut[i][0] = (float)mag_raw[i][0] * mag_k_x * 0.15f;                         /* set the mag x */
                    mag_ut[i][1] = (float)mag_raw[i][1] * mag_k_y * 0.15f;                         /* set the mag y */
                    mag_ut[i][2] = (float)mag_raw[i][2] * mag_k_z * 0.15f;                         /* set the mag z */
                }
                else                                                                               /* if 14 bits */
                {
                    mag_ut[i][0] = (float)mag_raw[i][0] * mag_k_x * 0.6f;                          /* set the mag x */
                    mag_ut[i][1] = (float)mag_raw[i][1] * mag_k_y * 0.6f;                          /* set the mag y */
                    mag_ut[i][2] = (float)mag_raw[i][2] * mag_k_z * 0.6f;                          /* set the mag z */
                }
            }
            
            return 0;                                                                              /* success return 0 */
        }
    }
    else                                                                                           /* if normal mode */
    {
        *len = 1;                                                                                  /* set 1 */
        res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_XOUT_H, handle->buf, 14);                   /* read data */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("mpu9250: read failed.\n");                                        /* read failed */
           
            return 1;                                                                              /* return error */
        }
        accel_raw[0][0] = (int16_t)((uint16_t)handle->buf[0] << 8) | handle->buf[1];               /* set raw accel x */
        accel_raw[0][1] = (int16_t)((uint16_t)handle->buf[2] << 8) | handle->buf[3];               /* set raw accel y */
        accel_raw[0][2] = (int16_t)((uint16_t)handle->buf[4] << 8) | handle->buf[5];               /* set raw accel z */
        gyro_raw[0][0] = (int16_t)((uint16_t)handle->buf[8] << 8) | handle->buf[9];                /* set raw gyro x */
        gyro_raw[0][1] = (int16_t)((uint16_t)handle->buf[10] << 8) | handle->buf[11];              /* set raw gyro y */
        gyro_raw[0][2] = (int16_t)((uint16_t)handle->buf[12] << 8) | handle->buf[13];              /* set raw gyro z */
        
        if (accel_conf == 0)                                                                       /* ±2g */
        {
            accel_g[0][0] = (float)(accel_raw[0][0]) / 16384.0f;                                   /* set accel x */
            accel_g[0][1] = (float)(accel_raw[0][1]) / 16384.0f;                                   /* set accel y */
            accel_g[0][2] = (float)(accel_raw[0][2]) / 16384.0f;                                   /* set accel z */
        }
        else if (accel_conf == 1)                                                                  /* ±4g */
        {
            accel_g[0][0] = (float)(accel_raw[0][0]) / 8192.0f;                                    /* set accel x */
            accel_g[0][1] = (float)(accel_raw[0][1]) / 8192.0f;                                    /* set accel y */
            accel_g[0][2] = (float)(accel_raw[0][2]) / 8192.0f;                                    /* set accel z */
        }
        else if (accel_conf == 2)                                                                  /* ±8g */
        {
            accel_g[0][0] = (float)(accel_raw[0][0]) / 4096.0f;                                    /* set accel x */
            accel_g[0][1] = (float)(accel_raw[0][1]) / 4096.0f;                                    /* set accel y */
            accel_g[0][2] = (float)(accel_raw[0][2]) / 4096.0f;                                    /* set accel z */
        }
        else                                                                                       /* ±16g */
        {
            accel_g[0][0] = (float)(accel_raw[0][0]) / 2048.0f;                                    /* set accel x */
            accel_g[0][1] = (float)(accel_raw[0][1]) / 2048.0f;                                    /* set accel y */
            accel_g[0][2] = (float)(accel_raw[0][2]) / 2048.0f;                                    /* set accel z */
        }
        
        if (gyro_conf == 0)                                                                        /* ±250dps */
        {
            gyro_dps[0][0] = (float)(gyro_raw[0][0]) / 131.0f;                                     /* set gyro x */
            gyro_dps[0][1] = (float)(gyro_raw[0][1]) / 131.0f;                                     /* set gyro y */
            gyro_dps[0][2] = (float)(gyro_raw[0][2]) / 131.0f;                                     /* set gyro z */
        }
        else if (gyro_conf == 1)                                                                   /* ±500dps */
        {
            gyro_dps[0][0] = (float)(gyro_raw[0][0]) / 65.5f;                                      /* set gyro x */
            gyro_dps[0][1] = (float)(gyro_raw[0][1]) / 65.5f;                                      /* set gyro y */
            gyro_dps[0][2] = (float)(gyro_raw[0][2]) / 65.5f;                                      /* set gyro z */
        }
        else if (gyro_conf == 2)                                                                   /* ±1000dps */
        {
            gyro_dps[0][0] = (float)(gyro_raw[0][0]) / 32.8f;                                      /* set gyro x */
            gyro_dps[0][1] = (float)(gyro_raw[0][1]) / 32.8f;                                      /* set gyro y */
            gyro_dps[0][2] = (float)(gyro_raw[0][2]) / 32.8f;                                      /* set gyro z */
        }
        else                                                                                       /* ±2000dps */
        {
            gyro_dps[0][0] = (float)(gyro_raw[0][0]) / 16.4f;                                      /* set gyro x */
            gyro_dps[0][1] = (float)(gyro_raw[0][1]) / 16.4f;                                      /* set gyro y */
            gyro_dps[0][2] = (float)(gyro_raw[0][2]) / 16.4f;                                      /* set gyro z */
        }
        if (handle->mag_inited == 1)                                                               /* if mag is inited */
        {
            float mag_k_x;
            float mag_k_y;
            float mag_k_z;
            
            res = a_mpu9250_mag_read(handle, AK8963_REG_ST1, handle->buf, 8);                      /* read the mag data */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("mpu9250: mag read failed.\n");                                /* mag read failed */
               
                return 1;                                                                          /* return error */
            }
            mag_raw[0][0] = (int16_t)((uint16_t)handle->buf[2] << 8) | handle->buf[1];             /* set raw mag x */
            mag_raw[0][1] = (int16_t)((uint16_t)handle->buf[4] << 8) | handle->buf[3];             /* set raw mag y */
            mag_raw[0][2] = (int16_t)((uint16_t)handle->buf[6] << 8) | handle->buf[5];             /* set raw mag z */

            mag_k_x = ((float)handle->mag_asa[0] - 128.0f) * 0.5f / 128.0f + 1.0f;                 /* get the x sensitivity adjustment */
            mag_k_y = ((float)handle->mag_asa[1] - 128.0f) * 0.5f / 128.0f + 1.0f;                 /* get the y sensitivity adjustment */
            mag_k_z = ((float)handle->mag_asa[2] - 128.0f) * 0.5f / 128.0f + 1.0f;                 /* get the z sensitivity adjustment */
            if ((handle->buf[7] & (1 << 4)) != 0)                                                  /* if 16 bits */
            {
                mag_ut[0][0] = (float)mag_raw[0][0] * mag_k_x * 0.15f;                             /* set the mag x */
                mag_ut[0][1] = (float)mag_raw[0][1] * mag_k_y * 0.15f;                             /* set the mag y */
                mag_ut[0][2] = (float)mag_raw[0][2] * mag_k_z * 0.15f;                             /* set the mag z */
            }
            else                                                                                   /* if 14 bits */
            {
                mag_ut[0][0] = (float)mag_raw[0][0] * mag_k_x * 0.6f;                              /* set the mag x */
                mag_ut[0][1] = (float)mag_raw[0][1] * mag_k_y * 0.6f;                              /* set the mag y */
                mag_ut[0][2] = (float)mag_raw[0][2] * mag_k_z * 0.6f;                              /* set the mag z */
            }
        }
        else
        {
            mag_raw[0][0] = 0;                                                                     /* set raw mag x 0 */
            mag_raw[0][1] = 0;                                                                     /* set raw mag y 0 */
            mag_raw[0][2] = 0;                                                                     /* set raw mag z 0 */
            mag_ut[0][0] = 0.0f;                                                                   /* set mag x 0.0f */
            mag_ut[0][1] = 0.0f;                                                                   /* set mag y 0.0f */
            mag_ut[0][2] = 0.0f;                                                                   /* set mag z 0.0f */
        }
        
        return 0;                                                                                  /* success return 0 */
    }
}

/**
 * @brief      read the temperature
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *raw points to raw data buffer
 * @param[out] *degrees points to a converted degrees data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_read_temperature(mpu9250_handle_t *handle, int16_t (*raw), float *degrees)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_TEMP_OUT_H, buf, 2);            /* read data */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("mpu9250: read failed.\n");                      /* read failed */
       
        return 1;                                                            /* return error */
    }
    *raw = (int16_t)((uint16_t)buf[0] << 8) | buf[1];                        /* get the raw */
    *degrees = (float)(*raw) / 321.0f + 21.0f;                               /* convert the degrees */
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief     irq handler
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_irq_handler(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_STATUS, (uint8_t *)&prev, 1);            /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read int status failed.\n");                        /* read int status failed */
       
        return 1;                                                                         /* return error */
    }
    if ((prev & (1 << MPU9250_INTERRUPT_MOTION)) != 0)                                    /* if motion */
    {
        if (handle->receive_callback != NULL)                                             /* if receive callback */
        {
            handle->receive_callback(MPU9250_INTERRUPT_MOTION);                           /* run callback */
        }
    }
    if ((prev & (1 << MPU9250_INTERRUPT_FIFO_OVERFLOW)) != 0)                             /* if fifo overflow */
    {
        if (handle->receive_callback != NULL)                                             /* if receive callback */
        {
            handle->receive_callback(MPU9250_INTERRUPT_FIFO_OVERFLOW);                    /* run callback */
        }
        (void)a_mpu9250_reset_fifo(handle);                                               /* reset the fifo */
    }
    if ((prev & (1 << MPU9250_INTERRUPT_FSYNC_INT)) != 0)                                 /* if fsync int */
    {
        if (handle->receive_callback != NULL)                                             /* if receive callback */
        {
            handle->receive_callback(MPU9250_INTERRUPT_FSYNC_INT);                        /* run callback */
        }
    }
    if ((prev & (1 << MPU9250_INTERRUPT_DMP)) != 0)                                       /* if dmp */
    {
        if (handle->receive_callback != NULL)                                             /* if receive callback */
        {
            handle->receive_callback(MPU9250_INTERRUPT_DMP);                              /* run callback */
        }
    }
    if ((prev & (1 << MPU9250_INTERRUPT_DATA_READY)) != 0)                                /* if data ready */
    {
        if (handle->receive_callback != NULL)                                             /* if receive callback */
        {
            handle->receive_callback(MPU9250_INTERRUPT_DATA_READY);                       /* run callback */
        }
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     initialize the magnetometer of mpu9250
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 mag init failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 reset failed
 *            - 5 mag id is invalid
 *            - 6 spi don't support
 * @note      none
 */
uint8_t mpu9250_mag_init(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    uint32_t timeout;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (handle->iic_spi == MPU9250_INTERFACE_IIC)                                           /* if iic interface */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);           /* read config */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read user ctrl failed.\n");                       /* read user ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 5);                                                                  /* disable iic master */
        res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);          /* write config */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write user ctrl failed.\n");                      /* write user ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read config */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read int pin cfg failed.\n");                     /* read int pin cfg failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 1);                                                                  /* clear config */
        prev |= 1 << 1;                                                                     /* enable bypass */
        res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);        /* write config */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write int pin cfg failed.\n");                    /* write int pin cfg failed */
           
            return 1;                                                                       /* return error */
        }
        
        res = a_mpu9250_mag_read(handle, AK8963_REG_WIA, (uint8_t *)&prev, 1);              /* read who am I */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: mag read who am i failed.\n");                    /* mag read who am I failed */
           
            return 1;                                                                       /* return error */
        }
        if (prev != 0x48)                                                                   /* check the id */
        {
            handle->debug_print("mpu9250: mag id is invalid.\n");                           /* mag id is invalid */
           
            return 5;                                                                       /* return error */
        }
        
        prev = 0x01;                                                                        /* set the reset command */
        res = a_mpu9250_mag_write(handle, AK8963_REG_CNTL2, &prev);                         /* reset the magnetometer */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: mag write cntl2 failed.\n");                      /* mag write cntl2 failed */
           
            return 1;                                                                       /* return error */
        }
        timeout = 100;                                                                      /* set timeout 1000ms */
        while (timeout != 0)                                                                /* check the timeout */
        {
            res = a_mpu9250_mag_read(handle, AK8963_REG_CNTL2, (uint8_t *)&prev, 1);        /* read cntl2 */
            if (res != 0)                                                                   /* check result */
            {
                handle->debug_print("mpu9250: mag read cntl2 failed.\n");                   /* mag read cntl2 failed */
               
                return 1;                                                                   /* return error */
            }
            if ((prev & 0x01) == 0x00)                                                      /* check finished */
            {
                break;                                                                      /* break */
            }
            handle->delay_ms(10);                                                           /* delay 10ms */
            timeout--;                                                                      /* timeout-- */
        }
        if (timeout == 0)                                                                   /* check the timeout */
        {
            handle->debug_print("mpu9250: reset failed.\n");                                /* reset failed */
           
            return 4;                                                                       /* return error */
        }
        
        prev = 0x00;                                                                        /* power down */
        res = a_mpu9250_mag_write(handle, AK8963_REG_CNTL1, &prev);                         /* set the settings */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: mag write cntl1 failed.\n");                      /* mag write cntl1 failed */
           
            return 1;                                                                       /* return error */
        }
        handle->delay_ms(10);                                                               /* delay 10ms */
        prev = 0x0F;                                                                        /* fuse rom access mode */
        res = a_mpu9250_mag_write(handle, AK8963_REG_CNTL1, &prev);                         /* set the settings */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: mag write cntl1 failed.\n");                      /* mag write cntl1 failed */
           
            return 1;                                                                       /* return error */
        }
        res = a_mpu9250_mag_read(handle, AK8963_REG_ASAX, handle->mag_asa, 3);              /* read the asa */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: mag read asa failed.\n");                         /* mag read asa failed */
           
            return 1;                                                                       /* return error */
        }
        prev = 0x00;                                                                        /* power down */
        res = a_mpu9250_mag_write(handle, AK8963_REG_CNTL1, &prev);                         /* set the settings */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: mag write cntl1 failed.\n");                      /* mag write cntl1 failed */
           
            return 1;                                                                       /* return error */
        }
        handle->delay_ms(10);                                                               /* delay 10ms */
        
        handle->mag_inited = 1;                                                             /* flag inited */
        
        return 0;                                                                           /* success return 0 */
    }
    else
    {
        handle->debug_print("mpu9250: spi don't support.\n");                               /* spi don't support */
       
        return 6;                                                                           /* return error */
    }
}

/**
 * @brief      magnetometer deinit
 * @param[in]  *handle points to an mpu9250 handle structure
 * @return     status code
 *             - 0 success
 *             - 1 mag deinit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_deinit(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->mag_inited != 1)                                                    /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                       /* mag is not inited */
       
        return 4;                                                                   /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                   /* read user ctrl failed */
       
        return 1;                                                                   /* return error */
    }
    prev &= ~(1 << 5);                                                              /* disable iic master */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mpu9250: write user ctrl failed.\n");                  /* write user ctrl failed */
       
        return 1;                                                                   /* return error */
    }
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);     /* read config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mpu9250: read int pin cfg failed.\n");                 /* read int pin cfg failed */
       
        return 1;                                                                   /* return error */
    }
    prev &= ~(1 << 1);                                                              /* clear config */
    prev |= 1 << 1;                                                                 /* enable bypass */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);    /* write config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mpu9250: write int pin cfg failed.\n");                /* write int pin cfg failed */
       
        return 1;                                                                   /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_WIA, (uint8_t *)&prev, 1);          /* read who am I */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mpu9250: mag read who am i failed.\n");                /* mag read who am I failed */
       
        return 1;                                                                   /* return error */
    }
    if (prev != 0x48)                                                               /* check the id */
    {
        handle->debug_print("mpu9250: mag id is invalid.\n");                       /* mag id is invalid */
       
        return 5;                                                                   /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_CNTL1, (uint8_t *)&prev, 1);        /* read cntl1 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag read cntl1 failed.\n");                   /* mag read cntl1 failed */
       
        return 1;                                                                   /* return error */
    }
    prev &= ~(0xF << 0);                                                            /* clear config */
    prev |= 0x00 << 0;                                                              /* set the power mode */
    res = a_mpu9250_mag_write(handle, AK8963_REG_CNTL1, (uint8_t *)&prev);          /* write cntl1 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag write cntl1 failed.\n");                  /* mag write cntl1 failed */
       
        return 1;                                                                   /* return error */
    }
    
    handle->mag_inited = 0;                                                         /* flag closed */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      mag read the data
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *mag_raw points to a mag raw data buffer
 * @param[out] *mag_ut points to a converted mag data buffer
 * @return     status code
 *             - 0 success
 *             - 1 mag read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_read(mpu9250_handle_t *handle, int16_t mag_raw[3], float mag_ut[3])
{
    uint8_t res;
    uint8_t buf[8];
    float mag_k_x;
    float mag_k_y;
    float mag_k_z;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    if (handle->mag_inited != 1)                                                  /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                     /* mag is not inited */
       
        return 4;                                                                 /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_ST1, buf, 8);                     /* read the mag data */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("mpu9250: mag read failed.\n");                       /* mag read failed */
       
        return 1;                                                                 /* return error */
    }
    mag_raw[0] = (int16_t)((uint16_t)buf[2] << 8) | buf[1];                       /* set raw mag x */
    mag_raw[1] = (int16_t)((uint16_t)buf[4] << 8) | buf[3];                       /* set raw mag y */
    mag_raw[2] = (int16_t)((uint16_t)buf[6] << 8) | buf[5];                       /* set raw mag z */

    mag_k_x = ((float)handle->mag_asa[0] - 128.0f) * 0.5f / 128.0f + 1.0f;        /* get the x sensitivity adjustment */
    mag_k_y = ((float)handle->mag_asa[1] - 128.0f) * 0.5f / 128.0f + 1.0f;        /* get the y sensitivity adjustment */
    mag_k_z = ((float)handle->mag_asa[2] - 128.0f) * 0.5f / 128.0f + 1.0f;        /* get the z sensitivity adjustment */
    if ((buf[7] & (1 << 4)) != 0)                                                 /* if 16 bits */
    {
        mag_ut[0] = (float)mag_raw[0] * mag_k_x * 0.15f;                          /* set the mag x */
        mag_ut[1] = (float)mag_raw[1] * mag_k_y * 0.15f;                          /* set the mag y */
        mag_ut[2] = (float)mag_raw[2] * mag_k_z * 0.15f;                          /* set the mag z */
    }
    else                                                                          /* if 14 bits */
    {
        mag_ut[0] = (float)mag_raw[0] * mag_k_x * 0.6f;                           /* set the mag x */
        mag_ut[1] = (float)mag_raw[1] * mag_k_y * 0.6f;                           /* set the mag y */
        mag_ut[2] = (float)mag_raw[2] * mag_k_z * 0.6f;                           /* set the mag z */
    }
    
    return 0;                                                                     /* success return 0 */
}

/**
 * @brief      magnetometer get the information
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *info points to an info buffer
 * @return     status code
 *             - 0 success
 *             - 1 mag get info failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_get_info(mpu9250_handle_t *handle, uint8_t *info)
{
    uint8_t res;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    if (handle->mag_inited != 1)                                                  /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                     /* mag is not inited */
       
        return 4;                                                                 /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_INFO, (uint8_t *)info, 1);        /* read info */
    if (res != 0)                                                                 /* check the result */
    {
        handle->debug_print("mpu9250: mag read info failed.\n");                  /* mag read info failed */
       
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

/**
 * @brief      magnetometer get the status1
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *status points to a status1 buffer
 * @return     status code
 *             - 0 success
 *             - 1 mag get status1 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_get_status1(mpu9250_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (handle->mag_inited != 1)                                                   /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                      /* mag is not inited */
       
        return 4;                                                                  /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_ST1, (uint8_t *)status, 1);        /* read status1 */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("mpu9250: mag read status1 failed.\n");                /* mag read status1 failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      magnetometer get the status2
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *status points to a status2 buffer
 * @return     status code
 *             - 0 success
 *             - 1 mag get status2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_get_status2(mpu9250_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (handle->mag_inited != 1)                                                   /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                      /* mag is not inited */
       
        return 4;                                                                  /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_ST2, (uint8_t *)status, 1);        /* read status2 */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("mpu9250: mag read status2 failed.\n");                /* mag read status2 failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     magnetometer set the mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] mode is the magnetometer mode
 * @return    status code
 *            - 0 success
 *            - 1 mag set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 mag is not inited
 * @note      none
 */
uint8_t mpu9250_mag_set_mode(mpu9250_handle_t *handle, mpu9250_magnetometer_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->mag_inited != 1)                                                    /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                       /* mag is not inited */
       
        return 4;                                                                   /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_CNTL1, (uint8_t *)&prev, 1);        /* read cntl1 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag read cntl1 failed.\n");                   /* mag read cntl1 failed */
       
        return 1;                                                                   /* return error */
    }
    prev &= ~(0xF << 0);                                                            /* clear config */
    prev |= mode << 0;                                                              /* set the mode */
    res = a_mpu9250_mag_write(handle, AK8963_REG_CNTL1, (uint8_t *)&prev);          /* write cntl1 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag write cntl1 failed.\n");                  /* mag write cntl1 failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      magnetometer get the mode
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *mode points to a magnetometer mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 mag get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_get_mode(mpu9250_handle_t *handle, mpu9250_magnetometer_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->mag_inited != 1)                                                    /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                       /* mag is not inited */
       
        return 4;                                                                   /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_CNTL1, (uint8_t *)&prev, 1);        /* read cntl1 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag read cntl1 failed.\n");                   /* mag read cntl1 failed */
       
        return 1;                                                                   /* return error */
    }
    *mode = (mpu9250_magnetometer_mode_t)(prev & 0xF);                              /* get the mode */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     magnetometer set the bits
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] bits is the magnetometer bits
 * @return    status code
 *            - 0 success
 *            - 1 mag set bits failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 mag is not inited
 * @note      none
 */
uint8_t mpu9250_mag_set_bits(mpu9250_handle_t *handle, mpu9250_magnetometer_bits_t bits)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->mag_inited != 1)                                                    /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                       /* mag is not inited */
       
        return 4;                                                                   /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_CNTL1, (uint8_t *)&prev, 1);        /* read cntl1 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag read cntl1 failed.\n");                   /* mag read cntl1 failed */
       
        return 1;                                                                   /* return error */
    }
    prev &= ~(1 << 4);                                                              /* clear config */
    prev |= bits << 4;                                                              /* set the mode */
    res = a_mpu9250_mag_write(handle, AK8963_REG_CNTL1, (uint8_t *)&prev);          /* write cntl1 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag write cntl1 failed.\n");                  /* mag write cntl1 failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      magnetometer get the bits
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *bits points to a magnetometer bits buffer
 * @return     status code
 *             - 0 success
 *             - 1 mag get bits failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_get_bits(mpu9250_handle_t *handle, mpu9250_magnetometer_bits_t *bits)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->mag_inited != 1)                                                    /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                       /* mag is not inited */
       
        return 4;                                                                   /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_CNTL1, (uint8_t *)&prev, 1);        /* read cntl1 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag read cntl1 failed.\n");                   /* mag read cntl1 failed */
       
        return 1;                                                                   /* return error */
    }
    *bits = (mpu9250_magnetometer_bits_t)((prev >> 4) & 0x1);                       /* get the bits */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     magnetometer enable or disable reset
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 mag set reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 mag is not inited
 * @note      none
 */
uint8_t mpu9250_mag_set_reset(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->mag_inited != 1)                                                    /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                       /* mag is not inited */
       
        return 4;                                                                   /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_CNTL2, (uint8_t *)&prev, 1);        /* read cntl2 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag read cntl2 failed.\n");                   /* mag read cntl2 failed */
       
        return 1;                                                                   /* return error */
    }
    prev &= ~(1 << 0);                                                              /* clear config */
    prev |= enable << 0;                                                            /* set the bool */
    res = a_mpu9250_mag_write(handle, AK8963_REG_CNTL2, (uint8_t *)&prev);          /* write cntl2 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag write cntl2 failed.\n");                  /* mag write cntl2 failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      magnetometer get the reset status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 mag get reset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_get_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->mag_inited != 1)                                                    /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                       /* mag is not inited */
       
        return 4;                                                                   /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_CNTL2, (uint8_t *)&prev, 1);        /* read cntl2 */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("mpu9250: mag read cntl2 failed.\n");                   /* mag read cntl2 failed */
       
        return 1;                                                                   /* return error */
    }
    *enable = (mpu9250_bool_t)(prev & 0x1);                                         /* get the bool */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     magnetometer enable or disable self test
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 mag set self test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 mag is not inited
 * @note      none
 */
uint8_t mpu9250_mag_set_self_test(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (handle->mag_inited != 1)                                                   /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                      /* mag is not inited */
       
        return 4;                                                                  /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_ASTC, (uint8_t *)&prev, 1);        /* read astc */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("mpu9250: mag read astc failed.\n");                   /* mag read astc failed */
       
        return 1;                                                                  /* return error */
    }
    prev &= ~(1 << 6);                                                             /* clear config */
    prev |= enable << 6;                                                           /* set the bool */
    res = a_mpu9250_mag_write(handle, AK8963_REG_ASTC, (uint8_t *)&prev);          /* write astc */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("mpu9250: mag write astc failed.\n");                  /* mag write astc failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      magnetometer get the self test status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 mag get self test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_get_self_test(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (handle->mag_inited != 1)                                                   /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                      /* mag is not inited */
       
        return 4;                                                                  /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_ASTC, (uint8_t *)&prev, 1);        /* read astc */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("mpu9250: mag read astc failed.\n");                   /* mag read astc failed */
       
        return 1;                                                                  /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 6) & 0x1);                                 /* get the bool */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     magnetometer disable the iic
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 mag iic disable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 mag is not inited
 * @note      none
 */
uint8_t mpu9250_mag_iic_disable(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (handle->mag_inited != 1)                                                   /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                      /* mag is not inited */
       
        return 4;                                                                  /* return error */
    }
    
    prev = 0x1B;                                                                   /* set the command */
    res = a_mpu9250_mag_write(handle, AK8963_REG_I2CDIS, (uint8_t *)&prev);        /* write i2c-dis */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("mpu9250: mag write i2cdis failed.\n");                /* mag write i2c-dis failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      magnetometer get the asa data
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *asa points to an asa buffer
 * @return     status code
 *             - 0 success
 *             - 1 mag get asa failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       none
 */
uint8_t mpu9250_mag_get_asa(mpu9250_handle_t *handle, uint8_t asa[3])
{
    uint8_t res;
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    if (handle->mag_inited != 1)                                                 /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                    /* mag is not inited */
       
        return 4;                                                                /* return error */
    }
    
    res = a_mpu9250_mag_read(handle, AK8963_REG_ASAX, (uint8_t *)asa, 3);        /* read asa */
    if (res != 0)                                                                /* check the result */
    {
        handle->debug_print("mpu9250: mag read asa failed.\n");                  /* mag read asa failed */
       
        return 1;                                                                /* return error */
    }
    
    return 0;                                                                    /* success return 0 */
}

/**
 * @brief      magnetometer set into fifo mode
 * @param[in]  *handle points to an mpu9250 handle structure
 * @return     status code
 *             - 0 success
 *             - 1 mag set fifo mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 mag is not inited
 * @note       fifo mode will use slave 0
 */
uint8_t mpu9250_mag_set_fifo_mode(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->mag_inited != 1)                                                    /* check mag initialization */
    {
        handle->debug_print("mpu9250: mag is not inited.\n");                       /* mag is not inited */
       
        return 4;                                                                   /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);           /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                       /* read user ctrl failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 5);                                                                  /* disable iic master */
    prev |= (1 << 5);                                                                   /* enable iic master */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);          /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write user ctrl failed.\n");                      /* write user ctrl failed */
       
        return 1;                                                                       /* return error */
    }
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read int pin cfg failed.\n");                     /* read int pin cfg failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 1);                                                                  /* disable bypass */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write int pin cfg failed.\n");                    /* write int pin cfg failed */
       
        return 1;                                                                       /* return error */
    }
    
    prev = 0x80 | AK8963_IIC_ADDRESS;                                                   /* set the read mode and iic address */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_ADDR, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write iic slv0 addr failed.\n");                  /* write iic slv0 addr failed */
       
        return 1;                                                                       /* return error */
    }
    prev = AK8963_REG_ST1;                                                              /* set the start register address */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_REG, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write iic slv0 reg failed.\n");                   /* write iic slv0 reg failed */
       
        return 1;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_FIFO_EN, (uint8_t *)&prev, 1);             /* read fifo enable */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read fifo enable failed.\n");                     /* read fifo enable failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 0);                                                                  /* clear the settings */
    prev |= 1 << 0;                                                                     /* enable slv0 fifo */
    res = a_mpu9250_write(handle, MPU9250_REG_FIFO_EN, (uint8_t *)&prev, 1);            /* write fifo enable ctrl */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write fifo enable failed.\n");                    /* write fifo enable failed */
       
        return 1;                                                                       /* return error */
    }
    
    prev = 0x88;                                                                        /* start and read 8 bytes */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write iic slv0 ctrl failed.\n");                  /* write iic slv0 ctrl failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable fifo
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_fifo(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                     /* read user ctrl failed */
       
        return 1;                                                                     /* return error */
    }
    prev &= ~(1 << 6);                                                                /* clear config */
    prev |= enable << 6;                                                              /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("mpu9250: write user ctrl failed.\n");                    /* write user ctrl failed */
       
        return 1;                                                                     /* return error */
    }
    
    return 0;                                                                         /* success return 0 */
}

/**
 * @brief      get the fifo status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_fifo(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 6) & 0x01);                                  /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     force reset the fifo
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 force fifo reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_force_fifo_reset(mpu9250_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_mpu9250_reset_fifo(handle);                                    /* reset the fifo */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("mpu9250: force reset fifo failed.\n");        /* force reset fifo failed */
       
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief     enable or disable the iic master mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic master failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic_master(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 5);                                                               /* clear config */
    prev |= enable << 5;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write user ctrl failed.\n");                   /* write user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the iic master status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic master failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_master(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 5) & 0x01);                                  /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     enable or disable the iic slave mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set disable iic slave failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_disable_iic_slave(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 4);                                                               /* clear config */
    prev |= enable << 4;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write user ctrl failed.\n");                   /* write user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the iic slave status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get disable iic slave failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_disable_iic_slave(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 4) & 0x01);                                  /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     reset the fifo
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 fifo reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_fifo_reset(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 2);                                                               /* clear config */
    prev |= 1 << 2;                                                                  /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write user ctrl failed.\n");                   /* write user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the fifo reset status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo reset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_fifo_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 2) & 0x01);                                  /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     reset the iic master controller
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic master reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_iic_master_reset(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 1);                                                               /* clear config */
    prev |= 1 << 1;                                                                  /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write user ctrl failed.\n");                   /* write user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the iic master reset status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic master reset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_master_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 1) & 0x01);                                  /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     reset all sensors
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 sensor reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_sensor_reset(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 0);                                                               /* clear config */
    prev |= 1 << 0;                                                                  /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write user ctrl failed.\n");                   /* write user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the sensor reset status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sensor reset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_sensor_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_USER_CTRL, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read user ctrl failed.\n");                    /* read user ctrl failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 0) & 0x01);                                  /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     reset the chip
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 device reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_device_reset(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 7);                                                               /* clear config */
    prev |= 1 << 7;                                                                  /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write power management 1 failed.\n");          /* write power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the device reset status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get device reset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_device_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 7) & 0x01);                                  /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     set the chip clock source
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] clock_source is chip main clock source
 * @return    status code
 *            - 0 success
 *            - 1 set clock source failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_clock_source(mpu9250_handle_t *handle, mpu9250_clock_source_t clock_source)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(0x7 << 0);                                                             /* clear config */
    prev |= clock_source << 0;                                                       /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write power management 1 failed.\n");          /* write power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the chip clock source
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *clock_source points to a clock source buffer
 * @return     status code
 *             - 0 success
 *             - 1 get clock source failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_clock_source(mpu9250_handle_t *handle, mpu9250_clock_source_t *clock_source)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    *clock_source = (mpu9250_clock_source_t)((prev >> 0) & (0x7));                   /* get clock source */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     enable or disable the temperature sensor 
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set temperature sensor failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_ptat(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 3);                                                               /* clear config */
    prev |= (!enable) << 3;                                                          /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write power management 1 failed.\n");          /* write power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the temperature sensor status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature sensor failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_ptat(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)(!((prev >> 3) & (0x1)));                              /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     enable or disable the cycle wake up mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set cycle wake up failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_cycle_wake_up(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 5);                                                               /* clear config */
    prev |= enable << 5;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write power management 1 failed.\n");          /* write power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the cycle wake up mode status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get cycle wake up failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_cycle_wake_up(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 5) & (0x1));                                 /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     enable or disable the sleep mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set sleep failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_sleep(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 6);                                                               /* clear config */
    prev |= enable << 6;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write power management 1 failed.\n");          /* write power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the sleep status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sleep failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_sleep(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 6) & (0x1));                                 /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     enable or disable the gyro standby
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set gyro standby failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_gyro_standby(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 4);                                                               /* clear config */
    prev |= enable << 4;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write power management 1 failed.\n");          /* write power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the gyro standby status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyro standby failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyro_standby(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");           /* read power management 1 failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 4) & (0x1));                                 /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     set source into standby mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] source is the input source
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set standby mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_standby_mode(mpu9250_handle_t *handle, mpu9250_source_t source, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_2, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 2 failed.\n");           /* read power management 2 failed */
       
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << source);                                                          /* clear config */
    prev |= enable << source;                                                        /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_2, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write power management 2 failed.\n");          /* write power management 2 failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the source mode
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  source is the input source
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get standby mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_standby_mode(mpu9250_handle_t *handle, mpu9250_source_t source, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_2, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: read power management 2 failed.\n");           /* read power management 2 failed */
       
        return 1;                                                                    /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> source) & (0x1));                            /* get bool */
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the fifo counter value
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *count points to a fifo count buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo count failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_fifo_count(mpu9250_handle_t *handle, uint16_t* count)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }

    res = a_mpu9250_read(handle, MPU9250_REG_FIFO_COUNTH, (uint8_t *)buf, 2);       /* read fifo count */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mpu9250: read fifo count failed.\n");                  /* read fifo count failed */
       
        return 1;                                                                   /* return error */
    }
    *count = (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);                          /* set count */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      fifo read bytes
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 fifo read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_fifo_get(mpu9250_handle_t *handle, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_R_W, buf, len);       /* read fifo */
    if (res != 0)                                                  /* check result */
    {
        handle->debug_print("mpu9250: fifo read failed.\n");       /* fifo read failed */
       
        return 1;                                                  /* return error */
    }
    
    return 0;                                                      /* success return 0 */
}

/**
 * @brief     fifo write bytes
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 fifo write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_fifo_set(mpu9250_handle_t *handle, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }

    res = a_mpu9250_write(handle, MPU9250_REG_R_W, buf, len);       /* write fifo */
    if (res != 0)                                                   /* check result */
    {
        handle->debug_print("mpu9250: fifo write failed.\n");       /* fifo write failed */
        
        return 1;                                                   /* return error */
    }
    
    return 0;                                                       /* success return 0 */
}

/**
 * @brief     set the signal path reset
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] path is the signal path
 * @return    status code
 *            - 0 success
 *            - 1 set signal path reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_signal_path_reset(mpu9250_handle_t *handle, mpu9250_signal_path_reset_t path)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_SIGNAL_PATH_RESET, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("mpu9250: read signal path reset failed.\n");                     /* read signal path reset failed */
       
        return 1;                                                                             /* return error */
    }
    prev &= ~(1 << path);                                                                     /* clear config */
    prev |= 1 << path;                                                                        /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_SIGNAL_PATH_RESET, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("mpu9250: write signal path reset failed.\n");                    /* write signal path reset failed */
       
        return 1;                                                                             /* return error */
    }
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     set the sample rate divider
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] d is the sample rate divider
 * @return    status code
 *            - 0 success
 *            - 1 set sample rate divider failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_sample_rate_divider(mpu9250_handle_t *handle, uint8_t d)
{
    uint8_t res;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_SMPRT_DIV, (uint8_t *)&d, 1);          /* write config */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("mpu9250: write smprt div failed.\n");                   /* write smprt div failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

/**
 * @brief      get the sample rate divider
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *d points to a sample rate divider buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sample rate divider failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_sample_rate_divider(mpu9250_handle_t *handle, uint8_t *d)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_SMPRT_DIV, (uint8_t *)d, 1);          /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read smprt div failed.\n");                  /* read smprt div failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     set the extern sync type
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] sync is the extern sync type
 * @return    status code
 *            - 0 success
 *            - 1 set extern sync failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_extern_sync(mpu9250_handle_t *handle, mpu9250_extern_sync_t sync)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_CONFIG, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read config failed.\n");                     /* read config failed */
       
        return 1;                                                                  /* return error */
    }
    prev &= ~(0x7 << 3);                                                           /* clear config */
    prev |= sync << 3;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_CONFIG, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: write config failed.\n");                    /* write config failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      get the extern sync type
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *sync points to an extern sync type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get extern sync failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_extern_sync(mpu9250_handle_t *handle, mpu9250_extern_sync_t *sync)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_CONFIG, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read config failed.\n");                     /* read config failed */
       
        return 1;                                                                  /* return error */
    }
    *sync = (mpu9250_extern_sync_t)((prev >> 3) & 0x7);                            /* get the extern sync */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     set the low pass filter
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] filter is the low pass filter
 * @return    status code
 *            - 0 success
 *            - 1 set low pass filter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_low_pass_filter(mpu9250_handle_t *handle, mpu9250_low_pass_filter_t filter)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_CONFIG, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read config failed.\n");                     /* read config failed */
       
        return 1;                                                                  /* return error */
    }
    prev &= ~(0x7 << 0);                                                           /* clear config */
    prev |= filter << 0;                                                           /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_CONFIG, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: write config failed.\n");                    /* write config failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      get the low pass filter
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *filter points to a low pass filter buffer
 * @return     status code
 *             - 0 success
 *             - 1 get low pass filter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_low_pass_filter(mpu9250_handle_t *handle, mpu9250_low_pass_filter_t *filter)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_CONFIG, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read config failed.\n");                     /* read config failed */
       
        return 1;                                                                  /* return error */
    }
    *filter = (mpu9250_low_pass_filter_t)((prev >> 0) & 0x7);                      /* get the filter */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     set the fifo mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] mode is the fifo mode
 * @return    status code
 *            - 0 success
 *            - 1 set fifo mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_fifo_mode(mpu9250_handle_t *handle, mpu9250_fifo_mode mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_CONFIG, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read config failed.\n");                     /* read config failed */
       
        return 1;                                                                  /* return error */
    }
    prev &= ~(1 << 6);                                                             /* clear config */
    prev |= mode << 6;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_CONFIG, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: write config failed.\n");                    /* write config failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      get the fifo mode
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *mode points to a fifo mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_fifo_mode(mpu9250_handle_t *handle, mpu9250_fifo_mode *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_CONFIG, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("mpu9250: read config failed.\n");                     /* read config failed */
       
        return 1;                                                                  /* return error */
    }
    *mode = (mpu9250_fifo_mode)((prev >> 6) & 0x1);                                /* get the mode */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     set the gyroscope test
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] axis is the tested axis
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set gyroscope test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_gyroscope_test(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&prev, 1);         /* read gyroscope config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read gyroscope config failed.\n");                /* read gyroscope config failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << axis);                                                               /* clear config */
    prev |= enable << axis;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&prev, 1);        /* write gyroscope config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write gyroscope config failed.\n");               /* write gyroscope config failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the gyroscope test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  axis is the tested axis
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyroscope test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyroscope_test(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&prev, 1);         /* read gyroscope config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read gyroscope config failed.\n");                /* read gyroscope config failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> axis) & 0x01);                                  /* get the bool */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the gyroscope range
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] range is the gyroscope range
 * @return    status code
 *            - 0 success
 *            - 1 set gyroscope range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_gyroscope_range(mpu9250_handle_t *handle, mpu9250_gyroscope_range_t range)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&prev, 1);         /* read gyroscope config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read gyroscope config failed.\n");                /* read gyroscope config failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(3 << 3);                                                                  /* clear config */
    prev |= range << 3;                                                                 /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&prev, 1);        /* write gyroscope config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write gyroscope config failed.\n");               /* write gyroscope config failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the gyroscope range
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *range points to a gyroscope range buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyroscope range failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyroscope_range(mpu9250_handle_t *handle, mpu9250_gyroscope_range_t *range)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&prev, 1);         /* read gyroscope config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read gyroscope config failed.\n");                /* read gyroscope config failed */
       
        return 1;                                                                       /* return error */
    }
    *range = (mpu9250_gyroscope_range_t)((prev >> 3) & 0x3);                            /* get the range */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the gyroscope choice
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] choice is the gyroscope choice
 * @return    status code
 *            - 0 success
 *            - 1 set gyroscope choice failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 choice > 3
 * @note      none
 */
uint8_t mpu9250_set_gyroscope_choice(mpu9250_handle_t *handle, uint8_t choice)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (choice > 3)                                                                     /* check the choice */
    {
        handle->debug_print("mpu9250: choice > 3.\n");                                  /* choice > 3 */
       
        return 4;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&prev, 1);         /* read gyroscope config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read gyroscope config failed.\n");                /* read gyroscope config failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(3 << 0);                                                                  /* clear config */
    prev |= choice << 0;                                                                /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&prev, 1);        /* write gyroscope config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write gyroscope config failed.\n");               /* write gyroscope config failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the gyroscope choice
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *choice points to a gyroscope choice buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyroscope choice failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyroscope_choice(mpu9250_handle_t *handle, uint8_t *choice)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_GYRO_CONFIG, (uint8_t *)&prev, 1);         /* read gyroscope config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read gyroscope config failed.\n");                /* read gyroscope config failed */
       
        return 1;                                                                       /* return error */
    }
    *choice = prev & 0x03;                                                              /* get the choice */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the accelerometer test
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] axis is the tested axis
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_test(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&prev, 1);        /* read accelerometer config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer config failed.\n");            /* read accelerometer config failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << axis);                                                               /* clear config */
    prev |= enable << axis;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&prev, 1);       /* write accelerometer config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write accelerometer config failed.\n");           /* write accelerometer config failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the accelerometer test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  axis is the tested axis
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_test(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&prev, 1);        /* read accelerometer config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer config failed.\n");            /* read accelerometer config failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> axis) & 0x01);                                  /* get the bool */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the accelerometer range
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] range is the accelerometer range
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_range(mpu9250_handle_t *handle, mpu9250_accelerometer_range_t range)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&prev, 1);        /* read accelerometer config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer config failed.\n");            /* read accelerometer config failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(3 << 3);                                                                  /* clear config */
    prev |= range << 3;                                                                 /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&prev, 1);       /* write accelerometer config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write accelerometer config failed.\n");           /* write accelerometer config failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the accelerometer range
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *range points to an accelerometer range buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer range failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_range(mpu9250_handle_t *handle, mpu9250_accelerometer_range_t *range)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG, (uint8_t *)&prev, 1);        /* read accelerometer config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer config failed.\n");            /* read accelerometer config failed */
       
        return 1;                                                                       /* return error */
    }
    *range = (mpu9250_accelerometer_range_t)((prev >> 3) & 0x3);                        /* get the range */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set fifo 1024kb
 * @param[in] *handle points to an mpu9250 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set fifo 1024kb failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_fifo_1024kb(mpu9250_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer 2 config failed.\n");            /* read accelerometer 2 config failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 6);                                                                    /* clear config */
    prev |= 1 << 6;                                                                       /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_ACCEL_CONFIG2, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write accelerometer 2 config failed.\n");           /* write accelerometer 2 config failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the accelerometer choice
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] choice is the accelerometer choice
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer choice failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 choice > 1
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_choice(mpu9250_handle_t *handle, uint8_t choice)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (choice > 1)                                                                     /* check the choice */
    {
        handle->debug_print("mpu9250: choice > 1.\n");                                  /* choice > 1 */
       
        return 4;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG2, (uint8_t *)&prev, 1);       /* read accelerometer config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer 2 config failed.\n");          /* read accelerometer 2 config failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 3);                                                                  /* clear config */
    prev |= choice << 3;                                                                /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_ACCEL_CONFIG2, (uint8_t *)&prev, 1);      /* write accelerometer config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write accelerometer 2 config failed.\n");         /* write accelerometer 2 config failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the accelerometer choice
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *choice points to an accelerometer choice buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer choice failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 choice > 3
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_choice(mpu9250_handle_t *handle, uint8_t *choice)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG2, (uint8_t *)&prev, 1);       /* read accelerometer config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer 2 config failed.\n");          /* read accelerometer 2 config failed */
       
        return 1;                                                                       /* return error */
    }
    *choice = (prev >> 3) & 0x1;                                                        /* get the choice */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the accelerometer low pass filter
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] filter is the accelerometer low pass filter
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer low pass filter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_low_pass_filter(mpu9250_handle_t *handle, mpu9250_accelerometer_low_pass_filter_t filter)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer 2 config failed.\n");            /* read accelerometer 2 config failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(0x7 << 0);                                                                  /* clear config */
    prev |= filter << 0;                                                                  /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_ACCEL_CONFIG2, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write accelerometer 2 config failed.\n");           /* write accelerometer 2 config failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the accelerometer low pass filter
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *filter points to an accelerometer low pass filter buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer low pass filter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_low_pass_filter(mpu9250_handle_t *handle, mpu9250_accelerometer_low_pass_filter_t *filter)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ACCEL_CONFIG2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read accelerometer 2 config failed.\n");            /* read accelerometer 2 config failed */
       
        return 1;                                                                         /* return error */
    }
    *filter = (mpu9250_accelerometer_low_pass_filter_t)(prev & 0x7);                      /* get the filter */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the low power accel output rate
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] rate is the low power accel output rate
 * @return    status code
 *            - 0 success
 *            - 1 set low power accel output rate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_low_power_accel_output_rate(mpu9250_handle_t *handle, mpu9250_low_power_accel_output_rate_t rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_LP_ACCEL_ODR, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read lp accelerometer odr failed.\n");             /* read lp accelerometer odr failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(0xF << 0);                                                                 /* clear config */
    prev |= rate << 0;                                                                   /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_LP_ACCEL_ODR, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write lp accelerometer odr failed.\n");            /* write lp accelerometer odr failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the low power accel output rate
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *rate points to a low power accel output rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 get low power accel output rate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_low_power_accel_output_rate(mpu9250_handle_t *handle, mpu9250_low_power_accel_output_rate_t *rate)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_LP_ACCEL_ODR, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read lp accelerometer odr failed.\n");             /* read lp accelerometer odr failed */
       
        return 1;                                                                        /* return error */
    }
    *rate = (mpu9250_low_power_accel_output_rate_t)(prev & 0xF);                         /* get the rate */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable wake on motion
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set wake on motion failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_wake_on_motion(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_MOT_DETECT_CTRL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("mpu9250: read motion detect ctrl failed.\n");                  /* read motion detect ctrl failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 7);                                                                      /* clear config */
    prev |= enable << 7;                                                                    /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_MOT_DETECT_CTRL, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("mpu9250: write motion detect ctrl failed.\n");                 /* write motion detect ctrl failed */
       
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the wake on motion status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get wake on motion failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_wake_on_motion(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_MOT_DETECT_CTRL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("mpu9250: read motion detect ctrl failed.\n");                  /* read motion detect ctrl failed */
       
        return 1;                                                                           /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 7) & 0x01);                                         /* get the bool */
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     enable or disable accel compare with previous sample
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set accel compare with previous sample failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accel_compare_with_previous_sample(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_MOT_DETECT_CTRL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("mpu9250: read motion detect ctrl failed.\n");                  /* read motion detect ctrl failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 6);                                                                      /* clear config */
    prev |= enable << 6;                                                                    /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_MOT_DETECT_CTRL, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("mpu9250: write motion detect ctrl failed.\n");                 /* write motion detect ctrl failed */
       
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the accel compare with previous sample status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accel compare with previous sample failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accel_compare_with_previous_sample(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_MOT_DETECT_CTRL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("mpu9250: read motion detect ctrl failed.\n");                  /* read motion detect ctrl failed */
       
        return 1;                                                                           /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 6) & 0x01);                                         /* get the bool */
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     enable or disable the fifo function
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] fifo is the fifo type
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_fifo_enable(mpu9250_handle_t *handle, mpu9250_fifo_t fifo, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_FIFO_EN, (uint8_t *)&prev, 1);         /* read fifo enable config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mpu9250: read fifo enable config failed.\n");          /* read fifo enable config failed */
       
        return 1;                                                                   /* return error */
    }
    prev &= ~(1 << fifo);                                                           /* clear config */
    prev |= enable << fifo;                                                         /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_FIFO_EN, (uint8_t *)&prev, 1);        /* write fifo enable config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mpu9250: write fifo enable config failed.\n");         /* write fifo enable config failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief      get the fifo function status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  fifo is the fifo type
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_fifo_enable(mpu9250_handle_t *handle, mpu9250_fifo_t fifo, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_FIFO_EN, (uint8_t *)&prev, 1);         /* read fifo enable config */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("mpu9250: read fifo enable config failed.\n");          /* read fifo enable config failed */
       
        return 1;                                                                   /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> fifo) & 0x01);                              /* get the bool */
    
    return 0;                                                                       /* success return 0 */
}

/**
 * @brief     set the interrupt level
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] level is the interrupt level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_interrupt_level(mpu9250_handle_t *handle, mpu9250_pin_level_t level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 7);                                                                  /* clear config */
    prev |= level << 7;                                                                 /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);        /* write interrupt pin */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write interrupt pin failed.\n");                  /* write interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the interrupt level
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *level points to an interrupt level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_interrupt_level(mpu9250_handle_t *handle, mpu9250_pin_level_t *level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    *level = (mpu9250_pin_level_t)((prev >> 7) & 0x01);                                 /* get the level */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the interrupt pin type
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] type is the interrupt pin type
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin type failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_interrupt_pin_type(mpu9250_handle_t *handle, mpu9250_pin_type_t type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 6);                                                                  /* clear config */
    prev |= type << 6;                                                                  /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);        /* write interrupt pin */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write interrupt pin failed.\n");                  /* write interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the interrupt pin type
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *type points to a pin type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_interrupt_pin_type(mpu9250_handle_t *handle, mpu9250_pin_type_t *type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    *type = (mpu9250_pin_type_t)((prev >> 6) & 0x01);                                   /* get the pin type */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable the interrupt latch
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt latch failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_interrupt_latch(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 5);                                                                  /* clear config */
    prev |= (!enable) << 5;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);        /* write interrupt pin */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write interrupt pin failed.\n");                  /* write interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the interrupt latch status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt latch failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_interrupt_latch(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (mpu9250_bool_t)(!((prev >> 5) & 0x01));                                  /* get the bool */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable the interrupt reading clear
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt read clear failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_interrupt_read_clear(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 4);                                                                  /* clear config */
    prev |= enable << 4;                                                                /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);        /* write interrupt pin */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write interrupt pin failed.\n");                  /* write interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the interrupt reading clear status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt read clear failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_interrupt_read_clear(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    *enable  = (mpu9250_bool_t)((prev >> 4) & 0x01);                                    /* get the bool */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the fsync interrupt level
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] level is the set level
 * @return    status code
 *            - 0 success
 *            - 1 set fsync interrupt level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_fsync_interrupt_level(mpu9250_handle_t *handle, mpu9250_pin_level_t level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 3);                                                                  /* clear config */
    prev |= level << 3;                                                                 /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);        /* write interrupt pin */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write interrupt pin failed.\n");                  /* write interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the fsync interrupt level
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *level points to a set level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fsync interrupt level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_fsync_interrupt_level(mpu9250_handle_t *handle, mpu9250_pin_level_t *level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    *level = (mpu9250_pin_level_t)((prev >> 3) & 0x01);                                 /* get the level */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable the fsync interrupt
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fsync interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_fsync_interrupt(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 2);                                                                  /* clear config */
    prev |= enable << 2;                                                                /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);        /* write interrupt pin */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write interrupt pin failed.\n");                  /* write interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the fsync interrupt status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fsync interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_fsync_interrupt(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 2) & 0x01);                                     /* get the bool */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable the iic bypass
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic bypass failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic_bypass(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 1);                                                                  /* clear config */
    prev |= enable << 1;                                                                /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);        /* write interrupt pin */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write interrupt pin failed.\n");                  /* write interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the iic bypass status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic bypass failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_bypass(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_PIN_CFG, (uint8_t *)&prev, 1);         /* read interrupt pin */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt pin failed.\n");                   /* read interrupt pin failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 1) & 0x01);                                     /* get the bool */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     enable or disable the interrupt
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] type is the set interrupt type
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_interrupt(mpu9250_handle_t *handle, mpu9250_interrupt_t type, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_ENABLE, (uint8_t *)&prev, 1);          /* read interrupt enable */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt enable failed.\n");                /* read interrupt enable failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << type);                                                               /* clear config */
    prev |= enable << type;                                                             /* set config */
    res = a_mpu9250_write(handle, MPU9250_REG_INT_ENABLE, (uint8_t *)&prev, 1);         /* write interrupt enable */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write interrupt enable failed.\n");               /* write interrupt enable failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the interrupt status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  type is the set interrupt type
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_interrupt(mpu9250_handle_t *handle, mpu9250_interrupt_t type, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_ENABLE, (uint8_t *)&prev, 1);          /* read interrupt enable */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt enable failed.\n");                /* read interrupt enable failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> type) & 0x01);                                  /* get the bool */
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the interrupt status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_interrupt_status(mpu9250_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_INT_STATUS, (uint8_t *)status, 1);         /* read interrupt status */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: read interrupt status failed.\n");                /* read interrupt status failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     set the gyroscope x test
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 set gyroscope x test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_gyroscope_x_test(mpu9250_handle_t *handle, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_SELF_TEST_X_GYRO, (uint8_t *)&data, 1);     /* write self test x */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write self test x failed.\n");                      /* write self test x failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the gyroscope x test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *data points to a set data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyroscope x test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyroscope_x_test(mpu9250_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_SELF_TEST_X_GYRO, (uint8_t *)data, 1);       /* read self test x */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read self test x failed.\n");                       /* read self test x failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the gyroscope y test
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 set gyroscope y test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_gyroscope_y_test(mpu9250_handle_t *handle, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_SELF_TEST_Y_GYRO, (uint8_t *)&data, 1);     /* write self test y */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write self test y failed.\n");                      /* write self test y failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the gyroscope y test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *data points to a set data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyroscope y test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyroscope_y_test(mpu9250_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_SELF_TEST_Y_GYRO, (uint8_t *)data, 1);       /* read self test y */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read self test y failed.\n");                       /* read self test y failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the gyroscope z test
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 set gyroscope z test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_gyroscope_z_test(mpu9250_handle_t *handle, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_SELF_TEST_Z_GYRO, (uint8_t *)&data, 1);     /* write self test z */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write self test z failed.\n");                      /* write self test z failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the gyroscope z test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *data points to a set data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyroscope z test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyroscope_z_test(mpu9250_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_SELF_TEST_Z_GYRO, (uint8_t *)data, 1);       /* read self test z */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read self test z failed.\n");                       /* read self test z failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the accelerometer x test
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer x test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_x_test(mpu9250_handle_t *handle, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_SELF_TEST_X_ACCEL, (uint8_t *)&data, 1);    /* write self test x */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write self test x failed.\n");                      /* write self test x failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the accelerometer x test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *data points to a set data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer x test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_x_test(mpu9250_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_SELF_TEST_X_ACCEL, (uint8_t *)data, 1);      /* read self test x */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read self test x failed.\n");                       /* read self test x failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the accelerometer y test
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer y test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_y_test(mpu9250_handle_t *handle, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_SELF_TEST_Y_ACCEL, (uint8_t *)&data, 1);    /* write self test y */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write self test y failed.\n");                      /* write self test y failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the accelerometer y test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *data points to a set data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer y test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_y_test(mpu9250_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_SELF_TEST_Y_ACCEL, (uint8_t *)data, 1);      /* read self test y */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read self test y failed.\n");                       /* read self test y failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the accelerometer z test
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer z test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_z_test(mpu9250_handle_t *handle, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_SELF_TEST_Z_ACCEL, (uint8_t *)&data, 1);    /* write self test z */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write self test z failed.\n");                      /* write self test z failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the accelerometer z test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *data points to a set data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer z test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_z_test(mpu9250_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_SELF_TEST_Z_ACCEL, (uint8_t *)data, 1);      /* read self test z */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read self test z failed.\n");                       /* read self test z failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the accelerometer x offset
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] offset is the accelerometer x offset
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer x offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_x_offset(mpu9250_handle_t *handle, int16_t offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    buf[0] = (offset >> 8) & 0xFF;                                         /* set high */
    buf[1] = (offset >> 0) & 0xFF;                                         /* set low */
    res = a_mpu9250_write(handle, MPU9250_REG_XA_OFFSET_H, buf, 2);        /* write xa offset */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("mpu9250: write xa offset failed.\n");         /* write xa offset failed*/
       
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      get the accelerometer x offset
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *offset points to an accelerometer x offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer x offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_x_offset(mpu9250_handle_t *handle, int16_t *offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_XA_OFFSET_H, buf, 2);        /* read xa offset */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: read xa offset failed.\n");         /* read xa offset failed*/
       
        return 1;                                                         /* return error */
    }
    *offset = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                /* set the offset */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief     set the accelerometer y offset
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] offset is the accelerometer y offset
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer y offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_y_offset(mpu9250_handle_t *handle, int16_t offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    buf[0] = (offset >> 8) & 0xFF;                                         /* set high */
    buf[1] = (offset >> 0) & 0xFF;                                         /* set low */
    res = a_mpu9250_write(handle, MPU9250_REG_YA_OFFSET_H, buf, 2);        /* write ya offset */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("mpu9250: write ya offset failed.\n");         /* write ya offset failed*/
       
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      get the accelerometer y offset
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *offset points to an accelerometer y offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer y offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_y_offset(mpu9250_handle_t *handle, int16_t *offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_YA_OFFSET_H, buf, 2);        /* read ya offset */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: read ya offset failed.\n");         /* read ya offset failed*/
       
        return 1;                                                         /* return error */
    }
    *offset = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                /* set the offset */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief     set the accelerometer z offset
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] offset is the accelerometer z offset
 * @return    status code
 *            - 0 success
 *            - 1 set accelerometer z offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_accelerometer_z_offset(mpu9250_handle_t *handle, int16_t offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    buf[0] = (offset >> 8) & 0xFF;                                         /* set high */
    buf[1] = (offset >> 0) & 0xFF;                                         /* set low */
    res = a_mpu9250_write(handle, MPU9250_REG_ZA_OFFSET_H, buf, 2);        /* write za offset */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("mpu9250: write za offset failed.\n");         /* write za offset failed*/
       
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      get the accelerometer z offset
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *offset points to an accelerometer z offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get accelerometer z offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_accelerometer_z_offset(mpu9250_handle_t *handle, int16_t *offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ZA_OFFSET_H, buf, 2);        /* read za offset */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: read za offset failed.\n");         /* read za offset failed*/
       
        return 1;                                                         /* return error */
    }
    *offset = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                /* set the offset */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief      convert the accelerometer offset to the register raw data
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  mg is the accelerometer offset
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_accelerometer_offset_convert_to_register(mpu9250_handle_t *handle, float mg, int16_t *reg)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *reg = (int16_t)(mg / 0.98f);       /* convert real data to register data */
    
    return 0;                           /* success return 0 */
}

/**
 * @brief      convert the register raw data to the accelerometer offset
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *mg points to an accelerometer offset buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_accelerometer_offset_convert_to_data(mpu9250_handle_t *handle, int16_t reg, float *mg)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *mg = (float)(reg) * 0.98f;        /* convert raw data to real data */
    
    return 0;                          /* success return 0 */
}

/**
 * @brief     set the gyro x offset
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] offset is the gyro x offset
 * @return    status code
 *            - 0 success
 *            - 1 set gyro x offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_gyro_x_offset(mpu9250_handle_t *handle, int16_t offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    buf[0] = (offset >> 8) & 0xFF;                                         /* set high */
    buf[1] = (offset >> 0) & 0xFF;                                         /* set low */
    res = a_mpu9250_write(handle, MPU9250_REG_XG_OFFSET_H, buf, 2);        /* write xg offset */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("mpu9250: write xg offset failed.\n");         /* write xg offset failed*/
       
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      get the gyro x offset
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *offset points to a gyro x offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyro x offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyro_x_offset(mpu9250_handle_t *handle, int16_t *offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_XG_OFFSET_H, buf, 2);        /* read xg offset */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: read xg offset failed.\n");         /* read xg offset failed*/
       
        return 1;                                                         /* return error */
    }
    *offset = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                /* set the offset */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief     set the gyro y offset
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] offset is the gyro y offset
 * @return    status code
 *            - 0 success
 *            - 1 set gyro y offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_gyro_y_offset(mpu9250_handle_t *handle, int16_t offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    buf[0] = (offset >> 8) & 0xFF;                                         /* set high */
    buf[1] = (offset >> 0) & 0xFF;                                         /* set low */
    res = a_mpu9250_write(handle, MPU9250_REG_YG_OFFSET_H, buf, 2);        /* write yg offset */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("mpu9250: write yg offset failed.\n");         /* write yg offset failed*/
       
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      get the gyro y offset
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *offset points to a gyro y offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyro y offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyro_y_offset(mpu9250_handle_t *handle, int16_t *offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_YG_OFFSET_H, buf, 2);        /* read yg offset */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: read yg offset failed.\n");         /* read yg offset failed*/
       
        return 1;                                                         /* return error */
    }
    *offset = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                /* set the offset */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief     set the gyro z offset
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] offset is the gyro z offset
 * @return    status code
 *            - 0 success
 *            - 1 set gyro z offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_gyro_z_offset(mpu9250_handle_t *handle, int16_t offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    buf[0] = (offset >> 8) & 0xFF;                                         /* set high */
    buf[1] = (offset >> 0) & 0xFF;                                         /* set low */
    res = a_mpu9250_write(handle, MPU9250_REG_ZG_OFFSET_H, buf, 2);        /* write zg offset */
    if (res != 0)                                                          /* check result */
    {
        handle->debug_print("mpu9250: write zg offset failed.\n");         /* write zg offset failed*/
       
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

/**
 * @brief      get the gyro z offset
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *offset points to a gyro z offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gyro z offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_gyro_z_offset(mpu9250_handle_t *handle, int16_t *offset)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_ZG_OFFSET_H, buf, 2);        /* read zg offset */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("mpu9250: read zg offset failed.\n");         /* read zg offset failed*/
       
        return 1;                                                         /* return error */
    }
    *offset = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                /* set the offset */
    
    return 0;                                                             /* success return 0 */
}

/**
 * @brief      convert the gyro offset to the register raw data
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  dps is the gyro offset
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_gyro_offset_convert_to_register(mpu9250_handle_t *handle, float dps, int16_t *reg)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->inited != 1)                /* check handle initialization */
    {
        return 3;                           /* return error */
    }
    
    *reg = (int16_t)(dps / 0.0305f);        /* convert real data to register data */
    
    return 0;                               /* success return 0 */
}

/**
 * @brief      convert the register raw data to the gyro offset
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *dps points to a gyro offset buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_gyro_offset_convert_to_data(mpu9250_handle_t *handle, int16_t reg, float *dps)
{
    if (handle == NULL)                   /* check handle */
    {
        return 2;                         /* return error */
    }
    if (handle->inited != 1)              /* check handle initialization */
    {
        return 3;                         /* return error */
    }
    
    *dps = (float)(reg) * 0.0305f;        /* convert raw data to real data */
    
    return 0;                             /* success return 0 */
}

/**
 * @brief     set the motion_threshold
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] threshold is the set threshold
 * @return    status code
 *            - 0 success
 *            - 1 set motion threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_motion_threshold(mpu9250_handle_t *handle, uint8_t threshold)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_WOM_THR, (uint8_t *)&threshold, 1);        /* write motion threshold */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write motion threshold failed.\n");                /* write motion threshold failed*/
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the motion_threshold
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *threshold points to a threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get motion threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_motion_threshold(mpu9250_handle_t *handle, uint8_t *threshold)
{
    uint8_t res;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_WOM_THR, (uint8_t *)threshold, 1);        /* read motion threshold */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("mpu9250: read motion threshold failed.\n");               /* read motion threshold failed*/
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

/**
 * @brief      convert the motion threshold to the register raw data
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  mg is the motion threshold
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_motion_threshold_convert_to_register(mpu9250_handle_t *handle, float mg, uint8_t *reg)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *reg = (uint8_t)(mg / 4.0f);        /* convert real data to register data */
    
    return 0;                           /* success return 0 */
}

/**
 * @brief      convert the register raw data to the motion threshold
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *mg points to a motion threshold buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_motion_threshold_convert_to_data(mpu9250_handle_t *handle, uint8_t reg, float *mg)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    *mg = (float)(reg) * 4.0f;         /* convert raw data to real data */
    
    return 0;                          /* success return 0 */
}

/**
 * @brief      run the self test
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *gyro_offset_raw points to a gyro offset raw buffer
 * @param[out] *accel_offset_raw points to an accel offset raw buffer
 * @return     status code
 *             - 0 success
 *             - 1 self test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_self_test(mpu9250_handle_t *handle, int32_t gyro_offset_raw[3], int32_t accel_offset_raw[3])
{
    uint8_t res;
    uint8_t prev;
    int32_t gyro_offset_raw_st[3];
    int32_t accel_offset_raw_st[3];
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_mpu9250_get_st_biases(handle, gyro_offset_raw, accel_offset_raw, 0);           /* get st biases */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("mpu9250: get st biases failed.\n");                           /* get st biases failed */
       
        return 1;                                                                          /* return error */
    }
    res = a_mpu9250_get_st_biases(handle, gyro_offset_raw_st, accel_offset_raw_st, 1);     /* get st biases */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("mpu9250: get st biases failed.\n");                           /* get st biases failed */
       
        return 1;                                                                          /* return error */
    }
    res = a_mpu9250_accel_self_test(handle, accel_offset_raw, accel_offset_raw_st);        /* accel self test */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("mpu9250: accel self test failed.\n");                         /* accel self test failed */
       
        return 1;                                                                          /* return error */
    }
    res = a_mpu9250_gyro_self_test(handle, gyro_offset_raw, gyro_offset_raw_st);           /* gyro self test */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("mpu9250: gyro self test failed.\n");                          /* gyro self test failed */
       
        return 1;                                                                          /* return error */
    }
    
    prev = 1 << 7;                                                                         /* reset the device */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, &prev, 1);                       /* write pwr mgmt 1 */
    if (res != 0)                                                                          /* check the result */
    {
        handle->debug_print("mpu9250: write pwr mgmt 1 failed.\n");                        /* write pwr mgmt 1 failed */
        
        return 1;                                                                          /* return error */
    }
    handle->delay_ms(100);                                                                 /* delay 100ms */
    res = a_mpu9250_read(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);             /* read config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("mpu9250: read power management 1 failed.\n");                 /* read power management 1 failed */
       
        return 1;                                                                          /* return error */
    }
    prev &= ~(1 << 6);                                                                     /* clear config */
    res = a_mpu9250_write(handle, MPU9250_REG_PWR_MGMT_1, (uint8_t *)&prev, 1);            /* write config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("mpu9250: write power management 1 failed.\n");                /* write power management 1 failed */
       
        return 1;                                                                          /* return error */
    }
    
    return 0;                                                                              /* success return 0 */
}

/**
 * @brief     set the iic clock
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] clk is the iic clock
 * @return    status code
 *            - 0 success
 *            - 1 set iic clock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic_clock(mpu9250_handle_t *handle, mpu9250_iic_clock_t clk)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);         /* read i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                     /* read i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~0xF;                                                                        /* clear the buffer */
    prev |= clk;                                                                         /* set the clock */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);        /* write i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write i2c mst ctrl failed.\n");                    /* write i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the iic clock
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *clk points to an iic clock buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic clock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_clock(mpu9250_handle_t *handle, mpu9250_iic_clock_t *clk)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);         /* read i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                     /* read i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    *clk = (mpu9250_iic_clock_t)(prev & 0x0F);                                           /* get the clock */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable iic multi master
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic multi master failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic_multi_master(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);         /* read i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                     /* read i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 7);                                                                   /* clear the settings */
    prev |= enable << 7;                                                                 /* set the bool */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);        /* write i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write i2c mst ctrl failed.\n");                    /* write i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the iic multi master status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic multi master failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_multi_master(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);         /* read i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                     /* read i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 7) & 0x1);                                       /* get the bool */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable iic wait for external sensor
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic wait for external sensor failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic_wait_for_external_sensor(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);         /* read i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                     /* read i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 6);                                                                   /* clear the settings */
    prev |= enable << 6;                                                                 /* set the bool */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);        /* write i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write i2c mst ctrl failed.\n");                    /* write i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the iic wait for external sensor status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic wait for external sensor failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_wait_for_external_sensor(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);         /* read i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                     /* read i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 6) & 0x1);                                       /* get the bool */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     set the iic read mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] mode is the read mode
 * @return    status code
 *            - 0 success
 *            - 1 set iic read mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic_read_mode(mpu9250_handle_t *handle, mpu9250_iic_read_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);         /* read i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                     /* read i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    prev &= ~(1 << 4);                                                                   /* clear the settings */
    prev |= mode << 4;                                                                   /* set the mode */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);        /* write i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: write i2c mst ctrl failed.\n");                    /* write i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief      get the iic read mode
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *mode points to a read mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic read mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_read_mode(mpu9250_handle_t *handle, mpu9250_iic_read_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);         /* read i2c mst ctrl */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                     /* read i2c mst ctrl failed */
       
        return 1;                                                                        /* return error */
    }
    *mode = (mpu9250_iic_read_mode_t)((prev >> 4) & 0x1);                                /* get the mode */
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     enable or disable the iic fifo
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic fifo enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 * @note      none
 */
uint8_t mpu9250_set_iic_fifo_enable(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if ((slave == MPU9250_IIC_SLAVE_0) ||
        (slave == MPU9250_IIC_SLAVE_1) || 
        (slave == MPU9250_IIC_SLAVE_2)
       )                                                                                    /* slave0-2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_FIFO_EN, (uint8_t *)&prev, 1);             /* read fifo enable */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read fifo enable failed.\n");                     /* read fifo enable failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << slave);                                                              /* clear the settings */
        prev |= enable << slave;                                                            /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_FIFO_EN, (uint8_t *)&prev, 1);            /* write fifo enable ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write fifo enable failed.\n");                    /* write fifo enable failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave3 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);        /* read i2c mst ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                    /* read i2c mst ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 5);                                                                  /* clear the settings */
        prev |= enable << 5;                                                                /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);       /* write i2c mst ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c mst ctrl failed.\n");                   /* write i2c mst ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic fifo status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic fifo enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_fifo_enable(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if ((slave == MPU9250_IIC_SLAVE_0) ||
        (slave == MPU9250_IIC_SLAVE_1) || 
        (slave == MPU9250_IIC_SLAVE_2)
       )                                                                                    /* slave0-2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_FIFO_EN, (uint8_t *)&prev, 1);             /* read fifo enable */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read fifo enable failed.\n");                     /* read fifo enable failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> slave) & 0x1);                                  /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave3 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_CTRL, (uint8_t *)&prev, 1);        /* read i2c mst ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c mst ctrl failed.\n");                    /* read i2c mst ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> 5) & 0x01);                                     /* get the bool */
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the iic mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] mode is the iic mode
 * @return    status code
 *            - 0 success
 *            - 1 set iic mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 * @note      none
 */
uint8_t mpu9250_set_iic_mode(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv0 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 addr failed.\n");                   /* read i2c slv0 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 7);                                                                  /* clear the settings */
        prev |= mode << 7;                                                                  /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv0 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv0 addr failed.\n");                  /* write i2c slv0 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv1 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 addr failed.\n");                   /* read i2c slv1 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 7);                                                                  /* clear the settings */
        prev |= mode << 7;                                                                  /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV1_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv1 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv1 addr failed.\n");                  /* write i2c slv1 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv2 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 addr failed.\n");                   /* read i2c slv2 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 7);                                                                  /* clear the settings */
        prev |= mode << 7;                                                                  /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV2_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv2 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv2 addr failed.\n");                  /* write i2c slv2 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv3 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 addr failed.\n");                   /* read i2c slv3 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 7);                                                                  /* clear the settings */
        prev |= mode << 7;                                                                  /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV3_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv3 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv3 addr failed.\n");                  /* write i2c slv3 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_4)                                                  /* slave4 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv4 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv4 addr failed.\n");                   /* read i2c slv4 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 7);                                                                  /* clear the settings */
        prev |= mode << 7;                                                                  /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV4_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv4 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv4 addr failed.\n");                  /* write i2c slv4 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic mode
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *mode points to an iic mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_mode(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv0 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 addr failed.\n");                   /* read i2c slv0 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *mode = (mpu9250_iic_mode_t)((prev >> 7) & 0x1);                                    /* get the mode */
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv1 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 addr failed.\n");                   /* read i2c slv1 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *mode = (mpu9250_iic_mode_t)((prev >> 7) & 0x1);                                    /* get the mode */
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv2 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 addr failed.\n");                   /* read i2c slv2 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *mode = (mpu9250_iic_mode_t)((prev >> 7) & 0x1);                                    /* get the mode */
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv3 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 addr failed.\n");                   /* read i2c slv3 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *mode = (mpu9250_iic_mode_t)((prev >> 7) & 0x1);                                    /* get the mode */
    }
    else if (slave == MPU9250_IIC_SLAVE_4)                                                  /* slave4 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv4 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv4 addr failed.\n");                   /* read i2c slv4 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *mode = (mpu9250_iic_mode_t)((prev >> 7) & 0x1);                                    /* get the mode */
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the iic address
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] addr_7bit is the iic address
 * @return    status code
 *            - 0 success
 *            - 1 set iic address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 * @note      none
 */
uint8_t mpu9250_set_iic_address(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t addr_7bit)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv0 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 addr failed.\n");                   /* read i2c slv0 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~0x7F;                                                                      /* clear the settings */
        prev |= addr_7bit & 0x7F;                                                           /* set the address */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv0 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv0 addr failed.\n");                  /* write i2c slv0 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv1 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 addr failed.\n");                   /* read i2c slv1 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~0x7F;                                                                      /* clear the settings */
        prev |= addr_7bit & 0x7F;                                                           /* set the address */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV1_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv1 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv1 addr failed.\n");                  /* write i2c slv1 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv2 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 addr failed.\n");                   /* read i2c slv2 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~0x7F;                                                                      /* clear the settings */
        prev |= addr_7bit & 0x7F;                                                           /* set the address */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV2_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv2 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv2 addr failed.\n");                  /* write i2c slv2 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv3 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 addr failed.\n");                   /* read i2c slv3 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~0x7F;                                                                      /* clear the settings */
        prev |= addr_7bit & 0x7F;                                                           /* set the address */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV3_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv3 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv3 addr failed.\n");                  /* write i2c slv3 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_4)                                                  /* slave4 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv4 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv4 addr failed.\n");                   /* read i2c slv4 addr failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~0x7F;                                                                      /* clear the settings */
        prev |= addr_7bit & 0x7F;                                                           /* set the address */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV4_ADDR, (uint8_t *)&prev, 1);      /* write i2c slv4 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv4 addr failed.\n");                  /* write i2c slv4 addr failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic address
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *addr_7bit points to an iic address buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_address(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t *addr_7bit)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv0 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 addr failed.\n");                   /* read i2c slv0 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *addr_7bit = prev & 0x7F;                                                           /* get the address */
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv1 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 addr failed.\n");                   /* read i2c slv1 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *addr_7bit = prev & 0x7F;                                                           /* get the address */
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv2 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 addr failed.\n");                   /* read i2c slv2 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *addr_7bit = prev & 0x7F;                                                           /* get the address */
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv3 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 addr failed.\n");                   /* read i2c slv3 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *addr_7bit = prev & 0x7F;                                                           /* get the address */
    }
    else if (slave == MPU9250_IIC_SLAVE_4)                                                  /* slave4 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_ADDR, (uint8_t *)&prev, 1);       /* read i2c slv4 addr */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv4 addr failed.\n");                   /* read i2c slv4 addr failed */
           
            return 1;                                                                       /* return error */
        }
        *addr_7bit = prev & 0x7F;                                                           /* get the address */
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the iic register
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] reg is the iic register
 * @return    status code
 *            - 0 success
 *            - 1 set iic register failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 * @note      none
 */
uint8_t mpu9250_set_iic_register(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t reg)
{
    uint8_t res;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_REG, (uint8_t *)&reg, 1);        /* write i2c slv0 reg */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv0 reg failed.\n");                   /* write i2c slv0 reg failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV1_REG, (uint8_t *)&reg, 1);        /* write i2c slv1 reg */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv1 reg failed.\n");                   /* write i2c slv1 reg failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV2_REG, (uint8_t *)&reg, 1);        /* write i2c slv2 reg */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv2 reg failed.\n");                   /* write i2c slv2 reg failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV3_REG, (uint8_t *)&reg, 1);        /* write i2c slv3 reg */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv3 reg failed.\n");                   /* write i2c slv3 reg failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_4)                                                  /* slave4 */
    {
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV4_REG, (uint8_t *)&reg, 1);        /* write i2c slv4 reg */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv4 reg failed.\n");                   /* write i2c slv4 reg failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic register
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *reg points to an iic register buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_register(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t *reg)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                     /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_REG, (uint8_t *)reg, 1);        /* read i2c slv0 reg */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 reg failed.\n");                  /* read i2c slv0 reg failed */
           
            return 1;                                                                     /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_REG, (uint8_t *)reg, 1);        /* read i2c slv1 reg */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 reg failed.\n");                  /* read i2c slv1 reg failed */
           
            return 1;                                                                     /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_REG, (uint8_t *)reg, 1);        /* read i2c slv2 reg */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 reg failed.\n");                  /* read i2c slv2 reg failed */
           
            return 1;                                                                     /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_REG, (uint8_t *)reg, 1);        /* read i2c slv3 reg */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 reg failed.\n");                  /* read i2c slv3 reg failed */
           
            return 1;                                                                     /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_4)                                                /* slave4 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_REG, (uint8_t *)reg, 1);        /* read i2c slv4 reg */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv4 reg failed.\n");                  /* read i2c slv4 reg failed */
           
            return 1;                                                                     /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                 /* invalid slave */
       
        return 4;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the iic data out
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 set iic data out failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 * @note      none
 */
uint8_t mpu9250_set_iic_data_out(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_DO, (uint8_t *)&data, 1);        /* write i2c slv0 do */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv0 do failed.\n");                    /* write i2c slv0 do fail */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV1_DO, (uint8_t *)&data, 1);        /* write i2c slv1 do */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv1 do failed.\n");                    /* write i2c slv1 do fail */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV2_DO, (uint8_t *)&data, 1);        /* write i2c slv2 do */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv2 do failed.\n");                    /* write i2c slv2 do fail */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV3_DO, (uint8_t *)&data, 1);        /* write i2c slv3 do */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv3 do failed.\n");                    /* write i2c slv3 do fail */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic data out
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *data points to a set data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic data out failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_data_out(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                     /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_DO, (uint8_t *)data, 1);        /* read i2c slv0 do */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 do failed.\n");                   /* read i2c slv0 do failed */
           
            return 1;                                                                     /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_DO, (uint8_t *)data, 1);        /* read i2c slv1 do */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 do failed.\n");                   /* read i2c slv1 do failed */
           
            return 1;                                                                     /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_DO, (uint8_t *)data, 1);        /* read i2c slv2 do */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 do failed.\n");                   /* read i2c slv2 do failed */
           
            return 1;                                                                     /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_DO, (uint8_t *)data, 1);        /* read i2c slv3 do */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 do failed.\n");                   /* read i2c slv3 do failed */
           
            return 1;                                                                     /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                 /* invalid slave */
       
        return 4;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     enable or disable the iic
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 * @note      none
 */
uint8_t mpu9250_set_iic_enable(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 7);                                                                  /* clear the settings */
        prev |= enable << 7;                                                                /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv0 ctrl failed.\n");                  /* write i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 7);                                                                  /* clear the settings */
        prev |= enable << 7;                                                                /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv1 ctrl failed.\n");                  /* write i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 7);                                                                  /* clear the settings */
        prev |= enable << 7;                                                                /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv2 ctrl failed.\n");                  /* write i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 7);                                                                  /* clear the settings */
        prev |= enable << 7;                                                                /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv3 ctrl failed.\n");                  /* write i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_enable(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> 7) & 0x1);                                      /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> 7) & 0x1);                                      /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> 7) & 0x1);                                      /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> 7) & 0x1);                                      /* get the bool */
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     enable or disable the iic byte swap
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic byte swap failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 * @note      none
 */
uint8_t mpu9250_set_iic_byte_swap(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 6);                                                                  /* clear the settings */
        prev |= enable << 6;                                                                /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv0 ctrl failed.\n");                  /* write i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 6);                                                                  /* clear the settings */
        prev |= enable << 6;                                                                /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv1 ctrl failed.\n");                  /* write i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 6);                                                                  /* clear the settings */
        prev |= enable << 6;                                                                /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv2 ctrl failed.\n");                  /* write i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 6);                                                                  /* clear the settings */
        prev |= enable << 6;                                                                /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv3 ctrl failed.\n");                  /* write i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic byte swap status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic byte swap failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_byte_swap(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> 6) & 0x1);                                      /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> 6) & 0x1);                                      /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> 6) & 0x1);                                      /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *enable = (mpu9250_bool_t)((prev >> 6) & 0x1);                                      /* get the bool */
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the iic transaction mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] mode is the iic transaction mode
 * @return    status code
 *            - 0 success
 *            - 1 set iic transaction mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 * @note      none
 */
uint8_t mpu9250_set_iic_transaction_mode(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_transaction_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 5);                                                                  /* clear the settings */
        prev |= mode << 5;                                                                  /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv0 ctrl failed.\n");                  /* write i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 5);                                                                  /* clear the settings */
        prev |= mode << 5;                                                                  /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv1 ctrl failed.\n");                  /* write i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 5);                                                                  /* clear the settings */
        prev |= mode << 5;                                                                  /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv2 ctrl failed.\n");                  /* write i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 5);                                                                  /* clear the settings */
        prev |= mode << 5;                                                                  /* set the bool */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv3 ctrl failed.\n");                  /* write i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic transaction mode
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *mode points to an iic transaction mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic transaction mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_transaction_mode(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_transaction_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *mode = (mpu9250_iic_transaction_mode_t)((prev >> 5) & 0x1);                        /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *mode = (mpu9250_iic_transaction_mode_t)((prev >> 5) & 0x1);                        /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *mode = (mpu9250_iic_transaction_mode_t)((prev >> 5) & 0x1);                        /* get the bool */
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *mode = (mpu9250_iic_transaction_mode_t)((prev >> 5) & 0x1);                        /* get the bool */
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the iic group order
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] order is the group order
 * @return    status code
 *            - 0 success
 *            - 1 set iic group order failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 * @note      none
 */
uint8_t mpu9250_set_iic_group_order(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_group_order_t order)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 4);                                                                  /* clear the settings */
        prev |= order << 4;                                                                 /* set the order */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv0 ctrl failed.\n");                  /* write i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 4);                                                                  /* clear the settings */
        prev |= order << 4;                                                                 /* set the order */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv1 ctrl failed.\n");                  /* write i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 4);                                                                  /* clear the settings */
        prev |= order << 4;                                                                 /* set the order */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv2 ctrl failed.\n");                  /* write i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~(1 << 4);                                                                  /* clear the settings */
        prev |= order << 4;                                                                 /* set the order */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv3 ctrl failed.\n");                  /* write i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic group order
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *order points to a group order buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic group order failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_group_order(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_group_order_t *order)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *order = (mpu9250_iic_group_order_t)((prev >> 4) & 0x1);                            /* get the order */
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *order = (mpu9250_iic_group_order_t)((prev >> 4) & 0x1);                            /* get the order */
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *order = (mpu9250_iic_group_order_t)((prev >> 4) & 0x1);                            /* get the order */
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *order = (mpu9250_iic_group_order_t)((prev >> 4) & 0x1);                            /* get the order */
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the iic transferred length
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] slave is the iic slave number
 * @param[in] len is the iic transferred length
 * @return    status code
 *            - 0 success
 *            - 1 set iic transferred len failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid slave
 *            - 5 len > 0xF
 * @note      none
 */
uint8_t mpu9250_set_iic_transferred_len(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t len)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    if (len > 0xF)                                                                          /* check handle initialization */
    {
        handle->debug_print("mpu9250: len > 0xF.\n");                                       /* len > 0xF */
       
        return 5;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~0xF;                                                                       /* clear the settings */
        prev |= len;                                                                        /* set the len */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv0 ctrl failed.\n");                  /* write i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~0xF;                                                                       /* clear the settings */
        prev |= len;                                                                        /* set the len */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv1 ctrl failed.\n");                  /* write i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~0xF;                                                                       /* clear the settings */
        prev |= len;                                                                        /* set the len */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv2 ctrl failed.\n");                  /* write i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        prev &= ~0xF;                                                                       /* clear the settings */
        prev |= len;                                                                        /* set the len */
        res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);      /* write i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: write i2c slv3 ctrl failed.\n");                  /* write i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic transferred length
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  slave is the iic slave number
 * @param[out] *len points to an iic transferred length buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic transferred len failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 invalid slave
 * @note       none
 */
uint8_t mpu9250_get_iic_transferred_len(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t *len)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (slave == MPU9250_IIC_SLAVE_0)                                                       /* slave0 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV0_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv0 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv0 ctrl failed.\n");                   /* read i2c slv0 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *len = prev & 0x0F;                                                                 /* get the len */
    }
    else if (slave == MPU9250_IIC_SLAVE_1)                                                  /* slave1 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV1_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv1 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv1 ctrl failed.\n");                   /* read i2c slv1 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *len = prev & 0x0F;                                                                 /* get the len */
    }
    else if (slave == MPU9250_IIC_SLAVE_2)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV2_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv2 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv2 ctrl failed.\n");                   /* read i2c slv2 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *len = prev & 0x0F;                                                                 /* get the len */
    }
    else if (slave == MPU9250_IIC_SLAVE_3)                                                  /* slave2 */
    {
        res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV3_CTRL, (uint8_t *)&prev, 1);       /* read i2c slv3 ctrl */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("mpu9250: read i2c slv3 ctrl failed.\n");                   /* read i2c slv3 ctrl failed */
           
            return 1;                                                                       /* return error */
        }
        *len = prev & 0x0F;                                                                 /* get the len */
    }
    else
    {
        handle->debug_print("mpu9250: invalid slave.\n");                                   /* invalid slave */
       
        return 4;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief      get the iic status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_status(mpu9250_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_STATUS, (uint8_t *)status, 1);        /* read i2c mst status */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst status failed.\n");                     /* read i2c mst status failed */
       
        return 1;                                                                          /* return error */
    }
    
    return 0;                                                                              /* success return 0 */
}

/**
 * @brief     enable or disable the iic delay
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] delay is the iic delay
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic delay enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic_delay_enable(mpu9250_handle_t *handle, mpu9250_iic_delay_t delay, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_DELAY_CTRL, (uint8_t *)&prev, 1);        /* read i2c mst delay ctrl */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst delay ctrl failed.\n");                    /* read i2c mst delay ctrl failed */
       
        return 1;                                                                             /* return error */
    }
    prev &= ~(1 << delay);                                                                    /* clear the settings */
    prev |= enable << delay;                                                                  /* set the bool */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_MST_DELAY_CTRL, (uint8_t *)&prev, 1);       /* write i2c mst delay ctrl */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("mpu9250: write i2c mst delay ctrl failed.\n");                   /* write i2c mst delay ctrl failed */
       
        return 1;                                                                             /* return error */
    }
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief      get the iic delay status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  delay is the iic delay
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic delay enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_delay_enable(mpu9250_handle_t *handle, mpu9250_iic_delay_t delay, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_MST_DELAY_CTRL, (uint8_t *)&prev, 1);        /* read i2c mst delay ctrl */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("mpu9250: read i2c mst delay ctrl failed.\n");                    /* read i2c mst delay ctrl failed */
       
        return 1;                                                                             /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> delay) & 0x1);                                        /* get the bool */
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     enable or disable the iic4
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic4 enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic4_enable(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);         /* read i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 ctrl failed.\n");                     /* read i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 7);                                                                    /* clear the settings */
    prev |= enable << 7;                                                                  /* set the bool */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);        /* write i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write i2c slv4 ctrl failed.\n");                    /* write i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the iic4 status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic4 enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic4_enable(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);         /* read i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 ctrl failed.\n");                     /* read i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 7) & 0x1);                                        /* get the bool */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     enable or disable the iic4 interrupt
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic4 interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic4_interrupt(mpu9250_handle_t *handle, mpu9250_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);         /* read i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 ctrl failed.\n");                     /* read i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 6);                                                                    /* clear the settings */
    prev |= enable << 6;                                                                  /* set the bool */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);        /* write i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write i2c slv4 ctrl failed.\n");                    /* write i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the iic4 interrupt status
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic4 interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic4_interrupt(mpu9250_handle_t *handle, mpu9250_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);         /* read i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 ctrl failed.\n");                     /* read i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    *enable = (mpu9250_bool_t)((prev >> 6) & 0x1);                                        /* get the bool */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the iic4 transaction mode
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] mode is the transaction mode
 * @return    status code
 *            - 0 success
 *            - 1 set iic4 transaction mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic4_transaction_mode(mpu9250_handle_t *handle, mpu9250_iic4_transaction_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);         /* read i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 ctrl failed.\n");                     /* read i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 5);                                                                    /* clear the settings */
    prev |= mode << 5;                                                                    /* set the mode */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);        /* write i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write i2c slv4 ctrl failed.\n");                    /* write i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the iic4 transaction mode
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *mode points to a transaction mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic4 transaction mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic4_transaction_mode(mpu9250_handle_t *handle, mpu9250_iic4_transaction_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);         /* read i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 ctrl failed.\n");                     /* read i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    *mode = (mpu9250_iic4_transaction_mode_t)((prev >> 5) & 0x1);                         /* get the mode */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the iic delay
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] delay is the iic delay
 * @return    status code
 *            - 0 success
 *            - 1 set iic delay failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 delay > 0x1F
 * @note      none
 */
uint8_t mpu9250_set_iic_delay(mpu9250_handle_t *handle, uint8_t delay)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    if (delay > 0x1F)                                                                     /* check the delay */
    {
        handle->debug_print("mpu9250: delay > 0x1F.\n");                                  /* delay > 0x1F */
        
        return 4;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);         /* read i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 ctrl failed.\n");                     /* read i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    prev &= ~0x1F;                                                                        /* clear the settings */
    prev |= delay;                                                                        /* set the delay */
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);        /* write i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: write i2c slv4 ctrl failed.\n");                    /* write i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get the iic delay
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *delay points to an iic delay buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic delay failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic_delay(mpu9250_handle_t *handle, uint8_t *delay)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_CTRL, (uint8_t *)&prev, 1);         /* read i2c slv4 ctrl */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 ctrl failed.\n");                     /* read i2c slv4 ctrl failed */
       
        return 1;                                                                         /* return error */
    }
    *delay = prev & 0x1F;                                                                 /* get the delay */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     set the iic4 data out
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 set iic4 data out failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic4_data_out(mpu9250_handle_t *handle, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV4_DO, (uint8_t *)&data, 1);        /* write i2c slv4 do */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write i2c slv4 do failed.\n");                    /* write i2c slv4 do fail */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the iic4 data out
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *data points to a set data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic4 data out failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic4_data_out(mpu9250_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_DO, (uint8_t *)data, 1);        /* read i2c slv4 do */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 do failed.\n");                   /* read i2c slv4 do failed */
       
        return 1;                                                                     /* return error */
    }
    
    return 0;                                                                         /* success return 0 */
}

/**
 * @brief     set the iic4 data in
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 set iic4 data in failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_iic4_data_in(mpu9250_handle_t *handle, uint8_t data)
{
    uint8_t res;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_mpu9250_write(handle, MPU9250_REG_I2C_SLV4_DI, (uint8_t *)&data, 1);        /* write i2c slv4 di */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("mpu9250: write i2c slv4 di failed.\n");                    /* write i2c slv4 di failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief      get the iic4 data in
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *data points to a set data buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic4 data in failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_iic4_data_in(mpu9250_handle_t *handle, uint8_t *data)
{
    uint8_t res;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_I2C_SLV4_DI, (uint8_t *)data, 1);        /* read i2c slv4 di */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("mpu9250: read i2c slv4 di failed.\n");                   /* read i2c slv4 di failed */
       
        return 1;                                                                     /* return error */
    }
    
    return 0;                                                                         /* success return 0 */
}

/**
 * @brief      read the extern sensor data
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *data points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read extern sensor data failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 len > 24
 * @note       none
 */
uint8_t mpu9250_read_extern_sensor_data(mpu9250_handle_t *handle, uint8_t *data, uint8_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    if (len > 24)                                                                            /* check handle initialization */
    {
        handle->debug_print("mpu9250: len > 24.\n");                                         /* len > 24 */
        
        return 4;                                                                            /* return error */
    }
    
    res = a_mpu9250_read(handle, MPU9250_REG_EXT_SENS_DATA_00, (uint8_t *)data, len);        /* read ext sens data 00 */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("mpu9250: read ext sens data 00 failed.\n");                     /* read ext sens data 00 failed */
       
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

/**
 * @brief     set the chip register
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mpu9250_set_reg(mpu9250_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                  /* check handle */
    {
        return 2;                                        /* return error */
    }
    if (handle->inited != 1)                             /* check handle initialization */
    {
        return 3;                                        /* return error */
    }
    
    return a_mpu9250_write(handle, reg, buf, len);       /* write data */
}

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mpu9250_get_reg(mpu9250_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }
    if (handle->inited != 1)                            /* check handle initialization */
    {
        return 3;                                       /* return error */
    }
    
    return a_mpu9250_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get the chip's information
 * @param[out] *info points to an mpu9250 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mpu9250_info(mpu9250_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(mpu9250_info_t));                        /* initialize mpu9250 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC SPI", 8);                         /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
