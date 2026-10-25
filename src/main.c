#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_PERIOD_MS 1000
#define LED1_PERIOD_MS 350   /* versão básica: 500 */
#define FLASH_MS       100

BUILD_ASSERT(FLASH_MS < LED0_PERIOD_MS && FLASH_MS < LED1_PERIOD_MS,
             "FLASH_MS deve ser menor que os periodos dos LEDs");

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "led0 nao definido para esta placa"
#endif
#if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
#error "led1 nao definido para esta placa"
#endif

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

static void led_thread(void *p1, void *p2, void *p3)
{
    const struct gpio_dt_spec *led = p1;
    int period_ms = (int)(intptr_t)p2;

    if (!gpio_is_ready_dt(led) ||
        gpio_pin_configure_dt(led, GPIO_OUTPUT_INACTIVE) < 0) {
        printk("Erro ao configurar LED (pino %d)\n", led->pin);
        return;
    }

    int64_t next = k_uptime_get();

    while (1) {
        gpio_pin_set_dt(led, 1);
        k_msleep(FLASH_MS);
        gpio_pin_set_dt(led, 0);

        next += period_ms;
        k_sleep(K_TIMEOUT_ABS_MS(next));
    }
}

K_THREAD_DEFINE(led0_tid, 512, led_thread, &led0,
                (void *)(intptr_t)LED0_PERIOD_MS, NULL, 5, 0, 0);
K_THREAD_DEFINE(led1_tid, 512, led_thread, &led1,
                (void *)(intptr_t)LED1_PERIOD_MS, NULL, 5, 0, 0);