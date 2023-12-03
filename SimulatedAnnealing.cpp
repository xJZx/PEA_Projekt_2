#include "SimulatedAnnealing.hpp"
#include <algorithm>
#include <unordered_map>


SimulatedAnnealing::SimulatedAnnealing(Tools lastSolution) {
	numberOfCities = lastSolution.numberOfCities;
	//minCostEnd = INT_MAX;
	//minPathEnd.resize(numberOfCities);
	minCost = INT_MAX;
	minPath.resize(numberOfCities);
	matrix = lastSolution.matrix;
	temperature = 0;
	bestTimeStamp = 0;
	executionTime = 0;
}

SimulatedAnnealing::~SimulatedAnnealing() {

}

void SimulatedAnnealing::simulatedAnnealing(double stopTime, double a, int epoque) {
	// ile musi przeszuka� kombinacji s�siad�w
	int stop = (numberOfCities * 5) + 1;

	// zapisujemy obecnie rozwa�an� �cie�k�
	std::vector<int> currentPath;
	currentPath.resize(numberOfCities);

	// ustawiamy pocz�tkowe rozwi�zanie (algorytm zach�anny)
	currentPath = setSolution(matrix, a);
	minPath = currentPath;
	std::cout << "Cost after the Greedy Algorithm: " << findCost(currentPath) << "\n";
	std::cout << "Temp Start: " << temperature << "\n";

	std::vector<int> neighbours;
	neighbours.resize(numberOfCities);

	// start mierzenia czasu
	Time time;
	time.start();

	while (time.totalTime() < stopTime) {
		int epoqueCounter = 0;
		while (epoqueCounter < epoque) {
			// znalezienie s�siad�w dla obecnej listy kandydat�w
			for (int i = 0; i < stop; i++) {
				neighbours = findNeighbour(currentPath);

				// je�li znaleziony s�siad oka�e si� korzystniejszy, staje si� nowym rozwi�zaniem
				if (findCost(neighbours) < findCost(currentPath)) {
					currentPath = neighbours;
					time.check();
					executionTime = time.totalTime();
				}
				// r�wnie� tak si� stanie je�li probabilistycznie mo�e si� okaza� optymalniejszy
				else {
					double randomNumber = ((double)rand() / RAND_MAX);
					double prob = probability(currentPath, neighbours);
					//std::cout << randomNumber << " " << prob << "\n";
					if (randomNumber < prob) {
						currentPath = neighbours;
						time.check();
						executionTime = time.totalTime();
					}
				}

				if (findCost(currentPath) < findCost(minPath)) {
					time.check();
					bestTimeStamp = time.totalTime();
					minPath = currentPath;
					minCost = findCost(minPath);
				}
			}

			epoqueCounter++;
		}

		// sch�adzanie
		temperature = cooling(a);
		time.check();
	}

	//minPathEnd = currentPath;
	//minCostEnd = findCost(minPathEnd);
	std::cout << "Temp End: " << temperature << "\n";
}

// ustalenie pocz�tkowego rozwi�zania metod� zach�ann�
std::vector<int> SimulatedAnnealing::setSolution(std::vector<std::vector<int>> matrix, double a) {
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
			if ((matrix[currentCity][i] < min) /*&& (matrix[currentCity][i] != INT_MAX)*/ && (visited[i] == false) && (currentCity != i)) {
				min = matrix[currentCity][i];
				minIndex = i;
			}
		}

		visited[minIndex] = true;

		currentCity = minIndex;

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
	// wz�r ze slajdu ze sta�� Boltzmanna (koszt to E - energia, kt�ra jest dzielona przez sta�� Boltzmanna (w tym przypadku a)
	// oraz przez t - temperatur�
	return exp(-(findCost(neighbours) - findCost(path)) / temperature);
}

double SimulatedAnnealing::cooling(double a) {
	return a * temperature;
}