// ---------------------------------------------------------------------------
// A simple C++98 TicTacToe game for Windows
//
// Author: Peter Sandor
// ---------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <windows.h>

#define MSG_VICTORY_PLAYER      cout << "Congratulations, you won!"
#define MSG_VICTORY_COMPUTER    cout << "Sorry, random AI won."
#define MSG_DRAW                cout << "A draw!"

using namespace std;

enum Player
{
    COMPUTER,
    PLAYER,
    GOD,
    EMPTY
};

enum Result
{
    WIN,
    LOSE,
    DRAW,
};

const int WIN_COMBINATIONS[8][3] = { { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 }, { 0, 3, 6 }, { 1, 4, 7 }, { 2, 5, 8 }, { 0, 4, 8 }, { 2, 4, 6 } };

class TicTacToe
{
public:
    // Destructor
    ~TicTacToe() {}

    // Constructor with variable initialization (default: computer goes first)
    TicTacToe() : nextMove ( COMPUTER ) {}

    void start()
    {
        resetGame();

        cout << "Welcome to Tic Tac Toe" << endl << endl;

        // Who starts first
        if ( isPlayerFirst() )
        {
            nextMove = PLAYER;
        }
        else
        {
            nextMove = COMPUTER;
        }

        // Main loop
        while ( !isGameOver )
        {
            drawGrid();

            if( nextMove == PLAYER )
            {
                playerMove();
            }
            else
            {
                computerMove();
            }

            // Redraw after making moves
            drawGrid();

            // Check if anyone won
            result = checkResult();

            if( result != EMPTY )
            {
                break;
            }

            ++nextMove %= 2;
        }

        switch( result )
        {
            case PLAYER:
                MSG_VICTORY_PLAYER;
            break;

            case COMPUTER:
                MSG_VICTORY_COMPUTER;
            break;

            default:
                MSG_DRAW;
        }

        string r;

        cout << "\n\nDo you want to play again? ( Y / N ) ";

        if( cin >> r != "Y" && r != "y" )
        {
            return;
        }
        else
        {
            start();
        }
    }

private:
    void resetGame()
    {
        result = DRAW;
        isGameOver = false;

        system("cls");

        for ( int i = 0; i < 9; i++ )
        {
            field[i] = EMPTY;
        }
    }

    bool isPlayerFirst()
    {
        string startsFirst;
        bool repeat;

        do {
            cout << "Do you want to start first? ( Y / N ): ";

            // Get the user input and turn it to uppercase
            if ( cin >> startsFirst && startsFirst[0] > 'Z' )
            {
                startsFirst[0] -= 32;
            }

            // Check for answer, repeat if not Y/N
            switch ( startsFirst[0] )
            {
                case 'Y':
                    return true;
                break;

                case 'N':
                    return false;
                break;

                default:
                    repeat = true;
            }
        } while( repeat );

        return false;   // It shouldn't get here
    }

    void drawGrid()
    {
        COORD cursorPosition = { 0, 0 };
        int rows = 5;
        int cols = 11;
        int r = 0;
        int c = 0;
        int nums[3][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };

        // Clear the console buffer
        system("cls");

        for( int i = 0; i < rows; i++ )
        {
            if(i%2)
            {
                cout << "---+---+---" << endl;
            }
            else
            {
                cout << " " << nums[r][0] << " | " << nums[r][1] << " | " << nums[r][2] << " " << endl;
                r++;
            }
        }

        // Update the corresponding cells
        for( int y = 0; y < rows; y += 2 )
        {
            for( int x = 1; x < cols; x += 4 )
            {
                if( field[c] != EMPTY )
                {
                    COORD markPosition = { x, y };
                    string symbol = field[c] == COMPUTER ? "X" : "O";

                    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), markPosition );
                    cout << symbol;
                }

                c++;
            }
        }

        // Set the cursor position below the grid for other messages
        cursorPosition.Y = 6;
        SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), cursorPosition );
    }

    int checkResult()
    {
        int i = 0;

        for( ; i < 8; i++ )
        {
            if( field[WIN_COMBINATIONS[i][0]] != EMPTY &&
                field[WIN_COMBINATIONS[i][0]] == field[WIN_COMBINATIONS[i][1]] &&
                field[WIN_COMBINATIONS[i][1]] == field[WIN_COMBINATIONS[i][2]] )
            {
                return field[WIN_COMBINATIONS[i][0]];
            }
        }

        i = 0;

        for( int f = 0; f < 9; f++ )
        {
            if( field[f] != EMPTY )
            i++;
        }
        if( i == 9 )
        {
            return DRAW;
        }

        return EMPTY;
    }

    void playerMove()
    {
        int n = 0;

        cout << "Enter your move ( 1 - 9 ) ";

        while( !((cin >> n) && (n >= 1) && (n <= 9)) || field[n - 1] != EMPTY )
        {
            cout << "Incorrect move, try again please." << endl;
            cin.clear();
            cin.ignore();
        }

        field[n - 1] = PLAYER;
    }

    void computerMove()
    {
        int next = 0;

        do
        {
            next = rand() % 9;
        }
        while( field[next] != EMPTY );

        for( int i = 0; i < 8; i++ )
        {
            int check1 = WIN_COMBINATIONS[i][0];
            int check2 = WIN_COMBINATIONS[i][1];
            int check3 = WIN_COMBINATIONS[i][2];

            if( field[check1] != EMPTY &&
                field[check1] == field[check2] &&
                field[check3] == EMPTY )
            {
                next = check3;

                if( field[check1] == COMPUTER )
                {
                    break;
                }
            }

            if( field[check1] != EMPTY &&
                field[check1] == field[check3] &&
                field[check2] == EMPTY )
            {
                next = check2;

                if( field[check1] == COMPUTER )
                {
                    break;
                }
            }

            if( field[check2] != EMPTY &&
                field[check2] == field[check3] &&
                field[check1] == EMPTY )
            {
                next = check1;

                if( field[check2] == COMPUTER )
                {
                    break;
                }
            }
        }

        field[next] = COMPUTER;
    }

    int nextMove, field[9], result;
    bool isGameOver;
};

int main()
{
    // Create an instance of the game
    TicTacToe game;

    // Start the game
    game.start();

    return system("pause");
}