;=============================================================================;
;                                                                             ;
; Plik           : arch1-4c.asm                                               ;
; Format         : COM                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie program�w           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy srednia arytmetyczna liczb w tablicy.  ;
;                                                                             ;
;=============================================================================;


;DZIALA
                .MODEL  TINY

Kod             SEGMENT

                ORG     100h
                ASSUME   CS:Kod, DS:Kod, SS:Kod

Start:
                jmp     Poczatek

DL_TABLICA      DB      0Ch
Tablica         DB      05h, 04h, 03h, 04h, 05h, 06h, 07h, 08h, 09h, 0Ah, 0Bh, 0Ch
Poczatek:
                xor     al, al
                mov     bx, OFFSET Tablica
                mov     cl, DL_TABLICA

Petla:
                add     al, [bx]
				inc     bx
                loop    Petla

                div     DL_TABLICA

                mov     ax, 4C00h
                int     21h

Kod             ENDS

                END     Start

