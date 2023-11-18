#include "Menu.hpp"
#include <iostream>


void Menu::start()
{
    std::string lastFilename;
    double stopTime = 300;
    double temperatureFactor = 0.99;

    while (true) {
        int creation = 0;
        std::cout << "Select options:\n";
        std::cout << "Any key - EXIT\n";
        std::cout << "1. Read from the file\n";
        std::cout << "2. Stop critera\n";
        std::cout << "3. Tabu Search - Critera\n";
        std::cout << "4. Tabu Search - Start\n";
        std::cout << "5. Simulated Annealing - Criteria\n";
        std::cout << "6. Simulated Annealing - Start\n";
        std::cin >> creation;

        switch (creation) {
        case 1:
        {
            std::string filename;
            std::cout << "Insert the filename: ";
            std::cin >> filename;
            lastFilename = filename;
            lastSolution.readFromFile(lastFilename);
            std::cout << std::endl;
        }
        break;

        case 2:
        {
            std::cout << "Insert the amount of time to stop in seconds: ";
            std::cin >> stopTime;
        }
        break;

        case 3:
        {
            
        }
        break;

        case 4:
        {
            
        }
        break;

        case 5:
        {
            
        }
        break;

        case 6:
        {
            
        }
        break;

        default:
            return;
        }
    }
    return;
}