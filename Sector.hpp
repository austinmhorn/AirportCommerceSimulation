#ifndef Sector_hpp
#define Sector_hpp
#include <iostream>

typedef struct {int x; int y;} Point;

enum Zone {
    BLANK                   = ' ',
    CARGO                   = 'C',
    DOMESTIC                = 'D',
    INTERNATIONAL           = 'I',
    POWERLINE               = 'T',
    TERMINAL                = '-',
    POWERLINE_OVER_TERMINAL = '#',
    POWER_PLANT             = 'P',
    UNKNOWN                 = '?'  /* Assigned at object creation; Always reassigned */
};


class Sector {
public:
    Sector() {
        population =  0;
        pollution  =  0;
        location.x = -1;
        location.y = -1;
        type = UNKNOWN;
        timeStepUpdateOccurs = -1;
        nextToPowerline = false;
    }
    
    /* MARK: Mutators */
    void setTimeStepForSectorUpdate(int timeStep) { this->timeStepUpdateOccurs = timeStep;      }
    void raisePopulation(int n)                   { this->population += n;                      }
    void raisePollution(int n)                    { this->pollution += n;                       }
    void setLocation(int X, int Y)                { this->location.x = X; this->location.y = Y; }
    void setZoneType(Zone zoneType)               { this->type = zoneType;                      }
    void setNearPowerline()                       { this->nextToPowerline = true;               }
    
    /* MARK: Accessors */
    int getTimeStepForSectorUpdate() { return this->timeStepUpdateOccurs; }
    int getPopulation()              { return this->population;           }
    int getPollution()               { return this->pollution;            }
    int getX()                       { return this->location.x;           }
    int getY()                       { return this->location.y;           }
    Zone getZoneType()               { return this->type;                 }
    bool isNearPowerLine()           { return this->nextToPowerline;      }
    
    /* MARK: Overloaded Operators */
    /* Overloaded ostream OP output can vary -- see Sector.cpp */
    friend std::ostream & operator << (std::ostream &out, const Sector &sector);
    friend bool operator == (const Sector &lhs, const Sector &rhs);
    
protected:
    int population;
    int pollution;
    Point location;
    Zone type;
    int timeStepUpdateOccurs;
    bool nextToPowerline;
};


#endif /* Sector_hpp */
