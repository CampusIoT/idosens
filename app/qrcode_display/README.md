# IdoSens Base LoRaWAN :: QR Code Display

Display a QR Code on the e-paper screen.

The QR Code contains a URL which is the concatenation of 
* a base URL which can include an endpoint ID (least significant bytes of the DevEUI for instance),
* a pseudo random number (generated for a seed and a secret key),
* a 32-bit long timestamp (seconds since the GPS epoch ie 6/1/1980).

The pseudo random number and the timestamp are encoded in `base64`.

The QR Code can be changed periodically.

The base URL, the seed, the secret key, the period of change can be configured and updated using a downlink.

The RTC of the MCU is synchronized using the App Clock Sync specification.

If a GNSS module is connected to the board (UART,I2C) and if the URL is prefied by `geo:`, the QRCode includes the lattitude and the longitude. 
(for instance, [geo:45.1941477,5.7656245?q=FabMSTIC](geo:45.1941477,5.7656245?q=FabMSTIC))

## References
* https://github.com/RIOT-OS/RIOT/tree/master/tests/pkg_qr-code-generator
* https://github.com/nayuki/QR-Code-generator
* https://github.com/CampusIoT/orbimote/blob/master/field_test_device/app_clock.c

## Snippet

```c

#include <inttypes.h>
#include <string.h>

#include <stdbool.h>
#include <stdint.h>

#include "qrcodegen.h"

#include "hashes/aes128_cmac.h"
#include "crypto/ciphers.h"


// URL_PREFIX can be set using downlink
char* URL_PREFIX="https://presence.com"

// AES128_CMAC_KEY and AES128_CMAC_KEY_VERSION can be set using downlink
uint16_t AES128_CMAC_KEY_VERSION=1;
const uint8_t AES128_CMAC_KEY[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

const char* URL_PROTO3="%s/%8x/%8x/%4x";
const char* URL_PROTO4="%s/%8x/%8x/%4x/%8x";

// URL max size is 1024
char URL_TO_ENCODE[1024];
sprintf(URL_TO_ENCODE, URL_PROTO3, BASE_ID, epoch_in_second, AES128_CMAC_KEY_VERSION);
size_t len = strlen(URL_TO_ENCODE);

// get the secure digest
uint8_t digest[16];
aes128_cmac_context_t ctx;
aes128_cmac_init(&ctx, AES128_CMAC_KEY, 16);
aes128_cmac_update(&ctx, URL_TO_ENCODE, len);
aes128_cmac_final(&ctx, digest);

// add the 4 first bytes of the secure digest
sprintf(URL_TO_ENCODE, URL_PROTO4, BASE_ID, epoch_in_second, AES128_CMAC_KEY_VERSION, *((uint32_t*)digest));

// generate the qrcode for the buffer 
uint8_t qr0[qrcodegen_BUFFER_LEN_FOR_VERSION(2)];
uint8_t buffer[qrcodegen_BUFFER_LEN_FOR_VERSION(2)];

if (!qrcodegen_encodeText(URL_TO_ENCODE,
                              buffer, qr0, qrcodegen_Ecc_MEDIUM,
                              qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX,
                              qrcodegen_Mask_AUTO, true)) {
        puts("Encoding error");
        return -1;
}

int size = qrcodegen_getSize(qr0);

...
```

## Check URL CMAC
```bash
echo https://presence.com/12345678/abcdef01/0001 | openssl dgst -mac cmac -macopt cipher:aes-128-cbc -macopt hexkey:11223344556677889900112233445566 -sha1
```
