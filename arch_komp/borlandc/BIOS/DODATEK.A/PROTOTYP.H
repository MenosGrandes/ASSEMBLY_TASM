/*
   BIOS dla programuj¥cych w C i C++
   Copyright (c) 1992 - Billy P. Taylor

   Dodatek A.  Prototypy funkcji
*/


/* Przerwanie 05h - Obsˆuga klawisza Print Screen */
unsigned char printScreen( void );


/* Przerwanie 10h / 00h - Ustaw tryb karty graficznej */
void setVideoMode( unsigned char newMode );


/* Przerwanie 10h / 01h - Ustaw rozmiary kursora */
void setCursorType( unsigned char topScanLine, 
                    unsigned char bottomScanLine );


/* Przerwanie 10h / 02h - Ustaw pozycj© kursora */
void setCursorPosition( unsigned char videoPage, 
                        unsigned char row, 
                        unsigned char column );


/* Przerwanie 10h / 03h - Odczytanie pozycji kursora */
void getCursorPosition( unsigned char videoPage,
                        unsigned char *row,
                        unsigned char *column,
                        unsigned char *topScanLine,
                        unsigned char *bottomScanLine );


/* Przerwanie 10h / 04h - Odczytaj pozycj© p¢ra ˜wietlnego */
unsigned char readLightPen( unsigned char *row,
                            unsigned char *column,
                            unsigned int *rasterLine,
                            unsigned int *pixelColumn );


/* Przerwanie 10h / 05h - Wybierz aktywn¥ stron© graficzn¥ */
void setVideoPage( unsigned char newPage );


/* Przerwanie 10h / 06h - Przewiä ekran w g¢r© */
void scrollUp( unsigned char lines, 
               unsigned char attribute, 
               unsigned char topRow, 
               unsigned char leftColumn, 
               unsigned char bottomRow, 
               unsigned char rightColumn );


/* Przerwanie 10h / 07h - Przewiä stron© w d¢ˆ */
void scrollDown( unsigned char lines, 
                 unsigned char attribute, 
                 unsigned char topRow, 
                 unsigned char leftColumn, 
                 unsigned char bottomRow, 
                 unsigned char rightColumn );


/* Przerwanie 10h / 08h - Odczytaj znak i atrybut */
void readCell( unsigned char videoPage, 
               unsigned char *pChar, 
               unsigned char *pAttribute );


/* Przerwanie 10h / 09h - Napisanie znaku i atrybutu */
void writeCell( unsigned char videoPage, 
                unsigned char character, 
                unsigned char attribute, 
                unsigned int times );


/* Przerwanie 10h / 0Ah - Napisz tylko znak */
void writeChar( unsigned char videoPage, 
                unsigned char ch, 
                unsigned int times );


/* Przerwanie 10h / 0Bh - Ustawienie palety barw */
void setPalette( unsigned char paletteEntry,
                 unsigned char color );


/* Przerwanie 10h / 0Ch - Rysuj piksel */
void writePixel( unsigned char color, 
                 unsigned char videoPage, 
                 unsigned int row, 
                 unsigned int column );


/* Przerwanie 10h / 0Dh - Odczytaj piksel */
unsigned char readPixel( unsigned char videoPage, 
                         unsigned int row, 
                         unsigned int column );


/* Przerwanie 10h / 0Eh - Napisz znak, przesuä kursor */
void writeTTYChar( unsigned char videoPage, 
                   unsigned char ch, 
                   unsigned char attribute );


/* Przerwanie 10h / 0Fh - Odczytaj bie¾¥cy stan karty graficznej */
void readVideoState( unsigned char *columns, 
                     unsigned char *currentMode, 
                     unsigned char *currentPage );


/* Przerwanie 10h / 1A00h - Odczytaj kod typu karty */
void getVideoType( unsigned char *primary, 
                   unsigned char *secondary );


/* Przerwanie 11h - Czytaj informacj© o zamontowanym wyposa¾eniu */
unsigned int getInstalledEquipment( void );


/* Przerwanie 12h - Podaj wielko˜† pami©ci podstawowej */
unsigned int getBaseMemorySize( void );


/* Przerwanie 13h / 00h - Reset nap©du (dyskietek lub dysku twardego) */
unsigned char resetDrive( unsigned char drive );


