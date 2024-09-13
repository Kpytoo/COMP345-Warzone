#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
int testLoadMaps(std::string fname)
{
    try
    {
        std::ifstream MyFile(fname);
        MyFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        if (!MyFile.is_open())
        {
            throw std::runtime_error("File could not be opened: " + fname);
        }
    }

    catch (const std::exception &e)
    {

        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
};