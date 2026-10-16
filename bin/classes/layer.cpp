#include <fstream>
#include <object.cpp>

class layer : public object{ 
    private :
        static const int mapheight = 29;
        std::string layer[mapheight];

    public : 
        std::string getLayerLine(int line){return layer[line];}

        void setLayer(int layerId){
            int currentMap = 0;
            int i = 0;
            std::string temp;
            std::ifstream inputFile("Data/mappe.txt");
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