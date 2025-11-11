#include "qmi.h"

#define QMI8658_SLAVE_ADDR_L 0x6a
#define QMI8658_SLAVE_ADDR_H 0x6b

#define QMI8658_UINT_MG_DPS

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define AXIS_TOTAL 3

static unsigned short acc_lsb_div = 0;
static unsigned short gyro_lsb_div = 0;
static unsigned short ae_q_lsb_div = (1 << 14);
static unsigned short ae_v_lsb_div = (1 << 10);
static unsigned int imu_timestamp = 0;
static struct QMI8658Config QMI8658_config;
static unsigned char QMI8658_slave_addr = QMI8658_SLAVE_ADDR_L;

// config accelerometer
static void qmi_config_acc(enum QMI8658_AccRange range, enum QMI8658_AccOdr odr, bool lpfEnable, bool stEnable)
{
    unsigned char ctl_dada;

    switch (range)
    {
    case QMI8658AccRange_2g:
        acc_lsb_div = (1 << 14);
        break;
    case QMI8658AccRange_4g:
        acc_lsb_div = (1 << 13);
        break;
    case QMI8658AccRange_8g:
        acc_lsb_div = (1 << 12);
        break;
    case QMI8658AccRange_16g:
        acc_lsb_div = (1 << 11);
        break;
    default:
        range = QMI8658AccRange_8g;
        acc_lsb_div = (1 << 12);
    }
    if (stEnable)
        ctl_dada = (unsigned char)range | (unsigned char)odr | 0x80;
    else
        ctl_dada = (unsigned char)range | (unsigned char)odr;

    qmi_write_register(QMI8658Register_Ctrl2, ctl_dada);
    // set LPF & HPF
    qmi_read_register(QMI8658Register_Ctrl5, &ctl_dada, 1);
    ctl_dada &= 0xf0;
    if (lpfEnable)
    {
        ctl_dada |= A_LSP_MODE_3;
        ctl_dada |= 0x01;
    }
    else
    {
        ctl_dada &= ~0x01;
    }
    ctl_dada = 0x00;
    qmi_write_register(QMI8658Register_Ctrl5, ctl_dada);
}

// config gyroscope
static void qmi_config_gyro(enum QMI8658_GyrRange range, enum QMI8658_GyrOdr odr, bool lpfEnable, bool stEnable)
{
    // Set the CTRL3 register to configure dynamic range and ODR
    unsigned char ctl_dada;

    // Store the scale factor for use when processing raw data
    switch (range)
    {
    case QMI8658GyrRange_32dps:
        gyro_lsb_div = 1024;
        break;
    case QMI8658GyrRange_64dps:
        gyro_lsb_div = 512;
        break;
    case QMI8658GyrRange_128dps:
        gyro_lsb_div = 256;
        break;
    case QMI8658GyrRange_256dps:
        gyro_lsb_div = 128;
        break;
    case QMI8658GyrRange_512dps:
        gyro_lsb_div = 64;
        break;
    case QMI8658GyrRange_1024dps:
        gyro_lsb_div = 32;
        break;
    case QMI8658GyrRange_2048dps:
        gyro_lsb_div = 16;
        break;
    case QMI8658GyrRange_4096dps:
        gyro_lsb_div = 8;
        break;
    default:
        range = QMI8658GyrRange_512dps;
        gyro_lsb_div = 64;
        break;
    }

    if (stEnable)
        ctl_dada = (unsigned char)range | (unsigned char)odr | 0x80;
    else
        ctl_dada = (unsigned char)range | (unsigned char)odr;
    qmi_write_register(QMI8658Register_Ctrl3, ctl_dada);

    // Conversion from degrees/s to rad/s if necessary
    // set LPF & HPF
    qmi_read_register(QMI8658Register_Ctrl5, &ctl_dada, 1);
    ctl_dada &= 0x0f;
    if (lpfEnable)
    {
        ctl_dada |= G_LSP_MODE_3;
        ctl_dada |= 0x10;
    }
    else
    {
        ctl_dada &= ~0x10;
    }
    ctl_dada = 0x00;
    qmi_write_register(QMI8658Register_Ctrl5, ctl_dada);
    // set LPF & HPF
}

