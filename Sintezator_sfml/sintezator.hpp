#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <assert.h>

#define AMPL 32000
#define S_RATE 44100
#define DUR 0.3f
#define D_LVL 0.8f
#define LIN_ATT 0.3f
#define OKTAVE 1.0f

class Note {
public:
    Note(float frequency, float distortion_level, float duration_in_sec) {
        freq = frequency;
        level = distortion_level;
        dur = duration_in_sec;
        make_samp(samples, freq, dur);
        //make_distortion(samples, level);
        //make_linear_attenuation(samples, dur);
        make_parabolic_attenuation(samples, dur);
    };
    Note(){};
    std::vector<sf::Int16> samples;
private:
    float freq;
    float level;
    float dur;
    void make_samp(std::vector<sf::Int16> &samples, float freq, float t) {
        float c_freq = 2 * M_PI * freq;
        for (std::size_t i = 0; i < (std::size_t)(S_RATE * t); i++) {
            sf::Int16 sample = (sf::Int16)(AMPL * sin(c_freq * i / S_RATE));
            samples.push_back(sample);
        }
    }
    void make_distortion(std::vector<sf::Int16> &sample, float part) {
        assert((part < 1) && (part > 0));
        float level = AMPL * part;
        for (auto& x : sample) {
            if (x > level) {
                x = level;
            } else if (x < ((-1) * level)) {
                x = (-1) * level;
            }
        }
    }
    void make_linear_attenuation(std::vector<sf::Int16> &sample, float t) {
        for (std::size_t i = 0; i < (std::size_t)(S_RATE * t); i++) {
            float lim = ((-1) * LIN_ATT * AMPL * i) / (t * S_RATE) + LIN_ATT * AMPL;
            if (sample[i] > lim) {
                sample[i] = lim;
            } else if (sample[i] < (-1) * lim) {
                sample[i] = (-1) * lim;
            }
        }
    }
    void make_parabolic_attenuation(std::vector<sf::Int16> &sample, float t) {
        for (std::size_t i = 0; i < (std::size_t)(S_RATE * t); i++) {
            float k = AMPL / pow(S_RATE * t, 2);
            float i_0 = t * S_RATE;
            float lim = k * pow(i - i_0, 2);
            if (sample[i] > lim) {
                sample[i] = lim;
            } else if (sample[i] < (-1) * lim) {
                sample[i] = (-1) * lim;
            }
        }
    }
};

class Sintezator {
public:
    Sintezator () {
        notes.push_back({OKTAVE * 261.63f, D_LVL, DUR});
        notes.push_back({OKTAVE * 277.18f, D_LVL, DUR});
        notes.push_back({OKTAVE * 293.66f, D_LVL, DUR});
        notes.push_back({OKTAVE * 311.13f, D_LVL, DUR});
        notes.push_back({OKTAVE * 329.63f, D_LVL, DUR});
        notes.push_back({OKTAVE * 349.23f, D_LVL, DUR});
        notes.push_back({OKTAVE * 369.99f, D_LVL, DUR});
        notes.push_back({OKTAVE * 392.00f, D_LVL, DUR});
        notes.push_back({OKTAVE * 415.30f, D_LVL, DUR});
        notes.push_back({OKTAVE * 440.00f, D_LVL, DUR});
        notes.push_back({OKTAVE * 466.16f, D_LVL, DUR});
        notes.push_back({OKTAVE * 493.88f, D_LVL, DUR});

        buffers.resize(12);
        sounds.resize(12);

        for (std::size_t i = 0; i < 12; i++) {
            buffers[i].loadFromSamples(&notes[i].samples[0], notes[i].samples.size(), 1, S_RATE);
            sounds[i].setBuffer(buffers[i]);
        }
        textures.resize(4);
        textures[0].loadFromFile("bongo_cat_0.png");
        textures[1].loadFromFile("bongo_cat_1.png");
        textures[2].loadFromFile("bongo_cat_2.png");
        textures[3].loadFromFile("bongo_cat_3.png");

        sprite.setTexture(textures[0]);

    };
    std::vector<Note> notes;
    std::vector<sf::SoundBuffer> buffers;
    std::vector<sf::Sound> sounds;

    std::vector<sf::Texture> textures;
    sf::Sprite sprite;

    sf::Clock clock;
    sf::Time time;

    void process(sf::Event event) {
        if (event.type == sf::Event::KeyPressed) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                sounds[0].play();
                sprite.setTexture(textures[1]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                sounds[1].play();
                sprite.setTexture(textures[1]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                sounds[2].play();
                sprite.setTexture(textures[1]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                sounds[3].play();
                sprite.setTexture(textures[1]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                sounds[4].play();
                sprite.setTexture(textures[2]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                sounds[5].play();
                sprite.setTexture(textures[2]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                sounds[6].play();
                sprite.setTexture(textures[2]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
                sounds[7].play();
                sprite.setTexture(textures[2]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
                sounds[8].play();
                sprite.setTexture(textures[3]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                sounds[9].play();
                sprite.setTexture(textures[3]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                sounds[10].play();
                sprite.setTexture(textures[3]);
                clock.restart();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                sounds[11].play();
                sprite.setTexture(textures[3]);
                clock.restart();
            }
        }
        
    }
    void update_tex() {
        time = clock.getElapsedTime();
        float t = time.asSeconds();
        if (t >= (DUR / 2)) {
            sprite.setTexture(textures[0]);
        }
    }
};