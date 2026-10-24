//IMPORTANTE: O código está correto, mas está usando threads ao invés de baremetal. Substitui pelo meu exemplo, mude depois.

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define PERIOD_LED0_MS 1000
#define PERIOD_LED1_MS 500   

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif
#if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

int main(void)
{
    int ret;

    if (!gpio_is_ready_dt(&led0) || !gpio_is_ready_dt(&led1)) {
        printk("Error: LED device not ready\n");
        return 0;
    }

    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED0\n", ret);
        return 0;
    }

    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED1\n", ret);
        return 0;
    }

    printk("LED0 pin %d (%d ms), LED1 pin %d (%d ms)\n",
           led0.pin, PERIOD_LED0_MS, led1.pin, PERIOD_LED1_MS);

    int64_t next0 = k_uptime_get() + PERIOD_LED0_MS;
    int64_t next1 = k_uptime_get() + PERIOD_LED1_MS;

    while (1) {
        int64_t now = k_uptime_get();

        if (now >= next0) {
            gpio_pin_toggle_dt(&led0);
            next0 += PERIOD_LED0_MS;
        }
        if (now >= next1) {
            gpio_pin_toggle_dt(&led1);
            next1 += PERIOD_LED1_MS;
        }

        k_msleep(5);
    }

    return 0;
}