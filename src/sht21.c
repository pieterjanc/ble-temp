#include <drivers/i2c.h>
#include <logging/log.h>
#include <sys/crc.h>
#include <zephyr.h>

#define SHT21_ADDR 0x40
#define SHT21_SOFT_RESET 0xfe
#define SHT21_TEMP_NO_HOLD 0xf3
#define SHT21_HUM_NO_HOLD 0xf5

LOG_MODULE_REGISTER(SHT21, LOG_LEVEL_DBG);

static const struct device *i2c_dev;

static inline bool check_crc(uint8_t *buf, size_t len)
{
    return (crc8(buf, len - 1, 0x31, 0, false) == buf[len - 1]);
}

void sht21_reset(void)
{
    uint8_t buf = SHT21_SOFT_RESET;
    i2c_write(i2c_dev, &buf, 1, SHT21_ADDR);
    k_sleep(K_MSEC(15));
}

int sht21_temperature(int16_t *temp)
{
    uint8_t cmd = SHT21_TEMP_NO_HOLD;
    uint8_t buf[3];
    int32_t raw = 0;

    memset(buf, 0, sizeof(buf));

    if (i2c_write(i2c_dev, &cmd, sizeof(cmd), SHT21_ADDR))
    {
        LOG_ERR("Failed to start temperature measurement");
        return -EIO;
    }

    k_sleep(K_MSEC(85));

    if (i2c_read(i2c_dev, buf, sizeof(buf), SHT21_ADDR))
    {
        LOG_ERR("Failed to read temperature");
        return -EIO;
    }

    if (!check_crc(buf, sizeof(buf)))
    {
        LOG_ERR("CRC checksum for temperature failed");
        return -EIO;
    }

    raw = ((buf[0] << 8) | buf[1]) & 0xfffc;
    raw *= 17572;
    raw = raw >> 16;
    raw -= 4685;
    *temp = (int16_t) raw;

    return 0;
}

int sht21_humidity(uint16_t *hum)
{
    uint8_t cmd = SHT21_HUM_NO_HOLD;
    uint8_t buf[3];
    int32_t raw = 0;

    memset(buf, 0, sizeof(buf));

    if (i2c_write(i2c_dev, &cmd, sizeof(cmd), SHT21_ADDR))
    {
        LOG_ERR("Failed to start humidity measurement");
        return -EIO;
    }

    k_sleep(K_MSEC(29));

    if (i2c_read(i2c_dev, buf, sizeof(buf), SHT21_ADDR))
    {
        LOG_ERR("Failed to read humidity");
        return -EIO;
    }

    if (!check_crc(buf, sizeof(buf)))
    {
        LOG_ERR("CRC checksum for humidity failed");
        return -EIO;
    }

    raw = ((buf[0] << 8) | buf[1]) & 0xfffc;
    raw *= 12500;
    raw = raw >> 16;
    raw -= 600;
    *hum = (uint16_t) raw;

    return 0;
}

static int sht21_init(const struct device *arg)
{
    ARG_UNUSED(arg);
    i2c_dev = device_get_binding(DT_LABEL(DT_NODELABEL(i2c0)));
    return 0;
}

SYS_INIT(sht21_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_OBJECTS);
