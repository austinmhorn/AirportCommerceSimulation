#include "Airport.hpp"
#include <ctime>
#include <iostream>

class Simulation {
public:
    Airport airport;
    int totalDomPop, totalCarPop, totalIntPop;
    int numWorkers, numGoods;
    
    Simulation() {
        totalDomPop  = 0;
        totalCarPop  = 0;
        totalIntPop  = 0;
        numWorkers   = 0;
        numGoods     = 0;
    }
    
    void initSimulation(ConfigurationValues configValues) { airport.populateAirport(configValues); }
    
    bool needsUpdate(ConfigurationValues configValues, int timeStep) { return ( airport.performTimeStep(timeStep) ) ? true : false; }
    
    void start(ConfigurationValues configValues) {
        /* Print Initial Zone State for Airport object */
        std::cout << "Initial Zone State" << std::endl;
        airport.printAirport();
        
        /* Numerical counter that tracks and increments time step with each iteration */
        int timeStep = 0;
        /* Declare counter variable that when equivalent to the refresh rate (supplied by configValues),
            prints Airport's 2D vector (Grid) to terminal using the printAirport() function. */
        int refreshCount = 0;
    
        /* MARK: Perform new time step with each iteration of this loop checking if updates should occur.  */
        for(unsigned int timeLimit = configValues.int_timeLimit; timeLimit > 0; timeLimit--) {
                        
            /* MARK: Verify if update needs to be performed on any Sector contained in Airport's Grid */
            if (this->needsUpdate(configValues, timeStep) == true) {
                                
                if(timeStep > 0) {
                    /* Inform user the current time step's available workers and goods */
                    std::cout << "Time Step: " << timeStep << std::endl;
                    std::cout << "Available Workers: " << airport.getAvailWorkers() << " Available Goods: " << airport.getAvailGoods() << std:: endl;
                }
                
                /* Print Grid to terminal when refresh counter variable is equivalent to refreshRate provided by
                    data structure configValues. Also, when branch executes, reset refresh count variale. */
                if(refreshCount == configValues.int_refreshRate) {
                    refreshCount = 0;
                    airport.printAirport();
                }
                
                /* Increment time step counter for new loop iteration */
                timeStep++;
                
                /* MARK: Perfrom updates for Sectors that met required criteria in previous time step */
                airport.updateSectors(timeStep);
                
                /* Controlling the execution of if-statement that determines
                    whether the Airport object is printed to the terminal. */
                refreshCount++;
            }
            /* 2D vector contains ZERO remaining sectors meeting valid criteria
                for cell data to ne updated; for-loop is terminate */
            else {
                /* Print final time step data */
                this->numWorkers = airport.getAvailWorkers();
                this->numGoods   = airport.getAvailGoods();
                std::cout << "Time Step: " << timeStep << std::endl;
                std::cout << "Available Workers: " << numWorkers << " Available Goods: " << numGoods << std:: endl;
                airport.printAirport();
                break;
            }
        }
        
        /* Print Final Region State for Airport object's 2D vector container member named Grid */
        std::cout << "Final Region State" << std::endl;
        airport.printAirport(true);
        
        /* TODO: Print pollution for Cargo Sectors */
        
    }
    
    void printResults() {
        airport.getTotalPopulations(totalDomPop, totalCarPop, totalIntPop);
        std::cout << "The total populations for the zone are:" << std::endl;
        std::cout << "Domestic: " << totalDomPop << std::endl;
        std::cout << "Cargo: " << totalCarPop << std::endl;
        std::cout << "International: " << totalIntPop << std::endl;
        std::cout << "The total amount of pollution im the zone is " << totalCarPop + totalDomPop + totalIntPop << " units." << std::endl;
    }
    
