#include "musicPlayer.h"
#include <iostream>

using namespace std;
using namespace sf;

MusicPlayer::MusicPlayer(): hitSound("./resources/sfx/hit.wav"),
                            deathSound("./resources/sfx/death.wav") {}

bool MusicPlayer::initialise() {
    if (loadAll()) { return true; }
    return false;
}

void MusicPlayer::cleanUp() {
    map<string, SoundBuffer*>::iterator snd_it;
    for (snd_it = soundBuffers.begin(); snd_it != soundBuffers.end(); snd_it++) {
        delete snd_it->second;
        snd_it->second = NULL;
    }
    soundBuffers.clear();
}

bool MusicPlayer::loadAll() {
    if (!loadSoundBuffer(hitSound)) return false;
    if (!loadSoundBuffer(deathSound)) return false;
    if (!loadSound(hitSound)) return false;
    if (!loadSound(deathSound)) return false;
  return true;
}

bool MusicPlayer::loadSoundBuffer(string filename){
    if (soundBuffers.find(filename) != soundBuffers.end()) return false;

    SoundBuffer *soundBuffer = new SoundBuffer();
    err().rdbuf(NULL);

    if (!soundBuffer->loadFromFile(filename)) {
        cout<<"ERROR CARGANDO"<<endl;
        delete soundBuffer; soundBuffer = NULL; return false;
    }
    
    Sound toPlay;
    toPlay.setBuffer(*soundBuffer);
    //toPlay.setBuffer(*soundBuffers.find(filename)->second);
    toPlay.play();
    
    soundBuffers.insert(pair<string, SoundBuffer*>(filename, soundBuffer));

    return true;
}

SoundBuffer &MusicPlayer::getSoundBuffer(string filename) {
    return *(soundBuffers.find(filename)->second);
}

bool MusicPlayer::loadSound(string filename){
    if (sounds.find(filename) != sounds.end()) return false;

    Sound *sound = new Sound();
    err().rdbuf(NULL);

    sound->setBuffer(getSoundBuffer(filename));
    
    sounds.insert(pair<string, Sound*>(filename, sound));

    return true;
}

Sound &MusicPlayer::getSound(string filename) {
    return *(sounds.find(filename)->second);
}

void MusicPlayer::playSound(string filename) {
    getSound(filename).play();
}