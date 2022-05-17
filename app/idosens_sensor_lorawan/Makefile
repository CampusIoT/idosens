#
# (c) 2022, Université Grenoble Alpes, LIG, FabMSTIC
#

# name of your application
APPLICATION = idosens-app

# If no BOARD is found in the environment, use this default:
BOARD ?= idosens_sensor 

DRIVER ?= sx1276

LORA_REGION ?= EU868

USEPKG += semtech-loramac
USEPKG += cayenne-lpp

USEMODULE += $(DRIVER)
USEMODULE += fmt
USEMODULE += printf_float

FEATURES_OPTIONAL += periph_rtc

# LoRaWAN id and keys
DEVEUI ?= 1234567890ABCDEF 
APPEUI ?= 0000000000000000
APPKEY ?= 1234567890ABCDEF1234567890ABCDEF 

# Time in seconds between two transmits : default is 5 minutes
TXPERIOD ?= (300U)

# Pass these enviroment variables to docker
DOCKER_ENV_VARS += DEVEUI
DOCKER_ENV_VARS += APPEUI
DOCKER_ENV_VARS += APPKEY


# we want to use SAUL:

# Module for ATMEL AT30TS74-XM8M-T (Sensor +-0.1° Accurate temperature sensor I2C) I2C addr 0x39
USEMODULE += at30tse75x
# TODO add module for AMS TAOS TSL25711 (Sensor Light to digital converter) I2C addr 0x48
# TODO add module for ST Microelectronics LSM303DTR (Sensor magneto+accelero)

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

ifndef CONFIG_KCONFIG_USEMODULE_LORAWAN
  # OTAA compile time configuration keys
  CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
  CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
  CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
endif
