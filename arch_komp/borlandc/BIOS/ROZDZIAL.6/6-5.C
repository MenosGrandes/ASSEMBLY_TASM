/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-5.  Inicjalizacja portu szeregowego
*/


/*
   Inicjalizacja portu szeregowego
*/


/* Warto˜ci dla rejestru AL - sterowanie przerw¥ w transmisji */
#define BREAK_OFF   0
#define BREAK_ON    1

/* Warto˜ci dla rejestru BH - kontrola parzysto˜ci */
#define PARITY_NONE 0
#define PARITY_ODD  1
#define PARITY_EVEN 2
#define PARITY_BIT0 3
#define PARITY_BIT1 4

/* Warto˜ci dla rejestru BL - bity stopu na znak */
#define STOPBITS_1  0
#define STOPBITS_2  1

/* Warto˜ci dla rejestru CH - bity danych na znak */
#define DATABITS_5  0
#define DATABITS_6  1
#define DATABITS_7  2
#define DATABITS_8  3

/* Warto˜ci dla rejestru CL - szybko˜† transmisji */
#define BAUD_110    0
#define BAUD_150    1
#define BAUD_300    2
#define BAUD_600    3
#define BAUD_1200   4
#define BAUD_2400   5
#define BAUD_4800   6
#define BAUD_9600   7
#define BAUD_19200  8

void extInitSerialPort( unsigned char breakCode,
                        unsigned char parity,
                        unsigned char stopBits,
                        unsigned char dataBits,
                        unsigned char lineSpeed,
                        unsigned int whichPort,
                        unsigned char *lineStatus,
                        unsigned char *modemStatus )
{
union REGS regs;

regs.h.ah = 4;                  /* AH = funkcja 4 - inicjalizacja */
regs.h.al = breakCode;          /* AL = sterowanie przerw¥        */
regs.h.bh = parity;             /* BH = kontrola parzysto˜ci      */
regs.h.bl = stopBits;           /* BL = liczba bit¢w stopu        */
regs.h.ch = dataBits;           /* CH = liczba bit¢w danych       */
regs.h.cl = lineSpeed;          /* CL = szybko˜† transmisji       */
regs.x.dx = whichPort;          /* DX = port 0,1,2 lub 3          */
int86( 0x14, &regs, &regs );    /* Wywoˆanie funkcji BIOS         */

*lineStatus = regs.h.ah;        /* Zwraca w AH stan linii         */
*modemStatus = regs.h.al;       /* Zwraca stan modemu w AL        */
}
