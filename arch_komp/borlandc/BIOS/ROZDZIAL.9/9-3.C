/*
   BIOS dla programujących w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-3.  Odczytanie bieżącego czasu z RTC
*/


/* Odczytanie bieżącego czasu z RTC i jego konwersja na liczby całkowite */
void readRTCTime( unsigned char *hour, 
                  unsigned char *min, 
                  unsigned char *sec )
{
#define tensDigit( bcdValue ) ( (bcdValue >> 4) * 10 )
#define onesDigit( bcdValue ) ( bcdValue & 15 )

union REGS regs;

regs.h.ah = 2;                     /* AH = 2 - odczytaj bieżący czas */
int86( 0x1a, &regs, &regs );       /* Wywołanie funkcji BIOS */

                                   /* CH = godzina w BCD */
*hour = tensDigit( regs.h.ch );    /* Zamiana dziesiątek */
*hour += onesDigit( regs.h.ch );   /* dodanie reszty */

                                   /* CL = minuty w BCD */
*min = tensDigit( regs.h.cl );     /* Zamiana dziesiątek */
*min += onesDigit( regs.h.cl );    /* dodanie reszty */

                                   /* DH = sekundy w BCD */
*sec = tensDigit( regs.h.dh );     /* Zamiana dziesiątek */
*sec += onesDigit( regs.h.dh );    /* dodanie reszty */
}
