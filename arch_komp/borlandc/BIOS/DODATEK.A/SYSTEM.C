/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Dodatek A.  Funkcje obsˆugi myszy i joysticka
*/


/* Przerwanie 15h / 84h - Czytaj stan joysticka */

unsigned char readJsSwitches( void )
{
union REGS regs;

regs.h.ah = 0x84;             /* AH = 0x84 - czytaj stan przycisk¢w joysticka */
regs.x.dx = 0;                /* DX = 0 */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.al );          /* AH = ustawienia przycisk¢w */
}

void readJsPots( unsigned *potAx,
                 unsigned *potAy,
                 unsigned *potBx,
                 unsigned *potBy )
{
union REGS regs;

regs.h.ah = 0x84;             /* AH = 0x84 - pozycja joysticka */
regs.x.dx = 0;                /* DX = 0 */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*potAx = regs.x.ax;           /* AX = warto˜† pot. A X */
*potAy = regs.x.bx;           /* BX = warto˜† pot. A Y */
*potBx = regs.x.cx;           /* CX = warto˜† pot. B X */
*potBy = regs.x.dx;           /* DX = warto˜† pot. B Y */
}



/* Przerwanie 15h / C200h - Wˆ¥cz / wyˆ¥cz mysz PDI */

#define mouseOn 1
#define mouseOff 0

unsigned char turnMouseOnOff( int flag )
{
union REGS regs;

regs.x.ax = 0xC200;           /* AX = 0xC200 - wˆ¥cz / wyˆ¥cz mysz PDI */
regs.h.bh = flag;             /* BH = 0 (wyˆ¥cz) lub 1 (wˆ¥cz) */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 15h / C201h - Reset myszy PDI */

unsigned char resetMouse( void )
{
union REGS regs;

regs.x.ax = 0xC201;           /* AX = 0xC201 - reset myszy */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 15h / C202h - Ustawienie cz©sto˜ci pr¢bkowania myszy PDI */

unsigned char setMouseSampleRate( unsigned char newRate )
{
union REGS regs;

regs.x.ax = 0xC202;           /* AX = 0xC202 - ustawienie cz©sto˜ci
                                               pr¢bkowania myszy PDI */
regs.h.bh = newRate;          /* BH = nowa g©sto˜† */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 15h / C203h - Ustaw rozdzielczo˜† myszy PDI */

unsigned char setMouseResolution( unsigned char newResolution )
{
union REGS regs;

regs.x.ax = 0xC203;           /* AX = 0xC203 - ustaw rozdzielczo˜† myszy PDI */
regs.h.bh = newResolution;    /* BH = nowa warto˜† rozdzielczo˜ci */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 15h / C204h - Odczytaj typ myszy PDI */

unsigned char readMouseIdByte( unsigned char *idByte )
{
union REGS regs;

regs.x.ax = 0xC204;           /* AX = 0xC204 - odczytaj typ myszy PDI */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*idByte = regs.h.bh;          /* BH = identyfikator */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 15h / C205h - Inicjalizacja myszy PDI */

unsigned char initPDI( unsigned char packetSize )
{
union REGS regs;

regs.x.ax = 0xC205;           /* AX = 0xC205 - inicjalizacja myszy PDI */
regs.h.bh = packetSize;       /* BH = nowy rozmiar pakietu */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 15h / C206h - Pobranie/ustawienie wsp¢ˆczynnika skalowania myszy PDI */

unsigned char getMouseStatus( unsigned char command,   /* 0, 1, or 2 */
                              unsigned char *buttons,
                              unsigned char *resolution,
                              unsigned char *sampleRate )
{
union REGS regs;

regs.x.ax = 0xC206;           /* AX = 0xC206 - wsp. skalowania */
regs.h.bh = command;          /* BH = bajt polecenia */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

*buttons = regs.h.bl;         /* BL = bie¾¥ca warto˜† stanu */
*resolution = regs.h.cl;      /* CL = warto˜† rozdzielczo˜ci */
*sampleRate = regs.h.dl;      /* CL = wsp¢ˆczynnik pr¢bkowania */

return( regs.h.ah );          /* AH = stan */
}



/* Przerwanie 15h / C207h - Ustawienie adresu sterownika myszy PDI */

unsigned char installMouseDriver( void far *func )
{
union REGS regs;
struct SREGS sregs;

regs.x.ax = 0xC207;                    /* AX = 0xC207 - instaluj sterownik */
regs.x.bx = FP_OFF(func);              /* BX = przesuni©cie (offset)
                                                     adresu sterownika */
sregs.es = FP_SEG(func);               /* ES = segment adresu sterownika*/
int86x( 0x15, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );                   /* AH = stan */
}

