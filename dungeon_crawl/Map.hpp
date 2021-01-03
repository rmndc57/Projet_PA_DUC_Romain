#ifndef _MAP_HPP
#define _MAP_HPP

#include "Entity.hpp"
#include "Global.hpp"

#include <vector>
using namespace std;

struct Room
{
    int x, y;
    int expandX, expandY;
};

class Map
{
public:
    Map() { }
    
    virtual ~Map() { }

    void Setup( int plafond, int mur, int sol, int roches )
    {
        m_ceilingColorCode = plafond;
        m_wallColorCode = mur;
        m_floorColorCode = sol;
        m_rockColorCode = roches;
    }

    int GetFloorColor()
    {
        return m_floorColorCode;
    }

    void GenerateMap( Terminal& terminal )
    {
        m_rooms.clear();

        // Create empty tiles first...
        for ( int y = 0; y < SCREEN_HEIGHT; y++ )
        {
            for ( int x = 0; x < SCREEN_WIDTH; x++ )
            {
                m_tiles[x][y].Setup( " ", m_ceilingColorCode );
                m_tiles[x][y].SetPosition( x, y );
            }
        }

        // Create rooms
        int roomCount = randint( 5, 10 );
        string roomSymbol = "";
        LOG << "Room count: " << roomCount << endl;

        for ( int i = 0; i < roomCount; i++ )
        {
            roomSymbol = IntToStr( i );

            Room newRoom;
            // Choose a location for this room
            newRoom.x = randint( 0, SCREEN_WIDTH );
            newRoom.y = randint( 1, SCREEN_HEIGHT ); // Make space for a wall on top

            // Expand the room size around the centerpoint
            newRoom.expandX = randint( 3, 10 );
            newRoom.expandY = randint( 4, 8 );

            for ( int ex = 0; ex < newRoom.expandX; ex++ )
            {
                for ( int ey = 0; ey < newRoom.expandY; ey++ )
                {
                    if ( newRoom.x + ex < SCREEN_WIDTH && newRoom.y + ey < SCREEN_HEIGHT )
                        m_tiles[newRoom.x + ex][newRoom.y + ey].Setup( roomSymbol, m_floorColorCode );
                }
            }

            m_rooms.push_back( newRoom );
        }

        // creer des chemins entre les salles 0 -> 1, 1 -> 2, etc.
        int x;
        int y;
        for ( auto i = 0; i < m_rooms.size() - 1; i++ )
        {
            LOG << endl << "Attach room " << i << " to room " << i+1 << "..." << endl;
            LOG << "\t Room " << i     << ": " << m_rooms[i].x << ", " << m_rooms[i].y << endl;
            LOG << "\t Room " << i+1   << ": " << m_rooms[i+1].x << ", " << m_rooms[i+1].y << endl;

            roomSymbol = IntToStr( i );

            int startX = m_rooms[i].x, startY = m_rooms[i].y;

            int endX = m_rooms[i+1].x, endY = m_rooms[i+1].y;

            x = startX;
            y = startY;

            // Move X first, then Y.
            while ( x != endX )
            {
                m_tiles[x][y].Setup( roomSymbol, m_floorColorCode );

                if ( endX < x )
                    x--;

                else
                    x++;
            }

            while ( y != endY )
            {
                m_tiles[x][y].Setup( roomSymbol, m_floorColorCode );

                if ( endY < y )
                    y--;
                
                else
                    y++;
            }

            LOG << endl << endl;
        }

        //generer les murs autour des pièces (sol)
        for ( int y = 0; y <= SCREEN_HEIGHT - 1; y++ )
        {
            for ( int x = 0; x <= SCREEN_WIDTH - 1; x++ )
            {
                /*cas du sol*/
                if ( m_tiles[x][y].GetColor() == m_floorColorCode )
                {
                    /*s'il y a un plafond a proximite directe, le remplacer avec un mur */
                    if ( !(y-1 < 0) && m_tiles[x][y-1].GetColor() == m_ceilingColorCode )
                        m_tiles[x][y-1].SetColor( m_wallColorCode );
                }
            }
        }

        // génère des rochers sur le sol
        int rockCount = randint( 5, 10 );
        for ( int i = 0; i <= rockCount - 1; i++ )
        {
            //choisir une salle de façon aléatoire
            int whichRoom = randint( 0, m_rooms.size() );

            int x = randint( m_rooms[whichRoom].x, m_rooms[whichRoom].x + m_rooms[whichRoom].expandX );
            int y = randint( m_rooms[whichRoom].y, m_rooms[whichRoom].y + m_rooms[whichRoom].expandY );

            m_tiles[x][y].Setup( "@", m_rockColorCode );
        }
    }

    Room& GetFirstRoom()
    {
        return m_rooms[0];
    }

    Room& GetLastRoom()
    {
        return m_rooms[m_rooms.size() - 1];
    }

    TileType GetTileType( int abs, int ord, Direction dir )
    {
        int x = abs;
        int y = ord;
		
		switch(dir){
			case UP:
				y--;
				break;
			case DOWN:
				y++;
				break;
			case LEFT:
				x--;
				break;
			case RIGHT:
				x++;
				break;
			default:
				break;
		}
        

        if ( !(x>=0) || !(y>=0) || !(x < SCREEN_WIDTH) || !(y < SCREEN_HEIGHT) )
        {
            return OUTOFBOUNDS;
        }

        if      ( m_tiles[x][y].GetColor() == m_floorColorCode )
        	return SOL; 
        else if ( m_tiles[x][y].GetColor() == m_wallColorCode )
        	return MUR;
        else if ( m_tiles[x][y].GetColor() == m_ceilingColorCode )
        	return PLAFOND;
        else if ( m_tiles[x][y].GetColor() == m_rockColorCode )
        	return ROCHE;
    }

    void Draw( Terminal& terminal )
    {
        for ( int y = 0; y < SCREEN_HEIGHT; y++ )
        {
            for ( int x = 0; x < SCREEN_WIDTH; x++ )
                m_tiles[x][y].Draw( terminal );
        }
    }

    void Mine( int x, int y, Direction direction )
    {
        
        if ( direction == UP )
            m_tiles[x][y-1].Setup( " ", m_floorColorCode );
        
        else if ( direction == DOWN )
            m_tiles[x][y+1].Setup( " ", m_floorColorCode );
        
        else if ( direction == RIGHT )
            m_tiles[x+1][y].Setup( " ", m_floorColorCode );
            
        else if ( direction == LEFT )
            m_tiles[x-1][y].Setup( " ", m_floorColorCode );
    }

private:
    Entity m_tiles[SCREEN_WIDTH][SCREEN_HEIGHT];
    vector<Room> m_rooms;

    int m_floorColorCode;
    int m_wallColorCode;
    int m_ceilingColorCode;
    int m_rockColorCode;
};

#endif

/*
_MAP_HPP included
*/
