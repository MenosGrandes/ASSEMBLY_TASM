/*
   BIOS dla programuj�cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-4.  Drukowanie pliku przerywane
                 naci�ni�ciem wybranego klawisza
*/


/*
   STOPPRNT.C
   Drukowanie pliku do portu LPT1, zatrzymywane
   naci�ni�ciem przez u�ytkownika S lub s.
   Sk�adnia: STOPPRNT <plik do wydrukowania>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dos.h>
#include <errno.h>

#define BUFFER_SIZE 81

void         getKbdChar       ( unsigned char *, 
                                unsigned char * );
unsigned int testKbdCharReady ( unsigned char *, 
                                unsigned char * );

void main( int argc, char *argv[] )
{
FILE *in, *out;
unsigned char scanCode,
              charCode,
              buffer[BUFFER_SIZE];

/* Sko�cz je�eli nie podano nazwy pliku */
if ( argc == 1 )
{
   printf( "Sk�adnia: %s <plik do wydrukowania>\7", argv[0] );
   exit( 1 );
   }

in = fopen( argv[1], "r+t" );        /* Otwarcie pliku */
if ( in == NULL )                    /* Przerwanie pracy je�eli */
{                                    /* nie mo�na otworzy� pliku */
   perror( "Nie mo�na otworzy� pliku " );
   exit( errno );
   }

out = fopen( "LPT1", "w+t" );        /* Otwarcie po��czenia z drukark� */
if ( in == NULL )                    /* Przerwanie pracy je�eli */
{                                    /* nie mo�na otworzy� po��czenia */
   perror( "Nie mo�na otworzy� LPT1 " );
   fclose( in );
   exit( errno );
   }

printf( "\nNaci�nij S, aby przerwa� drukowanie." );
while( fgets( buffer, BUFFER_SIZE, in ) )   /* powtarzaj a� do EOF */
{
   fputs( buffer, out );                    /* Drukuj nast�pny rekord */
   if ( testKbdCharReady( &charCode, 
                          &scanCode ) )     /* Naci�ni�to klawisz? */
   {                                        /* Tak */
      getKbdChar( &charCode, &scanCode );   /* Odczytaj klawisz z bufora */
      if ( toupper( charCode ) == 'S' )     /* Przerwij prac� je�eli
                                            /* to jest S lub s */
      {
         printf( "\n*** Drukowanie przerwane przez u�ytkownika ***" );
         break;
         }
      }
   }

fclose( in );
fclose( out );
exit( 0 );
}

/*
   Zwraca 0 je�eli bufor jest pusty 
   lub warto�� niezerow� je�eli w buforze s� dane
*/

unsigned int testKbdCharReady( unsigned char *charCode,
                               unsigned char *scanCode )
{
unsigned char localAH, 
              localAL;
unsigned int returnValue;

/* Przej�cie do asemblera aby wywo�a� przerwanie */
_asm  mov AH, 1             /* AH = 1 lub 11h - sprawdzenie klawiatury */
_asm  int 16h               /* Wywo�anie funkcji BIOS */
_asm  jnz endFunc           /* Zero = 0 oznacza znak w buforze */
_asm  xor AX,AX             /* W przeciwnym wypadku bufor jest pusty
                                - zwracamy 0 */
endFunc:                    /* Przeniesienie AH i AL do zmiennych */
_asm  mov localAH, AH       /* AH = kod klawisza */
_asm  mov localAL, AL       /* AL = kod znaku */
_asm  mov returnValue, AX   /* Zachowanie z�o�onej warto�ci 16-bitowej
                                jako warto�ci zwracanej przez funkcj� */
/* Powr�t do C i zako�czenie funkcji */
*scanCode = localAH;        /* Skopiowanie kodu klawisza */
*charCode = localAL;        /* Skopiowanie kodu znaku */
return( returnValue );      /* Je�eli s� dane, to returnValue != 0 */
}

/*
   Zwraca nast�pn� par� kod klawisza/kod znaku 
   z bufora klawiatury
*/

void getKbdChar( unsigned char *charCode,
                 unsigned char *scanCode )
{
union REGS regs;
regs.h.ah = 0;                /* AH = 0 lub 10h - odczytanie znaku  */
int86( 0x16, &regs, &regs );  /* Wywo�anie funkcji BIOS */
*scanCode = regs.h.ah;        /* AH = kod klawisza */
*charCode = regs.h.al;        /* AL = kod znaku */
}                             /* Niczego nie zwracamy */
