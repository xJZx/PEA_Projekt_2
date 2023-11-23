#include "SimulatedAnnealing.hpp"
#include <algorithm>


SimulatedAnnealing::SimulatedAnnealing(Tools lastSolution) {
	numberOfCities = lastSolution.numberOfCities;
	minCost = INT_MAX;
	minPath.resize(numberOfCities);
	matrix = lastSolution.matrix;
	temperature = 0;
	executionTime = 0;
}

SimulatedAnnealing::~SimulatedAnnealing() {

}

void SimulatedAnnealing::simulatedAnnealing(double stopTime, double a) {
	// ile musi przeszuka� kombinacji s�siad�w
	int stop = (numberOfCities * 5) + 1;

	// zapisujemy obecnie rozwa�an� �cie�k�
	std::vector<int> currentPath;
	currentPath.resize(numberOfCities);

	// ustawiamy jakie� pocz�tkowe rozwi�zanie
	currentPath = setSolution(matrix, a);
	std::cout << "Cost after the Greedy Algorithm: " << findCost(currentPath) << "\n";

	std::vector<int> neighbours;
	neighbours.resize(numberOfCities);

	// start mierzenia czasu
	Time time;
	time.start();

	while (time.totalTime() < stopTime) {
		// znalezienie s�siad�w dla obecnej listy kandydat�w
		for (int i = 0; i < stop; i++) {
			neighbours = findNeighbour(currentPath);

			// je�li znaleziony s�siad oka�e si� korzystniejszy, staje si� nowym rozwi�zaniem
			if (findCost(neighbours) < findCost(currentPath)) {
				currentPath = neighbours;
				time.check();
				executionTime = time.totalTime();
			}
			// r�wnie� tak si� stanie je�li propabilistycznie mo�e si� okaza� optymalniejszy
			else if (((double)rand() / RAND_MAX) < probability(currentPath, neighbours)) {
				currentPath = neighbours;
				time.check();
				executionTime = time.totalTime();
			}
		}

		// sch�adzanie
		temperature = cooling(a);
		time.check();
	}

	minPath = currentPath;
	minCost = findCost(minPath);
}

// ustalenie pocz�tkowego rozwi�zania metod� zach�ann�
std::vector<int> SimulatedAnnealing::setSolution(std::vector<std::vector<int>> matrix, double a) {
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

	// sch�adzanie
	temperature = findCost(solution) * a;

	return solution;
}

// znalezienie s�siada dla listy kandydat�w
std::vector<int> SimulatedAnnealing::findNeighbour(std::vector<int> path) {
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
int SimulatedAnnealing::findCost(std::vector<int>& path) {
	int cost = 0;
	int iterator = 0;

	for (int i = 1; i < path.size(); i++) {
		cost += matrix[path[iterator]][path[i]];
		iterator++;
	}

	cost += matrix[path[iterator]][path[0]];

	return cost;
}

double SimulatedAnnealing::probability(std::vector<int> path, std::vector<int> neighbours) {
	return exp(-(findCost(neighbours) - findCost(path)) / temperature);
}

double SimulatedAnnealing::cooling(double a) {
	return a * temperature;
}