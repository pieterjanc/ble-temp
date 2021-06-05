#pragma once

/**
 * Update BLE Environmental sensing service values
 * @param temp Temperature in 1/100 Celcius
 * @param hum Humidity percentage
 */
void ble_env_update(int16_t temp, int16_t hum);
