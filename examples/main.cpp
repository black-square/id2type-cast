// MapIdToType.cpp : Defines the entry point for the console application.
//

#include <iostream>

void packetsTest();
void serializationTest();

int main()
{
    packetsTest(); 

    std::cout << "\n-------------------------------\n" << std::endl;
    serializationTest();
    std::cout << "\n-------------------------------\n" << std::endl;

    std::cin.get();

	return 0;
}