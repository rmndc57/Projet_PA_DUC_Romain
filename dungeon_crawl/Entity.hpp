#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include "Global.hpp"

#include <map>
using namespace std;

class Entity
{
public:
    Entity() { }

    Entity( const string& symbol, int colorCode )
    {
        Setup( symbol, colorCode );
    }

    void Setup( const string& symbol, int colorCode )
    {
        m_symbol = symbol;
        SetColor( colorCode );

        for ( int i = 0; i < 4; i++ )
        {
            m_elementInventory[(Element)i] = 0;
        }
    }

    void SetColor( int colorCode )
    {
        m_colorCode = colorCode;
    }

    int GetColor()
    {
        return m_colorCode;
    }

    void SetPosition( int x, int y )
    {
        m_x = x;
        m_y = y;
    }

    int GetX()
    {
        return m_x;
    }

    int GetY()
    {
        return m_y;
    }

    void SetSymbol( const string& symbol )
    {
        m_symbol = symbol;
    }

    void Draw( Terminal& terminal )
    {
        terminal.Draw( m_x, m_y, m_symbol, m_colorCode );
    }

    void DrawInventory( Terminal& terminal )
    {
        int x = 70, y = 0;
        for ( auto it = m_elementInventory.begin(); it != m_elementInventory.end(); it++ )
        {
            string invString = ElementToString( it->first ) + ": " + IntToStr( it->second );
            terminal.Draw( x, y, invString, terminal.GetColor( "black", "white" ) );
            y++;
        }
    }

    void Move( Direction dir )
    {
        switch( dir )
        {
        case UP:
            m_y -= 1;
            break;

        case DOWN:
            m_y += 1;
            break;

        case RIGHT:
            m_x += 1;
            break;

        case LEFT:
            m_x -= 1;
            break;

        default: break;
        }

        this->debordement();
    }

    void Mine( Direction dir, Element element )
    {
        m_elementInventory[element]++;
    }

protected:
    int m_x, m_y;
    string m_symbol;
    int m_colorCode;
    map<Element, int> m_elementInventory; // to change later

    void debordement()
    {
        if      ( m_x < 0 )
            m_x = 0;
        else if ( m_x >= SCREEN_WIDTH )
            m_x = SCREEN_WIDTH - 1;

        if      ( m_y < 0 )
            m_y = 0;
        else if ( m_y >= SCREEN_HEIGHT )
            m_y = SCREEN_HEIGHT - 1;
    }
};

#endif
