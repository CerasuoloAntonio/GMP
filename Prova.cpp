#include <iostream>
#include <cstdlib> // per il random
#include <iomanip>
#include <map>
#include <conio.h>
#ifdef _WIN32
#include <windows.h>
#endif

/*              ~appunti~

    cout << "\033[A"; 

    ESC[A	cursore SU di 1 riga
    ESC[B	cursore GIÙ di 1 riga
    ESC[C	cursore DESTRA
    ESC[D	cursore SINISTRA

    \033[ [coordinata x] ; [coordinata y] H; posizione specifica

*/

using namespace std;

enum alignment {
    align_center,
    align_left,
    align_right,
    position_end,
    position_start 
};

enum directions{
    up,
    down,
    left,
    right
};

enum gameState{
    menu,
    world,
    combat
};

void intro();
void drawLine();
void write(string phrase, int line, enum alignment align);
void writeTitle(string phrase);
void drawLetter(int n);
void printAlphabet();
void setFixedPointer(enum alignment position);
void run();
void menuInputManager();
void combatInputManager();
void worldInputManager();
void clearLine(int line);

int calculateAlignmentX(enum alignment position, int textLength);
int calculateAlignmentY(enum alignment position, int textLength);

class button;

class coordinates{
    private :

        int x;
        int y;

    public :

        coordinates(){
            x = 1;
            y = 1;
        }
        coordinates(int x_coordinate, int y_coordinate){
            x = x_coordinate;
            y = y_coordinate;
        }

        coordinates(enum alignment position, int y_coordinate, int textLenght){
            y = y_coordinate; 
            x = calculateAlignmentX(position, textLenght);
        }

        void set_x(int i){x=i;}
        void set_y(int i){y=i;}
        int get_x(){return x;}
        int get_y(){return y;}
};

class interactable{
    protected :

        bool isSelected;
        char selectedIndicator;
        string content;
        string Basecontent;
        bool isShown;
        coordinates pos;

    public :

        interactable(string text = "Nuovo interagibile", bool selected = false, bool shown = false, char indicator = '<') : content(text), Basecontent(text),isSelected(selected), isShown(shown), selectedIndicator(indicator){
            
        }

        void display(coordinates position){
            cout << "\033[" << position.get_y() << ";" << position.get_x() << "H" << content << endl;
        }

        bool getShown(){return isShown;}
        bool getSelected(){return isSelected;}

};

class button : public interactable {
    public :

        button(string text, coordinates position, bool shown = false, char indicator = '<' , bool selected = false) : interactable(text,selected,shown, indicator){
            content = text;
            pos = position;
            toggleSelectedIcon();
            if(isShown){
                display(pos);
            }
        }

        button(string text = "nuovo bottone",enum alignment alignment = align_left, int yPosition = 1 , bool shown = false, char indicator = '<' , bool selected = false) : interactable(text,selected,shown, indicator){
            pos = coordinates (alignment,yPosition, text.length());
            toggleSelectedIcon();
            if(isShown){
                display(pos);
            }
        }

        void toggleSelected(){
            isSelected = !isSelected;
            toggleSelectedIcon();
            clearLine(pos.get_y());
            display(this->pos);
        }

        void toggleSelectedIcon(){
            if(isSelected){
                content = Basecontent + " " + selectedIndicator;
            }
            else{
                content = Basecontent;
            }
        }

}; 

class entity {
    

    private :
        const int minHealth = 0;

        int attack;
        int maxHealth;

    public :
        int health;
        int defense;

    entity(){
        maxHealth = 100;
        health = maxHealth;
        attack = 10;
    }

    void attackEnemy(entity &enemy){
        enemy.health -= attack;
    }
};

class player : public entity{


};

class monster : public entity{

};

class gameController{
    private:
        int button_count;
        button* buttons[10];
        gameState state;
        int selectedButton;
    public:        
    
        gameController(){
            button_count = 0;
            state = menu;
        }

        void addButton(button newButton){
            buttons[button_count] = new button(newButton);
            if(button_count == 0){
                buttons[button_count]->toggleSelected();
                selectedButton = button_count;
            }
            button_count++;
            
        }

        void changeSelectedButton(enum directions direction){
            switch (direction)
            {
            case directions::up:
                buttons[selectedButton]->toggleSelected();
                if(selectedButton == 0){
                    buttons[button_count - 1]->toggleSelected();
                    selectedButton = button_count - 1;
                }
                else{
                    buttons[selectedButton-1]->toggleSelected();
                    selectedButton -= 1;
                }
                break;
            case directions::down:
                buttons[selectedButton]->toggleSelected();
                if(selectedButton == button_count - 1){
                    buttons[0]->toggleSelected();
                    selectedButton = 0;
                }
                else{
                    buttons[selectedButton + 1]->toggleSelected();
                    selectedButton += 1;
                }
                break;
            case directions::left:
                break;
            case directions::right:
                break;
            default:
                break;
            }
        }
        
