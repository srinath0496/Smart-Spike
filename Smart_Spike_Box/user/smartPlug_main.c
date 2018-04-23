#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void user_procTask(os_event_t *events);

static volatile os_timer_t some_timer;
uint8 mode,status;
struct station_config config;
char ssid[32] = "HomeAlone";
char password[64] = "Srinath487496";
void some_timerfunc(void *arg)
{
    //Do blinky stuff
    if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT2)
    {
        //Set GPIO2 to LOW
        gpio_output_set(0, BIT2, BIT2, 0);
    }
    else
    {
        //Set GPIO2 to HIGH
        gpio_output_set(BIT2, 0, BIT2, 0);
    }
}

//Do nothing function
static void ICACHE_FLASH_ATTR
user_procTask(os_event_t *events)
{
    while(1)
    {
    os_delay_us(10);
    status = wifi_station_get_connect_status();
    os_printf("The wireless status is %d\n",status);
    }
}

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
    // Initialize the GPIO subsystem.
    gpio_init();
  //  uart_init(115200,115200);
    os_printf("Welcome to the Home Automation\n");
    wifi_set_opmode(0x01);
    mode = wifi_get_opmode();
    if(mode = 0x01)
	{
        gpio_output_set(BIT2, 0, BIT2, 0);
	config.bssid_set = 0;
	os_memcpy(&config.ssid,ssid,32);
	os_memcpy(&config.password,password,64);
	wifi_station_set_config(&config);
	wifi_station_disconnect();
	wifi_station_connect();
	}
    os_printf("The opmode is %d\n",mode);

    //Set GPIO2 to output mode
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

    //Set GPIO2 low
    gpio_output_set(0, BIT2, BIT2, 0);

    //Disarm timer
    os_timer_disarm(&some_timer);

    //Setup timer
 //   os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);

    //Arm the timer
    //&some_timer is the pointer
    //1000 is the fire time in ms
    //0 for once and 1 for repeating
  //  os_timer_arm(&some_timer, 1000, 1);
    
    //Start os task
    system_os_task(user_procTask, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
}
