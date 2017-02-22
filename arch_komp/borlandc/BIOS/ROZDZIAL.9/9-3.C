/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-3.  Odczytanie bie¾¥cego czasu z RTC
*/


/* Odczytanie bie¾¥cego czasu z RTC i jego konwersja na liczby caˆkowite */
void readRTCTime( unsigned char *hour, 
                  unsigned char *min, 
                  unsigned char *sec )
{
#define tensDigit( bcdValue ) ( (bcdValue >> 4) * 10 )
#define onesDigit( bcdValue ) ( bcdValue & 15 )

union REGS regs;

regs.h.ah = 2;                     /* AH = 2 - odczytaj bie¾¥cy czas */
int86( 0x1a, &regs, &regs );       /* Wywoˆanie funkcji BIOS */

                                   /* CH = godzina w BCD */
*hour = tensDigit( regs.h.ch );    /* Zamiana dziesi¥tek */
*hour += onesDigit( regs.h.ch );   /* dodanie reszty */

                                   /* CL = minuty w BCD */
*min = tensDigit( regs.h.cl );     /* Zamiana dziesi¥tek */
*min += onesDigit( regs.h.cl );    /* dodanie reszty */

                                   /* DH = sekundy w BCD */
*sec = tensDigit( regs.h.dh );     /* Zamiana dziesi¥tek */
*sec += onesDigit( regs.h.dh );    /* dodanie reszty */
}
