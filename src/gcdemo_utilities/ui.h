//#ifndef UI_H
//#define UI_H
//
//#ifdef __cpluslus
//	extern "C" {
//#endif // __cplusplus
//
//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#include "array.h"
//
///**
// * @defgroup UI UI
// * @{
// */
//
//#define UI_START                  0 
//#define DISPLAY_DATA              1 
//#define TRANSITION_RIGHT          2 
//#define TRANSITION_RIGHT_CANCEL   3 
//#define TRANSITION_LEFT           4 
//#define SUB_MENU_TRANSITION       5 
//#define DISPLAY_DEMO_TITLE        6 
//#define DISPLAY_DEMO_DESCRIPTION  7 
//
//#define TITLE_DELAY 45
//
//#define UI_LEFT    0
//#define UI_CIRCLE  1
//#define UI_RIGHT   2
//#define UI_BTN_MAX 3
//
//
//struct ui_tag;
//typedef void (*ui_btn_cb)(struct ui_tag *);
//
//typedef struct ui_tag
//{
//    struct
//    {
//        unsigned submenu_using_lr:1;
//        unsigned half_sec_oneshot:1;
//        unsigned two_sec_oneshot:1;
//    };
//    uint8_t goto_sleep;
//    char demo_description[60];
//    char demo_title[60];
//    int8_t state_internal;
//    int8_t menu_state;
//    int8_t menu_state_max;
//   // int8_t display_submenu_state;
//    int8_t submenu_state;
//    int8_t submenu_state_max;
//    lcd *p_lcd;
//    uint8_t button_event[UI_BTN_MAX];
//    uint8_t button_state[UI_BTN_MAX];
//    uint8_t button_state_prev[UI_BTN_MAX];
//    uint8_t demo_description_delay;
//    uint8_t demo_title_delay;
//    uint8_t btn_left_event;
//    uint8_t btn_right_event;
//    uint32_t refresh_rate_while_scrolling;
//    uint32_t Fosc;
//    uint32_t refresh_rate_not_scrolling;
//    ui_btn_cb btn_cb;
//    ui_btn_cb default_btn_cb;
    int8_t menu_state;
    int8_t menu_state_max;
    int8_t submenu_state;
    int8_t submenu_state_max;
    uint8_t goto_sleep;
//    void (*menu)(void);
//}ui;
//
//void uiCallback_setScrollSpeedFast();
//void uiCallback_setScrollSpeedSlow();
///**
// * \brief Allocates and initlizes a new UI Object
// * \param ui_menu_callback The user-selected menuing function
// * \returns reference to newly created UI object
// *
// *
// */
//ui* ui_new(void (*ui_menu_callback)(void));
//uint8_t ui_btn_pressed(ui *p, uint8_t btn);
//uint8_t ui_tasks(ui *p);
//
//#ifdef __cplusplus
//	}
//#endif // __cplusplus
//
///** @} */
//#endif //UI_H
//
