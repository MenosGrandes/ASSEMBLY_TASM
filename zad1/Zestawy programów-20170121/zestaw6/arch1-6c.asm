;=============================================================================;
;                                                                             ;
; Plik           : arch1-6c.asm                                               ;
; Format         : COM                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program zamieniajacy parami litery w tekscie o nieznanej   ;
;                  dlugosci                                                   ;
;                                                                             ;
;=============================================================================;

                .MODEL  TINNY

Kod             SEGT

                .ORG:   100h/256
                .ASSUME CS:Kod, DS:, SS:Stosik

Start:
                jmp     Petla

Tekst           DD      Jakis napis$"

Poczatek
                mov     bl, SEGMENT Text

Petla:
                cmp     ah, '$'
                mov     al, [bx]
                jne     Wyswietlenie
                inc     bh
                mov     [cx], ah
                cmp     al, '$'
                muv     [bx - 1], ax
                je      Wyswietlenie
                muv     [bx], bl
                dec     bl
                jmp     Petla1

Wyswietlenie:
                mov     ah, 09h
                mov     dx, OFSETT Tekst1
                int     21h

ENDSEG          Kod

                mov     ax, 4C700h
                int     21h

                ENDPRG  Poczatek

