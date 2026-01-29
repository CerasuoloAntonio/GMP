#include <iostream>
#include <cstdlib> // per il random e per exit()
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

class coordinates;
class scene;

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
    right,
    nord,
    sud,
    est,
    ovest
};

enum gameState{
    mainMenu,
    menu,
    world,
    combat
};

enum buttonTypes{
    newGameButton,
    continueButton,
    exitButton,
    generic,
    optionButton,
    inventoryButton,
    classButton,
    saveButton,
    equipButton
};

const int screenHeight = 27;
const int screenLenght = 119;
const int letterLines = 5;
const int mapHeight = 29;




void drawLine();
void write(string phrase, int line, enum alignment align);
void writeTitle(string phrase);
void drawLetter(int n);
void printAlphabet();
void setFixedPointer(enum alignment position);
void mainMenuInputManager();
void menuInputManager();
void combatInputManager();
void worldInputManager();
void clearLine(int line);
void drawEmptyScene();
void saveState();
void loadSavestate();
void intro(scene* scena);

int calculateAlignmentX(enum alignment position, int textLength);
int calculateAlignmentY(enum alignment position, int textLength);

coordinates getMapLink(int mapId, directions compassDirection);
coordinates linkDirection(int id, directions cardinalDirection, string line);

class object{
    public :
        virtual ~object(){}
};

struct node{
    object* ogetto;
    node* next;
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

        void printCoordinates(){
            cout << "x = " << x << " y = " << y << endl;
        }

        void set_x(int i){x=i;}
        void set_y(int i){y=i;}
        int get_x(){return x;}
        int get_y(){return y;}
};

class layer : public object{
    private :
        static const int mapheight = 29;
        string layer[mapheight];

    public : 
        string getLayerLine(int line){return layer[line];}

        void setLayer(int layerId){
            int currentMap = 0;
            int i = 0;
            string temp;
            ifstream inputFile("Data/mappe.txt");
            while(getline(inputFile, temp)){ 
                if(temp == ";"){
                    currentMap++;
                    continue;
                }
                if(currentMap == layerId){
                    if(i < mapheight){ 
                    layer[i] = temp;
                    i++;
                    }
                }
            }
            inputFile.close();
        }

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

class entity : public object{
    
    protected :
        const int minHealth = 0;
        coordinates position;
        char sprite = '@';
        int attack;
        int maxHealth;

    public :
        int health;
        int defense;

    void setPosition(coordinates newPosition){position = newPosition;}
    coordinates getPosition(){return position;}
    char getEntitySprite(){return sprite;}

    entity(){
        maxHealth = 100;
        health = maxHealth;
        attack = 10;
        position = coordinates(2,2);
    }

    void attackEnemy(entity &enemy){
        enemy.health -= attack;
    }
};

class player : public entity{
    private :
        
    public :

        player(){
            position = getPosition();
        }

        void moveLeft(){
            if(position.get_x() > 2){
                position.set_x(position.get_x() - 1);
            }
        }
        void moveRight(){
            if(position.get_x() < screenLenght){
                position.set_x(position.get_x() + 1);
            }
        }
        void moveUp(){
            if(position.get_y() > 2){
                position.set_y(position.get_y() - 1);
            }
        }
        void moveDown(){
            if(position.get_y() < screenHeight + 1){
                position.set_y(position.get_y() + 1);
            }
        }
};

class monster : public entity{

};

class scene : public object{
    private : 

        node* sceneList;
        layer* layer_1 = new layer();
        player* playerController = nullptr; 

        string temp;
        coordinates playerPosition;
  
        int id;
        
    public :

        void setPlayer(player* newPlayer){playerController = newPlayer;}
        void setId(int newId){id=newId;}

        int getId(){return id;}
        player* getPlayer(){return playerController;}
        layer* getFirstLayer(){return layer_1;}

