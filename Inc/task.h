#ifndef _TASK_H_
#define _TASK_H_

#include "cmsis_os.h"
#include "tos_k.h"

// LED debug
#define LED_TASK_STK_SIZE 256
extern k_task_t k_led_switch_rgb;
extern uint8_t k_led_switch_rgb_stk[LED_TASK_STK_SIZE];

void led_switch_rgb(void *pdata);

// LCD display and touch screen
#define DISPLAY_TOUCH_TASK_SIZE 2048
extern k_mutex_t display_touch_locker;
extern k_task_t k_display_touch;
extern uint8_t k_display_touch_stk[DISPLAY_TOUCH_TASK_SIZE];

void task_display_touch(void *pdata);

// Wifi connection test
#define WIFI_TEST_TASK_SIZE 4096
extern k_task_t k_wifi_test;
extern uint8_t k_wifi_test_stk[WIFI_TEST_TASK_SIZE];
extern char *wifi_ssid;
extern char *wifi_pwd;
extern char *server_ip;
extern char *server_port;
extern int socket_id_0;

void task_wifi_test(void *pdata);

// SDIO test
#define SDIO_TASK_SIZE 512
extern k_task_t k_task_sdio;
extern uint8_t k_sdio_stk[SDIO_TASK_SIZE];

void task_sdio(void *pdata);

// Application entry
#define APPLICATION_ENTRY_SIZE 1024
extern k_task_t k_application_entry;
extern uint8_t k_application_entry_stk[APPLICATION_ENTRY_SIZE];

void application_entry(void *arg);


#endif