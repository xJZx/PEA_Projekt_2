#include "Tools.hpp"
#include <iomanip>

Tools::Tools() {
	numberOfCities = 0;
	minPath.resize(numberOfCities);
	minCost = INT_MAX;
	typeOfSolution = 0;
    time = 0;
}

Tools::~Tools() {
	numberOfCities = 0;
	matrix.clear();
	minPath.clear();
	minCost = 0;
	typeOfSolution = 0;
	time = 0;
}

void Tools::readFromFile(std::string filename) {
    std::fstream file;

    file.open(filename);

    if (file.good())
    {
        file >> numberOfCities;

        matrix.clear();
        matrix.resize(numberOfCities, std::vector<int>(numberOfCities));

        int weight;
        for (int first = 0; first < numberOfCities; first++)
        {
            for (int second = 0; second < numberOfCities; second++)
            {
                file >> weight;
                matrix[first][second] = weight;
                // bo graf asymetryczny skierowany
                //array[second][first] = weight;
            }
        }
        file.close();
    }
    else
    {
        std::cout << "Error occurred!\n";
    }
}

void Tools::print() {
    if (matrix.empty() == true) {
        std::cout << "Array is empty!\n";
        return;
    }

    std::cout << "Graph stored in the matrix: \n";
    for (int first = 0; first < numberOfCities; first++)
    {
        for (int second = 0; second < numberOfCities; second++)
        {
            std::cout << std::setw(4) << matrix[first][second];
        }

        std::cout << "\n";
    }
    std::cout << std::endl;
}