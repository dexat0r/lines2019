/*поле 9х9
Шарики появляются в случайном месте поля.
поле состоит из ячеек или клеток
структура клетки:
координаты относительно поля;
наличие шарика;
цвет шарика;

структура поля:
массив из клеток;
количество занятых клеток;
0
1
2
3
4
5
6
к190913


Новая Игра
Загрузить Игру
Лучшие результаты
Выход

сохранить игру
выйти из игры
*/

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <fstream>


using namespace std;

enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

ConsoleColor xy = White;
const int l = 11;
unsigned score; //счет
char name[10]; //Никнейм 
bool loading = 0; //Загружалась ли игра или начали новую
char niknames[5][5]; // никнеймы лучшитх 5 игроков
int scores[5]; //счета 5 лучших игроков
bool GameOver = true; // Флаг выхода из игры
bool FlagName = false; //Было ли введено имя
int Pointerx(5), Pointery(6), Pointerx1(0), Pointery1(0);
int Pointerx0(5), Pointery0(6); //Переменные для курсора
int NikLength = 0;  // Длина Никнейма
bool FlagCheck = 1; //Проверяет имеется ли возможность ставить еще шары
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
unsigned xlog;
unsigned ylog;

int stack[2][100];
int position = 0;

struct t_Item {
	int x; 
	int y;
	unsigned index;
	t_Item *Prev = NULL;
};

struct ya4eika {
	int x;
	int y;
	bool flag = false;
	char cvet = 32;
	int  foreground= 10;
	int background = 7;
	bool flag_prov = 1;
};

struct pole {
	ya4eika razmer[l][l];
	unsigned zanyato = 0;
};

t_Item *Head;
t_Item *Current;
unsigned Index = 0;


void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

t_Item* CreateList() {
	t_Item *Head = 0;
	Head = new t_Item;
	Head->Prev = NULL;
	return Head;
}

void DeleteList(t_Item *Beg) {
	t_Item *Curr;
	while (Beg) {
		Curr=Beg->Prev;
		delete Beg;
		Beg = Curr;
	}
}

t_Item *InsItem(t_Item *Beg, int x,int y) {
	t_Item *Next;
	Next = new t_Item;
	Next->Prev = Beg;
	Next->x = x;
	Next->y = y;
	Beg = Next;
	xlog = x;
	ylog = y;
	return Beg;
}

 t_Item *DeleteItem(t_Item *Beg) {
	 t_Item *Previous;
	 if (Beg) {
		 Previous = Beg->Prev;
		 delete Beg;
		 Beg = Previous;
		 if (Beg) {
			 xlog = Beg->x;
			 ylog = Beg->y;
		 }
		 return Beg;
	 }
}  

bool Logick(int x,int y,int x0,int y0,pole &a) {
	t_Item *List = CreateList();
	
	bool flag = 1;
	xlog = x;
	ylog = y;
	a.razmer[xlog][ylog].flag_prov = 0;
	List->x = xlog;
	List->y = ylog;
	t_Item *Head = List;
	do {
		if (!a.razmer[xlog + 1][ylog].flag && a.razmer[xlog + 1][ylog].flag_prov && xlog < 8) {
			List = InsItem(List, xlog + 1, ylog);
			a.razmer[xlog][ylog].flag_prov = 0;
			
		}
		else
			if (!a.razmer[xlog][ylog + 1].flag && a.razmer[xlog][ylog + 1].flag_prov && ylog<8) {
				List = InsItem(List, xlog, ylog + 1);
				a.razmer[xlog][ylog].flag_prov = 0;
				
			}
			else
				if (!a.razmer[xlog - 1][ylog].flag && a.razmer[xlog - 1][ylog].flag_prov && xlog > 0) {
					List = InsItem(List, xlog - 1, ylog);
					a.razmer[xlog][ylog].flag_prov = 0;
					
				}
				else
					if (!a.razmer[xlog][ylog - 1].flag && a.razmer[xlog][ylog - 1].flag_prov && ylog > 0) {
						List = InsItem(List, xlog, ylog - 1);
						a.razmer[xlog][ylog].flag_prov = 0;
						
					}
					else
						/*if (List->x == Head->x&&List->y == Head->y) {

						}
						else*/
							if (List&&(List->x !=Head->x||List->y !=Head->y))
							List = DeleteItem(List);
						else 
							flag = 0;
		
	} while ((List->x != x0 || List->y != y0) && flag);
	DeleteList(List);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			a.razmer[i][j].flag_prov = 1;
	if (flag)
		return 1;
	else
		return 0;
}

pole NewField() {
	pole Field;
	for (int i = 0; i< l;i++)
		for (int j = 0; j < l; j++) {
			Field.razmer[i][j].x = i;
			Field.razmer[i][j].y = j;
		}
	Field.zanyato = 0;
	return Field;
}

void SetColor(int text, int background = 7)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void sto(int x, int y) {
	COORD cPos;
	cPos.X = x;
	cPos.Y = y;
	SetConsoleCursorPosition(hConsole, cPos);
}

