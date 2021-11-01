#ifndef USERINPUT_H_
#define USERINPUT_H_

typedef enum
{
    USERINPUT_eButton1,    /* Blue built-in button */
    USERINPUT_eButton2,    /* Lower button */
    USERINPUT_eButton3,    /* Upper button */
    USERINPUT_eButtonCount
} USERINPUT_teButton;

void USERINPUT_vInit(void);
bool USERINPUT_bPollButton(USERINPUT_teButton eButton);

#endif
