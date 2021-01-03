#include "Terminal.hpp"
#include "Entity.hpp"
#include "Global.hpp"
#include "Map.hpp"

#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	bool fin = false;
    srand((unsigned int)time(NULL));
    LOG.open( "log.txt" );

    Terminal terminal;

    Entity player( "&", terminal.GetColor( "jaune", "jaune" ) );
    Entity stairsDown( "*", terminal.GetColor( "rouge", "blanc" ) );

    string lastEvent = "";

    Map map;
    map.Setup(
        terminal.GetColor( "noir", "noir" ), /* plafonds */  
        terminal.GetColor( "cyan", "cyan" ), /* murs */
        terminal.GetColor( "bleu", "bleu" ),  /* sols */
        terminal.GetColor( "blanc", "blanc" ) /* roches */
    	);
    map.GenerateMap( terminal );

    Room startRoom = map.GetFirstRoom();
    Room endRoom = map.GetLastRoom();

    player.SetPosition( startRoom.x, startRoom.y );
    stairsDown.SetPosition( endRoom.x, endRoom.y );

    while ( !fin )
    {
        
        terminal.StartDraw();

        map.Draw( terminal );
        stairsDown.Draw( terminal );

        player.Draw( terminal );

        if ( lastEvent != "" )
            terminal.Draw( 0, 0, lastEvent, terminal.GetColor( "noir", "blanc" ) );

        player.DrawInventory( terminal );
        terminal.EndDraw();

        int input = terminal.GetKey();
        Direction moveDirection = UNSET;

        if      ( input == KEY_LEFT || input == 'Q' || input == 'q')
        	moveDirection = LEFT;
        else if ( input == KEY_RIGHT || input == 'D' || input == 'd')
        	moveDirection = RIGHT;
        else if ( input == KEY_UP || input == 'Z' || input == 'z')
        	moveDirection = UP;
        else if ( input == KEY_DOWN || input == 'S' || input == 's')
        	moveDirection = DOWN;

        TileType case_voisine = map.GetTileType( player.GetX(), player.GetY(), moveDirection );

        if ( moveDirection != UNSET && ( case_voisine == SOL ) )
            player.Move( moveDirection );
        
        else if ( moveDirection != UNSET && ( case_voisine == ROCHE ) )
        {
            Element randomElement = GetRandomElement();
            player.Mine( moveDirection, randomElement );
            map.Mine( player.GetX(), player.GetY(), moveDirection );
            lastEvent = "Trouvé  : " + ElementToString( randomElement ) + "!";
        }

        if ( input == KEY_F(1) )
        {
            map.GenerateMap( terminal );

            startRoom = map.GetFirstRoom();
            endRoom = map.GetLastRoom();
            player.SetPosition( startRoom.x, startRoom.y );
            stairsDown.SetPosition( endRoom.x, endRoom.y );
        }
        else if ( input == 27 )		//esc on keyboard
            fin = true;
    }

    LOG.close();

    return 0;
}
