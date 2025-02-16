#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include "WspanialyString.h"
#include "Stack.h"
using namespace std;

WspanialyString* MAX = new WspanialyString("MAX");
WspanialyString* MIN = new WspanialyString("MIN");
WspanialyString* IF = new WspanialyString("IF");
WspanialyString* N = new WspanialyString("N");

WspanialyString* IfFunction();
WspanialyString* NFunction();
WspanialyString* MinMaxFunction(WspanialyString* funcType);

bool isNormalOperator(char character) //sprawdza czy znak jest klasycznym operatorem
{
	if (character == '+' || character == '-' || character == '/' || character == '*') return true;
	else return false;
}

bool isFunction(WspanialyString* temp) //sprawdza czy znak jest funkcja
{
	if (temp->operator==(*MAX) || temp->operator==(*MIN) || temp->operator==(*IF) || temp->operator==(*N)) return true;
	else return false;
}

int priority(char character) //sprawdza priorytet operatora
{
	if (character == '+' || character == '-') return 1;
	else if (character == '*' || character == '/') return 2;
	else return 0;
}

void FunctionsSemaphore(WspanialyString* temp, WspanialyString* output) //odsyla do odpowiedniej funkcji
{
	WspanialyString* Str = new WspanialyString("");
	if (temp->operator==(*MAX) || temp->operator==(*MIN)) Str->appendText(MinMaxFunction(temp)->getText());
	else if (temp->operator==(*IF)) Str->appendText(IfFunction()->getText());
	else if (temp->operator==(*N)) Str->appendText(NFunction()->getText());
	*temp = ""; //czyszczenie zmiennej
	output->appendText(Str->getText()); //dodajemy do wyniku
	delete Str; //usuwamy zmienna
}

WspanialyString* func_intToWS(int a) //rzutuje int na WspanialyString
{
	int length = 0;
	int temp = a;
	if (temp == 0) length = 1; // obsluga zera
	else 
	{
		if (temp < 0) 
		{
			length++; // dodatkowy znak na minus
			temp = -temp; //zamiana na dodatnia liczbe
		}
		while (temp > 0) 
		{
			temp /= 10;
			length++;
		}
	}
	temp = a;
	// tworzenie tablicy znakow na liczbe
	char* result = new char[length + 1];
	if (a < 0) 
	{
		result[0] = '-'; // pierwszy znak
		temp = -temp; // zamiana na dodatnia liczbe
	}
	int limit;
	if (a < 0) limit = 1; // obsluga minusa
	else limit = 0;
	for (int i = length - 1; i >= limit; i--) 
	{
		result[i] = temp % 10 + '0';
		temp /= 10;
	}
	result[length] = '\0'; // koniec stringa
	WspanialyString* str = new WspanialyString(result);
	delete[] result;
	return str;
}

void WSFromStack(Stack* stack, WspanialyString* temp) //pobiera ze stosu i dodaje do wyniku
{
	WspanialyString* dataFromStack = new WspanialyString(stack->peek()->getText()); //pobieramy z wierzcholka stosu
	if (temp->getChar(-1) != ' ') temp->appendChar(' ');
	temp->appendText(dataFromStack->getText()); //dodajemy do wyniku
	temp->appendChar(' '); //dodajemy spacje
	delete dataFromStack; //usuwamy zmienna
}

