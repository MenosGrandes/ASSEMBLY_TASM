/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Dodatek A.  Funkcje obslugi timera
*/


/* Przerwanie 15h / 83h - Ustaw/skasuj timer */

unsigned char doneFlag;

unsigned char startTimer( unsigned long timeValue )
{
union REGS regs;
struct SREGS sregs;

unsigned char *doneFlagP;

doneFlagP = &doneFlag;
*doneFlagP = 0;                        /* Jeszcze nie skoäczone... */

regs.x.ax = 0x8301;                    /* Skasuj timer, na wszelki wypadek */
int86( 0x15, &regs, &regs );           /* gdyby byˆ aktywny */

regs.x.ax = 0x8300;                    /* AH = 0x83, AL = 0 - ustaw timer */
regs.x.dx = timeValue;                 /* DX = mˆodsze 16 bit¢w stanu timera */
regs.x.cx = (timeValue >> 16);         /* CX = starsze 16 bit¢w stanu timera */
sregs.es = FP_SEG(doneFlagP);          /* ES = segment adresu znacznika zakoäczenia */
regs.x.bx = FP_OFF(doneFlagP);         /* BX = przesuni©cie (offset)
                                               adresu znacznika zakoäczenia */
int86x( 0x15, &regs, &regs, &sregs );  /* Wywoˆanie funkcji BIOS */

if ( regs.x.cflag )                    /* Carry = 1 = bˆ¥d */
   return( 0 );                        /* Zwr¢† 0 = bˆ¥d */

return( 1 );                           /* Zwr¢† 1 = OK */
}

/* Makrodefinicje sprawdzaj¥ce czy timer skoäczyˆ, czy jeszcze liczy */
#define TIMER_HAS_EXPIRED( flag ) ( (flag & 128) == 1 )
#define TIMER_IS_RUNNING( flag )  ( (flag & 128) == 0 )



/* Przerwanie 15h / 86h - Bezwarunkowe oczekiwanie */

unsigned char suspendProgram( unsigned long timeValue )
{
union REGS regs;

regs.h.ah = 0x86;             /* AH = 0x86 - zawie˜ prac© programu */
regs.x.dx = timeValue;        /* DX = mˆodsze 16 bit¢w czasu oczekiwania */
timeValue >>= 16;             /* Ustawienie na odpowiedniej pozycji */
regs.x.cx = timeValue;        /* CX = starsze 16 bit¢w czasu oczekiwania */
int86( 0x15, &regs, &regs );  /* Wywoˆanie funkcji BIOS */

if ( regs.x.cflag )           /* Carry = 1 - timer ju¾ pracuje */
   return( 0 );               /* Zwr¢† 0 = bˆ¥d */

return( 1 );                  /* Zwr¢† 1 = OK */
}



/* Przerwanie 1Ah / 00h - Odczytaj stan licznika systemowego */

unsigned char readTickCounter( unsigned long *out )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - Odczytaj stan licznika */
int86( 0x1a, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
*out = regs.x.cx;             /* CX = starsze 16 bit¢w */
*out <<= 16;                  /* Przesuni©cie na miejsce */
*out |= regs.x.dx;            /* DX = mˆodsze 16 bit¢w */
return( regs.h.al );          /* AL = Przekroczono p¢ˆnoc */
}



/* Przerwanie 1Ah / 01h - Ustaw stan licznika systemowego */

