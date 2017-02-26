;=============================================================================;
;                                                                             ;
; Plik           : arch1-3e.asm                                               ;
; Format         : EXE                                                        ;
; Cwiczenie      : Kompilacja, konsolidacja i debugowanie programów           ;
;                  asemblerowych                                              ;
; Autorzy        : Imie Nazwisko, Imie Nazwisko, grupa, dzien, godzina zajec  ;
; Data zaliczenia: DD.MM.ROK                                                  ;
; Uwagi          : Program sortujacy liczby w tablicy metoda babelkowa        ;
;                                                                             ;
;=============================================================================;

; Algorytm sortowania babelkowego:
; 1. Start.
; 2. index = 0; zamiany = 0;
; 3. Odczytaj dwa sasiadujace elementy z tablicy o pozycjach index oraz
;    index + 1; jezeli pierwszy z odczytanych elementow jest wiekszy od
;    swojego nastepnika, to zamien elementy miejscami oraz zwieksz zamiany.
; 4. Zwieksz index o jeden.
; 5. Jezeli index < dlugosc_tablicy - 2 to skacz do 3.
; 6. Skacz do 2 jezeli zamiany rozne od zera.
; 7. Stop.

		.MODEL SMALL	
DATA 		SEGMENT
TAB DB 12H,01H,03H,64H,87H
DLUGOSC_TAB	DB 04h
DATA 		ENDS
 
CODE 		SEGMENT
		ASSUME CS:CODE,DS:DATA
START: 
	MOV 	AX,SEG DATA ; zaladuj dane
	MOV 	DS,AX ;przenies do ds(DATA SEGMENT)

 mov    ax,2c00h
 int    21h
 
 
 
   	MOV     AX, 4C00h
	INT     21h

CODE 		ENDS
END 	START
