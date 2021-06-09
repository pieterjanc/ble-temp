#pragma once

/**
 * Update BLE Environmental sensing service temperature
 * @param temp Temperature in 1/100 Celcius
 */
void ble_env_update_temperature(int16_t temp);

/**
 * Update BLE Environmental sensing service humidity
 * @param hum Humidity in 1/100 percent
 */
void ble_env_update_humidity(uint16_t hum);
