#include "Airport.hpp"

Airport::Airport() {
    //Grid.resize(0, std::vector<Sector*>(0, new Sector));
    rows         = 0;
    cols         = 0;
    availWorkers = 0;
    availGoods   = 0;
}

void Airport::populateAirport(ConfigurationValues configValues) {
    std::ifstream inFS;
    std::string line;
    Zone zone;
    std::vector<Sector*> gridRow;
    Sector* newSector;
    
    std::string zoneFile = "ExampleAirports/";
    zoneFile += configValues.string_fileName;
    
    try {
        inFS.open(zoneFile);
        if(!inFS.is_open()) {
            throw std::runtime_error("Input file stream failed to connect to data file");
        }
        else {
            while(getline(inFS, line)) {
                if(line.empty()) {
                    break;
                }
                gridRow.clear();
                
                this->cols = line.size() / 2;
                this->rows++;
                for(unsigned int i = 0; i < line.size()-1; i++) {
                    
                    zone = (Zone) line.at(i);
                    newSector = new Sector;
                    
                    switch(zone) {
                            /* For all cases where character being read from file is declared in Zone enumeration type, (defined in Sector.hpp)
                                
                             */
                        case BLANK:
                        case CARGO:
                        case DOMESTIC:
                        case INTERNATIONAL:
                        case POWERLINE:
                        case TERMINAL:
                        case POWERLINE_OVER_TERMINAL:
                        case POWER_PLANT:
                            newSector->setLocation(this->getRowSize(), (i/2)+1);
                            newSector->setZoneType(zone);
                            gridRow.push_back(newSector);
                            break;
                        default:
                            break;
                            
                    }
                }
                /* MARK: Push vector row into Grid */
                this->pushNewRow(gridRow);
            }
        }
    }
    catch(const std::runtime_error &excpt) {
        std::cout << "ERROR: " << excpt.what() << std::endl;
    }

    inFS.close();
}


void Airport::printAirport(bool printPollution) {
    /* MARK: Print entire airport to terminal */
    std::cout << '*' << std::setw(cols*2) << std::setfill('-') << '*' << std::endl;
    for(int i = 0; i < rows; i++) {
        std::cout << '|';
        for(int j = 0; j < cols; j++) {
            if(j+1 == cols) { /* Omits space if element is last in row */
                std::cout << *getGridElement(i, j);
            }
            else { /* Else, prints comma */
                std::cout << *getGridElement(i, j) << ' ';
            }
        }
        std::cout << '|' << std::endl;
    }
    std::cout << '*' << std::setw(cols*2) << std::setfill('-') << '*' << std::endl;
    std::cout << std::endl;
    
    /* Print Sector pollution value if applicable */
    if( printPollution == true ) {
        
        std::cout << "Pollution" << std::endl;
        std::cout << '*' << std::setw(cols*2) << std::setfill('-') << '*' << std::endl;
        for(int i = 0; i < rows; i++) {
            std::cout << '|';
            for(int j = 0; j < cols; j++) {
                if(j+1 == cols) { /* Omits space if element is last in row */
                    std::cout << this->getGridElement(i, j)->getPollution();
                }
                else { /* Else, prints comma */
                    std::cout << this->getGridElement(i, j)->getPollution() << ' ';
                }
            }
            std::cout << '|' << std::endl;
        }
        std::cout << '*' << std::setw(cols*2) << std::setfill('-') << '*' << std::endl;
        std::cout << std::endl;
    }
    else { return; }
}

void Airport::getTotalPopulations(int &D, int &C, int &I, bool setArea) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if( getGridElement(i, j)->getZoneType() == DOMESTIC ) {
                D+= getGridElement(i, j)->getPopulation();
            }
            else if( getGridElement(i, j)->getZoneType() == CARGO ) {
                C+= getGridElement(i, j)->getPopulation();
            }
            else if( getGridElement(i, j)->getZoneType() == INTERNATIONAL ) {
                I+= getGridElement(i, j)->getPopulation();
            }
        }
    }
}

