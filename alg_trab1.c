#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <math.h>

// 120 36
// mode con: cols=160 lines=78

void lang(char* lc) {
	// Permite caract�res acentuados
	if(strcmp(lc, "br") == 0) {
		setlocale(LC_ALL, "pt_br");
	} // Volta a tabela ASCII padr�o
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

void showCursor(bool show) {
    HANDLE hStdOut = NULL;
    CONSOLE_CURSOR_INFO curInfo;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = show;
    SetConsoleCursorInfo(hStdOut, &curInfo);
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

char arrowController() {
	char specialKey, key, finalKey;
	while(true) {
		specialKey = getch();
		
		if(specialKey == -32) {
			key = getch();
			switch(key) {
				case 72:
				case 75:
					finalKey = 'U';
					break;
				case 80:
				case 77:
					finalKey = 'D';
					break;
			}
			break;
		}
		else if(specialKey == 27) {
			finalKey = 'E';
			break;
		}
		else if(specialKey == 13) {
			finalKey = 'R';
			break;
		}
	}
	
	return finalKey;
}

double secureInputController(int xStart, int yStart) {
	gotoxy(xStart, yStart);
	
	bool isDelimiter;
	char key, input[30];
	int length = 0, lengthDelimiter = 0;
	while(key != 13) {
		key = getch();
		
		isDelimiter = (key == '.' || key == ',');
		
		if(!isdigit(key) && key != 8 && !isDelimiter) {
			if(key != 13) {
				setColor(12);
				gotoxy(xStart + 3, yStart - 4); printf("       Apenas n�meros s�o aceitos!       ");
				setColor(7);
			}
			else {
				gotoxy(xStart + 3, yStart - 4); printf("                                         ");
			}
			continue;
		}
		if(length == 9) {
			setColor(12);
			gotoxy(xStart + 3, yStart - 4); printf("O n�mero n�o pode ter mais que 9 d�gitos!");
			setColor(7);
		}
		else {
			gotoxy(xStart + 3, yStart - 4); printf("                                         ");
		}
		
		if(key == 8) {
			if(length > 0) {
				gotoxy(xStart + --length, yStart); printf(" ");
				if(input[length] == '.') {
					lengthDelimiter = 0;
				}
				
				input[length] = '\0';
			}
		}
		else if(length < 9) {
			if(lengthDelimiter == 0 || !isDelimiter) {
				gotoxy(xStart + length++, yStart); printf("%c", key);
				input[length-1] = (key == ',' ? '.' : key); // caso o input seja com v�rgula, troca pra ponto
				input[length] = '\0';	
			}
			
			if(isDelimiter)
				lengthDelimiter++;
		}
		
		if(length < 9) {
			gotoxy(xStart + 3, yStart - 4); printf("                                         ");
		}
		gotoxy(0, 1); printf("Length: %d", length);
	}
	gotoxy(0, 2); printf("Input: %s", input);
	
	char *temp;
	
	return strtod(input, &temp);
}

int selectCalcMeasure(int xStart, int yStart) {
	gotoxy(xStart + 5, yStart); printf("[ Graus | Rad ]");
	
	bool grausSelected = true;
	char key;
	
	while(true) {
		if(grausSelected) {
			setColor(3); gotoxy(xStart + 7, yStart); printf("Graus"); setColor(7);	
			gotoxy(xStart + 15, yStart); printf("Rad");
		}
		else {
			gotoxy(xStart + 7, yStart); printf("Graus");
			setColor(3); gotoxy(xStart + 15, yStart); printf("Rad"); setColor(7);
		}
		
		key = arrowController();
		if(key == 'U' || key == 'D')
			grausSelected = !grausSelected;
		else if(key == 'R') {
			return grausSelected ? 1 : 2;	
		}
	}
}

void submenuController(int xStart, int yStart, int selectedOption) {
	paintArea(xStart-2, xStart + 62, yStart-1, yStart + 18, 0, 219, 7, true);
	paintArea(xStart, xStart + 61, yStart, yStart + 1, 0, ' ', 0, true);
	
	gotoxy(xStart, yStart); printf("Op��o selecionada: ");
	switch(selectedOption) {
		case 1:
			printf("Seno"); break;
		case 2:
			printf("Cosseno"); break;
		case 3:
			printf("Tangente"); break;
		case 4:
			printf("Secante"); break;
		case 5:
			printf("Cossecante"); break;
		case 6:
			printf("Cotangente"); break;
		case 7:
			printf("Arco seno"); break;
		case 8:
			printf("Arco cosseno"); break;
		case 9:
			printf("Arco tangente"); break;
	}
	
	gotoxy(xStart, yStart + 2); printf("Valor: ");
	
	double value = secureInputController(xStart + 7, yStart + 2);
	gotoxy(0, 3); printf("Value: %f", value);
	int selectedCalcOption = selectCalcMeasure(xStart + 16, yStart + 2);
	
	// se a escolha for o valor em graus, ele define o multiplicador
	// para transformar graus em radianos, se n�o, mant�m em 1 (pois j� estar� em radianos)
	double multiplier = ((selectedCalcOption == 1) ? M_PI/180.0 : 1);
	double result = 0;
	
	switch(selectedOption) {
		case 1:
			result = sin(value * multiplier);
			break;
	}
	
	
	gotoxy(xStart, yStart + 3); printf("Resultado: %f", result);
}

void menuController(int xStart, int yStart) {
	int yCurrent = yStart;
	int selectedIndex;
	char key;
	
	while(true) {
		gotoxy(xStart, yCurrent); printf(">");
		key = arrowController();
		paintArea(xStart, xStart + 2, yCurrent, yCurrent + 1, 0, ' ', 0, false);
		
		if(key == 'U') {
			if(yCurrent == yStart)
				yCurrent = yStart + 16;
			else
				yCurrent -= 2;
		}
		else if(key == 'D') {
			if(yCurrent == yStart + 16)
				yCurrent = yStart;
			else
				yCurrent += 2;
		}
		else if(key == 'E') {
			system("cls");
			return;
		}
		else if(key == 'R') {
			selectedIndex = (yCurrent - yStart + 2)/2;
			gotoxy(0, 0); printf("Selected option: %d", selectedIndex);
			
			submenuController(xStart + 40, yStart, selectedIndex);
			
			gotoxy(xStart + 40, yStart + 15); printf("Pressione qualquer tecla para continuar...");
			getch();
			
			paintArea(xStart + 38, xStart + 102, yStart-1, yStart + 18, 1, ' ', 0, false);
		}
	}
}

void menu(int xStart, int yStart) {
	showCursor(false);
	xStart += 2;
	gotoxy(xStart, yStart); printf("Seno");
	gotoxy(xStart, yStart + 2); printf("Cosseno");
	gotoxy(xStart, yStart + 4); printf("Tangente");
	gotoxy(xStart, yStart + 6); printf("Secante");
	gotoxy(xStart, yStart + 8); printf("Cossecante");
	gotoxy(xStart, yStart + 10); printf("Cotangente");
	gotoxy(xStart, yStart + 12); printf("Arco seno");
	gotoxy(xStart, yStart + 14); printf("Arco cosseno");
	gotoxy(xStart, yStart + 16); printf("Arco tangente");
	
	setColor(3); gotoxy(10, 35); printf("Utilize as setas para mover o cursor e Enter para confirmar"); setColor(7);
	
	menuController(xStart - 2, yStart);
}

main () {
	header();
	
	menu(10, 8);
}	


















