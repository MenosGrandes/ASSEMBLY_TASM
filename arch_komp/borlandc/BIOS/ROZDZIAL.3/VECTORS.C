/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-4.  Odczytanie tablicy wektor¢w przerwaä
*/


/*
   VECTORS.C 
   Wy˜wietlenie tablicy wektor¢w przerwaä
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#define IRET     0xcf         /* instrukcja IRET procesora        */
#define ROMSEG 0xf000         /* Segment pocz¥tku ROM             */
#define OPTROM 0xa000         /* Segment pocz¥tku dodatkowego ROM */

unsigned int intNr,
             segment,
             offset;

/* vect jest wska«nikiem do tablicy wektor¢w przerwaä */
unsigned char far * far *vect = NULL;

void main( void )
{
printf( "\n            Adres       Warto˜†      Rodzaj" );
printf( "\nPrzerwanie  wektora     wektora      obsˆugi" );
printf( "\n________________________________________________" );

for( intNr=0; intNr!=256; intNr++ )
{
   segment = FP_SEG( *vect );
   offset  = FP_OFF( *vect );

   printf( "\n  %.2Xh     0000:%.4Xh   %.4X:%.4Xh   ",
            intNr,
            intNr * 4,
            segment,
            offset );

   if ( *vect == NULL )
      printf( "nieu¾ywany" );
   else
   {
      if ( (intNr==0x1d) || (intNr==0x1e) || 
           (intNr==0x1f) || (intNr==0x41) || 
           (intNr==0x43) || (intNr==0x46) )
         printf( "Wska«nik do tablicy" );
      else
      {
         if ( **vect == IRET )
            printf( "Nic nie robi" );
         else
         {
            if ( segment >= ROMSEG )
               printf( "ROM systemowy" );
            else
            {
               if ( segment >= OPTROM )
                  printf( "ROM dodatkowy" );
               else
                  printf( "Pami©† podstawowa" );
               }
            }
         }
      }
   ++vect;
   }
exit( 0 );
}
