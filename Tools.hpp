#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Time.hpp"

/*Klasa zawierająca potrzebne narzędzia 
oraz przechowujaca informacje o ostatnio wygenerowanych danych i rozwiazaniu.*/

class Tools
{
	public:

		std::vector<std::vector<int>> matrix;
		int numberOfCities;
		std::vector<int> minPath;
		int minCost;
		char typeOfSolution;
		long time;

		Tools();
		~Tools();

		void readFromFile(std::string filename);

		void random();

		void print();
		void printSolution();

};