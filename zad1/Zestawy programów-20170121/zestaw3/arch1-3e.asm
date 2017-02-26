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
	MOV 	CH,DLUGOSC_TAB; do CH przenies dlugosc tablicy
UP2: 
	MOV 	CL,DLUGOSC_TAB; do CL przenies dlugosc tablicy
	LEA 	SI,TAB; pobierz offset do TAB(pierwszy element)
	XOR 	BL,BL;wyczysc rejestr w ktorym trzymane jest czy byla zamiana czy nie
UP1: 
	MOV AX,[SI] ; wrzuc pierwszy element do AX (w AL bedzie pierwsza wartosc i AH bedzie nastepna bo tablica jest DB)
	CMP AL,AH ; porownaj wartosci. Tak naprawde oblicza sobie AH-AL. Wiec jezeli wyjdzie ujemna to CF jest ustawiane i przez to dziala.
	JC DOWN	;jezeli jest przeniesienie czyli
	;===== ZAMIEN
	MOV DL,[SI+1]
	XCHG [SI],DL
	MOV [SI+1],DL
	;=====
	INC BL
DOWN: 
	INC SI
	DEC CL ;sprawdz czy to ostatni indeks w petli wewnetrznej
	JNZ UP1
	TEST BL,BL ; byla chociaz jedna zamiana
	JZ	KONIEC;jezeli nie bylo to znaczy ze jest posortowane
	DEC CH
	JNZ UP2; sprawdz czy to ostatni ineks w petli zewnetrznej
 KONIEC:
 
   	MOV     AX, 4C00h
	INT     21h

CODE 		ENDS
END 	START
