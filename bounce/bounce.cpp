#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Vector2.hpp"

#define X 1000
#define Y 800
#define G 400               // accelleration
#define R1 50.f
#define M1 500.f
#define R2 20.f              // radius 2
#define N 6                 // number of small balls (2) added by one click  
#define M2 100.f             // mass 2
#define LOSSES_FACTOR 0.6   // 0 <= LOSSES_FACTOR <= 1 

struct Particle {
    Particle() {};
    Particle(Vector2 pos_, Vector2 vel_, float radius, float mass, std::size_t type) {
        pos = pos_;
        vel = vel_;
        r = radius;
        m = mass;
        t = type;
    }
    std::size_t t;
    Vector2 pos;
    Vector2 vel;
    float r;
    float m;
};

float dot(Vector2 a, Vector2 b) {
    return (a.x * b.x + a.y * b.y);
}

class World {
public:
    std::vector<Particle> particles;
    void add(Particle ball) {
        particles.push_back(ball);
    }
    void update(float dt) {
        for (auto& it: particles) {

            it.vel.y = it.vel.y + G * dt;
            it.pos.y = it.pos.y + G * dt * dt / 2;

            if ((it.pos.x <= 0) && (it.vel.x < 0)) { 
                it.vel.x = it.vel.x * (-1);
                it.pos.x = 0;
            } else if ((it.pos.x >= (X - 2 * it.r)) && (it.vel.x > 0)) {
                it.vel.x = it.vel.x * (-1);
                it.pos.x = X - 2 * it.r;
            } else if ((it.pos.y >= (Y - 2 * it.r)) && (it.vel.y > 0)) {
                it.vel.y = it.vel.y * (-1) * LOSSES_FACTOR;
                it.pos.y = Y - 2 * it.r;
            }
            
            for (auto& that: particles) {
                Vector2 e(1.f, 1.f);
                Vector2 d = (that.pos + e * that.r) - (it.pos + e * it.r);
                Vector2 v = that.vel - it.vel;

                if ((d.len() <= (it.r + that.r)) && (d.len() != 0) && (dot(d, v) <= 0)) {
                    Vector2 n = d / d.len();
                    Vector2 p = n * dot(v * (2 / (1 / it.m + 1 / that.m)), n);
                    float dl = it.r + that.r - d.len();

                    that.vel = that.vel - p / that.m;
                    it.vel = it.vel + p / it.m;
                    that.pos = that.pos + n * dl / 2;
                    it.pos = it.pos - n * dl / 2;
                }
            }
            it.pos = it.pos + it.vel * dt;
        }
    }
    std::size_t count() {
        return particles.size();
    }
};

sf::Color color_and_velocity(float vel_abs, std::size_t type) {
    sf::Uint16 cf = vel_abs / 2; 
    sf::Color color; 
    if (cf > 255) {
        cf = 255;
    }
    if (type == 1) {
        color.g = cf;
        color.r = (color.g + 255) / 2;
        color.b = 0;
    } else {
        color.g = 0;
        color.r = cf;
        color.b = 128; 
    }
    return color;
}

void change_type(std::size_t& type) {
    if (type == 0) {
        type = 1;
    } else {
        type = 0;
    }
}

class Mouse {
public:
    Mouse(float radius1, float mass1, float radius2, float mass2) {
        first.r = radius1;
        first.m = mass1;
        first.t = 0;
        second.r = radius2;
        second.m = mass2;
        second.t = 1;
    };
    sf::CircleShape phantom;
    sf::Vector2f particle_pos;
    sf::Vector2f cursor_pos;
    std::size_t ball_type = 1;

    void tracking(World& W, sf::RenderWindow& window) {
        sf::Vector2f e(1.f, 1.f);
        ball.push_back(first);
        ball.push_back(second);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            if (mode_r == 0) {
                change_type(ball_type);
                mode_r = 1;
            }
        } else {
            mode_r = 0;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i m_particle_pos;
            sf::Vector2i m_cursor_pos;
            if (mode == 0) {
                m_particle_pos = sf::Mouse::getPosition(window);
                particle_pos = window.mapPixelToCoords(m_particle_pos);
                mode = 1;
            } else {
                m_cursor_pos = sf::Mouse::getPosition(window);
                cursor_pos = window.mapPixelToCoords(m_cursor_pos);
                sf::Vertex line[] =
                {
                    sf::Vertex(particle_pos),
                    sf::Vertex(cursor_pos)
                };
                window.draw(line, 2, sf::Lines);
            }
            phantom.setRadius(ball[ball_type].r);
            phantom.setPosition(particle_pos - e * ball[ball_type].r);
            ball[ball_type].vel.x = 3.f * (cursor_pos.x - particle_pos.x);
            ball[ball_type].vel.y = 3.f * (cursor_pos.y - particle_pos.y);
            sf::Color color = color_and_velocity(ball[ball_type].vel.len(), ball_type);
            phantom.setFillColor(color);
            window.draw(phantom);
        } else {
            if (mode == 1) {
                ball[ball_type].pos.x = particle_pos.x - ball[ball_type].r;
                ball[ball_type].pos.y = particle_pos.y - ball[ball_type].r;            
                for(size_t i = 0; i <= ball_type * (N - 1); i++) {
                    W.add(ball[ball_type]);
                }     
                mode = 0;
            }
        }
        
    }
private:
    std::size_t mode_r = 0;
    std::size_t mode = 0;
    Particle first;
    Particle second;
    std::vector <Particle> ball;
};

void draw(World& W, sf::RenderWindow& window) {
    std::vector<sf::CircleShape> balls(W.count());
    std::vector<sf::Vector2f> new_pos(W.count());
    for (std::size_t i = 0; i < W.count(); i++) {
        new_pos[i].x = W.particles[i].pos.x;
        new_pos[i].y = W.particles[i].pos.y;
        balls[i].setPosition(new_pos[i]);
        balls[i].setRadius(W.particles[i].r);

        sf::Color color = color_and_velocity(W.particles[i].vel.len(), W.particles[i].t);
        balls[i].setFillColor(color);

        window.draw(balls[i]);
    }
}

int main() {

    sf::RenderWindow window(sf::VideoMode(X, Y), "Bounce");
    window.setFramerateLimit(60);
    
    sf::Texture texture;
    if (!texture.loadFromFile("gradient")) {
        std::cout << "Can not load a texture" << std::endl;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

    World W;
    Mouse mouse(R1, M1, R2, M2);

    sf::Clock clock;
    sf::Time prev_time;
    while(window.isOpen()) {

        sf::Event event;
        sf::Time time = clock.getElapsedTime();
        float dt = time.asSeconds() - prev_time.asSeconds();
        prev_time = time;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                
            }
        }
        window.clear();
        window.draw(sprite);
        mouse.tracking(W, window);
        W.update(dt);
        draw(W, window);
        window.display();
    }
}
