/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-8.  Wy˜wietlenie informacji o konfiguracji
*/


/* *********************************************************
   DEVICES.C
   U¾ycie przerwaä 11h, 12h i 15h (funkcja 88h) do okre˜lenia
   konfiguracji komputera.
   Uruchomienie:
      DEVICES<Enter>
************************************************************ */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

/*
   Makrodefinicja zwracaj¥ca wska«nik do tekstu "TAK"
   je¾eli bit ma warto˜† 1 lub wska«nik do tekstu "NIE"
   je¾eli bit ma warto˜† 0.
*/
#define YesOrNo( value, bit ) (value & bit) ? yes : no

/* Definicje danych w CMOS RAM */
#define CMOS_INDEX 0x70
#define CMOS_DATA  0x71
#define EXTENDED_MEM_LSB 24
#define EXTENDED_MEM_MSB 23

/* Pola sˆowa zainstalowanego wyposa¾enia
      (Installed Equipment Word)     15..............0 */
#define IEW_PRINTERS         3<<14 /* xx.............. */
#define IEW_MODEM            1<<13 /* ..x............. */
#define IEW_r1               1<<12 /* ...x............ */
#define IEW_SERIAL_PORTS     7<< 9 /* ....xxx......... */
#define IEW_r2               1<< 8 /* .......x........ */
#define IEW_DISKETTE_DRIVES  3<< 6 /* ........xx...... */
#define IEW_VIDEO_MODE       3<< 4 /* ..........xx.... */
#define IEW_r3               1<< 3 /* ............x... */
#define IEW_MOUSE            1<< 2 /* .............x.. */
#define IEW_COPROCESSOR      1<< 1 /* ..............x. */
#define IEW_DSKT_DRIVES_PRESENT  1 /* ...............x */

#define IEW_40x25COLOR           1 /* ..........01.... */
#define IEW_80x25COLOR           2 /* ..........10.... */
#define IEW_80x25MONO            3 /* ..........11.... */

/* Liczba bit¢w do przesuni©cia, aby uzyska† z p¢l
   warto˜ci liczbowe */
#define IEW_PRINTER_SHIFT                 14
#define IEW_SERIAL_PORT_SHIFT              9
#define IEW_DISKETTE_SHIFT                 6
#define IEW_VIDEO_SHIFT                    4

/* Prototypy funkcji */
unsigned int getBaseMemorySize          ( void );
unsigned int getFreeExtendedMemory      ( void );
unsigned int getInstalledExtendedMemory ( void );
unsigned int getInstalledEquipment      ( void );

void main( void )
{
static char yes[] = "TAK",
            no[]  = "NIE";

unsigned int baseMemory,
             freeExtendedMemory,
             installedExtendedMemory,
             iew,
             numberOfPrinters,
             numberOfDisketteDrives,
             numberOfSerialPorts,
             initialVideoMode;

/* Pobranie rozmiaru pami©ci podstawowej */
baseMemory = getBaseMemorySize();

/* Pobranie rozmiaru dost©pnej pami©ci rozszerzonej */
freeExtendedMemory = getFreeExtendedMemory();

/* Pobranie z CMOS RAM rozmiaru zainstalowanej pami©ci rozszerzonej */
installedExtendedMemory = getInstalledExtendedMemory();

/* Pobranie informacji o zainstalowanym wyposa¾eniu */
iew = getInstalledEquipment();

/* Przeksztaˆcenie odpowiednich p¢l w warto˜ci liczbowe */
numberOfPrinters = 
   ((iew & IEW_PRINTERS) >> IEW_PRINTER_SHIFT);

numberOfSerialPorts = 
   ((iew & IEW_SERIAL_PORTS) >> IEW_SERIAL_PORT_SHIFT);

if ( (iew & IEW_DSKT_DRIVES_PRESENT) )
   numberOfDisketteDrives = 1 + 
      ((iew & IEW_DISKETTE_DRIVES) >> IEW_DISKETTE_SHIFT);
else
   numberOfDisketteDrives = 0;

initialVideoMode = 
   ((iew & IEW_VIDEO_MODE) >> IEW_VIDEO_SHIFT);

/* Wy˜wietlenie informacji */
printf( "\n   %uKB zainstalowanej pami©ci podstawowej", baseMemory );
printf( "\n   %uKB pami©ci rozszerzonej (extended)", installedExtendedMemory );
printf( "\n   %uKB wolnej pami©ci rozszerzonej", freeExtendedMemory );
printf( "\n---------------------------------------------");
printf( "\n   %u drukarka (i)", numberOfPrinters );
printf( "\n   %u port(y) szeregowy(e)", numberOfSerialPorts );
printf( "\n   %u stacja(e) dyskietek", numberOfDisketteDrives );
printf( "\n---------------------------------------------");
printf( "\n   Wbudowany modem ?   %s", YesOrNo(iew,IEW_MODEM) );
printf( "\n   Mysz            ?   %s", YesOrNo(iew,IEW_MOUSE) );
printf( "\n   Koprocesor      ?   %s", YesOrNo(iew,IEW_COPROCESSOR) );
printf( "\n---------------------------------------------");
printf( "\n   Pocz¥tkowy tryb pracy karty graficznej: " );

switch ( initialVideoMode )
{
   case IEW_40x25COLOR:    
      printf( "40 x 25 kolorowy" );
      break;
   case IEW_80x25COLOR:    
      printf( "80 x 25 kolorowy" );
      break;
   case IEW_80x25MONO:     
      printf( "80 x 25 monochromatyczny" );
      break;
   default:                
      printf( "nieznany" );
      break;
   }
}

/* Podaje wielko˜† pami©ci podstawowej w KB */
unsigned int getBaseMemorySize( void )
{
union REGS regs;

int86( 0x12, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );          /* AX = KB */
}

/* Podaje w KB wielko˜† dost©pnej pami©ci rozszerzonej */
unsigned int getFreeExtendedMemory( void )
{
union REGS regs;

regs.h.ah = 0x88;             /* AH = 88h - podaj wielko˜† pami©ci
                                            rozszerzonej */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return ( regs.x.ax );         /* AX = KB */
}

/* Podaje wielko˜† zainstalowanej pami©ci rozszerzonej */
unsigned int getInstalledExtendedMemory( void )
{
unsigned int size;

outp( CMOS_INDEX, EXTENDED_MEM_LSB );
size = inp( CMOS_DATA );
size *= 256;

outp( CMOS_INDEX, EXTENDED_MEM_MSB );
size += inp( CMOS_DATA );

return( size );
}

/* Podaje informacj© o zainstalowanym wyposa¾eniu */
unsigned int getInstalledEquipment( void )
{
union REGS regs;

int86( 0x11, &regs, &regs);  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );         /* AX = informacja o wyposa¾eniu */
}
