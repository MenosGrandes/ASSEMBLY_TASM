;=============================================================================;
;                                                                             ;
; Plik           : arch1-0c.asm                                               ;
; Format         : COM                                                        ;
; Autor          : Przemys³aw Nowak                                           ;
; Data utworzenia: 21.02.2004                                                 ;
; Uwagi          : Program przyk³adowy wczytujacy do tablicy znaki            ;
;                  wprowadzane przez uzytkownika, a nastepnie wyswietlajacy   ;
;                  wszystkie te znaki.                                        ;
;                                                                             ;
;=============================================================================;

                .MODEL  TINY

        MAX_LICZBA_ZNAKOW       EQU     5       ; rozmiar tablicy
        KL_KONIEC               EQU     13      ; klawisz ENTER
        CRLF                    EQU     13,10   ; znak nowej linii

Kod             SEGMENT

                ORG     100h
                ASSUME  CS:Kod, DS:Kod, SS:Kod

Start:
                jmp     Poczatek        ; przeskocz obszar danych

Tablica         DB      MAX_LICZBA_ZNAKOW DUP (?)
LiczbaZnakow    DB      0
txtWprowadz     DB      CRLF,"Wprowadz znak (ENTER - pomin wprowadzanie): $"
txtBrak         DB      CRLF,"Nie wprowadziles zadnych znakow!$"
txtZnaki        DB      CRLF,"Wprowadziles znaki: $"

Poczatek:
                mov     bx, OFFSET Tablica ; ustaw wskaznik aktualnej pozycji
                                           ; w tablicy na jej poczatek
                mov     cx, MAX_LICZBA_ZNAKOW ; ustaw licznik petli na
                                              ; maksymalna liczbe znakow do
                                              ; wprowadzenia

; Wczytanie znakow
Zapytaj:
                mov     ah, 09h         ; wyswietl napis zachety
                mov     dx, OFFSET txtWprowadz
                int     21h
                mov     ah, 07h
Wczytaj:
                int     21h             ; wczytaj znak bez echa
                or      al, al          ; czy wczytano znak o kodzie
                                        ; rozszerzonym?
                jnz     Nierozsz        ; nie - skok
                int     21h             ; tak - wczytaj kod rozszerzony
                jmp     Wczytaj         ; zignoruj znak o kodzie rozszerzonym
                                        ; - skok do kolejnego wczytania znaku
Nierozsz:
                cmp     al, KL_KONIEC   ; czy wczytano znak konczacy
                                        ; wprowadzanie?
                je      Wprowadzone     ; tak - skok
                mov     [bx], al        ; nie - zapisz znak pod aktualna
                                        ; pozycja w tablicy
                inc     bx              ; zwieksz aktualna pozycje w tablicy
                inc     LiczbaZnakow    ; zwieksz licznik wczytanych znakow
                mov     ah, 02h         ; wyswietl wczytany znak
                mov     dl, al
                int     21h 
                loop    Zapytaj         ; powtorz wczytywanie o ile nie
                                        ; zapisano calej tablicy

; Sprawdzenie czy wprowadzono jakies znaki
Wprowadzone:
                cmp     LiczbaZnakow, 0 ; czy liczba wprowadzonych znakow jest
                                        ; wieksza od zera?
                jne     SaZnaki         ; tak - skok

                mov     ah, 09h            ; nie - wyswietl napis o braku
                mov     dx, OFFSET txtBrak ; wprowadzonych znakow
                int     21h
                jmp     Koniec          ; skok do zakonczenia programu

; Wyswietlenie wprowadzonych znakow
SaZnaki:
                mov     ah, 09h             ; wyswietl napis o wprowadzonych
                mov     dx, OFFSET txtZnaki ; znakach
                int     21h

                mov     bx, OFFSET Tablica ; ustaw wskaznik aktualnej pozycji
                                           ; w tablicy na jej poczatek
		xor     ch, ch             ; ustaw licznik petli na liczbe
                mov     cl, LiczbaZnakow   ; wprowadzonych znakow

                mov     ah, 02h
Wyswietl:
                mov     dl, [bx]        ; wyswietl znak spod aktualnej
                int     21h             ; pozycji w tablicy
                inc     bx              ; zwieksz aktualna pozycje w tablicy
                mov     dl, ' '         ; wyswietl spacje
                int     21h
                loop    Wyswietl        ; powtorz wyswietlanie az do
                                        ; ostatniego wprowadzonego znaku

; Zakonczenie programu
Koniec:
                mov     ax, 4C00h       ; zakoncz program z kodem powrotu                                        
                int     21h             ; zakonczenia poprawnego

Kod             ENDS

                END     Start

