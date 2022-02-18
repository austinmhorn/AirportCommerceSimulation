#ifndef Airport_hpp
#define Airport_hpp
#include "main.hpp"
#include "Sector.hpp"

class Airport {
public:
    /* Class Constructor */
    Airport();
    
    /*  1) Opens the .csv file determined by data member of required parameter
     *      configValues (member: configValues.string_fileName) of type-defined
     *      structure ConfigurationValues (defined in main.hpp).
     *  2) Retrieves line from file (until line is empty) and iterates through
     *      the characters in the string.
     *  3) Creates new Sector object for each characters read that is (also)
     *      assigned a constant expression in the enumerated data type Zone
     *      (defined in Sector.hpp).
     *  4) Pushes the new Sector object created into 1D vector, serving as row
            construction for 2D vector container Grid. */
    void populateAirport(ConfigurationValues configValues);
    
    /*  Iterates through this Airport object's 2D vector, Grid; print each
     *  element to terminal, flushing and ending line at the end of each row.
            - Dynamically adjusts frame size for varying 2D vectors row and column sizes. */
    void printAirport(bool printPollution = false);
    
    /*  1) Point eligible adjacent Sectors to adjacent Sector locations;
     *  2) Sector** container to store (MIN:3, MAX:8) adjacent Sector**'s for all Sectors in the Airport f */
    void assignAdjacentSectors(int i, int j,
                               std::vector<Sector*> &adjacentSectors,
                               Sector** sectorAboveLeft, Sector** sectorAbove, Sector** sectorAboveRight,
                               Sector** sectorToLeft, Sector** sectorToRight,
                               Sector** sectorBelowLeft, Sector** sectorBelow, Sector** sectorBelowRight);
    
    /* Called at end of performTimeStep() cehcking if any Sector meets its criteria to be updated */
    bool futureUpdatesOccuring(int timeStep);
    /* MARK: Responsible for performing updates to any Sectors in airport contianer when criteria is met */
    void updateSectors(int timeStep);
    /* MARK: Responsible for evaluation of Sectors in sirport container of for its respsective Zone criteria  */
    bool performTimeStep(int timeStep);
    /* Performs construction of 2D vector container by inserting vector into vector */
    void pushNewRow(std::vector<Sector*> &newRow) { this->Grid.push_back(newRow); }
    void getTotalPopulations(int &D, int &C, int &I, bool setArea = false);
    
    /* MARK: Mutators */
    void addWorkers(int workersToAdd)     { this->availWorkers += workersToAdd;    }
    void remWorkers(int workersToRemove)  { this->availWorkers -= workersToRemove; }
    void addGoods(int goodsToAdd)         { this->availGoods += goodsToAdd;        }
    void remGoods(int goodsToRemove)      { this->availGoods -= goodsToRemove;     }
    void setRowSize(unsigned int numRows) { this->rows = numRows;                  }
    void setColSize(unsigned int numCols) { this->cols = numCols;                  }
    

    /* MARK: Accessors */
    Sector* getGridElement(int i, int j) { return this->Grid[i][j]; }
    unsigned int getRowSize()            { return this->rows; }
    unsigned int getColSize()            { return this->cols; }
    int getAvailWorkers()                { return this->availWorkers; }
    int getAvailGoods()                  { return this->availGoods; }
    
    
private:
    std::vector<std::vector<Sector*> > Grid;
    unsigned int rows;
    unsigned int cols;
    int availWorkers;
    int availGoods;
};

#endif /* Airport_hpp */
