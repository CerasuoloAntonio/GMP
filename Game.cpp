#include <iostream>
#include <cstdlib> // per il random e per exit()
#include <iomanip>
#include <fstream>
#include <conio.h>
#include <classincludes.cpp>

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
            getPrincipalScene().loadNewScene(sceneID);
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

        void deleteButtonList(){ // cancellare i pulsanti con delete
            buttonList = nullptr;
        }
};

gameController gameManager;
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