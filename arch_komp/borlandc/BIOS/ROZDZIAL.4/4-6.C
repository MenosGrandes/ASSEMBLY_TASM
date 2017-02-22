/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-6.  Wˆ¥czenie/wyˆ¥czenie wy˜wietlania kursora 
*/


/* Wˆ¥czenie/wyˆ¥czenie wy˜wietlania kursora */

void cursorOnOff( void )
{
unsigned char topLine,
              bottomLine;

getCursorSize( &topLine, &bottomLine );
setCursorSize( bottomLine, topLine );
}
