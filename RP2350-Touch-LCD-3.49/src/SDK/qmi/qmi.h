// original author: Waveshare Team, translated from https://files.waveshare.com/wiki/RP2350-Touch-LCD-3.49/RP2350-Touch-LCD-3.49-LVGL.zip
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define QMI_I2C_PORT (i2c1)
#define DOF_INT1 (8)

#define QMI_ONE_G (9.807f)
#define G_LSP_MODE_3 (0x03 << 5)
#define A_LSP_MODE_3 (0x03 << 1)

#define QMI8658_Ctrl9_Cmd_WoM_Setting 0x08

#define QMI8658_CTRL7_DISABLE_ALL (0x0)
#define QMI8658_CTRL7_ACC_ENABLE (0x1)
#define QMI8658_CTRL7_GYR_ENABLE (0x2)
#define QMI8658_CTRL7_MAG_ENABLE (0x4)
#define QMI8658_CTRL7_AE_ENABLE (0x8)
#define QMI8658_CTRL7_GYR_SNOOZE_ENABLE (0x10)
#define QMI8658_CTRL7_ENABLE_MASK (0xF)

#define QMI8658_CONFIG_ACC_ENABLE QMI8658_CTRL7_ACC_ENABLE
#define QMI8658_CONFIG_GYR_ENABLE QMI8658_CTRL7_GYR_ENABLE
#define QMI8658_CONFIG_MAG_ENABLE QMI8658_CTRL7_MAG_ENABLE
#define QMI8658_CONFIG_AE_ENABLE QMI8658_CTRL7_AE_ENABLE
#define QMI8658_CONFIG_ACCGYR_ENABLE (QMI8658_CONFIG_ACC_ENABLE | QMI8658_CONFIG_GYR_ENABLE)
#define QMI8658_CONFIG_ACCGYRMAG_ENABLE (QMI8658_CONFIG_ACC_ENABLE | QMI8658_CONFIG_GYR_ENABLE | QMI8658_CONFIG_MAG_ENABLE)
#define QMI8658_CONFIG_AEMAG_ENABLE (QMI8658_CONFIG_AE_ENABLE | QMI8658_CONFIG_MAG_ENABLE)

