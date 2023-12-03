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
	// ile musi przeszukaæ kombinacji s¹siadów
	int stop = (numberOfCities * 5) + 1;

	// zapisujemy obecnie rozwa¿an¹ œcie¿kê
	std::vector<int> currentPath;
	currentPath.resize(numberOfCities);

	// ustawiamy pocz¹tkowe rozwi¹zanie (algorytm zach³anny)
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
			// znalezienie s¹siadów dla obecnej listy kandydatów
			for (int i = 0; i < stop; i++) {
				neighbours = findNeighbour(currentPath);

				// jeœli znaleziony s¹siad oka¿e siê korzystniejszy, staje siê nowym rozwi¹zaniem
				if (findCost(neighbours) < findCost(currentPath)) {
					currentPath = neighbours;
					time.check();
					executionTime = time.totalTime();
				}
				// równie¿ tak siê stanie jeœli probabilistycznie mo¿e siê okazaæ optymalniejszy
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

		// sch³adzanie
		temperature = cooling(a);
		time.check();
	}

	//minPathEnd = currentPath;
	//minCostEnd = findCost(minPathEnd);
	std::cout << "Temp End: " << temperature << "\n";
}

// ustalenie pocz¹tkowego rozwi¹zania metod¹ zach³ann¹
std::vector<int> SimulatedAnnealing::setSolution(std::vector<std::vector<int>> matrix, double a) {
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
	// wzór ze slajdu ze sta³¹ Boltzmanna (koszt to E - energia, która jest dzielona przez sta³¹ Boltzmanna (w tym przypadku a)
	// oraz przez t - temperaturê
	return exp(-(findCost(neighbours) - findCost(path)) / temperature);
}

double SimulatedAnnealing::cooling(double a) {
	return a * temperature;
}