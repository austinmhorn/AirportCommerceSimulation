#include "Sector.hpp"

std::ostream & operator << (std::ostream &out, const Sector &sector) {
    /* Print population amount if > 0 */
    if(sector.population > 0) {
        return( out << sector.population );
    }
    /* Otherwise, print Zone type character */
    else {
        return( out << (char) sector.type );
    }
}

bool operator == (Sector &lhs, Sector &rhs) {
    if(lhs.getPopulation() == rhs.getPopulation() &&
       lhs.getPollution()  == rhs.getPollution()  &&
       lhs.getX()          == rhs.getX()          &&
       lhs.getY()          == rhs.getY()          &&
       lhs.getZoneType()   == rhs.getZoneType() ) {
        return true;
    }
    return false;
}
