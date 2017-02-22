/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-4.  Odczytanie bie¾¥cej daty z RTC
*/


/* Odczytanie bie¾¥cej daty z RTC i jej konwersja na liczby caˆkowite */
void readRTCDate( unsigned int *year, 
                  unsigned char *month, 
                  unsigned char *day )
{
#define tensDigit( bcdValue ) ( (bcdValue >> 4) * 10 )
#define onesDigit( bcdValue ) ( bcdValue & 15 )

union REGS regs;
int temp;

regs.h.ah = 4;                     /* AH = 4 - odczytaj bie¾¥c¥ dat© */
int86( 0x1a, &regs, &regs );       /* Wywoˆanie funkcji BIOS */

                                   /* CH = stulecia w BCD */
*year = tensDigit( regs.h.ch );    /* Zamiana dziesi¥tek */
*year += onesDigit( regs.h.ch );   /* dodanie reszty */
*year *= 100;                      /* Pomno¾enie przez 100 */

                                   /* CL = rok w BCD */
temp = tensDigit( regs.h.cl );     /* Zamiana dziesi¥tek */
temp += onesDigit( regs.h.cl );    /* dodanie reszty */
*year += temp;                     /* dodanie lat */

                                   /* DH = miesi¥c BCD */
*month = tensDigit( regs.h.dh );   /* Zamiana dziesi¥tek */
*month += onesDigit( regs.h.dh );  /* dodanie reszty */

                                   /* DL = dzieä BCD */
*day = tensDigit( regs.h.dl );     /* Zamiana dziesi¥tek */
*day += onesDigit( regs.h.dl );    /* dodanie reszty */
}
