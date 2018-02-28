/* 
 * File:   lcd_display.c
 * Author: Nathan Yost
 *
 * Created on December 1, 2017, 12:23 PM
 */
#include "lcd_display.h"



//characters for the 7 seg displays as hex values
//                                        BLANK  0   1   2     3    4    5    6   7    8    9    -    =                      
const uint8_t seg7display[SEG7_SIZE]=  {0x00,0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F,0x40,0x48};
//characters for the 7 seg displays as charater values
const char seg7characters[SEG7_SIZE]= {' ','0','1','2','3','4','5','6','7','8','9','-','='};

/************************************************************************
 * lcd_7_seg

This routine sets a specific 7seg determined by segindex to display the input charater
and keeps track of the specific index it was set to in p->seg

Parameters:		LCD pointer,character,segindex
Return:			None
Side Effects:	sets p->seg[segindex] to the specific charater in the seg7display array.
************************************************************************/
void lcd_7_seg(lcd *p, char character, int segindex)
{
    int index=0;
    //segindex corresponds to the number of the seven seg display in the datasheet
    p->canvas[segindex].byte=0x00000000000;

    if(character==0x30) {p->canvas[segindex].byte=seg7display[1];  index=1;}
    else if(character==0x31) {p->canvas[segindex].byte=seg7display[2];  index=2;}
    else if(character==0x32) {p->canvas[segindex].byte=seg7display[3];  index=3;}
    else if(character==0x33) {p->canvas[segindex].byte=seg7display[4];  index=4;}
    else if(character==0x34) {p->canvas[segindex].byte=seg7display[5];  index=5;}
    else if(character==0x35) {p->canvas[segindex].byte=seg7display[6];  index=6;}
    else if(character==0x36) {p->canvas[segindex].byte=seg7display[7];  index=7;}
    else if(character==0x37) {p->canvas[segindex].byte=seg7display[8];  index=8;}
    else if(character==0x38) {p->canvas[segindex].byte=seg7display[9];  index=9;}
    else if(character==0x39) {p->canvas[segindex].byte=seg7display[10]; index=10;}
    else if(character==0x2D) {p->canvas[segindex].byte=seg7display[11]; index=11;}
    else if(character==0x3D) {p->canvas[segindex].byte=seg7display[12]; index=12;}
    else{p->canvas[segindex].byte=seg7display[0]; index=0;}
    p->seg[segindex]=index;
}

//characters for the 14 seg displays as hex values
//                                        A     B    C     D      E     F     G     H     I     J    K     L     M                 
const int16_t seg14display[SEG14_SIZE]={0x477,0x150F,0x39,0x110F,0x479,0x471,0x43D,0x476,0x1109,0x1E,0xA70,0x38,0x2B6,
//         N    O    P     Q     R     S     T     U    V      W      X      Y      Z      0     1     2                      
        0x8B6,0x3F,0x473,0x83F,0xC73,0x46D,0x1101,0x3E,0x2230,0x2836,0x2A80,0x1462,0x2209,0x223F,0x6,0x45B,
//         3    4     5     6    7    8     9    -       +      *     /     =     BLANK                              
        0x44F,0x466,0x46D,0x47D,0x7,0x47F,0x46F,0x440,0x1540,0x3FC0,0x2200,0x448,0x0000};
