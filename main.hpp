// I/O to the terminal
#include <iostream>
// Char * [] from C++ STL Library
#include <string>
// Vector will store all the AirportSector Objects within the airport
#include <vector>
// configuration settings values structure
#include <iomanip>
#include <fstream>
typedef struct ConfigurationValues
{
    std::string string_fileName;
    int int_timeLimit;
    int int_refreshRate;
} configValues;

// Functions for Importing the Configuration File
void ImportAirportConfigurationFile(std::string FileName, ConfigurationValues & configValues);

std::string string_ParseConfigurationFileFileName(std::string);
int int_ParseConfigurationFileTimeLimit(std::string);
int int_ParseConfigurationFileRefreshRate(std::string);
