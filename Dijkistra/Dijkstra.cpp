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
	//toute les distances differentes du sommet de depart
	//sont mises a +infini
	for (size_t i = 0; i < d.size(); i++)
		d[i] = INT_MAX;
	//distance du sommet de depart tjr a zero
	d.at(s) = 0;
}

//fct pour savoir si une valeur existe dans une liste
// true : existe
//false : n'existe pas
bool existe(list<int> &l, int val)
{
	//iterateur qui pointe sur l'élément solicité
	list<int>::iterator it = std::find(l.begin(), l.end(), val);
	// si on arrive pas a la fin "end()" donc notre element existe
	if (it != l.end())
		return true;
	//n'existe pas
	return false;
}

//fct pour trouver le noeud avec distance la plus minimale
//dans un vecteur
//retourne l'indice de ce noeud
int trouverMin(vector<int> &distances, list<int> &visited)
{
	int min = INT_MAX;
	int sommet;
	for (int i = 0; i < distances.size(); i++)
	{
		//si ce noeud n'est pas visité et distance minimale
		if (!existe(visited, i) && distances[i] < min)
		{
			//maj de derniere valeur minimum trouvé
			min = distances[i];
			//garder l'indice du noeud de distance minimal
			sommet = i;
		}
	}
	return sommet;
}

//sert a remplire une liste avec les voisins d'un noeud donnée
list<int> voisins(int** matrice, const int ind, const int dimm)
{	
	//declarer une liste à retourner
	list<int> voisins;
	//parcourir une ligne de la matrice : la ligne matrice[ind][x]
	for (int i = 0; i < dimm; i++)
	{
		//si distance differente de 0 donc c'est un voisin
		if (matrice[ind][i] != 0)
			//inserer ce noeud dans la liste des voisins
			voisins.push_back(i);
	}
	return voisins;
}

//renvoi la distance entre deux noeuds voisins !
int poids(int** matrice, const int s1, const int s2)
{
	return matrice[s1][s2];
}

/* (majDistance) : necessaire pour le fonctionnemet de dijkstra, utilisé à chqaue fois
on trouve une distance inferieure que celle d'avant !*/
void majDistances(vector<int> &distances, int min, int crt, int** matrice, map<int, char>& predec)
{
	//On met à jour les distances entre SommetDebut et crt
	//en se posant la question : vaut-il mieux passer par min ou pas ? 
	if (distances[crt] > distances[min] + poids(matrice, min, crt))
	{
		// On prend ce nouveau chemin qui est plus court 
		distances[crt] = distances[min] + poids(matrice, min, crt);
		//En notant par où on passe

		auto it = predec.find(crt);
		if (it != predec.end()) {
			//maj d'une valeur deja existante dans la map des predecesseurs
			char c = 65 + min;
			it->second = c;
		}
		else
		{
			//attribution d'un predecesseur à un sommet
			predec.insert(pair<int, char>(crt, 65 + min));
		}
	}

}

void dijkstra(int** &graphe, vector<int> &distances, list<int> &visited, map<int, char> &predecesseurs, const int &dimmension, int sommet)
{
	//le sommet n'a pas de predecesseur
	predecesseurs.insert(pair<int, char>(sommet, '-'));
	//initialisation du tableau des distances
	initialisation(distances, sommet);
	//Tq on a pas traité tous les noeuds
	while (visited.size() != dimmension)
	{
		//trouver un noeud non visité de distance minimale
		int min = trouverMin(distances, visited);
		//ce noeud devient visité
		visited.push_back(min);
		//obtenir les voisins de cet noeud
		list<int> listDesVoisins = voisins(graphe, min, dimmension);
		//maj des distances vers ses voisins à partir de cet noeud
		for (auto it = listDesVoisins.begin(); it != listDesVoisins.end(); ++it)
			majDistances(distances, min, *it, graphe, predecesseurs);
	}
}

void resultats(vector<int> &distances, map<int, char> &predecesseurs, int &dimmension)
{
	//affichge du noeud et la distance qu'on peut parcourir pour y'arriver
	cout << "Noeud\t\tDistance " << endl;
	cout << "_________________________" << endl;
	for (int k = 0; k < dimmension; k++)
	{
		char str = 65 + k;
		cout << str << "\t\t" << distances[k] << endl;
	}
	//affichage de chaque noeud et son predecesseur
	cout << "\n\nNoeud\t\tPredecesseur" << endl;
	cout << "_________________________" << endl;
	for (map<int, char>::iterator itr = predecesseurs.begin(); itr != predecesseurs.end(); ++itr) {
		char str1 = 65 + itr->first;
		cout << str1 << "\t\t" << itr->second << endl;
	}
}
//fct qui prend source et destination pour afficher le plus court chemin entre les deux
void cheminASuivre(int src, int dst, map<int, char> &predecesseurs)
{
	//pile pour garder la trace des predecesseurs 
	stack<char> pile;
	//iterateur pour parcour le conteneur des predecesseurs
	map<int, char>::iterator it;
	char s = char(dst+65);
	while (s != char(src+65))
	{
		//on ajoute notre prochaine destination en tete de pile
		pile.push(s);	
		//on continue de suivre le chemin
		it = predecesseurs.find(s-65);	
		s = it->second;
	}
	//on empile enfin le nœud de départ
	pile.push(src+65);
	//Affichage
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
	//Contenaires utilisé
	vector<int> distances(dimmension * dimmension);
	list<int> visited;
	map<int, char> predecesseurs;
	//variables pour garder source/destination
	char src, dst;
	cout << "\nEntrez votre point de depart :\t";
	cin >> src;
	cout << "\nEntrez votre point d' arrivee :\t";
	cin >> dst;
	cout << endl;
	//s'assurer que les noeuds existent
	if (src > dimmension + 65 || dst > dimmension + 65)
	{
		//cas echeant !
		cout << "ce point n'existe pas !" << endl;
		return 0;
	}
	else
	{
		//tous ce passe bien on commence l'execution de dijkstra
		dijkstra(graphe, distances, visited, predecesseurs, dimmension, src - 65);
		//affichages des distances et des predecesseurs 
		resultats(distances, predecesseurs, dimmension);
		//le chemin a prendre
		cheminASuivre(src-65, dst-65, predecesseurs);
	}
	delete graphe;
	return 1;
}