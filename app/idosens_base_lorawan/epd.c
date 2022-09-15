/*
 * Copyright (C) 2021-2022 LIG
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     app
 * @{
 *
 * @file
 * @brief       Drawing function for e-Paper Display
 *
 * @author      Germain Lemasson <germain.lemasson@univ-grenoble-alpes.fr>
 *
 * @}
 */

///////Include  Epaper
#include "epd_board.h"

#include "COG_FPL.h"
#include "BufferDraw.h"
#include "Arial24x23.h"
#include "Neu42x35.h"
#include "default_icon.h"
#include "door_closed_icon.h"
#include "liglab_icon.h"

#include "epd.h"

#define IMAGE_W (264)
#define IMAGE_H (176)
#define IMAGE_LEN (IMAGE_W*IMAGE_H/8)

//Two image buffer for EPD
static uint8_t image[IMAGE_LEN];
static uint8_t old_image[IMAGE_LEN];

//simple memcopy function
static void memcopy(const uint8_t * buffsource, uint8_t * buffdest, uint16_t len)
{
    for(uint16_t i =0; i<len; i++)
    {
        buffdest[i]=buffsource[i];
    }
}

/**
 * Init EPD
 */
void epd_init(void)
{
    //Init buffer image
    BD_init( image, IMAGE_LEN, IMAGE_W, IMAGE_H, 0, 1);
    BD_clear();
    BD_set_font(Courier_New19x36, Courier_New19x36_conf);
    //Init EPD
    spi_init(EPD_PARAM_SPI);
    EPD_display_hardware_init();
}

/**
 * Draw default picture on EPD
 */
void epd_draw_default(void)
{
    puts("Default");
    EPD_power_on(EPD_270, 18);
    puts("drive on");
    EPD_initialize_driver ();
    puts("drive init");
    memcopy(default_icon,image,IMAGE_LEN);
    EPD_display_from_array_prt ( old_image, image );
    puts("disp");
    memcopy(default_icon,old_image,IMAGE_LEN);
    EPD_power_off();
    spi_release(EPD_PARAM_SPI);
    puts("drive off");

}
/**
 * Draw closed picture with reopen time
 */
void epd_draw_closed(struct tm* ot)
{
    char buffer[32];

    //build image to draw
    BD_clear();
    BD_clip_image(door_closed,0,0,264,100);
    BD_locate(40,100);
    sprintf(buffer,"%02i-%02i-%04i",
             ot->tm_mday, ot->tm_mon + 1, ot->tm_year + 1900
           );
    puts(buffer);
    BD_puts(buffer);
    BD_locate(90,136);
    sprintf(buffer,"%02i:%02i",
            ot->tm_hour, ot->tm_min
          );
    BD_puts(buffer);
    
    //Draw image on EPD
    EPD_power_on(EPD_270, 18);
    EPD_initialize_driver ();
    EPD_display_from_array_prt ( old_image, image );
    memcopy(image,old_image,IMAGE_LEN);
    EPD_power_off();
    //EDP power use spi_aquire but you must use spi_release manualy
    spi_release(EPD_PARAM_SPI);
}
