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
 * @file      driver_mpu9250_read_test.c
 * @brief     driver mpu9250 read test source file
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

#include "driver_mpu9250_read_test.h"

static mpu9250_handle_t gs_handle;        /**< mpu9250 handle */

/**
 * @brief     read test
 * @param[in] interface is the used interface
 * @param[in] addr is the iic device address
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t mpu9250_read_test(mpu9250_interface_t interface, mpu9250_address_t addr, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    mpu9250_info_t info;
    
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
    
    /* get information */
    res = mpu9250_info(&info);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        mpu9250_interface_debug_print("mpu9250: chip is %s.\n", info.chip_name);
        mpu9250_interface_debug_print("mpu9250: manufacturer is %s.\n", info.manufacturer_name);
        mpu9250_interface_debug_print("mpu9250: interface is %s.\n", info.interface);
        mpu9250_interface_debug_print("mpu9250: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        mpu9250_interface_debug_print("mpu9250: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        mpu9250_interface_debug_print("mpu9250: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        mpu9250_interface_debug_print("mpu9250: max current is %0.2fmA.\n", info.max_current_ma);
        mpu9250_interface_debug_print("mpu9250: max temperature is %0.1fC.\n", info.temperature_max);
        mpu9250_interface_debug_print("mpu9250: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    mpu9250_interface_debug_print("mpu9250: start read test.\n");
    
    /* set the interface */
    res = mpu9250_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interface failed.\n");
       
        return 1;
    }
    
    /* set the addr pin */
    res = mpu9250_set_addr_pin(&gs_handle, addr);
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
    
    /* set pll */
    res = mpu9250_set_clock_source(&gs_handle, MPU9250_CLOCK_SOURCE_PLL);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set clock source failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set 50Hz */
    res = mpu9250_set_sample_rate_divider(&gs_handle, (1000 / 50) - 1);
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
    
    /* disable cycle wake up */
    res = mpu9250_set_cycle_wake_up(&gs_handle, MPU9250_BOOL_FALSE);
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
    
    /* set interrupt level low */
    res = mpu9250_set_interrupt_level(&gs_handle, MPU9250_PIN_LEVEL_LOW);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* push-pull */
    res = mpu9250_set_interrupt_pin_type(&gs_handle, MPU9250_PIN_TYPE_PUSH_PULL);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt pin type failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable motion */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_MOTION, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable fifo overflow */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_FIFO_OVERFLOW, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable dmp interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_DMP, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable fsync int */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_FSYNC_INT, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable data ready */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_DATA_READY, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable latch */
    res = mpu9250_set_interrupt_latch(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt latch failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable interrupt read clear */
    res = mpu9250_set_interrupt_read_clear(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt read clear failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable sync input */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_EXTERN_SYNC_INPUT_DISABLED);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable fsync interrupt */
    res = mpu9250_set_fsync_interrupt(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fsync interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* fsync interrupt level low */
    res = mpu9250_set_fsync_interrupt_level(&gs_handle, MPU9250_PIN_LEVEL_LOW);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fsync interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable iic master */
    res = mpu9250_set_iic_master(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable iic bypass */
    res = mpu9250_set_iic_bypass(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic bypass failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable gyro standby */
    res = mpu9250_set_gyro_standby(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyro standby failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set the fifo normal mode */
    res = mpu9250_set_fifo_mode(&gs_handle, MPU9250_FIFO_MODE_NORMAL);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set gyroscope choice 0 */
    res = mpu9250_set_gyroscope_choice(&gs_handle, 0);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope choice failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set low pass filter 3 */
    res = mpu9250_set_low_pass_filter(&gs_handle, MPU9250_LOW_PASS_FILTER_3);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set accelerometer choice 0 */
    res = mpu9250_set_accelerometer_choice(&gs_handle, 0);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer choice failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set accelerometer low pass filter 3 */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_ACCELEROMETER_LOW_PASS_FILTER_3);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set low power accel output rate 62.5Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_62P50);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* disable wake on motion */
    res = mpu9250_set_wake_on_motion(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set wake on motion failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* enable accel compare with previous sample */
    res = mpu9250_set_accel_compare_with_previous_sample(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accel compare with previous sample failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* ±2g */
    res = mpu9250_set_accelerometer_range(&gs_handle, MPU9250_ACCELEROMETER_RANGE_2G);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    mpu9250_interface_debug_print("mpu9250: set accelerometer range 2g.\n");
    for (i = 0; i < times; i++)
    {
        int16_t accel_raw[1][3];
        float accel_g[1][3];
        int16_t gyro_raw[1][3];
        float gyro_dps[1][3];
        int16_t mag_raw[1][3];
        float mag_ut[1][3];
        uint16_t len;
        
        len = 1;
        res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: read failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        mpu9250_interface_debug_print("mpu9250: acc x is %0.2fg.\n", accel_g[0][0]);
        mpu9250_interface_debug_print("mpu9250: acc y is %0.2fg.\n", accel_g[0][1]);
        mpu9250_interface_debug_print("mpu9250: acc z is %0.2fg.\n", accel_g[0][2]);
        
        /* delay 1000 ms */
        mpu9250_interface_delay_ms(1000);
    }
    
    /* ±4g */
    res = mpu9250_set_accelerometer_range(&gs_handle, MPU9250_ACCELEROMETER_RANGE_4G);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    mpu9250_interface_debug_print("mpu9250: set accelerometer range 4g.\n");
    for (i = 0; i < times; i++)
    {
        int16_t accel_raw[1][3];
        float accel_g[1][3];
        int16_t gyro_raw[1][3];
        float gyro_dps[1][3];
        int16_t mag_raw[1][3];
        float mag_ut[1][3];
        uint16_t len;
        
        len = 1;
        res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: read failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        mpu9250_interface_debug_print("mpu9250: acc x is %0.2fg.\n", accel_g[0][0]);
        mpu9250_interface_debug_print("mpu9250: acc y is %0.2fg.\n", accel_g[0][1]);
        mpu9250_interface_debug_print("mpu9250: acc z is %0.2fg.\n", accel_g[0][2]);
        
        /* delay 1000 ms */
        mpu9250_interface_delay_ms(1000);
    }
    
    /* ±8g */
    res = mpu9250_set_accelerometer_range(&gs_handle, MPU9250_ACCELEROMETER_RANGE_8G);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    mpu9250_interface_debug_print("mpu9250: set accelerometer range 8g.\n");
    for (i = 0; i < times; i++)
    {
        int16_t accel_raw[1][3];
        float accel_g[1][3];
        int16_t gyro_raw[1][3];
        float gyro_dps[1][3];
        int16_t mag_raw[1][3];
        float mag_ut[1][3];
        uint16_t len;
        
        len = 1;
        res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: read failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        mpu9250_interface_debug_print("mpu9250: acc x is %0.2fg.\n", accel_g[0][0]);
        mpu9250_interface_debug_print("mpu9250: acc y is %0.2fg.\n", accel_g[0][1]);
        mpu9250_interface_debug_print("mpu9250: acc z is %0.2fg.\n", accel_g[0][2]);
        
        /* delay 1000 ms */
        mpu9250_interface_delay_ms(1000);
    }
    
    /* ±16g */
    res = mpu9250_set_accelerometer_range(&gs_handle, MPU9250_ACCELEROMETER_RANGE_16G);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    mpu9250_interface_debug_print("mpu9250: set accelerometer range 16g.\n");
    for (i = 0; i < times; i++)
    {
        int16_t accel_raw[1][3];
        float accel_g[1][3];
        int16_t gyro_raw[1][3];
        float gyro_dps[1][3];
        int16_t mag_raw[1][3];
        float mag_ut[1][3];
        uint16_t len;
        
        len = 1;
        res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: read failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        mpu9250_interface_debug_print("mpu9250: acc x is %0.2fg.\n", accel_g[0][0]);
        mpu9250_interface_debug_print("mpu9250: acc y is %0.2fg.\n", accel_g[0][1]);
        mpu9250_interface_debug_print("mpu9250: acc z is %0.2fg.\n", accel_g[0][2]);
        
        /* delay 1000 ms */
        mpu9250_interface_delay_ms(1000);
    }
    
    /* ±250dps */
    res = mpu9250_set_gyroscope_range(&gs_handle, MPU9250_GYROSCOPE_RANGE_250DPS);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    mpu9250_interface_debug_print("mpu9250: set gyroscope range 250dps.\n");
    for (i = 0; i < times; i++)
    {
        int16_t accel_raw[1][3];
        float accel_g[1][3];
        int16_t gyro_raw[1][3];
        float gyro_dps[1][3];
        int16_t mag_raw[1][3];
        float mag_ut[1][3];
        uint16_t len;
        
        len = 1;
        res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: read failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        mpu9250_interface_debug_print("mpu9250: gyro x is %0.2fdps.\n", gyro_dps[0][0]);
        mpu9250_interface_debug_print("mpu9250: gyro y is %0.2fdps.\n", gyro_dps[0][1]);
        mpu9250_interface_debug_print("mpu9250: gyro z is %0.2fdps.\n", gyro_dps[0][2]);
        
        /* delay 1000 ms */
        mpu9250_interface_delay_ms(1000);
    }
    
    /* ±500dps */
    res = mpu9250_set_gyroscope_range(&gs_handle, MPU9250_GYROSCOPE_RANGE_500DPS);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    mpu9250_interface_debug_print("mpu9250: set gyroscope range 500dps.\n");
    for (i = 0; i < times; i++)
    {
        int16_t accel_raw[1][3];
        float accel_g[1][3];
        int16_t gyro_raw[1][3];
        float gyro_dps[1][3];
        int16_t mag_raw[1][3];
        float mag_ut[1][3];
        uint16_t len;
        
        len = 1;
        res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: read failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        mpu9250_interface_debug_print("mpu9250: gyro x is %0.2fdps.\n", gyro_dps[0][0]);
        mpu9250_interface_debug_print("mpu9250: gyro y is %0.2fdps.\n", gyro_dps[0][1]);
        mpu9250_interface_debug_print("mpu9250: gyro z is %0.2fdps.\n", gyro_dps[0][2]);
        
        /* delay 1000 ms */
        mpu9250_interface_delay_ms(1000);
    }
    
    /* ±1000dps */
    res = mpu9250_set_gyroscope_range(&gs_handle, MPU9250_GYROSCOPE_RANGE_1000DPS);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    mpu9250_interface_debug_print("mpu9250: set gyroscope range 1000dps.\n");
    for (i = 0; i < times; i++)
    {
        int16_t accel_raw[1][3];
        float accel_g[1][3];
        int16_t gyro_raw[1][3];
        float gyro_dps[1][3];
        int16_t mag_raw[1][3];
        float mag_ut[1][3];
        uint16_t len;
        
        len = 1;
        res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: read failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        mpu9250_interface_debug_print("mpu9250: gyro x is %0.2fdps.\n", gyro_dps[0][0]);
        mpu9250_interface_debug_print("mpu9250: gyro y is %0.2fdps.\n", gyro_dps[0][1]);
        mpu9250_interface_debug_print("mpu9250: gyro z is %0.2fdps.\n", gyro_dps[0][2]);
        
        /* delay 1000 ms */
        mpu9250_interface_delay_ms(1000);
    }
    
    /* ±2000dps */
    res = mpu9250_set_gyroscope_range(&gs_handle, MPU9250_GYROSCOPE_RANGE_2000DPS);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    mpu9250_interface_debug_print("mpu9250: set gyroscope range 2000dps.\n");
    for (i = 0; i < times; i++)
    {
        int16_t accel_raw[1][3];
        float accel_g[1][3];
        int16_t gyro_raw[1][3];
        float gyro_dps[1][3];
        int16_t mag_raw[1][3];
        float mag_ut[1][3];
        uint16_t len;
        
        len = 1;
        res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: read failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        mpu9250_interface_debug_print("mpu9250: gyro x is %0.2fdps.\n", gyro_dps[0][0]);
        mpu9250_interface_debug_print("mpu9250: gyro y is %0.2fdps.\n", gyro_dps[0][1]);
        mpu9250_interface_debug_print("mpu9250: gyro z is %0.2fdps.\n", gyro_dps[0][2]);
        
        /* delay 1000 ms */
        mpu9250_interface_delay_ms(1000);
    }
    
    mpu9250_interface_debug_print("mpu9250: read temperature.\n");
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        float degrees;
        
        /* read temperature */
        res = mpu9250_read_temperature(&gs_handle, &raw, &degrees);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: read temperature failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        mpu9250_interface_debug_print("mpu9250: temperature %0.2fC.\n", degrees);
        
        /* delay 1000 ms */
        mpu9250_interface_delay_ms(1000);
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
        
        /* 100Hz */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_CONTINUOUS2);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle); 
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        
        /* read magnetometer */
        mpu9250_interface_debug_print("mpu9250: read magnetometer.\n");
        
        /* set magnetometer 14 bits */
        mpu9250_interface_debug_print("mpu9250: set magnetometer 14 bits.\n");
        
        /* set 14 bits */
        res = mpu9250_mag_set_bits(&gs_handle, MPU9250_MAGNETOMETER_BITS_14);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set bits failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle); 
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        
        for (i = 0; i < times; i++)
        {
            int16_t accel_raw[1][3];
            float accel_g[1][3];
            int16_t gyro_raw[1][3];
            float gyro_dps[1][3];
            int16_t mag_raw[1][3];
            float mag_ut[1][3];
            uint16_t len;
            
            len = 1;
            res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
            if (res != 0)
            {
                mpu9250_interface_debug_print("mpu9250: read failed.\n");
                (void)mpu9250_mag_deinit(&gs_handle); 
                (void)mpu9250_deinit(&gs_handle);
               
                return 1;
            }
            mpu9250_interface_debug_print("mpu9250: mag x is %0.2fuT.\n", mag_ut[0][0]);
            mpu9250_interface_debug_print("mpu9250: mag y is %0.2fuT.\n", mag_ut[0][1]);
            mpu9250_interface_debug_print("mpu9250: mag z is %0.2fuT.\n", mag_ut[0][2]);
            
            /* delay 1000 ms */
            mpu9250_interface_delay_ms(1000);
        }
        
        /* set magnetometer 16 bits */
        mpu9250_interface_debug_print("mpu9250: set magnetometer 16 bits.\n");
        
        /* set 16 bits */
        res = mpu9250_mag_set_bits(&gs_handle, MPU9250_MAGNETOMETER_BITS_16);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set bits failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle); 
            (void)mpu9250_deinit(&gs_handle);
           
            return 1;
        }
        
        for (i = 0; i < times; i++)
        {
            int16_t accel_raw[1][3];
            float accel_g[1][3];
            int16_t gyro_raw[1][3];
            float gyro_dps[1][3];
            int16_t mag_raw[1][3];
            float mag_ut[1][3];
            uint16_t len;
            
            len = 1;
            res = mpu9250_read(&gs_handle, accel_raw, accel_g, gyro_raw, gyro_dps, mag_raw, mag_ut, &len);
            if (res != 0)
            {
                mpu9250_interface_debug_print("mpu9250: read failed.\n");
                (void)mpu9250_mag_deinit(&gs_handle); 
                (void)mpu9250_deinit(&gs_handle);
               
                return 1;
            }
            mpu9250_interface_debug_print("mpu9250: mag x is %0.2fuT.\n", mag_ut[0][0]);
            mpu9250_interface_debug_print("mpu9250: mag y is %0.2fuT.\n", mag_ut[0][1]);
            mpu9250_interface_debug_print("mpu9250: mag z is %0.2fuT.\n", mag_ut[0][2]);
            
            /* delay 1000 ms */
            mpu9250_interface_delay_ms(1000);
        }
        
        /* mpu9250 mag read test */
        mpu9250_interface_debug_print("mpu9250: mpu9250 mag read test.\n");
        
        for (i = 0; i < times; i++)
        {
            int16_t mag_raw[3];
            float mag_ut[3];
            
            res = mpu9250_mag_read(&gs_handle, mag_raw, mag_ut);
            if (res != 0)
            {
                mpu9250_interface_debug_print("mpu9250: mag read failed.\n");
                (void)mpu9250_mag_deinit(&gs_handle); 
                (void)mpu9250_deinit(&gs_handle);
               
                return 1;
            }
            mpu9250_interface_debug_print("mpu9250: mag x is %0.2fuT.\n", mag_ut[0]);
            mpu9250_interface_debug_print("mpu9250: mag y is %0.2fuT.\n", mag_ut[1]);
            mpu9250_interface_debug_print("mpu9250: mag z is %0.2fuT.\n", mag_ut[2]);
            
            /* delay 1000 ms */
            mpu9250_interface_delay_ms(1000);
        }
        
        /* mag deinit */
        (void)mpu9250_mag_deinit(&gs_handle); 
    }
    
    /* finish read test */
    mpu9250_interface_debug_print("mpu9250: finish read test.\n");
    (void)mpu9250_deinit(&gs_handle);
    
    return 0;
}