// config magnetometer
static void qmi_config_mag(enum QMI8658_MagDev device, enum QMI8658_MagOdr odr)
{
    qmi_write_register(QMI8658Register_Ctrl4, device | odr);
}

// config AttitudeEngine
static void qmi_config_ae(enum QMI8658_AeOdr odr)
{
    qmi_config_acc(QMI8658_config.accRange, QMI8658_config.accOdr, true, false);
    qmi_config_gyro(QMI8658_config.gyrRange, QMI8658_config.gyrOdr, true, false);
    qmi_config_mag(QMI8658_config.magDev, QMI8658_config.magOdr);
    qmi_write_register(QMI8658Register_Ctrl6, odr);
}

// enable/disable sensors
static void qmi_enable_sensors(unsigned char enableFlags)
{
    if (enableFlags & QMI8658_CONFIG_AE_ENABLE)
    {
        enableFlags |= QMI8658_CTRL7_ACC_ENABLE | QMI8658_CTRL7_GYR_ENABLE;
    }

    qmi_write_register(QMI8658Register_Ctrl7, enableFlags & QMI8658_CTRL7_ENABLE_MASK);
}

// apply configuration
void qmi_apply_config(struct QMI8658Config const *config)
{
    unsigned char fisSensors = config->inputSelection;

    if (fisSensors & QMI8658_CONFIG_AE_ENABLE)
    {
        qmi_config_ae(config->aeOdr);
    }
    else
    {
        if (config->inputSelection & QMI8658_CONFIG_ACC_ENABLE)
        {
            qmi_config_acc(config->accRange, config->accOdr, true, false);
        }
        if (config->inputSelection & QMI8658_CONFIG_GYR_ENABLE)
        {
            qmi_config_gyro(config->gyrRange, config->gyrOdr, true, false);
        }
    }

    if (config->inputSelection & QMI8658_CONFIG_MAG_ENABLE)
    {
        qmi_config_mag(config->magDev, config->magOdr);
    }
    qmi_enable_sensors(fisSensors);
}

// initialize QMI8658
bool qmi_init(void)
{
    unsigned char QMI8658_chip_id = 0x00;
    unsigned char QMI8658_revision_id = 0x00;
    unsigned char QMI8658_slave[2] = {QMI8658_SLAVE_ADDR_L, QMI8658_SLAVE_ADDR_H};
    unsigned char iCount = 0;
    int retry = 0;

    while (iCount < 2)
    {
        QMI8658_slave_addr = QMI8658_slave[iCount];
        retry = 0;

        while ((QMI8658_chip_id != 0x05) && (retry++ < 5))
        {
            qmi_read_register(QMI8658Register_WhoAmI, &QMI8658_chip_id, 1);
        }
        if (QMI8658_chip_id == 0x05)
        {
            break;
        }
        iCount++;
    }
    qmi_read_register(QMI8658Register_Revision, &QMI8658_revision_id, 1);
    if (QMI8658_chip_id == 0x05)
    {
        qmi_write_register(QMI8658Register_Ctrl1, 0x60);
        QMI8658_config.inputSelection = QMI8658_CONFIG_ACCGYR_ENABLE; // QMI8658_CONFIG_ACCGYR_ENABLE;
        QMI8658_config.accRange = QMI8658AccRange_8g;
        QMI8658_config.accOdr = QMI8658AccOdr_1000Hz;
        QMI8658_config.gyrRange = QMI8658GyrRange_512dps; // QMI8658GyrRange_2048dps   QMI8658GyrRange_1024dps
        QMI8658_config.gyrOdr = QMI8658GyrOdr_1000Hz;
        QMI8658_config.magOdr = QMI8658MagOdr_125Hz;
        QMI8658_config.magDev = MagDev_AKM09918;
        QMI8658_config.aeOdr = QMI8658AeOdr_128Hz;

        qmi_apply_config(&QMI8658_config);
        if (1)
        {
            unsigned char read_data = 0x00;
            qmi_read_register(QMI8658Register_Ctrl1, &read_data, 1);
            qmi_read_register(QMI8658Register_Ctrl2, &read_data, 1);
            qmi_read_register(QMI8658Register_Ctrl3, &read_data, 1);
            qmi_read_register(QMI8658Register_Ctrl4, &read_data, 1);
            qmi_read_register(QMI8658Register_Ctrl5, &read_data, 1);
            qmi_read_register(QMI8658Register_Ctrl6, &read_data, 1);
            qmi_read_register(QMI8658Register_Ctrl7, &read_data, 1);
        }
        return true;
    }
    else
    {
        printf("QMI8658_init fail\n");
        QMI8658_chip_id = 0;
        return false;
    }
}

