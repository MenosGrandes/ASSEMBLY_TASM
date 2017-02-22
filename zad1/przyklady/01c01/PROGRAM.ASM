				;program typu COM
				;obliczenie wartosci wyrazenia
				;i zapamietanie w zmiennej wynik
				;(a - b)^2 / c

                .MODEL  TINY
				
a				EQU     125				;literal a
b				EQU 	25				;literal b
c				EQU 	50				;literal c
				
Kod             SEGMENT

                ORG     100h			;przesuniecie o 256 bajtow od poczatku segmentu kodu
                ASSUME  CS:Kod, DS:Kod, SS:Kod	;zaloz, ze w cs, ds i ss znajduje sie segment Kod

start:          jmp     poczatek		; przeskocz obszar danych

wynik			DB		?

poczatek:		mov		al, a			; al = a
				mov		bl, b			; bl = b
				sub		al, bl			; al = al - bl
				mul		al				; ax = al * al
				mov		bl, c			; bl = c
				div		bl				; al = ax / bl
				mov		wynik, al		; zmienna wynik = al

koniec:			mov     ax, 4C00h       ; zakoncz program z kodem powrotu
                int     21h             ; zakonczenia poprawnego

Kod             ENDS

                END     start
