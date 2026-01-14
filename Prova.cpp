#include <iostream>
#include <cstdlib> // per il random
#include <iomanip>
#include <fstream>
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
    position_start,
    position_begin 
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

enum buttonTypes{
    newGameButton,
    continueButton,
    exitButton,
    generic,
    optionButton
};

const int screenHeight = 27;
const int screenLenght = 119;
const int letterLines = 5;




void drawLine();
void write(string phrase, int line, enum alignment align);
void writeTitle(string phrase);
void drawLetter(int n);
void printAlphabet();
void setFixedPointer(enum alignment position);
void menuInputManager();
void combatInputManager();
void worldInputManager();
void clearLine(int line);
int calculateAlignmentX(enum alignment position, int textLength);
int calculateAlignmentY(enum alignment position, int textLength);

class object{
    public :
        virtual ~object(){}
};

struct node{
    object* ogetto;
    node* next;
};

struct node4d{
    object* oggetto;
    coordinates posizione;
    node4d* down;
    node4d* up;
    node4d* left;
    node4d* right;
};

struct pointerlist4d{
    node4d* pointer;
    pointerlist4d* next;
};

class coordinates : public object{
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

class scene : public object{
    private : 
        node* sceneList;
        const static int mapHeight = 29;
        string sceneName;
        string map[mapHeight];
        string temp;
        
    public :
        

        scene(string name = "scena") : sceneName(name){
            initializeObjectList();
        }

        void draw(){
            for(int n = 0; n < mapHeight; n++){
                cout << map[n] << endl;
            }
        }

        void deleteScene(){
            setFixedPointer(position_begin);
            for(int i = 0; i < screenHeight + 2; i++ ){
                cout << setfill(' ') << setw(screenLenght + 1) << " " << endl;
            }
            setFixedPointer(position_begin);
        }

        void createMap(int mapId){
            int currentMap = 0;
            int i = 0;
            ifstream inputFile("mappe.txt");
            while(getline(inputFile, temp)){ 
                if(temp == ";"){
                    currentMap++;
                    continue;
                }
                if(currentMap == mapId){
                    if(i < mapHeight){ 
                    map[i] = temp;
                    i++;
                    }
                }
            }
            inputFile.close();
        }

        void initializeObjectList(){
            sceneList = nullptr;
        }

        //la lista aggiunge gli elementi in cima così quando vengono distrutti 
        void addObjectToObjectList(object* nuovo){
            node* nodo = new node;
            nodo->ogetto = nuovo;
            nodo->next = sceneList;

            sceneList = nodo;
        }

        void destroyScene(){
            node* temp = sceneList;
            while(temp != NULL){
                node* next = temp->next;
                delete temp->ogetto;
                delete temp;
                temp = next;
            }

            sceneList = nullptr;

            delete this;
        }
};

class interactable : public object{
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
    private : 
        enum buttonTypes buttonType;
    public :

        enum buttonTypes getType(){return buttonType;}

        button(string text, coordinates position, enum buttonTypes type = generic, bool shown = false, char indicator = '<' , bool selected = false) : buttonType(type), interactable(text,selected,shown, indicator){
            content = text;
            pos = position;
            toggleSelectedIcon();
            if(isShown){
                display(pos);
            }
        }