enum QMI8658Register
{
    /*! \brief FIS device identifier register. */
    QMI8658Register_WhoAmI = 0, // 0
    /*! \brief FIS hardware revision register. */
    QMI8658Register_Revision, // 1
    /*! \brief General and power management modes. */
    QMI8658Register_Ctrl1, // 2
    /*! \brief Accelerometer control. */
    QMI8658Register_Ctrl2, // 3
    /*! \brief Gyroscope control. */
    QMI8658Register_Ctrl3, // 4
    /*! \brief Magnetometer control. */
    QMI8658Register_Ctrl4, // 5
    /*! \brief Data processing settings. */
    QMI8658Register_Ctrl5, // 6
    /*! \brief AttitudeEngine control. */
    QMI8658Register_Ctrl6, // 7
    /*! \brief Sensor enabled status. */
    QMI8658Register_Ctrl7, // 8
    /*! \brief Reserved - do not write. */
    QMI8658Register_Ctrl8, // 9
    /*! \brief Host command register. */
    QMI8658Register_Ctrl9, // 10
    /*! \brief Calibration register 1 most significant byte. */
    QMI8658Register_Cal1_L = 11,
    /*! \brief Calibration register 1 least significant byte. */
    QMI8658Register_Cal1_H,
    /*! \brief Calibration register 2 most significant byte. */
    QMI8658Register_Cal2_L,
    /*! \brief Calibration register 2 least significant byte. */
    QMI8658Register_Cal2_H,
    /*! \brief Calibration register 3 most significant byte. */
    QMI8658Register_Cal3_L,
    /*! \brief Calibration register 3 least significant byte. */
    QMI8658Register_Cal3_H,
    /*! \brief Calibration register 4 most significant byte. */
    QMI8658Register_Cal4_L,
    /*! \brief Calibration register 4 least significant byte. */
    QMI8658Register_Cal4_H,
    /*! \brief FIFO control register. */
    QMI8658Register_FifoCtrl = 19,
    /*! \brief FIFO data register. */
    QMI8658Register_FifoData, // 20
    /*! \brief FIFO status register. */
    QMI8658Register_FifoStatus, // 21
    /*! \brief Output data overrun and availability. */
    QMI8658Register_StatusInt = 45,
    /*! \brief Output data overrun and availability. */
    QMI8658Register_Status0,
    /*! \brief Miscellaneous status register. */
    QMI8658Register_Status1,
    /*! \brief timestamp low. */
    QMI8658Register_Timestamp_L = 48,
    /*! \brief timestamp low. */
    QMI8658Register_Timestamp_M,
    /*! \brief timestamp low. */
    QMI8658Register_Timestamp_H,
    /*! \brief tempearture low. */
    QMI8658Register_Tempearture_L = 51,
    /*! \brief tempearture low. */
    QMI8658Register_Tempearture_H,
    /*! \brief Accelerometer X axis least significant byte. */
    QMI8658Register_Ax_L = 53,
    /*! \brief Accelerometer X axis most significant byte. */
    QMI8658Register_Ax_H,
    /*! \brief Accelerometer Y axis least significant byte. */
    QMI8658Register_Ay_L,
    /*! \brief Accelerometer Y axis most significant byte. */
    QMI8658Register_Ay_H,
    /*! \brief Accelerometer Z axis least significant byte. */
    QMI8658Register_Az_L,
    /*! \brief Accelerometer Z axis most significant byte. */
    QMI8658Register_Az_H,
    /*! \brief Gyroscope X axis least significant byte. */
    QMI8658Register_Gx_L = 59,
    /*! \brief Gyroscope X axis most significant byte. */
    QMI8658Register_Gx_H,
    /*! \brief Gyroscope Y axis least significant byte. */
    QMI8658Register_Gy_L,
    /*! \brief Gyroscope Y axis most significant byte. */
    QMI8658Register_Gy_H,
    /*! \brief Gyroscope Z axis least significant byte. */
    QMI8658Register_Gz_L,
    /*! \brief Gyroscope Z axis most significant byte. */
    QMI8658Register_Gz_H,
    /*! \brief Magnetometer X axis least significant byte. */
    QMI8658Register_Mx_L = 65,
    /*! \brief Magnetometer X axis most significant byte. */
    QMI8658Register_Mx_H,
    /*! \brief Magnetometer Y axis least significant byte. */
    QMI8658Register_My_L,
    /*! \brief Magnetometer Y axis most significant byte. */
    QMI8658Register_My_H,
    /*! \brief Magnetometer Z axis least significant byte. */
    QMI8658Register_Mz_L,
    /*! \brief Magnetometer Z axis most significant byte. */
    QMI8658Register_Mz_H,
    /*! \brief Quaternion increment W least significant byte. */
    QMI8658Register_Q1_L = 73,
    /*! \brief Quaternion increment W most significant byte. */
    QMI8658Register_Q1_H,
    /*! \brief Quaternion increment X least significant byte. */
    QMI8658Register_Q2_L,
    /*! \brief Quaternion increment X most significant byte. */
    QMI8658Register_Q2_H,
    /*! \brief Quaternion increment Y least significant byte. */
    QMI8658Register_Q3_L,
    /*! \brief Quaternion increment Y most significant byte. */
    QMI8658Register_Q3_H,
    /*! \brief Quaternion increment Z least significant byte. */
    QMI8658Register_Q4_L,
    /*! \brief Quaternion increment Z most significant byte. */
    QMI8658Register_Q4_H,
    /*! \brief Velocity increment X least significant byte. */
    QMI8658Register_Dvx_L = 81,
    /*! \brief Velocity increment X most significant byte. */
    QMI8658Register_Dvx_H,
    /*! \brief Velocity increment Y least significant byte. */
    QMI8658Register_Dvy_L,
    /*! \brief Velocity increment Y most significant byte. */
    QMI8658Register_Dvy_H,
    /*! \brief Velocity increment Z least significant byte. */
    QMI8658Register_Dvz_L,
    /*! \brief Velocity increment Z most significant byte. */
    QMI8658Register_Dvz_H,
    /*! \brief AttitudeEngine reg1. */
    QMI8658Register_AeReg1 = 87,
    /*! \brief AttitudeEngine overflow flags. */
    QMI8658Register_AeOverflow,

    QMI8658Register_I2CM_STATUS = 110
};

enum QMI8658_AccRange
{
    QMI8658AccRange_2g = 0x00 << 4, /*!< \brief +/- 2g range */
    QMI8658AccRange_4g = 0x01 << 4, /*!< \brief +/- 4g range */
    QMI8658AccRange_8g = 0x02 << 4, /*!< \brief +/- 8g range */
    QMI8658AccRange_16g = 0x03 << 4 /*!< \brief +/- 16g range */
};