void CreateWindoW(int cols, int rows, int x = 0, int y = 0, int peregorodka = 0) {
	SetConsoleOutputCP(866);
	sto(x, y);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			
				if (i == 0 && j == 0) {
					cout << "\311";
				}
				else if (i == 0 && j != cols - 1) {
					cout << "\315";
				}
				else if (i == 0 && j == cols - 1)
					cout << "\273";

				if ((j == 0 || j == cols - 1) && (i != 0 && i != rows - 1))
					cout << "\272";
				if (!(j == 0 || j == cols - 1) && (i != 0 && i != rows - 1)) cout << ' ';


				if (i == rows - 1 && j == 0) {
					cout << "\310";
				}
				else if (i == rows - 1 && j != cols - 1) {
					cout << "\315";
				}
				else if (i == rows - 1 && j == cols - 1)
					cout << "\274";
			
		}
		y++;
		sto(x, y);
		
	}
	SetConsoleOutputCP(1251);
}

void COUTFrame();

void COUTField_Field(pole &a) {
	int m(1), n(2);
	sto(20, 7);
	SetConsoleOutputCP(866);
	cout << right << "\332\304\304\304\304\304\302\304\304\304\304\304\302\304\304\304\304\304\302\304"
		"\304\304\304\304\302\304\304\304\304\304\302\304\304\304\304\304\302\304\304\304\304"
		"\304\302\304\304\304\304\304\302\304\304\304\304\304\277";
	for (int i = 0; i < 9; i++) {
		sto(20, 7 + m);
		for (int j = 0; j < 10; j++) {

			switch (a.razmer[i][j].foreground) {
			case 0:
				SetColor(0);
				cout << setw(2) << setfill('\263') << " ";
				SetColor(0, 15);
				cout << " " << a.razmer[i][j].cvet << " ";
				SetColor(0);
				cout << " ";
				break;
			case 1:
				SetColor(0);
				cout << setw(2) << setfill('\263') << " ";
				SetColor(0, 14);
				cout << " " << a.razmer[i][j].cvet << " ";
				SetColor(0);
				cout << " ";
				break;
			case 2:
				SetColor(0);
				cout << setw(2) << setfill('\263') << " ";
				SetColor(0, 13);
				cout << " " << a.razmer[i][j].cvet << " ";
				SetColor(0);
				cout << " ";
				break;
			case 3:
				SetColor(0);
				cout << setw(2) << setfill('\263') << " ";
				SetColor(0, 12);
				cout << " " << a.razmer[i][j].cvet << " ";
				SetColor(0);
				cout << " ";
				break;
			case 4:
				SetColor(0);
				cout << setw(2) << setfill('\263') << " ";
				SetColor(0, 11);
				cout << " " << a.razmer[i][j].cvet << " ";
				SetColor(0);
				cout << " ";
				break;
			case 5:
				SetColor(0);
				cout << setw(2) << setfill('\263') << " ";
				SetColor(0, 10);
				cout << " " << a.razmer[i][j].cvet << " ";
				SetColor(0);
				cout << " ";
				break;
			case 6:
				SetColor(0);
				cout << setw(2) << setfill('\263') << " ";
				SetColor(0, 8);
				cout << " " << a.razmer[i][j].cvet << " ";
				SetColor(0);
				cout << " ";
				break;
			case 10:
				SetColor(0);
				cout << setw(2) << setfill('\263') << " ";
				SetColor(0, a.razmer[i][j].background);
				cout << " " << a.razmer[i][j].cvet << " ";
				SetColor(0);
				cout << " ";
				break;
			}
		}
		sto(20, 7 + n);
		cout << "\303\304\304\304\304\304\305\304\304\304\304\304\305\304\304\304\304\304\305\304"
			"\304\304\304\304\305\304\304\304\304\304\305\304\304\304\304\304\305\304\304\304\304"
			"\304\305\304\304\304\304\304\305\304\304\304\304\304\264";
		m += 2;
		n += 2;
	}
	sto(20, 25);
	cout << "\300\304\304\304\304\304\301\304\304\304\304\304\301\304\304\304\304\304\301\304"
		"\304\304\304\304\301\304\304\304\304\304\301\304\304\304\304\304\301\304\304\304\304"
		"\304\301\304\304\304\304\304\301\304\304\304\304\304\331";
	SetConsoleOutputCP(1251);

	sto(21, 28);
	cout << "                                                    ";
	sto(21, 30);
	
	
}

void COUTField(pole &a) {
	
	COUTFrame();
	CreateWindoW(55,3,20,0,0);
	sto(27, 1);
	cout << "Выход в меню - ESC";
	sto(49, 1);
	cout << "Сохранение игры - F10";
	if (FlagName) {
		CreateWindoW(11, 3, 40, 2);
		switch(NikLength) {
		case 1:
			sto(45,3);
			break;
		case 2:
			sto(45, 3);
			break;
		case 3:
			sto(44, 3);
			break;
		case 4:
			sto(44, 3);
			break;
		case 5: 
			sto(43, 3);
			break;
		}
		
		cout << name;
	}
	COUTField_Field(a);
	CreateWindoW(55, 3, 20, 27, 0);
	CreateWindoW(22, 3, 76, 4);
	sto(85, 4);
	cout << "СЧЕТ";
	sto(77, 5);
	cout <<setfill(' ')<< setw(20) << score;
	
}

