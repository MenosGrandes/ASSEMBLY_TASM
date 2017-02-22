/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-5.  Drukowanie pliku tekstowego
*/


/*
   PRFILE.C
   Drukowanie pliku tekstowego
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define boolean unsigned char

#define lpt1Base    (unsigned int  far *)0x00400008
#define lpt2Base    (unsigned int  far *)0x0040000a
#define lpt3Base    (unsigned int  far *)0x0040000c
#define lpt1Timeout (unsigned char far *)0x00400078
#define lpt2Timeout (unsigned char far *)0x00400079
#define lpt3Timeout (unsigned char far *)0x0040007A

#define timeoutValue 20            /* Maksymalnie 20 sekund oczekiwania */

/* Bity stanu drukarki */
#define READY    128
#define ACK       64
#define PAPEROUT  32
#define SELECTED  16
#define IOERROR    8
#define TIMEOUT    1

#define FORMFEED 0x0c   /* Znak wysuni©cia strony w ASCII (form feed) */

/* Prototypy funkcji */
unsigned char getPrinterStatus( unsigned int );
unsigned char printChar( int, unsigned char );
void diagnosePrinterProblem( unsigned char );
void printString( unsigned char *, unsigned char );


void main( int argc, char *argv[] )
{
unsigned char printer;
unsigned int far *ioPort;
unsigned char far *bdaTimeout;

FILE *inFile;
#define FILEBUFFERSIZE 128
char buffer[FILEBUFFERSIZE];

if ( argc != 3 )    /* Je¾eli nie ma dw¢ch parametr¢w to bˆ¥d! */
{
   printf( "Skˆadnia: %s <nazwa pliku> <LPT1,LPT2,LPT3> \7", argv[0] );
   exit( 1 );
   }

/* Pobranie numeru portu i wybranie pozycji z BDA */
printer = 0xff;
if ( stricmp( argv[2], "LPT1" ) == 0 )
{
   printer = 0;
   ioPort = lpt1Base;
   bdaTimeout = lpt1Timeout;
   }

if ( stricmp( argv[2], "LPT2" ) == 0 )
{
   printer = 1;
   ioPort = lpt2Base;
   bdaTimeout = lpt2Timeout;
   }

if ( stricmp( argv[2], "LPT3" ) == 0 )
{
   printer = 2;
   ioPort = lpt3Base;
   bdaTimeout = lpt3Timeout;
   }

/* Koniec je¾eli niepoprawna nazwa drukarki */
if ( printer == 0xff )
{
   printf( "Dozwolone nazwy port¢w LPT1, LPT2 lub LPT3!\7" );
   exit( 1 );
   }

/* Koniec je¾eli port nie jest zainstalowany */
if ( *ioPort == 0 )
{
   printf( "Przykro mi, ale port %s nie jest zainstalowany.\7", argv[2] );
   exit( 1 );
   }

/* Przypomnienie w razie potrzeby o wˆ¥czeniu drukarki ON LINE */
if ( (getPrinterStatus( printer ) & READY) == 0 )
{
   printf( "Prosz© ustaw drukark© ON LINE i naci˜nij Enter\7" );
   getch();
   }

inFile = fopen( argv[1], "rb" );
if ( inFile == NULL )
{
   printf( "Nie mo¾na otworzy† pliku %s!\7", argv[1] );
   exit( 1 );
   }

/* Ustawienie maksymalnego czasu oczekiwania dla drukarki */
*bdaTimeout = timeoutValue;

/* Drukowanie pliku */
while( fgets(buffer, FILEBUFFERSIZE, inFile) != 0 )
   printString( buffer, printer );

fclose( inFile );

/* Wysuni©cie ostatniej strony z drukarki */
printChar( printer, FORMFEED );
exit( 0 );
}

unsigned char getPrinterStatus( unsigned int whichPrinter )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - pobranie sˆowa stanu drukarki */
regs.x.dx = whichPrinter;     /* DX = drukarka (0, 1, 2) */
int86( 0x17, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = sˆowo stanu */
}

/* Drukuj znak */
unsigned char printChar( int whichPrinter, 
                         unsigned char chToPrint )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - drukuj znak */
regs.h.al = chToPrint;        /* AL = znak */
regs.x.dx = whichPrinter;     /* DX = drukarka (0, 1, 2) */
int86( 0x17, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

return( regs.h.ah );          /* AH = sˆowo stanu */
}

/* Wy˜wietlenie znaczenia bit¢w stanu drukarki */
void diagnosePrinterProblem( unsigned char status )
{
printf( "\n*** Drukarka " );

if ( (status & PAPEROUT) != 0 )
   printf( "nie ma papieru-" );

if ( (status & SELECTED) == 0 )
   printf( "jest wyˆ¥czona-" );

if ( (status & TIMEOUT) == 0 )
   printf( "nie jest gotowa do drukowania" );

printf( "\nNaci˜nij Enter i spr¢buj ponownie\7" );
getch();
}

/* Drukowanie zmiennej ˆaäcuchowej zakoäczonej zerem (ASCIIZ) */
void printString( unsigned char *stringP, 
                  unsigned char whichPrinter )
{
unsigned char status;

status = getPrinterStatus( whichPrinter );
while( *stringP != 0 )
{
   if ( (status & (TIMEOUT+IOERROR+PAPEROUT)) != 0 )
      diagnosePrinterProblem( status );

   status = printChar( whichPrinter, *stringP );
   ++stringP;
   }
}