// read accelerometer data into acc_xyz array
void qmi_read_acc_xyz(float acc_xyz[3])
{
    unsigned char buf_reg[6];
    short raw_acc_xyz[3];

    qmi_read_register(QMI8658Register_Ax_L, buf_reg, 6); // 0x19, 25
    raw_acc_xyz[0] = (short)((unsigned short)(buf_reg[1] << 8) | (buf_reg[0]));
    raw_acc_xyz[1] = (short)((unsigned short)(buf_reg[3] << 8) | (buf_reg[2]));
    raw_acc_xyz[2] = (short)((unsigned short)(buf_reg[5] << 8) | (buf_reg[4]));

    acc_xyz[0] = (raw_acc_xyz[0] * QMI_ONE_G) / acc_lsb_div;
    acc_xyz[1] = (raw_acc_xyz[1] * QMI_ONE_G) / acc_lsb_div;
    acc_xyz[2] = (raw_acc_xyz[2] * QMI_ONE_G) / acc_lsb_div;
}

// read AttitudeEngine data into quat and velocity arrays
void qmi_read_ae(float quat[4], float velocity[3])
{
    unsigned char buf_reg[14];
    short raw_q_xyz[4];
    short raw_v_xyz[3];

    qmi_read_register(QMI8658Register_Q1_L, buf_reg, 14);
    raw_q_xyz[0] = (short)((unsigned short)(buf_reg[1] << 8) | (buf_reg[0]));
    raw_q_xyz[1] = (short)((unsigned short)(buf_reg[3] << 8) | (buf_reg[2]));
    raw_q_xyz[2] = (short)((unsigned short)(buf_reg[5] << 8) | (buf_reg[4]));
    raw_q_xyz[3] = (short)((unsigned short)(buf_reg[7] << 8) | (buf_reg[6]));

    raw_v_xyz[1] = (short)((unsigned short)(buf_reg[9] << 8) | (buf_reg[8]));
    raw_v_xyz[2] = (short)((unsigned short)(buf_reg[11] << 8) | (buf_reg[10]));
    raw_v_xyz[2] = (short)((unsigned short)(buf_reg[13] << 8) | (buf_reg[12]));

    quat[0] = (float)(raw_q_xyz[0] * 1.0f) / ae_q_lsb_div;
    quat[1] = (float)(raw_q_xyz[1] * 1.0f) / ae_q_lsb_div;
    quat[2] = (float)(raw_q_xyz[2] * 1.0f) / ae_q_lsb_div;
    quat[3] = (float)(raw_q_xyz[3] * 1.0f) / ae_q_lsb_div;

    velocity[0] = (float)(raw_v_xyz[0] * 1.0f) / ae_v_lsb_div;
    velocity[1] = (float)(raw_v_xyz[1] * 1.0f) / ae_v_lsb_div;
    velocity[2] = (float)(raw_v_xyz[2] * 1.0f) / ae_v_lsb_div;
}

// read gyroscope data into gyro_xyz array
void qmi_read_gyro_xyz(float gyro_xyz[3])
{
    unsigned char buf_reg[6];
    short raw_gyro_xyz[3];

    qmi_read_register(QMI8658Register_Gx_L, buf_reg, 6); // 0x1f, 31
    raw_gyro_xyz[0] = (short)((unsigned short)(buf_reg[1] << 8) | (buf_reg[0]));
    raw_gyro_xyz[1] = (short)((unsigned short)(buf_reg[3] << 8) | (buf_reg[2]));
    raw_gyro_xyz[2] = (short)((unsigned short)(buf_reg[5] << 8) | (buf_reg[4]));

    gyro_xyz[0] = (raw_gyro_xyz[0] * 1.0f) / gyro_lsb_div;
    gyro_xyz[1] = (raw_gyro_xyz[1] * 1.0f) / gyro_lsb_div;
    gyro_xyz[2] = (raw_gyro_xyz[2] * 1.0f) / gyro_lsb_div;
}