WspanialyString* MinMaxFunction(WspanialyString* funcType) //obsluga funkcji MIN i MAX - ETAP 1
{
	WspanialyString* MinMaxoutput = new WspanialyString("");
	WspanialyString* MinMaxtemp = new WspanialyString("");
	Stack* MinMaxStack = new Stack();
	int brackets = 0; //obsluga wyrazen w nawiasie
	int countParams = 1; //zliczanie parametrow funkcji
	char ch = getchar();
	while (!(ch == ')' && brackets == 1)) //ochrona przed wyrazeniami w nawiasie, kontroluje poprawny moment wyjscia z funkcji
	{		
		if (ch != ' ')
		{
			if (ch == '(')
			{
				brackets++; //dodajemy liczbe nawiasow - zabezpieczenie przed wyjsciem z funkcji
				WspanialyString* dataToStack = new WspanialyString("("); 
				MinMaxStack->push(dataToStack); //wpychamy na stos 
			}
			else if (ch == ')')
			{
				brackets--; //odejmujemy liczbe nawiasow
				while (MinMaxStack->peek()->getChar(0) != '(') //usuwamy ze stosu do otwierajacego nawiasu
				{
					WSFromStack(MinMaxStack, MinMaxoutput);
					MinMaxStack->chuff();
				}
				MinMaxStack->chuff(); //usuwamy nawias otwierajacy, nie jest juz potrzebny
			}
			else if (ch == ',')
			{
				countParams++; //zliczamy parametry
				while (MinMaxStack->peek()->getChar(0) != '(') //usuwamy ze stosu do otwierajacego nawiasu
				{
					WSFromStack(MinMaxStack, MinMaxtemp);
					MinMaxStack->chuff();
				}
			}
			else if (isNormalOperator(ch)) //sprawdzamy czy ch jest zwyklym operatorem
			{
				for (int i = 0; i < MinMaxStack->stackLength(); i++) 
				{
					if (*MinMaxStack->getIndex(i)->getText() == '(') break;
					if (priority(ch) <= priority(*MinMaxStack->getIndex(i)->getText())) //wyrzucamy wszystkie operatory o wyzszym/rownym priorytecie co nasz znak do konca stosu (lub do nawiasu otwierajacego jesli taki mamy)
					{
						WSFromStack(MinMaxStack, MinMaxtemp);
						MinMaxStack->deleteIndex(i);
						i--;
					}
				}
				WspanialyString* dataToStack = new WspanialyString("");
				dataToStack->appendChar(ch);
				MinMaxStack->push(dataToStack); //wpychamy nowy operator na stos
			}
			else MinMaxtemp->appendChar(ch);
		}
		else
		{
			if (isFunction(MinMaxtemp)) //jezeli temp jest ktoras z funkcji MIN, MAX, IF lub N
			{
				FunctionsSemaphore(MinMaxtemp, MinMaxoutput); //semafor odsylajacy do danej funkcji - dzialanie rekurencyjne
			}
			else
			{
				char t = MinMaxoutput->getChar(-1);
				if (t != ' ' && MinMaxoutput->getText() != "") MinMaxoutput->appendChar(ch); //dodajemy spacje jesli jest potrzebna (naprawienie ewentualnego bledu)
				MinMaxoutput->appendText(MinMaxtemp->getText()); //dodajemy temp do wyniku
			}
			*MinMaxtemp = ""; //czyszczenie temp
		}
		ch = getchar(); //pobieramy kolejny znak
	}
	//zakonczenie - czyszczenie stosu
	while (MinMaxStack->stackLength() > 1)
	{
		WSFromStack(MinMaxStack, MinMaxoutput); 
		MinMaxStack->chuff();
	}
	MinMaxStack->chuff(); //ostatnim znakiem jest nawias otwierajacy, nie potrzebujemy go
	WspanialyString* howMany = new WspanialyString(funcType->getText());
	WspanialyString* count = func_intToWS(countParams);
	howMany->appendText(count->getText()); //tworzenie outputu w wersji funkcja+ilosc_parameterow
	MinMaxoutput->appendChar(' ');
	MinMaxoutput->appendText(howMany->getText()); 
	MinMaxoutput->deleteChar(0);
	delete howMany;
	delete count;
 	delete MinMaxtemp;
	delete MinMaxStack;
	return MinMaxoutput;
}

