#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include "IncidenceMatrix.h"
//#include "AdjacencyList.h"
//#include "connectionmatrix.h"
#include <limits>
#include <cstdlib>
//#include "vld.h"
 

/******************************************************************/
bool IncidenceMatrix::setGraphType(int selectedType) {
	if (selectedType == 0) {
		gType = 0;
		return 0;
	}
	if (selectedType == 1) {
		gType = 1;
		return 0;
	}
	return 1;
}
/******************************************************************/
IncidenceMatrix::IncidenceMatrix(int numberOfTops, int numberOfEdges)
	:top(numberOfTops),
	edge(numberOfEdges),
	gType(0)
{
	matrix = allocateMatrix(numberOfTops, numberOfEdges);
	
}
/******************************************************************/
IncidenceMatrix::IncidenceMatrix(int)
	:top(1),
	edge(0),
	gType(0)
{
	while (top <= 1) {
		std::cout << "\nPodaj ilosc wierzcholkow (liczbe >= 2): ";
		cinSelectedInt(top);
	}
	while (edge <= 0) {
		std::cout << "Podaj ilosc wszystkich krawedzi (liczbe >=1): ";
		cinSelectedInt(edge);
	}
	matrix = allocateMatrix(top, edge);
}
/******************************************************************/
//IncidenceMatrix::IncidenceMatrix() {}
/******************************************************************/
IncidenceMatrix::IncidenceMatrix(const ConnectionMatrix<int>& conn)
{
	gType = 0;
	top = conn.getN();
	edge = conn.getL();
	matrix = allocateMatrix(top, edge);
	int i, j, e=0;
	for (i = 0; i < top; i++)
		for (j = i + 1; j < top; j++)
			if (conn(i, j) == 1) {
				setTopsOfEdge(e, i, j);
				e++;
			}
}
/******************************************************************/
bool IncidenceMatrix::setTopsOfEdge(int selectedEdge, int newOwner1, int newOwner2) {
	if (possibleEdge(newOwner1, newOwner2)) {
		if (edgeIndexExist(selectedEdge)) {
			if (!isThisEdgeFree(newOwner1, newOwner2)) {
				for (int t = 0; t < top; t++)
					matrix[t][selectedEdge] = 0;
				if (gType == 0) {
					matrix[newOwner1][selectedEdge] = 1;
					matrix[newOwner2][selectedEdge] = 1;
					return 0;
				}
				if (gType == 1) {
					matrix[newOwner1][selectedEdge] = 1;
					matrix[newOwner2][selectedEdge] = -1;
				}
			}
			else {
				std::cout << "setTopsOfEdge(" << selectedEdge << ", " << newOwner1 << ", " << newOwner2 << "): ";
				std::cout << "Taka krawedz juz istnieje! Nie zmieniono wierzcholkow!" << std::endl;
			}
		}
		else {
			std::cout << "setTopsOfEdge(" << selectedEdge << ", " << newOwner1 << ", " << newOwner2 << "): ";
			std::cout << "Indeks wierzcholka jest bledny!" << std::endl;
		}
	}
	else {
		std::cout << "setTopsOfEdge(" << selectedEdge << ", " << newOwner1 << ", " << newOwner2 << "): ";
		std::cout << "Podane indeksy nie stworza krawedzi!" << std::endl;
	}
	return 1;
}
/******************************************************************/
bool IncidenceMatrix::possibleEdge(int firstTop, int secondTop) const{
	//std::cout << "possibleedge" << std::endl; //fdssdf
	return (firstTop < top && secondTop < top && firstTop >= 0 && secondTop >= 0 && firstTop != secondTop);
}
/******************************************************************/
bool IncidenceMatrix::isThisEdgeFree(int firstTop, int secondTop) const{
	//std::cout << "isthisedgefree" << std::endl; //fdssdf
	if (possibleEdge(firstTop, secondTop)) {
		bool ValueOccured = false;
		if (gType == 0) {
			for (int e = 0; e < edge; e++) {
				if (matrix[firstTop][e] == 1 && matrix[secondTop][e] == 1) {
					ValueOccured = true;
				}
			}
		}
		if (gType == 1) {
			for (int e = 0; e < edge; e++) {
				if ((matrix[firstTop][e] == 1 && matrix[secondTop][e] == -1) || (matrix[firstTop][e] == -1 && matrix[secondTop][e] == 1)) {
					ValueOccured = true;
				}
			}
		}
		return ValueOccured;
	}
	else
		return true;
}
/******************************************************************/
bool IncidenceMatrix::edgeIndexExist(int questionedEdge) const {
	return ((questionedEdge < this->edge) && (questionedEdge >= 0));
}
/******************************************************************/
bool IncidenceMatrix::deleteEdge(int unwantedEdge) {
	///std::cout << "deleteEdge(" << unwantedEdge << ") :" << std::endl;
	///std::cout << "_____edgeIndexExist(unwantedEdge)==" << edgeIndexExist(unwantedEdge) << std::endl;
	if (edgeIndexExist(unwantedEdge)) {
		int** temp = allocateMatrix(top, edge - 1);
		int e;
		for (int t = 0; t < top; t++) {
			for (e = 0; e < unwantedEdge; e++) {
				temp[t][e] = matrix[t][e];
			}
			for (e = unwantedEdge; e < edge - 1; e++) {
				temp[t][e] = matrix[t][e+1];
			}
			delete[] matrix[t];
		}
		delete[] matrix;
		matrix = temp;
		edge--;
		return 0;
	}
	else return 1;
}
/******************************************************************/
bool IncidenceMatrix::addNewEdge(int newOwner1, int newOwner2) {
	if (possibleEdge(newOwner1, newOwner2)) {
		if (!isThisEdgeFree(newOwner1, newOwner2)) {
			int** temp = allocateMatrix(top, edge + 1);
			for (int t = 0; t < top; t++) {
				for (int e = 0; e < edge; e++) {
					temp[t][e] = matrix[t][e];
				}
				delete[] matrix[t];
				temp[t][edge] = 0;
			}
			delete[] matrix;
			if (gType == 0) {
				temp[newOwner1][edge] = 1;
				temp[newOwner2][edge] = 1;
			}
			if (gType == 1) {
				temp[newOwner1][edge] = 1;
				temp[newOwner2][edge] = -1;
			}
			matrix = temp;
			edge++;
			return 0;
		}
		else {
			std::cout << "addNewEdge(" << newOwner1 << ", " << newOwner2 << "):";
			std::cout << "Nie moge dodac takiej krawedzi! Taka juz istnieje!" << std::endl;
			return 1;
		}
	}
	else {
		std::cout << "addNewEdge(" << newOwner1 << ", " << newOwner2 << "):";
		std::cout << "Podane indeksy nie moga stworzyc krawedzi!" << std::endl;
		return 1;
	}
}
/******************************************************************/
bool IncidenceMatrix::setEntireMatrixByRows(){
	std::cout<<"\nWpisuj kolejne pola oddzielajac enterem \n";
	std::cout<<"Wprowadzasz wierszami (czyli 1 gdy do danego wierzcholka ma nalezec iterowana krawedz) \n";
	int temp, t, e;
	for (t = 0; t < top; t++) {
		std::cout<<"Wiersz numer "<<t<<": \n";
		for (e = 0; e < edge; e++) {
			cinSelectedInt(temp);
			if (gType == 0) {
				if ((temp == 0) || (temp == 1))
					matrix[t][e] = temp;
				else {
					std::cout << "Mozesz wpisac tylko 0 lub 1. Wpisz liczbe ponownie: ";
					e--;
				}
			}
			if (gType == 1) {
				if ((temp == 0) || (temp == 1) || (temp == -1))
					matrix[t][e] = temp;
				else {
					std::cout << "Mozesz wpisac tylko 0, 1, lub -1 Wpisz liczbe ponownie: ";
					e--;
				}
			}
		}
		std::cout<<std::endl;
	}
	bool errorOccured = eliminateAllMistakes();
	if (errorOccured)
		std::cout<<"\nMacierz byla zle wpisana. Naprawiono bledy."<<std::endl;
	return errorOccured;
}
/******************************************************************/
bool IncidenceMatrix::setEntireMatrixByColumns() { 
	std::cout << "\nWpisuj kolejne pola oddzielajac enterem \n";
	std::cout << "Wprowadzasz kolumnami (czyli 1 gdy iterowany wierzcholek ma byc poczatkiem/koncem krawedzi) \n";
	int temp, e, t;
	for (e = 0; e < edge; e++) {
		std::cout << "Kolumna numer " << e << ": \n";
		for (t = 0; t < top; t++) {
			cinSelectedInt(temp);
			if (gType == 0) {
				if ((temp == 0) || (temp == 1))
					matrix[t][e] = temp;
				else {
					std::cout << "Mozesz wpisac tylko 0 lub 1. Wpisz liczbe ponownie: ";
					t--;
				}
			}
			if (gType == 1) {
				if ((temp == 0) || (temp == 1) || (temp == -1))
					matrix[t][e] = temp;
				else {
					std::cout << "Mozesz wpisac tylko 0, 1, lub -1 Wpisz liczbe ponownie: ";
					t--;
				}
			}
		}
		std::cout << std::endl;
	}
	bool errorOccured = eliminateAllMistakes();
	if (errorOccured)
		std::cout << "\nMacierz byla zle wpisana. Naprawiono bledy." << std::endl;
	return errorOccured;
}
/******************************************************************/
bool IncidenceMatrix::eliminateInvalidEdges() {
	bool errorOccured = false;
	if (gType == 0) {
		int counter, e, t, isnot01;
		if (edge > 0) {
			for (e = edge - 1; e >= 0; e--) {
				counter = isnot01 = 0;
				for (t = 0; t < top; t++) {
					if (matrix[t][e])
						counter++;
					if (matrix[t][e] > 1)
						isnot01++;
				}
				if (counter != 2) {
					deleteEdge(e);
					std::cout << "Pozbyto sie krawedzi o indeksie " << e;
					std::cout << ": nalezala do blednej liczby wierzcholkow" << std::endl;
					errorOccured = true;
				}
				else if (isnot01) {
					deleteEdge(e);
					std::cout << "Pozbyto sie krawedzi o indeksie " << e;
					std::cout << ": posiadala w reprezentacji liczby wieksze niz 1" << std::endl;
					errorOccured = true;
				}
			}
		}
	}
	if (gType == 1) {
		int counter, e, t, have1, have_1;
		if (edge > 0) {
			for (e = edge - 1; e >= 0; e--) {
				counter = have1 = have_1 = 0;
				for (t = 0; t < top; t++) {
					if (matrix[t][e] != 0)
						counter++;
					if (matrix[t][e] == 1)
						have1 = true;
					if (matrix[t][e] == -1)
						have_1 = true;
				}
				if (counter != 2) {
					deleteEdge(e);
					std::cout << "Pozbyto sie krawedzi o indeksie " << e;
					std::cout << ": nalezala do blednej liczby wierzcholkow" << std::endl;
					errorOccured = true;
				}
				else if (!(have1 && have_1)) {
					deleteEdge(e);
					std::cout << "Pozbyto sie krawedzi o indeksie " << e;
					std::cout << ": posiadala w reprezentacji liczby inne niz para 1 i -1" << std::endl;
					errorOccured = true;
				}
			}
		}
	}
	if (errorOccured)
		std::cout << "!!!Zmieniono numeracje krawedzi z powodu usuniecia blednych krawedzi!" << std::endl;
	return errorOccured;
}
/******************************************************************/
bool IncidenceMatrix::getTopsOfEdge(int selectedEdge, int& first, int& second) const {
	first = second = -2137;
	if (!edgeIndexExist(selectedEdge)) {
		std::cout << "getTopsOfEdge(" << selectedEdge << ", ..., ...): Krawedz nie istnieje!!!" << std::endl;
		std::cout << "Pobranie wierzcholkow nie powiodlo sie!" << std::endl;
		return true;
	}
	else {
		int counter = 0;
		for (int t = 0; t < top; t++) {
			if (matrix[t][selectedEdge]) {
				if (counter == 0) {
					first = t;
					counter++;
				}
				else {
					if (counter == 1) {
						second = t;
						counter++;
					}
					else {
						std::cout << "Korzystasz z popsutej macierzy incydencji! Nie mozesz pobrac wierzcholkow!" << std::endl;
						std::abort();
					}
				}
			}
		}
		if (counter <= 1) {
			std::cout << "Korzystasz z popsutej (pustej) macierzy incydencji! Nie mozesz pobrac wierzcholkow!" << std::endl;
			std::abort();
		}
		if (second > first) { //obsula digrafu
			int temp;
			temp = first;
			first = second;
			second = temp;
		}
		return false;
	}
}
/******************************************************************/
bool IncidenceMatrix::eliminateDuplicates() {
	bool errorOccured = false;
	if (gType == 0) {
		if (edge >= 1) {
			int e, k, index1, index2, EdgeOccured;
			for (e = edge - 1; e >= 1; e--) {
				getTopsOfEdge(e, index1, index2);
				k = e - 1;
				EdgeOccured = false;
				while (k >= 0 && !EdgeOccured) {
					if (matrix[index1][k] == 1 && matrix[index2][k] == 1)
						EdgeOccured = true;
					k--;
				}
				if (EdgeOccured) {
					deleteEdge(e);
					errorOccured = true;
					std::cout << "Usunieto zmultiplikowana krawedz o indeksie " << e << std::endl;
				}
			}
		}
	}
	if (gType == 1) {
		if (edge >= 1) {
			int e, k, index1, index2, EdgeOccured;
			for (e = edge - 1; e >= 1; e--) {
				getTopsOfEdge(e, index1, index2);
				k = e - 1;
				EdgeOccured = false;
				while (k >= 0 && !EdgeOccured) {
					if ((matrix[index1][k] == 1) && (matrix[index2][k] == -1)) {
						std::cout << "debug" << std::endl; //______________________________________
						EdgeOccured = true;
					}
					k--;
				}
				if (EdgeOccured) {
					deleteEdge(e);
					errorOccured = true;
					std::cout << "Usunieto zmultiplikowana krawedz skierowana o indeksie " << e << std::endl;
				}
			}
		}
	}
	return errorOccured;
}
/******************************************************************/
bool IncidenceMatrix::eliminateAllMistakes() {
	bool x = eliminateInvalidEdges();
	bool y = eliminateDuplicates();
	return (x || y);
}
/******************************************************************/
IncidenceMatrix::~IncidenceMatrix(){
	for(int i=0;i<top;i++)
		delete [] matrix[i];
	delete [] matrix;
}
/******************************************************************/
void IncidenceMatrix::printEntireMatrix() const{
	std::cout<<"\nMacierz Incydencji: "<<std::endl;
	if (edge) {
		std::cout << "    ";
		for (int e = 0; e < edge; e++) std::cout << "E" << e << " "; std::cout << std::endl;
		for (int t = 0; t < top; t++) {
			std::cout << t << " |";
			for (int e = 0; e < edge; e++) {
				if (matrix[t][e] >= 0)
					std::cout << " " << matrix[t][e] << " ";
				else
					std::cout << "" << matrix[t][e] << " ";
			}
			std::cout << "|" << std::endl;
		}
	}
	else std::cout << " nie posiada wierzcholkow!" << std::endl;
}
/******************************************************************/
bool IncidenceMatrix::setEntireMatrixFromFile(const char* Filename, int numberOfTops, int numberOfEdges) {
	std::ifstream mystream;
	if (top == numberOfTops && edge == numberOfEdges) {
		mystream.close();
		mystream.open(Filename, std::ios::in); ///drugi argument == read-only mode
		if (!mystream.is_open()) {
			std::cout << "Odmowa dostepu lub nie ma pliku o tej nazwie!" << std::endl;
			mystream.clear();
			mystream.close();
		}
		else {
			int i, j, error=0;
			for (i = 0; i < top; i++) {
				for (j = 0; j < edge; j++)
					if (!(mystream >> matrix[i][j]))
						error++;
			}
			if (i == top && j == edge && error == 0) {
				std::cout << "\nPlik zostal wczytany poprawnie; sprawdzam bledy w reprezentacji..." << std::endl;
				if (!eliminateAllMistakes()) {
					std::cout << "W reprezentacji nie bylo zadnych bledow." << std::endl;
				}
				else {
					std::cout << "W reprezentacji byly bledy; plik zle wczytany; wylaczam program!" << std::endl;
					std::abort();
				}
				return 0;
			}
			else {
				std::cout << "Nie ustawiono calej macierzy!" << std::endl;
			}
		}
	}
	else {
		std::cout << "Wczytywanie z pliku nie powiodlo sie gdyz ";
		std::cout << "zaalokowana pamiec nie zgadza sie z podanymi wymiarami" << std::endl;
	}
	mystream.clear();
	mystream.close();
	return 1;
}
/******************************************************************/





