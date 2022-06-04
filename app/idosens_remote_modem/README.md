# Idosens Remote :: LoRaWAN Modem

## Build

```bash
export RIOTBASE=~/github/RIOT-OS/RIOT
cd $RIOTBASE/tests/pkg_semtech-loramac

# Generate UIDs and key
DEVEUI=$(openssl rand -hex 8)
APPEUI=0000000000000000
APPKEY=$(openssl rand -hex 16)
echo $DEVEUI $APPEUI $APPKEY

EXTERNAL_BOARD_DIRS=~/github/campusiot/idosens/boards make BOARD=idosens_remote LORA_REGION=EU868 LORA_DRIVER=sx1276 DEVEUI_OTA=$DEVEUI APPEUI=$APPEUI APPKEY=$APPKEY
```

## Usage

```bash
cat README.md
```
