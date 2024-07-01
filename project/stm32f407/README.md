### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

GPIO Pin: INT PB0.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. MPU9250

#### 3.1 Command Instruction

1. Show mpu9250 chip and driver information.

   ```shell
   mpu9250 (-i | --information)
   ```

2. Show mpu9250 help.

   ```shell
   mpu9250 (-h | --help)
   ```

3. Show mpu9250 pin connections of the current board.

   ```shell
   mpu9250 (-p | --port)
   ```

4. Run mpu9250 register test.

   ```shell
   mpu9250 (-t reg | --test=reg) [--addr=<0 | 1>] [--interface=<iic | spi>]
   ```

5. Run mpu9250 read test, num means the test times.

   ```shell
   mpu9250 (-t read | --test=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

6. Run mpu9250 fifo test, num means the test times.

   ```shell
   mpu9250 (-t fifo | --test=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

7. Run mpu9250 dmp test, num means the test times.

   ```shell
   mpu9250 (-t dmp | --test=dmp) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

8. Run mpu9250 motion test.

   ```shell
   mpu9250 (-t motion | --test=motion) [--addr=<0 | 1>] [--interface=<iic | spi>]
   ```

9. Run mpu9250 pedometer test, num means the test times.

   ```shell
   mpu9250 (-t pedometer | --test=pedometer) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

10. Run mpu9250 read function, num means the read times.

    ```shell
    mpu9250 (-e read | --example=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
    ```

11. Run mpu9250 fifo function, num means the read times.

    ```shell
    mpu9250 (-e fifo | --example=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
    ```

12. Run mpu9250 dmp function, num means the read times.

    ```shell
    mpu9250 (-e dmp | --example=dmp) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
    ```

13. Run mpu9250 motion function.

    ```shell
    mpu9250 (-e motion | --example=motion) [--addr=<0 | 1>] [--interface=<iic | spi>]
    ```

14. Run mpu9250 pedometer function, num means the read times.

    ```shell
    mpu9250 (-e pedometer | --example=pedometer) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
    ```

#### 3.2 Command Example

```shell
mpu9250 -i

mpu9250: chip is TDK MPU9250.
mpu9250: manufacturer is TDK.
mpu9250: interface is IIC SPI.
mpu9250: driver version is 1.0.
mpu9250: min supply voltage is 2.4V.
mpu9250: max supply voltage is 3.6V.
mpu9250: max current is 3.70mA.
mpu9250: max temperature is 85.0C.
mpu9250: min temperature is -40.0C.
```

```shell
mpu9250 -p

mpu9250: SPI interface SCK connected to GPIOA PIN5.
mpu9250: SPI interface MISO connected to GPIOA PIN6.
mpu9250: SPI interface MOSI connected to GPIOA PIN7.
mpu9250: SPI interface CS connected to GPIOA PIN4.
mpu9250: SCL connected to GPIOB PIN8.
mpu9250: SDA connected to GPIOB PIN9.
mpu9250: INT connected to GPIOB PIN0.
```

```shell
mpu9250 -t reg --addr=0 --interface=iic

mpu9250: chip is TDK MPU9250.
mpu9250: manufacturer is TDK.
mpu9250: interface is IIC SPI.
mpu9250: driver version is 1.0.
mpu9250: min supply voltage is 2.4V.
mpu9250: max supply voltage is 3.6V.
mpu9250: max current is 3.70mA.
mpu9250: max temperature is 85.0C.
mpu9250: min temperature is -40.0C.
mpu9250: start register test.
mpu9250: mpu9250_set_interface/mpu9250_get_interface test.
mpu9250: set interface iic.
mpu9250: check interface ok.
mpu9250: set interface spi.
mpu9250: check interface ok.
mpu9250: mpu9250_set_addr_pin/mpu9250_get_addr_pin test.
mpu9250: set addr pin low.
mpu9250: check addr pin ok.
mpu9250: set addr pin high.
mpu9250: check addr pin ok.
mpu9250: mpu9250_set_fifo/mpu9250_get_fifo test.
mpu9250: enable fifo.
mpu9250: check fifo ok.
mpu9250: disable fifo.
mpu9250: check fifo ok.
mpu9250: mpu9250_set_iic_master/mpu9250_get_iic_master test.
mpu9250: enable iic master.
mpu9250: check iic master ok.
mpu9250: disable iic master.
mpu9250: check iic master ok.
mpu9250: mpu9250_fifo_reset/mpu9250_get_fifo_reset test.
mpu9250: fifo reset.
mpu9250: check fifo reset ok.
mpu9250: mpu9250_iic_master_reset/mpu9250_get_iic_master_reset test.
mpu9250: iic master reset.
mpu9250: check iic master ok.
mpu9250: mpu9250_sensor_reset/mpu9250_get_sensor_reset test.
mpu9250: sensor reset.
mpu9250: check sensor reset ok.
mpu9250: mpu9250_device_reset/mpu9250_get_device_reset test.
mpu9250: device reset.
mpu9250: check device reset ok.
mpu9250: mpu9250_set_clock_source/mpu9250_get_clock_source test.
mpu9250: stop the clock.
mpu9250: check clock source ok.
mpu9250: set the clock source extern 20.0 MHz.
mpu9250: check clock source ok.
mpu9250: set the clock source pll.
mpu9250: check clock source ok.
mpu9250: mpu9250_set_ptat/mpu9250_get_ptat test.
mpu9250: enable temperature sensor.
mpu9250: check ptat ok.
mpu9250: disable temperature sensor.
mpu9250: check ptat ok.
mpu9250: mpu9250_set_cycle_wake_up/mpu9250_get_cycle_wake_up test.
mpu9250: enable cycle wake up.
mpu9250: check cycle wake up ok.
mpu9250: disable cycle wake up.
mpu9250: check cycle wake up ok.
mpu9250: mpu9250_set_sleep/mpu9250_get_sleep test.
mpu9250: enable the sleep.
mpu9250: check sleep ok.
mpu9250: disable the sleep.
mpu9250: check sleep ok.
mpu9250: mpu9250_set_standby_mode/mpu9250_get_standby_mode test.
mpu9250: enable acc x standby mode.
mpu9250: check standby mode ok.
mpu9250: disable acc x standby mode.
mpu9250: check standby mode ok.
mpu9250: enable acc y standby mode.
mpu9250: check standby mode ok.
mpu9250: disable acc y standby mode.
mpu9250: check standby mode ok.
mpu9250: enable acc z standby mode.
mpu9250: check standby mode ok.
mpu9250: disable acc z standby mode.
mpu9250: check standby mode ok.
mpu9250: enable gyro x standby mode.
mpu9250: check standby mode ok.
mpu9250: disable gyro x standby mode.
mpu9250: check standby mode ok.
mpu9250: enable gyro y standby mode.
mpu9250: check standby mode ok.
mpu9250: disable gyro y standby mode.
mpu9250: check standby mode ok.
mpu9250: enable gyro z standby mode.
mpu9250: check standby mode ok.
mpu9250: disable gyro z standby mode.
mpu9250: check standby mode ok.
mpu9250: mpu9250_fifo_get/mpu9250_fifo_set test.
mpu9250: check fifo write ok.
mpu9250: check fifo read ok.
mpu9250: mpu9250_get_fifo_count test.
mpu9250: fifo count 0.
mpu9250: mpu9250_set_signal_path_reset test.
mpu9250: temp signal path reset.
mpu9250: check signal path reset ok.
mpu9250: accel signal path reset.
mpu9250: check signal path reset ok.
mpu9250: gyro signal path reset.
mpu9250: check signal path reset ok.
mpu9250: mpu9250_set_sample_rate_divider/mpu9250_get_sample_rate_divider test.
mpu9250: set sample rate divider 0x80005EF.
mpu9250: check sample rate divider ok.
mpu9250: mpu9250_set_extern_sync/mpu9250_get_extern_sync test.
mpu9250: set extern sync input disabled.
mpu9250: check extern sync ok.
mpu9250: set extern sync temp out low.
mpu9250: check extern sync ok.
mpu9250: set extern sync gyro xout low.
mpu9250: check extern sync ok.
mpu9250: set extern sync gyro yout low.
mpu9250: check extern sync ok.
mpu9250: set extern sync gyro zout low.
mpu9250: check extern sync ok.
mpu9250: set extern sync accel xout low.
mpu9250: check extern sync ok.
mpu9250: set extern sync accel yout low.
mpu9250: check extern sync ok.
mpu9250: set extern sync accel zout low.
mpu9250: check extern sync ok.
mpu9250: mpu9250_set_low_pass_filter/mpu9250_get_low_pass_filter test.
mpu9250: set low pass filter 0.
mpu9250: check low pass filter ok.
mpu9250: set low pass filter 1.
mpu9250: check low pass filter ok.
mpu9250: set low pass filter 2.
mpu9250: check low pass filter ok.
mpu9250: set low pass filter 3.
mpu9250: check low pass filter ok.
mpu9250: set low pass filter 4.
mpu9250: check low pass filter ok.
mpu9250: set low pass filter 5.
mpu9250: check low pass filter ok.
mpu9250: set low pass filter 6.
mpu9250: check low pass filter ok.
mpu9250: mpu9250_set_gyroscope_test/mpu9250_get_gyroscope_test test.
mpu9250: enable axis x.
mpu9250: check gyroscope test ok.
mpu9250: disable axis x.
mpu9250: check gyroscope test ok.
mpu9250: enable axis y.
mpu9250: check gyroscope test ok.
mpu9250: disable axis y.
mpu9250: check gyroscope test ok.
mpu9250: enable axis z.
mpu9250: check gyroscope test ok.
mpu9250: disable axis z.
mpu9250: check gyroscope test ok.
mpu9250: mpu9250_set_gyroscope_range/mpu9250_get_gyroscope_range test.
mpu9250: set gyroscope range 250 dps.
mpu9250: check gyroscope range ok.
mpu9250: set gyroscope range 500 dps.
mpu9250: check gyroscope range ok.
mpu9250: set gyroscope range 1000 dps.
mpu9250: check gyroscope range ok.
mpu9250: set gyroscope range 2000 dps.
mpu9250: check gyroscope range ok.
mpu9250: mpu9250_set_accelerometer_test/mpu9250_get_accelerometer_test test.
mpu9250: enable accelerometer x.
mpu9250: check accelerometer test ok.
mpu9250: disable accelerometer x.
mpu9250: check accelerometer test ok.
mpu9250: enable accelerometer y.
mpu9250: check accelerometer test ok.
mpu9250: disable accelerometer y.
mpu9250: check accelerometer test ok.
mpu9250: enable accelerometer z.
mpu9250: check accelerometer test ok.
mpu9250: disable accelerometer z.
mpu9250: check accelerometer test ok.
mpu9250: mpu9250_set_accelerometer_range/mpu9250_get_accelerometer_range test.
mpu9250: set accelerometer range 2g.
mpu9250: check accelerometer range ok.
mpu9250: set accelerometer range 4g.
mpu9250: check accelerometer range ok.
mpu9250: set accelerometer range 8g.
mpu9250: check accelerometer range ok.
mpu9250: set accelerometer range 16g.
mpu9250: check accelerometer range ok.
mpu9250: mpu9250_set_fifo_enable/mpu9250_get_fifo_enable test.
mpu9250: set fifo temp enable.
mpu9250: check fifo enable ok.
mpu9250: set fifo temp disable.
mpu9250: check fifo enable ok.
mpu9250: set fifo gyroscope x enable.
mpu9250: check fifo enable ok.
mpu9250: set fifo gyroscope x disable.
mpu9250: check fifo enable ok.
mpu9250: set fifo gyroscope y enable.
mpu9250: check fifo enable ok.
mpu9250: set fifo gyroscope y disable.
mpu9250: check fifo enable ok.
mpu9250: set fifo gyroscope z enable.
mpu9250: check fifo enable ok.
mpu9250: set fifo gyroscope z disable.
mpu9250: check fifo enable ok.
mpu9250: set fifo accelerometer enable.
mpu9250: check fifo enable ok.
mpu9250: set fifo accelerometer disable.
mpu9250: check fifo enable ok.
mpu9250: mpu9250_set_interrupt_level/mpu9250_get_interrupt_level test.
mpu9250: set interrupt high level.
mpu9250: check interrupt level ok.
mpu9250: set interrupt low level.
mpu9250: check interrupt level ok.
mpu9250: mpu9250_set_interrupt_pin_type/mpu9250_get_interrupt_pin_type test.
mpu9250: set interrupt pin type push pull.
mpu9250: check interrupt pin type ok.
mpu9250: set interrupt pin type open drain.
mpu9250: check interrupt pin type ok.
mpu9250: mpu9250_set_interrupt_latch/mpu9250_get_interrupt_latch test.
mpu9250: enable interrupt latch.
mpu9250: check interrupt latch ok.
mpu9250: disable interrupt latch.
mpu9250: check interrupt latch ok.
mpu9250: mpu9250_set_interrupt_read_clear/mpu9250_get_interrupt_read_clear test.
mpu9250: enable interrupt read clear.
mpu9250: check interrupt read clear ok.
mpu9250: disable interrupt read clear.
mpu9250: check interrupt read clear ok.
mpu9250: mpu9250_set_fsync_interrupt_level/mpu9250_get_fsync_interrupt_level test.
mpu9250: set fsync interrupt level high.
mpu9250: check fsync interrupt level ok.
mpu9250: set fsync interrupt level low.
mpu9250: check fsync interrupt level ok.
mpu9250: mpu9250_set_fsync_interrupt/mpu9250_get_fsync_interrupt test.
mpu9250: enable fsync interrupt.
mpu9250: check fsync interrupt ok.
mpu9250: disable fsync interrupt.
mpu9250: check fsync interrupt ok.
mpu9250: mpu9250_set_iic_bypass/mpu9250_get_iic_bypass test.
mpu9250: enable iic bypass.
mpu9250: check iic bypass ok.
mpu9250: disable iic bypass.
mpu9250: check iic bypass ok.
mpu9250: mpu9250_set_interrupt/mpu9250_get_interrupt test.
mpu9250: enable motion interrupt.
mpu9250: check motion interrupt ok.
mpu9250: disable motion interrupt.
mpu9250: check motion interrupt ok.
mpu9250: enable fifo overflow interrupt.
mpu9250: check fifo overflow interrupt ok.
mpu9250: disable fifo overflow interrupt.
mpu9250: check fifo overflow interrupt ok.
mpu9250: enable dmp interrupt.
mpu9250: check dmp interrupt ok.
mpu9250: disable dmp interrupt.
mpu9250: check dmp interrupt ok.
mpu9250: enable fsync init interrupt.
mpu9250: check fsync init interrupt ok.
mpu9250: disable fsync init interrupt.
mpu9250: check fsync init interrupt ok.
mpu9250: enable data ready interrupt.
mpu9250: check data ready interrupt ok.
mpu9250: disable data ready interrupt.
mpu9250: check data ready interrupt ok.
mpu9250: mpu9250_get_interrupt_status test.
mpu9250: get interrupt status 0x01.
mpu9250: mpu9250_set_gyroscope_x_test/mpu9250_get_gyroscope_x_test test.
mpu9250: set gyroscope x test 0x04.
mpu9250: check gyroscope x test ok.
mpu9250: mpu9250_set_gyroscope_y_test/mpu9250_get_gyroscope_y_test test.
mpu9250: set gyroscope y test 0x0C.
mpu9250: check gyroscope y test ok.
mpu9250: mpu9250_set_gyroscope_z_test/mpu9250_get_gyroscope_z_test test.
mpu9250: set gyroscope z test 0x0B.
mpu9250: check gyroscope z test ok.
mpu9250: mpu9250_set_accelerometer_x_test/mpu9250_get_accelerometer_x_test test.
mpu9250: set accelerometer x test 0x1A.
mpu9250: check accelerometer x test ok.
mpu9250: mpu9250_set_accelerometer_y_test/mpu9250_get_accelerometer_y_test test.
mpu9250: set accelerometer y test 0x01.
mpu9250: check accelerometer y test ok.
mpu9250: mpu9250_set_accelerometer_z_test/mpu9250_get_accelerometer_z_test test.
mpu9250: set accelerometer z test 0x05.
mpu9250: check accelerometer z test ok.
mpu9250: mpu9250_set_motion_threshold/mpu9250_get_motion_threshold test.
mpu9250: set motion threshold 0xA1.
mpu9250: check motion threshold ok.
mpu9250: mpu9250_motion_threshold_convert_to_register/mpu9250_motion_threshold_convert_to_data test.
mpu9250: motion threshold convert to register 182.60.
mpu9250: check motion threshold 180.00.
mpu9250: mpu9250_set_iic_clock/mpu9250_get_iic_clock test.
mpu9250: set iic clock 348 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 333 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 320 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 308 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 296 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 286 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 276 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 267 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 258 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 500 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 471 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 444 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 421 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 400 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 381 kHz.
mpu9250: check iic clock ok.
mpu9250: set iic clock 364 kHz.
mpu9250: check iic clock ok.
mpu9250: mpu9250_set_iic_multi_master/mpu9250_get_iic_multi_master test.
mpu9250: enable iic multi master.
mpu9250: check iic multi master ok.
mpu9250: disable iic multi master.
mpu9250: check iic multi master ok.
mpu9250: mpu9250_set_iic_wait_for_external_sensor/mpu9250_get_iic_wait_for_external_sensor test.
mpu9250: enable iic wait for external sensor.
mpu9250: check iic wait for external sensor ok.
mpu9250: disable iic wait for external sensor.
mpu9250: check iic wait for external sensor ok.
mpu9250: mpu9250_set_iic_read_mode/mpu9250_get_iic_read_mode test.
mpu9250: set restart read mode.
mpu9250: check iic read mode ok.
mpu9250: set stop and start read mode.
mpu9250: check iic read mode ok.
mpu9250: mpu9250_set_iic_fifo_enable/mpu9250_get_iic_fifo_enable test.
mpu9250: enable iic fifo slave0.
mpu9250: check iic fifo enable ok.
mpu9250: disable iic fifo slave0.
mpu9250: check iic fifo enable ok.
mpu9250: enable iic fifo slave1.
mpu9250: check iic fifo enable ok.
mpu9250: disable iic fifo slave1.
mpu9250: check iic fifo enable ok.
mpu9250: enable iic fifo slave2.
mpu9250: check iic fifo enable ok.
mpu9250: disable iic fifo slave2.
mpu9250: check iic fifo enable ok.
mpu9250: enable iic fifo slave3.
mpu9250: check iic fifo enable ok.
mpu9250: disable iic fifo slave3.
mpu9250: check iic fifo enable ok.
mpu9250: mpu9250_set_iic_mode/mpu9250_get_iic_mode test.
mpu9250: set slave0 iic write mode.
mpu9250: check iic mode ok.
mpu9250: set slave0 iic read mode.
mpu9250: check iic mode ok.
mpu9250: set slave1 iic write mode.
mpu9250: check iic mode ok.
mpu9250: set slave1 iic read mode.
mpu9250: check iic mode ok.
mpu9250: set slave2 iic write mode.
mpu9250: check iic mode ok.
mpu9250: set slave2 iic read mode.
mpu9250: check iic mode ok.
mpu9250: set slave3 iic write mode.
mpu9250: check iic mode ok.
mpu9250: set slave3 iic read mode.
mpu9250: check iic mode ok.
mpu9250: set slave4 iic write mode.
mpu9250: check iic mode ok.
mpu9250: set slave4 iic read mode.
mpu9250: check iic mode ok.
mpu9250: mpu9250_set_iic_address/mpu9250_get_iic_address test.
mpu9250: set slave0 iic address 0x04.
mpu9250: check iic address ok.
mpu9250: set slave1 iic address 0x36.
mpu9250: check iic address ok.
mpu9250: set slave2 iic address 0x31.
mpu9250: check iic address ok.
mpu9250: set slave3 iic address 0x05.
mpu9250: check iic address ok.
mpu9250: set slave4 iic address 0x7A.
mpu9250: check iic address ok.
mpu9250: mpu9250_set_iic_register/mpu9250_get_iic_register test.
mpu9250: set slave0 iic register 0xB5.
mpu9250: check iic register ok.
mpu9250: set slave1 iic register 0x5A.
mpu9250: check iic register ok.
mpu9250: set slave2 iic register 0x39.
mpu9250: check iic register ok.
mpu9250: set slave3 iic register 0x68.
mpu9250: check iic register ok.
mpu9250: set slave4 iic register 0xE8.
mpu9250: check iic register ok.
mpu9250: mpu9250_set_iic_data_out/mpu9250_get_iic_data_out test.
mpu9250: set slave0 iic data out 0xC9.
mpu9250: check iic data out ok.
mpu9250: set slave1 iic data out 0xAA.
mpu9250: check iic data out ok.
mpu9250: set slave2 iic data out 0x5C.
mpu9250: check iic data out ok.
mpu9250: set slave3 iic data out 0x9F.
mpu9250: check iic data out ok.
mpu9250: mpu9250_set_iic_enable/mpu9250_get_iic_enable test.
mpu9250: slave0 iic enable.
mpu9250: check iic enable ok.
mpu9250: slave0 iic disable.
mpu9250: check iic enable ok.
mpu9250: slave1 iic enable.
mpu9250: check iic enable ok.
mpu9250: slave1 iic disable.
mpu9250: check iic enable ok.
mpu9250: slave2 iic enable.
mpu9250: check iic enable ok.
mpu9250: slave2 iic disable.
mpu9250: check iic enable ok.
mpu9250: slave3 iic enable.
mpu9250: check iic enable ok.
mpu9250: slave3 iic disable.
mpu9250: check iic enable ok.
mpu9250: mpu9250_set_iic_byte_swap/mpu9250_get_iic_byte_swap test.
mpu9250: enable slave0 byte swap.
mpu9250: check iic byte swap ok.
mpu9250: disable slave0 byte swap.
mpu9250: check iic byte swap ok.
mpu9250: enable slave1 byte swap.
mpu9250: check iic byte swap ok.
mpu9250: disable slave1 byte swap.
mpu9250: check iic byte swap ok.
mpu9250: enable slave2 byte swap.
mpu9250: check iic byte swap ok.
mpu9250: disable slave2 byte swap.
mpu9250: check iic byte swap ok.
mpu9250: enable slave3 byte swap.
mpu9250: check iic byte swap ok.
mpu9250: disable slave3 byte swap.
mpu9250: check iic byte swap ok.
mpu9250: mpu9250_set_iic_transaction_mode/mpu9250_get_iic_transaction_mode test.
mpu9250: set slave0 data transaction mode.
mpu9250: check iic transaction mode ok.
mpu9250: set slave0 reg data transaction mode.
mpu9250: check iic transaction mode ok.
mpu9250: set slave1 data transaction mode.
mpu9250: check iic transaction mode ok.
mpu9250: set slave1 reg data transaction mode.
mpu9250: check iic transaction mode ok.
mpu9250: set slave2 data transaction mode.
mpu9250: check iic transaction mode ok.
mpu9250: set slave2 reg data transaction mode.
mpu9250: check iic transaction mode ok.
mpu9250: set slave3 data transaction mode.
mpu9250: check iic transaction mode ok.
mpu9250: set slave3 reg data transaction mode.
mpu9250: check iic transaction mode ok.
mpu9250: mpu9250_set_iic_group_order/mpu9250_get_iic_group_order test.
mpu9250: set slave0 group order even.
mpu9250: check iic group order ok.
mpu9250: set slave0 group order odd.
mpu9250: check iic group order ok.
mpu9250: set slave1 group order even.
mpu9250: check iic group order ok.
mpu9250: set slave1 group order odd.
mpu9250: check iic group order ok.
mpu9250: set slave2 group order even.
mpu9250: check iic group order ok.
mpu9250: set slave2 group order odd.
mpu9250: check iic group order ok.
mpu9250: set slave3 group order even.
mpu9250: check iic group order ok.
mpu9250: set slave3 group order odd.
mpu9250: check iic group order ok.
mpu9250: mpu9250_set_iic_transferred_len/mpu9250_get_iic_transferred_len test.
mpu9250: set slave0 iic transferred len 0.
mpu9250: check iic transferred len ok.
mpu9250: set slave1 iic transferred len 1.
mpu9250: check iic transferred len ok.
mpu9250: set slave2 iic transferred len 8.
mpu9250: check iic transferred len ok.
mpu9250: set slave3 iic transferred len 10.
mpu9250: check iic transferred len ok.
mpu9250: mpu9250_get_iic_status test.
mpu9250: iic status is 0x00.
mpu9250: mpu9250_set_iic_delay_enable/mpu9250_get_iic_delay_enable test.
mpu9250: enable delay shadow.
mpu9250: check iic delay enable ok.
mpu9250: disable delay shadow.
mpu9250: check iic delay enable ok.
mpu9250: enable delay slave4.
mpu9250: check iic delay enable ok.
mpu9250: disable delay slave4.
mpu9250: check iic delay enable ok.
mpu9250: enable delay slave3.
mpu9250: check iic delay enable ok.
mpu9250: disable delay slave3.
mpu9250: check iic delay enable ok.
mpu9250: enable delay slave2.
mpu9250: check iic delay enable ok.
mpu9250: disable delay slave2.
mpu9250: check iic delay enable ok.
mpu9250: enable delay slave1.
mpu9250: check iic delay enable ok.
mpu9250: disable delay slave1.
mpu9250: check iic delay enable ok.
mpu9250: enable delay slave0.
mpu9250: check iic delay enable ok.
mpu9250: disable delay slave0.
mpu9250: check iic delay enable ok.
mpu9250: mpu9250_set_iic4_enable/mpu9250_get_iic4_enable test.
mpu9250: enable iic4.
mpu9250: check iic4 enable ok.
mpu9250: disable iic4.
mpu9250: check iic4 enable ok.
mpu9250: mpu9250_set_iic4_interrupt/mpu9250_get_iic4_interrupt test.
mpu9250: enable iic4 interrupt.
mpu9250: check iic4 interrupt ok.
mpu9250: disable iic4 interrupt.
mpu9250: check iic4 interrupt ok.
mpu9250: mpu9250_set_iic4_transaction_mode/mpu9250_get_iic4_transaction_mode test.
mpu9250: set iic4 transaction mode data.
mpu9250: check iic4 transaction mode ok.
mpu9250: set iic4 transaction mode reg.
mpu9250: check iic4 transaction mode ok.
mpu9250: mpu9250_set_iic_delay/mpu9250_get_iic_delay test.
mpu9250: set iic delay 0x00.
mpu9250: check iic delay ok.
mpu9250: mpu9250_set_iic4_data_out/mpu9250_get_iic4_data_out test.
mpu9250: set iic4 data out 0xBF.
mpu9250: check iic4 data out ok.
mpu9250: mpu9250_set_iic4_data_in/mpu9250_get_iic4_data_in test.
mpu9250: set iic4 data in 0xDD.
mpu9250: check iic4 data in ok.
mpu9250: mpu9250_set_gyro_standby/mpu9250_get_gyro_standby test.
mpu9250: enable gyro standby.
mpu9250: check gyro standby ok.
mpu9250: disable gyro standby.
mpu9250: check gyro standby ok.
mpu9250: mpu9250_set_fifo_mode/mpu9250_get_fifo_mode test.
mpu9250: set fifo stream mode.
mpu9250: check fifo mode ok.
mpu9250: set fifo normal mode.
mpu9250: check fifo mode ok.
mpu9250: mpu9250_set_gyroscope_choice/mpu9250_get_gyroscope_choice test.
mpu9250: set gyroscope choice 0x00.
mpu9250: check gyroscope choice ok.
mpu9250: mpu9250_set_accelerometer_choice/mpu9250_get_accelerometer_choice test.
mpu9250: set accelerometer choice 0x01.
mpu9250: check accelerometer choice ok.
mpu9250: mpu9250_set_accelerometer_low_pass_filter/mpu9250_get_accelerometer_low_pass_filter test.
mpu9250: set accelerometer low pass filter 0.
mpu9250: check accelerometer low pass filter ok.
mpu9250: set accelerometer low pass filter 1.
mpu9250: check accelerometer low pass filter ok.
mpu9250: set accelerometer low pass filter 2.
mpu9250: check accelerometer low pass filter ok.
mpu9250: set accelerometer low pass filter 3.
mpu9250: check accelerometer low pass filter ok.
mpu9250: set accelerometer low pass filter 4.
mpu9250: check accelerometer low pass filter ok.
mpu9250: set accelerometer low pass filter 5.
mpu9250: check accelerometer low pass filter ok.
mpu9250: set accelerometer low pass filter 6.
mpu9250: check accelerometer low pass filter ok.
mpu9250: set accelerometer low pass filter 7.
mpu9250: check accelerometer low pass filter ok.
mpu9250: mpu9250_set_low_power_accel_output_rate/mpu9250_get_low_power_accel_output_rate test.
mpu9250: set low power accel output rate 0.24Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 0.49Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 0.98Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 1.95Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 3.91Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 7.81Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 15.63Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 31.25Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 62.50Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 125Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 250Hz.
mpu9250: check low power accel output rate ok.
mpu9250: set low power accel output rate 500Hz.
mpu9250: check low power accel output rate ok.
mpu9250: mpu9250_set_wake_on_motion/mpu9250_get_wake_on_motion test.
mpu9250: enable wake on motion.
mpu9250: check wake on motion ok.
mpu9250: disable wake on motion.
mpu9250: check wake on motion ok.
mpu9250: mpu9250_set_accel_compare_with_previous_sample/mpu9250_get_accel_compare_with_previous_sample test.
mpu9250: enable accel compare with previous sample.
mpu9250: check accel compare with previous sample ok.
mpu9250: disable accel compare with previous sample.
mpu9250: check accel compare with previous sample ok.
mpu9250: mpu9250_set_accelerometer_x_offset/mpu9250_get_accelerometer_x_offset test.
mpu9250: set accelerometer x offset 11377.
mpu9250: check accelerometer x offset ok.
mpu9250: mpu9250_set_accelerometer_y_offset/mpu9250_get_accelerometer_y_offset test.
mpu9250: set accelerometer y offset -3771.
mpu9250: check accelerometer y offset ok.
mpu9250: mpu9250_set_accelerometer_z_offset/mpu9250_get_accelerometer_z_offset test.
mpu9250: set accelerometer z offset 5667.
mpu9250: check accelerometer z offset ok.
mpu9250: mpu9250_accelerometer_offset_convert_to_register/mpu9250_accelerometer_offset_convert_to_data test.
mpu9250: accelerometer offset convert to register 94.90.
mpu9250: check accelerometer offset 94.08.
mpu9250: mpu9250_set_gyro_x_offset/mpu9250_get_gyro_x_offset test.
mpu9250: set gyro x offset 6044.
mpu9250: check gyro x offset ok.
mpu9250: mpu9250_set_gyro_y_offset/mpu9250_get_gyro_y_offset test.
mpu9250: set gyro y offset -26997.
mpu9250: check gyro y offset ok.
mpu9250: mpu9250_set_gyro_z_offset/mpu9250_get_gyro_z_offset test.
mpu9250: set gyro z offset 28541.
mpu9250: check gyro z offset ok.
mpu9250: mpu9250_gyro_offset_convert_to_register/mpu9250_gyro_offset_convert_to_data test.
mpu9250: gyro offset convert to register 47.80.
mpu9250: check gyro offset 47.79.
mpu9250: magnetometer test.
mpu9250: mpu9250_mag_get_info test.
mpu9250: mag information is 0x9A.
mpu9250: mpu9250_mag_get_status1 test.
mpu9250: mag status1 is 0x00.
mpu9250: mpu9250_mag_get_status2 test.
mpu9250: mag status2 is 0x00.
mpu9250: mpu9250_mag_set_mode/mpu9250_mag_get_mode test.
mpu9250: set the single measurement mode.
mpu9250: check mag mode ok.
mpu9250: set the continuous measurement mode 1.
mpu9250: check mag mode ok.
mpu9250: set the continuous measurement mode 2.
mpu9250: check mag mode ok.
mpu9250: set the external trigger measurement mode.
mpu9250: check mag mode ok.
mpu9250: set the self test mode.
mpu9250: check mag mode ok.
mpu9250: set the fuse rom access mode.
mpu9250: check mag mode ok.
mpu9250: set the power down mode.
mpu9250: check mag mode ok.
mpu9250: mpu9250_mag_set_bits/mpu9250_mag_get_bits test.
mpu9250: set the 14 bits mode.
mpu9250: check mag bits ok.
mpu9250: set the 16 bits mode.
mpu9250: check mag bits ok.
mpu9250: mpu9250_mag_set_reset/mpu9250_mag_get_reset test.
mpu9250: disable the mag reset.
mpu9250: check reset ok.
mpu9250: mpu9250_mag_set_self_test/mpu9250_mag_get_self_test test.
mpu9250: disable mag self test.
mpu9250: check mag self test ok.
mpu9250: mpu9250_mag_get_asa test.
mpu9250: mag asax: 0xB1 asay: 0xB3 asaz: 0xA6.
mpu9250: finish register test.
```

```shell
mpu9250 -t read --addr=0 --interface=iic --times=3

mpu9250: chip is TDK MPU9250.
mpu9250: manufacturer is TDK.
mpu9250: interface is IIC SPI.
mpu9250: driver version is 1.0.
mpu9250: min supply voltage is 2.4V.
mpu9250: max supply voltage is 3.6V.
mpu9250: max current is 3.70mA.
mpu9250: max temperature is 85.0C.
mpu9250: min temperature is -40.0C.
mpu9250: start read test.
mpu9250: set accelerometer range 2g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: set accelerometer range 4g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: set accelerometer range 8g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: set accelerometer range 16g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.38g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: acc x is -0.34g.
mpu9250: acc y is -0.88g.
mpu9250: acc z is -0.39g.
mpu9250: set gyroscope range 250dps.
mpu9250: gyro x is -2.73dps.
mpu9250: gyro y is -5.93dps.
mpu9250: gyro z is -0.38dps.
mpu9250: gyro x is -2.54dps.
mpu9250: gyro y is -6.05dps.
mpu9250: gyro z is -0.37dps.
mpu9250: gyro x is -2.58dps.
mpu9250: gyro y is -6.00dps.
mpu9250: gyro z is -0.40dps.
mpu9250: set gyroscope range 500dps.
mpu9250: gyro x is -2.64dps.
mpu9250: gyro y is -6.05dps.
mpu9250: gyro z is -0.31dps.
mpu9250: gyro x is -2.76dps.
mpu9250: gyro y is -6.14dps.
mpu9250: gyro z is -0.38dps.
mpu9250: gyro x is -2.75dps.
mpu9250: gyro y is -6.03dps.
mpu9250: gyro z is -0.32dps.
mpu9250: set gyroscope range 1000dps.
mpu9250: gyro x is -2.71dps.
mpu9250: gyro y is -6.07dps.
mpu9250: gyro z is -0.40dps.
mpu9250: gyro x is -2.44dps.
mpu9250: gyro y is -6.07dps.
mpu9250: gyro z is -0.30dps.
mpu9250: gyro x is -2.65dps.
mpu9250: gyro y is -6.10dps.
mpu9250: gyro z is -0.27dps.
mpu9250: set gyroscope range 2000dps.
mpu9250: gyro x is -2.74dps.
mpu9250: gyro y is -6.04dps.
mpu9250: gyro z is -0.30dps.
mpu9250: gyro x is -2.56dps.
mpu9250: gyro y is -6.10dps.
mpu9250: gyro z is -0.37dps.
mpu9250: gyro x is -2.50dps.
mpu9250: gyro y is -6.10dps.
mpu9250: gyro z is -0.30dps.
mpu9250: read temperature.
mpu9250: temperature 30.72C.
mpu9250: temperature 30.73C.
mpu9250: temperature 30.72C.
mpu9250: read magnetometer.
mpu9250: set magnetometer 14 bits.
mpu9250: mag x is 99.36uT.
mpu9250: mag y is -39.57uT.
mpu9250: mag z is 29.63uT.
mpu9250: mag x is 97.93uT.
mpu9250: mag y is -40.29uT.
mpu9250: mag z is 28.94uT.
mpu9250: mag x is 97.22uT.
mpu9250: mag y is -42.45uT.
mpu9250: mag z is 28.94uT.
mpu9250: set magnetometer 16 bits.
mpu9250: mag x is 97.93uT.
mpu9250: mag y is -40.29uT.
mpu9250: mag z is 29.29uT.
mpu9250: mag x is 99.01uT.
mpu9250: mag y is -39.57uT.
mpu9250: mag z is 29.63uT.
mpu9250: mag x is 99.36uT.
mpu9250: mag y is -39.93uT.
mpu9250: mag z is 27.91uT.
mpu9250: mpu9250 mag read test.
mpu9250: mag x is 98.65uT.
mpu9250: mag y is -41.01uT.
mpu9250: mag z is 29.63uT.
mpu9250: mag x is 99.54uT.
mpu9250: mag y is -41.91uT.
mpu9250: mag z is 29.80uT.
mpu9250: mag x is 98.65uT.
mpu9250: mag y is -40.65uT.
mpu9250: mag z is 29.63uT.
mpu9250: finish read test.
```

```shell
mpu9250 -t fifo --addr=0 --interface=iic --times=3

mpu9250: chip is TDK MPU9250.
mpu9250: manufacturer is TDK.
mpu9250: interface is IIC SPI.
mpu9250: driver version is 1.0.
mpu9250: min supply voltage is 2.4V.
mpu9250: max supply voltage is 3.6V.
mpu9250: max current is 3.70mA.
mpu9250: max temperature is 85.0C.
mpu9250: min temperature is -40.0C.
mpu9250: start fifo test.
mpu9250: fifo 48.
mpu9250: acc x[0] is -0.09g.
mpu9250: acc y[0] is 0.07g.
mpu9250: acc z[0] is 0.97g.
mpu9250: gyro x[0] is -5.43dps.
mpu9250: gyro y[0] is -7.56dps.
mpu9250: gyro z[0] is -0.67dps.
mpu9250: mag x[0] is 2.86uT.
mpu9250: mag y[0] is -58.64uT.
mpu9250: mag z[0] is 93.37uT.
mpu9250: fifo 37.
mpu9250: acc x[0] is -0.07g.
mpu9250: acc y[0] is 0.07g.
mpu9250: acc z[0] is 0.93g.
mpu9250: gyro x[0] is -1.52dps.
mpu9250: gyro y[0] is -5.61dps.
mpu9250: gyro z[0] is -0.12dps.
mpu9250: mag x[0] is 3.40uT.
mpu9250: mag y[0] is -60.62uT.
mpu9250: mag z[0] is 92.85uT.
mpu9250: fifo 35.
mpu9250: acc x[0] is -0.08g.
mpu9250: acc y[0] is 0.06g.
mpu9250: acc z[0] is 0.95g.
mpu9250: gyro x[0] is -4.63dps.
mpu9250: gyro y[0] is -7.13dps.
mpu9250: gyro z[0] is -0.98dps.
mpu9250: mag x[0] is 4.29uT.
mpu9250: mag y[0] is -59.36uT.
mpu9250: mag z[0] is 94.75uT.
mpu9250: finish fifo test.
```

```shell
mpu9250 -t dmp --addr=0 --interface=iic --times=3

mpu9250: chip is TDK MPU9250.
mpu9250: manufacturer is TDK.
mpu9250: interface is IIC SPI.
mpu9250: driver version is 1.0.
mpu9250: min supply voltage is 2.4V.
mpu9250: max supply voltage is 3.6V.
mpu9250: max current is 3.70mA.
mpu9250: max temperature is 85.0C.
mpu9250: min temperature is -40.0C.
mpu9250: start dmp read test.
mpu9250: load dmp firmware.
mpu9250: load dmp firmware successful .
mpu9250: mpu9250_dmp_set_pedometer_walk_time/mpu9250_dmp_get_pedometer_walk_time test.
mpu9250: dmp set pedometer walk time 200 ms.
mpu9250: check pedometer walk time ok.
mpu9250: mpu9250_dmp_set_pedometer_step_count/mpu9250_dmp_get_pedometer_step_count test.
mpu9250: dmp set pedometer step count 768.
mpu9250: check pedometer step count ok.
mpu9250: mpu9250_dmp_set_shake_reject_timeout/mpu9250_dmp_get_shake_reject_timeout test.
mpu9250: dmp set shake reject timeout 10 ms.
mpu9250: check shake reject timeout ok.
mpu9250: mpu9250_dmp_set_shake_reject_time/mpu9250_dmp_get_shake_reject_time test.
mpu9250: dmp set shake reject time 40 ms.
mpu9250: check shake reject time ok.
mpu9250: mpu9250_dmp_set_shake_reject_thresh/mpu9250_dmp_get_shake_reject_thresh test.
mpu9250: set shake reject thresh 0 dps.
mpu9250: check shake reject thresh ok.
mpu9250: mpu9250_dmp_set_tap_time_multi/mpu9250_dmp_get_tap_time_multi test.
mpu9250: dmp set tap time multi 500 ms.
mpu9250: check tap time multi ok.
mpu9250: mpu9250_dmp_set_tap_time/mpu9250_dmp_get_tap_time test.
mpu9250: dmp set tap time 100 ms.
mpu9250: check tap time ok.
mpu9250: mpu9250_dmp_set_min_tap_count/mpu9250_dmp_get_min_tap_count test.
mpu9250: dmp set min tap count 1.
mpu9250: check min tap count ok.
mpu9250: mpu9250_dmp_set_tap_axes/mpu9250_dmp_get_tap_axes test.
mpu9250: disable tap axes x.
mpu9250: check tap axes ok.
mpu9250: enable tap axes x.
mpu9250: check tap axes ok.
mpu9250: disable tap axes y.
mpu9250: check tap axes ok.
mpu9250: enable tap axes y.
mpu9250: check tap axes ok.
mpu9250: disable tap axes z.
mpu9250: check tap axes ok.
mpu9250: enable tap axes z.
mpu9250: check tap axes ok.
mpu9250: mpu9250_dmp_set_tap_thresh/mpu9250_dmp_get_tap_thresh test.
mpu9250: dmp set tap thresh x 250 mg/ms.
mpu9250: check tap thresh ok.
mpu9250: dmp set tap thresh y 250 mg/ms.
mpu9250: check tap thresh ok.
mpu9250: dmp set tap thresh z 250 mg/ms.
mpu9250: check tap thresh ok.
mpu9250: mpu9250_dmp_set_fifo_rate/mpu9250_dmp_get_fifo_rate test.
mpu9250: dmp set fifo rate 200Hz.
mpu9250: check fifo rate ok.
mpu9250: mpu9250_dmp_set_gyro_calibrate test.
mpu9250: enable gyro calibrate.
mpu9250: disable gyro calibrate.
mpu9250: mpu9250_dmp_set_3x_quaternion test.
mpu9250: enable 3x quaternion.
mpu9250: disable 3x quaternion.
mpu9250: mpu9250_dmp_set_6x_quaternion test.
mpu9250: enable 6x quaternion.
mpu9250: disable 6x quaternion.
mpu9250: mpu9250_dmp_set_interrupt_mode test.
mpu9250: dmp set gesture interrupt mode.
mpu9250: dmp set gesture continuous mode.
mpu9250: mpu9250_dmp_set_orientation test.
mpu9250: set the dmp orientation.
mpu9250: mpu9250_dmp_set_feature test.
mpu9250: enable feature 6x quat.
mpu9250: enable feature tap.
mpu9250: enable feature pedometer.
mpu9250: enable feature orient.
mpu9250: enable feature send raw accel.
mpu9250: enable feature send cal gyro.
mpu9250: enable feature gyro cal.
mpu9250: fifo 3.
mpu9250: pitch[0] is -0.06deg.
mpu9250: roll[0] is 1.16deg.
mpu9250: yaw[0] is -0.02deg.
mpu9250: acc x[0] is -0.09g.
mpu9250: acc y[0] is 0.01g.
mpu9250: acc z[0] is 0.95g.
mpu9250: gyro x[0] is -0.12dps.
mpu9250: gyro y[0] is -0.06dps.
mpu9250: gyro z[0] is -0.06dps.
mpu9250: fifo 6.
mpu9250: pitch[0] is -0.06deg.
mpu9250: roll[0] is 1.14deg.
mpu9250: yaw[0] is -0.02deg.
mpu9250: acc x[0] is -0.08g.
mpu9250: acc y[0] is 0.01g.
mpu9250: acc z[0] is 0.94g.
mpu9250: gyro x[0] is -0.06dps.
mpu9250: gyro y[0] is 0.00dps.
mpu9250: gyro z[0] is -0.06dps.
mpu9250: fifo 7.
mpu9250: pitch[0] is -0.06deg.
mpu9250: roll[0] is 1.12deg.
mpu9250: yaw[0] is -0.02deg.
mpu9250: acc x[0] is -0.09g.
mpu9250: acc y[0] is 0.01g.
mpu9250: acc z[0] is 0.95g.
mpu9250: gyro x[0] is 0.00dps.
mpu9250: gyro y[0] is 0.00dps.
mpu9250: gyro z[0] is -0.06dps.
mpu9250: finish dmp read test.
```

```shell
mpu9250 -t motion --addr=0 --interface=iic

mpu9250: chip is TDK MPU9250.
mpu9250: manufacturer is TDK.
mpu9250: interface is IIC SPI.
mpu9250: driver version is 1.0.
mpu9250: min supply voltage is 2.4V.
mpu9250: max supply voltage is 3.6V.
mpu9250: max current is 3.70mA.
mpu9250: max temperature is 85.0C.
mpu9250: min temperature is -40.0C.
mpu9250: start dmp tap orient motion test.
mpu9250: load dmp firmware.
mpu9250: load dmp firmware successful .
mpu9250: mpu9250_dmp_set_pedometer_walk_time/mpu9250_dmp_get_pedometer_walk_time test.
mpu9250: dmp set pedometer walk time 200 ms.
mpu9250: check pedometer walk time ok.
mpu9250: mpu9250_dmp_set_pedometer_step_count/mpu9250_dmp_get_pedometer_step_count test.
mpu9250: dmp set pedometer step count 409.
mpu9250: check pedometer step count ok.
mpu9250: mpu9250_dmp_set_shake_reject_timeout/mpu9250_dmp_get_shake_reject_timeout test.
mpu9250: dmp set shake reject timeout 10 ms.
mpu9250: check shake reject timeout ok.
mpu9250: mpu9250_dmp_set_shake_reject_time/mpu9250_dmp_get_shake_reject_time test.
mpu9250: dmp set shake reject time 40 ms.
mpu9250: check shake reject time ok.
mpu9250: mpu9250_dmp_set_shake_reject_thresh/mpu9250_dmp_get_shake_reject_thresh test.
mpu9250: set shake reject thresh 200 dps.
mpu9250: check shake reject thresh error.
mpu9250: mpu9250_dmp_set_tap_time_multi/mpu9250_dmp_get_tap_time_multi test.
mpu9250: dmp set tap time multi 200 ms.
mpu9250: check tap time multi ok.
mpu9250: mpu9250_dmp_set_tap_time/mpu9250_dmp_get_tap_time test.
mpu9250: dmp set tap time 100 ms.
mpu9250: check tap time ok.
mpu9250: mpu9250_dmp_set_min_tap_count/mpu9250_dmp_get_min_tap_count test.
mpu9250: dmp set min tap count 1.
mpu9250: check min tap count ok.
mpu9250: mpu9250_dmp_set_tap_axes/mpu9250_dmp_get_tap_axes test.
mpu9250: disable tap axes x.
mpu9250: check tap axes ok.
mpu9250: enable tap axes x.
mpu9250: check tap axes ok.
mpu9250: disable tap axes y.
mpu9250: check tap axes ok.
mpu9250: enable tap axes y.
mpu9250: check tap axes ok.
mpu9250: disable tap axes z.
mpu9250: check tap axes ok.
mpu9250: enable tap axes z.
mpu9250: check tap axes ok.
mpu9250: mpu9250_dmp_set_tap_thresh/mpu9250_dmp_get_tap_thresh test.
mpu9250: dmp set tap thresh x 250 mg/ms.
mpu9250: check tap thresh ok.
mpu9250: dmp set tap thresh y 250 mg/ms.
mpu9250: check tap thresh ok.
mpu9250: dmp set tap thresh z 250 mg/ms.
mpu9250: check tap thresh ok.
mpu9250: mpu9250_dmp_set_fifo_rate/mpu9250_dmp_get_fifo_rate test.
mpu9250: dmp set fifo rate 50Hz.
mpu9250: check fifo rate ok.
mpu9250: mpu9250_dmp_set_gyro_calibrate test.
mpu9250: enable gyro calibrate.
mpu9250: disable gyro calibrate.
mpu9250: mpu9250_dmp_set_3x_quaternion test.
mpu9250: enable 3x quaternion.
mpu9250: disable 3x quaternion.
mpu9250: mpu9250_dmp_set_6x_quaternion test.
mpu9250: enable 6x quaternion.
mpu9250: disable 6x quaternion.
mpu9250: mpu9250_dmp_set_interrupt_mode test.
mpu9250: dmp set gesture interrupt mode.
mpu9250: dmp set gesture continuous mode.
mpu9250: mpu9250_dmp_set_orientation test.
mpu9250: set the dmp orientation.
mpu9250: mpu9250_dmp_set_feature test.
mpu9250: enable feature 6x quat.
mpu9250: enable feature tap.
mpu9250: enable feature pedometer.
mpu9250: enable feature orient.
mpu9250: enable feature send raw accel.
mpu9250: enable feature send cal gyro.
mpu9250: enable feature gyro cal.
mpu9250: irq motion.
mpu9250: tap irq x up with 1.
mpu9250: orient irq reverse portrait.
mpu9250: finish dmp tap orient motion test.
```

```shell
mpu9250 -t pedometer --addr=0 --interface=iic --times=3

mpu9250: chip is TDK MPU9250.
mpu9250: manufacturer is TDK.
mpu9250: interface is IIC SPI.
mpu9250: driver version is 1.0.
mpu9250: min supply voltage is 2.4V.
mpu9250: max supply voltage is 3.6V.
mpu9250: max current is 3.70mA.
mpu9250: max temperature is 85.0C.
mpu9250: min temperature is -40.0C.
mpu9250: start dmp pedometer test.
mpu9250: load dmp firmware.
mpu9250: load dmp firmware successful .
mpu9250: mpu9250_dmp_set_pedometer_walk_time/mpu9250_dmp_get_pedometer_walk_time test.
mpu9250: dmp set pedometer walk time 200 ms.
mpu9250: check pedometer walk time ok.
mpu9250: mpu9250_dmp_set_pedometer_step_count/mpu9250_dmp_get_pedometer_step_count test.
mpu9250: dmp set pedometer step count 383.
mpu9250: check pedometer step count ok.
mpu9250: dmp set gesture continuous mode.
mpu9250: mpu9250_dmp_set_fifo_rate/mpu9250_dmp_get_fifo_rate test.
mpu9250: dmp set fifo rate 50Hz.
mpu9250: check fifo rate ok.
mpu9250: mpu9250_dmp_set_feature test.
mpu9250: enable feature pedometer.
mpu9250: pedometer step count is 390.
mpu9250: pedometer step count is 391.
mpu9250: pedometer step count is 392.
mpu9250: finish dmp pedometer test.
```

```shell
mpu9250 -e read --addr=0 --interface=iic --times=3

mpu9250: 1/3.
mpu9250: acc x is -0.11g.
mpu9250: acc y is -0.96g.
mpu9250: acc z is -0.32g.
mpu9250: gyro x is -2.62dps.
mpu9250: gyro y is -6.10dps.
mpu9250: gyro z is -0.37dps.
mpu9250: mag x is 102.40uT.
mpu9250: mag y is -54.14uT.
mpu9250: mag z is 36.35uT.
mpu9250: temperature 31.08C.
mpu9250: 2/3.
mpu9250: acc x is -0.10g.
mpu9250: acc y is -0.96g.
mpu9250: acc z is -0.31g.
mpu9250: gyro x is -2.50dps.
mpu9250: gyro y is -6.16dps.
mpu9250: gyro z is -0.24dps.
mpu9250: mag x is 101.69uT.
mpu9250: mag y is -55.94uT.
mpu9250: mag z is 37.38uT.
mpu9250: temperature 31.59C.
mpu9250: 3/3.
mpu9250: acc x is -0.11g.
mpu9250: acc y is -0.96g.
mpu9250: acc z is -0.31g.
mpu9250: gyro x is -2.50dps.
mpu9250: gyro y is -6.16dps.
mpu9250: gyro z is -0.30dps.
mpu9250: mag x is 102.94uT.
mpu9250: mag y is -56.12uT.
mpu9250: mag z is 36.52uT.
mpu9250: temperature 31.77C.
```

```shell
mpu9250 -e fifo --addr=0 --interface=iic --times=3

mpu9250: 1/3.
mpu9250: fifo 48.
mpu9250: acc x[0] is -0.07g.
mpu9250: acc y[0] is 0.03g.
mpu9250: acc z[0] is 0.95g.
mpu9250: gyro x[0] is -2.50dps.
mpu9250: gyro y[0] is -6.52dps.
mpu9250: gyro z[0] is -0.61dps.
mpu9250: mag x[0] is 29.84uT.
mpu9250: mag y[0] is -59.90uT.
mpu9250: mag z[0] is 83.89uT.
mpu9250: 2/3.
mpu9250: fifo 38.
mpu9250: acc x[0] is -0.07g.
mpu9250: acc y[0] is 0.03g.
mpu9250: acc z[0] is 0.96g.
mpu9250: gyro x[0] is -2.56dps.
mpu9250: gyro y[0] is -6.71dps.
mpu9250: gyro z[0] is -0.49dps.
mpu9250: mag x[0] is 31.99uT.
mpu9250: mag y[0] is -59.18uT.
mpu9250: mag z[0] is 84.58uT.
mpu9250: 3/3.
mpu9250: fifo 35.
mpu9250: acc x[0] is -0.08g.
mpu9250: acc y[0] is 0.03g.
mpu9250: acc z[0] is 0.96g.
mpu9250: gyro x[0] is -2.44dps.
mpu9250: gyro y[0] is -6.71dps.
mpu9250: gyro z[0] is -0.61dps.
mpu9250: mag x[0] is 30.74uT.
mpu9250: mag y[0] is -59.00uT.
mpu9250: mag z[0] is 85.44uT.
```

```shell
mpu9250 -e dmp --addr=0 --interface=iic --times=3

mpu9250: 1/3.
mpu9250: fifo 6.
mpu9250: pitch[0] is 0.01deg.
mpu9250: roll[0] is 0.82deg.
mpu9250: yaw[0] is -0.03deg.
mpu9250: acc x[0] is -0.08g.
mpu9250: acc y[0] is 0.02g.
mpu9250: acc z[0] is 0.95g.
mpu9250: gyro x[0] is -0.12dps.
mpu9250: gyro y[0] is -0.12dps.
mpu9250: gyro z[0] is -0.06dps.
mpu9250: 2/3.
mpu9250: fifo 7.
mpu9250: pitch[0] is -0.01deg.
mpu9250: roll[0] is 0.75deg.
mpu9250: yaw[0] is -0.03deg.
mpu9250: acc x[0] is -0.07g.
mpu9250: acc y[0] is 0.02g.
mpu9250: acc z[0] is 0.95g.
mpu9250: gyro x[0] is -0.06dps.
mpu9250: gyro y[0] is -0.18dps.
mpu9250: gyro z[0] is -0.12dps.
mpu9250: 3/3.
mpu9250: fifo 8.
mpu9250: pitch[0] is -0.04deg.
mpu9250: roll[0] is 0.69deg.
mpu9250: yaw[0] is -0.04deg.
mpu9250: acc x[0] is -0.08g.
mpu9250: acc y[0] is 0.02g.
mpu9250: acc z[0] is 0.95g.
mpu9250: gyro x[0] is -0.06dps.
mpu9250: gyro y[0] is -0.18dps.
mpu9250: gyro z[0] is -0.06dps.
```

```shell
mpu9250 -e motion --addr=0 --interface=iic

mpu9250: irq motion.
mpu9250: irq dmp
mpu9250: irq data ready
mpu9250: tap irq x up with 3.
mpu9250: orient irq landscape.
mpu9250: finish dmp tap orient motion.
```

```shell
mpu9250 -e pedometer --addr=0 --interface=iic --times=3

mpu9250: pedometer step count is 7.
mpu9250: pedometer step count is 8.
mpu9250: pedometer step count is 9.
```

```shell
mpu9250 -h

Usage:
  mpu9250 (-i | --information)
  mpu9250 (-h | --help)
  mpu9250 (-p | --port)
  mpu9250 (-t reg | --test=reg) [--addr=<0 | 1>] [--interface=<iic | spi>]
  mpu9250 (-t read | --test=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  mpu9250 (-t fifo | --test=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  mpu9250 (-t dmp | --test=dmp) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  mpu9250 (-t motion | --test=motion) [--addr=<0 | 1>] [--interface=<iic | spi>]
  mpu9250 (-t pedometer | --test=pedometer) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  mpu9250 (-e read | --example=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  mpu9250 (-e fifo | --example=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  mpu9250 (-e dmp | --example=dmp) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  mpu9250 (-e motion | --example=motion) [--addr=<0 | 1>] [--interface=<iic | spi>]
  mpu9250 (-e pedometer | --example=pedometer) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]

Options:
      --addr=<0 | 1>      Set the addr pin.([default: 0])
  -e <read | fifo | dmp | motion | pedometer>, --example=<read | fifo | dmp | motion | pedometer>
                          Run the driver example.
  -h, --help              Show the help.
  -i, --information       Show the chip information.
      --interface=<iic | spi>
                          Set the chip interface.([default: iic])
  -p, --port              Display the pin connections of the current board.
  -t <reg | read | fifo | dmp | motion | pedometer>, --test=<reg | read | fifo | dmp | motion | pedometer>
                          Run the driver test.
      --times=<num>       Set the running times.([default: 3])
```

