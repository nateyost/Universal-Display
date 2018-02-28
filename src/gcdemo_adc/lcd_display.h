#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#ifdef __cpluslus
	extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
    
#define DIS_MENU_START 11
#define DIS_MIN_START 12
#define DIS_MAX_START 16
#define DIS_NUM_START 1
#define DIS_MENU_END 6
#define DIS_MIN_END 15
#define DIS_MAX_END 20
#define DIS_NUM_END 5
#define SEG14_SIZE 42
#define SEG7_SIZE 14
#define LCD_CANVAS_MAX 30
typedef union 
{
    uint16_t byte;
    struct
    {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
        uint8_t bit8:1;
        uint8_t bit9:1;
        uint8_t bit10:1;
        uint8_t bit11:1;
        uint8_t bit12:1;
        uint8_t bit13:1;
        uint8_t bit14:1;
        uint8_t stop:1;
    };
}uint16_c;
typedef struct 
{
    uint16_c canvas[LCD_CANVAS_MAX];
    uint16_t seg[26];
    uint16_t decimal;
    uint16_t decimal_temp;
    uint8_t menudisp;
    uint8_t menu;
    int16_t displayCursor;
    uint8_t enterButton;
    uint8_t upButton;
    uint8_t downButton;
    uint8_t menuButton;
    float max;
    float min;
    uint16_t time;
    uint16_t password[6];
    char menu_words[6];
    uint16_t units[6];
    uint16_t offset_array[8];
    uint16_t scale_array[8];
    uint16_t read_offset_array[8];
    uint16_t read_scale_array[8];
    uint8_t counter;
    uint8_t button_count;
    float offset;
    float scale;
    float read_offset;
    float read_scale;
    float graph_offset;
    float graph_scale;
    int8_t menu_state;
    int8_t menu_state_max;
    int8_t submenu_state;
    int8_t submenu_state_max;
    uint8_t goto_sleep;
    uint32_t Fosc;
}lcd;
lcd* lcd_new(void);
void lcd_14_seg(lcd *p, char character, int segindex);
void lcd_7_seg(lcd *p, char character, int segindex);
void display_min_max(lcd *p);
void display_menu_units(lcd *p);
void display_menu_character(lcd *p,int seg);
void display_7seg_character(lcd *p, int seg);
void update_read_offset_array(lcd *p);
void update_read_off_scale(lcd *p);
void lcd_update_offset_data(lcd *p);
void lcd_update_read_offset_data(lcd *p);
void lcd_update_scale_data(lcd *p);
void lcd_update_read_scale_data(lcd *p);
void update_off_scale(lcd *p);
void run_init(lcd *p);
uint16_t cmp_password(lcd *p);
void update_password(lcd *p);
void lcd_update_offset(lcd *p,uint8_t dir);
void lcd_update_scale(lcd *p);
void lcd_print_menu(lcd *p,char * str);
void lcd_print_number_display(lcd *p,char * str);
void lcd_print_min_max(lcd *p,char * min,char * max);
void lcd_meter(lcd *p,float percent);
void num_to_char(float number,char *c);
void lcd_update_sensor_data(lcd *p,float sensor_value);
void lcd_clear_screen(lcd *p);
void lcd_blink(lcd *p,int seg);
void lcd_7seg_scroll(lcd *p,bool dir,int seg);
void lcd_14seg_scroll(lcd *p,bool dir,int seg);
void lcd_14seg_scroll_scale_offset(lcd *p,bool dir,int seg,uint8_t type);
void lcd_14seg_scroll_read_scale_offset(lcd *p,bool dir,int seg,uint8_t type);
void lcd_7seg_decimal(lcd *p,bool up);
void lcd_14seg_decimal(lcd *p,bool up);
void update_menu(lcd *p);
void update_min_max(lcd *p);
void update_decimal(lcd *p);
void LCD_Update(lcd *p);
void all_on(lcd *p);
void lcd_menu_graph(lcd *p);
void lcd_run(lcd *p);
void lcd_set_canvas(lcd *p);
void segInit();

