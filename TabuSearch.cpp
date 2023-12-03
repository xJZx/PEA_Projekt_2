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
	// zapisujemy obecnie rozwa¿an¹ œcie¿kê (lista kandydatów)
	std::vector<int> currentPath;
	currentPath.resize(numberOfCities);

	// ustawiamy pocz¹tkowe rozwi¹zanie (algorytm zach³anny)
	currentPath = setSolution(matrix);
	minCost = findCost(currentPath);
	std::cout << "Cost after the Greedy Algorithm: " << minCost << "\n";

	// macierz odwiedzonych s¹siadów
	std::vector<std::vector<int>> neighbours;
	tabuMatrix.push_back(currentPath);

	// ile musi przeszukaæ kombinacji s¹siadów
	int stop = (numberOfCities * 5) + 1;
	// co ile wymieniamy tablicê tabu
	int change = 0;

	// start mierzenia czasu
	Time time;
	time.start();

	while (time.totalTime() < stopTime) {
		if (change < 200) {
			// znalezienie s¹siadów dla obecnej listy kandydatów
			for (int i = 0; i < stop; i++) {
				neighbours.push_back(findNeighbour(currentPath));
			}

			// jeœli znaleziony s¹siad nie bêdzie nale¿a³ do tablicy tabu oraz koszt s¹siada jest mniejszy od kosztu listy kandydatów,
			// to staje siê ona nowym rozwi¹zaniem
			for (auto& j : neighbours) {
				if (!(find(tabuMatrix.begin(), tabuMatrix.end(), j) != tabuMatrix.end()) && (findCost(j) < findCost(currentPath))) {
					currentPath = j;
				}
			}

			// jeœli koszt obecnej listy kandydatów oka¿e siê lepszy od minimalnego kosztu,
			// to staje siê on nowym najlepszym rozwi¹zaniem
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

			// dodanie obecnej œcie¿ki do tablicy tabu (zakazana œcie¿ka, nie mo¿na jej powtórzyæ)
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

// ustalenie pocz¹tkowego rozwi¹zania metod¹ zach³ann¹
std::vector<int> TabuSearch::setSolution(std::vector<std::vector<int>> matrix) {
	std::vector<int> solution;
	std::vector<bool> visited;

	// wype³nienie mapy
	visited.resize(numberOfCities, false);

	int currentCity = 0;
	solution.push_back(currentCity);
	visited[0] = true;
	// index na jakim znaleziono minimum w macierzy s¹siedztwa
	int minIndex = 0;

	int visitedCities = 1;
	while (visitedCities < numberOfCities) {
		int min = INT_MAX;

		// znalezienie minimum dla macierzy s¹siedztwa
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


// znalezienie s¹siada dla listy kandydatów
std::vector<int> TabuSearch::findNeighbour(std::vector<int> path) {
	int current = rand() % path.size();
	int next;

	do {
		next = rand() % path.size();
	} while (current == next);

	// zamiana elementów (a, b) ze sob¹
	iter_swap(path.begin() + current, path.begin() + next);

	return path;
}

// znalezieniu kosztu dla danej listy kandydatów
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