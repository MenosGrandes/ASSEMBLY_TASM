/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Dodatek A.  Funkcje obsˆugi portu szeregowego
*/


/* Przerwanie 14h / 00h - Inicjalizacja portu szeregowego */

unsigned int initSerialPort( unsigned int port, 
                             unsigned char setupVal )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - inicjalizacja portu */
regs.h.al = setupVal;         /* AL = charakterystyka portu */
regs.x.dx = port;             /* DX = numer portu */
int86( 0x14, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );          /* AH = stan linii, AL = stan modemu */
}



/* Przerwanie 14h / 01h - Wy˜lij znak przez port szeregowy */

unsigned char comTx( unsigned int port, 
                     unsigned char charToXmit )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - wy˜lij znak */
regs.h.al = charToXmit;       /* AL = znak do wysˆania */
regs.x.dx = port;             /* DX = numer portu */
int86( 0x14, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.h.ah );          /* AH = stan linii */
}



/* Przerwanie 14h / 02h - Odbierz znak z portu szeregowego */

unsigned int comRx( unsigned int port )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - odbierz znak */
regs.x.dx = port;             /* DX = numer portu */
int86( 0x14, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );          /* AH = stan linii, AL = znak */
}



/* Przerwanie 14h / 03h - Odczytaj stan portu szeregowego */

unsigned int comReadStatus( unsigned int port )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - odczytaj stan portu szeregowego */
regs.x.dx = port;             /* DX = numer portu */
int86( 0x14, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );          /* AH = stan linii, AL = stan modemu */
}



/* Przerwanie 14h / 04h - Rozszerzona inicjalizacja portu szeregowego */

unsigned int comExtInit( unsigned int port, 
                         unsigned char breakCode, 
                         unsigned char parity, 
                         unsigned char stopBits, 
                         unsigned char wordLgt, 
                         unsigned char baudRate )
{
union REGS regs;

regs.h.ah = 4;                /* AH = 4 - rozszerzona inicjalizacja */
regs.h.al = breakCode;        /* AL = niwy kod przerwy */
regs.h.bh = parity;           /* BH = nowe ustawienie parzysto˜ci */
regs.h.bl = stopBits;         /* BL = liczby bit¢w stopu */
regs.h.ch = wordLgt;          /* CH = nowa dˆugo˜† sˆowa */
regs.h.cl = baudRate;         /* CL = nowa szybko˜† transmisji */
regs.x.dx = port;             /* DX = numer portu */
int86( 0x14, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
return( regs.x.ax );          /* AH = stan linii, AL = stan modemu */
}



/* Przerwanie 14h / 05h - Rozszerzone serowanie modemem */

unsigned int comModemCtl( unsigned int port, 
                          unsigned char rwCode, 
                          unsigned char *modemCtlReg )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - rozszerzone sterowanie modemem */
regs.h.al = rwCode;           /* AL = kod odczytu/zapisu */
regs.h.bl = *modemCtlReg;     /* BL = nowa warto˜† sterowania portu */
regs.x.dx = port;             /* DX = numer portu */
int86( 0x14, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*modemCtlReg = regs.h.bl;     /* BL = warto˜† sterowania modemem */
return( regs.x.ax );          /* AH = stan linii, AL = stan modemu */
}