void Airport::assignAdjacentSectors(int i,
                                    int j,
                                    std::vector<Sector*> &adjacentSectors,
                                    Sector** sectorAboveLeft,
                                    Sector** sectorAbove,
                                    Sector** sectorAboveRight,
                                    Sector** sectorToLeft,
                                    Sector** sectorToRight,
                                    Sector** sectorBelowLeft,
                                    Sector** sectorBelow,
                                    Sector** sectorBelowRight) {
    /* Check if FIRST row */
    if(i == 0) { // currSector is in FIRST row
        /* Check if FIRST column
            - currSector WILL NOT have adjacent sectors at:
                - Above Left
                - Above
                - Above Right
                - Left
                - Below Left
         */
        if(j == 0) {
            *sectorToRight    = this->getGridElement(i, j+1);
            *sectorBelow      = this->getGridElement(i+1, j);
            *sectorBelowRight = this->getGridElement(i+1, j+1);
            
            adjacentSectors.push_back(*sectorToRight);
            adjacentSectors.push_back(*sectorBelow);
            adjacentSectors.push_back(*sectorBelowRight);
        }
        /* Check if LAST column
            - currSector WILL NOT have adjacent sectors at:
                - Above Left
                - Above
                - Above Right
                - Right
                - Below Right
      */
        else if(j == cols-1) {
            *sectorToLeft    = this->getGridElement(i, j-1);
            *sectorBelowLeft = this->getGridElement(i+1, j-1);
            *sectorBelow     = this->getGridElement(i+1, j);
            
            adjacentSectors.push_back(*sectorToLeft);
            adjacentSectors.push_back(*sectorBelowLeft);
            adjacentSectors.push_back(*sectorBelow);
        }
        /* currSector is in FIRST row
            - currSector WILL NOT have adjacent sectors at:
                - Above Left
                - Above
                - Above Right
         */
        else {
            *sectorToLeft     = this->getGridElement(i, j-1);
            *sectorToRight    = this->getGridElement(i, j+1);
            *sectorBelowLeft  = this->getGridElement(i+1, j-1);
            *sectorBelow      = this->getGridElement(i+1, j);
            *sectorBelowRight = this->getGridElement(i+1, j+1);
            
            adjacentSectors.push_back(*sectorToLeft);
            adjacentSectors.push_back(*sectorToRight);
            adjacentSectors.push_back(*sectorBelowLeft);
            adjacentSectors.push_back(*sectorBelow);
            adjacentSectors.push_back(*sectorBelowRight);
        }
    }
    /* Check if LAST row */
    else if(i == rows-1) { // currSector is in LAST row
        /* Check if FIRST column
            - currSector WILL NOT have adjacent sectors at:
                - Above Left
                - Left
                - Below Left
                - Below
                - Below Right
         */
        if(j == 0) {
            *sectorAbove      = this->getGridElement(i-1, j);
            *sectorAboveRight = this->getGridElement(i-1, j+1);
            *sectorToRight    = this->getGridElement(i, j+1);
            
            adjacentSectors.push_back(*sectorAbove);
            adjacentSectors.push_back(*sectorAboveRight);
            adjacentSectors.push_back(*sectorToRight);
        }
        /* Check if LAST column
            - currSector WILL NOT have adjacent sectors at:
                - Above Right
                - Right
                - Below Left
                - Below
                - Below Right
         */
        else if(j == cols-1) {
            *sectorAboveLeft = this->getGridElement(i-1, j-1);
            *sectorAbove     = this->getGridElement(i-1, j);
            *sectorToLeft    = this->getGridElement(i, j-1);
            
            adjacentSectors.push_back(*sectorAboveLeft);
            adjacentSectors.push_back(*sectorAbove);
            adjacentSectors.push_back(*sectorToLeft);
        }
        /* currSector is in LAST ROW
            - currSector WILL NOT have adjacent sectors at:
                - Below Left
                - Below
                - Below Right
         */
        else {
            *sectorAboveLeft  = this->getGridElement(i-1, j-1);
            *sectorAbove      = this->getGridElement(i-1, j);
            *sectorAboveRight = this->getGridElement(i-1, j+1);
            *sectorToLeft     = this->getGridElement(i, j-1);
            *sectorToRight    = this->getGridElement(i, j+1);
            
            adjacentSectors.push_back(*sectorAboveLeft);
            adjacentSectors.push_back(*sectorAbove);
            adjacentSectors.push_back(*sectorAboveRight);
            adjacentSectors.push_back(*sectorToLeft);
            adjacentSectors.push_back(*sectorToRight);
        }
    }
    /* currSector is NOT in either FIRST OR LAST row */
    else {
        /* Check if FIRST column
            - currSector WILL NOT have adjacent sectors at:
                - Above Left
                - Below
                - Below Left
         */
        if(j == 0) {
            *sectorAbove      = this->getGridElement(i-1, j);
            *sectorAboveRight = this->getGridElement(i-1, j+1);
            *sectorToRight    = this->getGridElement(i, j+1);
            *sectorBelow      = this->getGridElement(i+1, j);
            *sectorBelowRight = this->getGridElement(i+1, j+1);
            
            adjacentSectors.push_back(*sectorAbove);
            adjacentSectors.push_back(*sectorAboveRight);
            adjacentSectors.push_back(*sectorToRight);
            adjacentSectors.push_back(*sectorBelow);
            adjacentSectors.push_back(*sectorBelowRight);
        }
        /* Check if LAST column
            - currSector WILL NOT have adjacent sectors at:
                - Above Right
                - Right
                - Below Right
         */
        else if(j == cols-1) {
            *sectorAboveLeft = this->getGridElement(i-1, j-1);
            *sectorAbove     = this->getGridElement(i-1, j);
            *sectorToLeft    = this->getGridElement(i, j-1);
            *sectorBelowLeft = this->getGridElement(i+1, j-1);
            *sectorBelow     = this->getGridElement(i+1, j);
            
            adjacentSectors.push_back(*sectorAboveLeft);
            adjacentSectors.push_back(*sectorAbove);
            adjacentSectors.push_back(*sectorToLeft);
            adjacentSectors.push_back(*sectorBelowLeft);
            adjacentSectors.push_back(*sectorBelow);
        }
        /* currSector
            - currSector has adjacent sectors on ALL SIDES
         */
        else {
            *sectorAboveLeft  = this->getGridElement(i-1, j-1);
            *sectorAbove      = this->getGridElement(i-1, j);
            *sectorAboveRight = this->getGridElement(i-1, j+1);
            *sectorToLeft     = this->getGridElement(i, j-1);
            *sectorToRight    = this->getGridElement(i, j+1);
            *sectorBelowLeft  = this->getGridElement(i+1, j-1);
            *sectorBelow      = this->getGridElement(i+1, j);
            *sectorBelowRight = this->getGridElement(i+1, j+1);
            
            adjacentSectors.push_back(*sectorAboveLeft);
            adjacentSectors.push_back(*sectorAbove);
            adjacentSectors.push_back(*sectorAboveRight);
            adjacentSectors.push_back(*sectorToLeft);
            adjacentSectors.push_back(*sectorToRight);
            adjacentSectors.push_back(*sectorBelowLeft);
            adjacentSectors.push_back(*sectorBelow);
            adjacentSectors.push_back(*sectorBelowRight);
        }
    }
}