/* Przerwanie 13h / 01h - Podaj stan zakoäczenia ostatniej operacji wa/wy */
unsigned char readDriveStatus( unsigned char drive );


/* Przerwanie 13h / 02h - Czytaj sektory (dyskietka lub dysk twardy) */
unsigned char readSectors( unsigned char drive, 
                           unsigned char todo, 
                           unsigned char head, 
                           unsigned int cylinder, 
                           unsigned char sector, 
                           unsigned char far *bufferPtr );


/* Przerwanie 13h / 03h - Zapisz sektory (dyskietka lub dysk twardy) */
unsigned char writeSectors( unsigned char drive, 
                            unsigned char todo, 
                            unsigned char head, 
                            unsigned int cylinder, 
                            unsigned char sector, 
                            unsigned char far *bufferPtr );


/* Przerwanie 13h / 04h - Weryfikuj sektory (dyskietka lub dysk twardy) */
unsigned char verifySectors( unsigned char drive, 
                             unsigned char numberOfSectors, 
                             unsigned char head, 
                             unsigned int cylinder, 
                             unsigned char sector,
                             unsigned char far *bufferPtr );


/* Przerwanie 13h / 05h - Formatuj ˜cie¾k© (dysk twardy) */
unsigned char formatTrack( unsigned char drive, 
                           unsigned char head, 
                           unsigned char track, 
                           unsigned char numberOfSectors, 
                           unsigned char far *trackMap );


/* Przerwanie 13h / 05h - formatuj ˜cie¾k© (dyskietka) */
unsigned char formatHdTrack( unsigned char drive, 
                             unsigned char head, 
                             unsigned int cylinder, 
                             unsigned char sector, 
                             unsigned char far *flagTable );


/* Przerwanie 13h / 08h - Czytaj parametry dysku (dysk twardy) */
unsigned char getDisketteGeometry( unsigned char drive,
                                   unsigned int *maxTrack,
                                   unsigned char *maxSector,
                                   unsigned char *maxHead,
                                   unsigned char *nrDrives,
                                   unsigned char far **fdpt );


/* Przerwanie 13h / 08h - Czytaj parametry stacji (dyskietka) */
unsigned char getHdGeometry( unsigned char drive,
                             unsigned int *maxCylinder,
                             unsigned char *maxSector,
                             unsigned char *maxHead,
                             unsigned char *nrDrives,
                             unsigned char far **fdpt );


/* Przerwanie 13h / 09h - Inicjalizacja parametr¢w dysku (dysk tawrdy) */
unsigned char initHdController( unsigned char drive );


/* Przerwanie 13h / 0Ah - Czytaj "dˆugie" sektory (dysk twardy) */
unsigned char readLong( unsigned char drive, 
                        unsigned char numberOfSectors, 
                        unsigned char head, 
                        unsigned int cylinder, 
                        unsigned char sector, 
                        unsigned char far *bufferPtr );


/* Przerwanie 13h / 0Bh - Zapisz "dˆugie" sektory (dysk twardy) */
unsigned char writeLong( unsigned char drive, 
                         unsigned char numberOfSectors, 
                         unsigned char head, 
                         unsigned int cylinder, 
                         unsigned char sector, 
                         unsigned char far *bufferPtr );


/* Przerwanie 13h / 0Ch - Id« do cylindra (dysk twardy) */
unsigned char seekCylinder( unsigned char drive, 
                            unsigned char head, 
                            unsigned int cylinder, 
                            unsigned char sector );


/* Przerwanie 13h / 0Dh - Alternatywny reset dysku (dysk twardy) */
unsigned char altDiskReset( unsigned char drive );


/* Przerwanie 13h / 10h - Sprawdzenie gotowo˜ci dysku (dysk twardy) */
unsigned char testDriveReady( unsigned char drive );


/* Przerwanie 13h / 11h - Rekalibracja dysku (dysk twardy) */
unsigned char recalibrateDrive( unsigned char drive );


/* Przerwanie 13h / 12h - Wykonaj diagnostyk© RAM sterownika (dysk twardy) */
unsigned char doControllerRAMDiagnostic( unsigned char drive );


/* Przerwanie 13h / 13h - Wykonanie diagnostyki sterownika (dysk twardy) */
unsigned char doControllerDiagnostic( unsigned char drive );


/* Przerwanie 13h / 14h - Wykonaj diagnostyk© sterownika (dysk twardy) */
unsigned char executeControllerDiagnostic( unsigned char drive );