bool IncidenceMatrix::isSafe(int candidat, std::vector<int> path, int pos)
{
	//std::cout << "przed if:" << std::endl; //fdssdf
	int debug = path[pos - 1];
	//std::cout << "debug:" << debug << std::endl; //fdssdf
	if (isThisEdgeFree(debug, candidat) == 0)
		return false;
	//std::cout << "po if:" << std::endl; //fdssdf

	for (int i = 0; i <= pos; i++) {
		//std::cout << "w ifsafe for nr:" << i << std::endl; //fdssdf
		if (path[i] == candidat)
			return false;
	}
	return true;
}

bool IncidenceMatrix::hamCycleUtil(std::vector<int>& path, int pos)
{
	path[0] = 0;
	if (pos == top)
	{
		//std::cout << "if" << std::endl;//fdssdf
		if ((isThisEdgeFree(path[pos - 1], path[0]) == 1) && possibleEdge(path[pos -1], path[0])) {
			path[top] = 0;
			return true;
		}
		else
			return false;
	}

	for (int v = 1; v < top; v++)
	{
		//std::cout << "for nr:"<<v << std::endl; //fdssdf
		if (isSafe(v, path, pos))
		{
			//std::cout << "is safe" << v << std::endl; //fdssdf
			path[pos] = v;
			if (hamCycleUtil(path, pos + 1) == true) {
				return true;
			path[pos] = -1;
			}
		}
	}
	return false;

}

