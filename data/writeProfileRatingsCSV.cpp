#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main()
{
    std::vector<std::string> names = {"Tony","Jackson","Rodger","Joe","Charles","David"};

    srand(time(nullptr));
    std::ofstream myfile;
    myfile.open ("profileRatings.csv");
    for(auto& name : names)
    {
        myfile << name;
        for(int i = 0; i < 98791; ++i)
        {
            int num = rand() % 3 + -1; 
            myfile << "," << num;
        }
        myfile << "\n";
    }
    myfile.close();
}