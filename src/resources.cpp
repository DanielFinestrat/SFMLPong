#include "resources.h"
#include <iostream>

using namespace sf;
using namespace std;

Resources::Resources(): myFont("./resources/fonts/arial.ttf"),
                        myTexture("./resources/images/sprites.png"),
                        myBackground("./resources/images/background.png"){}

bool Resources::initialise() {
    if (loadAll()) { return true; }
    return false;
}

void Resources::cleanUp() {
    //Limpiar las fuentes
    map<string, Font*>::iterator font_it;
    for (font_it = fonts.begin(); font_it != fonts.end(); font_it++) {
        delete font_it->second;
        font_it->second = NULL;
    }
    fonts.clear();

    //Limpiar las texturas
    map<string, Texture*>::iterator texture_it;
    for (texture_it = textures.begin(); texture_it != textures.end(); texture_it++) {
        delete texture_it->second;
        texture_it->second = NULL;
    }
    textures.clear();
    
    //Limpiar las imagenes
    map<string, Image*>::iterator image_it;
    for (image_it = images.begin(); image_it != images.end(); image_it++) {
        delete image_it->second;
        image_it->second = NULL;
    }
    images.clear();
}

bool Resources::loadAll() {
    if (!loadFont(myFont)) return false;
    if (!loadTexture(myTexture)) return false;
    if (!loadTexture(myBackground)) return false;
  return true;
}

bool Resources::loadFont(string filename) {
    if (fonts.find(filename) != fonts.end()) return false;

    Font *font = new Font();
    err().rdbuf(NULL);

    if (!font->loadFromFile(filename)) {
        cout<<"ERROR CARGANDO"<<endl;
        delete font; font = NULL; return false;
    }

    const_cast<sf::Texture&>(font->getTexture(8)).setSmooth(false);
    const_cast<sf::Texture&>(font->getTexture(12)).setSmooth(false);
    const_cast<sf::Texture&>(font->getTexture(16)).setSmooth(false);
    const_cast<sf::Texture&>(font->getTexture(24)).setSmooth(false);

    fonts.insert(pair<string, Font*>(filename, font));

    return true;
}

Font &Resources::getFont(string filename) {
    return *(fonts.find(filename)->second);
}

bool Resources::loadTexture(string filename){
    if (textures.find(filename) != textures.end()) return false;

    loadImage(filename); //Cargamos la imagen si nuestra textura no existe
    
    Texture *texture = new Texture();
    err().rdbuf(NULL);

    //Le asignamos la imagen guardada a la textura
    if (!texture->loadFromImage(getImage(filename))) {
        cout<<"ERROR CARGANDO"<<endl;
        delete texture; texture = NULL; return false;
    }

    texture->setSmooth(true);
    textures.insert(pair<string, Texture*>(filename, texture));
    
    return true;
}

Texture &Resources::getTexture(string filename) {
    return *(textures.find(filename)->second);
}

bool Resources::loadImage(string filename){
    if (images.find(filename) != images.end()) return false;

    Image *image = new Image();
    err().rdbuf(NULL);

    if (!image->loadFromFile(filename)) {
        cout<<"ERROR CARGANDO"<<endl;
        delete image; image = NULL; return false;
    }

    images.insert(pair<string, Image*>(filename, image));

    return true;
}

Image &Resources::getImage(string filename) {
    return *(images.find(filename)->second);
}