WspanialyString* IfFunction()
{
 	WspanialyString* ifoutput = new WspanialyString("");
	WspanialyString* iftemp = new WspanialyString("");
	Stack* ifStack = new Stack();
	int brackets = 0; //obsluga wyrazen w nawiasie
	char ch = getchar();
	while (!(ch == ')' && brackets == 1))
	{
		if (ch != ' ')
		{
			if (ch == '(')
			{
				brackets++; //dodajemy liczbe nawiasow
				WspanialyString* dataToStack = new WspanialyString("(");
				ifStack->push(dataToStack);
			}
			else if (ch == ')')
			{
				brackets--; //odejmujemy liczbe nawiasow
				while (ifStack->peek()->getChar(0) != '(') //usuwamy ze stosu do otwierajacego nawiasu
				{
					WSFromStack(ifStack, ifoutput);
					ifStack->chuff();
				}
				ifStack->chuff(); //otwierajacy nawias, nie potrzebujemy go
			}
			else if (ch == ',')
			{
				while (ifStack->peek()->getChar(0) != '(') //usuwamy ze stosu do otwierajacego nawiasu
				{
					WSFromStack(ifStack, iftemp);
					ifStack->chuff();
				}
			}
			else if (isNormalOperator(ch)) //normalny operator
			{
				for (int i = 0; i < ifStack->stackLength(); i++) //usuwamy operatory o wyzszym priorytecie ze stosu do konca lub do otwierajacego nawiasu
				{
					if (*ifStack->getIndex(i)->getText() == '(') break;
					if (priority(ch) <= priority(*ifStack->getIndex(i)->getText())) 
					{
						WSFromStack(ifStack, iftemp);
						ifStack->deleteIndex(i);
						i--;
					}
				}
				WspanialyString* dataToStack = new WspanialyString("");
				dataToStack->appendChar(ch); //dodajemy operator na stos
				ifStack->push(dataToStack);
			}
			else iftemp->appendChar(ch); //dodajemy znak do temp
		}
		else
		{
			if (isFunction(iftemp)) FunctionsSemaphore(iftemp, ifoutput); //jezeli temp jest funkcja to przechodzimy do funkcji
			else
			{
				char t = ifoutput->getChar(-1);
				if (t != ' ' && ifoutput->getText() != "") ifoutput->appendChar(ch); //dodajemy spacje jesli potrzeba
				ifoutput->appendText(iftemp->getText()); //dodajemy temp do wyniku
			}
			*iftemp = "";
		}
		ch = getchar(); //pobieramy kolejny znak
	}
	//zakonczenie - czyszczenie stosu
	while (ifStack->stackLength() > 1)
	{
		WSFromStack(ifStack, ifoutput);
		ifStack->chuff();
	}
	ifStack->chuff(); //usuwamy znak otwierajacego nawiasu
	if (ifoutput->getChar(-1) != ' ') ifoutput->appendChar(' '); //dodajemy spacje jesli potrzeba
	ifoutput->appendText(IF->getText());
	ifoutput->appendChar(' '); 
	ifoutput->deleteChar(0);
	delete ifStack;
	delete iftemp;
	return ifoutput;
}

