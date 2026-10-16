#include <interactable.cpp>
#include <iostream>

class button : public interactable {
    private : 
        enum buttonTypes buttonType;
    public :

        enum buttonTypes getType(){return buttonType;}

        button(std::string text, coordinates position, enum buttonTypes type = buttonTypes::generic, bool shown = false, char indicator = '<' , bool selected = false) : buttonType(type), interactable(text,selected,shown, indicator){
            content = text;
            pos = position;
            toggleSelectedIcon();
            if(isShown){
                display(pos);
            }
        }

        button(std::string text = "nuovo bottone",enum buttonTypes type = buttonTypes::generic,enum alignment alignment = alignment::align_left, int yPosition = 1 , bool shown = false, char indicator = '<' , bool selected = false): buttonType(type), interactable(text,selected,shown, indicator){
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
                std::cout << "\033[" << pos.get_y() << ";" << xPos << "H" << selectedIndicator;
            }
            else{
                std::cout << "\033[" << pos.get_y() << ";" << xPos << "H" << " ";
            }
        }

        void toggleshow(){
            isShown = !isShown;
            if(isShown){
                display(pos);
            }
        }

};