#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class MusicPlayer {
    
    public:
        MusicPlayer();
        bool initialise();
        void cleanUp();
        bool loadAll();
        
        bool loadSoundBuffer(string filename);
        SoundBuffer &getSoundBuffer(string filename);
        
        bool loadSound(string filename);
        Sound &getSound(string filename);
        
        void playSound(string filename);

        const string hitSound;
        const string deathSound;

    private:
        map<string, SoundBuffer*> soundBuffers;
        map<string, Sound*> sounds;
        
};

#endif

