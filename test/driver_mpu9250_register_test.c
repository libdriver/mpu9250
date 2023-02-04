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
 * @file      driver_mpu9250_register_test.c
 * @brief     driver mpu9250 register test source file
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

#include "driver_mpu9250_register_test.h"
#include <stdlib.h>

static mpu9250_handle_t gs_handle;        /**< mpu9250 handle */

/**
 * @brief     register test
 * @param[in] interface is the used interface
 * @param[in] addr is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t mpu9250_register_test(mpu9250_interface_t interface, mpu9250_address_t addr)
{
    uint8_t res;
    uint8_t reg;
    uint8_t i;
    uint8_t div_in;
    uint8_t div_check;
    uint8_t data[8];
    uint8_t data_check[8];
    uint16_t cnt;
    uint8_t status;
    uint8_t test;
    uint8_t test_check;
    int16_t offset;
    int16_t offset_check;
    float test_f;
    float test_check_f;
    mpu9250_info_t info;
    mpu9250_address_t addr_pin;
    mpu9250_interface_t interface_check;
    mpu9250_bool_t enable;
    mpu9250_clock_source_t clock_source;
    mpu9250_extern_sync_t sync;
    mpu9250_low_pass_filter_t filter;
    mpu9250_gyroscope_range_t range_g;
    mpu9250_accelerometer_range_t range_a;
    mpu9250_pin_level_t level;
    mpu9250_pin_type_t pin;
    mpu9250_iic_clock_t clk;
    mpu9250_iic_read_mode_t read_mode;
    mpu9250_iic4_transaction_mode_t transaction_mode;
    mpu9250_iic_transaction_mode_t tran_mode;
    mpu9250_iic_mode_t mode;
    mpu9250_iic_group_order_t order;
    mpu9250_fifo_mode fifo_mode;
    mpu9250_accelerometer_low_pass_filter_t accelerometer_filter;
    mpu9250_low_power_accel_output_rate_t rate;
    mpu9250_magnetometer_mode_t mag_mode;
    mpu9250_magnetometer_bits_t bits;
    
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
    
    /* start register test */
    mpu9250_interface_debug_print("mpu9250: start register test.\n");
    
    /* mpu9250_set_interface/mpu9250_get_interface test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_interface/mpu9250_get_interface test.\n");
    
    /* iic */
    res = mpu9250_set_interface(&gs_handle, MPU9250_INTERFACE_IIC);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interface failed.\n");
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set interface iic.\n");
    res = mpu9250_get_interface(&gs_handle, &interface_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interface failed.\n");
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interface %s.\n", interface_check == MPU9250_INTERFACE_IIC ? "ok" : "error");
    
    /* spi */
    res = mpu9250_set_interface(&gs_handle, MPU9250_INTERFACE_SPI);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interface failed.\n");
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set interface spi.\n");
    res = mpu9250_get_interface(&gs_handle, &interface_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interface failed.\n");
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interface %s.\n", interface_check == MPU9250_INTERFACE_SPI ? "ok" : "error");
    
    /* mpu9250_set_addr_pin/mpu9250_get_addr_pin test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_addr_pin/mpu9250_get_addr_pin test.\n");
    
    /* set low */
    res = mpu9250_set_addr_pin(&gs_handle, MPU9250_ADDRESS_AD0_LOW);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set addr pin failed.\n");
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set addr pin low.\n");
    res = mpu9250_get_addr_pin(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get addr pin failed.\n");
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check addr pin %s.\n", addr_pin == MPU9250_ADDRESS_AD0_LOW ? "ok" : "error");
    
    /* set high */
    res = mpu9250_set_addr_pin(&gs_handle, MPU9250_ADDRESS_AD0_HIGH);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set addr pin failed.\n");
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set addr pin high.\n");
    res = mpu9250_get_addr_pin(&gs_handle, &addr_pin);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get addr pin failed.\n");
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check addr pin %s.\n", addr_pin == MPU9250_ADDRESS_AD0_HIGH ? "ok" : "error");
    
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
    
    /* mpu9250_set_fifo/mpu9250_get_fifo test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_fifo/mpu9250_get_fifo test.\n");
    
    /* enable fifo */
    res = mpu9250_set_fifo(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable fifo.\n");
    
    res = mpu9250_get_fifo(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable fifo */
    res = mpu9250_set_fifo(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable fifo.\n");
    res = mpu9250_get_fifo(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic_master/mpu9250_get_iic_master test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_master/mpu9250_get_iic_master test.\n");
    
    /* enable iic master */
    res = mpu9250_set_iic_master(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic master.\n");
    res = mpu9250_get_iic_master(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic master %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable iic master */
    res = mpu9250_set_iic_master(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic master.\n");
    res = mpu9250_get_iic_master(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic master %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_fifo_reset/mpu9250_get_fifo_reset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_fifo_reset/mpu9250_get_fifo_reset test.\n");
    
    /* fifo reset */
    res = mpu9250_fifo_reset(&gs_handle);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: fifo reset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: fifo reset.\n");
    res = mpu9250_get_fifo_reset(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo reset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo reset %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_iic_master_reset/mpu9250_get_iic_master_reset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_iic_master_reset/mpu9250_get_iic_master_reset test.\n");
    
    /* reset the iic master */
    res = mpu9250_iic_master_reset(&gs_handle);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: iic master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: iic master reset.\n");
    res = mpu9250_get_iic_master_reset(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic master %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_sensor_reset/mpu9250_get_sensor_reset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_sensor_reset/mpu9250_get_sensor_reset test.\n");
    
    /* sensor reset */
    res = mpu9250_sensor_reset(&gs_handle);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: sensor reset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: sensor reset.\n");
    res = mpu9250_get_sensor_reset(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get sensor reset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check sensor reset %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_device_reset/mpu9250_get_device_reset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_device_reset/mpu9250_get_device_reset test.\n");
    
    /* device reset */
    res = mpu9250_device_reset(&gs_handle);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: device reset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: device reset.\n");
    res = mpu9250_get_device_reset(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get device reset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check device reset %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
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
    
    /* mpu9250_set_clock_source/mpu9250_get_clock_source test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_clock_source/mpu9250_get_clock_source test.\n");
    
    /* stop the clock */
    res = mpu9250_set_clock_source(&gs_handle, MPU9250_CLOCK_SOURCE_STOP_CLOCK);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set clock source failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: stop the clock.\n");
    res = mpu9250_get_clock_source(&gs_handle, &clock_source);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get clock source failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check clock source %s.\n", clock_source == MPU9250_CLOCK_SOURCE_STOP_CLOCK ? "ok" : "error");
    
    /* internal 20.0 MHz */
    res = mpu9250_set_clock_source(&gs_handle, MPU9250_CLOCK_SOURCE_INTERNAL_20MHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set clock source failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set the clock source extern 20.0 MHz.\n");
    res = mpu9250_get_clock_source(&gs_handle, &clock_source);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get clock source failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check clock source %s.\n", clock_source == MPU9250_CLOCK_SOURCE_INTERNAL_20MHZ ? "ok" : "error");
    
    /* pll */
    res = mpu9250_set_clock_source(&gs_handle, MPU9250_CLOCK_SOURCE_PLL);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set clock source failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set the clock source pll.\n");
    res = mpu9250_get_clock_source(&gs_handle, &clock_source);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get clock source failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check clock source %s.\n", clock_source == MPU9250_CLOCK_SOURCE_PLL ? "ok" : "error");
    
    /* mpu9250_set_ptat/mpu9250_get_ptat test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_ptat/mpu9250_get_ptat test.\n");
    
    /* enable temperature sensor */
    res = mpu9250_set_ptat(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set ptat failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable temperature sensor.\n");
    res = mpu9250_get_ptat(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get ptat failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check ptat %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable temperature sensor */
    res = mpu9250_set_ptat(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set ptat failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable temperature sensor.\n");
    res = mpu9250_get_ptat(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get ptat failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check ptat %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_cycle_wake_up/mpu9250_get_cycle_wake_up test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_cycle_wake_up/mpu9250_get_cycle_wake_up test.\n");
    
    /* enable cycle wake up */
    res = mpu9250_set_cycle_wake_up(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set cycle wake up failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable cycle wake up.\n");
    res = mpu9250_get_cycle_wake_up(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get cycle wake up failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check cycle wake up %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable cycle wake up */
    res = mpu9250_set_cycle_wake_up(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set cycle wake up failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable cycle wake up.\n");
    res = mpu9250_get_cycle_wake_up(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get cycle wake up failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check cycle wake up %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_sleep/mpu9250_get_sleep test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_sleep/mpu9250_get_sleep test.\n");
    
    /* enable the sleep */
    res = mpu9250_set_sleep(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set sleep failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable the sleep.\n");
    res = mpu9250_get_sleep(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get sleep failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check sleep %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable the sleep */
    res = mpu9250_set_sleep(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set sleep failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable the sleep.\n");
    res = mpu9250_get_sleep(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get sleep failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check sleep %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_standby_mode/mpu9250_get_standby_mode test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_standby_mode/mpu9250_get_standby_mode test.\n");
    
    /* enable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_X, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable acc x standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_X, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_X, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable acc x standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_X, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Y, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable acc y standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Y, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Y, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable acc y standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Y, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Z, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable acc z standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Z, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Z, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable acc z standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_ACC_Z, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_X, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable gyro x standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_X, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_X, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable gyro x standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_X, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Y, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable gyro y standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Y, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Y, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable gyro y standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Y, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Z, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable gyro z standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Z, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable standby mode */
    res = mpu9250_set_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Z, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable gyro z standby mode.\n");
    res = mpu9250_get_standby_mode(&gs_handle, MPU9250_SOURCE_GYRO_Z, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get standby mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check standby mode %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_fifo_get/mpu9250_fifo_set test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_fifo_get/mpu9250_fifo_set test.\n");
    
    for (i = 0; i < 8; i++)
    {
        data[i] = (uint8_t)(rand() % 256);
    }
    /* fifo set */
    res = mpu9250_fifo_set(&gs_handle, data, 8);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: fifo write failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo write %s.\n", (res == 0) ? "ok" : "error");
    
    /* fifo get */
    res = mpu9250_fifo_get(&gs_handle, data_check, 8);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: fifo read failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo read %s.\n", (res == 0) ? "ok" : "error");
    
    /* mpu9250_get_fifo_count test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_get_fifo_count test.\n");
    
    /* fifo count */
    res = mpu9250_get_fifo_count(&gs_handle, &cnt);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: fifo count failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: fifo count %d.\n", cnt);
    
    /* mpu9250_set_signal_path_reset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_signal_path_reset test.\n");
    
    /* temp reset */
    res = mpu9250_set_signal_path_reset(&gs_handle, MPU9250_SIGNAL_PATH_RESET_TEMP);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set signal path reset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: temp signal path reset.\n");
    mpu9250_interface_debug_print("mpu9250: check signal path reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* accel reset */
    res = mpu9250_set_signal_path_reset(&gs_handle, MPU9250_SIGNAL_PATH_RESET_ACCEL);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set signal path reset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: accel signal path reset.\n");
    mpu9250_interface_debug_print("mpu9250: check signal path reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* gyro reset */
    res = mpu9250_set_signal_path_reset(&gs_handle, MPU9250_SIGNAL_PATH_RESET_GYRO);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set signal path reset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: gyro signal path reset.\n");
    mpu9250_interface_debug_print("mpu9250: check signal path reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* mpu9250_set_sample_rate_divider/mpu9250_get_sample_rate_divider test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_sample_rate_divider/mpu9250_get_sample_rate_divider test.\n");
    
    div_in = (uint8_t)(rand() % 256);
    res = mpu9250_set_sample_rate_divider(&gs_handle, div_in);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set sample rate divider failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set sample rate divider 0x%02X.\n", div);
    res = mpu9250_get_sample_rate_divider(&gs_handle, &div_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get sample rate divider failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check sample rate divider %s.\n", (div_in == div_check) ? "ok" : "error");
    
    /* mpu9250_set_extern_sync/mpu9250_get_extern_sync test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_extern_sync/mpu9250_get_extern_sync test.\n");
    
    /* input disabled */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_EXTERN_SYNC_INPUT_DISABLED);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set extern sync input disabled.\n");
    res = mpu9250_get_extern_sync(&gs_handle, &sync);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check extern sync %s.\n", (sync == MPU9250_EXTERN_SYNC_INPUT_DISABLED) ? "ok" : "error");
    
    /* temp out low */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_EXTERN_SYNC_TEMP_OUT_L);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set extern sync temp out low.\n");
    res = mpu9250_get_extern_sync(&gs_handle, &sync);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check extern sync %s.\n", (sync == MPU9250_EXTERN_SYNC_TEMP_OUT_L) ? "ok" : "error");
    
    /* gyro xout low */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_EXTERN_SYNC_GYRO_XOUT_L);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set extern sync gyro xout low.\n");
    res = mpu9250_get_extern_sync(&gs_handle, &sync);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check extern sync %s.\n", (sync == MPU9250_EXTERN_SYNC_GYRO_XOUT_L) ? "ok" : "error");
    
    /* gyro yout low */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_EXTERN_SYNC_GYRO_YOUT_L);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set extern sync gyro yout low.\n");
    res = mpu9250_get_extern_sync(&gs_handle, &sync);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check extern sync %s.\n", (sync == MPU9250_EXTERN_SYNC_GYRO_YOUT_L) ? "ok" : "error");
    
    /* gyro zout low */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_EXTERN_SYNC_GYRO_ZOUT_L);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set extern sync gyro zout low.\n");
    res = mpu9250_get_extern_sync(&gs_handle, &sync);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check extern sync %s.\n", (sync == MPU9250_EXTERN_SYNC_GYRO_ZOUT_L) ? "ok" : "error");
    
    /* accel xout low */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_EXTERN_SYNC_ACCEL_XOUT_L);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set extern sync accel xout low.\n");
    res = mpu9250_get_extern_sync(&gs_handle, &sync);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check extern sync %s.\n", (sync == MPU9250_EXTERN_SYNC_ACCEL_XOUT_L) ? "ok" : "error");
    
    /* accel yout low */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_EXTERN_SYNC_ACCEL_YOUT_L);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set extern sync accel yout low.\n");
    res = mpu9250_get_extern_sync(&gs_handle, &sync);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check extern sync %s.\n", (sync == MPU9250_EXTERN_SYNC_ACCEL_YOUT_L) ? "ok" : "error");
    
    /* accel zout low */
    res = mpu9250_set_extern_sync(&gs_handle, MPU9250_EXTERN_SYNC_ACCEL_ZOUT_L);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set extern sync accel zout low.\n");
    res = mpu9250_get_extern_sync(&gs_handle, &sync);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get extern sync failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check extern sync %s.\n", (sync == MPU9250_EXTERN_SYNC_ACCEL_ZOUT_L) ? "ok" : "error");
    
    /* mpu9250_set_low_pass_filter/mpu9250_get_low_pass_filter test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_low_pass_filter/mpu9250_get_low_pass_filter test.\n");
    
    /* filter 0 */
    res = mpu9250_set_low_pass_filter(&gs_handle, MPU9250_LOW_PASS_FILTER_0);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set low pass filter 0.\n");
    res = mpu9250_get_low_pass_filter(&gs_handle, &filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check low pass filter %s.\n", (filter == MPU9250_LOW_PASS_FILTER_0) ? "ok" : "error");
    
    /* filter 1 */
    res = mpu9250_set_low_pass_filter(&gs_handle, MPU9250_LOW_PASS_FILTER_1);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set low pass filter 1.\n");
    res = mpu9250_get_low_pass_filter(&gs_handle, &filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check low pass filter %s.\n", (filter == MPU9250_LOW_PASS_FILTER_1) ? "ok" : "error");
    
    /* filter 2 */
    res = mpu9250_set_low_pass_filter(&gs_handle, MPU9250_LOW_PASS_FILTER_2);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set low pass filter 2.\n");
    res = mpu9250_get_low_pass_filter(&gs_handle, &filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check low pass filter %s.\n", (filter == MPU9250_LOW_PASS_FILTER_2) ? "ok" : "error");
    
    /* filter 3 */
    res = mpu9250_set_low_pass_filter(&gs_handle, MPU9250_LOW_PASS_FILTER_3);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set low pass filter 3.\n");
    res = mpu9250_get_low_pass_filter(&gs_handle, &filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check low pass filter %s.\n", (filter == MPU9250_LOW_PASS_FILTER_3) ? "ok" : "error");
    
    /* filter 4 */
    res = mpu9250_set_low_pass_filter(&gs_handle, MPU9250_LOW_PASS_FILTER_4);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set low pass filter 4.\n");
    res = mpu9250_get_low_pass_filter(&gs_handle, &filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check low pass filter %s.\n", (filter == MPU9250_LOW_PASS_FILTER_4) ? "ok" : "error");
    
    /* filter 5 */
    res = mpu9250_set_low_pass_filter(&gs_handle, MPU9250_LOW_PASS_FILTER_5);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set low pass filter 5.\n");
    res = mpu9250_get_low_pass_filter(&gs_handle, &filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check low pass filter %s.\n", (filter == MPU9250_LOW_PASS_FILTER_5) ? "ok" : "error");
    
    /* filter 6 */
    res = mpu9250_set_low_pass_filter(&gs_handle, MPU9250_LOW_PASS_FILTER_6);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set low pass filter 6.\n");
    res = mpu9250_get_low_pass_filter(&gs_handle, &filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check low pass filter %s.\n", (filter == MPU9250_LOW_PASS_FILTER_6) ? "ok" : "error");
    
    /* mpu9250_set_gyroscope_test/mpu9250_get_gyroscope_test test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyroscope_test/mpu9250_get_gyroscope_test test.\n");
    
    /* enable axis x */
    res = mpu9250_set_gyroscope_test(&gs_handle, MPU9250_AXIS_X, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable axis x.\n");
    res = mpu9250_get_gyroscope_test(&gs_handle, MPU9250_AXIS_X, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope test %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable axis x */
    res = mpu9250_set_gyroscope_test(&gs_handle, MPU9250_AXIS_X, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable axis x.\n");
    res = mpu9250_get_gyroscope_test(&gs_handle, MPU9250_AXIS_X, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope test %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable axis y */
    res = mpu9250_set_gyroscope_test(&gs_handle, MPU9250_AXIS_Y, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable axis y.\n");
    res = mpu9250_get_gyroscope_test(&gs_handle, MPU9250_AXIS_Y, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope test %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable axis y */
    res = mpu9250_set_gyroscope_test(&gs_handle, MPU9250_AXIS_Y, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable axis y.\n");
    res = mpu9250_get_gyroscope_test(&gs_handle, MPU9250_AXIS_Y, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope test %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable axis z */
    res = mpu9250_set_gyroscope_test(&gs_handle, MPU9250_AXIS_Z, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable axis z.\n");
    res = mpu9250_get_gyroscope_test(&gs_handle, MPU9250_AXIS_Z, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope test %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable axis z */
    res = mpu9250_set_gyroscope_test(&gs_handle, MPU9250_AXIS_Z, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable axis z.\n");
    res = mpu9250_get_gyroscope_test(&gs_handle, MPU9250_AXIS_Z, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope test %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_gyroscope_range/mpu9250_get_gyroscope_range test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyroscope_range/mpu9250_get_gyroscope_range test.\n");
    
    /* 250 dps */
    res = mpu9250_set_gyroscope_range(&gs_handle, MPU9250_GYROSCOPE_RANGE_250DPS);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set gyroscope range 250 dps.\n");
    res = mpu9250_get_gyroscope_range(&gs_handle, &range_g);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope range %s.\n", range_g == MPU9250_GYROSCOPE_RANGE_250DPS ? "ok" : "error");
    
    /* 500 dps */
    res = mpu9250_set_gyroscope_range(&gs_handle, MPU9250_GYROSCOPE_RANGE_500DPS);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set gyroscope range 500 dps.\n");
    res = mpu9250_get_gyroscope_range(&gs_handle, &range_g);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope range %s.\n", range_g == MPU9250_GYROSCOPE_RANGE_500DPS ? "ok" : "error");
    
    /* 1000 dps */
    res = mpu9250_set_gyroscope_range(&gs_handle, MPU9250_GYROSCOPE_RANGE_1000DPS);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set gyroscope range 1000 dps.\n");
    res = mpu9250_get_gyroscope_range(&gs_handle, &range_g);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope range %s.\n", range_g == MPU9250_GYROSCOPE_RANGE_1000DPS ? "ok" : "error");
    
    /* 2000 dps */
    res = mpu9250_set_gyroscope_range(&gs_handle, MPU9250_GYROSCOPE_RANGE_2000DPS);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set gyroscope range 2000 dps.\n");
    res = mpu9250_get_gyroscope_range(&gs_handle, &range_g);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope range %s.\n", range_g == MPU9250_GYROSCOPE_RANGE_2000DPS ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_test/mpu9250_get_accelerometer_test test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_test/mpu9250_get_accelerometer_test test.\n");
    
    /* enable accelerometer x */
    res = mpu9250_set_accelerometer_test(&gs_handle, MPU9250_AXIS_X, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable accelerometer x.\n");
    res = mpu9250_get_accelerometer_test(&gs_handle, MPU9250_AXIS_X, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer test %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable accelerometer x */
    res = mpu9250_set_accelerometer_test(&gs_handle, MPU9250_AXIS_X, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable accelerometer x.\n");
    res = mpu9250_get_accelerometer_test(&gs_handle, MPU9250_AXIS_X, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer test %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable accelerometer y */
    res = mpu9250_set_accelerometer_test(&gs_handle, MPU9250_AXIS_Y, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable accelerometer y.\n");
    res = mpu9250_get_accelerometer_test(&gs_handle, MPU9250_AXIS_Y, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer test %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable accelerometer y */
    res = mpu9250_set_accelerometer_test(&gs_handle, MPU9250_AXIS_Y, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable accelerometer y.\n");
    res = mpu9250_get_accelerometer_test(&gs_handle, MPU9250_AXIS_Y, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer test %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable accelerometer z */
    res = mpu9250_set_accelerometer_test(&gs_handle, MPU9250_AXIS_Z, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable accelerometer z.\n");
    res = mpu9250_get_accelerometer_test(&gs_handle, MPU9250_AXIS_Z, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer test %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable accelerometer z */
    res = mpu9250_set_accelerometer_test(&gs_handle, MPU9250_AXIS_Z, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable accelerometer z.\n");
    res = mpu9250_get_accelerometer_test(&gs_handle, MPU9250_AXIS_Z, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer test %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_range/mpu9250_get_accelerometer_range test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_range/mpu9250_get_accelerometer_range test.\n");
    
    /* 2g */
    res = mpu9250_set_accelerometer_range(&gs_handle, MPU9250_ACCELEROMETER_RANGE_2G);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer range 2g.\n");
    res = mpu9250_get_accelerometer_range(&gs_handle, &range_a);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer range %s.\n", range_a == MPU9250_ACCELEROMETER_RANGE_2G ? "ok" : "error");
    
    /* 4g */
    res = mpu9250_set_accelerometer_range(&gs_handle, MPU9250_ACCELEROMETER_RANGE_4G);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer range 4g.\n");
    res = mpu9250_get_accelerometer_range(&gs_handle, &range_a);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer range %s.\n", range_a == MPU9250_ACCELEROMETER_RANGE_4G ? "ok" : "error");
    
    /* 8g */
    res = mpu9250_set_accelerometer_range(&gs_handle, MPU9250_ACCELEROMETER_RANGE_8G);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer range 8g.\n");
    res = mpu9250_get_accelerometer_range(&gs_handle, &range_a);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer range %s.\n", range_a == MPU9250_ACCELEROMETER_RANGE_8G ? "ok" : "error");
    
    /* 16g */
    res = mpu9250_set_accelerometer_range(&gs_handle, MPU9250_ACCELEROMETER_RANGE_16G);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer range 16g.\n");
    res = mpu9250_get_accelerometer_range(&gs_handle, &range_a);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer range failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer range %s.\n", range_a == MPU9250_ACCELEROMETER_RANGE_16G ? "ok" : "error");
    
    /* mpu9250_set_fifo_enable/mpu9250_get_fifo_enable test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_fifo_enable/mpu9250_get_fifo_enable test.\n");
    
    /* set fifo temp enable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_TEMP, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo temp enable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_TEMP, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* set fifo temp disable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_TEMP, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo temp disable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_TEMP, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* set fifo gyroscope x enable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_XG, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo gyroscope x enable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_XG, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* set fifo gyroscope x disable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_XG, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo gyroscope x disable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_XG, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* set fifo gyroscope y enable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_YG, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo gyroscope y enable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_YG, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* set fifo gyroscope y disable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_YG, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo gyroscope y disable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_YG, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* set fifo gyroscope z enable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_ZG, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo gyroscope z enable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_ZG, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* set fifo gyroscope z disable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_ZG, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo gyroscope z disable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_ZG, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* set fifo accelerometer enable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_ACCEL, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo accelerometer enable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_ACCEL, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* set fifo accelerometer disable */
    res = mpu9250_set_fifo_enable(&gs_handle, MPU9250_FIFO_ACCEL, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo accelerometer disable.\n");
    res = mpu9250_get_fifo_enable(&gs_handle, MPU9250_FIFO_ACCEL, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_interrupt_level/mpu9250_get_interrupt_level test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_interrupt_level/mpu9250_get_interrupt_level test.\n");
    
    /* high level */
    res = mpu9250_set_interrupt_level(&gs_handle, MPU9250_PIN_LEVEL_HIGH);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set interrupt high level.\n");
    res = mpu9250_get_interrupt_level(&gs_handle, &level);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interrupt level %s.\n", level == MPU9250_PIN_LEVEL_HIGH ? "ok" : "error");
    
    /* low level */
    res = mpu9250_set_interrupt_level(&gs_handle, MPU9250_PIN_LEVEL_LOW);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set interrupt low level.\n");
    res = mpu9250_get_interrupt_level(&gs_handle, &level);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interrupt level %s.\n", level == MPU9250_PIN_LEVEL_LOW ? "ok" : "error");
    
    /* mpu9250_set_interrupt_pin_type/mpu9250_get_interrupt_pin_type test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_interrupt_pin_type/mpu9250_get_interrupt_pin_type test.\n");
    
    /* push-pull */
    res = mpu9250_set_interrupt_pin_type(&gs_handle, MPU9250_PIN_TYPE_PUSH_PULL);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt pin type failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set interrupt pin type push pull.\n");
    res = mpu9250_get_interrupt_pin_type(&gs_handle, &pin);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt pin type failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interrupt pin type %s.\n", pin == MPU9250_PIN_TYPE_PUSH_PULL ? "ok" : "error");
    
    /* open drain */
    res = mpu9250_set_interrupt_pin_type(&gs_handle, MPU9250_PIN_TYPE_OPEN_DRAIN);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt pin type failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set interrupt pin type open drain.\n");
    res = mpu9250_get_interrupt_pin_type(&gs_handle, &pin);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt pin type failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interrupt pin type %s.\n", pin == MPU9250_PIN_TYPE_OPEN_DRAIN ? "ok" : "error");
    
    /* mpu9250_set_interrupt_latch/mpu9250_get_interrupt_latch test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_interrupt_latch/mpu9250_get_interrupt_latch test.\n");
    
    /* enable latch */
    res = mpu9250_set_interrupt_latch(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt latch failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable interrupt latch.\n");
    res = mpu9250_get_interrupt_latch(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt latch failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interrupt latch %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable latch */
    res = mpu9250_set_interrupt_latch(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt latch failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable interrupt latch.\n");
    res = mpu9250_get_interrupt_latch(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt latch failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interrupt latch %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_interrupt_read_clear/mpu9250_get_interrupt_read_clear test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_interrupt_read_clear/mpu9250_get_interrupt_read_clear test.\n");
    
    /* enable interrupt read clear */
    res = mpu9250_set_interrupt_read_clear(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt read clear failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable interrupt read clear.\n");
    res = mpu9250_get_interrupt_read_clear(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt read clear failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interrupt read clear %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable interrupt read clear */
    res = mpu9250_set_interrupt_read_clear(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt read clear failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable interrupt read clear.\n");
    res = mpu9250_get_interrupt_read_clear(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt read clear failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check interrupt read clear %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_fsync_interrupt_level/mpu9250_get_fsync_interrupt_level test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_fsync_interrupt_level/mpu9250_get_fsync_interrupt_level test.\n");
    
    /* set fsync interrupt level high */
    res = mpu9250_set_fsync_interrupt_level(&gs_handle, MPU9250_PIN_LEVEL_HIGH);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fsync interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fsync interrupt level high.\n");
    res = mpu9250_get_fsync_interrupt_level(&gs_handle, &level);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fsync interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fsync interrupt level %s.\n", level == MPU9250_PIN_LEVEL_HIGH ? "ok" : "error");
    
    /* set fsync interrupt level low */
    res = mpu9250_set_fsync_interrupt_level(&gs_handle, MPU9250_PIN_LEVEL_LOW);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fsync interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set fsync interrupt level low.\n");
    res = mpu9250_get_fsync_interrupt_level(&gs_handle, &level);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fsync interrupt level failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fsync interrupt level %s.\n", level == MPU9250_PIN_LEVEL_LOW ? "ok" : "error");
    
    /* mpu9250_set_fsync_interrupt/mpu9250_get_fsync_interrupt test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_fsync_interrupt/mpu9250_get_fsync_interrupt test.\n");
    
    /* enable fsync interrupt */
    res = mpu9250_set_fsync_interrupt(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fsync interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable fsync interrupt.\n");
    res = mpu9250_get_fsync_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fsync interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fsync interrupt %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable fsync interrupt */
    res = mpu9250_set_fsync_interrupt(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fsync interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable fsync interrupt.\n");
    res = mpu9250_get_fsync_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fsync interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fsync interrupt %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic_bypass/mpu9250_get_iic_bypass test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_bypass/mpu9250_get_iic_bypass test.\n");
    
    /* enable iic bypass */
    res = mpu9250_set_iic_bypass(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic bypass failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic bypass.\n");
    res = mpu9250_get_iic_bypass(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic bypass failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic bypass %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable iic bypass */
    res = mpu9250_set_iic_bypass(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic bypass failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic bypass.\n");
    res = mpu9250_get_iic_bypass(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic bypass failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic bypass %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_interrupt/mpu9250_get_interrupt test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_interrupt/mpu9250_get_interrupt test.\n");
    
    /* enable motion interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_MOTION, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable motion interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_MOTION, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check motion interrupt %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable motion interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_MOTION, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable motion interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_MOTION, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check motion interrupt %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable fifo overflow interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_FIFO_OVERFLOW, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable fifo overflow interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_FIFO_OVERFLOW, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo overflow interrupt %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable fifo overflow interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_FIFO_OVERFLOW, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable fifo overflow interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_FIFO_OVERFLOW, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo overflow interrupt %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable dmp interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_DMP, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable dmp interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_DMP, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check dmp interrupt %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable dmp interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_DMP, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable dmp interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_DMP, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check dmp interrupt %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable fsync init interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_FSYNC_INT, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable fsync init interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_FSYNC_INT, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fsync init interrupt %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable fsync init interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_FSYNC_INT, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable fsync init interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_FSYNC_INT, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check fsync init interrupt %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable data ready interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_DATA_READY, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable data ready interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_DATA_READY, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check data ready interrupt %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable data ready interrupt */
    res = mpu9250_set_interrupt(&gs_handle, MPU9250_INTERRUPT_DATA_READY, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable data ready interrupt.\n");
    res = mpu9250_get_interrupt(&gs_handle, MPU9250_INTERRUPT_DATA_READY, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check data ready interrupt %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_get_interrupt_status test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_get_interrupt_status test.\n");
    
    /* get interrupt status */
    res = mpu9250_get_interrupt_status(&gs_handle, &status);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get interrupt status failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: get interrupt status 0x%02x.\n", status);
    
    /* mpu9250_set_gyroscope_x_test/mpu9250_get_gyroscope_x_test test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyroscope_x_test/mpu9250_get_gyroscope_x_test test.\n");
    
    test = rand() % 0x20;
    res = mpu9250_set_gyroscope_x_test(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope x test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set gyroscope x test 0x%02X.\n", test);
    res = mpu9250_get_gyroscope_x_test(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope x test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope x test %s.\n", test == test_check ? "ok" : "error");
    
    /* mpu9250_set_gyroscope_y_test/mpu9250_get_gyroscope_y_test test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyroscope_y_test/mpu9250_get_gyroscope_y_test test.\n");
    
    test = rand() % 0x20;
    res = mpu9250_set_gyroscope_y_test(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope y test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set gyroscope y test 0x%02X.\n", test);
    res = mpu9250_get_gyroscope_y_test(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope y test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope y test %s.\n", test == test_check ? "ok" : "error");
    
    /* mpu9250_set_gyroscope_z_test/mpu9250_get_gyroscope_z_test test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyroscope_z_test/mpu9250_get_gyroscope_z_test test.\n");
    
    test = rand() % 0x20;
    res = mpu9250_set_gyroscope_z_test(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope z test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set gyroscope z test 0x%02X.\n", test);
    res = mpu9250_get_gyroscope_z_test(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope z test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope z test %s.\n", test == test_check ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_x_test/mpu9250_get_accelerometer_x_test test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_x_test/mpu9250_get_accelerometer_x_test test.\n");
    
    test = rand() % 0x20;
    res = mpu9250_set_accelerometer_x_test(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer x test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer x test 0x%02X.\n", test);
    res = mpu9250_get_accelerometer_x_test(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer x test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer x test %s.\n", test == test_check ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_y_test/mpu9250_get_accelerometer_y_test test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_y_test/mpu9250_get_accelerometer_y_test test.\n");
    
    test = rand() % 0x20;
    res = mpu9250_set_accelerometer_y_test(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer y test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer y test 0x%02X.\n", test);
    res = mpu9250_get_accelerometer_y_test(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer y test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer y test %s.\n", test == test_check ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_z_test/mpu9250_get_accelerometer_z_test test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_z_test/mpu9250_get_accelerometer_z_test test.\n");
    
    test = rand() % 0x20;
    res = mpu9250_set_accelerometer_z_test(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer z test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer z test 0x%02X.\n", test);
    res = mpu9250_get_accelerometer_z_test(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer z test failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer z test %s.\n", test == test_check ? "ok" : "error");
    
    /* mpu9250_set_motion_threshold/mpu9250_get_motion_threshold test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_motion_threshold/mpu9250_get_motion_threshold test.\n");
    
    test = rand() % 256;
    res = mpu9250_set_motion_threshold(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set motion threshold failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set motion threshold 0x%02X.\n", test);
    res = mpu9250_get_motion_threshold(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get motion threshold failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check motion threshold %s.\n", test == test_check ? "ok" : "error");
    
    /* mpu9250_motion_threshold_convert_to_register/mpu9250_motion_threshold_convert_to_data test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_motion_threshold_convert_to_register/mpu9250_motion_threshold_convert_to_data test.\n");
    
    test_f = (rand() % 10000) / 10.0f + 32.0f;
    res = mpu9250_motion_threshold_convert_to_register(&gs_handle, test_f, &reg);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: motion threshold convert to register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: motion threshold convert to register %0.2f.\n", test_f);
    res = mpu9250_motion_threshold_convert_to_data(&gs_handle, reg, &test_check_f);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: motion threshold convert to data failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check motion threshold %0.2f.\n", test_check_f);
    
    /* mpu9250_set_iic_clock/mpu9250_get_iic_clock test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_clock/mpu9250_get_iic_clock test.\n");
    
    /* 348 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_348_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 348 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_348_KHZ ? "ok" : "error");
    
    /* 333 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_333_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 333 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_333_KHZ ? "ok" : "error");
    
    /* 320 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_320_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 320 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_320_KHZ ? "ok" : "error");
    
    /* 308 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_308_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 308 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_308_KHZ ? "ok" : "error");
    
    /* 296 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_296_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 296 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_296_KHZ ? "ok" : "error");
    
    /* 286 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_286_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 286 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_286_KHZ ? "ok" : "error");
    
    /* 276 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_276_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 276 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_276_KHZ ? "ok" : "error");
    
    /* 267 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_267_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 267 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_267_KHZ ? "ok" : "error");
    
    /* 258 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_258_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 258 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_258_KHZ ? "ok" : "error");
    
    /* 500 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_500_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 500 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_500_KHZ ? "ok" : "error");
    
    /* 471 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_471_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 471 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_471_KHZ ? "ok" : "error");
    
    /* 444 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_444_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 444 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_444_KHZ ? "ok" : "error");
    
    /* 421 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_421_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 421 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_421_KHZ ? "ok" : "error");
    
    /* 400 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_400_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 400 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_400_KHZ ? "ok" : "error");
    
    /* 381 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_381_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 381 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_381_KHZ ? "ok" : "error");
    
    /* 364 kHz */
    res = mpu9250_set_iic_clock(&gs_handle, MPU9250_IIC_CLOCK_364_KHZ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set iic clock 364 kHz.\n");
    res = mpu9250_get_iic_clock(&gs_handle, &clk);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic clock failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic clock %s.\n", clk == MPU9250_IIC_CLOCK_364_KHZ ? "ok" : "error");
    
    /* mpu9250_set_iic_multi_master/mpu9250_get_iic_multi_master test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_multi_master/mpu9250_get_iic_multi_master test.\n");
    
    /* enable multi master */
    res = mpu9250_set_iic_multi_master(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic multi master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic multi master.\n");
    res = mpu9250_get_iic_multi_master(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic multi master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic multi master %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable multi master */
    res = mpu9250_set_iic_multi_master(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic multi master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic multi master.\n");
    res = mpu9250_get_iic_multi_master(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic multi master failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic multi master %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic_wait_for_external_sensor/mpu9250_get_iic_wait_for_external_sensor test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_wait_for_external_sensor/mpu9250_get_iic_wait_for_external_sensor test.\n");
    
    /* enable iic wait for external sensor */
    res = mpu9250_set_iic_wait_for_external_sensor(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic wait for external sensor failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic wait for external sensor.\n");
    res = mpu9250_get_iic_wait_for_external_sensor(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic wait for external sensor failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic wait for external sensor %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable iic wait for external sensor */
    res = mpu9250_set_iic_wait_for_external_sensor(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic wait for external sensor failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic wait for external sensor.\n");
    res = mpu9250_get_iic_wait_for_external_sensor(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic wait for external sensor failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic wait for external sensor %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic_read_mode/mpu9250_get_iic_read_mode test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_read_mode/mpu9250_get_iic_read_mode test.\n");
    
    /* restart read mode */
    res = mpu9250_set_iic_read_mode(&gs_handle, MPU9250_IIC_READ_MODE_RESTART);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic read mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set restart read mode.\n");
    res = mpu9250_get_iic_read_mode(&gs_handle, &read_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic read mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic read mode %s.\n", read_mode == MPU9250_IIC_READ_MODE_RESTART ? "ok" : "error");
    
    /* stop and start read mode */
    res = mpu9250_set_iic_read_mode(&gs_handle, MPU9250_IIC_READ_MODE_STOP_AND_START);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic read mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set stop and start read mode.\n");
    res = mpu9250_get_iic_read_mode(&gs_handle, &read_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic read mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic read mode %s.\n", read_mode == MPU9250_IIC_READ_MODE_STOP_AND_START ? "ok" : "error");
    
    /* mpu9250_set_iic_fifo_enable/mpu9250_get_iic_fifo_enable test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_fifo_enable/mpu9250_get_iic_fifo_enable test.\n");
    
    /* enable iic fifo slave0 */
    res = mpu9250_set_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic fifo slave0.\n");
    res = mpu9250_get_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_0, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic fifo enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable iic fifo slave0 */
    res = mpu9250_set_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic fifo slave0.\n");
    res = mpu9250_get_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_0, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic fifo enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable iic fifo slave1 */
    res = mpu9250_set_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic fifo slave1.\n");
    res = mpu9250_get_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_1, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic fifo enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable iic fifo slave1 */
    res = mpu9250_set_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic fifo slave1.\n");
    res = mpu9250_get_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_1, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic fifo enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable iic fifo slave2 */
    res = mpu9250_set_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic fifo slave2.\n");
    res = mpu9250_get_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_2, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic fifo enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable iic fifo slave2 */
    res = mpu9250_set_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic fifo slave2.\n");
    res = mpu9250_get_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_2, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic fifo enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable iic fifo slave3 */
    res = mpu9250_set_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic fifo slave3.\n");
    res = mpu9250_get_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_3, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic fifo enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable iic fifo slave3 */
    res = mpu9250_set_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic fifo slave3.\n");
    res = mpu9250_get_iic_fifo_enable(&gs_handle, MPU9250_IIC_SLAVE_3, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic fifo enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic fifo enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic_mode/mpu9250_get_iic_mode test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_mode/mpu9250_get_iic_mode test.\n");
    
    /* write mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_IIC_MODE_WRITE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 iic write mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_0, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_WRITE ? "ok" : "error");
    
    /* read mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_IIC_MODE_READ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 iic read mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_0, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_READ ? "ok" : "error");
    
    /* write mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_IIC_MODE_WRITE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 iic write mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_1, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_WRITE ? "ok" : "error");
    
    /* read mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_IIC_MODE_READ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 iic read mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_1, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_READ ? "ok" : "error");
    
    /* write mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_IIC_MODE_WRITE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 iic write mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_2, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_WRITE ? "ok" : "error");
    
    /* read mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_IIC_MODE_READ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 iic read mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_2, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_READ ? "ok" : "error");
    
    /* write mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_IIC_MODE_WRITE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 iic write mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_3, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_WRITE ? "ok" : "error");
    
    /* read mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_IIC_MODE_READ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 iic read mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_3, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_READ ? "ok" : "error");
    
    /* write mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_4, MPU9250_IIC_MODE_WRITE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave4 iic write mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_4, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_WRITE ? "ok" : "error");
    
    /* read mode */
    res = mpu9250_set_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_4, MPU9250_IIC_MODE_READ);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave4 iic read mode.\n");
    res = mpu9250_get_iic_mode(&gs_handle, MPU9250_IIC_SLAVE_4, &mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic mode %s.\n", mode == MPU9250_IIC_MODE_READ ? "ok" : "error");
    
    /* mpu9250_set_iic_address/mpu9250_get_iic_address test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_address/mpu9250_get_iic_address test.\n");
    
    /* slave0 */
    test = rand() % 0x7F;
    res = mpu9250_set_iic_address(&gs_handle, MPU9250_IIC_SLAVE_0, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 iic address 0x%02X.\n", test);
    res = mpu9250_get_iic_address(&gs_handle, MPU9250_IIC_SLAVE_0, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic address %s.\n", test_check == test ? "ok" : "error");
    
    /* slave1 */
    test = rand() % 0x7F;
    res = mpu9250_set_iic_address(&gs_handle, MPU9250_IIC_SLAVE_1, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 iic address 0x%02X.\n", test);
    res = mpu9250_get_iic_address(&gs_handle, MPU9250_IIC_SLAVE_1, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic address %s.\n", test_check == test ? "ok" : "error");
    
    /* slave2 */
    test = rand() % 0x7F;
    res = mpu9250_set_iic_address(&gs_handle, MPU9250_IIC_SLAVE_2, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 iic address 0x%02X.\n", test);
    res = mpu9250_get_iic_address(&gs_handle, MPU9250_IIC_SLAVE_2, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic address %s.\n", test_check == test ? "ok" : "error");
    
    /* slave3 */
    test = rand() % 0x7F;
    res = mpu9250_set_iic_address(&gs_handle, MPU9250_IIC_SLAVE_3, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 iic address 0x%02X.\n", test);
    res = mpu9250_get_iic_address(&gs_handle, MPU9250_IIC_SLAVE_3, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic address %s.\n", test_check == test ? "ok" : "error");
    
    /* slave4 */
    test = rand() % 0x7F;
    res = mpu9250_set_iic_address(&gs_handle, MPU9250_IIC_SLAVE_4, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave4 iic address 0x%02X.\n", test);
    res = mpu9250_get_iic_address(&gs_handle, MPU9250_IIC_SLAVE_4, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic address failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic address %s.\n", test_check == test ? "ok" : "error");
    
    /* mpu9250_set_iic_register/mpu9250_get_iic_register test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_register/mpu9250_get_iic_register test.\n");
    
    /* slave0 */
    test = rand() % 256;
    res = mpu9250_set_iic_register(&gs_handle, MPU9250_IIC_SLAVE_0, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 iic register 0x%02X.\n", test);
    res = mpu9250_get_iic_register(&gs_handle, MPU9250_IIC_SLAVE_0, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic register %s.\n", test_check == test ? "ok" : "error");
    
    /* slave1 */
    test = rand() % 256;
    res = mpu9250_set_iic_register(&gs_handle, MPU9250_IIC_SLAVE_1, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 iic register 0x%02X.\n", test);
    res = mpu9250_get_iic_register(&gs_handle, MPU9250_IIC_SLAVE_1, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic register %s.\n", test_check == test ? "ok" : "error");
    
    /* slave2 */
    test = rand() % 256;
    res = mpu9250_set_iic_register(&gs_handle, MPU9250_IIC_SLAVE_2, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 iic register 0x%02X.\n", test);
    res = mpu9250_get_iic_register(&gs_handle, MPU9250_IIC_SLAVE_2, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic register %s.\n", test_check == test ? "ok" : "error");
    
    /* slave3 */
    test = rand() % 256;
    res = mpu9250_set_iic_register(&gs_handle, MPU9250_IIC_SLAVE_3, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 iic register 0x%02X.\n", test);
    res = mpu9250_get_iic_register(&gs_handle, MPU9250_IIC_SLAVE_3, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic register %s.\n", test_check == test ? "ok" : "error");
    
    /* slave4 */
    test = rand() % 256;
    res = mpu9250_set_iic_register(&gs_handle, MPU9250_IIC_SLAVE_4, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave4 iic register 0x%02X.\n", test);
    res = mpu9250_get_iic_register(&gs_handle, MPU9250_IIC_SLAVE_4, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic register %s.\n", test_check == test ? "ok" : "error");
    
    /* mpu9250_set_iic_data_out/mpu9250_get_iic_data_out test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_data_out/mpu9250_get_iic_data_out test.\n");
    
    /* slave0 */
    test = rand() % 256;
    res = mpu9250_set_iic_data_out(&gs_handle, MPU9250_IIC_SLAVE_0, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 iic data out 0x%02X.\n", test);
    res = mpu9250_get_iic_data_out(&gs_handle, MPU9250_IIC_SLAVE_0, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic data out %s.\n", test_check == test ? "ok" : "error");
    
    /* slave1 */
    test = rand() % 256;
    res = mpu9250_set_iic_data_out(&gs_handle, MPU9250_IIC_SLAVE_1, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 iic data out 0x%02X.\n", test);
    res = mpu9250_get_iic_data_out(&gs_handle, MPU9250_IIC_SLAVE_1, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic data out %s.\n", test_check == test ? "ok" : "error");
    
    /* slave2 */
    test = rand() % 256;
    res = mpu9250_set_iic_data_out(&gs_handle, MPU9250_IIC_SLAVE_2, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 iic data out 0x%02X.\n", test);
    res = mpu9250_get_iic_data_out(&gs_handle, MPU9250_IIC_SLAVE_2, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic data out %s.\n", test_check == test ? "ok" : "error");
    
    /* slave3 */
    test = rand() % 256;
    res = mpu9250_set_iic_data_out(&gs_handle, MPU9250_IIC_SLAVE_3, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 iic data out 0x%02X.\n", test);
    res = mpu9250_get_iic_data_out(&gs_handle, MPU9250_IIC_SLAVE_3, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic data out %s.\n", test_check == test ? "ok" : "error");
    
    /* mpu9250_set_iic_enable/mpu9250_get_iic_enable test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_enable/mpu9250_get_iic_enable test.\n");
    
    /* slave0 iic enable */
    res = mpu9250_set_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: slave0 iic enable.\n");
    res = mpu9250_get_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_0, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* slave0 iic disable */
    res = mpu9250_set_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: slave0 iic disable.\n");
    res = mpu9250_get_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_0, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* slave1 iic enable */
    res = mpu9250_set_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: slave1 iic enable.\n");
    res = mpu9250_get_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_1, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* slave1 iic disable */
    res = mpu9250_set_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: slave1 iic disable.\n");
    res = mpu9250_get_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_1, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* slave2 iic enable */
    res = mpu9250_set_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: slave2 iic enable.\n");
    res = mpu9250_get_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_2, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* slave2 iic disable */
    res = mpu9250_set_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: slave2 iic disable.\n");
    res = mpu9250_get_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_2, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* slave3 iic enable */
    res = mpu9250_set_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: slave3 iic enable.\n");
    res = mpu9250_get_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_3, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* slave3 iic disable */
    res = mpu9250_set_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: slave3 iic disable.\n");
    res = mpu9250_get_iic_enable(&gs_handle, MPU9250_IIC_SLAVE_3, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic_byte_swap/mpu9250_get_iic_byte_swap test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_byte_swap/mpu9250_get_iic_byte_swap test.\n");
    
    /* enable slave0 byte swap */
    res = mpu9250_set_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable slave0 byte swap.\n");
    res = mpu9250_get_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_0, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic byte swap %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable slave0 byte swap */
    res = mpu9250_set_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable slave0 byte swap.\n");
    res = mpu9250_get_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_0, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic byte swap %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable slave1 byte swap */
    res = mpu9250_set_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable slave1 byte swap.\n");
    res = mpu9250_get_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_1, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic byte swap %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable slave1 byte swap */
    res = mpu9250_set_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable slave1 byte swap.\n");
    res = mpu9250_get_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_1, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic byte swap %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable slave2 byte swap */
    res = mpu9250_set_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable slave2 byte swap.\n");
    res = mpu9250_get_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_2, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic byte swap %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable slave2 byte swap */
    res = mpu9250_set_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable slave2 byte swap.\n");
    res = mpu9250_get_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_2, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic byte swap %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable slave3 byte swap */
    res = mpu9250_set_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable slave3 byte swap.\n");
    res = mpu9250_get_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_3, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic byte swap %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable slave3 byte swap */
    res = mpu9250_set_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable slave3 byte swap.\n");
    res = mpu9250_get_iic_byte_swap(&gs_handle, MPU9250_IIC_SLAVE_3, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic byte swap failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic byte swap %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic_transaction_mode/mpu9250_get_iic_transaction_mode test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_transaction_mode/mpu9250_get_iic_transaction_mode test.\n");
    
    /* set slave0 data transaction mode */
    res = mpu9250_set_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_IIC_TRANSACTION_MODE_DATA);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 data transaction mode.\n");
    res = mpu9250_get_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_0, &tran_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transaction mode %s.\n", tran_mode == MPU9250_IIC_TRANSACTION_MODE_DATA ? "ok" : "error");
    
    /* set slave0 reg data transaction mode */
    res = mpu9250_set_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_IIC_TRANSACTION_MODE_REG_DATA);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 reg data transaction mode.\n");
    res = mpu9250_get_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_0, &tran_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transaction mode %s.\n", tran_mode == MPU9250_IIC_TRANSACTION_MODE_REG_DATA ? "ok" : "error");
    
    /* set slave1 data transaction mode */
    res = mpu9250_set_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_IIC_TRANSACTION_MODE_DATA);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 data transaction mode.\n");
    res = mpu9250_get_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_1, &tran_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transaction mode %s.\n", tran_mode == MPU9250_IIC_TRANSACTION_MODE_DATA ? "ok" : "error");
    
    /* set slave1 reg data transaction mode */
    res = mpu9250_set_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_IIC_TRANSACTION_MODE_REG_DATA);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 reg data transaction mode.\n");
    res = mpu9250_get_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_1, &tran_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transaction mode %s.\n", tran_mode == MPU9250_IIC_TRANSACTION_MODE_REG_DATA ? "ok" : "error");
    
    /* set slave2 data transaction mode */
    res = mpu9250_set_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_IIC_TRANSACTION_MODE_DATA);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 data transaction mode.\n");
    res = mpu9250_get_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_2, &tran_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transaction mode %s.\n", tran_mode == MPU9250_IIC_TRANSACTION_MODE_DATA ? "ok" : "error");
    
    /* set slave2 reg data transaction mode */
    res = mpu9250_set_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_IIC_TRANSACTION_MODE_REG_DATA);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 reg data transaction mode.\n");
    res = mpu9250_get_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_2, &tran_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transaction mode %s.\n", tran_mode == MPU9250_IIC_TRANSACTION_MODE_REG_DATA ? "ok" : "error");
    
    /* set slave3 data transaction mode */
    res = mpu9250_set_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_IIC_TRANSACTION_MODE_DATA);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 data transaction mode.\n");
    res = mpu9250_get_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_3, &tran_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transaction mode %s.\n", tran_mode == MPU9250_IIC_TRANSACTION_MODE_DATA ? "ok" : "error");
    
    /* set slave3 reg data transaction mode */
    res = mpu9250_set_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_IIC_TRANSACTION_MODE_REG_DATA);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 reg data transaction mode.\n");
    res = mpu9250_get_iic_transaction_mode(&gs_handle, MPU9250_IIC_SLAVE_3, &tran_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transaction mode %s.\n", tran_mode == MPU9250_IIC_TRANSACTION_MODE_REG_DATA ? "ok" : "error");
    
    /* mpu9250_set_iic_group_order/mpu9250_get_iic_group_order test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_group_order/mpu9250_get_iic_group_order test.\n");
    
    /* set slave0 group order even */
    res = mpu9250_set_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_IIC_GROUP_ORDER_EVEN);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 group order even.\n");
    res = mpu9250_get_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_0, &order);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic group order %s.\n", order == MPU9250_IIC_GROUP_ORDER_EVEN ? "ok" : "error");
    
    /* set slave0 group order odd */
    res = mpu9250_set_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_0, MPU9250_IIC_GROUP_ORDER_ODD);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 group order odd.\n");
    res = mpu9250_get_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_0, &order);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic group order %s.\n", order == MPU9250_IIC_GROUP_ORDER_ODD ? "ok" : "error");
    
    /* set slave1 group order even */
    res = mpu9250_set_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_IIC_GROUP_ORDER_EVEN);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 group order even.\n");
    res = mpu9250_get_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_1, &order);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic group order %s.\n", order == MPU9250_IIC_GROUP_ORDER_EVEN ? "ok" : "error");
    
    /* set slave1 group order odd */
    res = mpu9250_set_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_1, MPU9250_IIC_GROUP_ORDER_ODD);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 group order odd.\n");
    res = mpu9250_get_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_1, &order);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic group order %s.\n", order == MPU9250_IIC_GROUP_ORDER_ODD ? "ok" : "error");
    
    /* set slave2 group order even */
    res = mpu9250_set_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_IIC_GROUP_ORDER_EVEN);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 group order even.\n");
    res = mpu9250_get_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_2, &order);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic group order %s.\n", order == MPU9250_IIC_GROUP_ORDER_EVEN ? "ok" : "error");
    
    /* set slave2 group order odd */
    res = mpu9250_set_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_2, MPU9250_IIC_GROUP_ORDER_ODD);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 group order odd.\n");
    res = mpu9250_get_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_2, &order);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic group order %s.\n", order == MPU9250_IIC_GROUP_ORDER_ODD ? "ok" : "error");
    
    /* set slave3 group order even */
    res = mpu9250_set_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_IIC_GROUP_ORDER_EVEN);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 group order even.\n");
    res = mpu9250_get_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_3, &order);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic group order %s.\n", order == MPU9250_IIC_GROUP_ORDER_EVEN ? "ok" : "error");
    
    /* set slave3 group order odd */
    res = mpu9250_set_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_3, MPU9250_IIC_GROUP_ORDER_ODD);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 group order odd.\n");
    res = mpu9250_get_iic_group_order(&gs_handle, MPU9250_IIC_SLAVE_3, &order);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic group order failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic group order %s.\n", order == MPU9250_IIC_GROUP_ORDER_ODD ? "ok" : "error");
    
    /* mpu9250_set_iic_transferred_len/mpu9250_get_iic_transferred_len test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_transferred_len/mpu9250_get_iic_transferred_len test.\n");
    
    test = rand() % 16;
    res = mpu9250_set_iic_transferred_len(&gs_handle, MPU9250_IIC_SLAVE_0, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transferred len failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave0 iic transferred len %d.\n", test);
    res = mpu9250_get_iic_transferred_len(&gs_handle, MPU9250_IIC_SLAVE_0, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transferred len failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transferred len %s.\n", test_check == test ? "ok" : "error");
    
    test = rand() % 16;
    res = mpu9250_set_iic_transferred_len(&gs_handle, MPU9250_IIC_SLAVE_1, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transferred len failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave1 iic transferred len %d.\n", test);
    res = mpu9250_get_iic_transferred_len(&gs_handle, MPU9250_IIC_SLAVE_1, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transferred len failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transferred len %s.\n", test_check == test ? "ok" : "error");
    
    test = rand() % 16;
    res = mpu9250_set_iic_transferred_len(&gs_handle, MPU9250_IIC_SLAVE_2, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transferred len failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave2 iic transferred len %d.\n", test);
    res = mpu9250_get_iic_transferred_len(&gs_handle, MPU9250_IIC_SLAVE_2, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transferred len failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transferred len %s.\n", test_check == test ? "ok" : "error");
    
    test = rand() % 16;
    res = mpu9250_set_iic_transferred_len(&gs_handle, MPU9250_IIC_SLAVE_3, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic transferred len failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: set slave3 iic transferred len %d.\n", test);
    res = mpu9250_get_iic_transferred_len(&gs_handle, MPU9250_IIC_SLAVE_3, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic transferred len failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic transferred len %s.\n", test_check == test ? "ok" : "error");
    
    /* mpu9250_get_iic_status test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_get_iic_status test.\n");
    
    /* get iic status */
    res = mpu9250_get_iic_status(&gs_handle, &status);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic status failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: iic status is 0x%02X.\n", status);
    
    /* mpu9250_set_iic_delay_enable/mpu9250_get_iic_delay_enable test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_delay_enable/mpu9250_get_iic_delay_enable test.\n");
    
    /* enable delay shadow */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_ES_SHADOW, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable delay shadow.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_ES_SHADOW, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable delay shadow */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_ES_SHADOW, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable delay shadow.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_ES_SHADOW, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable delay slave4 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_4, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable delay slave4.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_4, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable delay slave4 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_4, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable delay slave4.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_4, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable delay slave3 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_3, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable delay slave3.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_3, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable delay slave3 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_3, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable delay slave3.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_3, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable delay slave2 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_2, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable delay slave2.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_2, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable delay slave2 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_2, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable delay slave2.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_2, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable delay slave1 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_1, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: enable delay slave1.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_1, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable delay slave1 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_1, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: disable delay slave1.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_1, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 1;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* enable delay slave0 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_0, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: enable delay slave0.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_0, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable delay slave0 */
    res = mpu9250_set_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_0, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: disable delay slave0.\n");
    res = mpu9250_get_iic_delay_enable(&gs_handle, MPU9250_IIC_DELAY_SLAVE_0, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic4_enable/mpu9250_get_iic4_enable test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic4_enable/mpu9250_get_iic4_enable test.\n");
    
    /* enable iic4 */
    res = mpu9250_set_iic4_enable(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic4 enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic4.\n");
    res = mpu9250_get_iic4_enable(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic4 enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic4 enable %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable iic4 */
    res = mpu9250_set_iic4_enable(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic4 enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic4.\n");
    res = mpu9250_get_iic4_enable(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic4 enable failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic4 enable %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic4_interrupt/mpu9250_get_iic4_interrupt test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic4_interrupt/mpu9250_get_iic4_interrupt test.\n");
    
    /* enable iic4 interrupt */
    res = mpu9250_set_iic4_interrupt(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic4 interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: enable iic4 interrupt.\n");
    res = mpu9250_get_iic4_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic4 interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic4 interrupt %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable iic4 interrupt */
    res = mpu9250_set_iic4_interrupt(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic4 interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: disable iic4 interrupt.\n");
    res = mpu9250_get_iic4_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic4 interrupt failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic4 interrupt %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_iic4_transaction_mode/mpu9250_get_iic4_transaction_mode test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic4_transaction_mode/mpu9250_get_iic4_transaction_mode test.\n");
    
    /* transaction mode data */
    res = mpu9250_set_iic4_transaction_mode(&gs_handle, MPU9250_IIC4_TRANSACTION_MODE_DATA);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic4 transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set iic4 transaction mode data.\n");
    res = mpu9250_get_iic4_transaction_mode(&gs_handle, &transaction_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic4 transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic4 transaction mode %s.\n", transaction_mode == MPU9250_IIC4_TRANSACTION_MODE_DATA ? "ok" : "error");
    
    /* transaction mode reg */
    res = mpu9250_set_iic4_transaction_mode(&gs_handle, MPU9250_IIC4_TRANSACTION_MODE_REG);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic4 transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set iic4 transaction mode reg.\n");
    res = mpu9250_get_iic4_transaction_mode(&gs_handle, &transaction_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic4 transaction mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic4 transaction mode %s.\n", transaction_mode == MPU9250_IIC4_TRANSACTION_MODE_REG ? "ok" : "error");
    
    /* mpu9250_set_iic_delay/mpu9250_get_iic_delay test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic_delay/mpu9250_get_iic_delay test.\n");
    
    test = rand() % 0x1F;
    res = mpu9250_set_iic_delay(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic delay failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set iic delay 0x%02X.\n", test);
    res = mpu9250_get_iic_delay(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic delay failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic delay %s.\n", test == test_check ? "ok" : "error");
    
    /* mpu9250_set_iic4_data_out/mpu9250_get_iic4_data_out test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic4_data_out/mpu9250_get_iic4_data_out test.\n");
    
    test = rand() % 256;
    res = mpu9250_set_iic4_data_out(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic4 data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set iic4 data out 0x%02X.\n", test);
    res = mpu9250_get_iic4_data_out(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get iic4 data out failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check iic4 data out %s.\n", test == test_check ? "ok" : "error");
    
    /* mpu9250_set_iic4_data_in/mpu9250_get_iic4_data_in test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_iic4_data_in/mpu9250_get_iic4_data_in test.\n");
    
    test = rand() % 256;
    res = mpu9250_set_iic4_data_in(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set iic4 data in failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set iic4 data in 0x%02X.\n", test);
    mpu9250_interface_debug_print("mpu9250: check iic4 data in %s.\n", res == 0 ? "ok" : "error");
    
    /* if spi interface */
    if (interface == MPU9250_INTERFACE_SPI)
    {
        /* mpu9250_set_disable_iic_slave/mpu9250_get_disable_iic_slave test */
        mpu9250_interface_debug_print("mpu9250: mpu9250_set_disable_iic_slave/mpu9250_get_disable_iic_slave test.\n");
        
        /* enable iic slave */
        res = mpu9250_set_disable_iic_slave(&gs_handle, MPU9250_BOOL_FALSE);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: set disable iic slave failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: enable iic slave.\n");
        res = mpu9250_get_disable_iic_slave(&gs_handle, &enable);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: get disable iic slave failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check disable iic slave %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
        
        /* disable iic slave */
        res = mpu9250_set_disable_iic_slave(&gs_handle, MPU9250_BOOL_TRUE);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: set disable iic slave failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: disable iic slave.\n");
        res = mpu9250_get_disable_iic_slave(&gs_handle, &enable);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: get disable iic slave failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check disable iic slave %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    }
    
    /* mpu9250_set_gyro_standby/mpu9250_get_gyro_standby test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyro_standby/mpu9250_get_gyro_standby test.\n");
    
    /* enable gyro standby */
    res = mpu9250_set_gyro_standby(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyro standby failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: enable gyro standby.\n");
    res = mpu9250_get_gyro_standby(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyro standby failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check gyro standby %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable gyro standby */
    res = mpu9250_set_gyro_standby(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyro standby failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: disable gyro standby.\n");
    res = mpu9250_get_gyro_standby(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyro standby failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check gyro standby %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_fifo_mode/mpu9250_get_fifo_mode test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_fifo_mode/mpu9250_get_fifo_mode test.\n");
    
    /* set fifo stream mode */
    res = mpu9250_set_fifo_mode(&gs_handle, MPU9250_FIFO_MODE_STREAM);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo stream mode.\n");
    res = mpu9250_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo mode %s.\n", fifo_mode == MPU9250_FIFO_MODE_STREAM ? "ok" : "error");
    
    /* set fifo normal mode */
    res = mpu9250_set_fifo_mode(&gs_handle, MPU9250_FIFO_MODE_NORMAL);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set fifo mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set fifo normal mode.\n");
    res = mpu9250_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get fifo mode failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check fifo mode %s.\n", fifo_mode == MPU9250_FIFO_MODE_NORMAL ? "ok" : "error");
    
    /* mpu9250_set_gyroscope_choice/mpu9250_get_gyroscope_choice test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyroscope_choice/mpu9250_get_gyroscope_choice test.\n");
    
    test = rand() % 4;
    res = mpu9250_set_gyroscope_choice(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyroscope choice failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set gyroscope choice 0x%02X.\n", test);
    res = mpu9250_get_gyroscope_choice(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyroscope choice failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check gyroscope choice %s.\n", test_check == test ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_choice/mpu9250_get_accelerometer_choice test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_choice/mpu9250_get_accelerometer_choice test.\n");
    
    test = rand() % 2;
    res = mpu9250_set_accelerometer_choice(&gs_handle, test);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer choice failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer choice 0x%02X.\n", test);
    res = mpu9250_get_accelerometer_choice(&gs_handle, &test_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer choice failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer choice %s.\n", test_check == test ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_low_pass_filter/mpu9250_get_accelerometer_low_pass_filter test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_low_pass_filter/mpu9250_get_accelerometer_low_pass_filter test.\n");
    
    /* accelerometer low pass filter 0 */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_ACCELEROMETER_LOW_PASS_FILTER_0);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter 0.\n");
    res = mpu9250_get_accelerometer_low_pass_filter(&gs_handle, &accelerometer_filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer low pass filter %s.\n", 
                                  accelerometer_filter == MPU9250_ACCELEROMETER_LOW_PASS_FILTER_0 ? "ok" : "error");
    
    /* accelerometer low pass filter 1 */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_ACCELEROMETER_LOW_PASS_FILTER_1);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter 1.\n");
    res = mpu9250_get_accelerometer_low_pass_filter(&gs_handle, &accelerometer_filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer low pass filter %s.\n", 
                                  accelerometer_filter == MPU9250_ACCELEROMETER_LOW_PASS_FILTER_1 ? "ok" : "error");
    
    /* accelerometer low pass filter 2 */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_ACCELEROMETER_LOW_PASS_FILTER_2);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter 2.\n");
    res = mpu9250_get_accelerometer_low_pass_filter(&gs_handle, &accelerometer_filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer low pass filter %s.\n", 
                                  accelerometer_filter == MPU9250_ACCELEROMETER_LOW_PASS_FILTER_2 ? "ok" : "error");
    
    /* accelerometer low pass filter 3 */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_ACCELEROMETER_LOW_PASS_FILTER_3);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter 3.\n");
    res = mpu9250_get_accelerometer_low_pass_filter(&gs_handle, &accelerometer_filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer low pass filter %s.\n", 
                                  accelerometer_filter == MPU9250_ACCELEROMETER_LOW_PASS_FILTER_3 ? "ok" : "error");
    
    /* accelerometer low pass filter 4 */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_ACCELEROMETER_LOW_PASS_FILTER_4);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter 4.\n");
    res = mpu9250_get_accelerometer_low_pass_filter(&gs_handle, &accelerometer_filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer low pass filter %s.\n", 
                                  accelerometer_filter == MPU9250_ACCELEROMETER_LOW_PASS_FILTER_4 ? "ok" : "error");
    
    /* accelerometer low pass filter 5 */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_ACCELEROMETER_LOW_PASS_FILTER_5);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter 5.\n");
    res = mpu9250_get_accelerometer_low_pass_filter(&gs_handle, &accelerometer_filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer low pass filter %s.\n", 
                                  accelerometer_filter == MPU9250_ACCELEROMETER_LOW_PASS_FILTER_5 ? "ok" : "error");
    
    /* accelerometer low pass filter 6 */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_ACCELEROMETER_LOW_PASS_FILTER_6);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter 6.\n");
    res = mpu9250_get_accelerometer_low_pass_filter(&gs_handle, &accelerometer_filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer low pass filter %s.\n", 
                                  accelerometer_filter == MPU9250_ACCELEROMETER_LOW_PASS_FILTER_6 ? "ok" : "error");
    
    /* accelerometer low pass filter 7 */
    res = mpu9250_set_accelerometer_low_pass_filter(&gs_handle, MPU9250_ACCELEROMETER_LOW_PASS_FILTER_7);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer low pass filter 7.\n");
    res = mpu9250_get_accelerometer_low_pass_filter(&gs_handle, &accelerometer_filter);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer low pass filter failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer low pass filter %s.\n", 
                                  accelerometer_filter == MPU9250_ACCELEROMETER_LOW_PASS_FILTER_7 ? "ok" : "error");
    
    /* mpu9250_set_low_power_accel_output_rate/mpu9250_get_low_power_accel_output_rate test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_low_power_accel_output_rate/mpu9250_get_low_power_accel_output_rate test.\n");
    
    /* set 0.24Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_0P24);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 0.24Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_0P24 ? "ok" : "error");
    
    /* set 0.49Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_0P49);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 0.49Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_0P49 ? "ok" : "error");
    
    /* set 0.98Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_0P98);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 0.98Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_0P98 ? "ok" : "error");
    
    /* set 1.95Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_1P95);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 1.95Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_1P95 ? "ok" : "error");
    
    /* set 3.91Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_3P91);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 3.91Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_3P91 ? "ok" : "error");
    
    /* set 7.81Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_7P81);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 7.81Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_7P81 ? "ok" : "error");
    
    /* set 15.63Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_15P63);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 15.63Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_15P63 ? "ok" : "error");
    
    /* set 31.25Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_31P25);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 31.25Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_31P25 ? "ok" : "error");
    
    /* set 62.50Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_62P50);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 62.50Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_62P50 ? "ok" : "error");
    
    /* set 125Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_125);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 125Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_125 ? "ok" : "error");
    
    /* set 250Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_250);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 250Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_250 ? "ok" : "error");
    
    /* set 500Hz */
    res = mpu9250_set_low_power_accel_output_rate(&gs_handle, MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_500);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set low power accel output rate 500Hz.\n");
    res = mpu9250_get_low_power_accel_output_rate(&gs_handle, &rate);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get low power accel output rate failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check low power accel output rate %s.\n", 
                                  rate == MPU9250_LOW_POWER_ACCEL_OUTPUT_RATE_500 ? "ok" : "error");
    
    /* mpu9250_set_wake_on_motion/mpu9250_get_wake_on_motion test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_wake_on_motion/mpu9250_get_wake_on_motion test.\n");
    
    /* enable wake on motion */
    res = mpu9250_set_wake_on_motion(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set wake on motion failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: enable wake on motion.\n");
    res = mpu9250_get_wake_on_motion(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get wake on motion failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check wake on motion %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable wake on motion */
    res = mpu9250_set_wake_on_motion(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set wake on motion failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: disable wake on motion.\n");
    res = mpu9250_get_wake_on_motion(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get wake on motion failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check wake on motion %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_accel_compare_with_previous_sample/mpu9250_get_accel_compare_with_previous_sample test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accel_compare_with_previous_sample/mpu9250_get_accel_compare_with_previous_sample test.\n");
    
    /* enable accel compare with previous sample */
    res = mpu9250_set_accel_compare_with_previous_sample(&gs_handle, MPU9250_BOOL_TRUE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accel compare with previous sample failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: enable accel compare with previous sample.\n");
    res = mpu9250_get_accel_compare_with_previous_sample(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accel compare with previous sample failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accel compare with previous sample %s.\n", enable == MPU9250_BOOL_TRUE ? "ok" : "error");
    
    /* disable accel compare with previous sample */
    res = mpu9250_set_accel_compare_with_previous_sample(&gs_handle, MPU9250_BOOL_FALSE);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accel compare with previous sample failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: disable accel compare with previous sample.\n");
    res = mpu9250_get_accel_compare_with_previous_sample(&gs_handle, &enable);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accel compare with previous sample failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accel compare with previous sample %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_x_offset/mpu9250_get_accelerometer_x_offset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_x_offset/mpu9250_get_accelerometer_x_offset test.\n");
    
    offset = rand() % 32768;
    res = mpu9250_set_accelerometer_x_offset(&gs_handle, offset);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer x offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer x offset %d.\n", offset);
    res = mpu9250_get_accelerometer_x_offset(&gs_handle, &offset_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer x offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer x offset %s.\n", offset_check == offset ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_y_offset/mpu9250_get_accelerometer_y_offset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_y_offset/mpu9250_get_accelerometer_y_offset test.\n");
    
    offset = -(rand() % 32768);
    res = mpu9250_set_accelerometer_y_offset(&gs_handle, offset);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer y offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer y offset %d.\n", offset);
    res = mpu9250_get_accelerometer_y_offset(&gs_handle, &offset_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer y offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer y offset %s.\n", offset_check == offset ? "ok" : "error");
    
    /* mpu9250_set_accelerometer_z_offset/mpu9250_get_accelerometer_z_offset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_accelerometer_z_offset/mpu9250_get_accelerometer_z_offset test.\n");
    
    offset = rand() % 32768;
    res = mpu9250_set_accelerometer_z_offset(&gs_handle, offset);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set accelerometer z offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set accelerometer z offset %d.\n", offset);
    res = mpu9250_get_accelerometer_z_offset(&gs_handle, &offset_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get accelerometer z offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer z offset %s.\n", offset_check == offset ? "ok" : "error");
    
    /* mpu9250_accelerometer_offset_convert_to_register/mpu9250_accelerometer_offset_convert_to_data test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_accelerometer_offset_convert_to_register/mpu9250_accelerometer_offset_convert_to_data test.\n");
    
    test_f = (float)(rand() % 1000) / 10.0f;
    res = mpu9250_accelerometer_offset_convert_to_register(&gs_handle, test_f, &offset);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: accelerometer offset convert to register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: accelerometer offset convert to register %0.2f.\n", test_f);
    res = mpu9250_accelerometer_offset_convert_to_data(&gs_handle, offset, &test_check_f);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: accelerometer offset convert to data failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check accelerometer offset %0.2f.\n", test_check_f);
    
    /* mpu9250_set_gyro_x_offset/mpu9250_get_gyro_x_offset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyro_x_offset/mpu9250_get_gyro_x_offset test.\n");
    
    offset = rand() % 32768;
    res = mpu9250_set_gyro_x_offset(&gs_handle, offset);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyro x offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set gyro x offset %d.\n", offset);
    res = mpu9250_get_gyro_x_offset(&gs_handle, &offset_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyro x offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check gyro x offset %s.\n", offset_check == offset ? "ok" : "error");
    
    /* mpu9250_set_gyro_y_offset/mpu9250_get_gyro_y_offset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyro_y_offset/mpu9250_get_gyro_y_offset test.\n");
    
    offset = -(rand() % 32768);
    res = mpu9250_set_gyro_y_offset(&gs_handle, offset);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyro y offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set gyro y offset %d.\n", offset);
    res = mpu9250_get_gyro_y_offset(&gs_handle, &offset_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyro y offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check gyro y offset %s.\n", offset_check == offset ? "ok" : "error");
    
    /* mpu9250_set_gyro_z_offset/mpu9250_get_gyro_z_offset test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_set_gyro_z_offset/mpu9250_get_gyro_z_offset test.\n");
    
    offset = rand() % 32768;
    res = mpu9250_set_gyro_z_offset(&gs_handle, offset);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: set gyro z offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: set gyro z offset %d.\n", offset);
    res = mpu9250_get_gyro_z_offset(&gs_handle, &offset_check);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: get gyro z offset failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check gyro z offset %s.\n", offset_check == offset ? "ok" : "error");
    
    /* mpu9250_gyro_offset_convert_to_register/mpu9250_gyro_offset_convert_to_data test */
    mpu9250_interface_debug_print("mpu9250: mpu9250_gyro_offset_convert_to_register/mpu9250_gyro_offset_convert_to_data test.\n");
    
    test_f = (float)(rand() % 1000) / 10.0f;
    res = mpu9250_gyro_offset_convert_to_register(&gs_handle, test_f, &offset);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: gyro offset convert to register failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: gyro offset convert to register %0.2f.\n", test_f);
    res = mpu9250_gyro_offset_convert_to_data(&gs_handle, offset, &test_check_f);
    if (res != 0)
    {
        mpu9250_interface_debug_print("mpu9250: gyro offset convert to data failed.\n");
        (void)mpu9250_deinit(&gs_handle);
       
        return 0;
    }
    mpu9250_interface_debug_print("mpu9250: check gyro offset %0.2f.\n", test_check_f);
    
    /* if iic interface */
    if (interface == MPU9250_INTERFACE_IIC)
    {
        /* magnetometer test */
        mpu9250_interface_debug_print("mpu9250: magnetometer test.\n");
        
        /* mag init */
        res = mpu9250_mag_init(&gs_handle);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag init failed.\n");
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        
        /* mpu9250_mag_get_info test */
        mpu9250_interface_debug_print("mpu9250: mpu9250_mag_get_info test.\n");
        
        /* mag get the information */
        res = mpu9250_mag_get_info(&gs_handle, &test_check);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get info failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: mag information is 0x%2X.\n", test_check);
        
        /* mpu9250_mag_get_status1 test */
        mpu9250_interface_debug_print("mpu9250: mpu9250_mag_get_status1 test.\n");
        
        /* mag get the status1 */
        res = mpu9250_mag_get_status1(&gs_handle, &test_check);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get status1 failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: mag status1 is 0x%02X.\n", test_check);
        
        /* mpu9250_mag_get_status2 test */
        mpu9250_interface_debug_print("mpu9250: mpu9250_mag_get_status2 test.\n");
        
        /* mag get the status2 */
        res = mpu9250_mag_get_status1(&gs_handle, &test_check);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get status2 failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: mag status2 is 0x%02X.\n", test_check);
        
        /* mpu9250_mag_set_mode/mpu9250_mag_get_mode test */
        mpu9250_interface_debug_print("mpu9250: mpu9250_mag_set_mode/mpu9250_mag_get_mode test.\n");
        
        /* set the single measurement mode */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_SINGLE);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: set the single measurement mode.\n");
        res = mpu9250_mag_get_mode(&gs_handle, &mag_mode);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag mode %s.\n", mag_mode == MPU9250_MAGNETOMETER_MODE_SINGLE ? "ok" : "error");
        
        /* set the continuous measurement mode 1 */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_CONTINUOUS1);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: set the continuous measurement mode 1.\n");
        res = mpu9250_mag_get_mode(&gs_handle, &mag_mode);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag mode %s.\n", mag_mode == MPU9250_MAGNETOMETER_MODE_CONTINUOUS1 ? "ok" : "error");
        
        /* set the continuous measurement mode 2 */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_CONTINUOUS2);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: set the continuous measurement mode 2.\n");
        res = mpu9250_mag_get_mode(&gs_handle, &mag_mode);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag mode %s.\n", mag_mode == MPU9250_MAGNETOMETER_MODE_CONTINUOUS2 ? "ok" : "error");
        
        /* set the external trigger measurement mode */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_EXT_TRIGGER);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: set the external trigger measurement mode.\n");
        res = mpu9250_mag_get_mode(&gs_handle, &mag_mode);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag mode %s.\n", mag_mode == MPU9250_MAGNETOMETER_MODE_EXT_TRIGGER ? "ok" : "error");
        
        /* set the self test mode */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_SELF_TEST);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: set the self test mode.\n");
        res = mpu9250_mag_get_mode(&gs_handle, &mag_mode);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag mode %s.\n", mag_mode == MPU9250_MAGNETOMETER_MODE_SELF_TEST ? "ok" : "error");
        
        /* set the fuse rom access mode */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_FUSE_ROM);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: set the fuse rom access mode.\n");
        res = mpu9250_mag_get_mode(&gs_handle, &mag_mode);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag mode %s.\n", mag_mode == MPU9250_MAGNETOMETER_MODE_FUSE_ROM ? "ok" : "error");
        
        /* set the power down mode */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_POWER_DOWN);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: set the power down mode.\n");
        res = mpu9250_mag_get_mode(&gs_handle, &mag_mode);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag mode %s.\n", mag_mode == MPU9250_MAGNETOMETER_MODE_POWER_DOWN ? "ok" : "error");
        
        /* mpu9250_mag_set_bits/mpu9250_mag_get_bits test */
        mpu9250_interface_debug_print("mpu9250: mpu9250_mag_set_bits/mpu9250_mag_get_bits test.\n");
        
        /* set 14 bits */
        res = mpu9250_mag_set_bits(&gs_handle, MPU9250_MAGNETOMETER_BITS_14);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set bits failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: set the 14 bits mode.\n");
        res = mpu9250_mag_get_bits(&gs_handle, &bits);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get bits failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag bits %s.\n", bits == MPU9250_MAGNETOMETER_BITS_14 ? "ok" : "error");
        
        /* set 16 bits */
        res = mpu9250_mag_set_bits(&gs_handle, MPU9250_MAGNETOMETER_BITS_16);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set bits failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: set the 16 bits mode.\n");
        res = mpu9250_mag_get_bits(&gs_handle, &bits);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get bits failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag bits %s.\n", bits == MPU9250_MAGNETOMETER_BITS_16 ? "ok" : "error");
        
        /* mpu9250_mag_set_reset/mpu9250_mag_get_reset test */
        mpu9250_interface_debug_print("mpu9250: mpu9250_mag_set_reset/mpu9250_mag_get_reset test.\n");
        
        /* disable mag reset */
        res = mpu9250_mag_set_reset(&gs_handle, MPU9250_BOOL_FALSE);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set reset failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: disable the mag reset.\n");
        res = mpu9250_mag_get_reset(&gs_handle, &enable);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get reset failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check reset %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
        
        /* mpu9250_mag_set_self_test/mpu9250_mag_get_self_test test */
        mpu9250_interface_debug_print("mpu9250: mpu9250_mag_set_self_test/mpu9250_mag_get_self_test test.\n");
        
        /* disable mag self test */
        res = mpu9250_mag_set_self_test(&gs_handle, MPU9250_BOOL_FALSE);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set self test failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: disable mag self test.\n");
        res = mpu9250_mag_get_self_test(&gs_handle, &enable);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get self test failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: check mag self test %s.\n", enable == MPU9250_BOOL_FALSE ? "ok" : "error");
        
        /* mpu9250_mag_get_asa test */
        mpu9250_interface_debug_print("mpu9250: mpu9250_mag_get_asa test.\n");
        
        /* set the fuse rom access mode */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_FUSE_ROM);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        res = mpu9250_mag_get_asa(&gs_handle, data);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag get asa failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        mpu9250_interface_debug_print("mpu9250: mag asax: 0x%02X asay: 0x%02X asaz: 0x%02X.\n", data[0], data[1], data[2]);
        
        /* set the power down mode */
        res = mpu9250_mag_set_mode(&gs_handle, MPU9250_MAGNETOMETER_MODE_POWER_DOWN);
        if (res != 0)
        {
            mpu9250_interface_debug_print("mpu9250: mag set mode failed.\n");
            (void)mpu9250_mag_deinit(&gs_handle);
            (void)mpu9250_deinit(&gs_handle);
           
            return 0;
        }
        
        /* deinit */
        (void)mpu9250_mag_deinit(&gs_handle);
    }
     
    /* finish register test */
    mpu9250_interface_debug_print("mpu9250: finish register test.\n");
    (void)mpu9250_deinit(&gs_handle);
    
    return 0;
}
