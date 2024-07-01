[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MPU9250

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mpu9250/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MPU9250 は、1 つの QFN パッケージに統合された 2 つのダイで構成されるマルチチップ モジュール (MCM) です。 1 つのダイには、3 軸ジャイロスコープと 3 軸加速度計が収納されています。もう一方のダイには、旭化成エレクトロニクス株式会社の AK8963 3 軸磁力計が収納されています。したがって、MPU9250 は、3 軸ジャイロスコープ、3 軸加速度計、3 軸磁力計、デジタル モーション プロセッサ™ (DMP) を組み合わせた 9 軸 MotionTracking デバイスであり、すべてピン互換のアップグレードとして利用可能な 3x3x1mm の小型パッケージに収められています。専用の I2C センサー バスを使用して、MPU9250 は完全な 9 軸 MotionFusion™ 出力を直接提供します。 MPU9250 MotionTracking デバイスは、9 軸統合、オンチップ MotionFusion™、およびランタイム キャリブレーション ファームウェアを備えており、製造業者はディスクリート デバイスのコストがかかり複雑な選択、認定、およびシステム レベルの統合を排除し、消費者に最適なモーション パフォーマンスを保証できます。 MPU9250 は、補助 I2C ポートで、圧力センサーなどの複数の非慣性デジタル センサーとインターフェイスするようにも設計されています。加速度計の出力をデジタル化するための ADC と、磁力計の出力をデジタル化するための 3 つの 16 ビット ADC。速い動きと遅い動きの両方を正確に追跡するために、パーツはユーザーがプログラム可能なジャイロスコープのフルスケール範囲 ±250、±500、±1000、および ±2000°/秒 (dps)、ユーザーがプログラム可能な加速度センサーのフルスケールを備えています。 ±2g、±4g、±8g、および±16gの範囲、および±4800μTの磁力計フルスケール範囲。

LibDriver MPU9250 は、LibDriver によって起動される mpu9250 のフル機能ドライバーです。 加速度の読み取り、角速度の読み取り、磁力計の読み取り、姿勢角の読み取り、dmp の読み取り、タップ検出などの機能を提供します。 LibDriver は MISRA 準拠です。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example dmp](#example-dmp)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver MPU9250のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver MPU9250用のプラットフォームに依存しないIIC, SPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver MPU9250ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver MPU9250プログラミング例が含まれています。

/ docディレクトリには、LibDriver MPU9250オフラインドキュメントが含まれています。

/ datasheetディレクトリには、MPU9250データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIIC, SPIバステンプレートを参照して、指定したプラットフォームのIIC, SPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

#### example basic

```C
#include "driver_mpu9250_basic.h"

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

```c
#include "driver_mpu9250_fifo.h"

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
#include "driver_mpu9250_dmp.h"

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/mpu9250/index.html](https://www.libdriver.com/docs/mpu9250/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。