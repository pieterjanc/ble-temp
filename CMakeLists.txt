cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ble-temp)

target_sources(app PRIVATE
	src/ble-env.c
	src/main.c
	src/sht21.c
	)
