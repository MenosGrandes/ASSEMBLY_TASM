/*
   BIOS dla programuj쩭ych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-6.  Odczytanie dyskietki 쁟ie푟a po 쁟ie푖e
*/


/*
   READTRAK.C
   Odczytanie ca늖j dyskietki (znanego typu) 쁟ie푟a po 쁟ie푖e
*/

#include <dos.h>

/* Prototyp funkcji */
unsigned char readDisketteSectors( unsigned char, unsigned char, 
                                   unsigned char, unsigned char, 
                                   unsigned char, unsigned char far * );

#define LASTHEAD         1  /* 2 g늨wice (0 i 1) */
#define LASTTRACK       79  /* 80 쁟ie푘k (0 - 79) */
#define LASTSECTOR      15  /* 15 sektor쥄 na 쁟ie푖e (1 - 15) */
#define BYTESPERSECTOR 512  /* 512 bajt쥄 w sektorze */
#define driveToRead      0  /* Nap쯣 A = 0 */

char diskBuffer[ BYTESPERSECTOR * LASTSECTOR ];

void main( void )
{
unsigned char currentHead,
              currentTrack;

for( currentTrack = 0; 
        currentTrack <= LASTTRACK; 
           currentTrack++ )
   for( currentHead = 0; 
           currentHead <= LASTHEAD; 
              currentHead++ )
/* Odczytanie 15 sektor쥄 poczynaj쩭 od sektora nr 1 */
      readDisketteSectors( driveToRead,
                           LASTSECTOR,
                           currentHead,
                           currentTrack, 
                           1,
                           diskBuffer );
}

/*
   Odczytanie sektor쥄 z dyskietki.
   Zwraca 0 lub kod b닶du przerwania 13h.
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

regs.h.ah = 2;                 /* AH = 2 - czytaj sektory */
regs.h.al = sectorsToRead;     /* AL = liczba sektor쥄 do odczytania */
regs.x.bx = FP_OFF(bufferPtr); /* BX = przesuni쯢ie (offset) bufora */
regs.h.dh = head;              /* DH = g늨wica */
regs.h.dl = drive;             /* DL = numer nap쯣u */
regs.h.ch = track;             /* CH = 쁟ie푟a */
regs.h.cl = sector;            /* CL = numer sektora */
sregs.es  = FP_SEG(bufferPtr); /* ES = segment bufora */
int86x( 0x13, &regs, 
        &regs, &sregs );       /* Wywo늏nie funkcji BIOS */
return( regs.h.ah );           /* AH = 0 lub kod b닶du */
}
