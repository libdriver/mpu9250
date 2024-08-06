[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MPU9250

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mpu9250/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MPU9250是一個多晶片模塊，它由集成到單個QFN封裝中的兩個晶片組成。 一個晶片封裝3軸陀螺儀和3軸加速度計，另一個晶片封裝3軸磁力計AK8963。 MPU9250是一種9軸運動跟踪設備，它在3.0x3.0x1.0mm封裝中集成了3軸陀螺儀、3軸加速度計、3軸磁力計和數位運動處理器（DMP）。 MPU9250通過其專用的IIC感測器匯流排直接提供完整的9軸運動融合輸出。 擁有9軸集成、數位運動處理器（（DMP）和運行時校準固件的MPU9250運動跟踪設備，可以使製造商降低離散運動跟踪設備昂貴又複雜的選擇、鑒定和系統集成成本，保證消費者產品的最佳運動跟踪效能。 MPU9250還設計用於在其輔助IIC匯流排上與多個非慣性數位感測器（如壓力感測器）的介面並具備自動FIFO採集功能。 MPU9250具有3個用於陀螺儀輸出的16比特ADC、3個用於加速度計輸出的16比特ADC和3個用於磁力計輸出的16比特ADC。 用戶可程式設計陀螺儀滿標度範圍為±250dps、±500dps、±1000dps和±2000dps，加速度計滿標度範圍為±2g、±4g、±8g和±16g，磁力計滿標度範圍為±4800μT。

LibDriver MPU9250是LibDriver推出的MPU9250的全功能驅動，該驅動提供加速度讀取、角速度讀取、磁力計讀取、姿態角讀取、DMP讀取和敲擊檢測等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example dmp](#example-dmp)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver MPU9250的源文件。

/interface目錄包含了LibDriver MPU9250與平台無關的IIC，SPI總線模板。

/test目錄包含了LibDriver MPU9250驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver MPU9250編程範例。

/doc目錄包含了LibDriver MPU9250離線文檔。

/datasheet目錄包含了MPU9250數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC，SPI總線模板，完成指定平台的IIC，SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

#### example basic

```C
uint8_t res;
uint32_t i;
uint32_t times;
float g[3];
float dps[3];
float ut[3];
float degrees;
mpu9250_address_t addr;

/* init */
addr = MPU9250_ADDRESS_AD0_LOW;
res = mpu9250_basic_init(MPU9250_INTERFACE_IIC, addr);
if (res != 0)
{
    return 1;
}

...
    
/* read all */
times = 3;
for (i = 0; i < times; i++)
{
    /* read */
    if (mpu9250_basic_read(g, dps, ut) != 0)
    {
        (void)mpu9250_basic_deinit();

        return 1;
    }

    ...
        
    if (mpu9250_basic_read_temperature(&degrees) != 0)
    {
        (void)mpu9250_basic_deinit();

        return 1;
    }

    ...
        
    /* output */
    mpu9250_interface_debug_print("mpu9250: %d/%d.\n", i + 1, times);
    mpu9250_interface_debug_print("mpu9250: acc x is %0.2fg.\n", g[0]);
    mpu9250_interface_debug_print("mpu9250: acc y is %0.2fg.\n", g[1]);
    mpu9250_interface_debug_print("mpu9250: acc z is %0.2fg.\n", g[2]);
    mpu9250_interface_debug_print("mpu9250: gyro x is %0.2fdps.\n", dps[0]);
    mpu9250_interface_debug_print("mpu9250: gyro y is %0.2fdps.\n", dps[1]);
    mpu9250_interface_debug_print("mpu9250: gyro z is %0.2fdps.\n", dps[2]);
    mpu9250_interface_debug_print("mpu9250: mag x is %0.2fuT.\n", ut[0]);
    mpu9250_interface_debug_print("mpu9250: mag y is %0.2fuT.\n", ut[1]);
    mpu9250_interface_debug_print("mpu9250: mag z is %0.2fuT.\n", ut[2]);
    mpu9250_interface_debug_print("mpu9250: temperature %0.2fC.\n", degrees);

    ...
        
    /* delay 1000 ms */
    mpu9250_interface_delay_ms(1000);

    ...
}

...
    
/* deinit */
(void)mpu9250_basic_deinit();

return 0;
```

#### example fifo

```C
uint32_t i;
uint32_t times;
uint16_t len;
uint8_t (*g_gpio_irq)(void) = NULL;
static int16_t gs_accel_raw[128][3];
static float gs_accel_g[128][3];
static int16_t gs_gyro_raw[128][3];
static float gs_gyro_dps[128][3];
atic int16_t gs_mag_raw[128][3];
static float gs_mag_ut[128][3];
mpu9250_address_t addr;

/* gpio init */
if (gpio_interrupt_init() != 0)
{
    return 1;
}
g_gpio_irq = mpu9250_fifo_irq_handler;

/* init */
addr = MPU9250_ADDRESS_AD0_LOW;
if (mpu9250_fifo_init(MPU9250_INTERFACE_IIC, addr) != 0)
{
    g_gpio_irq = NULL;
    (void)gpio_interrupt_deinit();

    return 1;
}

/* delay 100 ms */
mpu9250_interface_delay_ms(100);

...

times = 3;
for (i = 0; i < times; i++)
{
    len = 128;

    /* read */
    if (mpu9250_fifo_read(gs_accel_raw, gs_accel_g,
                          gs_gyro_raw, gs_gyro_dps, gs_mag_raw, gs_mag_ut, &len) != 0)
    {
        (void)mpu9250_fifo_deinit();
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();

        return 1;
    }
    
    ...
        
    /* output */
    mpu9250_interface_debug_print("mpu9250: %d/%d.\n", i + 1, times);
    mpu9250_interface_debug_print("mpu9250: fifo %d.\n", len);
    mpu9250_interface_debug_print("mpu9250: acc x[0] is %0.2fg.\n", gs_accel_g[0][0]);
    mpu9250_interface_debug_print("mpu9250: acc y[0] is %0.2fg.\n", gs_accel_g[0][1]);
    mpu9250_interface_debug_print("mpu9250: acc z[0] is %0.2fg.\n", gs_accel_g[0][2]);
    mpu9250_interface_debug_print("mpu9250: gyro x[0] is %0.2fdps.\n", gs_gyro_dps[0][0]);
    mpu9250_interface_debug_print("mpu9250: gyro y[0] is %0.2fdps.\n", gs_gyro_dps[0][1]);
    mpu9250_interface_debug_print("mpu9250: gyro z[0] is %0.2fdps.\n", gs_gyro_dps[0][2]);
    mpu9250_interface_debug_print("mpu9250: mag x[0] is %0.2fuT.\n", gs_mag_ut[0][0]);
    mpu9250_interface_debug_print("mpu9250: mag y[0] is %0.2fuT.\n", gs_mag_ut[0][1]);
    mpu9250_interface_debug_print("mpu9250: mag z[0] is %0.2fuT.\n", gs_mag_ut[0][2]);
    
    ...
        
    /* delay 100 ms */
    mpu9250_interface_delay_ms(100);
    
    ...
}

...
    
/* deinit */
(void)mpu9250_fifo_deinit();
g_gpio_irq = NULL;
(void)gpio_interrupt_deinit();

return 0;
```

#### example dmp

```C
uint32_t i;
uint32_t times;
uint32_t cnt;
uint16_t len;
uint8_t (*g_gpio_irq)(void) = NULL;
static int16_t gs_accel_raw[128][3];
static float gs_accel_g[128][3];
static int16_t gs_gyro_raw[128][3];      
static float gs_gyro_dps[128][3];
static int32_t gs_quat[128][4];          
static float gs_pitch[128];              
static float gs_roll[128];                
static float gs_yaw[128];                      
mpu9250_address_t addr;

static void a_receive_callback(uint8_t type)
{
    switch (type)
    {
        case MPU9250_INTERRUPT_MOTION :
        {
            mpu9250_interface_debug_print("mpu9250: irq motion.\n");
            
            break;
        }
        case MPU9250_INTERRUPT_FIFO_OVERFLOW :
        {
            mpu9250_interface_debug_print("mpu9250: irq fifo overflow.\n");
            
            break;
        }
        case MPU9250_INTERRUPT_FSYNC_INT :
        {
            mpu9250_interface_debug_print("mpu9250: irq fsync int.\n");
            
            break;
        }
        case MPU9250_INTERRUPT_DMP :
        {
            mpu9250_interface_debug_print("mpu9250: irq dmp\n");
            
            break;
        }
        case MPU9250_INTERRUPT_DATA_READY :
        {
            mpu9250_interface_debug_print("mpu9250: irq data ready\n");
            
            break;
        }
        default :
        {
            mpu9250_interface_debug_print("mpu9250: irq unknown code.\n");
            
            break;
        }
    }
}

static void a_dmp_tap_callback(uint8_t count, uint8_t direction)
{
    switch (direction)
    {
        case MPU9250_DMP_TAP_X_UP :
        {
            mpu9250_interface_debug_print("mpu9250: tap irq x up with %d.\n", count);
            
            break;
        }
        case MPU9250_DMP_TAP_X_DOWN :
        {
            mpu9250_interface_debug_print("mpu9250: tap irq x down with %d.\n", count);
            
            break;
        }
        case MPU9250_DMP_TAP_Y_UP :
        {
            mpu9250_interface_debug_print("mpu9250: tap irq y up with %d.\n", count);
            
            break;
        }
        case MPU9250_DMP_TAP_Y_DOWN :
        {
            mpu9250_interface_debug_print("mpu9250: tap irq y down with %d.\n", count);
            
            break;
        }
        case MPU9250_DMP_TAP_Z_UP :
        {
            mpu9250_interface_debug_print("mpu9250: tap irq z up with %d.\n", count);
            
            break;
        }
        case MPU9250_DMP_TAP_Z_DOWN :
        {
            mpu9250_interface_debug_print("mpu9250: tap irq z down with %d.\n", count);
            
            break;
        }
        default :
        {
            mpu9250_interface_debug_print("mpu9250: tap irq unknown code.\n");
            
            break;
        }
    }
}

static void a_dmp_orient_callback(uint8_t orientation)
{
    switch (orientation)
    {
        case MPU9250_DMP_ORIENT_PORTRAIT :
        {
            mpu9250_interface_debug_print("mpu9250: orient irq portrait.\n");
            
            break;
        }
        case MPU9250_DMP_ORIENT_LANDSCAPE :
        {
            mpu9250_interface_debug_print("mpu9250: orient irq landscape.\n");
            
            break;
        }
        case MPU9250_DMP_ORIENT_REVERSE_PORTRAIT :
        {
            mpu9250_interface_debug_print("mpu9250: orient irq reverse portrait.\n");
            
            break;
        }
        case MPU9250_DMP_ORIENT_REVERSE_LANDSCAPE :
        {
            mpu9250_interface_debug_print("mpu9250: orient irq reverse landscape.\n");
            
            break;
        }
        default :
        {
            mpu9250_interface_debug_print("mpu9250: orient irq unknown code.\n");
            
            break;
        }
    }
}

/* init */
if (gpio_interrupt_init() != 0)
{
    return 1;
}
g_gpio_irq = mpu9250_dmp_irq_handler;

/* init */
addr = MPU9250_ADDRESS_AD0_LOW;
if (mpu9250_dmp_init(MPU9250_INTERFACE_IIC, addr, a_receive_callback, 
                     a_dmp_tap_callback, a_dmp_orient_callback) != 0)
{
    g_gpio_irq = NULL;
    (void)gpio_interrupt_deinit();

    return 1;
}

/* delay 500 ms */
mpu9250_interface_delay_ms(500);

...
    
times = 3;
for (i = 0; i < times; i++)
{
    len = 128;

    /* read */
    if (mpu9250_dmp_read_all(gs_accel_raw, gs_accel_g,
                             gs_gyro_raw, gs_gyro_dps, 
                             gs_quat,
                             gs_pitch, gs_roll, gs_yaw,
                             &len) != 0)
    {
        (void)mpu9250_dmp_deinit();
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();

        return 1;
    }

    /* output */
    mpu9250_interface_debug_print("mpu9250: %d/%d.\n", i + 1, times);
    mpu9250_interface_debug_print("mpu9250: fifo %d.\n", len);
    mpu9250_interface_debug_print("mpu9250: pitch[0] is %0.2fdeg.\n", gs_pitch[0]);
    mpu9250_interface_debug_print("mpu9250: roll[0] is %0.2fdeg.\n", gs_roll[0]);
    mpu9250_interface_debug_print("mpu9250: yaw[0] is %0.2fdeg.\n", gs_yaw[0]);
    mpu9250_interface_debug_print("mpu9250: acc x[0] is %0.2fg.\n", gs_accel_g[0][0]);
    mpu9250_interface_debug_print("mpu9250: acc y[0] is %0.2fg.\n", gs_accel_g[0][1]);
    mpu9250_interface_debug_print("mpu9250: acc z[0] is %0.2fg.\n", gs_accel_g[0][2]);
    mpu9250_interface_debug_print("mpu9250: gyro x[0] is %0.2fdps.\n", gs_gyro_dps[0][0]);
    mpu9250_interface_debug_print("mpu9250: gyro y[0] is %0.2fdps.\n", gs_gyro_dps[0][1]);
    mpu9250_interface_debug_print("mpu9250: gyro z[0] is %0.2fdps.\n", gs_gyro_dps[0][2]);

    /* delay 500 ms */
    mpu9250_interface_delay_ms(500);
    
    ....
        
    /* get the pedometer step count */
    res = mpu9250_dmp_get_pedometer_counter(&cnt);
    if (res != 0)
    {
        (void)mpu9250_dmp_deinit();
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();

        return 1;
    }
    
    ...
}

...

/* deinit */
(void)mpu9250_dmp_deinit();
g_gpio_irq = NULL;
(void)gpio_interrupt_deinit();

return 0;
```

### 文檔

在線文檔: [https://www.libdriver.com/docs/mpu9250/index.html](https://www.libdriver.com/docs/mpu9250/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。