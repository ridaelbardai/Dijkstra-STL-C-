// Dijkistra.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
#include<list>
#include<stack>


using namespace std;

int** lireMatrice(string fichier, int& d)
{
	int dimmension;
	ifstream myfile;
	myfile.open(fichier);
	//lecture de dimmension de la matrice
	myfile >> dimmension;
	d = dimmension;
	//allocation de la matrice
	int** matrice = new int*[dimmension];
	for (int i = 0; i < dimmension; i++)
		matrice[i] = new int[dimmension];

	//remplissage de la matrice a partir du fichier
	for (int i = 0; i < dimmension; i++)
	{
		for (int j = 0; j < dimmension; j++)
		{
			myfile >> matrice[i][j];
		}
	}
	//matrice prete !
	return matrice;
}

void initialisation(vector<int> &d, int s)
{
	for (size_t i = 0; i < d.size(); i++)
		d[i] = INT_MAX;
	d.at(s) = 0;
}

bool existe(list<int> &l, int val)
{
	list<int>::iterator it = std::find(l.begin(), l.end(), val);

	if (it != l.end())
		return true;
	return false;
}


int trouverMin(vector<int> &d, list<int> &l)
{
	int min = INT_MAX;
	int sommet;
	for (int i = 0; i < d.size(); i++)
	{
		if (!existe(l, i) && d[i] < min)
		{
			min = d[i];
			sommet = i;
		}
	}
	return sommet;
}

list<int> voisins(int** matrice, const int ind, const int dimm)
{
	list<int> voisins;
	for (int i = 0; i < dimm; i++)
	{
		if (matrice[ind][i] != 0)
			voisins.push_back(i);
	}
	return voisins;
}
int poids(int** matrice, const int s1, const int s2)
{
	return matrice[s1][s2];
}

void majDistances(vector<int> &v, int min, int crt, int** matrice, map<int, char>& predec)
{
	if (v[crt] > v[min] + poids(matrice, min, crt))
	{
		v[crt] = v[min] + poids(matrice, min, crt);

		auto it = predec.find(crt);
		if (it != predec.end()) {
			char c = 65 + min;
			it->second = c;
		}
		else
		{
			predec.insert(pair<int, char>(crt, 65 + min));
		}
	}

}

void dijkstra(int** &graphe, vector<int> &distances, list<int> &visited, map<int, char> &predecesseurs, const int &dimmension, int sommet)
{
	predecesseurs.insert(pair<int, char>(sommet, '-'));
	initialisation(distances, sommet);
	predecesseurs.insert(pair<int, int>(0, 0));
	while (visited.size() != dimmension)
	{
		int min = trouverMin(distances, visited);
		visited.push_back(min);
		list<int> listDesVoisins = voisins(graphe, min, dimmension);
		for (auto it = listDesVoisins.begin(); it != listDesVoisins.end(); ++it)
			majDistances(distances, min, *it, graphe, predecesseurs);
	}
}

void resultats(vector<int> &distances, map<int, char> &predecesseurs, int &dimmension)
{
	cout << "Noeud\t\tDistance " << endl;
	cout << "_________________________" << endl;
	for (int k = 0; k < dimmension; k++)
	{
		char str = 65 + k;
		cout << str << "\t\t" << distances[k] << endl;
	}

	cout << "\n\nNoeud\t\tPredecesseur" << endl;
	cout << "_________________________" << endl;
	for (map<int, char>::iterator itr = predecesseurs.begin(); itr != predecesseurs.end(); ++itr) {
		char str1 = 65 + itr->first;
		cout << str1 << "\t\t" << itr->second << endl;
	}
}

void cheminASuivre(int src, int dst, map<int, char> &predecesseurs)
{
	stack<char> pile;
	map<int, char>::iterator it;
	char s = char(dst+65);
	while (s != char(src+65))
	{
		pile.push(s);
		it = predecesseurs.find(s-65);
	
		s = it->second;
	}
	pile.push(src+65);

	cout << "\nchemin a suivre : ";
	while (!pile.empty())
	{
		if (pile.size() == 1)
		{
			cout << pile.top();
		}
		else {
		cout << pile.top() << " -> ";
		}
		pile.pop();
	}
	cout << endl;
}

int main()
{
	int dimmension;
	int** graphe = lireMatrice("matrice2.txt", dimmension);

	vector<int> distances(dimmension * dimmension);
	list<int> visited;
	map<int, char> predecesseurs;
	char src, dst;
	cout << "\nEntrez votre point de depart :\t";
	cin >> src;
	cout << "\nEntrez votre point d' arrivee :\t";
	cin >> dst;
	cout << endl;
	if (src > dimmension + 65 || dst > dimmension + 65)
	{
		cout << "ce point n'existe pas !" << endl;
		return 0;
	}
	else
	{

		dijkstra(graphe, distances, visited, predecesseurs, dimmension, src - 65);
		resultats(distances, predecesseurs, dimmension);
		cheminASuivre(src-65, dst-65, predecesseurs);
	}
	delete graphe;
	return 1;
}