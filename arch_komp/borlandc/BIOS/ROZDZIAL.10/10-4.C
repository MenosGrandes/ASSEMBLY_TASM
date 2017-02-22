/* 
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Program 10-4.  Przegl¥danie pliku konfiguracyjnego
*/


/*
   Przeszukiwanie pliku konfiguracyjnego karty EISA
   i skopiowanie warto˜ci p¢l NAME= i MFR= do bufor¢w
   wskazanych przez program wywoˆuj¥cy.
*/

void searchConfigFile( FILE *stream, 
                       char *name, 
                       char *vendor )
{
#define BUFFERSIZE 128
char buffer[BUFFERSIZE];

char *pValue;

*name = 0;
*vendor = 0;

while( fgets(buffer, BUFFERSIZE, stream) )
{
   while( isspace(buffer[0]) )           /* Pomiä wiod¥ce biaˆe znaki */
      strcpy( buffer, &buffer[1] );

   pValue = strchr( buffer, '=' );       /* Znajd« w tek˜cie '=' */

   if ( pValue )                         /* Kontynuuj je¾eli znalazˆe˜ */
   {
      do { ++pValue; }                   /* Znajd« warto˜† */
         while (isspace(*pValue) );

      if ( *pValue == '"' )         /* Je¾eli warto˜† jest w cudzysˆowie */
      {
         ++pValue;                      /* Przejd« do nast©pnego znaku */
         *strchr( pValue, '"' ) = 0;    /* Ignoruj zamykaj¥cy cudzysˆ¢w */
         }

      if ( memicmp( buffer, "NAME=", 5) == 0 )
         strcpy( name, pValue );         /* Kopiuj warto˜† pola NAME=  */

      if ( memicmp( buffer, "MFR=", 4) == 0 )
         strcpy( vendor, pValue );       /* Kopiuj warto˜† pola MFR= */

      if ( (*name != 0) && (*vendor != 0) )
         break;                /* Je¾eli znalazˆe˜ obydwa pola to skoäcz */
      }
   }
}
