# Idosens :: Applications

## Idosens sensor
* [LoRaWAN temperature endpoint](./idosens_sensor_lorawan)

## Idosens base

### TODO

* [ ] Idosens base : LoRaWAN class-C endpoint for displaying messages on e-paper screen panel
* [ ] Idosens base : LoRaWAN class-B endpoint for displaying messages on e-paper screen panel

## Idosens remote

### TODO

* [ ] Idosens remote : LoRaWAN class-A endpoint with panic buttons (ON/OFF)
* [ ] Idosens remote : LoRaWAN class-ABC UART modem

## Console

Open the serial console of the board (TagConnect UART pins to USB serial adapter)
```bash
brew install tio
tio -L
tio -b 115200 -m INLCRNL /dev/tty.usbserial-142xxx
```
