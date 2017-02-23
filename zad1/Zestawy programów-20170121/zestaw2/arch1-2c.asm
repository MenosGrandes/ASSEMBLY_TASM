 ; COM                                                        ;
 ; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
 ;                  asemblerowych                                              ;
 ; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
 ; Data zaliczenia: DD.MM.ROK                                                  ;
 ; Uwagi          : Program znajdujacy najmniejsza liczbe w tablicy            ;
 ;                                                                             ;
 ;=============================================================================;
 ;dziala
                 .MODEL  TINY
 
 Kod             SEGMENT
 
                 ORG     100h
                 ASSUME  CS:Kod, DS:Kod, SS:Kod
 
 Start:
 
 DL_TABLICA      EQU     0Ch
 Tablica         DB      04h, 05h, 03h, 10h, 11h, 33h, 15h, 09h, 11h, 02h, 01h, 00h
 Najmniejsza     DB      ?
 
                 jmp     Poczatek
 
 Poczatek:
                mov     cl, DL_TABLICA
                mov     bx, OFFSET Tablica
                xor 	ax,ax 
		mov     al, [bx]
                dec     cl
                inc     bx
  Petla:
                ;mov dx,[bx] 
		cmp     al, [bx]
                 jbe     Skok
                 mov     al, [bx]
  Skok:
 		 inc     bx
                loop     Petla
 
                 mov     ax, 4C00h
                 int     21h
 
 Kod            ENDS
 END		Start


