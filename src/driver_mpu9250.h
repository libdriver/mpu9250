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
 * @file      driver_mpu9250.h
 * @brief     driver mpu9250 header file
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

#ifndef DRIVER_MPU9250_H
#define DRIVER_MPU9250_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup mpu9250_driver mpu9250 driver function
 * @brief    mpu9250 driver modules
 * @{
 */

/**
 * @addtogroup mpu9250_basic_driver
 * @{
 */

/**
 * @brief mpu9250 interface enumeration definition
 */
typedef enum
{
    MPU9250_INTERFACE_IIC = 0x00,        /**< iic interface function */
    MPU9250_INTERFACE_SPI = 0x01,        /**< spi interface function */
} mpu9250_interface_t;

/**
 * @brief mpu9250 address enumeration definition
 */
typedef enum  
{
    MPU9250_ADDRESS_AD0_LOW  = 0xD0,        /**< AD0 pin set LOW */
    MPU9250_ADDRESS_AD0_HIGH = 0xD2,        /**< AD0 pin set HIGH */
} mpu9250_address_t;

/**
 * @brief mpu9250 bool enumeration definition
 */
typedef enum  
{
    MPU9250_BOOL_FALSE = 0x00,        /**< disable function */
    MPU9250_BOOL_TRUE  = 0x01,        /**< enable function */
} mpu9250_bool_t;

/**
 * @brief mpu9250 source enumeration definition
 */
typedef enum  
{
    MPU9250_SOURCE_ACC_X  = 0x05,        /**< accelerometer x */
    MPU9250_SOURCE_ACC_Y  = 0x04,        /**< accelerometer y */
    MPU9250_SOURCE_ACC_Z  = 0x03,        /**< accelerometer z */
    MPU9250_SOURCE_GYRO_X = 0x02,        /**< gyroscope x */
    MPU9250_SOURCE_GYRO_Y = 0x01,        /**< gyroscope y */
    MPU9250_SOURCE_GYRO_Z = 0x00,        /**< gyroscope z */
} mpu9250_source_t;

/**
 * @brief mpu9250 clock source enumeration definition
 */
typedef enum  
{
    MPU9250_CLOCK_SOURCE_INTERNAL_20MHZ = 0x00,        /**< internal 20MHz */
    MPU9250_CLOCK_SOURCE_PLL            = 0x01,        /**< pll reference */
    MPU9250_CLOCK_SOURCE_STOP_CLOCK     = 0x07,        /**< stop the clock */
} mpu9250_clock_source_t;

/**
 * @brief mpu9250 signal path reset enumeration definition
 */
typedef enum  
{
    MPU9250_SIGNAL_PATH_RESET_TEMP  = 0x00,        /**< temperature sensor analog and digital signal paths */
    MPU9250_SIGNAL_PATH_RESET_ACCEL = 0x01,        /**< accelerometer analog and digital signal paths */
    MPU9250_SIGNAL_PATH_RESET_GYRO  = 0x02,        /**< gyroscope analog and digital signal paths */
} mpu9250_signal_path_reset_t;

/**
 * @brief mpu9250 extern sync enumeration definition
 */
typedef enum  
{
    MPU9250_EXTERN_SYNC_INPUT_DISABLED = 0x00,        /**< input disabled */
    MPU9250_EXTERN_SYNC_TEMP_OUT_L     = 0x01,        /**< temp out low */
    MPU9250_EXTERN_SYNC_GYRO_XOUT_L    = 0x02,        /**< gyro xout low */
    MPU9250_EXTERN_SYNC_GYRO_YOUT_L    = 0x03,        /**< gyro yout low */
    MPU9250_EXTERN_SYNC_GYRO_ZOUT_L    = 0x04,        /**< gyro zout low */
    MPU9250_EXTERN_SYNC_ACCEL_XOUT_L   = 0x05,        /**< accel xout low */
    MPU9250_EXTERN_SYNC_ACCEL_YOUT_L   = 0x06,        /**< accel yout low */
    MPU9250_EXTERN_SYNC_ACCEL_ZOUT_L   = 0x07,        /**< accel zout low */
} mpu9250_extern_sync_t;

/**
 * @brief mpu9250 low pass filter enumeration definition
 */
typedef enum                                 /**< fchoice_b              gyroscope                    temperature       */
{                                            /**<            bandwidth(Hz) fs(KHz) delay(ms)   bandwidth(Hz) delay(ms)  */
                                             /**<  11/01          8800        32     0.064         4000        0.04     */
                                             /**<   10            3600        32     0.11         4000         0.04     */
    MPU9250_LOW_PASS_FILTER_0 = 0x00,        /**<   00            250         8      0.97          4000        0.04     */
    MPU9250_LOW_PASS_FILTER_1 = 0x01,        /**<   00            184         1      2.9           188         1.9      */
    MPU9250_LOW_PASS_FILTER_2 = 0x02,        /**<   00            92          1      3.9           98          2.8      */
    MPU9250_LOW_PASS_FILTER_3 = 0x03,        /**<   00            41          1      5.9           42          4.8      */
    MPU9250_LOW_PASS_FILTER_4 = 0x04,        /**<   00            20          1      9.9           20          8.3      */
    MPU9250_LOW_PASS_FILTER_5 = 0x05,        /**<   00            10          1      17.85         10          13.4     */
    MPU9250_LOW_PASS_FILTER_6 = 0x06,        /**<   00            5           1      33.48         5           18.6     */
    MPU9250_LOW_PASS_FILTER_7 = 0x07,        /**<   00            3600        8      0.17          4000        0.04     */
} mpu9250_low_pass_filter_t;

/**
 * @brief mpu9250 accelerometer low pass filter enumeration definition
 */
typedef enum                                               /**< accelerometer fchoice              accelerometer         */
{                                                          /**<                          3db BW(Hz) rate(KHz) delay(ms)  */
                                                           /**<              1             1046         4      0.503     */
    MPU9250_ACCELEROMETER_LOW_PASS_FILTER_0 = 0x00,        /**<              0             218.1        1      1.88      */
    MPU9250_ACCELEROMETER_LOW_PASS_FILTER_1 = 0x01,        /**<              0             218.1        1      1.88      */
    MPU9250_ACCELEROMETER_LOW_PASS_FILTER_2 = 0x02,        /**<              0             99           1      2.88      */
    MPU9250_ACCELEROMETER_LOW_PASS_FILTER_3 = 0x03,        /**<              0             44.8         1      4.88      */
    MPU9250_ACCELEROMETER_LOW_PASS_FILTER_4 = 0x04,        /**<              0             21.2         1      8.87      */
    MPU9250_ACCELEROMETER_LOW_PASS_FILTER_5 = 0x05,        /**<              0             10.2         1      16.83     */
    MPU9250_ACCELEROMETER_LOW_PASS_FILTER_6 = 0x06,        /**<              0             5.05         1      32.48     */
    MPU9250_ACCELEROMETER_LOW_PASS_FILTER_7 = 0x07,        /**<              0             420          1      1.38      */
} mpu9250_accelerometer_low_pass_filter_t;

/**
 * @brief mpu9250 low power accel output rate enumeration definition
 */
typedef enum  
{
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_0P24  = 0x00,        /**< 0.24Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_0P49  = 0x01,        /**< 0.49Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_0P98  = 0x02,        /**< 0.98Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_1P95  = 0x03,        /**< 1.95Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_3P91  = 0x04,        /**< 3.91Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_7P81  = 0x05,        /**< 7.81Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_15P63 = 0x06,        /**< 15.63Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_31P25 = 0x07,        /**< 31.25Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_62P50 = 0x08,        /**< 62.50Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_125   = 0x09,        /**< 125Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_250   = 0x0A,        /**< 250Hz */
    MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_500   = 0x0B,        /**< 500Hz */
} mpu9250_low_power_accel_output_rate_t;

/**
 * @brief mpu9250 axis enumeration definition
 */
typedef enum  
{
    MPU9250_AXIS_Z = 0x05,        /**< z */
    MPU9250_AXIS_Y = 0x06,        /**< y */
    MPU9250_AXIS_X = 0x07,        /**< x */
} mpu9250_axis_t;

/**
 * @brief mpu9250 gyroscope range enumeration definition
 */
typedef enum  
{
    MPU9250_GYROSCOPE_RANGE_250DPS  = 0x00,        /**< ±250 dps */
    MPU9250_GYROSCOPE_RANGE_500DPS  = 0x01,        /**< ±500 dps */
    MPU9250_GYROSCOPE_RANGE_1000DPS = 0x02,        /**< ±1000 dps */
    MPU9250_GYROSCOPE_RANGE_2000DPS = 0x03,        /**< ±2000 dps */
} mpu9250_gyroscope_range_t;

/**
 * @brief mpu9250 accelerometer range enumeration definition
 */
typedef enum  
{
    MPU9250_ACCELEROMETER_RANGE_2G  = 0x00,        /**< ±2 g */
    MPU9250_ACCELEROMETER_RANGE_4G  = 0x01,        /**< ±4 g */
    MPU9250_ACCELEROMETER_RANGE_8G  = 0x02,        /**< ±8 g */
    MPU9250_ACCELEROMETER_RANGE_16G = 0x03,        /**< ±16 g */
} mpu9250_accelerometer_range_t;

/**
 * @brief mpu9250 fifo enumeration definition
 */
typedef enum  
{
    MPU9250_FIFO_TEMP  = 0x07,        /**< temperature */
    MPU9250_FIFO_XG    = 0x06,        /**< gyroscope x */
    MPU9250_FIFO_YG    = 0x05,        /**< gyroscope y */
    MPU9250_FIFO_ZG    = 0x04,        /**< gyroscope z */
    MPU9250_FIFO_ACCEL = 0x03,        /**< accelerometer */
} mpu9250_fifo_t;

/**
 * @brief mpu9250 fifo mode enumeration definition
 */
typedef enum  
{
    MPU9250_FIFO_MODE_STREAM = 0x00,        /**< when the fifo is full, additional writes will be written to the fifo, 
                                                 replacing the oldest data */
    MPU9250_FIFO_MODE_NORMAL = 0x01,        /**< when the fifo is full, additional writes will not be written to fifo */
} mpu9250_fifo_mode;

/**
 * @brief mpu9250 pin level enumeration definition
 */
typedef enum  
{
    MPU9250_PIN_LEVEL_HIGH = 0x00,        /**< active low */
    MPU9250_PIN_LEVEL_LOW  = 0x01,        /**< active high */
} mpu9250_pin_level_t;

/**
 * @brief mpu9250 pin type enumeration definition
 */
typedef enum  
{
    MPU9250_PIN_TYPE_PUSH_PULL  = 0x00,        /**< push pull */
    MPU9250_PIN_TYPE_OPEN_DRAIN = 0x01,        /**< open drain */
} mpu9250_pin_type_t;

/**
 * @brief mpu9250 interrupt enumeration definition
 */
typedef enum  
{
    MPU9250_INTERRUPT_MOTION        = 6,        /**< motion */
    MPU9250_INTERRUPT_FIFO_OVERFLOW = 4,        /**< fifo overflow */
    MPU9250_INTERRUPT_FSYNC_INT     = 3,        /**< fsync int */
    MPU9250_INTERRUPT_DMP           = 1,        /**< dmp */
    MPU9250_INTERRUPT_DATA_READY    = 0,        /**< data ready */
} mpu9250_interrupt_t;

/**
 * @brief mpu9250 iic slave enumeration definition
 */
typedef enum  
{
    MPU9250_IIC_SLAVE_0 = 0x00,        /**< slave0 */
    MPU9250_IIC_SLAVE_1 = 0x01,        /**< slave1 */
    MPU9250_IIC_SLAVE_2 = 0x02,        /**< slave2 */
    MPU9250_IIC_SLAVE_3 = 0x03,        /**< slave3 */
    MPU9250_IIC_SLAVE_4 = 0x04,        /**< slave4 */
} mpu9250_iic_slave_t;

/**
 * @brief mpu9250 iic clock enumeration definition
 */
typedef enum  
{
    MPU9250_IIC_CLOCK_348_KHZ = 0x00,        /**< 348 kHz */
    MPU9250_IIC_CLOCK_333_KHZ = 0x01,        /**< 333 kHz */
    MPU9250_IIC_CLOCK_320_KHZ = 0x02,        /**< 320 kHz */
    MPU9250_IIC_CLOCK_308_KHZ = 0x03,        /**< 308 kHz */
    MPU9250_IIC_CLOCK_296_KHZ = 0x04,        /**< 296 kHz */
    MPU9250_IIC_CLOCK_286_KHZ = 0x05,        /**< 286 kHz */
    MPU9250_IIC_CLOCK_276_KHZ = 0x06,        /**< 276 kHz */
    MPU9250_IIC_CLOCK_267_KHZ = 0x07,        /**< 267 kHz */
    MPU9250_IIC_CLOCK_258_KHZ = 0x08,        /**< 258 kHz */
    MPU9250_IIC_CLOCK_500_KHZ = 0x09,        /**< 500 kHz */
    MPU9250_IIC_CLOCK_471_KHZ = 0x0A,        /**< 471 kHz */
    MPU9250_IIC_CLOCK_444_KHZ = 0x0B,        /**< 444 kHz */
    MPU9250_IIC_CLOCK_421_KHZ = 0x0C,        /**< 421 kHz */
    MPU9250_IIC_CLOCK_400_KHZ = 0x0D,        /**< 400 kHz */
    MPU9250_IIC_CLOCK_381_KHZ = 0x0E,        /**< 381 kHz */
    MPU9250_IIC_CLOCK_364_KHZ = 0x0F,        /**< 364 kHz */
} mpu9250_iic_clock_t;

/**
 * @brief mpu9250 iic read mode enumeration definition
 */
typedef enum  
{
    MPU9250_IIC_READ_MODE_RESTART        = 0x00,        /**< restart */
    MPU9250_IIC_READ_MODE_STOP_AND_START = 0x01,        /**< stop and start */
} mpu9250_iic_read_mode_t;

/**
 * @brief mpu9250 iic mode enumeration definition
 */
typedef enum  
{
    MPU9250_IIC_MODE_WRITE = 0x00,        /**< write */
    MPU9250_IIC_MODE_READ  = 0x01,        /**< read */
} mpu9250_iic_mode_t;

/**
 * @brief mpu9250 iic transaction mode enumeration definition
 */
typedef enum  
{
    MPU9250_IIC_TRANSACTION_MODE_DATA     = 0x00,        /**< data only */
    MPU9250_IIC_TRANSACTION_MODE_REG_DATA = 0x01,        /**< write a register address prior to reading or writing data */
} mpu9250_iic_transaction_mode_t;

/**
 * @brief mpu9250 iic4 transaction mode enumeration definition
 */
typedef enum  
{
    MPU9250_IIC4_TRANSACTION_MODE_DATA = 0x00,        /**< data only */
    MPU9250_IIC4_TRANSACTION_MODE_REG  = 0x01,        /**< register only */
} mpu9250_iic4_transaction_mode_t;

/**
 * @brief mpu9250 iic group order enumeration definition
 */
typedef enum  
{
    MPU9250_IIC_GROUP_ORDER_EVEN = 0x00,        /**< when cleared to 0, bytes from register addresses 0 and 1, 2 and 3, 
                                                     etc (even, then odd register addresses) are paired to form a word */
    MPU9250_IIC_GROUP_ORDER_ODD  = 0x01,        /**< when set to 1, bytes from register addresses are paired 1 and 2, 3 and 4, 
                                                     etc (odd, then even register addresses) are paired to form a word */
} mpu9250_iic_group_order_t;

/**
 * @brief mpu9250 iic status enumeration definition
 */
typedef enum  
{
    MPU9250_IIC_STATUS_PASS_THROUGH  = 0x80,        /**< pass through */
    MPU9250_IIC_STATUS_IIC_SLV4_DONE = 0x40,        /**< slave4 done */
    MPU9250_IIC_STATUS_IIC_LOST_ARB  = 0x20,        /**< lost arbitration */
    MPU9250_IIC_STATUS_IIC_SLV4_NACK = 0x10,        /**< slave4 nack */
    MPU9250_IIC_STATUS_IIC_SLV3_NACK = 0x08,        /**< slave3 nack */
    MPU9250_IIC_STATUS_IIC_SLV2_NACK = 0x04,        /**< slave2 nack */
    MPU9250_IIC_STATUS_IIC_SLV1_NACK = 0x02,        /**< slave1 nack */
    MPU9250_IIC_STATUS_IIC_SLV0_NACK = 0x01,        /**< slave0 nack */
} mpu9250_iic_status_t;

/**
 * @brief mpu9250 iic delay enumeration definition
 */
typedef enum  
{
    MPU9250_IIC_DELAY_ES_SHADOW = 7,        /**< delays shadowing of external sensor data until 
                                                 all data has been received */
    MPU9250_IIC_DELAY_SLAVE_4   = 4,        /**< slave 4 */
    MPU9250_IIC_DELAY_SLAVE_3   = 3,        /**< slave 3 */
    MPU9250_IIC_DELAY_SLAVE_2   = 2,        /**< slave 2 */
    MPU9250_IIC_DELAY_SLAVE_1   = 1,        /**< slave 1 */
    MPU9250_IIC_DELAY_SLAVE_0   = 0,        /**< slave 0 */
} mpu9250_iic_delay_t;

/**
 * @}
 */

/**
 * @addtogroup mpu9250_magnetometer_driver
 * @{
 */

/**
 * @brief mpu9250 magnetometer status1 enumeration definition
 */
typedef enum
{
    MPU9250_MAGNETOMETER_STATUS1_DRDY = (1 << 0),        /**< data is ready */
    MPU9250_MAGNETOMETER_STATUS1_DOR  = (1 << 1),        /**< data overrun */
} mpu9250_magnetometer_status1_t;

/**
 * @brief mpu9250 magnetometer status2 enumeration definition
 */
typedef enum
{
    MPU9250_MAGNETOMETER_STATUS2_HOFL = (1 << 3),        /**< magnetic sensor overflow occurred */
    MPU9250_MAGNETOMETER_STATUS2_BITM = (1 << 4),        /**< 0: 14bits / 1: 16bits */
} mpu9250_magnetometer_status2_t;

/**
 * @brief mpu9250 magnetometer mode enumeration definition
 */
typedef enum
{
    MPU9250_MAGNETOMETER_MODE_POWER_DOWN  = 0x00,        /**< power down mode */
    MPU9250_MAGNETOMETER_MODE_SINGLE      = 0x01,        /**< single measurement mode */
    MPU9250_MAGNETOMETER_MODE_CONTINUOUS1 = 0x02,        /**< continuous measurement mode 1 */
    MPU9250_MAGNETOMETER_MODE_CONTINUOUS2 = 0x06,        /**< continuous measurement mode 2 */
    MPU9250_MAGNETOMETER_MODE_EXT_TRIGGER = 0x04,        /**< external trigger measurement mode */
    MPU9250_MAGNETOMETER_MODE_SELF_TEST   = 0x08,        /**< self test mode */
    MPU9250_MAGNETOMETER_MODE_FUSE_ROM    = 0x0F,        /**< fuse rom access mode */
} mpu9250_magnetometer_mode_t;

/**
 * @brief mpu9250 magnetometer bits enumeration definition
 */
typedef enum
{
    MPU9250_MAGNETOMETER_BITS_14 = 0x00,        /**< 14 bits output */
    MPU9250_MAGNETOMETER_BITS_16 = 0x01,        /**< 16 bits output */
} mpu9250_magnetometer_bits_t;

/**
 * @}
 */

/**
 * @addtogroup mpu9250_dmp_driver
 * @{
 */

/**
 * @brief mpu9250 dmp interrupt mode enumeration definition
 */
typedef enum  
{
    MPU9250_DMP_INTERRUPT_MODE_CONTINUOUS = 0x00,        /**< continuous mode */
    MPU9250_DMP_INTERRUPT_MODE_GESTURE    = 0x01,        /**< gesture mode */
} mpu9250_dmp_interrupt_mode_t;

/**
 * @brief mpu9250 dmp feature enumeration definition
 */
typedef enum  
{
    MPU9250_DMP_FEATURE_TAP            = 0x001,        /**< feature tap */
    MPU9250_DMP_FEATURE_ORIENT         = 0x002,        /**< feature orient */
    MPU9250_DMP_FEATURE_3X_QUAT        = 0x004,        /**< feature 3x quat */
    MPU9250_DMP_FEATURE_PEDOMETER      = 0x008,        /**< feature pedometer */
    MPU9250_DMP_FEATURE_6X_QUAT        = 0x010,        /**< feature 6x quat */
    MPU9250_DMP_FEATURE_GYRO_CAL       = 0x020,        /**< feature gyro cal */
    MPU9250_DMP_FEATURE_SEND_RAW_ACCEL = 0x040,        /**< feature send raw accel */
    MPU9250_DMP_FEATURE_SEND_RAW_GYRO  = 0x080,        /**< feature send raw gyro */
    MPU9250_DMP_FEATURE_SEND_CAL_GYRO  = 0x100,        /**< feature send cal gyro */
} mpu9250_dmp_feature_t;

/**
 * @brief mpu9250 dmp tap enumeration definition
 */
typedef enum  
{
    MPU9250_DMP_TAP_X_UP   = 0x01,        /**< tap x up */
    MPU9250_DMP_TAP_X_DOWN = 0x02,        /**< tap x down */
    MPU9250_DMP_TAP_Y_UP   = 0x03,        /**< tap y up */
    MPU9250_DMP_TAP_Y_DOWN = 0x04,        /**< tap y down */
    MPU9250_DMP_TAP_Z_UP   = 0x05,        /**< tap z up */
    MPU9250_DMP_TAP_Z_DOWN = 0x06,        /**< tap z down */
} mpu9250_dmp_tap_t;

/**
 * @brief mpu9250 dmp orient enumeration definition
 */
typedef enum  
{
    MPU9250_DMP_ORIENT_PORTRAIT          = 0x00,        /**< portrait */
    MPU9250_DMP_ORIENT_LANDSCAPE         = 0x01,        /**< landscape */
    MPU9250_DMP_ORIENT_REVERSE_PORTRAIT  = 0x02,        /**< reverse portrait */
    MPU9250_DMP_ORIENT_REVERSE_LANDSCAPE = 0x03,        /**< reverse landscape */
} mpu9250_dmp_orient_t;

/**
 * @}
 */

/**
 * @addtogroup mpu9250_basic_driver
 * @{
 */

/**
 * @brief mpu9250 handle structure definition
 */
typedef struct mpu9250_handle_s
{
    uint8_t iic_addr;                                                                   /**< iic device address */
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*spi_init)(void);                                                          /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                        /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);                       /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);                      /**< point to a spi_write function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*dmp_tap_callback)(uint8_t count, uint8_t direction);                         /**< point to a dmp_tap_callback function address */
    void (*dmp_orient_callback)(uint8_t orientation);                                   /**< point to a dmp_orient_callback function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t mag_inited;                                                                 /**< magnetometer inited flag */
    uint8_t mag_asa[3];                                                                 /**< magnetometer asa */
    uint8_t iic_spi;                                                                    /**< iic spi interface type */
    uint8_t dmp_inited;                                                                 /**< dmp inited flag */
    uint16_t orient;                                                                    /**< orient */
    uint16_t mask;                                                                      /**< mask */
    uint8_t buf[1024];                                                                  /**< inner buffer */
} mpu9250_handle_t;