WspanialyString* NFunction()
{
	WspanialyString* noutput = new WspanialyString("");
	WspanialyString* ntemp = new WspanialyString("");
	Stack* nStack = new Stack();
	int brackets = 0; //obsluga wyrazen w nawiasie
	char ch = getchar();
	if (ch == '(') //opcja 1 - N z czyms w nawiasach
	{
		while (!(ch == ')' && brackets == 1))
		{
			if (ch != ' ')
			{
				if (ch == '(')
				{
					brackets++; //dodajemy liczbe nawiasow
					WspanialyString* dataToStack = new WspanialyString("(");
					nStack->push(dataToStack); //wrzucamy nawias na stos
				}
				else if (ch == ')')
				{
					brackets--; //odejmujemy liczbe nawiasow
					while (nStack->peek()->getChar(0) != '(') //usuwamy parametry do otwierajacego nawiasu
					{
						WSFromStack(nStack, noutput);
						nStack->chuff();
					}
					nStack->chuff(); //usuwamy otwierajacy nawias
				}
				else if (isNormalOperator(ch)) //sprawdzamy czy normlany operator
				{
					for (int i = 0; i < nStack->stackLength(); i++) //usuwamy operatory o wyzszym priorytecie ze stosu do konca lub do otwierajacego nawiasu
					{
						if (*nStack->getIndex(i)->getText() == '(') break;
						if (priority(ch) <= priority(*nStack->getIndex(i)->getText())) 
						{
							WSFromStack(nStack, noutput);
							nStack->deleteIndex(i);
							i--;
						}
					}
					WspanialyString* dataToStack = new WspanialyString("");
					dataToStack->appendChar(ch); //wrzucamy operator na stos
					nStack->push(dataToStack);
				}
				else ntemp->appendChar(ch); //dodajemy znak do temp
			}
			else
			{
				if (isFunction(ntemp)) FunctionsSemaphore(ntemp, noutput); //jezeli temp jest funkcja to przechodzimy do funkcji
				else
				{
					noutput->appendText(ntemp->getText()); //dodajemy temp do wyniku
					char t = noutput->getChar(-1);
					if (t != ' ' && noutput->getText() != "") noutput->appendChar(ch); //dodajemy spacje jesli potrzeba
					*ntemp = ""; //czyscimy temp
				}
			}
			ch = getchar();
		}
		//zakonczenie - czyszczenie stosu
		while (nStack->stackLength() > 1)
		{
			WSFromStack(nStack, noutput);
			nStack->chuff();
		}
		noutput->appendText(N->getText());
		noutput->appendChar(' ');
		noutput->deleteChar(0);	//usuniecie poczatkowej spacji powstalej w wyniku 
	}
	else //opcja 2 - N bez nawiasow
	{ 
		while (ch != ' ') //do napotkania spacji dodajemy parametr ktory chcemy zanegowac
		{
			ntemp->appendChar(ch);
			ch = getchar();
		}
		if (isFunction(ntemp)) FunctionsSemaphore(ntemp, noutput); //jesli temp jest funkcja to odsylamy do funkcji
		else
		{
			noutput->appendText(ntemp->getText()); //dodajemy temp do wyniku
			char t = noutput->getChar(-1);
			if (t != ' ' && t != '\0') noutput->appendChar(ch); //dodajemy spacje jesli potrzeba
		}
		*ntemp = ""; //czyscimy temp
		if (noutput->getChar(-1) != ' ' && noutput->getChar(-1) != '\0') noutput->appendChar(' '); //dodajemy spacje jesli potrzeba
		noutput->appendText(N->getText());
		noutput->appendChar(' ');
	}
	nStack->chuff();
	delete ntemp;
	delete nStack;
	return noutput;
}

//funkcje przeliczajace

int func_Max(int param, WspanialyString* tofind[])  //zwroc najwieksza wartosc
{
	int max = atoi(tofind[0]->getText());
	for (int i = 1; i < param; i++)
	{
		if (atoi(tofind[i]->getText()) > max) max = atoi(tofind[i]->getText());
	}
	return max;
}

int func_Min(int param, WspanialyString* tofind[]) //zwroc najmniejsza wartosc
{
	int min = atoi(tofind[0]->getText());
	for (int i = 1; i < param; i++)
	{
		if (atoi(tofind[i]->getText()) < min) min = atoi(tofind[i]->getText());
	}
	return min;
}

