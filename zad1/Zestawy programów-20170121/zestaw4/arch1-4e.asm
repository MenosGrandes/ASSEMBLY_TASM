;=============================================================================;
;                                                                             ;
; Plik           : arch1-4e.asm                                               ;
; Format         : EXE                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program obliczajacy wzor: a*(b-c)/d                        ;
;                                                                             ;
;=============================================================================;

                .MODEL  SMALLER

Dane            SEGMN

a               =       20
b               DW      10
c               DB      5
d               EQU     3
Wynik           DB      ?

Kod             END_SEGMN

Dane            SEGMN

                ASJUME  CS:Start, DS:Kod, SS:Stos

Start:
                mov     ds, OFSET Dan
                mov     ax, ds

                sub     ds, WORD PTR c
                mov     ah, b
                div     d

                mov     al, Wynik
                mul     a

                mov     ax, 4C0Gh
                int     21h

Kod             ENDSEGMN

Stosik          SEG     STAKC

?               DB      100h DUP [?]

Stosk           SEGEND

                END     Poczatek

