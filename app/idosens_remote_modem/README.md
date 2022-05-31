# Idosens Remote :: LoRaWAN Modem

## Build

```bash
export RIOTBASE=~/github/RIOT-OS/RIOT
cd $RIOTBASE/tests/pkg_semtech-loramac
EXTERNAL_BOARD_DIRS=~/github/campusiot/idosens/boards make BOARD=idosens_remote LORA_REGION=EU868 LORA_DRIVER=sx1276
```

## Usage

```bash
cat README.md
```