        void checkState(){
            switch(state){
                case menu :
                    menuInputManager();
                    break;
                case combat :
                    combatInputManager();
                    break;
                case world :
                    worldInputManager();
                    break;
            }
        }
    
};

gameController gameManager;

const int screenHeight = 27;
const int screenLenght = 119;
const int letterLines = 5;

string alphabet[26][letterLines] = {
    /* a */ {" █████ ", "██   ██", "███████", "██   ██", "██   ██"},
    /* b */ {"██████ ", "██   ██", "██████ ", "██   ██", "██████ "},
    /* c */ {" ██████", "██     ", "██     ", "██     ", " ██████"},
    /* d */ {"██████ ", "██   ██", "██   ██", "██   ██", "██████ "},
    /* e */ {"███████", "██     ", "█████  ", "██     ", "███████"},
    /* f */ {"███████", "██     ", "█████  ", "██     ", "██     "},
    /* g */ {" ██████", "██     ", "██  ███", "██   ██", " ██████"},
    /* h */ {"██   ██", "██   ██", "███████", "██   ██", "██   ██"},
    /* i */ {"███████", "   ██  ", "   ██  ", "   ██  ", "███████"},
    /* j */ {"███████", "     ██", "     ██", "██   ██", " █████ "},
    /* k */ {"██   ██", "██  ██ ", "█████  ", "██  ██ ", "██   ██"},
    /* l */ {"██     ", "██     ", "██     ", "██     ", "███████"},
    /* m */ {"███   ███", "████ ████", "██ ███ ██", "██     ██", "██     ██"},
    /* n */ {"███   ██", "████  ██", "██ ██ ██", "██  ████", "██   ███"},
    /* o */ {" █████ ", "██   ██", "██   ██", "██   ██", " █████ "},
    /* p */ {"██████ ", "██   ██", "██████ ", "██     ", "██     "},
    /* q */ {" █████ ", "██   ██", "██   ██", "██  ███", " ██████"},
    /* r */ {"██████ ", "██   ██", "██████ ", "██  ██ ", "██   ██"},
    /* s */ {" ██████", "██     ", " █████ ", "     ██", "██████ "},
    /* t */ {"███████", "   ██  ", "   ██  ", "   ██  ", "   ██  "},
    /* u */ {"██   ██", "██   ██", "██   ██", "██   ██", " █████ "},
    /* v */ {"██   ██", "██   ██", "██   ██", " ██ ██ ", "  ███  "},
    /* w */ {"██     ██", "██     ██", "██  █  ██", "██ ███ ██", " ███ ███ "},
    /* x */ {"██   ██", " ██ ██ ", "  ███  ", " ██ ██ ", "██   ██"},
    /* y */ {"██   ██", " ██ ██ ", "  ███  ", "   ██  ", "   ██  "},
    /* z */ {"███████", "   ███ ", "  ███  ", " ███   ", "███████"}
};

/* Main */

int main(){

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    
    run();

    system("Pause");

    return 0;
}

void run(){
    intro();
}


void intro(){
    drawLine();
    for(int i = 0; i < screenHeight; i++){
        cout << "|" << setfill(' ') << setw(screenLenght) << "|" << endl;
    }
    drawLine();

    button primobottone("New Game", align_center, 20, true), 
           secondobottone("Continue", align_center , 21, true),
           terzobottone("Options ",  align_center , 22, true),
           quartobottone("Exit",  align_center , 23, true);
           gameManager.addButton(primobottone);
           gameManager.addButton(secondobottone);
           gameManager.addButton(terzobottone);
           gameManager.addButton(quartobottone);

    writeTitle("the game");
    setFixedPointer(position_end);
    gameManager.checkState();
}

void drawLine(){
    cout << "|" << setfill('~') << setw(screenLenght) << "|" << endl;
}


void write(string phrase, int line,enum alignment align){

    int stringSize = phrase.length();
    int screenCenter = (screenLenght/2)+(stringSize/2);

    for(int i = screenHeight; i > line; i--){
        cout << "\033[A";
    }

    switch (align) {
        case align_center:
            cout << "\033[C" << setfill(' ') << setw(screenCenter) << phrase ;
            break;
        case align_left:
            cout << "\033[C" << phrase;
            break;
        case align_right:
            cout << "\033[C" << setfill(' ') << setw(screenLenght - 1) << phrase;
            break;
    }

    for(int i = line; i < screenHeight; i++){
        cout << "\033[B\r";
    }

}