// read register
void qmi_read_register(unsigned char reg, unsigned char *buf, unsigned short len)
{
    i2c_write_blocking(QMI_I2C_PORT, QMI8658_slave_addr, &reg, 1, true);
    i2c_read_blocking(QMI_I2C_PORT, QMI8658_slave_addr, buf, len, false);
}

// read temperature
float qmi_read_temperature(void)
{
    unsigned char buf[2];
    short temp = 0;
    float temp_f = 0;

    qmi_read_register(QMI8658Register_Tempearture_L, buf, 2);
    temp = ((short)buf[1] << 8) | buf[0];
    temp_f = (float)temp / 256.0f;

    return temp_f;
}

// read accelerometer and gyroscope data
void qmi_read_xyz(float acc[3], float gyro[3], unsigned int *tim_count)
{
    unsigned char buf_reg[12];
    short raw_acc_xyz[3];
    short raw_gyro_xyz[3];
    if (tim_count)
    {
        unsigned char buf[3];
        unsigned int timestamp;
        qmi_read_register(QMI8658Register_Timestamp_L, buf, 3); // 0x18	24
        timestamp = (unsigned int)(((unsigned int)buf[2] << 16) | ((unsigned int)buf[1] << 8) | buf[0]);
        if (timestamp > imu_timestamp)
            imu_timestamp = timestamp;
        else
            imu_timestamp = (timestamp + 0x1000000 - imu_timestamp);

        *tim_count = imu_timestamp;
    }

    qmi_read_register(QMI8658Register_Ax_L, buf_reg, 12); // 0x19, 25
    raw_acc_xyz[0] = (short)((unsigned short)(buf_reg[1] << 8) | (buf_reg[0]));
    raw_acc_xyz[1] = (short)((unsigned short)(buf_reg[3] << 8) | (buf_reg[2]));
    raw_acc_xyz[2] = (short)((unsigned short)(buf_reg[5] << 8) | (buf_reg[4]));

    raw_gyro_xyz[0] = (short)((unsigned short)(buf_reg[7] << 8) | (buf_reg[6]));
    raw_gyro_xyz[1] = (short)((unsigned short)(buf_reg[9] << 8) | (buf_reg[8]));
    raw_gyro_xyz[2] = (short)((unsigned short)(buf_reg[11] << 8) | (buf_reg[10]));

#if defined(QMI8658_UINT_MG_DPS)
    // mg
    acc[AXIS_X] = (float)(raw_acc_xyz[AXIS_X] * 1000.0f) / acc_lsb_div;
    acc[AXIS_Y] = (float)(raw_acc_xyz[AXIS_Y] * 1000.0f) / acc_lsb_div;
    acc[AXIS_Z] = (float)(raw_acc_xyz[AXIS_Z] * 1000.0f) / acc_lsb_div;
#else
    // m/s2
    acc[AXIS_X] = (float)(raw_acc_xyz[AXIS_X] * QMI_ONE_G) / acc_lsb_div;
    acc[AXIS_Y] = (float)(raw_acc_xyz[AXIS_Y] * QMI_ONE_G) / acc_lsb_div;
    acc[AXIS_Z] = (float)(raw_acc_xyz[AXIS_Z] * QMI_ONE_G) / acc_lsb_div;
#endif

#if defined(QMI8658_UINT_MG_DPS)
    // dps
    gyro[0] = (float)(raw_gyro_xyz[0] * 1.0f) / gyro_lsb_div;
    gyro[1] = (float)(raw_gyro_xyz[1] * 1.0f) / gyro_lsb_div;
    gyro[2] = (float)(raw_gyro_xyz[2] * 1.0f) / gyro_lsb_div;
#else
    // rad/s
    gyro[AXIS_X] = (float)(raw_gyro_xyz[AXIS_X] * 0.01745f) / gyro_lsb_div; // *pi/180
    gyro[AXIS_Y] = (float)(raw_gyro_xyz[AXIS_Y] * 0.01745f) / gyro_lsb_div;
    gyro[AXIS_Z] = (float)(raw_gyro_xyz[AXIS_Z] * 0.01745f) / gyro_lsb_div;
#endif
}

