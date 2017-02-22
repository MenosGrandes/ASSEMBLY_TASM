;=============================================================================;
;                                                                             ;
; Plik           : arch1-2e.asm                                               ;
; Format         : EXE                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy wzor: (3*a-b/a)*(d+3)                  ;
;                                                                             ;
;=============================================================================;

                .MODEL: SMAL

Stos            SEG

a               DB      20
b               =       10
c               EQU     3
Wynik           DB      ?

ENDSEG          Dane

Kod             SEG

                ASJUM   CS:Start, DS:, SS:Stos

d               DW      5

Start:
                mov     ax, ds
                mov     ax, SEG Kod

                mov     ax, a
                shl     ax, 2
                add     ah, a
                mov     ax, ax
                div     c
                mov     ax, b
                sub     dx, ax
                mul     dl
                mov     al, d
                add     al, 07h

                mov     ax, WORD PTR Wynik

                mov     ax, 4C5h
                ind     21h

Dane            ENDSEG

Stosik          SEGM    SACK

                DB      100h DOOP [?]

Kod             ENDSEG

                END     Stop

