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
 * @file      driver_mpu9250_basic.c
 * @brief     driver mpu9250 basic source file
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

#include "driver_mpu9250_basic.h"

static mpu9250_handle_t gs_handle;        /**< mpu9250 handle */

/**
 * @brief     basic example init
 * @param[in] interface used interface
 * @param[in] addr_pin iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      spi can't read magnetometer data
 */
uint8_t mpu9250_basic_init(mpu9250_interface_t interface, mpu9250_address_t addr_pin)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_MPU9250_LINK_INIT(&gs_handle, mpu9250_handle_t);
    DRIVER_MPU9250_LINK_IIC_INIT(&gs_handle, mpu9250_interface_iic_init);
    DRIVER_MPU9250_LINK_IIC_DEINIT(&gs_handle, mpu9250_interface_iic_deinit);
    DRIVER_MPU9250_LINK_IIC_READ(&gs_handle, mpu9250_interface_iic_read);
    DRIVER_MPU9250_LINK_IIC_WRITE(&gs_handle, mpu9250_interface_iic_write);
    DRIVER_MPU9250_LINK_SPI_INIT(&gs_handle, mpu9250_interface_spi_init);
    DRIVER_MPU9250_LINK_SPI_DEINIT(&gs_handle, mpu9250_interface_spi_deinit);
    DRIVER_MPU9250_LINK_SPI_READ(&gs_handle, mpu9250_interface_spi_read);
    DRIVER_MPU9250_LINK_SPI_WRITE(&gs_handle, mpu9250_interface_spi_write);
    DRIVER_MPU9250_LINK_DELAY_MS(&gs_handle, mpu9250_interface_delay_ms);
    DRIVER_MPU9250_LINK_DEBUG_PRINT(&gs_handle, mpu9250_interface_debug_print);
    DRIVER_MPU9250_LINK_RECEIVE_CALLBACK(&gs_handle, mpu9250_interface_receive_callback);
    
    /* set the interface */
    res = mpu9250_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interface failed.\n");
       
        return 1;
    }
    
    /* set the addr pin */
    res = mpu9250_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init */
    res = mpu9250_init(&gs_handle);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: init failed.\n");
       
        return 1;
    }
    
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    /* disable sleep */
    res = mpu9250_set_sleep(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set sleep failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* if spi interface, disable iic interface */
    if (interface == MPU9250_INTERFACE_SPI)
    {
        /* disable iic */
        res = mpu9250_set_disable_iic_slave(&gs_handle, MPU9250_BOOL_TRUE);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: set disable iic slave failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
    }
    
    /* set fifo 1024kb */
    res = mpu9250_set_fifo_1024kb(&gs_handle);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo 1024kb failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default clock source */
    res = mpu9250_set_clock_source(&gs_handle, MPU9250_BASIC_DEFAULT_CLOCK_SOURCE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set clock source failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default rate */
    res = mpu9250_set_sample_rate_divider(&gs_handle, (1000 / MPU9250_BASIC_DEFAULT_RATE) - 1);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set sample rate divider failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable temperature sensor */
    res = mpu9250_set_ptat(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set ptat failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default cycle wake up */
    res = mpu9250_set_cycle_wake_up(&gs_handle, MPU9250_BASIC_DEFAULT_CYCLE_WAKE_UP);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set cycle wake up failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable acc x */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_X, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable acc y */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Y, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable acc z */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Z, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable gyro x */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_X, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable gyro y */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Y, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable gyro z */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Z, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable gyroscope x test */
    res = mpu9250_set_gyroscope_test(&gs_handle, MPU9250_AXIS_X, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable gyroscope y test */
    res = mpu9250_set_gyroscope_test(&gs_handle, MPU9250_AXIS_Y, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable gyroscope z test */
    res = mpu9250_set_gyroscope_test(&gs_handle, MPU9250_AXIS_Z, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable accelerometer x test */
    res = mpu9250_set_accelerometer_test(&gs_handle, MPU9250_AXIS_X, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable accelerometer y test */
    res = mpu9250_set_accelerometer_test(&gs_handle, MPU9250_AXIS_Y, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable accelerometer z test */
    res = mpu9250_set_accelerometer_test(&gs_handle, MPU9250_AXIS_Z, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable fifo */
    res = mpu9250_set_fifo(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable temp fifo */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_TEMP, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable xg fifo */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_XG, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable yg fifo */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_YG, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable zg fifo */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_ZG, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable accel fifo */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_ACCEL, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default interrupt level */
    res = mpu9250_set_interrupt_level(&gs_handle, MPU9250_BASIC_DEFAULT_INTERRUPT_PIN_LEVEL);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default pin type */
    res = mpu9250_set_interrupt_pin_type(&gs_handle, MPU9250_BASIC_DEFAULT_INTERRUPT_PIN_TYPE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt pin type failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default motion interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_MOTION, MPU9250_BASIC_DEFAULT_INTERRUPT_MOTION);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default fifo overflow interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_FIFO_OVERFLOW, MPU9250_BASIC_DEFAULT_INTERRUPT_FIFO_OVERFLOW);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default dmp interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_DMP, MPU9250_BASIC_DEFAULT_INTERRUPT_DMP);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default fsync int interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_FSYNC_INT, MPU9250_BASIC_DEFAULT_INTERRUPT_FSYNC_INT);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default data ready interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_DATA_READY, MPU9250_BASIC_DEFAULT_INTERRUPT_DATA_READY);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default interrupt latch */
    res = mpu9250_set_interrupt_latch(&gs_handle, MPU9250_BASIC_DEFAULT_INTERRUPT_LATCH);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt latch failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default interrupt read clear */
    res = mpu9250_set_interrupt_read_clear(&gs_handle, MPU9250_BASIC_DEFAULT_INTERRUPT_READ_CLEAR);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt read clear failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the extern sync */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_BASIC_DEFAULT_EXTERN_SYNC);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default fsync interrupt */
    res = mpu9250_set_fsync_interrupt(&gs_handle, MPU9250_BASIC_DEFAULT_FSYNC_INTERRUPT);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fsync interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default fsync interrupt level */
    res = mpu9250_set_fsync_interrupt_level(&gs_handle, MPU9250_BASIC_DEFAULT_FSYNC_INTERRUPT_LEVEL);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fsync interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default iic master */
    res = mpu9250_set_iic_master(&gs_handle, MPU9250_BASIC_DEFAULT_IIC_MASTER);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default iic bypass */
    res = mpu9250_set_iic_bypass(&gs_handle, MPU9250_BASIC_DEFAULT_IIC_BYPASS);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic bypass failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default accelerometer range */
    res = mpu9250_set_accelerometer_range(&gs_handle, MPU9250_BASIC_DEFAULT_ACCELEROMETER_RANGE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default gyroscope range */
    res = mpu9250_set_gyroscope_range(&gs_handle, MPU9250_BASIC_DEFAULT_GYROSCOPE_RANGE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default gyro standby */
    res = mpu9250_set_gyro_standby(&gs_handle, MPU9250_BASIC_DEFAULT_GYROSCOPE_STANDBY);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyro standby failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default fifo mode */
    res = mpu9250_set_fifo_mode(&gs_handle, MPU9250_BASIC_DEFAULT_FIFO_MODE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default gyroscope choice */
    res = mpu9250_set_gyroscope_choice(&gs_handle, MPU9250_BASIC_DEFAULT_GYROSCOPE_CHOICE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope choice failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default low pass filter */
    res = mpu9250_set_low_pass_filter(&gs_handle, MPU9250_BASIC_DEFAULT_LOW_PASS_FILTER);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default accelerometer choice */
    res = mpu9250_set_accelerometer_choice(&gs_handle, MPU9250_BASIC_DEFAULT_ACCELEROMETER_CHOICE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer choice failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default accelerometer low pass filter */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_BASIC_DEFAULT_ACCELEROMETER_LOW_PASS_FILTER);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default low power accel output rate */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_BASIC_DEFAULT_LOW_POWER_ACCEL_OUTPUT_RATE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default wake on motion */
    res = mpu9250_set_wake_on_motion(&gs_handle, MPU9250_BASIC_DEFAULT_WAKE_ON_MOTION);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set wake on motion failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the default accel compare with previous sample */
    res = mpu9250_set_accel_compare_with_previous_sample(&gs_handle, MPU9250_BASIC_DEFAULT_ACCELEROMETER_COMPARE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accel compare with previous sample failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* if iic interface */
    if (interface == MPU9250_INTERFACE_IIC)
    {
        /* mag init */
        res = mpu9250_mag_init(&gs_handle); 
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag init failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        
        /* set the mag default mode */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_BASIC_DEFAULT_MAGNETOMETER_MODE);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle); 
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        
        /* set the mag default bits */
        res = mpu9250_mag_set_bits(&gs_handle, MPU9250_BASIC_DEFAULT_MAGNETOMETER_BITS);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set bits failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle); 
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
    }
    
    return 0;
}

/**
 * @brief      basic example read temperature
 * @param[out] *degrees pointer to a converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 * @note       none
 */
uint8_t mpu9250_basic_read_temperature(float *degrees)
{
    int16_t raw;
    
    /* read temperature */
    if (mpu9250_read_temperature(&gs_handle, &raw, degrees) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *g pointer to a converted data buffer
 * @param[out] *dps pointer to a converted data buffer
 * @param[out] *ut pointer to a converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mpu9250_basic_read(float g[3], float dps[3], float ut[3])
{
    uint16_t len;
    int16_t accel_raw[3];
    int16_t gyro_raw[3];
    int16_t mag_raw[3];
    float accel[3];
    float gyro[3];
    float mag[3];
    
    /* set 1 */
    len = 1;
    
    /* read data */
    if (mpu9250_read(&gs_handle,
                    (int16_t (*)[3])&accel_raw, (float (*)[3])&accel,
                    (int16_t (*)[3])&gyro_raw, (float (*)[3])&gyro,
                    (int16_t (*)[3])&mag_raw, (float (*)[3])&mag,
                     &len) != 0
                    )
    {
        return 1;
    }
    
    /* copy the data */
    g[0] = accel[0];
    g[1] = accel[1];
    g[2] = accel[2];
    dps[0] = gyro[0];
    dps[1] = gyro[1];
    dps[2] = gyro[2];
    ut[0] = mag[0];
    ut[1] = mag[1];
    ut[2] = mag[2];
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t mpu9250_basic_deinit(void)
{
    mpu9250_interface_t interface;
    
    /* get the interface */
    if (mpu9250_get_interface(&gs_handle, &interface) != 0)
    {
        return 1;
    }
    
    /* if interface iic*/
    if (interface == MPU9250_INTERFACE_IIC)
    {
        /* mag deinit */
        if (mpu9250_mag_deinit(&gs_handle) != 0)
        {
            return 1;
        }
    }
    
    /* deinit */
    if (mpu9250_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
