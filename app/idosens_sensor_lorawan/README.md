# Idosens Sensor :: LoRaWAN endpoint

This application sends periodically the temperature (LPP Cayenne encoded) from the AT30TS74-XM8M-T temperature sensor.

## Configuration

Set `DEVEUI`, `APPEUI`, `APPKEY` and `TXPERIOD` into the `Makefile` or in the `make` command line.

## Build

```bash
export RIOTBASE=~/github/RIOT-OS/RIOT
EXTERNAL_BOARD_DIRS=../../boards make DEVEUI=1234567890ABCDEF APPEUI=0000000000000000 APPKEY=1234567890ABCDEF1234567890ABCDEF
```

## TODO

* [x] Move to LPP Cayenne for the frame payload
* [ ] Control GPIO for load switch
* [ ] Read light from TSL25711
* [ ] Read magneto+accelero from LSM303DTR
* [ ] Decrement DR before retrying
* [ ] Set ADR false/true
* [ ] Set Confirmed/Unconfirmed
* [ ] Process downlink https://github.com/RIOT-OS/RIOT/blob/73ccd1e2e721bee38f958f8906ac32e5e1fceb0c/tests/pkg_semtech-loramac/main.c#L37
