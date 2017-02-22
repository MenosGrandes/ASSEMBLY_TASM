/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-3.  Odczytanie i ustawienie maksymalnego czasu
                 oczekiwania na reakcj© portu szeregowego
*/ 


/*
   readSerialPortTimeout() odczytuje warto˜ci czasu oczekiwania z BDA
   setSerialPortTimeout() ustawia w BDA warto˜ci czasu oczekiwania
*/

#define com1Timeout (unsigned char far *)0x0040007c
#define com2Timeout (unsigned char far *)0x0040007d
#define com3Timeout (unsigned char far *)0x0040007e
#define com4Timeout (unsigned char far *)0x0040007f

/* Zwraca warto˜† czasu oczekiwania dla wskazanego portu */
unsigned char readSerialPortTimeout( unsigned char whichPort )
{
whichPort &= 3;
return( *(com1Timeout + whichPort) );
}

/* Ustawia czas oczekiwania dla wskazanego portu */
void setSerialPortTimeout( unsigned char whichPort,
                           unsigned char timeoutValue )
{
whichPort &= 3;
*(com1Timeout + whichPort) = timeoutValue;
}
