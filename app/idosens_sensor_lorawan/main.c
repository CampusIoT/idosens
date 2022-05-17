/*
 * (c) 2022, Université Grenoble Alpes, LIG, FabMSTIC
*/

/**
 * @ingroup     apps
 * @{
 *
 * @file
 * @brief       Idosens sensor application
 * @}
 */

#include "board.h"
#include "periph_conf.h"
#include "clk.h"
#include "timex.h"
#include "ztimer.h"
#include "at30tse75x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "msg.h"
#include "thread.h"
#include "fmt.h"
#include "cayenne_lpp.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

#include "sx127x.h"
#include "sx127x_netdev.h"
#include "sx127x_params.h"


#ifndef TXPERIOD
#define TXPERIOD                       (60U)
#endif

#ifndef JOIN_RETRY_PERIOD
#define JOIN_RETRY_PERIOD            (60U)
#endif


#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

static semtech_loramac_t loramac;

static sx127x_t sx127x;

static ztimer_t timer;

static at30tse75x_t temp_driver;

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

static cayenne_lpp_t lpp;

static void _alarm_cb(void *arg)
{
    (void) arg;
    msg_t msg;
    msg_send(&msg, sender_pid);
}

static void _prepare_next_alarm(void)
{
    timer.callback = _alarm_cb;
    ztimer_set(ZTIMER_MSEC, &timer, TXPERIOD * MS_PER_SEC);
}

static void _send_message(void)
{

    // TODO exit at30tse75x from deepsleep
    float temp = 0.0f;

    if (at30tse75x_get_temperature(&temp_driver, &temp) == 0){
        printf("Temperature: %f\n", temp);
        cayenne_lpp_reset(&lpp);
        cayenne_lpp_add_temperature(&lpp, 3, temp);
        /* Try to send the message */
        uint8_t ret = semtech_loramac_send(&loramac,
                                            (uint8_t *) &lpp.buffer, sizeof(lpp.cursor));
        if (ret != SEMTECH_LORAMAC_TX_DONE)  {
            puts("Fail to send message");
        } else {
            puts("Message sent");
        }
    } else {
        puts("Fail to read temperature");
    }
    
    // TODO add other sensors measurement to lpp

    // TODO set at30tse75x inot deepsleep mode

    return;
}

static void *sender(void *arg)
{
    (void)arg;

    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);

        /* Trigger the message send */
        _send_message();

        /* Schedule the next wake-up alarm */
        _prepare_next_alarm();
    }

    /* this should never be reached */
    return NULL;
}

int main(void)
{

    puts("Starting temperature driver");
    while (at30tse75x_init(&temp_driver, PORT_A, AT30TSE75X_TEMP_ADDR) != 0) {
        puts("error temperature driver sensor");
        ztimer_sleep(ZTIMER_USEC, 1 * US_PER_SEC);
    };
    puts("Starting init temperature driver succeeded");

    /* Convert identifiers and application key */
    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);

    /* Initialize the radio driver */
    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = &sx127x.netdev;
    loramac.netdev->driver = &sx127x_driver;

    /* Initialize the loramac stack */
    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    // TODO printf deveui appeui appkey

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5 );

    /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
     * generated device address and to get the network and application session
     * keys.
     */
    puts("Starting join procedure");
    while (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        ztimer_sleep(ZTIMER_USEC, JOIN_RETRY_PERIOD * US_PER_SEC);

        // TODO decrement DR before retrying
        // semtech_loramac_set_dr(&loramac, LORAMAC_DR_5 );
    }
    puts("Join procedure succeeded");

    // TODO Set ADR
    // TODO Set Confirmed/Unconfirmed

        /* start the sender thread */
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_PRIO, 0, sender, NULL, "sender");

    /* trigger the first send */
    msg_t msg;
    msg_send(&msg, sender_pid);
    return 0;
}