void setTickCounter( unsigned long tickValue )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - ustaw stan licznika */
regs.x.cx = tickValue >> 16;  /* CX = starsze 16 bit¢w */
regs.x.dx = tickValue;        /* DX = mˆodsze 16 bit¢w */
int86( 0x1a, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 1Ah / 02h - Odczytaj stan zegara (RTC) */

void readRTCTime( unsigned char *hour, 
                  unsigned char *min, 
                  unsigned char *sec )
{
union REGS regs;

regs.h.ah = 2;                    /* AH = 2 - odczytaj czas z RTC */
int86( 0x1a, &regs, &regs );      /* Wywoˆanie funkcji BIOS */

*hour = ( regs.h.ch >> 4 ) * 10;  /* CH = godzina */
*hour += ( regs.h.ch & 15 );      
*min = ( regs.h.cl >> 4 ) * 10;   /* CL = minuty */
*min += ( regs.h.cl & 15 );       
*sec = ( regs.h.dh >> 4 ) * 10;   /* DH = sekundy */
*sec += ( regs.h.dh & 15 );       
}



/* Przerwanie 1Ah / 03h - Ustaw czas (RTC) */

void setRTCTime( unsigned char hour, 
                 unsigned char min, 
                 unsigned char sec )
{
union REGS regs;

regs.h.ah  = 3;                    /* AH = 3 - ustawienie czasu RTC */
regs.h.ch  = ( hour / 10 ) << 4;   /* CH = godzina */
regs.h.ch |= ( hour % 10 );        
regs.h.cl  = ( min / 10 ) << 4;    /* CL = minuty */
regs.h.cl |= ( min % 10 );         
regs.h.dh  = ( sec / 10 ) << 4;    /* DH = sekundy */
regs.h.dh |= ( sec % 10 );         
regs.h.dl  = 0;                    /* DL = czas zimowy/letni */
int86( 0x1a, &regs, &regs );       /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 1Ah / 04h - Odczytaj dat© (RTC) */

void readRTCDate( unsigned int *year, 
                  unsigned char *month, 
                  unsigned char *day )
{
union REGS regs;

regs.h.ah = 4;                     /* AH = 4 - odczytaj dat© z RTC */
int86( 0x1a, &regs, &regs );       /* Wywoˆanie funkcji BIOS */

*year += ( regs.h.ch >> 4 ) * 10;  /* CH = stulecia */
*year += ( regs.h.ch & 15 );
*year *= 100;
*year += ( regs.h.cl >> 4 ) * 10;  /* CL = rok */
*year += ( regs.h.cl & 15 );

*month = ( regs.h.dh >> 4 ) * 10;  /* DH = miesi¥c */
*month += ( regs.h.dh & 15 );

*day = ( regs.h.dl >> 4 ) * 10;    /* DL = dzieä */
*day += ( regs.h.dl & 15 );
}



/* Przerwanie 1Ah / 05h - Ustawienie daty (RTC) */

void setRTCDate( unsigned int year, 
                 unsigned char month, 
                 unsigned char day )
{
union REGS regs;

regs.h.ah = 5;                             /* AH = 5 - ustawienie daty RTC */
regs.h.ch  = ( (year / 100) / 10 ) << 4;   /* CH = stulecia */
regs.h.ch |= ( (year / 100) % 10 );
regs.h.cl  = ( (year % 100) / 10 ) << 4;   /* CL = rok */
regs.h.cl |= ( year % 10 );
regs.h.dh  = ( month / 10 ) << 4;          /* DH = miesi¥c */
regs.h.dh |= ( month % 10 );
regs.h.dl  = ( day / 10 ) << 4;            /* DL = dzieä */
regs.h.dl |= ( day % 10 );
int86( 0x1a, &regs, &regs );               /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 1Ah / 06h - Ustawienie alarmu (RTC) */

void setRTCAlarm( unsigned char hour, 
                  unsigned char min, 
                  unsigned char sec )
{
union REGS regs;

regs.h.ah  = 6;                    /* AH = 6 - ustawienie alarmu (RTC) */
regs.h.ch  = ( hour / 10 ) << 4;   /* CH = godzina */
regs.h.ch |= ( hour % 10 );
regs.h.cl  = ( min / 10 ) << 4;    /* CL = minity */
regs.h.cl |= ( min % 10 );
regs.h.dh  = ( sec / 10 ) << 4;    /* DH = sekundy */
regs.h.dh |= ( sec % 10 );
int86( 0x1a, &regs, &regs );       /* Wywoˆanie funkcji BIOS */
}



/* Przerwanie 1Ah / 07h - Reset alarmu RTC */

void resetRTCAlarm( void )
{
union REGS regs;

regs.h.ah = 7;                /* AH = 7 - reset alarmu RTC */
int86( 0x1a, &regs, &regs );  /* Wywoˆanie funkcji BIOS */
}

