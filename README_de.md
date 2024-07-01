[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MPU9250

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mpu9250/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

MPU9250 ist ein Multi-Chip-Modul (MCM), das aus zwei Chips besteht, die in einem einzigen QFN-Gehäuse integriert sind. Ein Chip beherbergt das 3-Achsen-Gyroskop und den 3-Achsen-Beschleunigungsmesser. Der andere Chip beherbergt das 3-Achsen-Magnetometer AK8963 von Asahi Kasei Microdevices Corporation. Daher ist der MPU9250 ein 9-Achsen-MotionTracking-Gerät, das ein 3-Achsen-Gyroskop, einen 3-Achsen-Beschleunigungsmesser, einen 3-Achsen-Magnetometer und einen Digital Motion Processor™ (DMP) in einem kleinen 3 x 3 x 1 mm-Gehäuse kombiniert, das als Pin-kompatibles Upgrade erhältlich ist vom MPU6515. Mit seinem dedizierten I2C-Sensorbus bietet der MPU9250 direkt eine vollständige 9-Achsen-MotionFusion™-Ausgabe. Das MPU9250 MotionTracking-Gerät mit seiner 9-Achsen-Integration, On-Chip-MotionFusion™ und Laufzeitkalibrierungsfirmware ermöglicht es Herstellern, die kostspielige und komplexe Auswahl, Qualifizierung und Systemintegration diskreter Geräte zu eliminieren und eine optimale Bewegungsleistung für Verbraucher zu gewährleisten. Der MPU9250 ist auch für die Verbindung mit mehreren nicht-inertialen Digitalsensoren, wie z. B. Drucksensoren, an seinem zusätzlichen I2C-Anschluss ausgelegt. Der MPU9250 verfügt über drei 16-Bit-Analog-Digital-Wandler (ADCs) zur Digitalisierung der Gyroskopausgänge, drei 16-Bit ADCs zum Digitalisieren der Beschleunigungsmesserausgänge und drei 16-Bit-ADCs zum Digitalisieren der Magnetometerausgänge. Zur präzisen Verfolgung sowohl schneller als auch langsamer Bewegungen verfügen die Teile über einen benutzerprogrammierbaren Gyroskop-Vollausschlagbereich von ±250, ±500, ±1000 und ±2000°/s (dps), einen benutzerprogrammierbaren Beschleunigungsmesser-Vollausschlag Bereich von ±2 g, ±4 g, ±8 g und ±16 g und einem Magnetometer-Vollbereich von ±4800 μT.

LibDriver MPU9250 ist der voll funktionsfähige Treiber von mpu9250, der von LibDriver gestartet wurde. Es bietet Beschleunigungsablesung, Winkelgeschwindigkeitsablesung, Magnetometerablesung, Lagewinkelablesung, DMP-Messung, Tap-Erkennung und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example dmp](#example-dmp)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver MPU9250-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver MPU9250 IIC, SPI.

/test enthält den Testcode des LibDriver MPU9250-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver MPU9250-Beispielcode.

/doc enthält das LibDriver MPU9250-Offlinedokument.

/Datenblatt enthält MPU9250-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC, SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC, SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/mpu9250/index.html](https://www.libdriver.com/docs/mpu9250/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.