bool IncidenceMatrix::hamPathUtil(std::vector<int>& path, int pos, int first)
{
	path[0] = first;
	if (pos == top)
	{
		return true;
	}

	for (int v = 0; v < top; v++)
	{
		if (isSafe(v, path, pos))
		{
			path[pos] = v;
			if (hamPathUtil(path, pos + 1, first) == true) {
				return true;
				path[pos] = -1;
			}
		}
	}
	return false;

}

std::vector<int> IncidenceMatrix::findHamiltionianGraph()
{
	std::cout << std::endl;
	std::vector<int> path;
	for (int i = 0; i < top + 1; i++)
		path.push_back(-1);
	if (top < 3) {
		std::cout << "Graf polhamiltonowski, sciezka 0,1." << std::endl;
		path[0] = 0;
		path[1] = 1;
		return path;
	}
	if (edge < top - 1) {
		std::cout << "Nie mozna utworzyc z tego grafu ani sciezki, ani cyklu (edge < top - 1)" << std::endl;
		for (int i = 0; i < top + 1; i++)
			path[i] = -100;
		return path;
	}


	int i = 0, first = 0;
	int *c = new int[top];
	bool canBeHamilton = true;
	bool isHamilton = true;
	bool isHalf = false;
	for (int i = 0; i < top; i++) {
		c[i] = 0;
		for (int j = 0; j < edge; j++) {
			c[i] += matrix[i][j];
		}
		if (c[i] < 2) {
			canBeHamilton = false;
			std::cout << "Graf NIE moze byc hamiltonowski (liczba krawedzi przy choc 1 wierzcholku <2)" << std::endl;
			i = top;
			break;
		}
	}
	delete[] c;

	if (canBeHamilton) {
		std::cout << "Graf moze byc hamiltonowski!" << std::endl;
		if (hamCycleUtil(path, 1) == false) {
			std::cout << "Graf NIE jest hamiltonowski (nie posiada cyklu Hamiltona)!" << std::endl;
			isHamilton = false;
		}
	}
	else
		isHamilton = false;

	if (!canBeHamilton || !isHamilton) { //polhamiltonowski
		int first = 0;
		std::cout << "Graf moze byc polhamiltonowski!" << std::endl;
		isHalf = false;

	//  delete[] path;
	// jak dodasz te 2 linijki to zmien w forze na i<top bez +1
	//	path = new int[top];
	
		for (int i = 0; i < top+1; i++)
			path[i] = -1;

		while ((first < top) && (!isHalf)) {
			//std::cout << "while dla first="<<first<< std::endl;
			if (hamPathUtil(path, 1, first) == false) {
				isHalf = false;
				//std::cout << "poszedl if" << std::endl;
			}
			else {
				isHalf = true;
				//std::cout << "poszedl else" << std::endl;
				break;
			}
			first++;
		}
	}




	if (isHamilton && canBeHamilton) {
		std::cout << "Graf jest hamiltonowski." << std::endl;
		std::cout << "Cykl Hamiltona:  ";
		for (int i = 0; i < top + 1; i++) {
			std::cout << path[i];
			if (i < top)
				std::cout << ",";
		}
		std::cout << std::endl;
	}

	if (isHalf) {
		std::cout << "Graf jest POLhamiltonowski." << std::endl;
		std::cout << "Sciezka Hamiltona:  ";
		for (int i = 0; i < top+1; i++) {
			std::cout << path[i];
			if (path[i + 1] < 0)
				break;
			if (i < top)
				std::cout << ",";
		}
		std::cout << std::endl;
	}

	if (!isHalf && !isHamilton) {
		std::cout << "Nie jest tez jednak polhamiltonowski" << std::endl;
		path.erase(path.begin(), path.end());
	}
	return path;
}