void writeTitle(string phrase){
    int offset = 30;
    int phraseLenght = phrase.length();
    string segment = "";
    cout << "\033[5;1H";
    for(int n = 0; n < 5; n++){
        segment = "";
        for(int i = 0; i < phraseLenght; i++){
            switch (phrase[i]){
                case 'a': segment = segment + alphabet[0][n] + " "; break;
                case 'b': segment = segment + alphabet[1][n] + " "; break;
                case 'c': segment = segment + alphabet[2][n] + " "; break;
                case 'd': segment = segment + alphabet[3][n] + " "; break;
                case 'e': segment = segment + alphabet[4][n] + " "; break;
                case 'f': segment = segment + alphabet[5][n] + " "; break;
                case 'g': segment = segment + alphabet[6][n] + " "; break;
                case 'h': segment = segment + alphabet[7][n] + " "; break;
                case 'i': segment = segment + alphabet[8][n] + " "; break;
                case 'j': segment = segment + alphabet[9][n] + " "; break;
                case 'k': segment = segment + alphabet[10][n] + " "; break;
                case 'l': segment = segment + alphabet[11][n] + " "; break;
                case 'm': segment = segment + alphabet[12][n] + " "; break;
                case 'n': segment = segment + alphabet[13][n] + " "; break;
                case 'o': segment = segment + alphabet[14][n] + " "; break;
                case 'p': segment = segment + alphabet[15][n] + " "; break;
                case 'q': segment = segment + alphabet[16][n] + " "; break;
                case 'r': segment = segment + alphabet[17][n] + " "; break;
                case 's': segment = segment + alphabet[18][n] + " "; break;
                case 't': segment = segment + alphabet[19][n] + " "; break;
                case 'u': segment = segment + alphabet[20][n] + " "; break;
                case 'v': segment = segment + alphabet[21][n] + " "; break;
                case 'w': segment = segment + alphabet[22][n] + " "; break;
                case 'x': segment = segment + alphabet[23][n] + " "; break;
                case 'y': segment = segment + alphabet[24][n] + " "; break;
                case 'z': segment = segment + alphabet[25][n] + " "; break;
                case ' ': segment += "   ";break;
            }
        }
        cout << "\033[" << offset << "C" << segment << endl;
    }
    
}

void drawLetter(int n){
    for(int i = 0; i < letterLines; i++){
        cout << alphabet[n][i] << "\033[B\033[" << alphabet[n][i].length() <<"D";
    }
    cout << "\033[" << 5 << "C" << "\033[" << letterLines <<"A";
}

void printAlphabet(){
    for(int i = 0; i < 26; i++){
        for(int n = 0; n < letterLines; n++){
            cout << alphabet[i][n] << endl;
        }
        cout << endl;
    }
}



void setFixedPointer(enum alignment position){
    if(position == alignment::position_end){
        cout << "\033[30;1H";
    }
    if(position == alignment::position_start){
        cout << "\033[2;2H";
    }
}

int calculateAlignmentX(enum alignment position, int textLength){
    switch (position) {
        case align_center:
            return (screenLenght - textLength) / 2;
            break;
        case align_left:
            return 1;
            break;
        case align_right:
            return screenLenght - 1 - textLength;
            break;
        default :
            return 1;
    }
}

int calculateAlignmentY(enum alignment position, int textLength){
    switch (position) {
        case align_center:
            return screenHeight / 2;
            break;
        case align_left:
            return 1;
            break;
        case align_right:
            return screenHeight - 1;
            break;
        default :
            return 1;
    }
    
}

void menuInputManager(){
    char input = _getch();
    switch(input){
        case 'w' :
            gameManager.changeSelectedButton(up);
            break;
        case 'a' :
            gameManager.changeSelectedButton(up);
            break;
        case 's' :
            gameManager.changeSelectedButton(down);
            break;
        case 'd' :
            cout << input << endl;
            break;    
        default:
            return;
    }
    setFixedPointer(position_end);
    menuInputManager();
}

void combatInputManager(){
    char input = _getch();
    switch(input){
        case 'w' :
            break;
        case 'a' :
            break;
        case 's' :
            break;
        case 'd' :
            break;    
        default:
            return;
    }
    setFixedPointer(position_end);
    combatInputManager();
}

void worldInputManager(){
    char input = _getch();
    switch(input){
        case 'w' :
            break;
        case 'a' :
            break;
        case 's' :
            break;
        case 'd' :
            break;    
        default:
            return;
    }
    setFixedPointer(position_end);
    worldInputManager();
}

void clearLine(int line){
    cout << "\033[" <<line << ";1H" << "|" << setfill(' ') << setw(screenLenght) << "|";
    setFixedPointer(position_end);
}