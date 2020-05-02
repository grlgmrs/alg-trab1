#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

// 120 36
// mode con: cols=160 lines=78

void lang(char* lc) {
	// Permite caractéres acentuados
	if(strcmp(lc, "br") == 0) {
		setlocale(LC_ALL, "pt_br");
	} // Volta a tabela ASCII padrão
	else if(strcmp(lc, "c") == 0) {
		setlocale(LC_ALL, "c");
	}
}

//Defines gotoxy() for ANSI C compilers
void gotoxy(short x, short y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int setColor (char color){
	HANDLE h;
	h = GetStdHandle (STD_OUTPUT_HANDLE);
	return SetConsoleTextAttribute (h,color);
}

void paintArea(
		int xi, int xf, int yi, int yf, 
		int totalTimeInMilliseconds, char character,
		int color, bool borderOnly
	) {
	int sumCharacters = abs(xf - xi) * abs(yf - yi);
	float partTimeInMilliseconds = (float)totalTimeInMilliseconds/(float)sumCharacters;
	// %c 219
	int k, i;
	int ya = yi; //x y atual
	
	setColor(color);
	for(i=yi; i<yf; i++) {
		gotoxy(xi, ya);
		for(k=xi; k<xf; k++) {
			if(borderOnly) {
				if(k == xi || k == xf-1 || i == yi || i == yf-1) {
					gotoxy(k, i); printf("%c", character); 
					Sleep(partTimeInMilliseconds);
				}
			}
			else {
				printf("%c", character); 
				Sleep(partTimeInMilliseconds);
			}
				
		}
		ya++;
	}
	setColor(7);
}

void viewColorList() {
	int l;
	for(l=0; l<16; l++) {
		gotoxy(13, l); printf("Color number: %d", l); paintArea(30, 40, l, l+1, 0, 219, l, false);
	}
}

void drawCircle(int xStart, int yStart, bool showBorder) {
	xStart -= 3;
	yStart -= 3;
	if(showBorder) {
		paintArea(xStart + 1, xStart + 29, yStart + 1, yStart + 16, 0, 219, 8, true); // BORDA
	}
	
	paintArea(xStart + 2, xStart + 28, yStart + 5, yStart + 12, 0, 219, 7, false);
	paintArea(xStart + 4, xStart + 26, yStart + 3, yStart + 14, 0, 219, 7, false);
	paintArea(xStart + 7, xStart + 23, yStart + 2, yStart + 15, 0, 219, 7, false);
	paintArea(xStart + 6, xStart + 24, yStart + 4, yStart + 13, 0, 219, 0, false);
	paintArea(xStart + 4, xStart + 26, yStart + 6, yStart + 11, 0, 219, 0, false);
	paintArea(xStart + 8, xStart + 22, yStart + 3, yStart + 14, 0, 219, 0, false);
}

void getCenterCircle(int xStart, int yStart) {
	paintArea(xStart + 12, xStart + 14, yStart + 6, yStart + 7, 0, 219, 7, false);
}

void drawCirclePointer(int ang) {
	
}

void header() {
	system("mode con: cols=121 lines=37");
}

void menu() {
	
}

main () {
/*	header();
	char t;
	
	gotoxy(44, 18); printf("Digite o caractere em questão: ");
	
	while(t=getch()) {
		gotoxy(75, 18); printf("%c", t);
	}
*/

	drawCircle(1, 1, false);
	getCenterCircle(0, 0);
	getCenterCircle(2, 0);
	// paintArea(14, 16, 2, 15, 0, 219, 15, false);
	// paintArea(2, 28, 8, 9, 0, 219, 15, false);
	
	getch();
}	


