WspanialyString* ConvertToONP() //ETAP 1 - konwersja na ONP
{
	bool over = false; //flaga zakonczenia odczytu
	Stack* stack = new Stack();
	WspanialyString* output = new WspanialyString("");
	WspanialyString* temp = new WspanialyString("");
	char s = getchar(); //pobranie znaku nowej linii, nieprzydatny
	while (!over)
	{
		s = getchar(); //pobranie nowego znaku
		while (s == '\n') s = getchar(); //jesli znak nowej linii to pobierz nastepny
		while (s != '.') //dopoki nie koniec
		{
			if (s != ' ') //jesli nie spacja - operujemy na pojedynczych znakach, tempie i stosie
			{
				if (isNormalOperator(s)) //sprawdzamy czy zwykly operator
				{
					for (int i = 0; i < stack->stackLength(); i++) //usuwamy operatory o wyzszym priorytecie ze stosu do konca lub do otwierajacego nawiasu
					{
						if (*stack->getIndex(i)->getText() == '(') break;
						if (priority(s) <= priority(*stack->getIndex(i)->getText())) 
						{
							WSFromStack(stack, output);
							stack->deleteIndex(i);
							i--;
						}
					}
					WspanialyString* dataToStack = new WspanialyString("");
					dataToStack->appendChar(s); //wrzucamy operator na stos
					stack->push(dataToStack);
				}
				else if (s == '(') //jesli nawias otwierajacy
				{
					WspanialyString* dataToStack = new WspanialyString("(");
					stack->push(dataToStack);
				}
				else if (s == ')') //jesli nawias zamykajacy
				{
					while (stack->peek()->getChar(0) != '(') //sciagamy ze stosu do otwierajacego nawiasu
					{
						WSFromStack(stack, output);
						stack->chuff();
					}
					stack->chuff(); //znak otwierajacego nawiasu, niepotrzebny
				}
				else temp->appendChar(s); //dodajemy znak do temp
			}
			else //jest spacja, operujemy na tym co zebralismy do tempa
			{
				if (isFunction(temp)) FunctionsSemaphore(temp, output); //jesli temp jest funkcja to odsylamy do funkcji
				else
				{
					char t = output->getChar(-1);
					if (t != ' ' && t != '\0') output->appendChar(s); //dodajemy spacje jesli potrzeba
					if (temp->getText() != "" && temp->getText() != " " && temp->getText() != "\0") output->appendText(temp->getText()); //dodajemy temp do wyniku
				}
				*temp = ""; //czyscimy temp
			}
			s = getchar(); //pobieramy nowy znak
		}
		//zakonczenie - czyszczenie stosu
		while (stack->stackLength() > 0)
		{
			WSFromStack(stack, output);
			stack->chuff();
		}
		//ewentualna naprawa outputu - po 1 spacji pomiedzy parametrami
		for (int i = 0; i < output->getLength() - 1; i++)
		{
			if (output->getChar(i) == ' ')
			{
				if (output->getChar(i + 1) == ' ')
				{
					output->deleteChar(i);
					i--;
				}
			}
		}
		*temp = "";
		output->deleteChar(output->getLength() - 1); //usuniecie spacji po ostatnim dodanym tempie
		output->printStr();
		over = true;
	}
	delete temp;
	delete stack;
	return output;
}