/******************************************************************/
/*Funkcje spoza klasy IncidenceMatrix*/
/******************************************************************/

bool cinSelectedInt(int& destination) {
	int temp;
	std::cin >> temp;
	if (!std::cin) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		destination = -2137;
		return false;
	}
	else {
		destination = temp;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return true;
	}
}
/******************************************************************/
float randomfloat(int A, int B) {
	srand((unsigned int)time(NULL));
	return (float)(rand() / (RAND_MAX + 1.0)*(B - A) + A);
}
/******************************************************************/
int randomint(int A, int B) {
	srand((unsigned int)time(NULL));
	return (int)(rand() / (RAND_MAX + 1.0)*(B - A) + A);
}
/******************************************************************/
int** allocateMatrix(int top, int edge) {
	int** matrix = new int*[top];
	for (int t = 0; t < top; t++) {
		matrix[t] = new int[edge];
		for (int e = 0; e < edge; e++)
			matrix[t][e] = 0;
	}
	return matrix;
}
/******************************************************************/
void printNormalIncMatrix(int** matrix, int top, int edge){
	std::cout << "Macierz Incydencji: " << std::endl;
	if (edge) {
		std::cout << "    ";
		for (int e = 0; e < edge; e++) std::cout << "E" << e << " "; std::cout << std::endl;
		for (int t = 0; t < top; t++) {
			std::cout << t << " |";
			for (int e = 0; e < edge; e++) {
				if (matrix[t][e] >= 0)
					std::cout << " " << matrix[t][e] << " ";
				else
					std::cout << "" << matrix[t][e] << " ";
			}
			std::cout << "|" << std::endl;
		}
	}
	else std::cout << " nie posiada wierzcholkow!" << std::endl;
}
/******************************************************************/
/*
IncidenceMatrix getRandomGraph(int tops, int edges) {
	int maxedges = (tops*(tops - 1) / 2);
	IncidenceMatrix *graph = new IncidenceMatrix(tops, edges);
	int** temp = allocateMatrix(2, maxedges);
	int *losowe = new int[maxedges];
	int i, counter = 0, rand, j, e = 0;
	for (i = 0; i < edges; i++) {
		losowe[i] = 0;
	}
	while (counter < edges) {
		rand = randomint(0, maxedges - 1);
		if (losowe[rand] == 0) {
			losowe[rand] = 1;
			counter++;
		}
	}

	for (i = 0; i < tops; i++) {
		for (j = i + 1; j < tops && e < maxedges; j++) {
			if (losowe[e])
				graph->setTopsOfEdge(e, i, j);			
			e++;
		}
	}

	delete[] temp[0];
	delete[] temp[1];
	delete[] temp;
	delete[] losowe;
	return *graph;
}
*/
