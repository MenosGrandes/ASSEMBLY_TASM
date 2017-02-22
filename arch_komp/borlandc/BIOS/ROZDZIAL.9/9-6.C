/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-6.  Ustawienie daty w RTC
*/


/* Zamienia dat© podan¥ jako liczby caˆkowite na kod BCD i ustawia RTC.
   Dozwolone warto˜ci:  Miesi¥c = 1 (styczeä) do 12 (grudzieä)
                        Dzieä   = 1 do 31
                        Lata w peˆnej formie (np. 1992)
   Zwraca 0 przy powodzeniu lub 1 przy zˆych parametrach.
*/

unsigned char setRTCDate( int year, 
                          char month, 
                          char day )
{
static daysInMonths[] = { 00, 31,28,31,30,31,30,
                              31,31,30,31,30,31 };
union REGS regs;

/* Sprawdzenie poprawno˜ci daty */
if( (month > 12)  || (month == 0) ||
    (day > 31)    || (day == 0) )
   return( 1 );                            /* Zwracamy 1 - zˆy parametr */

/* Uwzgl©dnienie lat przest©pnych */
daysInMonths[2] = 28;
daysInMonths[2] += ( (year%4) == 0 );

if( day > daysInMonths[month] )
   return( 1 );                            /* Zwracamy 1 - zˆy parametr */

regs.h.ah = 5;                             /* AH = 5 - ustaw dat© w RTC */

                                           /* CH = stulecia w BCD */
regs.h.ch  = ( ( year / 100) / 10 ) << 4;  /* Konwersja dziesi¥tek */
regs.h.ch |= ( ( year / 100) % 10 );       /* Konwersja jedno˜ci */

                                           /* CL = lata w BCD */
regs.h.cl  = ( (year % 100) / 10 ) << 4;   /* Konwersja dziesi¥tek */
regs.h.cl |= ( year % 10 );                /* Konwersja jedno˜ci */

                                           /* DH = miesi¥c w BCD */
regs.h.dh  = ( month / 10 ) << 4;          /* Konwersja dziesi¥tek */
regs.h.dh |= ( month % 10 );               /* Konwersja jedno˜ci */

                                           /* DL = dzieä w BCD */
regs.h.dl  = ( day / 10 ) << 4;            /* Konwersja dziesi¥tek */
regs.h.dl |= ( day % 10 );                 /* Konwersja jedno˜ci */

int86( 0x1a, &regs, &regs );               /* Wywoˆanie funkcji BIOS */
return( 0 );                               /* Zwracamy 0 = powodzenie */
}