    void inspectArea() {
        int x1, y1, x2, y2;
        int totalCarPopInArea = 0;
        int totalDomPopInArea = 0;
        int totalIntPopInArea = 0;
        
        std::cout << "Please enter the diagonal corners of the area you wish to have more information about.";
        std::cout << "(MinX = 0, MinY = 0, MaxX = " << airport.getRowSize() - 1 << ", MaxY = " << airport.getColSize() - 1 << ")" << std::endl;
        std::cout << "X1:"; std::cin >> x1;
        std::cout << "Y1:"; std::cin >> y1;
        std::cout << "X2:"; std::cin >> x2;
        std::cout << "Y2:"; std::cin >> y2;

        for(int i = 0; i < airport.getRowSize(); i++) {
            for(int j = 0; j < airport.getColSize(); j++) {
                if( (i >= x1 && i <= x2) && (j >= y1 && j <= y2) ) {
                    switch(airport.getGridElement(i, j)->getZoneType()) {
                        case CARGO:
                            totalCarPopInArea += airport.getGridElement(i, j)->getPopulation();
                            break;
                        case DOMESTIC:
                            totalDomPopInArea += airport.getGridElement(i, j)->getPopulation();
                            break;
                        case INTERNATIONAL:
                            totalIntPopInArea += airport.getGridElement(i, j)->getPopulation();
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        
        std::cout << "The total populations for the zone are:" << std::endl;
        std::cout << "Domestic: " << totalDomPopInArea << std::endl;
        std::cout << "Cargo: " << totalCarPopInArea << std::endl;
        std::cout << "International: " << totalIntPopInArea << std::endl;
        std::cout << "The total amount of pollution im the zone is " << totalCarPopInArea + totalDomPopInArea + totalIntPopInArea << " units." << std::endl;
    }
};

int main(int argc, const char * argv[]) {
 
    // Declare simulator object of type Simulation for Airport object creation/handling
    Simulation simulator;
    std::string path = "ExampleAirports/";
    // Store the line of text that was entered into Command Prompt by user
    std::string fileName;
    // Declare variable of type-defined data struct ConfigurationValues
    // Stores appropriate data read from configiguration file (timeLimit, refreshRate, zone.csv)
    ConfigurationValues configValues;
    
    std::cout << "Beginning simulation" << std::endl;
    std::cout << "Please enter the name of the configuration file:";
    getline(std::cin, fileName); std::cout << std::endl;
    
    path += fileName;
    /* Read configuration file and store relevant data */
    ImportAirportConfigurationFile(path, configValues);
    
    /* Initialize simulation:
     *  - See more detail on functions of ImportAirportConfigurationFile at definition below
     - Populates airport (represented by 2D vector) by reading .csv file */
    simulator.initSimulation(configValues);
    
    /* Start simulation */
    simulator.start(configValues);
    
    /* Print final report:
        - Final Region State
        - Pollution
        - Total Populations */
    simulator.printResults();
    
    simulator.inspectArea();
    
    std::cout << "Simulation complete" << std::endl;

    return 0;
}

/* Import configurations settings for the simualtion from .txt file

    If airport simulation configuration file was successfully uploaded then
    program will give a successfully uploaded message and then import the
    configuration settings from the simulation configuration file.
    If airport simulation configuration file failed then the program will spit out
    a failure message followed by whatever it can see within the buffer.

    The importingairport simulation confiuration process:
        read in File Name and save it in string variable called string_ConfigurationFileNameFromConfigurationFile
        read in time limit and save it in a int variable called int_TimeLimit
        read in refresh rate and save it in a int varible called int_RefreshRate

*/
void ImportAirportConfigurationFile(std::string FileName, ConfigurationValues &configValues)
{
    std::ifstream ifstream_importAirportSimulationConfigurationFile(FileName);
    try {
        if (ifstream_importAirportSimulationConfigurationFile.bad())
        {
            throw std::runtime_error("Airport Simulation File Configuration Upload Failed!");
            std::cout << ifstream_importAirportSimulationConfigurationFile.rdbuf();
            ifstream_importAirportSimulationConfigurationFile.close();
        }
    }
    catch(const std::runtime_error &excpt) {
        std::cout << std::endl << "ERROR: " << excpt.what() << std::endl;
    }

    std::string string_fileName;
    std::getline(ifstream_importAirportSimulationConfigurationFile, string_fileName);
    configValues.string_fileName = string_ParseConfigurationFileFileName(string_fileName);
    
    std::string string_timeLimit;
    std::getline(ifstream_importAirportSimulationConfigurationFile, string_timeLimit);
    configValues.int_timeLimit = int_ParseConfigurationFileTimeLimit(string_timeLimit);
    
    std::string string_refreshRate;
    std::getline(ifstream_importAirportSimulationConfigurationFile, string_refreshRate);
    configValues.int_refreshRate = int_ParseConfigurationFileRefreshRate(string_refreshRate);
    
    ifstream_importAirportSimulationConfigurationFile.close();
}

// Indivdual Parsing Functions for the ImportAirportConfigurationFile() Function
std::string string_ParseConfigurationFileFileName(std::string string_fileName)
{
    return string_fileName.substr(14);
}
int int_ParseConfigurationFileTimeLimit(std::string string_timeLimit)
{
    std::string string_parsedtimeLimitstring = string_timeLimit.substr(11);
    return std::stoi(string_parsedtimeLimitstring);
}
int int_ParseConfigurationFileRefreshRate(std::string string_refreshRate)
{
    std::string string_parsedrefreshRatestring = string_refreshRate.substr(13);
    return std::stoi(string_parsedrefreshRatestring);
}

