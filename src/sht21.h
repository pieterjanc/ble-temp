#pragma once

/**
 * Reset SHT21 chip
 */
void sht21_reset(void);

/**
 * Start temperature measurement and read result
 * @param[out] temp Temperature in 1/100 Celsius
 * @return 0 on success, negative otherwise
 */
int sht21_temperature(int16_t *temp);

/**
 * Start humidity measurement and read result
 * @param[out] hum Humidity in 1/100 percent
 * @return 0 on success, negative otherwise
 */
int sht21_humidity(uint16_t *hum);