/** @} */
#include <xc.h>
#define	LCD_S5   _S47C0 
#define	LCD_S4   _S47C1  
#define	LCD_S3   _S47C2  
#define	LCD_S2   _S47C3

#define	LCD_1F   _S27C0 
#define	LCD_1G   _S27C1  
#define	LCD_1E   _S27C2  
#define	LCD_S1   _S27C3

#define	LCD_1A   _S26C0 
#define	LCD_1B   _S26C1  
#define	LCD_1C   _S26C2  
#define	LCD_1D   _S26C3

#define	LCD_2F   _S25C0 
#define	LCD_2G   _S25C1  
#define	LCD_2E   _S25C2  
#define	LCD_P2   _S25C3

#define	LCD_2A   _S24C0 
#define	LCD_2B   _S24C1  
#define	LCD_2C   _S24C2  
#define	LCD_2D   _S24C3

#define	LCD_3F   _S23C0 
#define	LCD_3G   _S23C1  
#define	LCD_3E   _S23C2  
#define	LCD_P3   _S23C3

#define	LCD_3A   _S45C0 
#define	LCD_3B   _S45C1  
#define	LCD_3C   _S45C2  
#define	LCD_3D   _S45C3

#define	LCD_4F   _S44C0 
#define	LCD_4G   _S44C1  
#define	LCD_4E   _S44C2  
#define	LCD_P4   _S44C3

#define	LCD_4A   _S22C0 
#define	LCD_4B   _S22C1  
#define	LCD_4C   _S22C2  
#define	LCD_4D   _S22C3

#define	LCD_5F   _S21C0 
#define	LCD_5G   _S21C1  
#define	LCD_5E   _S21C2  
#define	LCD_P5   _S21C3
 
#define	LCD_5A   _S20C0 
#define	LCD_5B   _S20C1  
#define	LCD_5C   _S20C2  
#define	LCD_5D   _S20C3

#define	LCD_B    _S17C0 
#define	LCD_B0   _S17C1  
#define	LCD_B2   _S17C2  
#define	LCD_B1   _S17C3

#define	LCD_8N   _S16C0 
#define	LCD_8M   _S16C1  
#define	LCD_8I   _S16C2  
#define	LCD_8A   _S16C3

#define	LCD_8E   _S15C0 
#define	LCD_8G   _S15C1  
#define	LCD_8H   _S15C2  
#define	LCD_8F   _S15C3

#define	LCD_9N   _S14C0 
#define	LCD_9M   _S14C1  
#define	LCD_9I   _S14C2  
#define	LCD_9A   _S14C3

#define	LCD_9E   _S13C0 
#define	LCD_9G   _S13C1  
#define	LCD_9H   _S13C2  
#define	LCD_9F   _S13C3

#define	LCD_10N  _S43C0 
#define	LCD_10M  _S43C1  
#define	LCD_10I  _S43C2  
#define	LCD_10A  _S43C3

#define	LCD_10E  _S42C0 
#define	LCD_10G  _S42C1  
#define	LCD_10H  _S42C2  
#define	LCD_10F  _S42C3

#define	LCD_11N  _S28C0 
#define	LCD_11M  _S28C1  
#define	LCD_11I  _S28C2  
#define	LCD_11A  _S28C3

#define	LCD_11E  _S29C0 
#define	LCD_11G  _S29C1  
#define	LCD_11H  _S29C2  
#define	LCD_11F  _S29C3

#define	LCD_12F  _S56C0 
#define	LCD_12G  _S56C1  
#define	LCD_12E  _S56C2  
#define	LCD_X1   _S56C3

#define	LCD_12A  _S55C0 
#define	LCD_12B  _S55C1  
#define	LCD_12C  _S55C2  
#define	LCD_12D  _S55C3

#define	LCD_13F  _S41C0 
#define	LCD_13G  _S41C1  
#define	LCD_13E  _S41C2  
#define	LCD_P13  _S41C3

#define	LCD_13A  _S40C0 
#define	LCD_13B  _S40C1  
#define	LCD_13C  _S40C2  
#define	LCD_13D  _S40C3

