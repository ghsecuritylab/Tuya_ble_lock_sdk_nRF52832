copy _build\nrf52832_xxaa.hex .\hex\tuya_ble_lock_common_nRF52832.hex
copy _build\nrf52832_xxaa.hex .\ota\tuya_ble_lock_common_nRF52832.hex
.\ota\hex2bin -k 1 .\ota\tuya_ble_lock_common_nRF52832.hex