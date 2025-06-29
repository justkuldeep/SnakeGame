//Snake Game is a 3 component Game! "Screen , Snake , Food". 
#include<bits/stdc++.h>
#include<conio.h> // Keyboard hit detection ke liye, taaki user input le sake
#include<windows.h> // Console ki coordinates manipulate karne ke liye
using namespace std;

#define MAX_LENGTH 1000 // Snake ka maximum length

// Directions
const char DIR_UP = 'U'; // Char is constant, to let it become only readable.
const char DIR_DOWN = 'D';
const char DIR_RIGHT = 'R';
const char DIR_LEFT = 'L';

int consoleWidth , consoleHeight;

// Initaialising Console screen - Screen ki dimensions aur food ki randomness setup karta hai
void initScreen(){
    HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE); //  handles Console  
    // GetStdHandle(STD_OUTPUT_HANDLE) -> console pr koi point draw krne ke liye use hota hai.
    CONSOLE_SCREEN_BUFFER_INFO csbi; // CSBI is a class and csbi is a object(of CSBI)
    GetConsoleScreenBufferInfo(hconsole, &csbi); // getting the buffer info of console
    // Console ki height aur width calculate karenge
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

// Component -> 1 : Food
struct Point{
    int xCoOrdinate;
    int yCoOrdinate;
    Point(){} // Default constructor
    Point(int x, int y){
        xCoOrdinate = x; // X aur Y coordinate ko initialize kar rahe hain
        yCoOrdinate = y;
    }
};

// Component -> 2 : Snake
class Snake{
    int length; // Snake ki length
    char direction; // Snake ka direction

public:
    Point body[MAX_LENGTH]; // Snake ka body array, jisme coordinates store honge
    Snake(int x, int y){
        length = 1; // Initial snake ki length 1 hogi
        body[0] = Point(x,y); // Snake center m appear hoga
        direction = DIR_RIGHT; // Snake ki Initial direction right hogi
    }

    int getLength(){
        return length; // To store current lenght and later fetch to print snake , everytime the console clear
    }

    // Direction change karne ka function
    void changeDirection(char newDirection){
        // Agar new direction valid hai to change direction
        if(newDirection == DIR_UP && direction != DIR_DOWN){
            direction = newDirection;
        }
        else if (newDirection == DIR_DOWN && direction != DIR_UP){
            direction = newDirection;
        }
        else if (newDirection == DIR_LEFT && direction != DIR_RIGHT){
            direction = newDirection;
        }
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT){
            direction = newDirection;
        }
    }

    // Snake ko move karne 
    bool move(Point food){
        for(int i = length - 1; i > 0; i--){
            body[i] = body[i - 1]; // Body ke sare elements ko shift kar rahe h
        }

        // Direction ke according head ko update kar rahe 
        switch(direction){
            case DIR_UP:
                body[0].yCoOrdinate--;
                break;
            case DIR_DOWN:
                body[0].yCoOrdinate++;
                break;
            case DIR_RIGHT:
                body[0].xCoOrdinate++;
                break;
            case DIR_LEFT:
                body[0].xCoOrdinate--;
                break;
        }

        // Agar snake border ke andar nahi aata to game over ho jayega
        if (body[0].xCoOrdinate < 0 || body[0].xCoOrdinate >= consoleWidth || body[0].yCoOrdinate < 0 || body[0].yCoOrdinate >= consoleHeight) {
            return false;
        }

        // Agar snake apne aapko hi kaat le to game over
        for (int i = 1; i < length; i++) {
            if (body[0].xCoOrdinate == body[i].xCoOrdinate && body[0].yCoOrdinate == body[i].yCoOrdinate) {
                return false;
            }
        }

        // Agar food kha liya to snake ka size badh jayega
        if (food.xCoOrdinate == body[0].xCoOrdinate && food.yCoOrdinate == body[0].yCoOrdinate) {
            body[length] = body[length - 1]; // Snake ki body me ek naya part add ho jayega
            length++; // Length increase karenge
            return true;
        } 
        return true;
    }
};

// Component -> 3
class Board{
    Snake *snake;
    const char SNAKE_BODY = 'O'; // Snake esa dikhega
    Point food;
    const char FOOD = 'o'; // Food ka symbol
    int score; 

public:
    Board(){
        spawnFood(); // randomly Food spawn karenge 
        snake = new Snake(consoleWidth / 2, consoleHeight / 2); // Snake ko center me initialize karenge
        score = 0; // Initial score 0
    }
    ~Board(){
        delete snake; // Destructor me snake ko delete karenge
    }

    int getScore(){
        return score; // Current score return karenge
    }

    // Food ko random position par spawn karenge
    void spawnFood(){
        int x = rand() % (consoleWidth - 2) + 1; // Random x position between console screen
        int y = rand() % (consoleHeight - 2) + 1; // Random y position between console screen
        food = Point(x, y); // Food ko set karenge
    }

    void displayCurrentScore(){
        goto_xy(consoleWidth / 2, 0); // Score ko console ke top pr center m display karenge
        cout << "Score: " << score; // Score print karenge
    }

    // Console par cursor ko move karne ke liye coOrdinates 
    void goto_xy(int x, int y){
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Cursor ko set karenge specified coordinates par
    }

    // Game ko draw karne ka function
    void Draw(){
        system("cls"); // clearing console .
        for (int i = 0; i < snake->getLength(); i++) {
            goto_xy(snake->body[i].xCoOrdinate, snake->body[i].yCoOrdinate); // displaying snake
            cout << SNAKE_BODY;
        }
        goto_xy(food.xCoOrdinate, food.yCoOrdinate); // Food ko display karenge
        cout << FOOD;
        displayCurrentScore(); // Score ko display karenge
    }

    // Game update karne ka function
    bool update(){
        bool isAlive = snake->move(food); // Snake ko move karenge
        if (!isAlive) {
            return false; // Agar snake dead ho gaya to game over
        }

        // Agar food khaya to score increment hoga aur new food spawn hoga
        if (food.xCoOrdinate == snake->body[0].xCoOrdinate && food.yCoOrdinate == snake->body[0].yCoOrdinate) {
            score++;
            spawnFood();
        }

        return true;
    }

    // User se input lene ka function
    void getInput(){
        if (_kbhit()) { // Agar keyboard hit hua hai to
            int key = _getch(); // Key press ko capture karenge
            // Direction change karenge
            if (key == 'w' || key == 'W') {
                snake->changeDirection(DIR_UP);
            }
            else if (key == 'a' || key == 'A') {
                snake->changeDirection(DIR_LEFT);
            }
            else if (key == 's' || key == 'S') {
                snake->changeDirection(DIR_DOWN);
            }
            else if (key == 'd' || key == 'D') {
                snake->changeDirection(DIR_RIGHT);
            }
        }
    }
};

int main(){
    initScreen(); // Screen ko initialize karenge
    Board* board = new Board(); // Creating our board

    while (board->update()) {
        board->getInput(); // Input lete rahenge
        board->Draw(); // Screen ko redraw karenge , every second !!!!
        Sleep(80); // Game speed
    }

    cout << "Game Over!" << endl; 
    cout << "Final Score: " << board->getScore() << endl; 
    delete board; 

    return 0;
}