#define	LCD_14F  _S12C0 
#define	LCD_14G  _S12C1  
#define	LCD_14E  _S12C2  
#define	LCD_P14  _S12C3

#define	LCD_14A  _S11C0 
#define	LCD_14B  _S11C1  
#define	LCD_14C  _S11C2  
#define	LCD_14D  _S11C3

#define	LCD_15F  _S10C0 
#define	LCD_15G  _S10C1  
#define	LCD_15E  _S10C2  
#define	LCD_P15  _S10C3

#define	LCD_15A  _S39C0 
#define	LCD_15B  _S39C1  
#define	LCD_15C  _S39C2  
#define	LCD_15D  _S39C3

#define	LCD_16F  _S38C0 
#define	LCD_16G  _S38C1  
#define	LCD_16E  _S38C2  
#define	LCD_X2   _S38C3

#define	LCD_16A  _S9C0 
#define	LCD_16B  _S9C1  
#define	LCD_16C  _S9C2  
#define	LCD_16D  _S9C3

#define	LCD_17F  _S8C0 
#define	LCD_17G  _S8C1  
#define	LCD_17E  _S8C2  
#define	LCD_P17  _S8C3

#define	LCD_17A  _S19C0 
#define	LCD_17B  _S19C1  
#define	LCD_17C  _S19C2  
#define	LCD_17D  _S19C3

#define	LCD_18F  _S18C0 
#define	LCD_18G  _S18C1  
#define	LCD_18E  _S18C2  
#define	LCD_P18  _S18C3

#define	LCD_18A  _S54C0 
#define	LCD_18B  _S54C1  
#define	LCD_18C  _S54C2  
#define	LCD_18D  _S54C3

#define	LCD_19F  _S53C0 
#define	LCD_19G  _S53C1  
#define	LCD_19E  _S53C2  
#define	LCD_P19  _S53C3

#define	LCD_19A  _S31C0 
#define	LCD_19B  _S31C1  
#define	LCD_19C  _S31C2  
#define	LCD_19D  _S31C3

#define	LCD_20F  _S37C0 
#define	LCD_20G  _S37C1  
#define	LCD_20E  _S37C2  
#define	LCD_P20  _S37C3

#define	LCD_20A  _S36C0 
#define	LCD_20B  _S36C1  
#define	LCD_20C  _S36C2  
#define	LCD_20D  _S36C3

#define	LCD_11D  _S30C0 
#define	LCD_11L  _S30C1  
#define	LCD_11K  _S30C2  
#define	LCD_11J  _S30C3

#define	LCD_P11  _S7C0 
#define	LCD_11C  _S7C1  
#define	LCD_11B  _S7C2 

#define	LCD_10D  _S6C0 
#define	LCD_10L  _S6C1  
#define	LCD_10K  _S6C2  
#define	LCD_10J  _S6C3

#define	LCD_P10  _S5C0 
#define	LCD_10C  _S5C1  
#define	LCD_10B  _S5C2

#define	LCD_9D   _S4C0 
#define	LCD_9L   _S4C1  
#define	LCD_9K   _S4C2  
#define	LCD_9J   _S4C3

#define	LCD_P9   _S3C0 
#define	LCD_9C   _S3C1  
#define	LCD_9B   _S3C2

#define	LCD_8D   _S2C0 
#define	LCD_8L   _S2C1  
#define	LCD_8K   _S2C2  
#define	LCD_8J   _S2C3

#define	LCD_P8   _S35C0 
#define	LCD_8C   _S35C1  
#define	LCD_8B   _S35C2

#define	LCD_7E   _S34C0 
#define	LCD_7G   _S34C1  
#define	LCD_7H   _S34C2  
#define	LCD_7F   _S34C3

#define	LCD_7N   _S48C0 
#define	LCD_7M   _S48C1  
#define	LCD_7I   _S48C2
#define	LCD_7A   _S48C3

