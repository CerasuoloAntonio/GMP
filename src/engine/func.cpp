#include <iostream>
#include <fstream>
#include <gamenum.cpp>
#include <coordinates.cpp>
#include <iomanip>
#include <arrays.cpp>
#include <scene.cpp>

#ifdef _WIN32
    #include <windows.h>
#endif

void setFixedPointer(enum alignment position){
    switch(position){
        case position_end :
            std::cout << "\033[30;1H";
            break;
        case position_start :
            std::cout << "\033[2;2H";
            break;
        case position_begin :
            std::cout << "\033[1;1H";
            break;
    }
}

coordinates linkDirection(int id, directions cardinalDirection, std::string line){
    coordinates result;
    std::string word;
    char direction;
    int p1,p2,p3;
    word.clear();
    switch(cardinalDirection){
        case directions::nord :
            direction = 'S';
            break;
        case directions::sud :
            direction = 'N';
            break;
        case directions::est :
            direction = 'O';
            break;
        case directions::ovest :
            direction = 'E';
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

coordinates getMapLink(int mapId, directions compassDirection){
    coordinates position;
    std::string line;
    int i = 0;
    std::ifstream linkFile("Data/MapData.txt");
    while(getline(linkFile,line)){
        if(i == mapId - 1){
            position = linkDirection(mapId,compassDirection,line);
        }
        i++;
    }
    linkFile.close();
    return position;
}

void intro(scene* scena){

    drawEmptyScene();

    button* primo = new button("New Game", buttonTypes::newGameButton, alignment::align_center, 20, true);
    button* secondo = new button("Continue", buttonTypes::continueButton, alignment::align_center, 21, true);
    button* terzo = new button("Options", buttonTypes::optionButton, alignment::align_center, 22, true);
    button* quarto = new button("Exit", buttonTypes::exitButton, alignment::align_center, 23, true);

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
    setFixedPointer(alignment::position_end);
    gameManager.checkState();
}

void drawLine(){
    std::cout << "|" << std::setfill('~') << std::setw(screenLenght) << "|" << std::endl;
}

void write(std::string phrase, int line,enum alignment align){

    int stringSize = phrase.length();
    int screenCenter = (screenLenght/2)+(stringSize/2);

    for(int i = screenHeight; i > line; i--){
        std::cout << "\033[A";
    }

    switch (align) {
        case align_center:
            std::cout << "\033[C" << std::setfill(' ') << std::setw(screenCenter) << phrase ;
            break;
        case align_left:
            std::cout << "\033[C" << phrase;
            break;
        case align_right:
            std::cout << "\033[C" << std::setfill(' ') << std::setw(screenLenght - 1) << phrase;
            break;
    }

    for(int i = line; i < screenHeight; i++){
        std::cout << "\033[B\r";
    }

}

void writeTitle(std::string phrase){
    int offset = 30;
    int phraseLenght = phrase.length();
    std::string segment = "";
    std::cout << "\033[5;1H";
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
        std::cout << "\033[" << offset << "C" << segment << std::endl;
    }
    
}

void drawLetter(int n){ // non utilizzata
    for(int i = 0; i < letterLines; i++){
        std::cout << alphabet[n][i] << "\033[B\033[" << alphabet[n][i].length() <<"D";
    }
    std::cout << "\033[" << 5 << "C" << "\033[" << letterLines <<"A";
}

void printAlphabet(){
    for(int i = 0; i < 26; i++){
        for(int n = 0; n < letterLines; n++){
            std::cout << alphabet[i][n] << std::endl;
        }
        std::cout << std::endl;
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
    setFixedPointer(alignment::position_end);
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
    setFixedPointer(alignment::position_end);
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
    setFixedPointer(alignment::position_end);
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
    setFixedPointer(alignment::position_end);
}

void clearLine(int line){
    std::cout << "\033[" <<line << ";1H" << "|" << std::setfill(' ') << std::setw(screenLenght) << "|";
    setFixedPointer(alignment::position_end);
}

void drawEmptyScene(){
    drawLine();
    for(int i = 0; i < screenHeight; i++){
        std::cout << "|" << std::setfill(' ') << std::setw(screenLenght) << "|" << std::endl;
    }
    drawLine();
}

void saveState(){
    coordinates lastPlayerPosition = gameManager.getPrincipalScene().getPlayer()->getPosition();
    int mapId = gameManager.getPrincipalScene().getId(); 
    std::ofstream savefile("Data/SaveFile.txt");
    savefile  << "map = " << mapId << std::endl << "player_position = (" << lastPlayerPosition.get_x() << ";" << lastPlayerPosition.get_y() << ")"; 
    savefile.close();
}

void loadSavestate(){
    int i = 0;
    std::string word,line;
    int x,y;
    int sceneId;
    int p1,p2,p3;
    std::ifstream savefile("Data/SaveFile.txt");
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