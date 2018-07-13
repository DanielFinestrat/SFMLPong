#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Resources {
    
    public:
        Resources();
        bool initialise();
        void cleanUp();
        bool loadAll();
        
        bool loadFont(string filename);
        Font &getFont(string filename);
        
        bool loadTexture(string filename);
        Texture &getTexture(string filename);
        
        bool loadImage(string filename);
        Image &getImage(string filename);

        const string myFont;
        const string myTexture;
        const string myBackground;

    private:
        map<string, Font*> fonts;
        map<string, Texture*> textures;
        map<string, Image*> images;

};

#endif
