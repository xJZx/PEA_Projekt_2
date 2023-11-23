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
	// ile musi przeszukaæ kombinacji s¹siadów
	int stop = (numberOfCities * 5) + 1;

	// zapisujemy obecnie rozwa¿an¹ œcie¿kê
	std::vector<int> currentPath;
	currentPath.resize(numberOfCities);

	// ustawiamy jakieœ pocz¹tkowe rozwi¹zanie
	currentPath = setSolution(matrix, a);
	std::cout << "Cost after the Greedy Algorithm: " << findCost(currentPath) << "\n";

	std::vector<int> neighbours;
	neighbours.resize(numberOfCities);

	// start mierzenia czasu
	Time time;
	time.start();

	while (time.totalTime() < stopTime) {
		// znalezienie s¹siadów dla obecnej listy kandydatów
		for (int i = 0; i < stop; i++) {
			neighbours = findNeighbour(currentPath);

			// jeœli znaleziony s¹siad oka¿e siê korzystniejszy, staje siê nowym rozwi¹zaniem
			if (findCost(neighbours) < findCost(currentPath)) {
				currentPath = neighbours;
				time.check();
				executionTime = time.totalTime();
			}
			// równie¿ tak siê stanie jeœli propabilistycznie mo¿e siê okazaæ optymalniejszy
			else if (((double)rand() / RAND_MAX) < probability(currentPath, neighbours)) {
				currentPath = neighbours;
				time.check();
				executionTime = time.totalTime();
			}
		}

		// sch³adzanie
		temperature = cooling(a);
		time.check();
	}

	minPath = currentPath;
	minCost = findCost(minPath);
}

// ustalenie pocz¹tkowego rozwi¹zania metod¹ zach³ann¹
std::vector<int> SimulatedAnnealing::setSolution(std::vector<std::vector<int>> matrix, double a) {
	std::vector<int> solution;
	int visitedCities = 0;
	int currentCity = 0;

	while (visitedCities < numberOfCities) {
		int min = INT_MAX;
		// index na jakim znaleziono minimum w macierzy s¹siedztwa
		int minIndex = 0;

		// znalezienie minimum dla macierzy s¹siedztwa
		for (int i = 0; i < numberOfCities; i++) {
			if ((matrix[currentCity][i] < min) && (matrix[currentCity][i] != -1)) {
				min = matrix[currentCity][i];
				minIndex = i;
			}
		}

		// wyeliminowanie podcykli z macierzy s¹siedztwa
		for (int i = 0; i < numberOfCities; i++) {
			matrix[currentCity][minIndex] = INT_MAX;
			matrix[minIndex][currentCity] = INT_MAX;
			matrix[minIndex][0] = INT_MAX;
		}

		solution.push_back(minIndex);
		visitedCities++;
	}

	// sch³adzanie
	temperature = findCost(solution) * a;

	return solution;
}

// znalezienie s¹siada dla listy kandydatów
std::vector<int> SimulatedAnnealing::findNeighbour(std::vector<int> path) {
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