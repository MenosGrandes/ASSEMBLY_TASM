/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-10.  Odczytanie dyskietki i wy˜wietlenie informacji o bˆ©dach
*/


/*
   READDSKT.C
   Odczytanie caˆej dyskietki (znanego typu) ˜cie¾ka po ˜cie¾ce 
   oraz informacja o bˆ©dach, kt¢re pozostaj¥ nawet po kilku
   powt¢rzeniach operacji.
*/

#include <dos.h>
#include <stdio.h>

/* Prototypy funkcji */
unsigned char readDisketteSectors( unsigned char, unsigned char, 
                                   unsigned char, unsigned char, 
                                   unsigned char, unsigned char far * );
unsigned char resetDrive( unsigned char );

#define LASTHEAD         1      /*   2 gˆowice (0 i 1) */
#define LASTTRACK       79      /*  80 ˜cie¾ek (0 - 79) */
#define LASTSECTOR      15      /*  15 sektor¢w na ˜cie¾ce (1 - 15) */
#define BYTESPERSECTOR 512      /* 512 bajt¢w w sektorze */
#define driveToRead      0      /* Nap©d A = 0 */
#define MAXRETRIES       3      /* Liczba powt¢rzeä nieudanych operacji */
#define NO_ERROR         0      /* OK - zwracane przez przerwanie 13h */

char diskBuffer[ BYTESPERSECTOR * LASTSECTOR ];

void main( void )
{
unsigned int currentHead, 
             currentTrack;
unsigned char errorCode;
int retryCount;

for( currentTrack = 0; 
        currentTrack <= LASTTRACK; 
           currentTrack++ )
{
   for( currentHead = 0; 
           currentHead <= LASTHEAD; 
              currentHead++ )
   {
      for( retryCount = 0; 
              retryCount < MAXRETRIES; 
                 retryCount++ )
      {
         errorCode = readDisketteSectors( driveToRead,
/* Przeczytaj 15 sektor¢w */              LASTSECTOR,
                                          currentHead,
                                          currentTrack, 
/* Zaczynaj¥c od sektora nr 1 */          1,
                                          diskBuffer );
         if ( errorCode == NO_ERROR )
            break;

         resetDrive( driveToRead );
         }

      if ( errorCode != NO_ERROR )
         printf( "Bˆ¥d odczytu - gˆowica %u cylinder %u\n\7",
                 currentHead, currentTrack );
      }
   }
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

regs.h.ah = 2;                 /* AH = 2 - czytaj sektory */
regs.h.al = sectorsToRead;     /* AL = liczba sektor¢w do odczytania */
regs.x.bx = FP_OFF(bufferPtr); /* BX = przesuni©cie (offset) bufora */
regs.h.dh = head;              /* DH = gˆowica */
regs.h.dl = drive;             /* DL = numer nap©du */
regs.h.ch = track;             /* CH = ˜cie¾ka */
regs.h.cl = sector;            /* CL = numer sektora */
sregs.es  = FP_SEG(bufferPtr); /* ES = segment bufora */
int86x( 0x13, &regs, 
        &regs, &sregs );       /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );           /* AH = 0 lub kod bˆ©du */
}

/* Reset nap©du i sterownika */
unsigned char resetDrive( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - reset nap©du i sterownika */
regs.h.dl = drive;            /* DL = numer nap©du */
int86( 0x13, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = 0 i Carry = 0 je¾eli OK */
}                             /* AH = kod bˆ©du i Carry = 1 przy bˆ©dzie */
