/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Dodatek A.  Funkcje testuj¥ce ˜rodowisko pracy
*/


/* Przerwanie 11h - Czytaj informacj© o zamontowanym wyposa¾eniu */

unsigned int getInstalledEquipment( void )
{
union REGS regs;

int86( 0x11, &regs, &regs);  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );         /* AX = informacja o zainstalowanym wyposa¾eniu */
}



/* Przerwanie 12h - Podaj wielko˜† pami©ci podstawowej */

unsigned int getBaseMemorySize( void )
{
union REGS regs;

int86( 0x12, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );          /* AX = ilo˜† KB pami©ci podstawowej */
}



/* Przerwanie 15h / 88h - Podaj wielko˜† pami©ci Extended */

unsigned int getExtendedMemorySize( void )
{
union REGS regs;

regs.h.ah = 0x88;             /* AH = 0x88 - czytaj rozmiar pami©ci extended */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return ( regs.x.ax );         /* AX = dost©pna pami©† extended w KB */
}



/* Przerwanie 15h / C0h - Podaj informacj© o ˜rodowisku pracy */

struct cfgParamTbl
{
   unsigned int  tableLength;
   unsigned char systemModelCode;
   unsigned char systemTypeCode;
   unsigned char BIOSRevisionNumber;
   unsigned char cfgInfoBits;
   };

struct cfgParamTbl far *getCfgParmTblAddr( void )
{
union REGS regs;
struct SREGS sregs;

struct cfgParamTbl far *pCh;

regs.h.ah = 0xc0;                      /* AH = 0xC0 - podaj info o ˜rodowisku */
int86x( 0x15, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */

(unsigned long)pCh = sregs.es;         /* ES = segment adresu tablicy */
(unsigned long)pCh <<= 16;             /* przesuni©cie na miejsce */
(unsigned long)pCh |= regs.x.bx;       /* BX = przesuni©cie (offset) adresu tablicy */

return( pCh );                         /* zwracamy adres do tablicy */
}



/* Przerwanie 15h / C1h - Podaj segment rozszerzonego BDA 
   (Extended BIOS Data Area - EBDA) */

unsigned char getEBDASegment( unsigned char *segment )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 0xC1;                      /* AH = 0xC1 - czytaj segment EBDA */
int86x( 0x15, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */

if ( regs.x.cflag )                    /* Je¾eli bˆ¥d */
   return( regs.h.ah );                /* AH = stan */

*segment = sregs.es;                   /* ES = segment adresu EBDA */
return( 0 );
}



/* Przerwanie 15h / D800h - Czytaj informacje o gnie«dzie EISA */

struct EISASlotInfo
{
   unsigned int boardId1;
   unsigned int boardId2;
   unsigned char boardInfo;
   unsigned char cfgUtilMajorVer;
   unsigned char cfgUtilMinorVer;
   unsigned int  checkSum;
   unsigned char functionCount;
   unsigned char functionSummary;
   };

unsigned char readEISASlotInfo( unsigned char slot,
                                struct EISASlotInfo far *out )
{
union REGS regs;
struct SREGS sregs;

regs.x.ax = 0xd800;                    /* AX = 0xD800 - czytaj info gniazda */
regs.h.cl = slot;                      /* CL = numer gniazda */
int86x( 0x15, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */

out->boardId1 = regs.x.di;             /* DI = 1 cz©˜† skompresowanego ID */
out->boardId2 = regs.x.si;             /* SI = 2 cz©˜† skompresowanego ID */
out->boardInfo = regs.h.al;            /* AL = bajt informacji o karcie */
out->cfgUtilMajorVer = regs.h.bh;      /* BH = gˆ¢wny numer wersji
                                               prog. konfiguracyjnego */
out->cfgUtilMinorVer = regs.h.bl;      /* BH = dodatkowy numer wersji
                                               prog. konfiguracyjnego */
out->checkSum = regs.x.cx;             /* CX = suma kontrolna */
out->functionCount = regs.h.dh;        /* DH = liczba funkcji */
out->functionSummary = regs.h.dl;      /* DL = bajt sumy funkcji */

return( regs.h.ah );                   /* AH = stan */
}

