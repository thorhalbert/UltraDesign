#include <stdio.h>
#include <intuition/intuition.h>

extern short DisplayAlert();
extern struct Library *OpenLibrary();
extern void CloseLibrary();

sysalert(message)
char *message;
{
    static char msg1[] = "UltraDesign -- Insufficient Memory";
    
    char msg2[80];
    char workstr[200];
    int i, endcnt;

	sprintf(msg2,"%s - Click to Continue",message);

    for (i = 0; i < 200; i++)
        workstr[i] = '\0';
    workstr[0] = (UBYTE)0x00;
    workstr[1] = (UBYTE)0xa4;
    workstr[2] = (UBYTE)0x10;

    for (i = 0; i < strlen(msg1); i++)
        workstr[3 + i] = msg1[i];
    endcnt = 4 + i;
    workstr[endcnt++] = (UBYTE)0x01;
    workstr[endcnt++] = (UBYTE)0x00;
    workstr[endcnt++] = (UBYTE)0x80;
    workstr[endcnt++] = (UBYTE)0x20;
    for (i = 0; i < strlen(msg2); i++)
        workstr[endcnt + i] = msg2[i];

    DisplayAlert(0x00010000L, workstr, 50L);

}
