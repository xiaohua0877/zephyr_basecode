/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

//#include <zephyr.h>
//#include <misc/printk.h>
#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>

/* change this to use another GPIO port */
#ifndef SW0_GPIO_CONTROLLER
    #ifdef SW0_GPIO_NAME
        #define SW0_GPIO_CONTROLLER SW0_GPIO_NAME
    #else
        #error SW0_GPIO_NAME or SW0_GPIO_CONTROLLER needs to be set in board.h
    #endif
#endif
#define PORT    SW0_GPIO_CONTROLLER

/* change this to use another GPIO pin */
#ifdef SW0_GPIO_PIN
    #define PIN     SW0_GPIO_PIN
#else
    #error SW0_GPIO_PIN needs to be set in board.h
#endif

/* change to use another GPIO pin interrupt config */
#ifdef SW0_GPIO_FLAGS
    #define EDGE    (SW0_GPIO_FLAGS | GPIO_INT_EDGE)
#else
    /*
    * If SW0_GPIO_FLAGS not defined used default EDGE value.
    * Change this to use a different interrupt trigger
    */
    #define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)
#endif

/* change this to enable pull-up/pull-down */
#ifndef SW0_GPIO_FLAGS
    #ifdef SW0_GPIO_PIN_PUD
        #define SW0_GPIO_FLAGS SW0_GPIO_PIN_PUD
    #else
        #define SW0_GPIO_FLAGS 0
    #endif
#endif
#define PULL_UP SW0_GPIO_FLAGS

/* Sleep time */
#define SLEEP_TIME          (500)


void button_pressed(struct device *gpiob, struct gpio_callback *cb,
                    u32_t pins)
{
    printk("Button pressed at %d\n", k_cycle_get_32());
}

static struct gpio_callback gpio_cb;
extern void fs_main(void);
int main_run_count = 0;
void main(void)
{
    struct device *gpiob;
    K_DEBUG("Hello World! %s\n", CONFIG_BOARD);
    K_DEBUG("Press the user defined button on the board\n");
    gpiob = device_get_binding(PORT);
    if (!gpiob)
    {
        printk("error\n");
        return;
    }

    gpio_pin_configure(gpiob, PIN,
                       GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE);

    gpio_init_callback(&gpio_cb, button_pressed, BIT(PIN));

    gpio_add_callback(gpiob, &gpio_cb);
    gpio_pin_enable_callback(gpiob, PIN);

    ///fs_main();

    while (1)
    {
        u32_t val = 0U;

        gpio_pin_read(gpiob, PIN, &val);
			main_run_count++;
        k_sleep(SLEEP_TIME);
    }
}

//void main(void)
//{
//    printk("Hello World! %s\n", CONFIG_BOARD);
//}