        button(string text = "nuovo bottone",enum buttonTypes type = generic,enum alignment alignment = align_left, int yPosition = 1 , bool shown = false, char indicator = '<' , bool selected = false): buttonType(type), interactable(text,selected,shown, indicator){
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

class entity : public object{
    

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

class gameController : public object{
    private:
        int button_count;
        node4d* buttonList = nullptr;
        pointerlist4d* buttonPointers = nullptr;
        gameState state;
        node4d* selectedButton;
        scene* scenaPrincipale;
    public:        

        void setPrincipalScene(scene& scena){scenaPrincipale = &scena;}

        scene& getPrincipalScene(){return *scenaPrincipale;} 
        
        enum buttonTypes getSelectedButtonType(){
            button* tmp =dynamic_cast<button*>(selectedButton->oggetto);
            if(tmp == NULL){
                return generic;
            }
            return tmp->getType();
        }
    
        gameController(){
            button_count = 0;
            state = menu;
        }

        void addButton(button* newButton, coordinates position){
            node4d* nodo = new node4d;
            if(buttonList == nullptr){
                
                newButton->toggleSelected();
                selectedButton = nodo;

                nodo->oggetto = newButton;
                nodo->up = NULL;
                nodo->down = NULL;
                nodo->left = NULL;
                nodo->right = NULL;
                nodo->posizione = position;

                buttonList = nodo;

                buttonPointers = new pointerlist4d();
                buttonPointers->pointer = nodo;
                buttonPointers->next = NULL;

                return;
            }

            addbuttonPointer(nodo);

            nodo->oggetto = newButton;
            nodo->up = getNeighbour(position,directions::up);
            nodo->down = getNeighbour(position,directions::down);
            nodo->left = getNeighbour(position,directions::left);
            nodo->right = getNeighbour(position,directions::right);
            nodo->posizione = position;
            
        }

        void addbuttonPointer(node4d* nodo){
            pointerlist4d* tmp = new pointerlist4d();
            tmp->pointer = nodo;
            tmp->next = buttonPointers;
            buttonPointers = tmp;
        }

        node4d* getNeighbour(coordinates position, directions direction){
            node4d* neighbour = gotoNode(position);
            if(neighbour == NULL){
                return NULL;
            }
            switch(direction){
                case directions::up: 
                    return neighbour->up;
                case directions::down: 
                    return neighbour->down;
                case directions::right:
                    return neighbour->right; 
                case directions::left: 
                    return neighbour->left;
                default:
                    return NULL;
            }
        }

        node4d* gotoNode(coordinates position){
            node4d* pointerNode = buttonList;
            for(int i = 0; i != position.get_x(); i++){
                if(pointerNode->right == NULL){
                    return NULL;
                }
                pointerNode = pointerNode->right;  
            }
            for(int i = 0; i != position.get_y(); i++){
                if(pointerNode->down == NULL){
                    return NULL;
                }
                pointerNode = pointerNode->down;  
            }
            return pointerNode;
        }

        void changeSelectedButton(enum directions direction){
            button* tmp = dynamic_cast<button*>(selectedButton->oggetto);
            tmp->toggleSelected();
            switch (direction)
            {
            case directions::up:
                if(selectedButton->up != NULL){
                    selectedButton = selectedButton->up;
                }
                break;
            case directions::down:
                if(selectedButton->down != NULL){
                    selectedButton = selectedButton->down;
                }
                break;
            case directions::left:
                if(selectedButton->left != NULL){
                    selectedButton = selectedButton->left;
                }
                break;
            case directions::right:
                if(selectedButton->right != NULL){
                    selectedButton = selectedButton->right;
                }
                break;
            default:
                break;
            }
            tmp = dynamic_cast<button*>(selectedButton->oggetto);
            tmp->toggleSelected();
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
        
        void onButtonClicked(enum buttonTypes type){
            switch (type){
                case newGameButton:
                    getPrincipalScene().deleteScene();
                    getPrincipalScene().createMap(1);
                    getPrincipalScene().draw();
                    break;
            
            default:
                break;
            }
        }
};

gameController gameManager;

void run(scene* scena);
void intro(scene* scena);

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

    scene* scena1 = new scene("prova");
    gameManager.setPrincipalScene(*scena1);

    run(scena1);

    system("Pause");

    return 0;
}

void run(scene* scena){
    intro(scena);
}


void intro(scene* scena){
    drawLine();
    for(int i = 0; i < screenHeight; i++){
        cout << "|" << setfill(' ') << setw(screenLenght) << "|" << endl;
    }
    drawLine();

    button* primo = new button("New Game", newGameButton, align_center, 20, true);
    button* secondo = new button("Continue", continueButton, align_center, 21, true);
    button* terzo = new button("Options", optionButton, align_center, 22, true);
    button* quarto = new button("Exit", exitButton, align_center, 23, true);

    gameManager.addButton(primo, coordinates(1,1));
    gameManager.addButton(secondo, coordinates(1,2));
    gameManager.addButton(terzo, coordinates(1,3));
    gameManager.addButton(quarto, coordinates(1,4));

    scena->addObjectToObjectList(primo);
    scena->addObjectToObjectList(secondo);
    scena->addObjectToObjectList(terzo);
    scena->addObjectToObjectList(quarto);

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
    switch(position){
        case position_end :
            cout << "\033[30;1H";
            break;
        case position_start :
            cout << "\033[2;2H";
            break;
        case position_begin :
            cout << "\033[1;1H";
            break;
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

/*
Su → 72

Giù → 80

Sinistra → 75

Destra → 77

        
*/

void menuInputManager(){
    
    while(true){
        int input = _getch();
        if(input == 0 || input == 224){ //l'input deve essere controllato 2 volte nelle frecce direzionali perchè restituiscono 2 interi
            int arrow = _getch();
            switch (arrow){
                case 72 :
                    gameManager.changeSelectedButton(directions::up);
                    break;
                case 80 :
                    gameManager.changeSelectedButton(directions::down);
                    break;
                case 75 :
                    gameManager.changeSelectedButton(directions::left);
                    break;
                case 77 :
                    gameManager.changeSelectedButton(directions::right);
                    break;
            }
            continue;
        }

        switch(input){
        case 'w' :
            gameManager.changeSelectedButton(directions::up);
            break;
        case 'a' :
            gameManager.changeSelectedButton(directions::left);
            break;
        case 's' :
            gameManager.changeSelectedButton(directions::down);
            break;
        case 'd' :
            gameManager.changeSelectedButton(directions::right);
            break;    
        case '\r' :
            gameManager.onButtonClicked(gameManager.getSelectedButtonType());
            break;
        case 27 :
            cout << "exit" << endl;
            break;
        default:
            return;
        }
    } 
    setFixedPointer(position_end);
}

void combatInputManager(){
    while(true){
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
        case '\r' :
            
            break;
        case '\033' :
            
            break; 
        default:
            return;
        }
    }
    setFixedPointer(position_end);
}

void worldInputManager(){
    while(true){
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
        case '\r' :
            break;
        case '\033' :
            break; 
        default:
            return;
        }
    }
    setFixedPointer(position_end);
}

void clearLine(int line){
    cout << "\033[" <<line << ";1H" << "|" << setfill(' ') << setw(screenLenght) << "|";
    setFixedPointer(position_end);
}