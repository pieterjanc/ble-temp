#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
#include <logging/log.h>
#include <zephyr.h>

#include "ble-env.h"
#include "sht21.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static const struct bt_data adv[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
        BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_ESS_VAL)),
};

void main(void)
{
    int err;

    err = bt_enable(NULL);
    if (err)
    {
        LOG_ERR("Bluetooth init failed (err %d)\n", err);
        return;
    }

    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, adv, ARRAY_SIZE(adv), NULL, 0);
    if (err)
    {
        LOG_ERR("Advertising failed to start (err %d)\n", err);
        return;
    }

    sht21_reset();

    while (1)
    {
        int16_t temp;
        err = sht21_temperature(&temp);
        if (err)
        {
            k_sleep(K_SECONDS(10));
            continue;
        }

        ble_env_update(temp, 0);

        k_sleep(K_SECONDS(10));
    }
}
