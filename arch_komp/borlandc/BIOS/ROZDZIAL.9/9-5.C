/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-5.  Ustawienie czasu w RTC
*/


/* Konwersja czasu z liczb caˆkowitych na BCD i ustawienie go w RTC.
   Warto˜ci s¥ obliczane (i sprawdzane) w formacie 24-ro godzinnym.
   Funkcja zwraca 0 przy powodzeniu lub 1 przy zˆych parametrach.
*/

unsigned char setRTCTime( char hour, 
                          char minute, 
                          char second )
{
union REGS regs;

if( (hour > 23) || (minute > 59) || (second > 59) )
   return( 1 );                     /* Zwracamy 1 = zˆy parametr */

regs.h.ah  = 3;                     /* AH = 3 - ustaw czas */

                                    /* CH = godzina w BCD */
regs.h.ch  = ( hour / 10 ) << 4;    /* Konwersja dziesi¥tek */
regs.h.ch |= ( hour % 10 );         /* Konwersja jedno˜ci */

                                    /* CL = minuty w BCD */
regs.h.cl  = ( minute / 10 ) << 4;  /* Konwersja dziesi¥tek */
regs.h.cl |= ( minute % 10 );       /* Konwersja jedno˜ci */

                                    /* DH = sekundy w BCD */
regs.h.dh  = ( second / 10 ) << 4;  /* Konwersja dziesi¥tek */
regs.h.dh |= ( second % 10 );       /* Konwersja jedno˜ci */

                                    /* DL = 0 czas zimowy */
                                    /*      1 czas letni */
regs.h.dl  = 0;                     /* Ustawiamy czas zimowy */

int86( 0x1a, &regs, &regs );        /* Wywoˆanie funkcji BIOS */
return( 0 );                        /* Zwracamy 0 = powodzenie */
}
