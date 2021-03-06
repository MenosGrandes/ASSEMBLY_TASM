/*
   BIOS dla programuj�cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-9.  Wpisanie tekstu do bufora klawiatury
*/


/*
   STUFFKBD.C
   Wpisywanie tekstu do bufora klawiatury.
   Uruchomienie:  STUFFKBD "tekst".
   Program rozpoznaje \E jako klawisz Enter
       i \T jako tabulator.
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#define TAB    9                    /* Tabulator   */
#define ENTER 13                    /* Enter       */

union REGS regs;

void main( int argc, char *argv[] )
{
char *pCh;

/* Sprawd� czy linia polecenia jest kompletna */
if( argc == 1 )
   printf( "Sk�adnia: %s \"tekst do bufora\"\7",argv[0] );
else
{
   pCh = argv[1];
/* Za�adowanie wszystkich znak�w z linii polecenia */
   do
   {
/* Rozpoznanie sekwencji \E dla Entera i \T dla tabulatora */
      if ( *pCh == '\\' )
      {                                /* Znaleziono znak \ */
         ++pCh;                        /* Sprawdzenie nast�pnego znaku */
         if ( *pCh == 0 )              /* Je�eli koniec tekstu, */
            break;                     /*    sko�cz p�tl� */
         switch ( *pCh )
         {
/* Je�eli znak to \E lub \e, wy�lij do bufora kod Entera */
            case 'e' :
            case 'E' :
               regs.h.cl = ENTER;
               break;
/* Je�eli znak to \T lub  \t, wy�lij do bufora znak tabulatora */
            case 't' :
            case 'T' :
               regs.h.cl = TAB;
               break;
/* W pozosta�ych wypadkach wy�lij znak po znaku \ */
            default  :
               regs.h.cl = *pCh;
               break;
            }
         }
/* Znak jest r��ny od \ - dodanie go do bufora */
      else
         regs.h.cl = *pCh;

/* Wywo�anie funkcji BIOSu wpisuj�cej znak do bufora */
      regs.h.ah = 5;      /* Przerwanie 16h funkcja 5 */
      regs.h.ch = 0;      /* kod klawisza = 0 (zwyk�y znak) */
      int86( 0x16, &regs, &regs );

/* Ostrze�enie je�eli bufor jest pe�en */
      if ( regs.h.al != 0 )
      {
         printf( "Przykro mi, ale bufor klawiatury jest zape�niony!\7" );
         break;
         }

/* Przejd� do nast�pnego znaku i powtarzaj do ko�ca tekstu */
      ++pCh;
      } while( *pCh != 0 );
   }
}
