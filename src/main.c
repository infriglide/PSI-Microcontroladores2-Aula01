#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

/* Períodos de inversão de cada LED.
 * Parte 1: LED0 = 1000 ms, LED1 = 500 ms
 * Desafio: LED0 = 1000 ms, LED1 = 350 ms */
#define LED0_PERIOD_MS 1000
#define LED1_PERIOD_MS 350     /* use 500 para a parte 1 */

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

/* Callbacks dos timers (executam em contexto de interrupção do kernel:
 * mantê-los curtos, sem printk nem k_msleep) */
static void led0_timer_handler(struct k_timer *timer)
{
    gpio_pin_toggle_dt(&led0);
}

static void led1_timer_handler(struct k_timer *timer)
{
    gpio_pin_toggle_dt(&led1);
}

K_TIMER_DEFINE(led0_timer, led0_timer_handler, NULL);
K_TIMER_DEFINE(led1_timer, led1_timer_handler, NULL);

static int led_init(const struct gpio_dt_spec *led, const char *name)
{
    int ret;

    if (!gpio_is_ready_dt(led)) {
        printk("Error: %s device %s is not ready\n", name, led->port->name);
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure %s pin\n", ret, name);
        return ret;
    }

    printk("%s ready on %s pin %d\n", name, led->port->name, led->pin);
    return 0;
}

int main(void)
{
    if (led_init(&led0, "LED0") < 0 || led_init(&led1, "LED1") < 0) {
        return 0;
    }

    /* Ambos iniciam acesos em t = 0; depois invertem periodicamente */
    k_timer_start(&led0_timer, K_MSEC(LED0_PERIOD_MS), K_MSEC(LED0_PERIOD_MS));
    k_timer_start(&led1_timer, K_MSEC(LED1_PERIOD_MS), K_MSEC(LED1_PERIOD_MS));

    printk("LED0 toggle: %d ms | LED1 toggle: %d ms\n",
           LED0_PERIOD_MS, LED1_PERIOD_MS);

    k_sleep(K_FOREVER);   /* o trabalho é feito pelos timers */
    return 0;
}