void Calculate_ONP(WspanialyString* output) //ETAP 2 - przeliczanie ONP
{
	WspanialyString* temp = new WspanialyString("");
	Stack* stack = new Stack();
	bool errorflag = false; //flaga bledu
	int iterator = 0;
	while (iterator < output->getLength()) //zrzucanie elementow outputu na stos
	{
		if (output->getChar(iterator) != ' ') temp->appendChar(output->getChar(iterator));
		else 
		{
			WspanialyString* dataToStack = new WspanialyString("");
			dataToStack->appendText(temp->getText());
			stack->push(dataToStack);
			*temp = "";
		}
		iterator++;
	}
	//jesli cos zostalo w tempie to wrzucamy na stos
	if (temp->getLength() > 0)
	{
		WspanialyString* dataToStack = new WspanialyString("");
		dataToStack->appendText(temp->getText());
		stack->push(dataToStack);
		*temp = "";
	}
	int stackElements = stack->stackLength(); //ilosc elementow na stosie
	stack->flipStack(); //odwrocenie stosu
	while (stackElements != 1 && !errorflag) //operowanie na stosie
	{
		bool neg_flag = 0; //flaga liczby ujemnej
		for (int i = 0; i < stackElements; i++)
		{
			char check = stack->getIndex(i)->getChar(0); //sprawdzamy pierwszy znak elementu
			if (!(check >= '0' && check <= '9')) //jesli nie jest liczba
			{
				if (stack->getIndex(i)->getLength() > 1) //jesli liczba ma wiecej niz 1 znak - mozliwa liczba ujemna
				{
					char pos_negative = stack->getIndex(i)->getChar(1);
					if (pos_negative >= '0' && pos_negative <= '9') neg_flag = 1; //jesli drugi znak to liczba to jest to liczba ujemna
				}
				if (!neg_flag) //jesli nie liczba ujemna to funkcja lub operand
				{
					for (int j = i; j > 0; j--) //wypisanie fragmentu stosu
					{
						stack->getIndex(j)->printStrLine();
						printf(" ");
					}
					stack->getIndex(0)->printStr(); //wypisanie poczatkowego elementu tak aby na koniec byl znak nowej linii
					if (stack->getIndex(i)->getLength() == 1 && isNormalOperator(check)) //jesli zwykly operator
					{
						int operand1 = atoi(stack->getIndex(i - 2)->getText());
						int operand2 = atoi(stack->getIndex(i - 1)->getText());
						int result;
						switch (check)
						{
							case '+':
								result = operand1 + operand2;
								break;
							case '-':
								result = operand1 - operand2;
								break;
							case '*':
								result = operand1 * operand2;
								break;
							case '/':
								if (operand2 == 0) errorflag = true; //dzielenie przez 0
								else result = operand1 / operand2;
								break;
						}
						if (!errorflag) //jesli nie bylo bledu to zamieniamy wynik na string i wrzucamy na stos
						{
							WspanialyString* resultString = func_intToWS(result);
							stack->getIndex(i)->setText(resultString->getText());
							stack->deleteIndex(i - 1);
							stack->deleteIndex(i - 2);
							delete resultString;
							stackElements -= 2;
						}
					}
					else
					{
						switch (check)
						{
							case 'M': //MIN lub MAX
							{
								WspanialyString* func = new WspanialyString("");
								WspanialyString* howMany = new WspanialyString(stack->getIndex(i)->getText());
								for (int k = 0; k < 3; k++)
								{
									func->appendChar(howMany->getChar(0)); //pobieramy pierwsze 3 znaki - rodzaj funkcji
									howMany->deleteChar(0); //usuwamy 3 pierwsze znaki - liczba parametrow
								}
								int howManyInt = atoi(howMany->getText());
								delete howMany;
								WspanialyString** temptab = new WspanialyString * [howManyInt];
								for (int j = 0; j < howManyInt; j++) temptab[j] = new WspanialyString(stack->getIndex(i - j - 1)->getText());
								if (func->operator==(*MAX))
								{
									int max = func_Max(howManyInt, temptab);
									WspanialyString* maxi = func_intToWS(max);
									stack->getIndex(i)->setText(maxi->getText());
									for (int j = 1; j <= howManyInt; j++) stack->deleteIndex(i - j);
									delete maxi;
								}
								else
								{
									int min = func_Min(howManyInt, temptab);
									WspanialyString* mini = func_intToWS(min);
									stack->getIndex(i)->setText(mini->getText());
									for (int j = 1; j <= howManyInt; j++) stack->deleteIndex(i - j);
									delete mini;
								}
								for (int j = 0; j < howManyInt; j++) delete temptab[j];
								delete[] temptab;
								delete func;
								stackElements -= howManyInt;
								break;
							}
							case 'N': //funkcja N
							{
								int number = atoi(stack->getIndex(i - 1)->getText());
								number = -number;
								WspanialyString* numberString = func_intToWS(number);
								stack->getIndex(i)->setText(numberString->getText());
								stack->deleteIndex(i - 1);
								stackElements--;
								delete numberString;
								break;
							}
							case 'I': //funkcja IF
							{
								int number1 = atoi(stack->getIndex(i - 3)->getText());
								int number2 = atoi(stack->getIndex(i - 2)->getText());
								int number3 = atoi(stack->getIndex(i - 1)->getText());
								if (number1 > 0) stack->getIndex(i)->setText(stack->getIndex(i - 2)->getText());
								else stack->getIndex(i)->setText(stack->getIndex(i - 1)->getText());
								for (int j = 1; j <= 3; j++) stack->deleteIndex(i - j);
								stackElements -= 3;
								break;
							}
						}
					}
					break;
				}
				else neg_flag = false; //jesli liczba ujemna to zerujemy flage, aby nie byla brana pod uwage przy kolejnych liczbach
			}
		}
	}
	if (!errorflag)
	{
		stack->getIndex(0)->printStr(); //wypisanie wyniku
		printf("\n");
		stack->chuff();
	}
	else
	{
		printf("ERROR\n"); //wypisanie bledu
		while (stack->stackLength() > 0) stack->chuff(); //wyczyszczenie stosu
	}
}

int main() {
	int loop = 0;
	scanf("%d", &loop);
	
	for (int i = 0; i < loop; i++) 
	{
		WspanialyString* output = ConvertToONP();  //ETAP 1 - konwersja na ONP
		Calculate_ONP(output); //ETAP 2 - obliczanie ONP
		delete output;
	}
 	delete MAX;
	delete MIN;
	delete IF;
	delete N;
	return 0;
}