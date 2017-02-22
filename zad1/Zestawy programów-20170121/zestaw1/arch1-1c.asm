;=============================================================================;
;                                                                             ;
; Plik           : arch1-1c.asm                                               ;
; Format         : COM                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy wzor: (a-b)*c/d                        ;
;                                                                             ;
;=============================================================================;

                .MODEL TINY

a               EQU      20
b               EQU     10
c               EQU    100
d               EQU     5

Kod             SEGMENT

                ORG    100h
                ASSUME   CS:Kod, DS:Kod, SS:Kod

Start:
                jmp Poczatek


Wynik           DW      ?

Poczatek:
				mov     ax,c  ;wrzuc c do AX
				mov		bx,d  ;wrzuc d do BX
                div     bx	  ; podziel AX przez d ->  c/d -> zapisz w AX
                mov     bx, ax;skopiuj ax do bx
                mov     ax, a ; skopiuj a do ax
                sub     ax, b ;odejmij od AX B -> a-b - > zapisz w AX

                mov     Wynik,ax

                mov     ax, 4C00h
                int     21h

Kod             ENDS

                END     Start

