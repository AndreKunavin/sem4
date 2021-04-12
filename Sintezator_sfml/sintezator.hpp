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
#define DUR 0.4f
#define D_LVL 0.8f

class Note {
public:
    Note(float frequency, float distortion_level, float duration_in_sec) {
        freq = frequency;
        level = distortion_level;
        dur = duration_in_sec;
        make_samp(samples, freq, dur);
        make_distortion(samples, level);
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
    void make_distortion(std::vector<sf::Int16> &samples, float part) {
        assert((part < 1) && (part > 0));
        float level = AMPL * part;
        for (auto& x : samples) {
            if (x > level) {
                x = level;
            } else if (x < ((-1) * level)) {
                x = (-1) * level;
            }
        }
    }
};

class Sintezator {
public:
    Sintezator () {
        C = {261.63f, D_LVL, DUR};
        Cd = {277.18f, D_LVL, DUR};
        D = {293.66f, D_LVL, DUR};
        Dd = {311.13f, D_LVL, DUR};
        E = {329.63f, D_LVL, DUR};
        F = {349.23f, D_LVL, DUR};
        Fd = {369.99f, D_LVL, DUR};
        G = {392.00f, D_LVL, DUR};
        Gd = {415.30f, D_LVL, DUR};
        A = {440.00f, D_LVL, DUR};
        Ad = {466.16f, D_LVL, DUR};
        H = {493.88f, D_LVL, DUR};

        buffer_C.loadFromSamples(&C.samples[0], C.samples.size(), 1, S_RATE);
        buffer_Cd.loadFromSamples(&Cd.samples[0], Cd.samples.size(), 1, S_RATE);
        buffer_D.loadFromSamples(&D.samples[0], D.samples.size(), 1, S_RATE);
        buffer_Dd.loadFromSamples(&Dd.samples[0], Dd.samples.size(), 1, S_RATE);
        buffer_E.loadFromSamples(&E.samples[0], E.samples.size(), 1, S_RATE);
        buffer_F.loadFromSamples(&F.samples[0], F.samples.size(), 1, S_RATE);
        buffer_Fd.loadFromSamples(&Fd.samples[0], Fd.samples.size(), 1, S_RATE);
        buffer_G.loadFromSamples(&G.samples[0], G.samples.size(), 1, S_RATE);
        buffer_Gd.loadFromSamples(&Gd.samples[0], Gd.samples.size(), 1, S_RATE);
        buffer_A.loadFromSamples(&A.samples[0], A.samples.size(), 1, S_RATE);
        buffer_Ad.loadFromSamples(&Ad.samples[0], Ad.samples.size(), 1, S_RATE);
        buffer_H.loadFromSamples(&H.samples[0], H.samples.size(), 1, S_RATE);
        
        sound_C.setBuffer(buffer_C);
        sound_Cd.setBuffer(buffer_Cd);
        sound_D.setBuffer(buffer_D);
        sound_Dd.setBuffer(buffer_Dd);
        sound_E.setBuffer(buffer_E);
        sound_G.setBuffer(buffer_G);
        sound_Gd.setBuffer(buffer_Gd);
        sound_F.setBuffer(buffer_F);
        sound_Fd.setBuffer(buffer_Fd);
        sound_A.setBuffer(buffer_A);
        sound_Ad.setBuffer(buffer_Ad);
        sound_H.setBuffer(buffer_H);
    };
    
    Note C;
    Note Cd;
    Note D;
    Note Dd;
    Note E;
    Note F;
    Note Fd;
    Note G;
    Note Gd;
    Note A;
    Note Ad;
    Note H;

    sf::SoundBuffer buffer_C;
    sf::SoundBuffer buffer_Cd;
    sf::SoundBuffer buffer_D;
    sf::SoundBuffer buffer_Dd;
    sf::SoundBuffer buffer_E;
    sf::SoundBuffer buffer_F;
    sf::SoundBuffer buffer_Fd;
    sf::SoundBuffer buffer_G;
    sf::SoundBuffer buffer_Gd;
    sf::SoundBuffer buffer_A;
    sf::SoundBuffer buffer_Ad;
    sf::SoundBuffer buffer_H;
    
    sf::Sound sound_C;
    sf::Sound sound_Cd;
    sf::Sound sound_D;
    sf::Sound sound_Dd;
    sf::Sound sound_E;
    sf::Sound sound_F;
    sf::Sound sound_Fd;
    sf::Sound sound_G;
    sf::Sound sound_Gd;
    sf::Sound sound_A;
    sf::Sound sound_Ad;
    sf::Sound sound_H;

    void buffer_update(sf::Event event) {
        if (event.type == sf::Event::KeyPressed) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                sound_C.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                sound_Cd.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                sound_D.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                sound_Dd.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                sound_E.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                sound_F.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                sound_Fd.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
                sound_G.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
                sound_Gd.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                sound_A.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                sound_Ad.play();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                sound_H.play();
            }
            
        }
    }
};