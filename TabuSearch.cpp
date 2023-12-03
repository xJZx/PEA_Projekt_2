#include "TabuSearch.hpp"
#include <algorithm>


TabuSearch::TabuSearch(Tools lastSolution) {
	minCost = INT_MAX;
	numberOfCities = lastSolution.numberOfCities;
	tabuArraySize = pow(numberOfCities, 2);
	minPath.resize(numberOfCities);
	matrix = lastSolution.matrix;
	bestTimeStamp = 0;
	executionTime = 0;
}

TabuSearch::~TabuSearch() {

}

void TabuSearch::tabuSearch(double stopTime) {
	// zapisujemy obecnie rozwa�an� �cie�k� (lista kandydat�w)
	std::vector<int> currentPath;
	currentPath.resize(numberOfCities);

	// ustawiamy pocz�tkowe rozwi�zanie (algorytm zach�anny)
	currentPath = setSolution(matrix);
	minCost = findCost(currentPath);
	std::cout << "Cost after the Greedy Algorithm: " << minCost << "\n";

	// macierz odwiedzonych s�siad�w
	std::vector<std::vector<int>> neighbours;
	tabuMatrix.push_back(currentPath);

	// ile musi przeszuka� kombinacji s�siad�w
	int stop = (numberOfCities * 5) + 1;
	// co ile wymieniamy tablic� tabu
	int change = 0;

	// start mierzenia czasu
	Time time;
	time.start();

	while (time.totalTime() < stopTime) {
		if (change < 200) {
			// znalezienie s�siad�w dla obecnej listy kandydat�w
			for (int i = 0; i < stop; i++) {
				neighbours.push_back(findNeighbour(currentPath));
			}

			// je�li znaleziony s�siad nie b�dzie nale�a� do tablicy tabu oraz koszt s�siada jest mniejszy od kosztu listy kandydat�w,
			// to staje si� ona nowym rozwi�zaniem
			for (auto& j : neighbours) {
				if (!(find(tabuMatrix.begin(), tabuMatrix.end(), j) != tabuMatrix.end()) && (findCost(j) < findCost(currentPath))) {
					currentPath = j;
				}
			}

			// je�li koszt obecnej listy kandydat�w oka�e si� lepszy od minimalnego kosztu,
			// to staje si� on nowym najlepszym rozwi�zaniem
			if (findCost(currentPath) < minCost) {
				minPath = currentPath;
				minCost = findCost(currentPath);
				time.check();
				executionTime = time.totalTime();
				bestTimeStamp = time.totalTime();
				change = 0;
				tabuMatrix.clear();
			}
			else {
				change++;
			}

			// dodanie obecnej �cie�ki do tablicy tabu (zakazana �cie�ka, nie mo�na jej powt�rzy�)
			tabuMatrix.push_back(currentPath);
		}
		// przeszukiwanie lokalne
		else {
			for (int i = 0; i < numberOfCities / 2; i++) {
				currentPath = findNeighbour(currentPath);

				if (findCost(currentPath) < minCost) {
					minPath = currentPath;
					minCost = findCost(currentPath);
					time.check();
					executionTime = time.totalTime();
					bestTimeStamp = time.totalTime();
				}
			}
			/*tabuMatrix.clear();
			change = 0;*/
		}

		time.check();
		executionTime = time.totalTime();
	}
}

// ustalenie pocz�tkowego rozwi�zania metod� zach�ann�
std::vector<int> TabuSearch::setSolution(std::vector<std::vector<int>> matrix) {
	std::vector<int> solution;
	std::vector<bool> visited;

	// wype�nienie mapy
	visited.resize(numberOfCities, false);

	int currentCity = 0;
	solution.push_back(currentCity);
	visited[0] = true;
	// index na jakim znaleziono minimum w macierzy s�siedztwa
	int minIndex = 0;

	int visitedCities = 1;
	while (visitedCities < numberOfCities) {
		int min = INT_MAX;

		// znalezienie minimum dla macierzy s�siedztwa
		for (int i = 0; i < numberOfCities; i++) {
			if ((matrix[currentCity][i] < min) && (visited[i] == false) && (currentCity != i)) {
				min = matrix[currentCity][i];
				minIndex = i;
			}
		}

		visited[minIndex] = true;

		currentCity = minIndex;

		solution.push_back(minIndex);
		visitedCities++;
	}

	return solution;
}


// znalezienie s�siada dla listy kandydat�w
std::vector<int> TabuSearch::findNeighbour(std::vector<int> path) {
	int current = rand() % path.size();
	int next;

	do {
		next = rand() % path.size();
	} while (current == next);

	// zamiana element�w (a, b) ze sob�
	iter_swap(path.begin() + current, path.begin() + next);

	return path;
}

// znalezieniu kosztu dla danej listy kandydat�w
int TabuSearch::findCost(std::vector<int>& path) {
	int cost = 0;
	int iterator = 0;

	for (int i = 1; i < path.size(); i++) {
		cost += matrix[path[iterator]][path[i]];
		iterator++;
	}

	cost += matrix[path[iterator]][path[0]];

	return cost;
}