/* Przerwanie 13h / 15h - Czytaj typ stacji (dyskietka) */
unsigned char getDriveType( unsigned char drive );


/* Przerwanie 13h / 15h - Czytaj typ dysku (dysk twardy) */
unsigned char driveIsValid( unsigned char drive );
unsigned char getSectorsOnDrive( unsigned char drive, unsigned long *out );


/* Przerwanie 13h / 16h - Sprawd« stan linii zmiany no˜nika (dyskietka) */
unsigned char getChangeSignal( unsigned char drive );


/* Przerwanie 13h / 17h - Ustaw typ dyskietki do formatowania (dyskietka) */
unsigned char setDriveType( unsigned char drive, 
                            unsigned char type );


/* Przerwanie 13h / 18h - Ustaw typ no˜nika do formatowania (dyskietka) */
unsigned char setMediaType( unsigned char drive, 
                            unsigned char maxTracks,
                            unsigned char sectorsPerTrack,
                            unsigned char far **tablePtr );


/* Przerwanie 13h / 19h - Parkuj gˆowice (dysk twardy) */
unsigned char parkHeads( unsigned char drive );


/* Przerwanie 14h / 00h - Inicjalizacja portu szeregowego */
unsigned int initSerialPort( unsigned int port, 
                             unsigned char setupVal );


/* Przerwanie 14h / 01h - Wy˜lij znak przez port szeregowy */
unsigned char comTx( unsigned int port, 
                     unsigned char charToXmit );


/* Przerwanie 14h / 02h - Odbierz znak z portu szeregowego */
unsigned int comRx( unsigned int port );


/* Przerwanie 14h / 03h - Odczytaj stan portu szeregowego */
unsigned int comReadStatus( unsigned int port );


/* Przerwanie 14h / 04h - Rozszerzona inicjalizacja portu szeregowego */
unsigned int comExtInit( unsigned int port, 
                         unsigned char breakCode, 
                         unsigned char parity, 
                         unsigned char stopBits, 
                         unsigned char wordLgt, 
                         unsigned char baudRate );


/* Przerwanie 14h / 05h - Rozszerzone serowanie modemem */
unsigned int comModemCtl( unsigned int port, 
                          unsigned char rwCode, 
                          unsigned char *modemCtlReg );


/* Przerwanie 15h / 83h - Ustaw/skasuj timer */
extern unsigned char doneFlag;
unsigned char startTimer( unsigned long timeValue );

/* Makrodefinicje sprawdzaj¹ce czy timer pracuje */
#define TIMER_HAS_EXPIRED( flag ) ( (flag & 128) == 1 )
#define TIMER_IS_RUNNING( flag )  ( (flag & 128) == 0 )


/* Przerwanie 15h / 84h - Czytaj stan joysticka */
unsigned char readJsSwitches( void );
void readJsPots( unsigned *potAx,
                 unsigned *potAy,
                 unsigned *potBx,
                 unsigned *potBy );


/* Przerwanie 15h / 86h - Bezwarunkowe oczekiwanie */
unsigned char suspendProgram( unsigned long timeValue );


/* Przerwanie 15h / 88h - Podaj wielko˜† pami©ci Extended */
unsigned int getExtendedMemorySize( void );


/* Przerwanie 15h / C0h - Podaj informacj© o ˜rodowisku pracy */
struct cfgParamTbl
{
   unsigned int  tableLength;
   unsigned char systemModelCode;
   unsigned char systemTypeCode;
   unsigned char BIOSRevisionNumber;
   unsigned char cfgInfoBits;
   };

struct cfgParamTbl far *getCfgParmTblAddr( void );


/* Przerwanie 15h / C1h - Podaj segment rozszerzonego BDA 
unsigned char getEBDASegment( unsigned char *segment );


/* Przerwanie 15h / C200h - Wˆ¥cz / wyˆ¥cz mysz PDI */
#define mouseOn 1
#define mouseOff 0
unsigned char turnMouseOnOff( int flag );


/* Przerwanie 15h / C201h - Reset myszy PDI */
unsigned char resetMouse( void );


/* Przerwanie 15h / C202h - Ustawienie cz©sto˜ci pr¢bkowania myszy PDI */
unsigned char setMouseSampleRate( unsigned char newRate );


