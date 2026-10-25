#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>


#define LED0_PERIOD_MS 1000
#define LED1_PERIOD_MS 350      /* led 1 precisa piscar a cada 350ms */
#define FLASH_MS       100     /* tempo em cada piscada deve ser 1s */


static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);


static void blink(void *p_led, void *p_period, void *unused)
{
   const struct gpio_dt_spec *led = p_led;
   int period = (int)(intptr_t)p_period;


   if (!gpio_is_ready_dt(led) ||
       gpio_pin_configure_dt(led, GPIO_OUTPUT_INACTIVE) < 0) {
       return;
   }


   int64_t next = k_uptime_get();


   while (1) {
       gpio_pin_set_dt(led, 1);
       k_msleep(FLASH_MS);
       gpio_pin_set_dt(led, 0);


       next += period;                       /* sem acumular atraso, porque na 1 mudança estava misturando as cores */
       k_sleep(K_TIMEOUT_ABS_MS(next));
   }
}


K_THREAD_DEFINE(led0_tid, 512, blink, &led0, (void *)LED0_PERIOD_MS, NULL, 7, 0, 0);
K_THREAD_DEFINE(led1_tid, 512, blink, &led1, (void *)LED1_PERIOD_MS, NULL, 7, 0, 0);


int main(void)
{
   return 0;   //threads
  
}