bool Proverka(pole &a) {
	int s4et4ik(0);
	if (!loading) {
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if (a.razmer[i][j].flag == true) {
					for (int m = 0; m <= (9 - i) && a.razmer[i][j].foreground == a.razmer[i + m][j].foreground; m++) {

						s4et4ik++;

					}

					switch (s4et4ik) {
					case 5:
						score += 5;
						for (int p = 0; p < 5; p++) {
							a.razmer[i + p][j].cvet = 32;
							a.razmer[i + p][j].flag = false;
							a.razmer[i + p][j].foreground = 10;
							
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 5;
						s4et4ik = 0;
						return true;
						break;
					case 6:
						score += 12;
						for (int p = 0; p < 6; p++) {
							a.razmer[i + p][j].cvet = 32;
							a.razmer[i + p][j].flag = false;
							a.razmer[i + p][j].foreground = 10;
							
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 6;
						s4et4ik = 0;
						return true;
						break;
					case 7:
						score += 18;
						for (int p = 0; p < 7; p++) {
							a.razmer[i + p][j].cvet = 32;
							a.razmer[i + p][j].flag = false;
							a.razmer[i + p][j].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 7;
						s4et4ik = 0;
						return true;
						break;
					case 8:
						score += 28;
						for (int p = 0; p < 8; p++) {
							a.razmer[i + p][j].cvet = 32;
							a.razmer[i + p][j].flag = false;
							a.razmer[i + p][j].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 8;
						s4et4ik = 0;
						return true;
						break;
					case 9:
						score += 42;
						for (int p = 0; p < 9; p++) {
							a.razmer[i + p][j].cvet = 32;
							a.razmer[i + p][j].flag = false;
							a.razmer[i + p][j].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 9;
						s4et4ik = 0;
						return true;
						break;
					default:
						s4et4ik = 0;
						break;
					}


					for (int m = 0; m <= (9 - j) && a.razmer[i][j].foreground == a.razmer[i][j + m].foreground; m++) {

						s4et4ik++;

					}
					switch (s4et4ik) {
					case 5:
						score += 5;
						for (int p = 0; p < 5; p++) {
							a.razmer[i][j + p].cvet = 32;
							a.razmer[i][j + p].flag = false;
							a.razmer[i][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 5;
						s4et4ik = 0;
						return true;
						break;
					case 6:
						score += 12;
						for (int p = 0; p < 6; p++) {
							a.razmer[i][j + p].cvet = 32;
							a.razmer[i][j + p].flag = false;
							a.razmer[i][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 6;
						s4et4ik = 0;
						return true;
						break;
					case 7:
						score += 18;
						for (int p = 0; p < 7; p++) {
							a.razmer[i][j + p].cvet = 32;
							a.razmer[i][j + p].flag = false;
							a.razmer[i][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 7;
						s4et4ik = 0;
						return true;
						break;
					case 8:
						score += 28;
						for (int p = 0; p < 8; p++) {
							a.razmer[i][j + p].cvet = 32;
							a.razmer[i][j + p].flag = false;
							a.razmer[i][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 8;
						s4et4ik = 0;
						return true;
						break;
					case 9:
						score += 42;
						for (int p = 0; p < 9; p++) {
							a.razmer[i][j + p].cvet = 32;
							a.razmer[i][j + p].flag = false;
							a.razmer[i][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 9;
						s4et4ik = 0;
						return true;
						break;
					default:
						s4et4ik = 0;
						break;
					}

					for (int m = 0; m <= (9 - i) && a.razmer[i][j].foreground == a.razmer[i + m][j + m].foreground; m++)
						s4et4ik++;
					switch (s4et4ik) {
					case 5:
						score += 5;
						for (int p = 0; p < 5; p++) {
							a.razmer[i+p][j + p].cvet = 32;
							a.razmer[i+p][j + p].flag = false;
							a.razmer[i+p][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 5;
						s4et4ik = 0;
						return true;
						break;
					case 6:
						score += 12;
						for (int p = 0; p < 6; p++) {
							a.razmer[i + p][j + p].cvet = 32;
							a.razmer[i + p][j + p].flag = false;
							a.razmer[i + p][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 6;
						s4et4ik = 0;
						return true;
						break;
					case 7:
						score += 18;
						for (int p = 0; p < 7; p++) {
							a.razmer[i + p][j + p].cvet = 32;
							a.razmer[i + p][j + p].flag = false;
							a.razmer[i + p][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 7;
						s4et4ik = 0;
						return true;
						break;
					case 8:
						score += 28;
						for (int p = 0; p < 8; p++) {
							a.razmer[i + p][j + p].cvet = 32;
							a.razmer[i + p][j + p].flag = false;
							a.razmer[i + p][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 8;
						s4et4ik = 0;
						return true;
						break;
					case 9:
						score += 42;
						for (int p = 0; p < 9; p++) {
							a.razmer[i + p][j + p].cvet = 32;
							a.razmer[i + p][j + p].flag = false;
							a.razmer[i + p][j + p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 9;
						s4et4ik = 0;
						return true;
						break;
					default:
						s4et4ik = 0;
						break;
					}

					for (int m = 0; m <= (9 - i) && a.razmer[i][j].foreground == a.razmer[i + m][j - m].foreground; m++)
						s4et4ik++;
					switch (s4et4ik) {
					case 5:
						score += 5;
						for (int p = 0; p < 5; p++) {
							a.razmer[i+p][j - p].cvet = 32;
							a.razmer[i+p][j - p].flag = false;
							a.razmer[i+p][j - p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 5;
						s4et4ik = 0;
						return true;
						break;
					case 6:
						score += 12;
						for (int p = 0; p < 6; p++) {
							a.razmer[i + p][j - p].cvet = 32;
							a.razmer[i + p][j - p].flag = false;
							a.razmer[i + p][j - p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 6;
						s4et4ik = 0;
						return true;
						break;
					case 7:
						score += 18;
						for (int p = 0; p < 7; p++) {
							a.razmer[i + p][j - p].cvet = 32;
							a.razmer[i + p][j - p].flag = false;
							a.razmer[i + p][j - p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 7;
						s4et4ik = 0;
						return true;
						break;
					case 8:
						score += 28;
						for (int p = 0; p < 8; p++) {
							a.razmer[i + p][j - p].cvet = 32;
							a.razmer[i + p][j - p].flag = false;
							a.razmer[i + p][j - p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 8;
						s4et4ik = 0;
						return true;
						break;
					case 9:
						score += 42;
						for (int p = 0; p < 9; p++) {
							a.razmer[i + p][j - p].cvet = 32;
							a.razmer[i + p][j - p].flag = false;
							a.razmer[i + p][j - p].foreground = 10;
						}
						sto(77, 5);
						cout << setfill(' ') << setw(20) << score;
						a.zanyato -= 9;
						s4et4ik = 0;
						return true;
						break;
					default:
						s4et4ik = 0;
						break;
					}
				}
			}
		}
		return false;
	}
 else return true;
}

void Turn(pole &a) {
	//4 iteracii
	int x, y;
	srand(time(0));
	if (81 - a.zanyato >= 3) {
		for (int i = 0; i < 3; i++) {

			x = 0 + rand() % 9;
			y = 0 + rand() % 9;

			if (a.razmer[x][y].flag == false) {
				a.razmer[x][y].foreground = 0 + rand() % 6;
				a.razmer[x][y].flag = true;
				a.zanyato++;
			}
			else {
				while (a.razmer[x][y].flag != false) {
					x = 0 + rand() % 9;
					y = 0 + rand() % 9;
				}
				a.razmer[x][y].foreground = 0 + rand() % 6;
				a.razmer[x][y].flag = true;
				a.zanyato++;
			}
			x = 0;
			y = 0;

		}
		Proverka(a);
	}
	else {
		for (int i = 0; i < 81 - a.zanyato; i++) {
			x = 0 + rand() % 9;
			y = 0 + rand() % 9;

			if (a.razmer[x][y].flag == false) {
				a.razmer[x][y].foreground = 0 + rand() % 6;
				a.razmer[x][y].flag = true;
				a.zanyato++;
			}
			else {
				while (a.razmer[x][y].flag != false) {
					x = 0 + rand() % 9;
					y = 0 + rand() % 9;
				}
				a.razmer[x][y].foreground = 0 + rand() % 6;
				a.razmer[x][y].flag = true;
				a.zanyato++;
			}

		}
		FlagCheck = 0;
	}
}

void SaveGame(pole &a);

void Dvizenie(pole &a, bool &over) {
	
	char dir(0);
	bool flag = false;
	bool flag2 = true;
	a.razmer[Pointerx][Pointery].cvet = char(220);
	COUTField_Field(a);
	if (FlagCheck) {
		while (dir != 27 && flag2) {
			dir = _getwch();
			if (dir == -32) {
				dir = _getwch();
				switch (dir) {
				case 72:
					Pointerx0 = Pointerx;
					Pointery0 = Pointery;
					if (Pointerx > 0) {
						Pointerx--;
						a.razmer[Pointerx][Pointery].cvet = char(220);
						a.razmer[Pointerx0][Pointery0].background = 7;
						a.razmer[Pointerx0][Pointery0].cvet = ' ';

					}
					COUTField_Field(a);

					break;
				case 75:
					Pointerx0 = Pointerx;
					Pointery0 = Pointery;

					if (Pointery > 0) {
						Pointery--;
						a.razmer[Pointerx][Pointery].cvet = char(220);
						a.razmer[Pointerx0][Pointery0].background = 7;
						a.razmer[Pointerx0][Pointery0].cvet = ' ';
					}
					COUTField_Field(a);
					break;
				case 77:
					Pointerx0 = Pointerx;
					Pointery0 = Pointery;
					if (Pointery < 8) {
						Pointery++;

						a.razmer[Pointerx][Pointery].cvet = char(220);
						a.razmer[Pointerx0][Pointery0].background = 7;
						a.razmer[Pointerx0][Pointery0].cvet = ' ';
					}
					COUTField_Field(a);
					break;
				case 80:
					Pointerx0 = Pointerx;
					Pointery0 = Pointery;
					if (Pointerx < 8) {
						Pointerx++;
						a.razmer[Pointerx][Pointery].cvet = char(220);
						a.razmer[Pointerx0][Pointery0].background = 7;
						a.razmer[Pointerx0][Pointery0].cvet = ' ';
					}
					COUTField_Field(a);
					break;
				}
			}
			if (dir == 13) {
				if (a.razmer[Pointerx][Pointery].flag == true) {
					Pointerx1 = Pointerx;
					Pointery1 = Pointery;
					flag = true;
					sto(40,28);
					SetColor(0, 10);
					cout << " Шарик выбран! ";
					SetColor(0);
				}
				else {
					if (flag == true && a.razmer[Pointerx][Pointery].flag != true) {
						if (Logick(Pointerx1,Pointery1,Pointerx,Pointery,a)) {
							a.razmer[Pointerx][Pointery].cvet = ' ';
							a.razmer[Pointerx][Pointery].foreground = a.razmer[Pointerx1][Pointery1].foreground;
							a.razmer[Pointerx1][Pointery1].cvet = 32;
							a.razmer[Pointerx][Pointery].flag = true;
							a.razmer[Pointerx1][Pointery1].flag = false;
							flag = false;
							COUTField_Field(a);
							flag2 = false;
							a.razmer[Pointerx][Pointery].background = 7;
							a.razmer[Pointerx1][Pointery1].foreground = 10;

							//sto(1, 30);
							//cout << setw(57) << setfill(' ') << " ";
						}
						else {
							sto(34, 28);
							SetColor(15, 12);
							cout << " В данную клетку не попасть! ";
							SetColor(0);
						}
					}
					else {
						sto(23, 28);
						SetColor(15, 12);
						cout << " В выбранной клетке нет шарика. Повторите выбор! ";
						SetColor(0);
					}

				}
			}
			if (dir == 0) {
				dir = _getwch();
				if (dir == 68)
					SaveGame(a);
			}
		}
		if (dir == 27)
			over = false;
	}
	else 
	GameOver = 0;
}

void CreateField(pole &a) {
	a = NewField();
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) {
			a.razmer[i][j].cvet = 32;
		}
	
}

void ClearField(pole &a) {
	for (int i = 0; i < l-1;i++)
		for (int j = 0; j < l-1; j++) {
			a.razmer[i][j].flag = false;
			a.razmer[i][j].cvet = 32;
			a.razmer[i][j].foreground = 10;
			a.razmer[i][j].background = 7;
		}
	a.zanyato = 0;
}

void AddHighScore(int a, int index);

void GameOverr() {
	
	int index = -1;
	int choose;
	int k = 1;
	bool FlagChoose = 1;
	//CreateWindoW();
	CreateWindoW(43, 5, 26, 13);
	sto(39, 15);
	cout << "Игра завершена!";
	Sleep(1500);
	for (int i = 4; i > -1; i--) {
		if (score >= scores[i] && score != 0) {
			index = i;
		}
	}
	if (index != -1) {
		CreateWindoW(43, 8, 26, 13);
		sto(28, 15);
		cout << "Записать  ваш счет  в таблицу рекордов?";
		sto(39, 18);
		cout << " Да ";
		sto(52, 18);
		SetColor(0, 8);
		cout << " Нет ";
		SetColor(0);
		while (FlagChoose) {
			choose = _getwch();
			if (choose == 224) {
				choose = _getwch();
				switch (choose) {
				case 75:
					if (k == 1) {
						k--;
						sto(39, 18);
						SetColor(0, 8);
						cout << " Да ";
						sto(52, 18);
						SetColor(0);
						cout << " Нет ";
					}
					break;
				case 77:
					if (k == 0) {
						k++;
						sto(39, 18);
						cout << " Да ";
						sto(52, 18);
						SetColor(0, 8);
						cout << " Нет ";
						SetColor(0);
					}
					break;
				}
			}
			else if (choose == 13) {
				switch (k) {
				case 0:
					AddHighScore(score,index);
				case 1:
					FlagChoose = 0;
					system("cls");
					break;
				}
			}
		}
	}
	
}

void EnterName() {
	COUTFrame();
	int k = 0;
	NikLength = 0;
	FlagName = 0;
	int enter = 0;
	CreateWindoW(45, 8, 26, 7);
	sto(30, 10);
	cout << "Введите ваше имя (max 5):  ";
	sto(58, 11);
	cout << "_____";
	sto(58, 10);
	while (enter != 13) {
		enter = _getwch();
		if (enter == 8 && NikLength > 0) {
			cout << char(enter);
			cout << ' ';
			cout << char(enter);
			NikLength--;
			name[NikLength] = ' ';
		}
		else
			if (NikLength < 5 && enter != 8 && ((enter >= 48&&enter <=57) || (enter >= 65 && enter <= 90) || (enter >= 97 && enter <= 122))) {
				cout << char(enter);
				name[NikLength] = char(enter);
				NikLength++;
			}
	}
	if (NikLength != 0)
		FlagName = true;
	system("cls");
}

void Game(pole &a) {	
	int in = 0;	
	bool check = 1;
	if (!loading) {
		EnterName();
	}
	COUTField(a);
		do {

			if (!Proverka(a)) {	
				check = 1;
				Turn(a); //xod esli wary ne ubralis'
				COUTField_Field(a);
				Dvizenie(a, GameOver);				
			}
			else {
				if (check) {
					check = 0;
					COUTField_Field(a);
					Dvizenie(a, GameOver);
					loading = false;
				}
			}
		} while (a.zanyato < 80 && GameOver);
		GameOverr();
		score = 0;
		GameOver = true;
		for (int i = 0; i < 10; i++)
			name[i] = '\0';
		ClearField(a);
}

void Rules() {
	COUTFrame();
	CreateWindoW(55, 3, 24, 0, 0);
	sto(43, 1);
	cout << "Выход в меню - ESC";
	
	int esc = 0;
	sto(44, 7);
	cout << "Правила";
	sto(4, 9);
	cout  << "На поле размером  9x9  (стандартный размер)  каждый  ход появляются  3  шара  разных  цветов\n";
		sto(4, 10);
		cout << "(всего семь цветов).  Из  них  нужно  составлять  линии  одного  цвета  в  пять и более штук\n";
		sto(4, 11);
		cout << "по горизонтали, вертикали  или  диагонали. За  один ход можно переместить  только  один  шар\n";
		sto(4, 12); 
		cout << "и путь между начальной и конечной позициями должен быть свободен. Путь считается  свободным,\n";
		sto(4, 13); 
		cout << "если состоит из одного или нескольких  перемещений  шара  на  одну  клетку  по  вертикали или";
		sto(4, 14);
		cout << "горизонтали,  но  не  диагонали.  Если  после перемещения шара образуется  линия одного цвета";
		sto(4, 15);
		cout << "длиной 5 и более шаров, то она уничтожается, игроку начисляются очки (10 за 5 шаров, 12 за 6,";
		sto(4, 16);
		cout << "18 за 7, 28 за 8, 42 за 9) и появление трёх следующих шаров откладывается до следующего хода.";
		sto(4, 17);
		cout << "Цель игры - набрать как можно больше очков и продержаться как можно больше ходов.";
		
		while (esc != 27) 
			esc = _getwch();
		
}

void LoadHighscore() {
	ifstream in;
	char x = 0;
	int j = 0;
	in.open("HighScores.txt");
	if (in.is_open()) {
		for (int i = 0; i < 5; i++) {
			in >> x;
			while (x != '.') {
				niknames[i][j] = x;
				in >> x;
				j++;
			}
			in >> scores[i];
			j = 0;
			in >> x;
		}
	}
	in.close();
}

void SaveHighscore() {
	ofstream of;
	of.open("HighScores.txt");
	if (of.is_open()) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				of << niknames[i][j];
			}
			of << '.';
			of << scores[i];
			of << '.';
		}
	}
	of.close();
}

void COUTHighscore() {
	ifstream in;
	system("cls");
	COUTFrame();
	CreateWindoW(62, 3, 20, 0, 0);
	sto(24, 1);
	cout << "Выход в меню - ESC";
	sto(46, 1);
	cout << "Очистить лучшие результаты - Del";
	sto(41, 8);
	cout << "ЛУЧШИЕ РЕЗУЛЬТАТЫ";
	CreateWindoW(25, 15, 37, 10);
	for (int i = 0; i < 5; i++) {
		sto(44, 13 + 2 * i);
		cout << i + 1 << ". ";
		if (scores[i] > 1) {
			for (int j = 0; j < 5; j++)
				cout << niknames[i][j];
			cout << "  " << right<< scores[i];
		}
	}
}

void ClearHighscore();

void HighScore() {
	LoadHighscore();
	int trigger = 0;
	int choose = 0;
	int k = 1;
	bool flagHighScore = 1;
	
	int esc = 0;
	
	COUTHighscore();
	do {
		trigger = _getwch();
		if (trigger == 224) {
			trigger = _getwch();
			if (trigger == 83) {
				flagHighScore = 1;
				CreateWindoW(50,8,26,13);
				sto(33, 14);
				cout << "Вы действительно хотите очистить все?";
				sto(36, 16);
				cout << "ДАННОЕ ДЕЙСТВИЕ НЕ ОТМЕНИТЬ!";
				sto(45,18);
				cout << " Да ";
				sto(52, 18);
				SetColor(0, 8);
				cout << " Нет ";
				SetColor(0);
				while (flagHighScore) {
					choose = _getwch();
					if (choose == 224) {
						choose = _getwch();
						switch (choose) {
						case 75:
							if (k == 1) {
								k--;
								sto(45, 18);
								SetColor(0, 8);
								cout << " Да ";
								sto(52, 18);
								SetColor(0);
								cout << " Нет ";								
							}
							break;
						case 77:
							if (k == 0) {
								k++;
								sto(45, 18);
								cout << " Да ";
								sto(52, 18);
								SetColor(0, 8);
								cout << " Нет ";
								SetColor(0);
							}
							break;
						}
					}
					else if (choose == 13) {
						switch (k) {
						case 0:
							ClearHighscore();
							CreateWindoW(50, 8, 26, 13);
							sto(40, 16);
							cout << "Результаты очищены!";
							Sleep(1000);
						case 1:
							system("cls");
							flagHighScore = 0;
							COUTHighscore();
							k = 1;
							break;
						}
					}
				}
			}
		}///22483  77   75

	} while (trigger != 27);
	SaveHighscore();
	system("cls");
}

void AddHighScore(int a, int index) {
	int v = 0;
	if (!FlagName) {
		system("cls");
		while (!FlagName) {
			EnterName();
		}
	}
	switch (index) {
	case 4:
		scores[4] = a;
		for (int j = 0; j < 0; j++)
			niknames[index][j] = name[j];
		break;
	case 3:
		scores[4] = scores[3];
		scores[3] = a;
		for (int j = 0; j < 5; j++) {
			niknames[4][j] = niknames[3][j];
			niknames[index][j] = name[j];
		}
		break;
	case 2:
		scores[4] = scores[3];
		scores[3] = scores[2];
		scores[2] = a;
		for (int j = 0; j < 5; j++) {
			niknames[4][j] = niknames[3][j];
			niknames[3][j] = niknames[2][j];
			niknames[index][j] = name[j];
		}
		break;

	case 1:
		scores[4] = scores[3];
		scores[3] = scores[2];
		scores[2] = scores[1];
		scores[1] = a;
		for (int j = 0; j < 5; j++) {
			niknames[4][j] = niknames[3][j];
			niknames[3][j] = niknames[2][j];
			niknames[2][j] = niknames[1][j];
			niknames[index][j] = name[j];
		}
		break;
	case 0:
		scores[4] = scores[3];
		scores[3] = scores[2];
		scores[2] = scores[1];
		scores[1] = scores[0];
		scores[0] = a;
		for (int j = 0; j < 5; j++) {
			niknames[4][j] = niknames[3][j];
			niknames[3][j] = niknames[2][j];
			niknames[2][j] = niknames[1][j];
			niknames[1][j] = niknames[0][j];
			niknames[index][j] = name[j];
		}
		break;
	}
	SaveHighscore();
}

void ClearHighscore() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0;j<5;j++)
		niknames[i][j] = '\0';
		scores[i] = 1;
	}
	SaveHighscore();
}

void SaveGame(pole &a) {
	ofstream out;
	out.open("SaveGame.txt");
	if (out.is_open()) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 10; j++) {
				out<<" " <<a.razmer[i][j].foreground;
			}
			
		}
		out <<" "<< score;
		out <<name<<'\n';
		out << " " << a.zanyato;
		out << " " << NikLength;
		out << " " << FlagName;
		CreateWindoW(25, 3, 35, 8);
		sto(40, 9);
		cout << "Игра сохранена";
		Sleep(1000);
	
		COUTField(a);
	}
	else {
		CreateWindoW(25, 3, 35, 8);
		sto(40, 9);
		cout << "Ошибка сохранения!";
	}
	out.close();
}

void LoadGame(pole &a) {
	ifstream in;
	score = 0;
	int x = 0;
	in.open("SaveGame.txt");
	if (in.is_open() && !in.eof()) {
		for(int i = 0;i<9;i++)
			for (int j = 0; j < 10; j++) {
				in >> x;
				if (x != 10) {
					a.razmer[i][j].foreground = x;
					a.razmer[i][j].flag = true;
					
				}
				if (a.razmer[i][j].background == 8) {
					a.razmer[i][j].background = 7;
				}
			}
		in >> score;
		in.getline(name,10);
		in >> a.zanyato;
		in >> NikLength;
		in >> FlagName;
	}
	
	loading = true;
	in.close();
}

void COUTFrame() {
	SetConsoleOutputCP(866);
	sto(0, 0);
	cout << "\311\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315"
		"\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315"
		"\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\273";
	sto(0, 34);
	cout << "\310\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315"
		"\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315"
		"\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\274";
	for (int i = 1; i < 34; i++) {
		sto(0, i);
		cout << "\272";
		sto(99, i);
		cout << "\272";
	}
	SetConsoleOutputCP(1251);
}

bool Menu1(pole &a) {
	bool flag(1);
	int choose;
	int k = 0;
	int esc = 0;
	bool flag2 = 0;
	sto(42, 10);	
	cout << "LINES";
	sto(40, 14);
	SetColor(0, 8);
	cout << " Новая игра ";
	sto(40, 16);
	SetColor(0);
	cout << " Загрузить игру ";
	sto(40, 18);
	cout << " Правила ";
	sto(40, 20);
	cout << " Таблица лидеров ";
	sto(40, 22);
	cout << " Выход из игры \n";
	while (flag) {
		if (flag2) {
			COUTFrame();
			sto(42, 10);
			cout << "LINES";
			sto(40, 14);
			SetColor(0, 8);
			cout << " Новая игра ";
			sto(40, 16);
			SetColor(0);
			cout << " Загрузить игру ";
			sto(40, 18);
			cout << " Правила ";
			sto(40, 20);
			cout << " Таблица лидеров ";
			sto(40, 22);
			cout << " Выход из игры \n";
			flag2 = 0;
		}

		choose = _getwch();
		if (choose == 224) {
			choose = _getwch();
			switch (choose) {
			case 80:	
				k++;
				switch (k) {
				case 1:
					sto(40, 14);
					SetColor(0);
					cout << " Новая игра ";					
					SetColor(0, 8);
					sto(40, 16);
					cout << " Загрузить игру ";
					SetColor(0);
					sto(40, 18);
					cout << " Правила ";
					sto(40, 20);
					cout << " Таблица лидеров ";
					sto(40, 22);
					cout << " Выход из игры \n";
					break;
				case 2:
					sto(40, 14);
					SetColor(0);
					cout << " Новая игра ";					
					sto(40, 16);
					cout << " Загрузить игру ";
					SetColor(0, 8);
					sto(40, 18);
					cout << " Правила ";
					sto(40, 20);
					SetColor(0);
					cout << " Таблица лидеров ";
					sto(40, 22);
					cout << " Выход из игры \n";
					break;

				case 3:
					sto(40, 14);
					SetColor(0);
					cout << " Новая игра ";					
					sto(40, 16);
					cout << " Загрузить игру ";
					SetColor(0);
					sto(40, 18);
					cout << " Правила ";
					sto(40, 20);
					SetColor(0, 8);
					cout << " Таблица лидеров ";
					sto(40, 22);
					SetColor(0);
					cout << " Выход из игры \n";
					break;
				case 4:
					sto(40, 14);
					SetColor(0);
					cout << " Новая игра ";					
					sto(40, 16);
					cout << " Загрузить игру ";					
					sto(40, 18);
					cout << " Правила ";
					sto(40, 20);
					cout << " Таблица лидеров ";
					sto(40, 22);
					SetColor(0, 8);
					cout << " Выход из игры \n";
					SetColor(0);
					break;
				default:
					k = 4;
					break;
				}
				break;			
			case 72:
				k--;
				switch (k) {
				case 3:
					sto(40, 14);
					SetColor(0);
					cout << " Новая игра ";
					sto(40, 16);
					cout << " Загрузить игру ";
					sto(40, 18);
					cout << " Правила ";
					sto(40, 20);
					SetColor(0, 8);
					cout << " Таблица лидеров ";
					sto(40, 22);
					SetColor(0);
					cout << " Выход из игры \n";					
					break;
				case 2:
					sto(40, 14);
					SetColor(0);
					cout << " Новая игра ";
					sto(40, 16);
					cout << " Загрузить игру ";
					sto(40, 18);
					SetColor(0, 8);
					cout << " Правила ";
					sto(40, 20);
					SetColor(0);
					cout << " Таблица лидеров ";
					sto(40, 22);					
					cout << " Выход из игры \n";
					break;
				case 1:
					sto(40, 14);
					SetColor(0);
					cout << " Новая игра ";
					sto(40, 16);
					SetColor(0, 8);
					cout << " Загрузить игру ";
					sto(40, 18);
					SetColor(0);
					cout << " Правила ";
					sto(40, 20);					
					cout << " Таблица лидеров ";
					sto(40, 22);
					cout << " Выход из игры \n";
					break;
				case 0:
					sto(40, 14);
					SetColor(0, 8);
					cout << " Новая игра ";
					sto(40, 16);
					SetColor(0);
					cout << " Загрузить игру ";
					sto(40, 18);					
					cout << " Правила ";
					sto(40, 20);
					cout << " Таблица лидеров ";
					sto(40, 22);
					cout << " Выход из игры \n";
					break;
				default:
					k = 0;
				}
				break;
			}
		}
		if (choose == 13) {
			switch (k) {
			case 0:
				system("cls");
				COUTFrame();
				Game(a);
				system("cls");
				flag2 = 1;
				k = 0;
				ClearField(a);
				break;
			case 1:
				LoadGame(a);
				Game(a);
				system("cls");
				flag2 = 1;
				k = 0;
				break;
			case 2:
				system("cls");
				Rules();
				system("cls");
				flag2 = 1;
				k = 0;
				esc = 0;
				ClearField(a);
				break;
			case 3:
				HighScore();
				system("cls");
				flag2 = 1;
				k = 0;
				break;
			case 4:
				return 1;
				break;
			}
		}
	}
}

int ascii() {
	int g;
	for (int i = 0; i < 5; i++) {
		g = _getwch();
		cout << g;
	}
	return 0;
}

void Setup() {
	hidecursor();
	HWND consoleWindow = GetConsoleWindow(), hWnd = GetForegroundWindow();
	//EnableWindow(hWnd, false); 
	RECT r;
	GetWindowRect(consoleWindow, &r);
	int x = GetSystemMetrics(SM_CXSCREEN) / 2 - (r.right - r.left) / 2;
	int y = GetSystemMetrics(SM_CYSCREEN) / 2 - (r.bottom - r.top) / 2;
	SetWindowPos(consoleWindow, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	HMENU hm;
	hm = GetSystemMenu(consoleWindow, FALSE);
	RemoveMenu(hm, SC_SIZE, MF_BYCOMMAND | MF_REMOVE);
	RemoveMenu(hm, SC_MAXIMIZE, MF_BYCOMMAND | MF_REMOVE);
	DrawMenuBar(consoleWindow);
	HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
	DWORD dwPreviousMode = 0;
	GetConsoleMode(hConsole, &dwPreviousMode);
	DWORD dwNewMode = dwPreviousMode | ENABLE_MOUSE_INPUT;
	dwNewMode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hConsole, dwNewMode);



	system("color 70");
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	GetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
	wcscpy_s(fontInfo.FaceName, L"Lucida Console");
	fontInfo.dwFontSize.Y = 18;
	SetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
	ShowCursor(FALSE);


}

int main() {
	system("Title Lines 2019");
	system("mode con cols=100 lines=35");
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
	Setup();
	pole Polyana;
	srand(time(0));
	CreateField(Polyana);
	COUTFrame();	
	Menu1(Polyana);
	SaveHighscore();
}

