#ifndef _GLOBAL_HPP
#define _GLOBAL_HPP

#include <fstream>
#include <sstream>
using namespace std;

#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 24;
ofstream LOG;

enum Direction { UNSET, UP, DOWN, RIGHT, LEFT };
enum TileType { OUTOFBOUNDS, PLAFOND, MUR, SOL, ROCHE, ENNEMI };
enum Element { CHARBON=0, FER=1, ARGENT=2, OR=3 };


int randint( int borninf, int bornsup )
{
	if(!(borninf < bornsup))
		return borninf;
	else
    	return rand() % (bornsup - borninf) + borninf;
}

Element GetRandomElement()
{
    int type = randint( 0, 4 );
    return (Element)type;
}

string ElementToString( Element element )
{
	const char* res;
	
    if      ( element == CHARBON )
    	res = "Charbon";
    else if ( element == FER )
    	res = "Fer"; 
    else if ( element == ARGENT )
    	res = "Argent"; 
    else if ( element == OR )
    	res = "Or"; 
    
    return res;
    
}

int min( int a, int b )
{
    return (a<b)? a : b;
}

int max( int a, int b )
{
    return (a<=b)? b : a;
}

string IntToStr( int value )
{
    stringstream ss;
    ss << value;
    return ss.str();
}

#endif

/*_GLOBAL_HPP included*/