/* Przerwanie 15h / C203h - Ustaw rozdzielczo˜† myszy PDI */
unsigned char setMouseResolution( unsigned char newResolution );


/* Przerwanie 15h / C204h - Odczytaj typ myszy PDI */
unsigned char readMouseIdByte( unsigned char *idByte );


/* Przerwanie 15h / C205h - Inicjalizacja myszy PDI */
unsigned char initPDI( unsigned char packetSize );


/* Przerwanie 15h / C206h - Pobranie/ustawienie wsp¢ˆczynnika skalowania myszy PDI */
unsigned char getMouseStatus( unsigned char command,
                              unsigned char *buttons,
                              unsigned char *resolution,
                              unsigned char *sampleRate );


/* Przerwanie 15h / C207h - Ustawienie adresu sterownika myszy PDI */
unsigned char installMouseDriver( void far *func );


/* Przerwanie 15h / D800h - Czytaj informacje o gnie«dzie EISA */

struct EISASlotInfo
{
   unsigned int boardId1;
   unsigned int boardId2;
   unsigned char boardInfo;
   unsigned char cfgUtilMajorVer;
   unsigned char cfgUtilMinorVer;
   unsigned int  checkSum;
   unsigned char functionCount;
   unsigned char functionSummary;
   };

unsigned char readEISASlotInfo( unsigned char slot,
                                struct EISASlotInfo far *out );


/* Przerwanie 16h / 00h - Odczytaj znak z klawiatury */
unsigned int getKbdChar( void );


/* Przerwanie 16h / 01h - Sprawd« czy s¥ gotowe dane */
unsigned int testKbdCharReady( void );


/* Przerwanie 16h / 02h - Czytaj stan klawiatury */
unsigned char readShiftStatus( void );


/* Przerwanie 16h / 03h - Ustaw op¢«nienie/szybko˜† powtarzania */
void setRepeatRate( unsigned char delay, 
                    unsigned char repeatRate );


/* Przerwanie 16h / 05h - Zapisz znak do bufora */
unsigned char writeKbdData( unsigned char scanCode, 
                            unsigned char charCode );


/* Przerwanie 16h / 10h - Odczytaj znak z klawiatury rozszerzonej */
unsigned int getEnhancedKbdChar( void );


/* Przerwanie 16h / 11h - Sprawd« czy s¥ gotowe dane z klawiatury rozszerzonej*/
unsigned int testEnhancedKbdCharReady( void );


/* Przerwanie 16h / 12h - Odczataj stan klawiatury rozszerzonej */
unsigned int readExtendedShiftStatus( void );


/* Przerwanie 17h / 00h - Drukuj znak */
unsigned char printChar( unsigned int printer, 
                         unsigned char chToPrint );


/* Przerwanie 17h / 01h - Inicjalizacja portu drukarki */
unsigned char initPrinterPort( unsigned int port );


/* Przerwanie 17h / 02h - Pobierz sˆowo stanu drukarki */
unsigned char getPrinterStatus( unsigned int printer );


/* Przerwanie 1Ah / 00h - Odczytaj stan licznika systemowego */
unsigned char readTickCounter( unsigned long *out );


/* Przerwanie 1Ah / 01h - Ustaw stan licznika systemowego */
void setTickCounter( unsigned long tickValue );


/* Przerwanie 1Ah / 02h - Odczytaj stan zegara (RTC) */
void readRTCTime( unsigned char *hour, 
                  unsigned char *min, 
                  unsigned char *sec );


/* Przerwanie 1Ah / 03h - Ustaw czas (RTC) */
void setRTCTime( unsigned char hour, 
                 unsigned char min, 
                 unsigned char sec );


/* Przerwanie 1Ah / 04h - Odczytaj dat© (RTC) */
void readRTCDate( unsigned int *year, 
                  unsigned char *month, 
                  unsigned char *day );


/* Przerwanie 1Ah / 05h - Ustawienie daty (RTC) */
void setRTCDate( unsigned int year, 
                 unsigned char month, 
                 unsigned char day );


/* Przerwanie 1Ah / 06h - Ustawienie alarmu (RTC) */
void setRTCAlarm( unsigned char hour, 
                  unsigned char min, 
                  unsigned char sec );


/* Przerwanie 1Ah / 07h - Reset alarmu RTC */
void resetRTCAlarm( void );

