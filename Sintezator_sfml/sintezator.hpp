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
        textures.resize(8);
        textures[0].loadFromFile("bongo_cat/bongo_cat_0.png");
        textures[1].loadFromFile("bongo_cat/bongo_cat_1.png");
        textures[2].loadFromFile("bongo_cat/bongo_cat_2.png");
        textures[3].loadFromFile("bongo_cat/bongo_cat_3.png");
        textures[4].loadFromFile("bongo_cat/bongo_cat_4.png");
        textures[5].loadFromFile("bongo_cat/bongo_cat_5.png");
        textures[6].loadFromFile("bongo_cat/bongo_cat_inhales.png");
        textures[7].loadFromFile("bongo_cat/bongo_cat_dre.png");

        sprite.setTexture(textures[0]);

        music.openFromFile("still_dre.ogg");

    };
    std::vector<Note> notes;
    std::vector<sf::SoundBuffer> buffers;
    std::vector<sf::Sound> sounds;

    std::vector<sf::Texture> textures;
    sf::Sprite sprite;
    std::size_t tex_type = 0;

    sf::Clock clock;
    sf::Time time;

    sf::SoundBuffer m_buff;
    sf::Music music;

    void process(sf::Event event) {
        if (event.type == sf::Event::KeyPressed) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                process_update(0);
                sprite.setTexture(textures[1]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                process_update(1);
                sprite.setTexture(textures[1]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                process_update(2);
                sprite.setTexture(textures[2]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                process_update(3);
                sprite.setTexture(textures[2]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                process_update(4);
                sprite.setTexture(textures[3]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
                process_update(5);
                sprite.setTexture(textures[3]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
                process_update(6);
                sprite.setTexture(textures[4]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                process_update(7);
                sprite.setTexture(textures[4]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                process_update(8);
                sprite.setTexture(textures[5]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                process_update(9);
                sprite.setTexture(textures[5]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
                process_update(10);
                sprite.setTexture(textures[1]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                process_update(11);
                sprite.setTexture(textures[1]);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                sprite.setTexture(textures[6]);
                if (tex_type == 1) {
                    tex_type = 0;
                } else {
                    tex_type = 1;
                }
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                sprite.setTexture(textures[7]);
                if (tex_type == 1) {
                    tex_type = 0;
                    music.pause();
                } else {
                    tex_type = 1;
                    music.play();
                }
            } 
        }
        
    }
    void process_update(std::size_t i) {
        sounds[i].play();
        clock.restart();
        tex_type = 0;
    }
    void update_tex() {
        time = clock.getElapsedTime();
        float t = time.asSeconds();
        if (t >= (DUR / 2)) {
            if (tex_type != 1) {
            sprite.setTexture(textures[0]);
            }
        }
    }
};