/**
 * @brief mpu9250 information structure definition
 */
typedef struct mpu9250_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} mpu9250_info_t;

/**
 * @}
 */

/**
 * @defgroup mpu9250_link_driver mpu9250 link driver function
 * @brief    mpu9250 link driver modules
 * @ingroup  mpu9250_driver
 * @{
 */

/**
 * @brief     initialize mpu9250_handle_t structure
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] STRUCTURE is mpu9250_handle_t
 * @note      none
 */
#define DRIVER_MPU9250_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_IIC_INIT(HANDLE, FUC)           (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_IIC_DEINIT(HANDLE, FUC)         (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_IIC_READ(HANDLE, FUC)           (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_IIC_WRITE(HANDLE, FUC)          (HANDLE)->iic_write = FUC

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_SPI_INIT(HANDLE, FUC)           (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_SPI_DEINIT(HANDLE, FUC)         (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to a spi_read function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_SPI_READ(HANDLE, FUC)           (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to a spi_write function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_SPI_WRITE(HANDLE, FUC)          (HANDLE)->spi_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_DELAY_MS(HANDLE, FUC)           (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_DEBUG_PRINT(HANDLE, FUC)        (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to an mpu9250 handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_MPU9250_LINK_RECEIVE_CALLBACK(HANDLE, FUC)   (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup mpu9250_basic_driver mpu9250 basic driver function
 * @brief    mpu9250 basic driver modules
 * @ingroup  mpu9250_driver
 * @{
 */

/**
 * @brief      get the chip's information
 * @param[out] *info points to an mpu9250 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mpu9250_info(mpu9250_info_t *info);

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t mpu9250_set_interface(mpu9250_handle_t *handle, mpu9250_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mpu9250_get_interface(mpu9250_handle_t *handle, mpu9250_interface_t *interface);

/**
 * @brief     set the chip address pin
 * @param[in] *handle points to an mpu9250 handle structure
 * @param[in] addr_pin is the chip address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t mpu9250_set_addr_pin(mpu9250_handle_t *handle, mpu9250_address_t addr_pin);

/**
 * @brief      get the chip address pin
 * @param[in]  *handle points to an mpu9250 handle structure
 * @param[out] *addr_pin points to a chip address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mpu9250_get_addr_pin(mpu9250_handle_t *handle, mpu9250_address_t *addr_pin);

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
uint8_t mpu9250_irq_handler(mpu9250_handle_t *handle);

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
uint8_t mpu9250_init(mpu9250_handle_t *handle);

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
uint8_t mpu9250_deinit(mpu9250_handle_t *handle);

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
                    );

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
uint8_t mpu9250_read_temperature(mpu9250_handle_t *handle, int16_t (*raw), float *degrees);

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
uint8_t mpu9250_set_fifo(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_fifo(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_force_fifo_reset(mpu9250_handle_t *handle);

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
uint8_t mpu9250_set_iic_master(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic_master(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_disable_iic_slave(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_disable_iic_slave(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_fifo_reset(mpu9250_handle_t *handle);

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
uint8_t mpu9250_get_fifo_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_iic_master_reset(mpu9250_handle_t *handle);

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
uint8_t mpu9250_get_iic_master_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_sensor_reset(mpu9250_handle_t *handle);

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
uint8_t mpu9250_get_sensor_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_device_reset(mpu9250_handle_t *handle);

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
uint8_t mpu9250_get_device_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_clock_source(mpu9250_handle_t *handle, mpu9250_clock_source_t clock_source);

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
uint8_t mpu9250_get_clock_source(mpu9250_handle_t *handle, mpu9250_clock_source_t *clock_source);

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
uint8_t mpu9250_set_ptat(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_ptat(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_cycle_wake_up(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_cycle_wake_up(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_sleep(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_sleep(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_gyro_standby(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_gyro_standby(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_standby_mode(mpu9250_handle_t *handle, mpu9250_source_t source, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_standby_mode(mpu9250_handle_t *handle, mpu9250_source_t source, mpu9250_bool_t *enable);

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
uint8_t mpu9250_get_fifo_count(mpu9250_handle_t *handle, uint16_t* count);

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
uint8_t mpu9250_fifo_get(mpu9250_handle_t *handle, uint8_t *buf, uint16_t len);

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
uint8_t mpu9250_fifo_set(mpu9250_handle_t *handle, uint8_t *buf, uint16_t len);

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
uint8_t mpu9250_set_signal_path_reset(mpu9250_handle_t *handle, mpu9250_signal_path_reset_t path);

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
uint8_t mpu9250_set_sample_rate_divider(mpu9250_handle_t *handle, uint8_t d);

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
uint8_t mpu9250_get_sample_rate_divider(mpu9250_handle_t *handle, uint8_t *d);

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
uint8_t mpu9250_set_extern_sync(mpu9250_handle_t *handle, mpu9250_extern_sync_t sync);

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
uint8_t mpu9250_get_extern_sync(mpu9250_handle_t *handle, mpu9250_extern_sync_t *sync);

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
uint8_t mpu9250_set_low_pass_filter(mpu9250_handle_t *handle, mpu9250_low_pass_filter_t filter);

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
uint8_t mpu9250_get_low_pass_filter(mpu9250_handle_t *handle, mpu9250_low_pass_filter_t *filter);

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
uint8_t mpu9250_set_fifo_mode(mpu9250_handle_t *handle, mpu9250_fifo_mode mode);

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
uint8_t mpu9250_get_fifo_mode(mpu9250_handle_t *handle, mpu9250_fifo_mode *mode);

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
uint8_t mpu9250_set_gyroscope_test(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_gyroscope_test(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_gyroscope_range(mpu9250_handle_t *handle, mpu9250_gyroscope_range_t range);

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
uint8_t mpu9250_get_gyroscope_range(mpu9250_handle_t *handle, mpu9250_gyroscope_range_t *range);

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
uint8_t mpu9250_set_gyroscope_choice(mpu9250_handle_t *handle, uint8_t choice);

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
uint8_t mpu9250_get_gyroscope_choice(mpu9250_handle_t *handle, uint8_t *choice);

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
uint8_t mpu9250_set_accelerometer_test(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_accelerometer_test(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_accelerometer_range(mpu9250_handle_t *handle, mpu9250_accelerometer_range_t range);

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
uint8_t mpu9250_get_accelerometer_range(mpu9250_handle_t *handle, mpu9250_accelerometer_range_t *range);

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
uint8_t mpu9250_set_fifo_1024kb(mpu9250_handle_t *handle);

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
uint8_t mpu9250_set_accelerometer_choice(mpu9250_handle_t *handle, uint8_t choice);

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
uint8_t mpu9250_get_accelerometer_choice(mpu9250_handle_t *handle, uint8_t *choice);

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
uint8_t mpu9250_set_accelerometer_low_pass_filter(mpu9250_handle_t *handle, mpu9250_accelerometer_low_pass_filter_t filter);

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
uint8_t mpu9250_get_accelerometer_low_pass_filter(mpu9250_handle_t *handle, mpu9250_accelerometer_low_pass_filter_t *filter);

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
uint8_t mpu9250_set_low_power_accel_output_rate(mpu9250_handle_t *handle, mpu9250_low_power_accel_output_rate_t rate);

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
uint8_t mpu9250_get_low_power_accel_output_rate(mpu9250_handle_t *handle, mpu9250_low_power_accel_output_rate_t *rate);

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
uint8_t mpu9250_set_wake_on_motion(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_wake_on_motion(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_accel_compare_with_previous_sample(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_accel_compare_with_previous_sample(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_accelerometer_x_offset(mpu9250_handle_t *handle, int16_t offset);

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
uint8_t mpu9250_get_accelerometer_x_offset(mpu9250_handle_t *handle, int16_t *offset);

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
uint8_t mpu9250_set_accelerometer_y_offset(mpu9250_handle_t *handle, int16_t offset);

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
uint8_t mpu9250_get_accelerometer_y_offset(mpu9250_handle_t *handle, int16_t *offset);

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
uint8_t mpu9250_set_accelerometer_z_offset(mpu9250_handle_t *handle, int16_t offset);

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
uint8_t mpu9250_get_accelerometer_z_offset(mpu9250_handle_t *handle, int16_t *offset);

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
uint8_t mpu9250_accelerometer_offset_convert_to_register(mpu9250_handle_t *handle, float mg, int16_t *reg);

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
uint8_t mpu9250_accelerometer_offset_convert_to_data(mpu9250_handle_t *handle, int16_t reg, float *mg);

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
uint8_t mpu9250_set_gyro_x_offset(mpu9250_handle_t *handle, int16_t offset);

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
uint8_t mpu9250_get_gyro_x_offset(mpu9250_handle_t *handle, int16_t *offset);

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
uint8_t mpu9250_set_gyro_y_offset(mpu9250_handle_t *handle, int16_t offset);

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
uint8_t mpu9250_get_gyro_y_offset(mpu9250_handle_t *handle, int16_t *offset);

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
uint8_t mpu9250_set_gyro_z_offset(mpu9250_handle_t *handle, int16_t offset);

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
uint8_t mpu9250_get_gyro_z_offset(mpu9250_handle_t *handle, int16_t *offset);

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
uint8_t mpu9250_gyro_offset_convert_to_register(mpu9250_handle_t *handle, float dps, int16_t *reg);

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
uint8_t mpu9250_gyro_offset_convert_to_data(mpu9250_handle_t *handle, int16_t reg, float *dps);

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
uint8_t mpu9250_set_fifo_enable(mpu9250_handle_t *handle, mpu9250_fifo_t fifo, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_fifo_enable(mpu9250_handle_t *handle, mpu9250_fifo_t fifo, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_interrupt_level(mpu9250_handle_t *handle, mpu9250_pin_level_t level);

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
uint8_t mpu9250_get_interrupt_level(mpu9250_handle_t *handle, mpu9250_pin_level_t *level);

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
uint8_t mpu9250_set_interrupt_pin_type(mpu9250_handle_t *handle, mpu9250_pin_type_t type);

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
uint8_t mpu9250_get_interrupt_pin_type(mpu9250_handle_t *handle, mpu9250_pin_type_t *type);

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
uint8_t mpu9250_set_interrupt_latch(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_interrupt_latch(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_interrupt_read_clear(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_interrupt_read_clear(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_fsync_interrupt_level(mpu9250_handle_t *handle, mpu9250_pin_level_t level);

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
uint8_t mpu9250_get_fsync_interrupt_level(mpu9250_handle_t *handle, mpu9250_pin_level_t *level);

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
uint8_t mpu9250_set_fsync_interrupt(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_fsync_interrupt(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_iic_bypass(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic_bypass(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_interrupt(mpu9250_handle_t *handle, mpu9250_interrupt_t type, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_interrupt(mpu9250_handle_t *handle, mpu9250_interrupt_t type, mpu9250_bool_t *enable);

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
uint8_t mpu9250_get_interrupt_status(mpu9250_handle_t *handle, uint8_t *status);

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
uint8_t mpu9250_set_gyroscope_x_test(mpu9250_handle_t *handle, uint8_t data);

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
uint8_t mpu9250_get_gyroscope_x_test(mpu9250_handle_t *handle, uint8_t *data);

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
uint8_t mpu9250_set_gyroscope_y_test(mpu9250_handle_t *handle, uint8_t data);

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
uint8_t mpu9250_get_gyroscope_y_test(mpu9250_handle_t *handle, uint8_t *data);

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
uint8_t mpu9250_set_gyroscope_z_test(mpu9250_handle_t *handle, uint8_t data);

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
uint8_t mpu9250_get_gyroscope_z_test(mpu9250_handle_t *handle, uint8_t *data);

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
uint8_t mpu9250_set_accelerometer_x_test(mpu9250_handle_t *handle, uint8_t data);

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
uint8_t mpu9250_get_accelerometer_x_test(mpu9250_handle_t *handle, uint8_t *data);

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
uint8_t mpu9250_set_accelerometer_y_test(mpu9250_handle_t *handle, uint8_t data);

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
uint8_t mpu9250_get_accelerometer_y_test(mpu9250_handle_t *handle, uint8_t *data);

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
uint8_t mpu9250_set_accelerometer_z_test(mpu9250_handle_t *handle, uint8_t data);

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
uint8_t mpu9250_get_accelerometer_z_test(mpu9250_handle_t *handle, uint8_t *data);

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
uint8_t mpu9250_set_motion_threshold(mpu9250_handle_t *handle, uint8_t threshold);

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
uint8_t mpu9250_get_motion_threshold(mpu9250_handle_t *handle, uint8_t *threshold);

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
uint8_t mpu9250_motion_threshold_convert_to_register(mpu9250_handle_t *handle, float mg, uint8_t *reg);

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
uint8_t mpu9250_motion_threshold_convert_to_data(mpu9250_handle_t *handle, uint8_t reg, float *mg);

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
uint8_t mpu9250_self_test(mpu9250_handle_t *handle, int32_t gyro_offset_raw[3], int32_t accel_offset_raw[3]);

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
uint8_t mpu9250_set_iic_clock(mpu9250_handle_t *handle, mpu9250_iic_clock_t clk);

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
uint8_t mpu9250_get_iic_clock(mpu9250_handle_t *handle, mpu9250_iic_clock_t *clk);

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
uint8_t mpu9250_set_iic_multi_master(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic_multi_master(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_iic_wait_for_external_sensor(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic_wait_for_external_sensor(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_iic_read_mode(mpu9250_handle_t *handle, mpu9250_iic_read_mode_t mode);

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
uint8_t mpu9250_get_iic_read_mode(mpu9250_handle_t *handle, mpu9250_iic_read_mode_t *mode);

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
uint8_t mpu9250_set_iic_fifo_enable(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic_fifo_enable(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_iic_mode(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_mode_t mode);

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
uint8_t mpu9250_get_iic_mode(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_mode_t *mode);

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
uint8_t mpu9250_set_iic_address(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t addr_7bit);

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
uint8_t mpu9250_get_iic_address(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t *addr_7bit);

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
uint8_t mpu9250_set_iic_register(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t reg);

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
uint8_t mpu9250_get_iic_register(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t *reg);

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
uint8_t mpu9250_set_iic_data_out(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t data);

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
uint8_t mpu9250_get_iic_data_out(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t *data);

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
uint8_t mpu9250_set_iic_enable(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic_enable(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_iic_byte_swap(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic_byte_swap(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_iic_transaction_mode(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_transaction_mode_t mode);

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
uint8_t mpu9250_get_iic_transaction_mode(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_transaction_mode_t *mode);

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
uint8_t mpu9250_set_iic_group_order(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_group_order_t order);

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
uint8_t mpu9250_get_iic_group_order(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, mpu9250_iic_group_order_t *order);

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
uint8_t mpu9250_set_iic_transferred_len(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t len);

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
uint8_t mpu9250_get_iic_transferred_len(mpu9250_handle_t *handle, mpu9250_iic_slave_t slave, uint8_t *len);

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
uint8_t mpu9250_get_iic_status(mpu9250_handle_t *handle, uint8_t *status);

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
uint8_t mpu9250_set_iic_delay_enable(mpu9250_handle_t *handle, mpu9250_iic_delay_t delay, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic_delay_enable(mpu9250_handle_t *handle, mpu9250_iic_delay_t delay, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_iic4_enable(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic4_enable(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_iic4_interrupt(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_get_iic4_interrupt(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_set_iic4_transaction_mode(mpu9250_handle_t *handle, mpu9250_iic4_transaction_mode_t mode);

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
uint8_t mpu9250_get_iic4_transaction_mode(mpu9250_handle_t *handle, mpu9250_iic4_transaction_mode_t *mode);

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
uint8_t mpu9250_set_iic_delay(mpu9250_handle_t *handle, uint8_t delay);

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
uint8_t mpu9250_get_iic_delay(mpu9250_handle_t *handle, uint8_t *delay);

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
uint8_t mpu9250_set_iic4_data_out(mpu9250_handle_t *handle, uint8_t data);

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
uint8_t mpu9250_get_iic4_data_out(mpu9250_handle_t *handle, uint8_t *data);

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
uint8_t mpu9250_set_iic4_data_in(mpu9250_handle_t *handle, uint8_t data);

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
uint8_t mpu9250_get_iic4_data_in(mpu9250_handle_t *handle, uint8_t *data);

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
uint8_t mpu9250_read_extern_sensor_data(mpu9250_handle_t *handle, uint8_t *data, uint8_t len);

/**
 * @}
 */

/**
 * @defgroup mpu9250_dmp_driver mpu9250 dmp driver function
 * @brief    mpu9250 dmp driver modules
 * @ingroup  mpu9250_driver
 * @{
 */

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
uint8_t mpu9250_dmp_load_firmware(mpu9250_handle_t *handle);

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
uint8_t mpu9250_dmp_set_pedometer_walk_time(mpu9250_handle_t *handle, uint32_t ms);

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
uint8_t mpu9250_dmp_get_pedometer_walk_time(mpu9250_handle_t *handle, uint32_t *ms);

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
uint8_t mpu9250_dmp_set_pedometer_step_count(mpu9250_handle_t *handle, uint32_t count);

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
uint8_t mpu9250_dmp_get_pedometer_step_count(mpu9250_handle_t *handle, uint32_t *count);

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
uint8_t mpu9250_dmp_set_shake_reject_timeout(mpu9250_handle_t *handle, uint16_t ms);

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
uint8_t mpu9250_dmp_get_shake_reject_timeout(mpu9250_handle_t *handle, uint16_t *ms);

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
uint8_t mpu9250_dmp_set_shake_reject_time(mpu9250_handle_t *handle, uint16_t ms);

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
uint8_t mpu9250_dmp_get_shake_reject_time(mpu9250_handle_t *handle, uint16_t *ms);

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
uint8_t mpu9250_dmp_set_shake_reject_thresh(mpu9250_handle_t *handle, uint16_t dps);

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
uint8_t mpu9250_dmp_get_shake_reject_thresh(mpu9250_handle_t *handle, uint16_t *dps);

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
uint8_t mpu9250_dmp_set_tap_time_multi(mpu9250_handle_t *handle, uint16_t ms);

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
uint8_t mpu9250_dmp_get_tap_time_multi(mpu9250_handle_t *handle, uint16_t *ms);

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
uint8_t mpu9250_dmp_set_tap_time(mpu9250_handle_t *handle, uint16_t ms);

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
uint8_t mpu9250_dmp_get_tap_time(mpu9250_handle_t *handle, uint16_t *ms);

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
uint8_t mpu9250_dmp_set_min_tap_count(mpu9250_handle_t *handle, uint8_t cnt);

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
uint8_t mpu9250_dmp_get_min_tap_count(mpu9250_handle_t *handle, uint8_t *cnt);

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
uint8_t mpu9250_dmp_set_gyro_calibrate(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_dmp_set_3x_quaternion(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_dmp_set_6x_quaternion(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_dmp_set_interrupt_mode(mpu9250_handle_t *handle, mpu9250_dmp_interrupt_mode_t mode);

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
uint8_t mpu9250_dmp_set_gyro_bias(mpu9250_handle_t *handle, int32_t bias[3]);

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
uint8_t mpu9250_dmp_set_accel_bias(mpu9250_handle_t *handle, int32_t bias[3]);

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
uint8_t mpu9250_dmp_set_orientation(mpu9250_handle_t *handle, int8_t mat[9]);

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
uint8_t mpu9250_dmp_set_feature(mpu9250_handle_t *handle, uint16_t mask);

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
uint8_t mpu9250_dmp_set_fifo_rate(mpu9250_handle_t *handle, uint16_t rate);

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
uint8_t mpu9250_dmp_get_fifo_rate(mpu9250_handle_t *handle, uint16_t *rate);

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
uint8_t mpu9250_dmp_set_tap_axes(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t enable);

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
uint8_t mpu9250_dmp_get_tap_axes(mpu9250_handle_t *handle, mpu9250_axis_t axis, mpu9250_bool_t *enable);

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
uint8_t mpu9250_dmp_set_tap_thresh(mpu9250_handle_t *handle, mpu9250_axis_t axis, uint16_t mg_ms);

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
uint8_t mpu9250_dmp_get_tap_thresh(mpu9250_handle_t *handle, mpu9250_axis_t axis, uint16_t *mg_ms);

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
                        );

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
uint8_t mpu9250_dmp_set_tap_callback(mpu9250_handle_t *handle, void (*callback)(uint8_t count, uint8_t direction));

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
uint8_t mpu9250_dmp_set_orient_callback(mpu9250_handle_t *handle, void (*callback)(uint8_t orientation));

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
uint8_t mpu9250_dmp_set_enable(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
                                                  int32_t gyro_offset[3], int32_t accel_offset[3]);

/**
 * @}
 */

/**
 * @defgroup mpu9250_magnetometer_driver mpu9250 magnetometer driver function
 * @brief    mpu9250 magnetometer driver modules
 * @ingroup  mpu9250_driver
 * @{
 */

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
uint8_t mpu9250_mag_init(mpu9250_handle_t *handle);

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
uint8_t mpu9250_mag_deinit(mpu9250_handle_t *handle);

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
uint8_t mpu9250_mag_read(mpu9250_handle_t *handle, int16_t mag_raw[3], float mag_ut[3]);

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
uint8_t mpu9250_mag_get_info(mpu9250_handle_t *handle, uint8_t *info);

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
uint8_t mpu9250_mag_get_status1(mpu9250_handle_t *handle, uint8_t *status);

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
uint8_t mpu9250_mag_get_status2(mpu9250_handle_t *handle, uint8_t *status);

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
uint8_t mpu9250_mag_set_mode(mpu9250_handle_t *handle, mpu9250_magnetometer_mode_t mode);

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
uint8_t mpu9250_mag_get_mode(mpu9250_handle_t *handle, mpu9250_magnetometer_mode_t *mode);

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
uint8_t mpu9250_mag_set_bits(mpu9250_handle_t *handle, mpu9250_magnetometer_bits_t bits);

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
uint8_t mpu9250_mag_get_bits(mpu9250_handle_t *handle, mpu9250_magnetometer_bits_t *bits);

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
uint8_t mpu9250_mag_set_reset(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_mag_get_reset(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_mag_set_self_test(mpu9250_handle_t *handle, mpu9250_bool_t enable);

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
uint8_t mpu9250_mag_get_self_test(mpu9250_handle_t *handle, mpu9250_bool_t *enable);

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
uint8_t mpu9250_mag_iic_disable(mpu9250_handle_t *handle);

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
uint8_t mpu9250_mag_get_asa(mpu9250_handle_t *handle, uint8_t asa[3]);

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
uint8_t mpu9250_mag_set_fifo_mode(mpu9250_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup mpu9250_extern_driver mpu9250 extern driver function
 * @brief    mpu9250 extern driver modules
 * @ingroup  mpu9250_driver
 * @{
 */

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
uint8_t mpu9250_set_reg(mpu9250_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

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
uint8_t mpu9250_get_reg(mpu9250_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
