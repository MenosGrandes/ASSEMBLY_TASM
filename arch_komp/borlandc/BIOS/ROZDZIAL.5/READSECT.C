/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-5.  Odczytanie dyskietki sektor po sektorze
*/


/*
   READSECT.C
   Odczytanie caˆej dyskietki (znanego typu) sektor po sektorze
*/

#include <dos.h>

/* Prototyp funkcji */
unsigned char readDisketteSectors( unsigned char, unsigned char, 
                                   unsigned char, unsigned char, 
                                   unsigned char, unsigned char far * );

#define LASTHEAD         1          /*   2 gˆowice (0 i 1) */
#define LASTTRACK       79          /*  80 ˜cie¾ek (0 - 79) */
#define LASTSECTOR      15          /*  15 sektor¢w na ˜cie¾ce (1 - 15) */
#define BYTESPERSECTOR 512          /* 512 bajt¢w w sektorze */
#define driveToRead      0          /* Nap©d A = 0 */
#define ONE_SECTOR       1          /* Czytaj tylko jeden sektor w cyklu */
char diskBuffer[ BYTESPERSECTOR ];  /* Alokacja bufora dysku */

void main( void )
{
unsigned char currentHead, 
              currentTrack, 
              currentSector;

for( currentTrack = 0; 
        currentTrack <= LASTTRACK; 
           currentTrack++ )
   for( currentHead = 0; 
           currentHead <= LASTHEAD; 
              currentHead++ )
      for( currentSector = 1; 
              currentSector <= LASTSECTOR; 
                 currentSector++ )
         readDisketteSectors( driveToRead,
                              ONE_SECTOR,
                              currentHead,
                              currentTrack, 
                              currentSector, 
                              diskBuffer );
}

/*
   Czytanie sektor¢w z dyskietki.
   Zwraca 0 lub kod bˆ©du przerwania 13h.
*/
unsigned char readDisketteSectors( unsigned char drive, 
                                   unsigned char sectorsToRead, 
                                   unsigned char head, 
                                   unsigned char track,
                                   unsigned char sector, 
                                   unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 2;                 /* AH = 2 - odczyt sektor¢w */
regs.h.al = sectorsToRead;     /* AL = liczba sektor¢w do odczytania */
regs.x.bx = FP_OFF(bufferPtr); /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;              /* DH = gˆowica */
regs.h.dl = drive;             /* DL = numer nap©du */
regs.h.ch = track;             /* CH = numer ˜cie¾ki */
regs.h.cl = sector;            /* CL = numer sektora */
sregs.es  = FP_SEG(bufferPtr); /* ES = segment bufora */
int86x( 0x13, &regs, 
        &regs, &sregs );       /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );           /* AH = 0 lub kod bˆ©du */
}