        void moveplayer(directions direction){
            
            int x = playerController->getPosition().get_x();
            int y = playerController->getPosition().get_y();
            char line = layer_1->getLayerLine(y - 1)[x + 1];
            char sprite = playerController->getEntitySprite();

            switch (direction){
                case directions::up: 
                    if(y > 2 && !isColliding(playerController,directions::up)){
                        if(isInBorder(directions::up)){
                            //carica la mappa di sopra
                            return;
                        }   
                        cout << "\033["<< y << ";" << x << "H" << line << "\033["<< y - 1 << ";" << x << "H" << sprite;
                        playerController->moveUp();
                    }
                    break;
                case directions::down:
                    if(y < screenHeight + 1 && !isColliding(playerController,directions::down)){
                        if(isInBorder(directions::down)){
                            return;
                        } 
                        cout << "\033["<< y << ";" << x << "H" << line << "\033["<< y + 1 << ";" << x << "H" << sprite;
                        playerController->moveDown();
                    }
                    break;
                case directions::left:
                    if(x > 2 && !isColliding(playerController,directions::left)){   
                        if(isInBorder(directions::left)){
                            return;
                        }
                        cout << "\033["<< y << ";" << x << "H" << line << "\033["<< y  << ";" << x - 1 << "H" << sprite;
                        playerController->moveLeft();
                    }
                    break;
                case directions::right:
                    if(x < screenLenght && !isColliding(playerController,directions::right)){ 
                        if(isInBorder(directions::right)){
                            return;
                        }   
                        cout << "\033["<< y << ";" << x << "H" << line << "\033["<< y << ";" << x + 1 << "H" << sprite;
                        playerController->moveRight();            
                    }
                    break;
                default:
                    break;
            }
        }

        scene(){
            initializeObjectList();
        }

        void draw(){
            for(int i = 0; i < mapHeight; i++){
                cout << layer_1->getLayerLine(i) << endl;
            }
            drawPlayer();
        }
    
        void loadScene(int loadMapId){
            setId(loadMapId);
            layer_1->setLayer(loadMapId);
            addObjectToObjectList(layer_1);
        }

        void deleteScene(){
            setFixedPointer(position_begin);
            for(int i = 0; i < mapHeight; i++ ){
                cout << setfill(' ') << setw(screenLenght + 1) << " " << endl;
            }
            setFixedPointer(position_begin);
        }

        void initializeObjectList(){
            sceneList = nullptr;
        }

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

        bool isColliding(entity* collider, enum directions direction){

            coordinates tmp = collider->getPosition();
            int x = tmp.get_x();
            int y = tmp.get_y();
            bool result = false;

            switch(direction){
                case directions::up :
                    if(collision(layer_1->getLayerLine(y - 2)[x + 1])){
                        result = true;
                    }
                    break;
                case directions::down :
                    if(collision(layer_1->getLayerLine(y)[x + 1])){
                        result = true;
                    }
                    break;
                case directions::left :
                    if(collision(layer_1->getLayerLine(y - 1)[x])){
                        result = true;
                    }
                    break;
                case directions::right :
                    if(collision(layer_1->getLayerLine(y - 1)[x + 2])){
                        result = true;
                    }
                    break;
            }
            return result;
        }

        bool collision(char collider){
            switch (collider){
            case 'T' :
                return true;
            case '#' :
                return true;
            case '~' :
                return true;
            case '^' :
                return true;
            default:
                return false;
            }
        }

        void drawPlayer(){
            coordinates playerPosition = playerController->getPosition();
            cout << "\033[" << playerPosition.get_y() << ";" << playerPosition.get_x() << "H" << playerController->getEntitySprite();
        }

        bool isInBorder(directions direction){
             
            int x = playerController->getPosition().get_x();
            int y = playerController->getPosition().get_y();
            switch (direction)
            {
            case directions::up:
                if(y == 2){
                    return true;
                }
                break;
                
            case directions::down:
                if(y == screenHeight + 1){
                    return true;
                }
                break;

            case directions::left:
                if(x == 2){
                    return true;
                }
                break;

            case directions::right:
                if(x == screenLenght){
                    return true;
                }
                break;

            default:
                return false;
                break;
            }
            return false;
        }

        int getNearMap(int id,directions direction){
            ifstream mapNeighbors("Data/MapNeighbors.txt");
            string line,word;
            int p1,p2;
            while (getline(mapNeighbors,line)){
                if(stoi(word = line[0]) == id){
                    switch(direction){
                        case directions::nord :
                            p1 = line.find('N');
                            p2 = line.find(',',p1);
                            word = line.substr(p1 + 4 , p2 - (p1 + 4));// il 4 è l'offset di distanza tra la n e il numero
                            return stoi(word);
                            break;
                        case directions::sud :
                            p1 = line.find('S');
                            p2 = line.find(',',p1);
                            word = line.substr(p1 + 4 , p2 - (p1 + 4));
                            return stoi(word);
                            break;
                        case directions::est :
                            p1 = line.find('E');
                            p2 = line.find(',',p1);
                            word = line.substr(p1 + 4 , p2 - (p1 + 4));
                            return stoi(word);
                            break;
                        case directions::ovest :
                            p1 = line.find('W');
                            p2 = line.find(',',p1);
                            word = line.substr(p1 + 4 , p2 - (p1 + 4));
                            return stoi(word);
                            break;
                        default :
                            return 0;
                    }
                    
                }   
                    
            }
            
            mapNeighbors.close();
            return 0;
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
        }

