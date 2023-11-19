#include "TabuSearch.hpp"
#include <algorithm>


TabuSearch::TabuSearch(Tools lastSolution) {
	minCost = INT_MAX;
	numberOfCities = lastSolution.numberOfCities;
	tabuArraySize = pow(numberOfCities, 2);
	minPath.resize(numberOfCities);
	matrix = lastSolution.matrix;
	executionTime = 0;
}

TabuSearch::~TabuSearch() {

}

void TabuSearch::tabuSearch(double stopTime) {
	// zapisujemy obecnie rozwa�an� �cie�k� (lista kandydat�w)
	std::vector<int> currentPath;
	currentPath.resize(numberOfCities);

	// ustawiamy jakie� pocz�tkowe rozwi�zanie
	currentPath = setSolution(matrix);
	minCost = findCost(currentPath);

	std::vector<std::vector<int>> neighbours;
	tabuMatrix.push_back(currentPath);

	// ile musi przeszuka� kombinacji s�siad�w
	int stop = numberOfCities * 5;
	int change = 0;

	// start mierzenia czasu
	Time time;
	time.start();

	while (time.totalTime() < stopTime) {
		if (change < 500) {
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
				time.stop();
				executionTime = time.totalTime();
				change = 0;
			}
			else {
				change++;
			}

			// dodanie obecnej �cie�ki do tablicy tabu (zakazana �cie�ka, nie mo�na jej powt�rzy�)
			tabuMatrix.push_back(currentPath);
		}
		else {
			for (int i = 0; i < numberOfCities / 2; i++) {
				currentPath = findNeighbour(currentPath);
			}
		}

		time.stop();
	}
}

std::vector<int> TabuSearch::setSolution(std::vector<std::vector<int>> matrix) {
	std::vector<int> solution;
	int visitedCities = 0;
	int currentCity = 0;

	while (visitedCities < numberOfCities) {
		int min = INT_MAX;
		// index na jakim znaleziono minimum w macierzy s�siedztwa
		int minIndex = 0;

		// znalezienie minimum dla macierzy s�siedztwa
		for (int i = 0; i < numberOfCities; i++) {
			if ((matrix[currentCity][i] < min) && (matrix[currentCity][i] != -1)) {
				min = matrix[currentCity][i];
				minIndex = i;
			}
		}

		// wyeliminowanie podcykli z macierzy s�siedztwa
		for (int i = 0; i < numberOfCities; i++) {
			matrix[currentCity][minIndex] = INT_MAX;
			matrix[minIndex][currentCity] = INT_MAX;
			matrix[minIndex][0] = INT_MAX;
		}

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