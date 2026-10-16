#include <object.cpp>
#include <gamenum.cpp>
#include <iostream>
#include <info.cpp>
#include <iomanip>
#include <node.cpp>
#include <layer.cpp>
#include <player.cpp>
#include <func.cpp>
#include <gamenum.cpp>

class scene : public object{
    private : 

        node* sceneList = nullptr;
        layer* layer_1 = new layer();
        player* playerController = nullptr; 

        std::string temp;
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
                    if(y > 1 && !isColliding(playerController,directions::up)){
                        if(isInBorder(directions::up)){
                            loadNearMap(id,directions::nord);
                            return;
                        }   
                        std::cout << "\033["<< y << ";" << x << "H" << line << "\033["<< y - 1 << ";" << x << "H" << sprite;
                        playerController->moveUp();
                    }
                    break;
                case directions::down:
                    if(y - 1 < screenHeight + 1 && !isColliding(playerController,directions::down)){
                        if(isInBorder(directions::down)){
                            loadNearMap(id,directions::sud);
                            return;
                        } 
                        std::cout << "\033["<< y << ";" << x << "H" << line << "\033["<< y + 1 << ";" << x << "H" << sprite;
                        playerController->moveDown();
                    }
                    break;
                case directions::left:
                    if(x > 1 && !isColliding(playerController,directions::left)){   
                        if(isInBorder(directions::left)){
                            loadNearMap(id,directions::ovest);
                            return;
                        }
                        std::cout << "\033["<< y << ";" << x << "H" << line << "\033["<< y  << ";" << x - 1 << "H" << sprite;
                        playerController->moveLeft();
                    }
                    break;
                case directions::right:
                    if(x - 1 < screenLenght && !isColliding(playerController,directions::right)){ 
                        if(isInBorder(directions::right)){
                            loadNearMap(id,directions::est);
                            return;
                        }   
                        std::cout << "\033["<< y << ";" << x << "H" << line << "\033["<< y << ";" << x + 1 << "H" << sprite;
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

        ~scene(){
            delete layer_1;
            delete playerController;
            destroyScene();
        }

        void draw(){
            for(int i = 0; i < mapHeight; i++){
                std::cout << layer_1->getLayerLine(i) << std::endl;
            }
            drawPlayer();
        }
    
        void loadScene(int loadMapId){
            setId(loadMapId);
            layer_1->setLayer(loadMapId);
        }

        void deleteScene(){
            setFixedPointer(alignment::position_begin);
            for(int i = 0; i < mapHeight; i++ ){
                std::cout << std::setfill(' ') << std::setw(screenLenght + 1) << " " << std::endl;
            }
            setFixedPointer(alignment::position_begin);
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
            std::cout << "\033[" << playerPosition.get_y() << ";" << playerPosition.get_x() << "H" << playerController->getEntitySprite();
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
            std::ifstream mapNeighbors("Data/MapNeighbors.txt");
            std::string line,word;
            int p1,p2;
            while (getline(mapNeighbors,line)){
                if(stoi(word = line[0]) == id){ // cambiare stoi per fargi ricevere più di un numero
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
        
        void loadNearMap(int mapid, directions direction){
            int newId = getNearMap(mapid,direction);;
            if(newId != 0){
                playerController->setPosition(getMapLink(newId,direction));
                loadNewScene(newId);
                id = newId;
            }
        }

        void loadNewScene(int id){
            destroyScene();
            deleteScene();
            loadScene(id);
            draw();
        }

};