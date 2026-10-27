#include <object.cpp>
#include <iostream>
#include <coordinates.cpp>

class interactable : public object{
    protected :

        bool isSelected;
        char selectedIndicator;
        std::string content;
        std::string Basecontent;
        bool isShown;
        coordinates pos;

    public :

        interactable(std::string text = "Nuovo interagibile", bool selected = false, bool shown = false, char indicator = '<') : content(text), Basecontent(text),isSelected(selected), isShown(shown), selectedIndicator(indicator){

        }   

        void display(coordinates position){
            
            std::cout << "\033[" << position.get_y() << ";" << position.get_x() << "H" << content << std::endl;
        }

        bool getShown(){return isShown;}
        bool getSelected(){return isSelected;}

};
