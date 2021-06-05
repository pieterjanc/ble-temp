#include <sys/byteorder.h>

#include <bluetooth/gatt.h>

#include "ble-env.h"

struct env_data_t
{
    int16_t temp_value;
    int16_t hum_value;
};

static struct env_data_t env_data = {
        .temp_value = 0,
        .hum_value = 0,
};

static ssize_t read_u16(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
    const uint16_t *u16 = attr->user_data;
    uint16_t value = sys_cpu_to_le16(*u16);

    return bt_gatt_attr_read(conn, attr, buf, len, offset, &value, sizeof(value));
}

BT_GATT_SERVICE_DEFINE(ess_service,
                       BT_GATT_PRIMARY_SERVICE(BT_UUID_ESS),
                       BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE,
                                              BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                                              BT_GATT_PERM_READ,
                                              read_u16, NULL, &env_data.temp_value),
                       BT_GATT_CCC(NULL,
                                   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
                       BT_GATT_CHARACTERISTIC(BT_UUID_HUMIDITY,
                                              BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                                              BT_GATT_PERM_READ,
                                              read_u16, NULL, &env_data.hum_value),
                       BT_GATT_CCC(NULL,
                                   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE), );

void ble_env_update(int16_t temp, int16_t hum)
{
    if (temp != env_data.temp_value)
    {
        env_data.temp_value = temp;
        bt_gatt_notify(NULL, &ess_service.attrs[2], &temp, sizeof(temp));
    }

    if (hum != env_data.hum_value)
    {
        env_data.hum_value = hum;
        bt_gatt_notify(NULL, &ess_service.attrs[5], &hum, sizeof(hum));
    }
}
