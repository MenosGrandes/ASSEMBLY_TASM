

;=============================================================================;
;                                                                             ;
; Plik           : arch1-1e.asm                                               ;
; Format         : EXE                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program znajdujacy najwieksza liczbe w tablicy             ;
;                                                                             ;
;=============================================================================;

                .MODEL  TINY

Dane            SEGMENT

DL_TABLICA      EQU     0Bh
Tablica         DB      05h, 04h, 07h, 10h, 12h, 33h, 15h, 01h, 11h, 08h, 0Ah, 05h

Dane            ENDS


Kod             SEGMENT
                ASSUME  CS:Kod, DS:Dane, SS:Kod

Start:
                mov     ax, SEG Dane
                mov     ds, ax
                xor     ax, ax
                mov     bx, OFFSET Tablica ; pierwszy element tablicy
                mov     cl, DL_TABLICA ; dlugosc tablicy do CX
                mov     al, [bx]
                inc     bx ; nastepny element
                dec     cl;
Petla:  
                cmp     al, [bx] ;porownaj
                jae     Skok ; jezeli jest  al<=[bx] (mniejsze lub rowne)
                mov     al, [bx]
Skok:
                inc     bx
                loop    Petla

                mov     ax, 4C00h
                int     21h

Kod             ENDS
                END    Start