// read raw accelerometer and gyroscope data
void qmi_read_xyz_raw(short raw_acc_xyz[3], short raw_gyro_xyz[3], unsigned int *tim_count)
{
    unsigned char buf_reg[12];

    if (tim_count)
    {
        unsigned char buf[3];
        unsigned int timestamp;
        qmi_read_register(QMI8658Register_Timestamp_L, buf, 3); // 0x18	24
        timestamp = (unsigned int)(((unsigned int)buf[2] << 16) | ((unsigned int)buf[1] << 8) | buf[0]);
        if (timestamp > imu_timestamp)
            imu_timestamp = timestamp;
        else
            imu_timestamp = (timestamp + 0x1000000 - imu_timestamp);

        *tim_count = imu_timestamp;
    }
    qmi_read_register(QMI8658Register_Ax_L, buf_reg, 12); // 0x19, 25

    raw_acc_xyz[0] = (short)((unsigned short)(buf_reg[1] << 8) | (buf_reg[0]));
    raw_acc_xyz[1] = (short)((unsigned short)(buf_reg[3] << 8) | (buf_reg[2]));
    raw_acc_xyz[2] = (short)((unsigned short)(buf_reg[5] << 8) | (buf_reg[4]));

    raw_gyro_xyz[0] = (short)((unsigned short)(buf_reg[7] << 8) | (buf_reg[6]));
    raw_gyro_xyz[1] = (short)((unsigned short)(buf_reg[9] << 8) | (buf_reg[8]));
    raw_gyro_xyz[2] = (short)((unsigned short)(buf_reg[11] << 8) | (buf_reg[10]));
}

// enable/disable Wake-on-Motion
void qmi_set_wake_on_motion(bool enable)
{
    if (enable)
    {
        unsigned char womCmd[3];
        enum QMI8658_Interrupt interrupt = QMI8658_Int1;
        enum QMI8658_InterruptState initialState = QMI8658State_low;
        enum QMI8658_WakeOnMotionThreshold threshold = QMI8658WomThreshold_low;
        unsigned char blankingTime = 0x00;
        const unsigned char blankingTimeMask = 0x3F;

        qmi_enable_sensors(QMI8658_CTRL7_DISABLE_ALL);
        qmi_config_acc(QMI8658AccRange_2g, QMI8658AccOdr_LowPower_21Hz, false, false);

        womCmd[0] = QMI8658Register_Cal1_L; // WoM Threshold: absolute value in mg (with 1mg/LSB resolution)
        womCmd[1] = threshold;
        womCmd[2] = (unsigned char)interrupt | (unsigned char)initialState | (blankingTime & blankingTimeMask);
        qmi_write_register(QMI8658Register_Cal1_L, womCmd[1]);
        qmi_write_register(QMI8658Register_Cal1_H, womCmd[2]);

        // ctrl9 command
        qmi_write_register(QMI8658Register_Ctrl9, QMI8658_Ctrl9_Cmd_WoM_Setting);
        gpio_init(DOF_INT1);
        gpio_pull_up(DOF_INT1);
        gpio_set_dir(DOF_INT1, GPIO_IN);

        unsigned char val;
        while (1)
        {
            qmi_read_register(QMI8658Register_Status1, &val, sizeof(val));
            if (val & 0x04)
                break;
            sleep_ms(10);
        }

        // Enable sensors
        qmi_enable_sensors(QMI8658_CTRL7_ACC_ENABLE);
    }
    else
    {
        qmi_enable_sensors(QMI8658_CTRL7_DISABLE_ALL);
        qmi_write_register(QMI8658Register_Cal1_L, 0);
    }
}

// write register
bool qmi_write_register(unsigned char reg, unsigned char value)
{
    int ret = 0;
    unsigned int retry = 0;
    uint8_t data[2] = {reg, value};

    while ((ret <= 0) && (retry++ < 5))
    {
        ret = i2c_write_blocking(QMI_I2C_PORT, QMI8658_slave_addr, data, 2, false);
    }

    return (ret > 0) ? 0 : 1; // Return 0 for success, 1 for failure
}