#define	LCD_7D   _S1C0 
#define	LCD_7L   _S1C1  
#define	LCD_7K   _S1C2
#define	LCD_7J   _S1C3

#define	LCD_P7   _S0C0 
#define	LCD_7C   _S0C1  
#define	LCD_7B   _S0C2

#define	LCD_6E   _S52C0 
#define	LCD_6G   _S52C1  
#define	LCD_6H   _S52C2  
#define	LCD_6F   _S52C3

#define	LCD_6N   _S33C0 
#define	LCD_6M   _S33C1  
#define	LCD_6I   _S33C2
#define	LCD_6A   _S33C3

#define	LCD_6D   _S51C0 
#define	LCD_6L   _S51C1  
#define	LCD_6K   _S51C2
#define	LCD_6J   _S51C3

#define	LCD_S35  _S32C0 
#define	LCD_6C   _S32C1  
#define	LCD_6B   _S32C2

#define	LCD_S31  _S50C0 
#define	LCD_S32  _S50C1  
#define	LCD_S33  _S50C2
#define	LCD_S34  _S50C3

#define	LCD_S30  _S62C0 
#define	LCD_S29  _S62C1  
#define	LCD_S28  _S62C2
#define	LCD_S27  _S62C3

#define	LCD_S23  _S61C0 
#define	LCD_S24  _S61C1  
#define	LCD_S25  _S61C2
#define	LCD_S26  _S61C3

#define	LCD_S22  _S60C0 
#define	LCD_S21  _S60C1  
#define	LCD_S20  _S60C2
#define	LCD_S19  _S60C3

#define	LCD_S15  _S59C0 
#define	LCD_S16  _S59C1  
#define	LCD_S17  _S59C2
#define	LCD_S18  _S59C3

#define	LCD_T1   _S58C0 
#define	LCD_T2   _S58C1  
#define	LCD_T4   _S58C2
#define	LCD_T3   _S58C3

#define	LCD_S14  _S57C0 
#define	LCD_A1   _S57C1  
#define	LCD_A2   _S57C2
#define	LCD_A3   _S57C3

#define	LCD_S13  _S49C0 
#define	LCD_S12  _S49C1  
#define	LCD_S11  _S49C2
#define	LCD_S10  _S49C3

#define	LCD_S6   _S46C0 
#define	LCD_S7   _S46C1  
#define	LCD_S8   _S46C2
#define	LCD_S9   _S46C3

#define S1      _SE00  
#define S2      _SE01 
#define S3      _SE02
#define S4 	    _SE03
#define S5 	    _SE04
#define S6 		_SE05
#define S7 		_SE06
#define S8 		_SE07
#define S9		_SE08
#define S10		_SE09
#define S11		_SE10
#define S12		_SE11
#define S13		_SE12
#define S14		_SE13
#define S15		_SE14
#define S16		_SE15
#define S17		_SE16
#define S18		_SE17
#define S19		_SE18
#define S20		_SE19
#define S21		_SE20
#define S22		_SE21
#define S23		_SE22
#define S24		_SE23
#define S25		_SE24
#define S26		_SE25
#define S27		_SE26
#define S28		_SE27
#define S29		_SE28
#define S30		_SE29
#define S31		_SE30
#define S32		_SE31
#define S33		_SE32
#define S34		_SE33
#define S35		_SE34
#define S36		_SE35
#define S37		_SE36
#define S38		_SE37
#define S39		_SE38
#define S40		_SE39
#define S41		_SE40
#define S42		_SE41
#define S43		_SE42
#define S44		_SE43
#define S45		_SE44
#define S46		_SE45
#define S47		_SE46
#define S48		_SE47
#define S49		_SE48
#define S50		_SE49
#define S51		_SE50
#define S52		_SE51
#define S53		_SE52
#define S54		_SE53
#define S55		_SE54
#define S56		_SE55
#define S57		_SE56
#define S58		_SE57
#define S59		_SE58
#define S60		_SE59
#define S61		_SE60
#define S62		_SE61
#define S63     _SE62
#define S64     _SE63



#endif //LCD_DISPLAY_H

