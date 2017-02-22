;=============================================================================;
;                                                                             ;
; Plik           : arch1-0e.asm                                               ;
; Format         : EXE                                                        ;
; Autor          : Przemys³aw Nowak                                           ;
; Data utworzenia: 23.02.2003                                                 ;
; Uwagi          : Program przykladowy wyswietlajacy na ekranie napis         ;
;                  powitalny.                                                 ;
;                                                                             ;
;=============================================================================;

                .MODEL  SMALL

Dane            SEGMENT

Napis           DB      "Witaj swiecie!",13,10,"$"

Dane            ENDS

Kod             SEGMENT

                ASSUME  CS:Kod, DS:Dane, SS:Stosik

Start:
; Zaladowanie rejestru segmentowego danych
                mov     ax, SEG Dane    ; zaladowanie rejestru DS
                mov     ds, ax          ; segmentem danych

; Wyczyszczenie ekranu
                mov     ah, 06h         ; funkcja karty grafiki przewijajaca
                                        ; linie fragmentu ekranu w gore
                mov     al, 0           ; liczba linii do przewiniecia -
                                        ; caly ekran (czyszczenie ekranu)
                mov     bh, 07          ; atrybut pustych linii - jasnoszary
                mov     ch, 0           ; wiersz lewego gornego rogu
                                        ; przewijanego framgmentu - lewy
                                        ; gorny rog ekranu
                mov     cl, 0           ; kolumna lewego gornego rogu
                                        ; przewijanego framgmentu - lewy
                                        ; gorny rog ekranu
                mov     dh, 24          ; wiersz prawego dolnego rogu
                                        ; przewijanego framgmentu - prawy
                                        ; dolny rog ekranu
                mov     dl, 79          ; kolumna prawego dolnego rogu
                                        ; przewijanego framgmentu - prawy
                                        ; dolny rog ekranu
                int     10h             ; wywolanie przerwania karty grafiki

; Ustawienie kursora na poczatku ekranu
                mov     ah, 02h         ; funkcja karty grafiki ustawiajaca
                                        ; kursor na nowej pozycji
                mov     bh, 0           ; numer strony trybu graficznego
                mov     dh, 0           ; wiersz nowej pozycji kursora - lewy
                                        ; gorny rog ekranu
                mov     dl, 0           ; kolumna nowej pozycji kursora -
                                        ; lewy gorny rog ekranu
                int     10h             ; wywolanie przerwania karty grafiki

; Wyswietlenie napisu w miejscu ustawienia kursora
                mov     ah, 09h         ; funkcja systemowa wyswietlajaca
                                        ; napis (zakonczony znakiem '$')
                mov     dx, OFFSET Napis ; adres napisu do wyswietlenia
                                         ; (przesuniecie w segmencie danych)
                int     21h             ; wywolanie przerwania systemowego

; Zakonczenie programu
                mov     ax, 4C00h       ; funkcja konczaca program, kod
                                        ; powrotu - zakonczenie poprawne
                int     21h             ; wywolanie przerwania systemowego

Kod             ENDS

Stosik          SEGMENT STACK

                DB      100h DUP (?)

Stosik          ENDS

                END     Start

