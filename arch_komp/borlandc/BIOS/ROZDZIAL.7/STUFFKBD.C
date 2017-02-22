/*
   BIOS dla programuj¥cych w C i C++
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

/* Sprawd« czy linia polecenia jest kompletna */
if( argc == 1 )
   printf( "Skˆadnia: %s \"tekst do bufora\"\7",argv[0] );
else
{
   pCh = argv[1];
/* Zaˆadowanie wszystkich znak¢w z linii polecenia */
   do
   {
/* Rozpoznanie sekwencji \E dla Entera i \T dla tabulatora */
      if ( *pCh == '\\' )
      {                                /* Znaleziono znak \ */
         ++pCh;                        /* Sprawdzenie nast©pnego znaku */
         if ( *pCh == 0 )              /* Je¾eli koniec tekstu, */
            break;                     /*    skoäcz p©tl© */
         switch ( *pCh )
         {
/* Je¾eli znak to \E lub \e, wy˜lij do bufora kod Entera */
            case 'e' :
            case 'E' :
               regs.h.cl = ENTER;
               break;
/* Je¾eli znak to \T lub  \t, wy˜lij do bufora znak tabulatora */
            case 't' :
            case 'T' :
               regs.h.cl = TAB;
               break;
/* W pozostaˆych wypadkach wy˜lij znak po znaku \ */
            default  :
               regs.h.cl = *pCh;
               break;
            }
         }
/* Znak jest r¢¾ny od \ - dodanie go do bufora */
      else
         regs.h.cl = *pCh;

/* Wywoˆanie funkcji BIOSu wpisuj¥cej znak do bufora */
      regs.h.ah = 5;      /* Przerwanie 16h funkcja 5 */
      regs.h.ch = 0;      /* kod klawisza = 0 (zwykˆy znak) */
      int86( 0x16, &regs, &regs );

/* Ostrze¾enie je¾eli bufor jest peˆen */
      if ( regs.h.al != 0 )
      {
         printf( "Przykro mi, ale bufor klawiatury jest zapeˆniony!\7" );
         break;
         }

/* Przejd« do nast©pnego znaku i powtarzaj do koäca tekstu */
      ++pCh;
      } while( *pCh != 0 );
   }
}
