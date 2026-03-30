#include <iostream>
#include <conio.h> // For _kbhit() and _getch()
#include <cstdlib> // For system("cls")
#include <ctime>   // For tracking time
#include <thread>  // For sleep_for()
#include <chrono>  // For duration

using namespace std;
using namespace chrono;

//Define the size of the game grid
#define WIDTH 30        //columns
#define HEIGHT 20       //rows

// ANSI color codes for terminal
#define RESET "\033[0m"
#define BLUE_BG "\033[44m"
#define RED_ON_BLUE "\033[31;44m"
#define GREEN_ON_BLUE "\033[32;44m"
#define WHITE_ON_BLUE "\033[37;44m"

// Game grid
char grid[HEIGHT][WIDTH + 1] = {
    "##############################",
    "#..............#............#",
    "#.######.......#..######....#",
    "#.#....#..............#.....#",
    "#.#.####.######..#####.#.....",
    "#.#......#....#..#.....#.....",
    "#.#.####.#.###.#.#.#####.....",
    "#.#......#...#.#.............",
    "#.######.###.#.######.######.",
    "#......X....#...............#",
    "#.###.#.###########.######..#",
    "#.#...#.............#....#..#",
    "#.#.###.##############.#.##.#",
    "#...#....#............#.....#",
    "#.###.####.########.#.#####.#",
    "#.#.............O....#......#",
    "#.#.######.######.#..#.####.#",
    "#..................#........#",
    "#.###.###########.###.######.",
    "##############################"
};

bool is_error_happened = false;
bool triggered_first_time = true;

int playerX = 0, playerY = 0;       // Player position
int ghostX = 0, ghostY = 0;         // Ghost position

void find_player() {
    if (is_error_happened == true) {
        return; // do not waste any time previously there is an error just return :)
    }
    // Ok so far there is no error goood!!!
    for (int i = 0; i < HEIGHT; i++) { // horizontal location
        for (int j = 0; j < WIDTH; j++) { //vertical location
            if (grid[i][j] == 'O') {
                playerX = i;
                playerY = j;
                return;
            }
        }
    }
    is_error_happened = true; // Oops...there is an error.

}
void find_ghost() {
    if (is_error_happened == true) {
        return; // do not waste any time previously there is an error just return
    }

    // Ok so far there is no error goood!!!
    for (int i = 0; i < HEIGHT; i++) { // horizontal location
        for (int j = 0; j < WIDTH; j++) { //vertical location
            if (grid[i][j] == 'X') {
                ghostX = i;
                ghostY = j;
                return;
            }
        }
    }
    is_error_happened = true; // Oops...there is an error.

}

void go_to_secure() { // this location was tested before so use default location.
    playerX = 15;
    playerY = 14;
    ghostX = 17;
    ghostY = 12;

}

// Function to reset the game state and receive the location of player and ghost
void resetGame() {
    if (is_error_happened == false) { //first control
        find_player();
        find_ghost();
    }
    // CHECK AGAIN for RESULT OF FIND_PLAYER AND GHOST FUNCTIONS
    if (is_error_happened == true) {
        if (triggered_first_time == true) {
            cout << RED_ON_BLUE << "ERROR! PLAYER OR GHOST NOT IN THE MAP, New Game is loading...";
            this_thread::sleep_for(milliseconds(3000));
            go_to_secure();
            triggered_first_time = false; // if this step triggered again it will not be first time anymore so I updated the flag :)
        }
        else {
            // already error was triggered
            go_to_secure();
        }
    }

}

// Function to display the grid with timer
void displayGrid(int elapsedTime) {
    system("cls"); // Clear the console on Windows

    // Display timer at the top center
    cout << "\033[1;33m"; // Set text color to yellow
    cout << "Time Elapsed: " << elapsedTime << "s" << endl;
    cout << RESET; // Reset text formatting

    // Draw the grid
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == playerX && j == playerY) {
                cout << GREEN_ON_BLUE << "O"; // Player in green on blue
            }
            else if (i == ghostX && j == ghostY) {
                cout << RED_ON_BLUE << "X";   // Ghost in red on blue
            }
            else if (grid[i][j] == '#') {
                cout << WHITE_ON_BLUE << "#"; // Walls in white on blue
            }
            else {
                cout << BLUE_BG << " ";      // Empty space with blue background
            }
        }
        cout << RESET << endl; // Reset after each row

        //This function clears the screen and redraws the game grid, including the positions of the player.
        //It doesn�t calculate or return anything to the main program.So there�s no need to send any value back.
    }
}

// Function to move the player
//Updates the player's position

