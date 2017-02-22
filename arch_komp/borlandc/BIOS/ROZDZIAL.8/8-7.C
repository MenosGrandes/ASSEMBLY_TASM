/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-7.  Wl¥czenie/wyˆ¥czenie dziaˆania klawisza Print Screen
*/


/* Wyˆ¥czenie klawisza Print Screen */

#define pStat (unsigned char far *)0x00500000

void disablePrintScrn( void )
{
#define INPROGRESS 1         /* 1 = trwa drukowanie */

*pStat = INPROGRESS;         /* Powoduje ignorowanie przez BIOS
                                ¾¥dania drukowania zawarto˜ci ekranu */
}


/* Wˆ¥czenie klawisza Print Screen */
void enablePrintScrn( void )
{
#define COMPLETE 0           /* 0 = drukowanie zakoäczone */

*pStat = COMPLETE;           /* BIOS akceptuje ¾¥danie druku */
}
