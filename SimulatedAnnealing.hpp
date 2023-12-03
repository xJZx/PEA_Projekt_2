#include "Tools.hpp"


class SimulatedAnnealing
{
public:
	double temperature;
	//int minCostEnd;
	int minCost;
	//std::vector<int> minPathEnd;
	std::vector<int> minPath;
	std::vector<std::vector<int>> matrix;
	int numberOfCities;
	long bestTimeStamp;
	long executionTime;

	SimulatedAnnealing(Tools lastSolution);
	~SimulatedAnnealing();

	void simulatedAnnealing(double stopTime, double a, int epoque);
	double cooling(double a);
	double probability(std::vector<int> path, std::vector<int> neighbours);
	std::vector<int> setSolution(std::vector<std::vector<int>> matrix, double a);
	std::vector<int> findNeighbour(std::vector<int> path);
	int findCost(std::vector<int>& path);

};