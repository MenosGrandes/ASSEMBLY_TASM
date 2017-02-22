/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-7.  Odebranie znaku z portu szeregowego
*/


/* Odczytanie bajtu z portu szeregowego i zwr¢cenie stanu linii */
unsigned char rxSerialChar( unsigned int port,
                            unsigned char *byteRead )
{
union REGS regs;

regs.h.ah = 2;                     /* AH = funkcja 2                   */
regs.x.dx = port;                  /* DX = numer portu (0,1,2,3)       */
int86( 0x14, &regs, &regs );       /* Wywoˆanie funkcji BIOS           */
*byteRead = regs.h.al;             /* AL = odczytany bajt              */
return( regs.h.ah );               /* AH = stan linii                  */
}
