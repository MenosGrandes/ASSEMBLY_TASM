/*
   BIOS dla programujących w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-6.  Włączenie/wyłączenie wyświetlania kursora 
*/


/* Włączenie/wyłączenie wyświetlania kursora */

void cursorOnOff( void )
{
unsigned char topLine,
              bottomLine;

getCursorSize( &topLine, &bottomLine );
setCursorSize( bottomLine, topLine );
}
