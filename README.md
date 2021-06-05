# ble-temp
Small BLE temperature sensor implementation.

## Hardware
- nRF52 DK
- SHT21

## Building
Setup Zephyr: https://docs.zephyrproject.org/latest/getting_started/index.html 
```shell
cmake -Bbuild -GNinja -DBOARD=nrf52dk_nrf52832 .
ninja -C build
```

## Flashing
```shell
ninja -C build flash
```