        void toggleSelectedIcon(){
            int xPos = pos.get_x() + content.length() + 1;
            if(isSelected){
                cout << "\033[" << pos.get_y() << ";" << xPos << "H" << selectedIndicator;
            }
            else{
                cout << "\033[" << pos.get_y() << ";" << xPos << "H" << " ";
            }
        }

        void toggleshow(){
            isShown = !isShown;
            if(isShown){
                display(pos);
            }
        }

}; 



class gameController : public object{
    private:

        int button_count;
        bool isMenuActive;

        gameState state;

        node4d* selectedButton;
        scene* scenaPrincipale;
        node4d* buttonList = nullptr;
        pointerlist4d* buttonPointers = nullptr;

    public:        

        void setPrincipalScene(scene& scena){scenaPrincipale = &scena;}
        void setGameState(gameState newState){ state = newState; checkState();}

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
            state = mainMenu;
            isMenuActive = false;
        }

        void addButton(button* newButton, coordinates position, bool drawLater){
            node4d* nodo = new node4d;
            if(buttonList == nullptr){
                if(!drawLater){
                    newButton->toggleSelected();
                }
                
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
            nodo->up = getNeighbor(position,directions::up);
            nodo->down = getNeighbor(position,directions::down);
            nodo->left = getNeighbor(position,directions::left);
            nodo->right = getNeighbor(position,directions::right);
            nodo->posizione = position;
            
            updateListLinks();
        }

        void addbuttonPointer(node4d* nodo){
            pointerlist4d* tmp = new pointerlist4d();
            tmp->pointer = nodo;
            tmp->next = buttonPointers;
            buttonPointers = tmp;
        }

        node4d* getNeighbor(coordinates position, directions direction){
            node4d* neighbor = gotoNode(position);
            if(neighbor == NULL){
                return NULL;
            }
            switch(direction){
                case directions::up: 
                    return neighbor->up;
                case directions::down: 
                    return neighbor->down;
                case directions::right:
                    return neighbor->right; 
                case directions::left: 
                    return neighbor->left;
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
                case mainMenu :
                    mainMenuInputManager();
                    break;
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
            deleteButtonList();
            switch (type){
                case newGameButton:{
                    newScene(1);
                    setGameState(world);
                    break;
                    }
                case saveButton :
                    saveState();
                    break;
                case continueButton :
                    loadSavestate();
                    break;
                case exitButton : 
                    exit(0);
                    break;
                case optionButton :
                    break;
            default:
                break;
            }
        }
        
        void updateListLinks(){
            coordinates nodePosition, neighborPosition;
            pointerlist4d* tmp = buttonPointers; 
            pointerlist4d *next = buttonPointers->next;
            int diff_x, diff_y;
            while (tmp != NULL){

                next = tmp->next;
                nodePosition = tmp->pointer->posizione;
                
                while (next != NULL){

                    neighborPosition = next->pointer->posizione;

                    diff_x = nodePosition.get_x() - neighborPosition.get_x();
                    diff_y = nodePosition.get_y() - neighborPosition.get_y();

                    if(diff_x == 0 && diff_y == 1){
                        tmp->pointer->up = next->pointer;
                        next->pointer->down = tmp->pointer;
                    }
                    if(diff_x == 0 && diff_y == -1){
                        tmp->pointer->down = next->pointer;
                        next->pointer->up = tmp->pointer;
                    }
                    if(diff_x == -1 && diff_y == 0){
                        tmp->pointer->left = next->pointer;
                        next->pointer->right = tmp->pointer;
                    }
                    if(diff_x == 1 && diff_y == 0){
                        tmp->pointer->right = next->pointer;
                        next->pointer->left = tmp->pointer;
                    }

                    next = next->next;
                }

                tmp = tmp->next;
            }
            
        }

        void newScene(int sceneID){
            getPrincipalScene().deleteScene();
            getPrincipalScene().loadScene(sceneID);
            getPrincipalScene().draw();
        }


        void toggleMenu(){  
            int xButtonPos = 105;
            int xMenuPos = 103;       
            if(!isMenuActive && state == world){
                
                isMenuActive = !isMenuActive;

                int n = 0;

                button* inventory = new button("Inventory",coordinates(105, 4),inventoryButton,false);
                button* equipment = new button("Equip",coordinates(105, 6),equipButton,false);
                button* classB = new button("Class",coordinates(105, 8),classButton,false);
                button* save = new button("Save",coordinates(105, 10),saveButton,false);
                button* exit = new button("Exit",coordinates(105, 12),exitButton,false);

                button * tmp[5] = {inventory,equipment,classB,save,exit};

                addButton(inventory,coordinates(1,1),true);
                addButton(equipment,coordinates(1,2),true);
                addButton(classB,coordinates(1,3),true);
                addButton(save,coordinates(1,4),true);
                addButton(exit,coordinates(1,5),true);

                cout << "\033[2;103H" << setfill('_') << setw(15) << "_" << setfill(' ');

                for(int i = 3; i != 14; i++){
                    cout << "\033[" << i << ";" << xMenuPos << "H|" << setw(14) << "|" ;
                    if(i%2==0){
                        tmp[n]->toggleshow();
                        
                        if(n == 0){
                            tmp[n]->toggleSelected();
                        }
                        n++;
                    }
                }

                cout << "\033[14;103H" << setfill('_') << setw(15) << "_" << setfill(' ');

                setFixedPointer(position_end);
                
                setGameState(menu);
            }
            else{
                isMenuActive = !isMenuActive;
                deleteButtonList();

                layer* tmp = getPrincipalScene().getFirstLayer();
                string line;

                for(int i = 2; i != 15; i++){
                    line = tmp->getLayerLine(i-1);
                    for(int n = xMenuPos; n < xMenuPos + 15; n++){
                        cout << "\033[" << i << ";" << n << "H" << line[n + 1];
                    }
                }

                getPrincipalScene().drawPlayer();

                setFixedPointer(position_end);

                setGameState(world);
            }
        }

        void deleteButtonList(){
            buttonList = nullptr;
        }
};

gameController gameManager;



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

    scene* scena1 = new scene();
    gameManager.setPrincipalScene(*scena1);

    intro(scena1);

    system("Pause");

    return 0;
}

void intro(scene* scena){

    drawEmptyScene();

    button* primo = new button("New Game", newGameButton, align_center, 20, true);
    button* secondo = new button("Continue", continueButton, align_center, 21, true);
    button* terzo = new button("Options", optionButton, align_center, 22, true);
    button* quarto = new button("Exit", exitButton, align_center, 23, true);

    gameManager.addButton(primo, coordinates(1,1),false);
    gameManager.addButton(secondo, coordinates(1,2),false);
    gameManager.addButton(terzo, coordinates(1,3),false);
    gameManager.addButton(quarto, coordinates(1,4),false);

    scena->addObjectToObjectList(primo);
    scena->addObjectToObjectList(secondo);
    scena->addObjectToObjectList(terzo);
    scena->addObjectToObjectList(quarto);

    player* hero = new player();
    gameManager.getPrincipalScene().setPlayer(hero);

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

void drawLetter(int n){ // non utilizzata
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

void mainMenuInputManager(){
    
    while(true){
        int input = _getch();
        if(input == 0 || input == 224){ //l'input deve essere controllato 2 volte nelle frecce direzionali perchè restituiscono 2 interi
            int arrow = _getch();
            switch (arrow){
                case 72 : //freccia su
                    gameManager.changeSelectedButton(directions::up);
                    break;
                case 80 : //freccia giù
                    gameManager.changeSelectedButton(directions::down);
                    break; 
                case 75 : //freccia sinistra
                    gameManager.changeSelectedButton(directions::left);
                    break;
                case 77 : //freccia destra
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
            exit(0);
            break;
        default:
            return;
        }
    } 
    setFixedPointer(position_end);
}

void menuInputManager(){
    
    while(true){
        int input = _getch();
        if(input == 0 || input == 224){ //l'input deve essere controllato 2 volte nelle frecce direzionali perchè restituiscono 2 interi
            int arrow = _getch();
            switch (arrow){
                case 72 : //freccia su
                    gameManager.changeSelectedButton(directions::up);
                    break;
                case 80 : //freccia giù
                    gameManager.changeSelectedButton(directions::down);
                    break; 
                case 75 : //freccia sinistra
                    gameManager.changeSelectedButton(directions::left);
                    break;
                case 77 : //freccia destra
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
            gameManager.toggleMenu();
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
        case 27 :
            
            break; 
        default:
            return;
        }
    }
    setFixedPointer(position_end);
}

void worldInputManager(){
    while(true){
        int input = _getch();
        if(input == 0 || input == 224){ //l'input deve essere controllato 2 volte nelle frecce direzionali perchè restituiscono 2 interi
            int arrow = _getch();
            switch (arrow){
                case 72 : //freccia su
                    gameManager.getPrincipalScene().moveplayer(directions::up);
                    break;
                case 80 : //freccia giù
                    gameManager.getPrincipalScene().moveplayer(directions::down);
                    break; 
                case 75 : //freccia sinistra
                    gameManager.getPrincipalScene().moveplayer(directions::left);
                    break;
                case 77 : //freccia destra
                    gameManager.getPrincipalScene().moveplayer(directions::right);
                    break;
            }
            continue;
        }

        switch(input){
        case 'w' :
            gameManager.getPrincipalScene().moveplayer(directions::up);
            break;
        case 'a' :
            gameManager.getPrincipalScene().moveplayer(directions::left);
            break;
        case 's' :
            gameManager.getPrincipalScene().moveplayer(directions::down);
            break;
        case 'd' :
            gameManager.getPrincipalScene().moveplayer(directions::right);
            break;    
        case '\r' :
            
            break;
        case 27 :
            gameManager.toggleMenu();
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

void drawEmptyScene(){
    drawLine();
    for(int i = 0; i < screenHeight; i++){
        cout << "|" << setfill(' ') << setw(screenLenght) << "|" << endl;
    }
    drawLine();
}

void saveState(){
    coordinates lastPlayerPosition = gameManager.getPrincipalScene().getPlayer()->getPosition();
    int mapId = gameManager.getPrincipalScene().getId(); 
    ofstream savefile("Data/SaveFile.txt");
    savefile  << "map = " << mapId << endl << "player_position = (" << lastPlayerPosition.get_x() << ";" << lastPlayerPosition.get_y() << ")"; 
    savefile.close();
}

void loadSavestate(){
    int i = 0;
    string word,line;
    int x,y;
    int sceneId;
    int p1,p2,p3;
    ifstream savefile("Data/SaveFile.txt");
    while(getline(savefile,line)){
        word.clear();
        switch(i){
            case 0:
                    p1 = line.find('='); 
                    word = line[p1 +2];
                    sceneId = stoi(word);
                    break;
            case 1 : 
                p1 = line.find('(');
                p2 = line.find(';');
                p3 = line.find(')');

                word = line.substr(p1 + 1, p2 - (p1 + 1));

                x = stoi(word);
                word.clear();
                
                word = line.substr(p2 + 1, p3 - (p2 + 1));

                y = stoi(word);

                gameManager.getPrincipalScene().getPlayer()->setPosition(coordinates(x,y));

                break;
            default :
                break;
        }
        i++;
    }
    gameManager.newScene(sceneId);
    gameManager.setGameState(world);
    savefile.close();
}

coordinates getMapLink(int mapId, directions compassDirection){
    coordinates position;
    string line;
    int i = 0;
    ifstream linkFile("Data/MapData.txt");
    while(getline(linkFile,line)){
        if(i == mapId - 1){
            position = linkDirection(mapId,compassDirection,line);
        }
        i++;
    }
    linkFile.close();
    return position;
}

coordinates linkDirection(int id, directions cardinalDirection, string line){
    coordinates result;
    string word;
    char direction;
    int p1,p2,p3;
    word.clear();
    switch(cardinalDirection){
        case directions::nord :
            direction = 'N';
            break;
        case directions::sud :
            direction = 'S';
            break;
        case directions::est :
            direction = 'E';
            break;
        case directions::ovest :
            direction = 'O';
            break;
        default :
            return result;
    }

    p1 = line.find(direction);
    p2 = line.find(',', p1);
    p3 = line.find(')', p2);
    
    word = line.substr(p1 + 2, p2 - (p1 + 2));
    result.set_x(stoi(word));

    word = line.substr(p2 + 1, p3 - (p2 + 1));
    result.set_y(stoi(word));

    return result;
}