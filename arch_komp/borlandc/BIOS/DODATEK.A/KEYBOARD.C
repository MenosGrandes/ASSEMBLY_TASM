/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Dodatek A.  Funkcje obsˆugi klawiatury
*/


/* Przerwanie 16h / 00h - Odczytaj znak z klawiatury */

unsigned int getKbdChar( void )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - odczytaj znak z klawiatury */
int86( 0x16, &regs, &regs );  /* Wywoˆaj funkcj© BIOS */
return( regs.x.ax );          /* AH = kod klawisza, AL = kod znaku */
}


/* Przerwanie 16h / 01h - Sprawd« czy s¥ gotowe dane */

unsigned int testKbdCharReady( void )
{
unsigned int  returnValue;
_asm  mov AH, 1            /* AH = 1 - sprawd« czy s¥ dane */
_asm  int 16h              /* Wywoˆaj funkcj© BIOS */
_asm  jnz endFunc1         /* Zero = 0 - znak w buforze */
_asm  xor AX,AX            /* Brak znaku - zwr¢† 0 */
endFunc1:
_asm  mov returnValue, AX  /* AH = kod klawisza, AL = kod znaku */
return( returnValue );     /* Zwr¢† 0 lub kod klawisza/znaku */
}



/* Przerwanie 16h / 02h - Czytaj stan klawiatury */

unsigned char readShiftStatus( void )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - czytaj stan klawiatury */
int86( 0x16, &regs, &regs );  /* Wywoˆaj funkcj© BIOS */
return( regs.h.al );          /* AL = stan klawitury */
}



/* Przerwanie 16h / 03h - Ustaw op¢«nienie/szybko˜† powtarzania */

void setRepeatRate( unsigned char delay, 
                    unsigned char repeatRate )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - ustaw op¢«nienie
                                      i szybko˜† powtarzania */
regs.h.al = 5;                /* AL musi by† 5 */
regs.h.bh = delay;            /* BH = warto˜† op¢«nienia */
regs.h.bl = repeatRate;       /* BL = szybko˜† powtarzania */
int86( 0x16, &regs, &regs );  /* Wywoˆaj funkcj© BIOS */
}



/* Przerwanie 16h / 05h - Zapisz znak do bufora */

unsigned char writeKbdData( unsigned char scanCode, 
                            unsigned char charCode )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - zapisz do bufora */
regs.h.ch = scanCode;         /* CH = kod klawisza */
regs.h.cl = charCode;         /* CL = znak ASCII */
int86( 0x16, &regs, &regs );  /* Wywoˆaj funkcj© BIOS */
return( regs.h.al );          /* AL = 0 (OK) lub 1 (bufor peˆen) */
}



/* Przerwanie 16h / 10h - Odczytaj znak z klawiatury rozszerzonej */

unsigned int getEnhancedKbdChar( void )
{
union REGS regs;

regs.h.ah = 0x10;             /* AH = 0x10 - odczytaj znak */
int86( 0x16, &regs, &regs );  /* Wywoˆaj funkcj© BIOS */
return( regs.x.ax );          /* AH = kod klawisza, AL = kod znaku */
}



/* Przerwanie 16h / 11h - Sprawd« czy s¥ gotowe dane z klawiatury rozszerzonej*/

unsigned int testEnhancedKbdCharReady( void )
{
unsigned int returnValue;
_asm  mov AH, 11h          /* AH = 0x11 - sprawdzenie czy s¥ gotowe dane */
_asm  int 16h              /* Wywoˆaj funkcj© BIOS */
_asm  jnz endFunc11        /* Zero = 0 znak w buforze */
_asm  xor AX,AX            /* bufor pusty - zwr¢† 0 */
endFunc11:
_asm  mov returnValue, AX  /* AH = kod klawisza, AL = kod znaku */
return( returnValue );     /* Zwr¢† 0 lub kod klawisza/znaku */
}



/* Przerwanie 16h / 12h - Odczataj stan klawiatury rozszerzonej */

unsigned int readExtendedShiftStatus( void )
{
union REGS regs;

regs.h.ah = 0x12;             /* AH = 0x12 - odczytaj stan klawiatury */
int86( 0x16, &regs, &regs );  /* Wywoˆaj funkcj© BIOS */
return( regs.x.ax );          /* AH = dodatkowy bajt stanu klawiatury */
                              /* AL = standardowy bajt stanu klawiatury */
}

