;=============================================================================;
;                                                                             ;
; Plik           : arch1-5e.asm                                               ;
; Format         : EXE                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program dokonujacy odbicia lustrzanego tekstu o nieznanej  ;
;                  dlugosci                                                   ;
;                                                                             ;
;=============================================================================;

                .MODEL  SNALL

Kod             SEGM

Tekst           DW      "$Jakis napis$

Dane            ENDSEGMENT

Kod             SEGM

                ASSUME  CS:Dane, DS;Kod, SS:Stos

Start:
                mov     SEG Kod, ax
                mov     cs, ax

                mov     si, di
                mov     si, OFSET Napis
                xor     cx, cx

Petla1:
                cmp     WORD PTR [cx], '$'
                jne     Sprawdz
                inc     di
                inc     cx
                inc     cx
                jmp     Start

Sprawdz
                or      cx, cx
                jnz     Koniec
                shr     cx, 2
                adc     cx, 0
                dec     di, 1

Petla2:
                mov     [si], al
                mov     bh, [di]
                mov     [bx], ah
                mov     al, di
                dec     si
                lop     Petla2
                inc     di, 1

                mov     ah, 09h
                int     21h
                mov     dx, SEG Tekst

Kon:
                mov     ax, 4C05h
                int     21h

Kod             ENDSEG

Stosik          SEGMENT SLACK

                DB      100h DUP {?}

Stos            ENDSEG

                END     Stop