void movePlayer(char direction) {
    int newX = playerX, newY = playerY;


    switch (direction) {
    case 'w':  //break through
    case 'W': {
        newX--; //
        break; // Up
    }
    case 's':
    case 'S': {
        newX++;
        break;
    } // Down
    case 'a':
    case 'A': {
        newY--;
        break; // Left
    }
    case 'd':
    case 'D': {
        newY++;
        break; // Right
    }
    default:
        return;   // Invalid input
    }

    // Check if new position is not a wall
    if (grid[newX][newY] != '#') {
        playerX = newX;
        playerY = newY;

        //Returning a value (like the new position) isn�t necessary. 
        //because the main program doesn�t need the result�it only cares that the player moves.
    }// else return to original do not save newx and newy
}

// Function to move the ghost towards the player
void moveGhost() {
    int dx = playerX - ghostX; // Difference in X positions
    int dy = playerY - ghostY; // Difference in Y positions

    int newGhostX = ghostX, newGhostY = ghostY;

    //It determines how the ghost (X) moves toward the player (O) while avoiding walls (#). The ghost prioritizes moving in the direction of 
    // the largest distance between its position and the player's position

    // Prioritize the larger difference to move toward the player

    if (abs(dx) > abs(dy)) {   //comparison between dx = horizontal, dy = vertical
        // Move down or up
        if (dx > 0) {
            newGhostX += 1;

        }
        else {
            newGhostX -= 1;
        }

        if (grid[newGhostX][ghostY] == '#') { // Check for walls
            newGhostX = ghostX; // Reset if blocked

            // Try moving sideways      
            if (dy > 0) {
                newGhostY += 1;
            }
            else {
                newGhostY -= 1;
            }
        }
    }
    else {

        //If the vertical distance (dy) is greater than or equal to the horizontal distance (dx), the ghost prioritizes moving vertically
        // Move right or left
        if (dy > 0) {
            newGhostY += 1;
        }
        else {
            newGhostY -= 1;
        }

        if (grid[ghostX][newGhostY] == '#') { //Checks if the ghost's is blocked by a wall
            newGhostY = ghostY; // Reset if blocked
            // Try moving vertically
            if (dx > 0) {

                newGhostX += 1;
            }
            else {
                newGhostX -= 1;
            }
        }
    }
    //Ensures the new position is not blocked by a wall (#).
    if (grid[newGhostX][newGhostY] != '#') {
        ghostX = newGhostX;
        ghostY = newGhostY;
    }

}

// Main function
int main() {

    while (true) {
        resetGame();
        srand(time(0)); // Seed for random movement of ghost
        char input;     //Declares a variable to store the player�s keyboard input
        int ghostMoveCounter = 0;   // Counter to control ghost's movement speed
        const int ghostMoveSpeed = 2; // Ghost moves every 2 iterations

        // Timer variables
        auto startTime = steady_clock::now();

        while (true) {
            // Calculate elapsed time
            auto currentTime = steady_clock::now();

            //Calculates how many seconds have passed since startTime
            int elapsedTime = duration_cast<seconds>(currentTime - startTime).count();

            displayGrid(elapsedTime);

            // Check if player and ghost collide
            if (playerX == ghostX && playerY == ghostY) {
                cout << BLUE_BG << "Nah, You're BAD! Game Over :(" << RESET << endl;
                cout << "Woow... You survived for " << elapsedTime << " seconds." << endl;
                break;
            }


            // Handle player input (non-blocking)
            if (_kbhit()) {
                input = _getch(); // Get the key pressed

                // Exit condition
                if ((input == 'r') || (input == 'R')) {
                    cout << "Press one more time to [R]estart." << endl;
                    break;
                }
                else if ((input == 'q') || (input == 'Q')) {
                    cout << "You pressed [Q]uit button...." << endl;
                    break;
                }
                // Move player based on input
                movePlayer(input);
            }

            // Control ghost's MOVEMENT SPEED
            //This block of code slows down the ghost's movement to make the game fair.
            ghostMoveCounter++;
            if (ghostMoveCounter >= ghostMoveSpeed) {
                moveGhost();
                ghostMoveCounter = 0; // Reset counter after ghost moves
            }

            //Adds a small delay (200 milliseconds) to each iteration of the loop, making the game smoother and easier to play.

            this_thread::sleep_for(milliseconds(200));
        }

        // Handle end-game options
        while (true) {
            cout << "Press Q to quit or R to restart: \r\n";
            char option = _getch();
            if (option == 'q' || option == 'Q') {
                cout << "BYE BYE!" << endl;
                return 0; // Quit the game
            }
            else if (option == 'r' || option == 'R') {
                break; // Restart the game
            }
        }
    }
}