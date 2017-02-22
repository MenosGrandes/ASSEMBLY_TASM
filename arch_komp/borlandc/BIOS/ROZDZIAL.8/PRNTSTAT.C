/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-3.  Wy˜wietlenie stanu drukarki
*/


/*
   PRNTSTAT.C
   Wy˜wietlenie stanu dla ka¾dej drukarki
   podˆ¥czonej do portu r¢wnolegˆego
*/

#include <dos.h>
#include <stdio.h>

#define boolean unsigned char

#define bitOnOff( value, bit ) (value & bit) ? '1' : '0'

#define lpt1Base (unsigned int far *)0x00400008
#define lpt2Base (unsigned int far *)0x0040000a
#define lpt3Base (unsigned int far *)0x0040000c

/* Bity stanu drukarki */
#define READY    128
#define ACK       64
#define PAPEROUT  32
#define SELECTED  16
#define IOERROR    8
#define TIMEOUT    1

/* Prototypy funkcji */
boolean checkPrinter( char *, unsigned int far *);
unsigned char getPrinterStatus( unsigned int );
void showPrinterStatus( unsigned int );

void main( void )
{
if ( checkPrinter( "LPT1", lpt1Base ) )
   showPrinterStatus( 0 );

if ( checkPrinter( "LPT2", lpt2Base ) )
   showPrinterStatus( 1 );

if ( checkPrinter( "LPT3", lpt3Base ) )
   showPrinterStatus( 2 );
}


/* Zwraca 1 = port drukarki jest zainstalowany 
          0 = port drukarki nie jest zainstalowany
*/
boolean checkPrinter( char *printerName,
                      unsigned int far *ioAddress )
{
if( *ioAddress == 0 )
   printf( "\n%s : nie jest zainstalowany.", printerName );
else
   printf( "\n%s : jest zainstalowany pod adresem %.4X.",
           printerName,*ioAddress );

return( ( *ioAddress != 0 ) );
}

/*
   Odczytanie stanu portu drukarki
*/


unsigned char getPrinterStatus( unsigned int whichPrinter )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - pobranie sˆowa stanu drukarki */
regs.x.dx = whichPrinter;     /* DX = drukarka (0, 1, 2) */
int86( 0x17, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = sˆowo stanu */
}


/* Dekodowanie i wy˜wietlenie informacji o stanie drukarki */
void showPrinterStatus( unsigned int printer )
{
unsigned char status;

status = getPrinterStatus( printer );

printf( "\n\tBit 7 = %c = Drukarka gotowa",      
        bitOnOff(status, READY) );

printf( "\n\tBit 6 = %c = Potwierdzenie",        
        bitOnOff(status, ACK) );

printf( "\n\tBit 5 = %c = Brak papieru",       
        bitOnOff(status, PAPEROUT) );

printf( "\n\tBit 4 = %c = Drukarka wybrana",   
        bitOnOff(status, SELECTED) );

printf( "\n\tBit 3 = %c = Bˆ¥d wej˜cia/wyj˜cia", 
        bitOnOff(status, IOERROR) );

printf( "\n\tBit 2 = %c = Zarezerwowany",           
        bitOnOff(status, 4) );

printf( "\n\tBit 1 = %c = Zarezerwowany",           
        bitOnOff(status, 2) );

printf( "\n\tBit 0 = %c = Przekroczony dop. czas oczekiwania",  
        bitOnOff(status, TIMEOUT) );
}