enum QMI8658_AccOdr
{
    QMI8658AccOdr_8000Hz = 0x00,         /*!< \brief High resolution 8000Hz output rate. */
    QMI8658AccOdr_4000Hz = 0x01,         /*!< \brief High resolution 4000Hz output rate. */
    QMI8658AccOdr_2000Hz = 0x02,         /*!< \brief High resolution 2000Hz output rate. */
    QMI8658AccOdr_1000Hz = 0x03,         /*!< \brief High resolution 1000Hz output rate. */
    QMI8658AccOdr_500Hz = 0x04,          /*!< \brief High resolution 500Hz output rate. */
    QMI8658AccOdr_250Hz = 0x05,          /*!< \brief High resolution 250Hz output rate. */
    QMI8658AccOdr_125Hz = 0x06,          /*!< \brief High resolution 125Hz output rate. */
    QMI8658AccOdr_62_5Hz = 0x07,         /*!< \brief High resolution 62.5Hz output rate. */
    QMI8658AccOdr_31_25Hz = 0x08,        /*!< \brief High resolution 31.25Hz output rate. */
    QMI8658AccOdr_LowPower_128Hz = 0x0c, /*!< \brief Low power 128Hz output rate. */
    QMI8658AccOdr_LowPower_21Hz = 0x0d,  /*!< \brief Low power 21Hz output rate. */
    QMI8658AccOdr_LowPower_11Hz = 0x0e,  /*!< \brief Low power 11Hz output rate. */
    QMI8658AccOdr_LowPower_3Hz = 0x0f    /*!< \brief Low power 3Hz output rate. */
};

enum QMI8658_GyrRange
{
    QMI8658GyrRange_32dps = 0 << 4,   /*!< \brief +-32 degrees per second. */
    QMI8658GyrRange_64dps = 1 << 4,   /*!< \brief +-64 degrees per second. */
    QMI8658GyrRange_128dps = 2 << 4,  /*!< \brief +-128 degrees per second. */
    QMI8658GyrRange_256dps = 3 << 4,  /*!< \brief +-256 degrees per second. */
    QMI8658GyrRange_512dps = 4 << 4,  /*!< \brief +-512 degrees per second. */
    QMI8658GyrRange_1024dps = 5 << 4, /*!< \brief +-1024 degrees per second. */
    QMI8658GyrRange_2048dps = 6 << 4, /*!< \brief +-2048 degrees per second. */
    QMI8658GyrRange_4096dps = 7 << 4  /*!< \brief +-2560 degrees per second. */
};

/*!
 * \brief Gyroscope output rate configuration.
 */
enum QMI8658_GyrOdr
{
    QMI8658GyrOdr_8000Hz = 0x00, /*!< \brief High resolution 8000Hz output rate. */
    QMI8658GyrOdr_4000Hz = 0x01, /*!< \brief High resolution 4000Hz output rate. */
    QMI8658GyrOdr_2000Hz = 0x02, /*!< \brief High resolution 2000Hz output rate. */
    QMI8658GyrOdr_1000Hz = 0x03, /*!< \brief High resolution 1000Hz output rate. */
    QMI8658GyrOdr_500Hz = 0x04,  /*!< \brief High resolution 500Hz output rate. */
    QMI8658GyrOdr_250Hz = 0x05,  /*!< \brief High resolution 250Hz output rate. */
    QMI8658GyrOdr_125Hz = 0x06,  /*!< \brief High resolution 125Hz output rate. */
    QMI8658GyrOdr_62_5Hz = 0x07, /*!< \brief High resolution 62.5Hz output rate. */
    QMI8658GyrOdr_31_25Hz = 0x08 /*!< \brief High resolution 31.25Hz output rate. */
};

