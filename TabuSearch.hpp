#include "Tools.hpp"
#include <vector>


class TabuSearch
{
public:
	// macierz, w której przetrzymujemy "zakazane" trasy 
	std::vector<std::vector<int>> tabuMatrix;
	std::vector<std::vector<int>> matrix;
	int tabuArraySize;
	int minCost;
	int numberOfCities;
	std::vector<int> minPath;
	long executionTime;

	TabuSearch(Tools lastSolution);
	~TabuSearch();

	void tabuSearch(double stopTime);
	std::vector<int> setSolution(std::vector<std::vector<int>> tabuMatrix);
	std::vector<int> findNeighbour(std::vector<int> path);
	int findCost(std::vector<int>& path);
};