const char seg14characters[SEG14_SIZE]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9','-','+','/','=',' '};
const int seg14numbers[10]={0,1,2,3,4,5,6,7,8,9};                                                                                                                                                                                                                                                  
/************************************************************************
 * lcd_14_seg

This routine sets a specific 14seg determined by segindex to display the input charater
and keeps track of the specific index it was set to in p->seg

Parameters:		LCD pointer,character,segindex
Return:			None
Side Effects:	sets p->seg[segindex] to the specific charater in the seg14display array.
************************************************************************/
void lcd_14_seg(lcd *p, char character, int segindex)
{
    int index=0;
    //segindex corresponds to the number of the fourteen seg display in the datasheet
    p->canvas[segindex].byte=0x0000000000;
    if(character==0x41)      {p->canvas[segindex].byte=seg14display[0]; index=0;} //A
    else if(character==0x42) {p->canvas[segindex].byte=seg14display[1]; index=1;} //B
    else if(character==0x43) {p->canvas[segindex].byte=seg14display[2]; index=2;} //C
    else if(character==0x44) {p->canvas[segindex].byte=seg14display[3]; index=3;} //D
    else if(character==0x45) {p->canvas[segindex].byte=seg14display[4]; index=4;} //E
    else if(character==0x46) {p->canvas[segindex].byte=seg14display[5]; index=5;} //F
    else if(character==0x47) {p->canvas[segindex].byte=seg14display[6]; index=6;} //G
    else if(character==0x48) {p->canvas[segindex].byte=seg14display[7]; index=7;} //H
    else if(character==0x49) {p->canvas[segindex].byte=seg14display[8]; index=8;} //I
    else if(character==0x4A) {p->canvas[segindex].byte=seg14display[9]; index=9;} //J
    else if(character==0x4B) {p->canvas[segindex].byte=seg14display[10]; index=10;} //K
    else if(character==0x4C) {p->canvas[segindex].byte=seg14display[11]; index=11;} //L
    else if(character==0x4D) {p->canvas[segindex].byte=seg14display[12]; index=12;} //M
    else if(character==0x4E) {p->canvas[segindex].byte=seg14display[13]; index=13;} //N
    else if(character==0x4F) {p->canvas[segindex].byte=seg14display[14]; index=14;} //O 
    else if(character==0x50) {p->canvas[segindex].byte=seg14display[15]; index=15;} //P
    else if(character==0x51) {p->canvas[segindex].byte=seg14display[16]; index=16;} //Q
    else if(character==0x52) {p->canvas[segindex].byte=seg14display[17]; index=17;} //R
    else if(character==0x53) {p->canvas[segindex].byte=seg14display[18]; index=18;} //S
    else if(character==0x54) {p->canvas[segindex].byte=seg14display[19]; index=19;} //T
    else if(character==0x55) {p->canvas[segindex].byte=seg14display[20]; index=20;} //U
    else if(character==0x56) {p->canvas[segindex].byte=seg14display[21]; index=21;} //V
    else if(character==0x57) {p->canvas[segindex].byte=seg14display[22]; index=22;} //W
    else if(character==0x58) {p->canvas[segindex].byte=seg14display[23]; index=23;} //X
    else if(character==0x59) {p->canvas[segindex].byte=seg14display[24]; index=24;} //Y
    else if(character==0x5A) {p->canvas[segindex].byte=seg14display[25]; index=25;} //Z
    else if(character==0x30) {p->canvas[segindex].byte=seg14display[26]; index=26;} //0
    else if(character==0x31) {p->canvas[segindex].byte=seg14display[27]; index=27;} //1
    else if(character==0x32) {p->canvas[segindex].byte=seg14display[28]; index=28;} //2
    else if(character==0x33) {p->canvas[segindex].byte=seg14display[29]; index=29;} //3
    else if(character==0x34) {p->canvas[segindex].byte=seg14display[30]; index=30;} //4
    else if(character==0x35) {p->canvas[segindex].byte=seg14display[31]; index=31;} //5
    else if(character==0x36) {p->canvas[segindex].byte=seg14display[32]; index=32;} //6
    else if(character==0x37) {p->canvas[segindex].byte=seg14display[33]; index=33;} //7
    else if(character==0x38) {p->canvas[segindex].byte=seg14display[34]; index=34;} //8
    else if(character==0x39) {p->canvas[segindex].byte=seg14display[35]; index=35;} //9
    else if(character==0x2D) {p->canvas[segindex].byte=seg14display[36]; index=36;} //-
    else if(character==0x2B) {p->canvas[segindex].byte=seg14display[37]; index=37;} //+
    else if(character==0x2A) {p->canvas[segindex].byte=seg14display[38]; index=38;} //*
    else if(character==0x2F) {p->canvas[segindex].byte=seg14display[39]; index=39;} // /
    else if(character==0x3D) {p->canvas[segindex].byte=seg14display[40]; index=40;} // =
    else {p->canvas[segindex].byte=seg14display[41]; index=41;} // BLANK
    p->seg[segindex]=index;
}
/************************************************************************
 * segInint

This routine initializes the 64 segments needed for this display

Parameters:		None
Return:			None
Side Effects:	None
************************************************************************/
void segInit()
{
    S1=1; S2=1; S3=1; S4=1; S5=1; S6=1; S7=1; S8=1; S9=1; S10=1;
    S11=1; S12=1; S13=1; S14=1; S15=1; S16=1; S17=1; S18=1; S19=1; S20=1;
    S21=1; S22=1; S23=1; S24=1; S25=1; S26=1; S27=1; S28=1; S29=1; S30=1;
    S31=1; S32=1; S33=1; S34=1; S35=1; S36=1; S37=1; S38=1; S39=1; S40=1;
    S41=1; S42=1; S43=1; S44=1; S45=1; S46=1; S47=1; S48=1; S49=1; S50=1;
    S51=1; S52=1; S53=1; S54=1; S55=1; S56=1; S57=1; S58=1; S59=1; S60=1;
    S61=1; S62=1; S63=1; S64=1;
}
/************************************************************************
 * lcd_print_menu

This routine sets the 6 14seg displays in the menu determined by the input 
character string

Parameters:		LCD pointer,character string
Return:			None
Side Effects:	None
************************************************************************/
void lcd_print_menu(lcd *p,char * str)
{
  
  int segment=0;//Segment A-N
  int seg_14=0;//which 14-seg display This function is for displays 6 to 11
  int strLength = strlen(str);
  if(strLength>11) strLength=11;
  
  for (segment = 0; segment < strLength; segment++) 
  {
      if(seg_14<DIS_MENU_END && *(str+segment)!=0x2E)//this if statement accounts for all items in str except periods.
      {
        lcd_14_seg(p,*(str+segment),DIS_MENU_START-seg_14);
        seg_14++;
      }
      if(*(str+segment-1)==0x2E)//when a period is present in the string
      {
        p->canvas[DIS_MENU_START-seg_14+1].bit14=1;
      }
  }
}
/************************************************************************
 * lcd_print_number_display

This routine sets the 5 7seg displays in the number display determined by the input 
character string

Parameters:		LCD pointer,character string
Return:			None
Side Effects:	None
************************************************************************/
void lcd_print_number_display(lcd *p,char * str)
{
  
  int segment=0;//Segment A-G
  int seg_7=DIS_NUM_START;//which 7-seg display. This function is for displays 1 to 5
  int strLength = strlen(str);
  if(strLength>9) strLength=9;
  
  for (segment = 0; segment < strLength; segment++) 
  {
      if(seg_7<=DIS_NUM_END && *(str+segment)!=0x2E )//this if statement accounts for all items in str except periods.
      {
        lcd_7_seg(p,*(str+segment),seg_7);
        seg_7++;
      }
      if((*(str+segment-1)==0x2E) && p->decimal!=1)//when a period is present in the string
      {
        p->canvas[seg_7-1].bit7=1;
      }
  }
} 
/************************************************************************
 * lcd_print_menu

This routine sets the 4 7seg displays in the min display and the 5 7seg displays in the max display determined by the input 
character strings min and max

Parameters:		LCD pointer,character string min, character string max
Return:			None
Side Effects:	None
************************************************************************/
void lcd_print_min_max(lcd *p,char * min,char * max)
{
  
  int segment=0;//Segment A-G
  int seg_7=DIS_MIN_START;//which 7-seg display This function is for displays 12 to 20
  int minLength = strlen(min);
  int maxLength = strlen(max);
  if(minLength>7) minLength=7;
  if(maxLength>9) maxLength=9;
  p->canvas[26].byte = 0x3;//TURN ON OUTLINES FOR MIN AND MAX
  for (segment = 0; segment < minLength; segment++) 
  {
      if(seg_7<=DIS_MIN_END && *(min+segment)!=0x2E)//this if statement accounts for all items in min except periods.
      {
        lcd_7_seg(p,*(min+segment),seg_7);
        seg_7++;
      }
      if(*(min+segment-1)==0x2E)//when a period is present in the string
      {
        p->canvas[seg_7-1].bit7=1;
      }
  }
  seg_7=DIS_MAX_START;
  for (segment = 0; segment < maxLength; segment++) 
  {
      if(seg_7<=DIS_MAX_END && *(max+segment)!=0x2E)//this if statement accounts for all items in max except periods.
      {
        lcd_7_seg(p,*(max+segment),seg_7);
        seg_7++;
      }
      if(*(max+segment-1)==0x2E)//when a period is present in the string
      {
        p->canvas[seg_7-1].bit7=1;
      }
  }
}
/************************************************************************
 * lcd_meter

This routine takes a percent value and maps it on the round meter display
each tick on the meter represents 3.125% the left hand arrow tick that 
points down represents less then 0 percent and the right hand arrow tick 
that points down represents more than 100%

Parameters:		LCD pointer,percentage
Return:			None
Side Effects:	None
************************************************************************/
void lcd_meter(lcd *p,float percent)
{
    p->canvas[21].byte=0x0;
    p->canvas[22].byte=0x0;
    p->canvas[23].byte=0x0;
    p->canvas[24].byte=0x0;
    p->canvas[25].byte=0x0;
    if(percent<0) p->canvas[21].byte=0x1;//when the percentage is negative which equals less then the chosen minimum.
    else if((0<percent)&&(percent<3.125)) p->canvas[21].byte=0x2;
    else if(percent<(3.125*2)) p->canvas[21].byte=0x6;
    else if(percent<(3.125*3)) p->canvas[21].byte=0xE;
    else if(percent<(3.125*4)) p->canvas[21].byte=0x1E;
    else if(percent<(3.125*5)) p->canvas[21].byte=0x3E;
    else if(percent<(3.125*6)) p->canvas[21].byte=0x7E;
    else if(percent<(3.125*7)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x1;
    }
    else if(percent<(3.125*8)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x3;
    }
    else if(percent<(3.125*9)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7;
    }
    else if(percent<(3.125*10)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0xF;
    }
    else if(percent<(3.125*11)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x1F;
    }
    else if(percent<(3.125*12)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x3F;
    }
    else if(percent<(3.125*13)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
    }
    else if(percent<(3.125*14)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x1;
    }
    else if(percent<(3.125*15)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x3;
    }
    else if(percent<(3.125*16)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7;
    }
    else if(percent<(3.125*17)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0xF;
    }
    else if(percent<(3.125*18)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x1F;
    }
    else if(percent<(3.125*19)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x3F;
    }
    else if(percent<(3.125*20)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
    }
    else if(percent<(3.125*21)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x1;
    }
    else if(percent<(3.125*22)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x3;
    }
    else if(percent<(3.125*23)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x7;
    }
    else if(percent<(3.125*24)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0xF;
    }
    else if(percent<(3.125*25)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x1F;
    }
    else if(percent<(3.125*26)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x3F;
    }
    else if(percent<(3.125*27)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x7F;
    }
    else if(percent<(3.125*28)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x7F;
        p->canvas[25].byte=0x1;
    }
    else if(percent<(3.125*29)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x7F;
        p->canvas[25].byte=0x3;
    }
    else if(percent<(3.125*30)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x7F;
        p->canvas[25].byte=0x7;
    }
    else if(percent<(3.125*31)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x7F;
        p->canvas[25].byte=0xF;
    }
    else if(percent<(3.125*32)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x7F;
        p->canvas[25].byte=0x1F;
    }
    else if(percent==(3.125*32)) 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x7F;
        p->canvas[25].byte=0x3F;
    }
    else if(percent>(3.125*32))//This is when the percentage is greater than 100 which equals greater than the chosen max. 
    {
        p->canvas[21].byte=0x7E;
        p->canvas[22].byte=0x7F;
        p->canvas[23].byte=0x7F;
        p->canvas[24].byte=0x7F;
        p->canvas[25].byte=0x7F;
    }
}
/************************************************************************
 * num_to_char

This routine converts a float to a string

Parameters:		LCD pointer,pointer string
Return:			None
Side Effects:	None
************************************************************************/
void num_to_char(float number,char *c)
{
    sprintf(c, "%f", (double)number);
}
void display_min_max(lcd *p)
{
    int i;
    for(i=DIS_MIN_START;i<(DIS_MAX_END+1);i++)
    {
        display_7seg_character(p,i);
    }
}
void display_7seg_character(lcd *p, int seg)
{
    p->canvas[seg].byte=seg7display[p->seg[seg]];
}
void display_menu_character(lcd *p,int seg)
{
    p->canvas[seg].byte=seg14display[p->seg[seg]];
}
void display_menu_units(lcd *p)
{
    int i;
    p->seg[11]=p->units[0];
    p->seg[10]=p->units[1];
    p->seg[9]=p->units[2];
    p->seg[8]=p->units[3];
    p->seg[7]=p->units[4];
    p->seg[6]=p->units[5];
    for(i=DIS_MENU_START;i>=DIS_MENU_END;i--)
    {
        display_menu_character(p,i);
    }
}
void update_password(lcd *p)
{
    p->password[0]=p->seg[11];
    p->password[1]=p->seg[10];
    p->password[2]=p->seg[9];
    p->password[3]=p->seg[8];
    p->password[4]=p->seg[7];
    p->password[5]=p->seg[6];
}
uint16_t cmp_password(lcd *p)
{
    int check=0;
    if(p->seg[11]==p->password[0])check++; 
    if(p->seg[10]==p->password[1])check++;
    if(p->seg[9]==p->password[2])check++;
    if(p->seg[8]==p->password[3])check++;
    if(p->seg[7]==p->password[4])check++;
    if(p->seg[6]==p->password[5])check++;
    if(check==6) {return 1;}
    else{return 0;}
}
void run_init(lcd *p)
{
    display_min_max(p);
    p->canvas[26].byte = 0x3;//TURN ON OUTLINES FOR MIN AND MAX
    p->canvas[p->decimal].bit7=1;//number display
    p->canvas[p->decimal+10].bit7=1;//min display
    if(p->decimal==2)
    {
       p->canvas[p->decimal+11].bit7=1;//min display 
    }
    p->canvas[p->decimal+15].bit7=1;//max display
    display_menu_units(p);
}