void Airport::updateSectors(int timeStep) {
    Sector* currSector = nullptr;
    int numCarUpdating = 0;
    int numDomUpdating = 0;
    int numIntUpdating = 0;
    bool growthConflict = false;
    
    //std::cout<<"CAR: "<<numCarUpdating<<std::endl;
    //std::cout<<"DOM: "<<numDomUpdating<<std::endl;
    //std::cout<<"INT: "<<numIntUpdating<<std::endl;

    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            
            currSector = this->getGridElement(i, j);
            
            if( currSector->getTimeStepForSectorUpdate() == timeStep ) {
                
                switch( currSector->getZoneType() ) {
                        
                    case INTERNATIONAL:
                        numIntUpdating++;
                        break;
                    case CARGO:
                        numCarUpdating++;
                        break;
                    case DOMESTIC:
                        numDomUpdating++;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    if( numCarUpdating >= 1 && numIntUpdating >= 1 ) {
        if( numCarUpdating * 2 >= this->getAvailWorkers() ) {
            growthConflict = true;
        }
    }
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            
            currSector = getGridElement(i, j);
            
            if( currSector->getTimeStepForSectorUpdate() == timeStep ) {
                
                switch(currSector->getZoneType()) {
                    case INTERNATIONAL:
                        if( this->getAvailWorkers() >= 1 &&
                            this->getAvailGoods()   >= 1) {
                            /* Update this sector */
                            currSector->raisePopulation(1);
                            this->remWorkers(1);
                            this->remGoods(1);
                        }
                        break;
                    case CARGO:
                        if(this->getAvailWorkers() >= 2) {
                            if( this->getAvailWorkers() <= 2 && growthConflict == true ) { break; }
                            currSector->raisePopulation(1);
                            currSector->raisePollution(1);
                            this->remWorkers(2);
                            this->addGoods(1);
                        }
                        break;
                    case DOMESTIC:
                        currSector->raisePopulation(1);
                        this->addWorkers(1);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

bool Airport::futureUpdatesOccuring(int nextTimeStep) { /* MARK: Receives timeStep + 1 from performTimeStep() */
    // Iterate through 2D vector container to determine if any Sector object inside meets valid criteria to update
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            /* If Sector object's int data member timeStepUpdateOccurs is equal to nextTimeStep:
             *          - Return true: Updates remain to occur
             * Otherwise,
             *          - Proceed until a Sector DOES contain update scheduled for next time step
             *        OR,
                        - All sectors processed */
            if(this->getGridElement(i, j)->getTimeStepForSectorUpdate() == nextTimeStep) { return true; }
            else { continue; }
        }
    }
    /* No remaining Sector objects contained within 2D vector require further updates to the cell. Initiates Termination. */
    return false;
}

bool Airport::performTimeStep(int timeStep) {
    // Store current sector being examined
    Sector* currSector;
    /* Store adjacent sectors  */
    std::vector<Sector*> adjacentSectors;
    std::vector<Sector*>::iterator iter;
    /* Pointers to adjacent sectors within boundaries */
    Sector* sectorAboveLeft;
    Sector* sectorAbove;
    Sector* sectorAboveRight;
    Sector* sectorToLeft;
    Sector* sectorToRight;
    Sector* sectorBelowLeft;
    Sector* sectorBelow;
    Sector* sectorBelowRight;
    /* Track adjacent sector population amounts */
    int adjSectorsWithOnePop   = 0;
    int adjSectorsWithTwoPop   = 0;
    int adjSectorsWithThreePop = 0;
    int adjSectorsWithFourPop  = 0;

    //
    //
    // MARK: Iterate through 2D vector
    //
    //
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            
            // Point currSector to element in Grid
            currSector = this->getGridElement(i, j);
            // Reset adjacent sectors
            sectorAboveLeft  = nullptr;
            sectorAbove      = nullptr;
            sectorAboveRight = nullptr;
            sectorToLeft     = nullptr;
            sectorToRight    = nullptr;
            sectorBelowLeft  = nullptr;
            sectorBelow      = nullptr;
            sectorBelowRight = nullptr;
            // Reset multi-purpose adjacent sector validation counters
            adjSectorsWithOnePop   = 0;
            adjSectorsWithTwoPop   = 0;
            adjSectorsWithThreePop = 0;
            adjSectorsWithFourPop  = 0;
            
            // Empty vector containing adjacent sectors
            adjacentSectors.clear();
            
            /* Call function to assign adjacent sector pointers to point to respective adjacent sector locations */
            assignAdjacentSectors(i,
                                  j,
                                  adjacentSectors,
                                  &sectorAboveLeft,
                                  &sectorAbove,
                                  &sectorAboveRight,
                                  &sectorToLeft,
                                  &sectorToRight,
                                  &sectorBelowLeft,
                                  &sectorBelow,
                                  &sectorBelowRight);
            
            /* Read adjacent sectors populations */
            for(iter = adjacentSectors.begin(); iter != adjacentSectors.end(); ++iter) {
                switch((*iter)->getPopulation()) {
                    case 0: // If adjacent sector (to currSector) has population of zero, currSector's status is unaffected
                        break;
                    case 1: // Adjacent sector has population value of 1
                        // Increment counter variable responsible for tracking currSector's adjacent sectors with a population value of 1 or less
                        adjSectorsWithOnePop++;
                        break;
                    case 2: // Adjacent sector has population value of 2
                        // Increment counter(s) variable responsible for tracking currSector's adjacent sectors with a population value of 2 or less
                        adjSectorsWithTwoPop++;
                        adjSectorsWithOnePop++;
                        break;
                    case 3: // Adjacent sector has population value of 3
                        // Increment counter(s) variable responsible for tracking currSector's adjacent sectors with a population value of 3 or less
                        adjSectorsWithThreePop++;
                        adjSectorsWithTwoPop++;
                        adjSectorsWithOnePop++;
                        break;
                    case 4: // Adjacent sector has population value of 4
                        // Increment counter(s) variable responsible for tracking currSector's adjacent sectors with a population value of 4 or less
                        adjSectorsWithFourPop++;
                        adjSectorsWithThreePop++;
                        adjSectorsWithTwoPop++;
                        adjSectorsWithOnePop++;
                        break;
                        /* MARK: No event behavior for sectors containing population value of 5 or more */
                    default:
                        break;
                }
            }
            
            //
            // Determine if update to currSector should be initiated to take place the following time step.
            // Criteria for update to currSector is determined by its Zone type -- determined inside populateAirport()
            // (Enumerated data type Zone is defined in Sector.hpp)
            //
            switch(currSector->getZoneType()) {
                case INTERNATIONAL:
                    switch(currSector->getPopulation()) {
                        case 0:
                            /* Iterate through adjacent sectors of currSector searching for Sector of Zone type POWERLINE */
                            for(iter = adjacentSectors.begin(); iter != adjacentSectors.end(); ++iter) {
                                
                                /* If an adjacent sector is of enumerated constant Zone type POWERLINE
                                    AND there is 1 or more available workers provided by Domestic Zones,
                                    AND there is 1 or more available good(s) provided by Cargo Zones */
                                if( ( (*iter)->getZoneType() == POWERLINE || (*iter)->getZoneType() == POWERLINE_OVER_TERMINAL ) &&
                                    this->getAvailWorkers()  >= 1 &&
                                    this->getAvailGoods()    >= 1 ) {
                                    /* Assigns Sector object's data member timeStepUpdateOccurs with value of next time step  */
                                    currSector->setTimeStepForSectorUpdate(timeStep + 1);
                                    currSector->setNearPowerline();
                                }
                            }
                            /* One or more adjacent sectors contain a population value of 1 of higher */
                            if( adjSectorsWithOnePop    >= 1 &&
                                this->getAvailWorkers() >= 1 &&
                                this->getAvailGoods()   >= 1 ) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        case 1:
                            if( adjSectorsWithOnePop    >= 2 &&
                                this->getAvailWorkers() >= 1 &&
                                this->getAvailGoods()   >= 1 ) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case CARGO:
                    switch(currSector->getPopulation())
                    {
                        case 0:
                            /* Iterate through adjacent sectors of currSector searching for Sector of Zone type POWERLINE */
                            for(iter = adjacentSectors.begin(); iter != adjacentSectors.end(); ++iter) {
                                
                                /* If an adjacent sector is of enumerated constant Zone type POWERLINE
                                    AND there are 2 or more available workers provided by Domestic Zones */
                                if( ( (*iter)->getZoneType() == POWERLINE || (*iter)->getZoneType() == POWERLINE_OVER_TERMINAL ) &&
                                   this->getAvailWorkers() >= 2) {
                                    /* Assigns Sector object's data member timeStepUpdateOccurs with value of next time step  */
                                    currSector->setTimeStepForSectorUpdate(timeStep + 1);
                                    currSector->setNearPowerline();
                                }
                            }
                            /* One or more adjacent sectors contain a population value of 1 of higher */
                            if(adjSectorsWithOnePop >= 1 &&
                               this->getAvailWorkers() >= 2) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        case 1:
                            /* Two or more adjacent sectors contain a population value of 1 of higher */
                            if(adjSectorsWithOnePop >= 2 &&
                               this->getAvailWorkers() >= 2) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        case 2:
                            /* Four or more adjacent sectors contain a population value of 2 of higher */
                            if(adjSectorsWithTwoPop >= 4 &&
                               this->getAvailWorkers() >= 2) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case DOMESTIC:
                    // Switch case based on currSector population amount
                    switch(currSector->getPopulation()) {
                        case 0:
                            /* Iterate through adjacent sectors of currSector searching for Sector of Zone type POWERLINE */
                            for(iter = adjacentSectors.begin(); iter != adjacentSectors.end(); ++iter) {
                                
                                /* If an adjacent sector is eligible, check if update is set to occur this time step */
                                if( (*iter)->getZoneType() == POWERLINE || (*iter)->getZoneType() == POWERLINE_OVER_TERMINAL ) {
                                    /* Assigns Sector object's data member timeStepUpdateOccurs with value of next time step  */
                                    currSector->setTimeStepForSectorUpdate(timeStep + 1);
                                }
                            }
                            /* One or more adjacent sectors contain a population value of 1 of higher */
                            if(adjSectorsWithOnePop >= 1) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        case 1:
                            /* Two or more adjacent sectors contain a population value of 1 of higher */
                            if(adjSectorsWithOnePop >= 2) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        case 2:
                            /* Four or more adjacent sectors contain a population value of 2 of higher */
                            if(adjSectorsWithTwoPop >= 4) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        case 3:
                            if(adjSectorsWithThreePop >= 6) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        case 4:
                            if(adjSectorsWithFourPop >= 8) {
                                currSector->setTimeStepForSectorUpdate(timeStep + 1);
                            }
                            break;
                        default:
                            /* Do nothing */
                            break;
                    }
                    break;
                default:
                    break;
                    
            }
        }
    }
    
    return this->futureUpdatesOccuring(timeStep+1) ? true : false;
}
