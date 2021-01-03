#ifndef _TERMINAL_HPP
#define _TERMINAL_HPP

#include "Global.hpp"

#include <ncurses.h>

#include <map>
#include <string>
using namespace std;

class Terminal
{
public:
    Terminal()
    {
        Setup();
    }

    ~Terminal()
    {
        Teardown();
    }

    void Setup()
    {
        initscr();
        raw();
        keypad( stdscr, TRUE );			//capture special keystrokes like the four arrows by getch()
        noecho();		//suppress the automatic echoing of typed characters 

        if ( has_colors() )
            start_color();

        m_done = false;
        m_lastBackground = "";
        m_lastForeground = "";
        m_lastColor = -1;

        SetupColors();
    }

    void Teardown()
    {
        endwin();
    }

    int GetKey()
    {
        return getch();
    }

    void StartDraw()
    {
        clear();
    }

    void EndDraw()
    {
        refresh();
    }


    int GetColor( const string& background, const string& foreground )
    {
        string colorName = background + "-" + foreground;
        return m_colors[colorName];
    }

    void StartColor( const string& background, const string& foreground )
    {
        if ( m_lastBackground != "" )
            EndColor();

        m_lastBackground = background;
        m_lastForeground = foreground;

        attron( COLOR_PAIR( GetColor( background, foreground ) ) );
    }

    void StartColor( int number )
    {
        if ( m_lastColor != -1 )
            EndColor();

        m_lastColor = number;
        attron( COLOR_PAIR( m_lastColor ) );
    }

    void EndColor()
    {
        if ( m_lastBackground != "" )
        {
            attroff( COLOR_PAIR( GetColor( m_lastBackground, m_lastForeground ) ) );
            m_lastBackground = "";
            m_lastForeground = "";
        }
        else if ( m_lastColor != -1 )
        {
            attroff( COLOR_PAIR( GetColor( m_lastBackground, m_lastForeground ) ) );
            m_lastColor = -1;
        }
    }

    void Draw( int x, int y, const string& symbol )
    {
        mvprintw( y, x, symbol.c_str() );
    }

    void Draw( int x, int y, const string& symbol, int colorCode )
    {
        StartColor( colorCode );
        Draw( x, y, symbol );
        EndColor();
    }

    /*void Draw( int x, int y, const string& symbol, const string& background, const string& foreground )
    {
        StartColor( background, foreground );
        Draw( x, y, symbol );
        EndColor();
    }*/

private:
    bool m_done;
    map<string,int> m_colors;
    string m_lastBackground;
    string m_lastForeground;
    int m_lastColor;

    void SetupColors()
    {
        // Black = 0, Red = 1, Green = 2, Yellow = 3,
        // Blue = 4, Magenta = 5, Cyan = 6, White = 7

        string colorNames[] =
        {
            "noir", "rouge", "vert", "jaune",
            "bleu", "magenta", "cyan", "blanc"
        };

       	/*
       	Enregistrer toutes les combinaisons possibles
       	*/
        int counter = 10;
        for ( int fg = 0; fg < 8; fg++ )
        {
            for ( int bg = 0; bg < 8; bg++ )
            {
                string colorName = colorNames[bg] + "-" + colorNames[fg];
                init_pair( counter, fg, bg );

                LOG << "Color #" << counter << " = " << colorName << endl;

                m_colors[colorName] = counter;
                counter++;
            }
        }
    }
};

#endif
//_TERMINAL_HPP INCLUDED