void lcd_update_scale_data(lcd *p)
{
    char str[20];
    num_to_char(p->scale,str);
    lcd_print_menu(p,str);
}
void lcd_update_read_scale_data(lcd *p)
{
    char str[20];
    num_to_char(p->read_scale,str);
    if(p->read_scale<10)
    {
        str[6]=str[5];
        str[5]=str[4];
        str[4]=str[3];
        str[3]=str[2];
        str[2]=str[1];
        str[1]=str[0];
        str[0]='0';
        
    }
    lcd_print_menu(p,str);
}
void lcd_update_read_offset_data(lcd *p)
{

    if(p->read_offset<0)p->seg[11]=36;
    else p->seg[11]=37;
    p->seg[10]=p->read_offset_array[0];
    p->seg[9]=p->read_offset_array[1];
    p->seg[8]=p->read_offset_array[2];
    p->seg[7]=p->read_offset_array[3];
    p->seg[6]=p->read_offset_array[5];
    display_menu_character(p,11);
    display_menu_character(p,10);
    display_menu_character(p,9);
    display_menu_character(p,8);
    display_menu_character(p,7);
    display_menu_character(p,6);
}
void lcd_update_offset_data(lcd *p)
{
    char str[20];
    float temp;
    temp=p->offset;
    if(p->offset<0)temp=p->offset*-1;
    num_to_char(temp,str);
    str[6]=str[5];
    str[5]=str[4];
    str[4]=str[3];
    str[3]=str[2];
    str[2]=str[1];
    str[1]=str[0];
    str[0]='0';
    if(temp<10)
    {
        str[6]=str[5];
        str[5]=str[4];
        str[4]=str[3];
        str[3]=str[2];
        str[2]=str[1];
        str[1]=str[0];
        str[0]='0';
        
    }
    if(p->offset<0) str[0]='-';
    else str[0]='+';
    lcd_print_menu(p,str);
}
void update_read_offset_array(lcd *p)
{
    char str1[20];
    int i;
    //p->read_offset_array[0]=p->seg[11];
    p->read_offset_array[0]=p->seg[10];
    p->read_offset_array[1]=p->seg[9];
    p->read_offset_array[2]=p->seg[8];
    p->read_offset_array[3]=p->seg[7];
    p->read_offset_array[4]=26;
    p->read_offset_array[5]=p->seg[6];
    
    for(i=0;i<6;i++)
    {
        str1[i]=seg14characters[p->read_offset_array[i]];
    }
    str1[4]='.'; 
    p->read_offset=atof(str1);
    if(p->seg[11]==36)
    {
        p->read_offset=p->read_offset*-1;
        p->read_offset_array[6]=1;
    }
    else
    {
        p->read_offset_array[6]=0;
    }
    
}
/************************************************************************
 * lcd_update_sensor_data
 * Make sure and set p->min and p->max before calling this function

This routine updates the number display with the sensor value and updates
the meter display with the percentage of the senor value depending on 
p->min and p->max values. 
 
Parameters:		LCD pointer,senpsor_value
Return:			None
Side Effects:	None
************************************************************************/
void lcd_update_sensor_data(lcd *p,float sensor_value)
{
    //Make sure to set the max and min before the use of this function.
    char str[20];
    int decimal=0;
    int i;
    int decimal_move=0;

    num_to_char(sensor_value,str);
    for(i=1;i<7;i++)
    {
        if(str[i]=='.') decimal=i;
    }
    if(p->decimal==1)
    {
       decimal_move=decimal-p->decimal+1-5; 
    }
    else
    {
       decimal_move=(decimal-p->decimal)+1;
    }
    for(i=1;i<=abs(decimal_move);i++)
    {
        if(decimal_move<0)//move decimal right
        {
            str[7]=str[6];
            str[6]=str[5];
            str[5]=str[4];
            str[4]=str[3];
            str[3]=str[2];
            str[2]=str[1];
            str[1]=str[0];
            str[0]=' ';
        }
        else//move number left
        {
            str[0]=str[1];
            str[1]=str[2];
            str[2]=str[3];
            str[3]=str[4];
            str[4]=str[5];
            str[5]=str[6];
            str[6]=str[7];
            str[7]=' ';
        }
    }
    lcd_print_number_display(p,str);
}
/************************************************************************
 * lcd_clear_screen

This routine sets the whole lcd display to display nothing

Parameters:		LCD pointer
Return:			None
Side Effects:	None
************************************************************************/
void lcd_clear_screen(lcd *p)
{
    int i;
    for(i=0;i<LCD_CANVAS_MAX;i++)
    {
        p->canvas[i].byte=0x0000;
    }
    
}
/************************************************************************
 * lcd_blink

This routine doesn't function yet. It will be made to blink any specific seg display.

Parameters:		LCD pointer,segment
Return:			None
Side Effects:	None
************************************************************************/
void lcd_blink(lcd *p,int seg)
{
    if(p->counter>10)
    {
        p->canvas[seg].bit3=1;
        p->counter=0;
    }
    else if(p->counter==5)
    {
        p->canvas[seg].bit3=0;  
    }
}
/************************************************************************
 * update_menu

This routine stores each of the menu characters into a string called p->menu_words

Parameters:		LCD pointer
Return:			None
Side Effects:	None
************************************************************************/
void update_menu (lcd *p)
{
    int i;
    int j=0;
    char menu[10];
    //menu
    for(i=DIS_MENU_START;i<=DIS_MENU_END;i--)
    {
    menu[j]=seg14characters[p->seg[i]];
    j++;
    }
    strcpy(p->menu_words,menu);     
}
void update_off_scale(lcd *p)
{
    char off[8];
    char scl[8];
    int i;
    for(i=0;i<8;i++)
    {
        off[i]=0;
        scl[i]=0;
    }
    for(i=0;i<7;i++)
    {
        scl[i]=seg14characters[p->scale_array[i]];
        if(i<6)off[i]=seg14characters[p->offset_array[i]];
    }
    scl[1]='.';
    off[2]='.';
    p->scale=atof(scl);
    p->offset=atof(off);
    if(p->offset_array[6]==1) p->offset=p->offset*-1;
    //if(p->scale_array[6]==1) p->scale=p->scale*-1;
}
void update_read_off_scale(lcd *p)
{
    char str1[8];
    char scl[8];
    int i;
    for(i=0;i<8;i++)
    {
        scl[i]=0;
        str1[i]=0;
    }
    for(i=0;i<7;i++)
    {
        scl[i]=seg14characters[p->read_scale_array[i]];
        if(i<6)str1[i]=seg14characters[p->read_offset_array[i]];
    }
    str1[4]='.';
    p->read_offset=atof(str1);
    if(p->read_offset_array[0]==1)p->read_offset=p->read_offset*-1;
    scl[2]='.';
    p->read_scale=atof(scl);

}
/************************************************************************
 * update_min_max

This routine stores whatever is displayed for min and max and updates the
the variables p-min and p->max.

Parameters:		LCD pointer
Return:			None
Side Effects:	None
************************************************************************/
void update_min_max(lcd *p)
{
    char min[7];
    char max[9];
    int i;
    int j=0;
    for(i=0;i<9;i++)
    {
        if(i<7) min[i]=0;
        max[i]=0;
    }
    
    //min
    for(i=DIS_MIN_START;i<=DIS_MIN_END;i++)
    {
        if(p->decimal==i-10 && p->decimal!=1)
        {
            min[j]='.';
            j++;
        }
        min[j]=seg7characters[p->seg[i]];
        j++;
    }
    p->min=atof(min);
    j=0;
    
    //max
    for(i=DIS_MAX_START;i<=DIS_MAX_END;i++)
    {
        if(p->decimal==i-15 && p->decimal!=1)
        {
            max[j]='.';
            j++;
        }
        max[j]=seg7characters[p->seg[i]];
        j++;
    }
    p->max=atof(max);
    p->graph_scale=(p->max-p->min)/16; //20-4 = 16mA
    p->graph_offset=(p->max-(20*p->graph_scale));
}
/************************************************************************
 * LCD_update

This routine updates the menu, min, and max displays. This should be called
after reading in the flash at the beginning of the program.

Parameters:		LCD pointer
Return:			None
Side Effects:	None
************************************************************************/
void LCD_Update(lcd *p)
{
    update_min_max(p);
    update_off_scale(p);
    update_read_off_scale(p);
}
/************************************************************************
 * lcd_7_seg_scroll

This routine scrolls the array seg7display of a specific 7segment defined 
by the input seg. The direction is set by the input dir. When dir==true it 
will scroll up and when dir==false it will scroll down.

Parameters:		LCD pointer,direction(true=up,false=down),segment (1-5,12-20)
Return:			None
Side Effects:	None
************************************************************************/
void lcd_7seg_scroll(lcd *p,bool dir,int seg)
{
    int index=p->seg[seg];
            if(dir==true)//up
            {
                if(index==SEG7_SIZE-4) index=-1;
                p->canvas[seg].byte=seg7display[index+1];
                p->seg[seg]=index+1;
            }
            else//down
            {
                if(index==0) index=SEG7_SIZE-3;
                p->canvas[seg].byte=seg7display[index-1];
                p->seg[seg]=index-1;
            }
}
/************************************************************************
 * lcd_14_seg_scroll

This routine scrolls the array seg7display of a specific 14segment defined 
by the input seg. The direction is set by the input dir. When dir==true it 
will scroll up and when dir==false it will scroll down.

Parameters:		LCD pointer,direction(true=up,false=down),segment (6-11)
Return:			None
Side Effects:	None
************************************************************************/
void lcd_14seg_scroll(lcd *p,bool dir,int seg)
{
        int index=p->seg[seg];
            if(dir==true)//up
            {
                if(index==SEG14_SIZE-1) index=-1;
                p->canvas[seg].byte=seg14display[index+1];
                p->seg[seg]=index+1;
            }
            else//down
            {
                if(index==0) index=SEG14_SIZE;
                p->canvas[seg].byte=seg14display[index-1];
                p->seg[seg]=index-1;
            }
}
void lcd_14seg_scroll_read_scale_offset(lcd *p,bool dir,int seg,uint8_t type)
{
        int index=p->seg[seg];
        float value=0;
            
        if(type==0){
            
        if(dir==true)//up
            {
                switch(p->displayCursor)
                {
                    case 10:
                        value=value+1000;
                        if(index==35)value=value-10000; 
                        break;
                    case 9:
                        value=value+100;
                        if(index==35) value=value-1000;
                        break;
                    case 8:
                        value=value+10;
                        if(index==35) value=value-100;
                        break;
                    case 7:
                        value=value+1;
                        if(index==35) value=value-10;
                        break;
                    case 6:
                        value=value+0.1;
                        if(index==35) value=value-1;
                        break;
                }
                
                if(index==35) index=25; 
               
                p->canvas[seg].byte=seg14display[index+1];
                p->seg[seg]=index+1;
            }
            else//down26-35
            {
                switch(p->displayCursor)
                {
                    case 10:
                        value=value-1000;
                        if(index==26) value=value+10000;
                        break;
                    case 9:
                        value=value-100;
                        if(index==26) value=value+1000;
                        break;
                    case 8:
                        value=value-10;
                        if(index==26) value=value+100;
                        break;
                    case 7:
                        value=value-1;
                        if(index==26) value=value+10;
                        break;
                    case 6:
                        value=value-0.1;
                        if(index==26) value=value+1;
                        break;
                }
                if(index==26) index=36; 
                p->canvas[seg].byte=seg14display[index-1];
                p->seg[seg]=index-1;
            }
            p->read_offset=p->read_offset+value;
        }
        if(type==1)
        {
            if(dir==true)//up
            {
                switch(p->displayCursor)
                {
                    case 11:
                        value=value+10;
                        if(index==35) value=value-100;
                        break;
                    case 10:
                        value=value+1;
                        if(index==35)value=value-10; 
                        break;
                    case 9:
                        value=value+.1;
                        if(index==35) value=value-1;
                        break;
                    case 8:
                        value=value+0.01;
                        if(index==35) value=value-.1;
                        break;
                    case 7:
                        value=value+0.001;
                        if(index==35) value=value-0.01;
                        break;
                    case 6:
                        value=value+0.0001;
                        if(index==35) value=value-0.001;
                        break;
                }
                
                if(index==35) index=25; 
               
                p->canvas[seg].byte=seg14display[index+1];
                p->seg[seg]=index+1;
            }
            else//down26-35
            {
                switch(p->displayCursor)
                {
                    case 11:
                        value=value-10;
                        if(index==26) value=value+100;
                        break;
                    case 10:
                        value=value-1;
                        if(index==26) value=value+10;
                        break;
                    case 9:
                        value=value-0.1;
                        if(index==26) value=value+1;
                        break;
                    case 8:
                        value=value-0.01;
                        if(index==26) value=value+0.1;
                        break;
                    case 7:
                        value=value-0.001;
                        if(index==26) value=value+0.01;
                        break;
                    case 6:
                        value=value-0.0001;
                        if(index==26) value=value+0.001;
                        break;
                }
                if(index==26) index=36; 
                p->canvas[seg].byte=seg14display[index-1];
                p->seg[seg]=index-1;
            }
            p->read_scale=p->read_scale+value;
        }
}
void lcd_14seg_scroll_scale_offset(lcd *p,bool dir,int seg,uint8_t type)
{
        int index=p->seg[seg];
        float value=0;
            
        if(type==0) 
        {
            if(dir==true)//up
            {
                switch(p->displayCursor)
                {
                    case 10:
                        value=value+10;
                        if(index==35)value=value-100; 
                        break;
                    case 9:
                        value=value+1;
                        if(index==35) value=value-10;
                        break;
                    case 8:
                        value=value+0.1;
                        if(index==35) value=value-1;
                        break;
                    case 7:
                        value=value+0.01;
                        if(index==35) value=value-0.1;
                        break;
                    case 6:
                        value=value+0.001;
                        if(index==35) value=value-0.01;
                        break;
                }
                
                if(index==35) index=25; 
               
                p->canvas[seg].byte=seg14display[index+1];
                p->seg[seg]=index+1;
            }
            else//down26-35
            {
                switch(p->displayCursor)
                {
                    case 10:
                        value=value-10;
                        if(index==26) value=value+100;
                        break;
                    case 9:
                        value=value-1;
                        if(index==26) value=value+10;
                        break;
                    case 8:
                        value=value-0.1;
                        if(index==26) value=value+1;
                        break;
                    case 7:
                        value=value-0.01;
                        if(index==26) value=value+0.1;
                        break;
                    case 6:
                        value=value-0.001;
                        if(index==26) value=value+0.01;
                        break;
                }
                if(index==26) index=36; 
                p->canvas[seg].byte=seg14display[index-1];
                p->seg[seg]=index-1;
            }
            if(p->offset<0) value=value*-1;
            p->offset=p->offset+value;
        }
        if(type==1)
        {
            if(dir==true)//up
            {
                switch(p->displayCursor)
                {
                    case 11:
                        value=value+1;
                        if(index==35) value=value-10;
                        break;
                    case 10:
                        value=value+.1;
                        if(index==35)value=value-1; 
                        break;
                    case 9:
                        value=value+.01;
                        if(index==35) value=value-.1;
                        break;
                    case 8:
                        value=value+0.001;
                        if(index==35) value=value-.01;
                        break;
                    case 7:
                        value=value+0.0001;
                        if(index==35) value=value-0.001;
                        break;
                    case 6:
                        value=value+0.00001;
                        if(index==35) value=value-0.0001;
                        break;
                }
                
                if(index==35) index=25; 
               
                p->canvas[seg].byte=seg14display[index+1];
                p->seg[seg]=index+1;
            }
            else//down26-35
            {
                switch(p->displayCursor)
                {
                    case 11:
                        value=value-1;
                        if(index==26) value=value+10;
                        break;
                    case 10:
                        value=value-0.1;
                        if(index==26) value=value+1;
                        break;
                    case 9:
                        value=value-0.01;
                        if(index==26) value=value+0.1;
                        break;
                    case 8:
                        value=value-0.001;
                        if(index==26) value=value+0.01;
                        break;
                    case 7:
                        value=value-0.0001;
                        if(index==26) value=value+0.001;
                        break;
                    case 6:
                        value=value-0.00001;
                        if(index==26) value=value+0.0001;
                        break;
                }
                if(index==26) index=36; 
                p->canvas[seg].byte=seg14display[index-1];
                p->seg[seg]=index-1;
            }
            p->scale=p->scale+value;
        }
}
/************************************************************************
 * lcd_7seg_decimal

This routine scrolls the decimal of number which sets the decimal for 
the number, min, and max displays. It will display where the decimal 
only on the number display. The direction is set by the input dir. 
When dir==true it will scroll left and when dir==false it will scroll right.

Parameters:		LCD pointer,direction(true=left,false=right)
Return:			None
Side Effects:	p->decimal is set
************************************************************************/
void lcd_7seg_decimal(lcd *p,bool dir)
{
    int seg;
 //   if(d==1)
 //   {
    seg=p->decimal; 
    if(dir==true)
    {
        p->canvas[seg].bit7=0;
        if(seg==DIS_NUM_START) seg=DIS_NUM_END+1;
        p->canvas[seg-1].bit7=1;
        p->decimal=seg-1;//1-5,1=no decimal showing
    }
    else
    {
        p->canvas[seg].bit7=0;
        if(seg==DIS_NUM_END) seg=DIS_NUM_START-1;
        p->canvas[seg+1].bit7=1;
        p->decimal=seg+1;
    }
}
/************************************************************************
 * lcd_14seg_decimal

This routine scrolls the decimal of number which sets the decimal for 
the menu display. The direction is set by the input dir.
When dir==true it will scroll left and when dir==false it will scroll right.

Parameters:		LCD pointer,direction(true=left,false=right)
Return:			None
Side Effects:	None
************************************************************************/
void lcd_14seg_decimal(lcd *p,bool up)
{
    int seg;
    seg=p->menudisp;
    if(up)
    {
        p->canvas[seg].bit14=0;
        if(seg==DIS_MENU_START-1) seg=DIS_MENU_END-1;
        p->canvas[seg+1].bit14=1;
        p->menudisp=seg+1;
    }
    else if (!up)
    {
        p->canvas[seg].bit14=0;
        if(seg==DIS_MENU_END) seg=DIS_MENU_START;
        p->canvas[seg-1].bit14=1;
        p->menudisp=seg-1;
    }
}
/************************************************************************
 * all_on

 * Turns the whole display on in the pointer

Parameters:		LCD pointer
Return:			None
Side Effects:	None
************************************************************************/
void all_on(lcd *p)
{ 
    int i;
    
    for(i=0; i<27; i++)
    {
      p->canvas[i].byte=0xFFFF;  
    }    
 
}
/************************************************************************
 * lcd_set_canvas

 This function actually turns on whatever the lcd canvas is set to.

Parameters:		LCD pointer
Return:			None
Side Effects:	None
************************************************************************/
void lcd_set_canvas(lcd *p)
{
    LCD_1A = p->canvas[1].bit0;
    LCD_1B = p->canvas[1].bit1;
    LCD_1C = p->canvas[1].bit2;
    LCD_1D = p->canvas[1].bit3;
    LCD_1E = p->canvas[1].bit4;
    LCD_1F = p->canvas[1].bit5;
    LCD_1G = p->canvas[1].bit6;
    
    LCD_2A = p->canvas[2].bit0;
    LCD_2B = p->canvas[2].bit1;
    LCD_2C = p->canvas[2].bit2;
    LCD_2D = p->canvas[2].bit3;
    LCD_2E = p->canvas[2].bit4;
    LCD_2F = p->canvas[2].bit5;
    LCD_2G = p->canvas[2].bit6;
    LCD_P2 = p->canvas[2].bit7;
    
    LCD_3A = p->canvas[3].bit0;
    LCD_3B = p->canvas[3].bit1;
    LCD_3C = p->canvas[3].bit2;
    LCD_3D = p->canvas[3].bit3;
    LCD_3E = p->canvas[3].bit4;
    LCD_3F = p->canvas[3].bit5;
    LCD_3G = p->canvas[3].bit6;
    LCD_P3 = p->canvas[3].bit7;
   
    LCD_4A = p->canvas[4].bit0;
    LCD_4B = p->canvas[4].bit1;
    LCD_4C = p->canvas[4].bit2;
    LCD_4D = p->canvas[4].bit3;
    LCD_4E = p->canvas[4].bit4;
    LCD_4F = p->canvas[4].bit5;
    LCD_4G = p->canvas[4].bit6;
    LCD_P4 = p->canvas[4].bit7;
   
    LCD_5A = p->canvas[5].bit0;
    LCD_5B = p->canvas[5].bit1;
    LCD_5C = p->canvas[5].bit2;
    LCD_5D = p->canvas[5].bit3;
    LCD_5E = p->canvas[5].bit4;
    LCD_5F = p->canvas[5].bit5;
    LCD_5G = p->canvas[5].bit6;
    LCD_P5 = p->canvas[5].bit7;
    
    LCD_6A = p->canvas[6].bit0;
    LCD_6B = p->canvas[6].bit1;
    LCD_6C = p->canvas[6].bit2;
    LCD_6D = p->canvas[6].bit3;
    LCD_6E = p->canvas[6].bit4;
    LCD_6F = p->canvas[6].bit5;
    LCD_6G = p->canvas[6].bit6;
    LCD_6H = p->canvas[6].bit7;
    LCD_6I = p->canvas[6].bit8;
    LCD_6J = p->canvas[6].bit9;
    LCD_6K = p->canvas[6].bit10;
    LCD_6L = p->canvas[6].bit11;
    LCD_6M = p->canvas[6].bit12;
    LCD_6N = p->canvas[6].bit13;
    LCD_P7 = p->canvas[6].bit14;
   
    LCD_7A = p->canvas[7].bit0;
    LCD_7B = p->canvas[7].bit1;
    LCD_7C = p->canvas[7].bit2;
    LCD_7D = p->canvas[7].bit3;
    LCD_7E = p->canvas[7].bit4;
    LCD_7F = p->canvas[7].bit5;
    LCD_7G = p->canvas[7].bit6;
    LCD_7H = p->canvas[7].bit7;
    LCD_7I = p->canvas[7].bit8;
    LCD_7J = p->canvas[7].bit9;
    LCD_7K = p->canvas[7].bit10;
    LCD_7L = p->canvas[7].bit11;
    LCD_7M = p->canvas[7].bit12;
    LCD_7N = p->canvas[7].bit13;
    LCD_P8 = p->canvas[7].bit14;
    
    LCD_8A = p->canvas[8].bit0;
    LCD_8B = p->canvas[8].bit1;
    LCD_8C = p->canvas[8].bit2;
    LCD_8D = p->canvas[8].bit3;
    LCD_8E = p->canvas[8].bit4;
    LCD_8F = p->canvas[8].bit5;
    LCD_8G = p->canvas[8].bit6;
    LCD_8H = p->canvas[8].bit7;
    LCD_8I = p->canvas[8].bit8;
    LCD_8J = p->canvas[8].bit9;
    LCD_8K = p->canvas[8].bit10;
    LCD_8L = p->canvas[8].bit11;
    LCD_8M = p->canvas[8].bit12;
    LCD_8N = p->canvas[8].bit13;
    LCD_P9 = p->canvas[8].bit14;
    
    LCD_9A = p->canvas[9].bit0;
    LCD_9B = p->canvas[9].bit1;
    LCD_9C = p->canvas[9].bit2;
    LCD_9D = p->canvas[9].bit3;
    LCD_9E = p->canvas[9].bit4;
    LCD_9F = p->canvas[9].bit5;
    LCD_9G = p->canvas[9].bit6;
    LCD_9H = p->canvas[9].bit7;
    LCD_9I = p->canvas[9].bit8;
    LCD_9J = p->canvas[9].bit9;
    LCD_9K = p->canvas[9].bit10;
    LCD_9L = p->canvas[9].bit11;
    LCD_9M = p->canvas[9].bit12;
    LCD_9N = p->canvas[9].bit13;
    LCD_P10 = p->canvas[9].bit14;
   
    LCD_10A = p->canvas[10].bit0;
    LCD_10B = p->canvas[10].bit1;
    LCD_10C = p->canvas[10].bit2;
    LCD_10D = p->canvas[10].bit3;
    LCD_10E = p->canvas[10].bit4;
    LCD_10F = p->canvas[10].bit5;
    LCD_10G = p->canvas[10].bit6;
    LCD_10H = p->canvas[10].bit7;
    LCD_10I = p->canvas[10].bit8;
    LCD_10J = p->canvas[10].bit9;
    LCD_10K = p->canvas[10].bit10;
    LCD_10L = p->canvas[10].bit11;
    LCD_10M = p->canvas[10].bit12;
    LCD_10N = p->canvas[10].bit13;
    LCD_P11 = p->canvas[10].bit14;
    
    LCD_11A = p->canvas[11].bit0;
    LCD_11B = p->canvas[11].bit1;
    LCD_11C = p->canvas[11].bit2;
    LCD_11D = p->canvas[11].bit3;
    LCD_11E = p->canvas[11].bit4;
    LCD_11F = p->canvas[11].bit5;
    LCD_11G = p->canvas[11].bit6;
    LCD_11H = p->canvas[11].bit7;
    LCD_11I = p->canvas[11].bit8;
    LCD_11J = p->canvas[11].bit9;
    LCD_11K = p->canvas[11].bit10;
    LCD_11L = p->canvas[11].bit11;
    LCD_11M = p->canvas[11].bit12;
    LCD_11N = p->canvas[11].bit13;
    
    LCD_12A = p->canvas[12].bit0;
    LCD_12B = p->canvas[12].bit1;
    LCD_12C = p->canvas[12].bit2;
    LCD_12D = p->canvas[12].bit3;
    LCD_12E = p->canvas[12].bit4;
    LCD_12F = p->canvas[12].bit5;
    LCD_12G = p->canvas[12].bit6;
    
    LCD_13A = p->canvas[13].bit0;
    LCD_13B = p->canvas[13].bit1;
    LCD_13C = p->canvas[13].bit2;
    LCD_13D = p->canvas[13].bit3;
    LCD_13E = p->canvas[13].bit4;
    LCD_13F = p->canvas[13].bit5;
    LCD_13G = p->canvas[13].bit6;
    LCD_P13 = p->canvas[13].bit7;
    
    LCD_14A = p->canvas[14].bit0;
    LCD_14B = p->canvas[14].bit1;
    LCD_14C = p->canvas[14].bit2;
    LCD_14D = p->canvas[14].bit3;
    LCD_14E = p->canvas[14].bit4;
    LCD_14F = p->canvas[14].bit5;
    LCD_14G = p->canvas[14].bit6;
    LCD_P14 = p->canvas[14].bit7;
    
    LCD_15A = p->canvas[15].bit0;
    LCD_15B = p->canvas[15].bit1;
    LCD_15C = p->canvas[15].bit2;
    LCD_15D = p->canvas[15].bit3;
    LCD_15E = p->canvas[15].bit4;
    LCD_15F = p->canvas[15].bit5;
    LCD_15G = p->canvas[15].bit6;
    LCD_P15 = p->canvas[15].bit7;
   
    LCD_16A = p->canvas[16].bit0;
    LCD_16B = p->canvas[16].bit1;
    LCD_16C = p->canvas[16].bit2;
    LCD_16D = p->canvas[16].bit3;
    LCD_16E = p->canvas[16].bit4;
    LCD_16F = p->canvas[16].bit5;
    LCD_16G = p->canvas[16].bit6;
   
    LCD_17A = p->canvas[17].bit0;
    LCD_17B = p->canvas[17].bit1;
    LCD_17C = p->canvas[17].bit2;
    LCD_17D = p->canvas[17].bit3;
    LCD_17E = p->canvas[17].bit4;
    LCD_17F = p->canvas[17].bit5;
    LCD_17G = p->canvas[17].bit6;
    LCD_P17 = p->canvas[17].bit7;
    
    LCD_18A = p->canvas[18].bit0;
    LCD_18B = p->canvas[18].bit1;
    LCD_18C = p->canvas[18].bit2;
    LCD_18D = p->canvas[18].bit3;
    LCD_18E = p->canvas[18].bit4;
    LCD_18F = p->canvas[18].bit5;
    LCD_18G = p->canvas[18].bit6;
    LCD_P18 = p->canvas[18].bit7;
  
    LCD_19A = p->canvas[19].bit0;
    LCD_19B = p->canvas[19].bit1;
    LCD_19C = p->canvas[19].bit2;
    LCD_19D = p->canvas[19].bit3;
    LCD_19E = p->canvas[19].bit4;
    LCD_19F = p->canvas[19].bit5;
    LCD_19G = p->canvas[19].bit6;
    LCD_P19 = p->canvas[19].bit7;
    
    LCD_20A = p->canvas[20].bit0;
    LCD_20B = p->canvas[20].bit1;
    LCD_20C = p->canvas[20].bit2;
    LCD_20D = p->canvas[20].bit3;
    LCD_20E = p->canvas[20].bit4;
    LCD_20F = p->canvas[20].bit5;
    LCD_20G = p->canvas[20].bit6;
    LCD_P20 = p->canvas[20].bit7;
    
    LCD_S1 = p->canvas[21].bit0;
    LCD_S2 = p->canvas[21].bit1;
    LCD_S3 = p->canvas[21].bit2;
    LCD_S4 = p->canvas[21].bit3;
    LCD_S5 = p->canvas[21].bit4;
    LCD_S6 = p->canvas[21].bit5;
    LCD_S7 = p->canvas[21].bit6;
    
    LCD_S8 =  p->canvas[22].bit0;
    LCD_S9 =  p->canvas[22].bit1;
    LCD_S10 = p->canvas[22].bit2;
    LCD_S11 = p->canvas[22].bit3;
    LCD_S12 = p->canvas[22].bit4;
    LCD_S13 = p->canvas[22].bit5;
    LCD_S14 = p->canvas[22].bit6;
    
    LCD_S15 = p->canvas[23].bit0;
    LCD_S16 = p->canvas[23].bit1;
    LCD_S17 = p->canvas[23].bit2;
    LCD_S18 = p->canvas[23].bit3;
    LCD_S19 = p->canvas[23].bit4;
    LCD_S20 = p->canvas[23].bit5;
    LCD_S21 = p->canvas[23].bit6;
    
    LCD_S22 = p->canvas[24].bit0;
    LCD_S23 = p->canvas[24].bit1;
    LCD_S24 = p->canvas[24].bit2;
    LCD_S25 = p->canvas[24].bit3;
    LCD_S26 = p->canvas[24].bit4;
    LCD_S27 = p->canvas[24].bit5;
    LCD_S28 = p->canvas[24].bit6;
    
    LCD_S29 = p->canvas[25].bit0;
    LCD_S30 = p->canvas[25].bit1;
    LCD_S31 = p->canvas[25].bit2;
    LCD_S32 = p->canvas[25].bit3;
    LCD_S33 = p->canvas[25].bit4;
    LCD_S34 = p->canvas[25].bit5;
    LCD_S35 = p->canvas[25].bit6;
  
    LCD_X1 = p->canvas[26].bit0;
    LCD_X2 = p->canvas[26].bit1;
    
    
    
}

lcd *lcd_new(void)
{
    lcd *p = (lcd *)malloc(sizeof(lcd));
    return p;
}