enum QMI8658_AeOdr
{
    QMI8658AeOdr_1Hz = 0x00,   /*!< \brief 1Hz output rate. */
    QMI8658AeOdr_2Hz = 0x01,   /*!< \brief 2Hz output rate. */
    QMI8658AeOdr_4Hz = 0x02,   /*!< \brief 4Hz output rate. */
    QMI8658AeOdr_8Hz = 0x03,   /*!< \brief 8Hz output rate. */
    QMI8658AeOdr_16Hz = 0x04,  /*!< \brief 16Hz output rate. */
    QMI8658AeOdr_32Hz = 0x05,  /*!< \brief 32Hz output rate. */
    QMI8658AeOdr_64Hz = 0x06,  /*!< \brief 64Hz output rate. */
    QMI8658AeOdr_128Hz = 0x07, /*!< \brief 128Hz output rate. */
    /*!
     * \brief Motion on demand mode.
     *
     * In motion on demand mode the application can trigger AttitudeEngine
     * output samples as necessary. This allows the AttitudeEngine to be
     * synchronized with external data sources.
     *
     * When in Motion on Demand mode the application should request new data
     * by calling the QMI8658_requestAttitudeEngineData() function. The
     * AttitudeEngine will respond with a data ready event (INT2) when the
     * data is available to be read.
     */
    QMI8658AeOdr_motionOnDemand = 128
};

enum QMI8658_MagOdr
{
    QMI8658MagOdr_1000Hz = 0x00, /*!< \brief 1000Hz output rate. */
    QMI8658MagOdr_500Hz = 0x01,  /*!< \brief 500Hz output rate. */
    QMI8658MagOdr_250Hz = 0x02,  /*!< \brief 250Hz output rate. */
    QMI8658MagOdr_125Hz = 0x03,  /*!< \brief 125Hz output rate. */
    QMI8658MagOdr_62_5Hz = 0x04, /*!< \brief 62.5Hz output rate. */
    QMI8658MagOdr_31_25Hz = 0x05 /*!< \brief 31.25Hz output rate. */
};

enum QMI8658_MagDev
{
    MagDev_AKM09918 = (0 << 3), /*!< \brief AKM09918. */
};

struct QMI8658Config
{
    /*! \brief Sensor fusion input selection. */
    unsigned char inputSelection;
    /*! \brief Accelerometer dynamic range configuration. */
    enum QMI8658_AccRange accRange;
    /*! \brief Accelerometer output rate. */
    enum QMI8658_AccOdr accOdr;
    /*! \brief Gyroscope dynamic range configuration. */
    enum QMI8658_GyrRange gyrRange;
    /*! \brief Gyroscope output rate. */
    enum QMI8658_GyrOdr gyrOdr;
    /*! \brief AttitudeEngine output rate. */
    enum QMI8658_AeOdr aeOdr;
    /*!
     * \brief Magnetometer output data rate.
     *
     * \remark This parameter is not used when using an external magnetometer.
     * In this case the external magnetometer is sampled at the FIS output
     * data rate, or at an integer divisor thereof such that the maximum
     * sample rate is not exceeded.
     */
    enum QMI8658_MagOdr magOdr;

    /*!
     * \brief Magnetometer device to use.
     *
     * \remark This parameter is not used when using an external magnetometer.
     */
    enum QMI8658_MagDev magDev;
};

enum QMI8658_Interrupt
{
    /*! \brief FIS INT1 line. */
    QMI8658_Int1 = (0 << 6),
    /*! \brief FIS INT2 line. */
    QMI8658_Int2 = (1 << 6)
};

enum QMI8658_InterruptState
{
    QMI8658State_high = (1 << 7), /*!< Interrupt high. */
    QMI8658State_low = (0 << 7)   /*!< Interrupt low. */
};

enum QMI8658_WakeOnMotionThreshold
{
    QMI8658WomThreshold_high = 128, /*!< High threshold - large motion needed to wake. */
    QMI8658WomThreshold_low = 32    /*!< Low threshold - small motion needed to wake. */
};

#ifdef __cplusplus
extern "C"
{
#endif

    // Low level read/write functions
    bool qmi_init(void);
    void qmi_read_register(unsigned char reg, unsigned char *buf, unsigned short len);
    bool qmi_write_register(unsigned char reg, unsigned char value);

    // Configuration functions
    void qmi_apply_config(struct QMI8658Config const *config);
    void qmi_set_wake_on_motion(bool enable);

    // High level data read functions
    void qmi_read_acc_xyz(float acc_xyz[3]);
    void qmi_read_ae(float quat[4], float velocity[3]);
    void qmi_read_gyro_xyz(float gyro_xyz[3]);
    float qmi_read_temperature(void);
    void qmi_read_xyz(float acc[3], float gyro[3], unsigned int *tim_count);
    void qmi_read_xyz_raw(short raw_acc_xyz[3], short raw_gyro_xyz[3], unsigned int *tim_count);

#ifdef __cplusplus
}
#endif