#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Vector2.hpp"

#define X 1000
#define Y 800
#define G 400               // accelleration
#define R 20.f              // radius
#define N 5                 // number of balls added by one click  
#define M 100.f             // mass
#define LOSSES_FACTOR 0.6   // 0 <= LOSSES_FACTOR <= 1 

struct Particle {
    Particle() {};
    Particle(Vector2 pos_, Vector2 vel_, float radius, float mass) {
        pos = pos_;
        vel = vel_;
        r = radius;
        m = mass;
    }
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

sf::Color color_and_velocity(float vel_abs) {
    sf::Uint16 cf = vel_abs / 2; 
    sf::Color color; 
    if (cf > 255) {
        cf = 255;
    }
    color.g = cf;
    color.r = (color.g + 255) / 2;
    color.b = 0;
    return color;
}

class Mouse {
public:
    Mouse(float radius, float mass) {
        ball.r = radius;
        ball.m = mass;
        phantom.setRadius(radius);
    };
    sf::CircleShape phantom;
    sf::Vector2f particle_pos;
    sf::Vector2f cursor_pos;

    void tracking(World& W, sf::RenderWindow& window) {
        sf::Vector2f e(1.f, 1.f);
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
            phantom.setPosition(particle_pos - e * ball.r);
            ball.vel.x = 3.f * (cursor_pos.x - particle_pos.x);
            ball.vel.y = 3.f * (cursor_pos.y - particle_pos.y);
            
            sf::Color color = color_and_velocity(ball.vel.len());
            phantom.setFillColor(color);
            
            window.draw(phantom);
        } else {
            if (mode == 1) {
                ball.pos.x = particle_pos.x - ball.r;
                ball.pos.y = particle_pos.y - ball.r;            
                for(size_t i = 0; i < N; i++) {
                    W.add(ball);
                }
                    
                mode = 0;
            }
        }
        
    }
private:
    std::size_t mode = 0;
    Particle ball;
};

void draw(World& W, sf::RenderWindow& window) {
    std::vector<sf::CircleShape> balls(W.count());
    std::vector<sf::Vector2f> new_pos(W.count());
    for (std::size_t i = 0; i < W.count(); i++) {
        new_pos[i].x = W.particles[i].pos.x;
        new_pos[i].y = W.particles[i].pos.y;
        balls[i].setPosition(new_pos[i]);
        balls[i].setRadius(W.particles[i].r);

        sf::Color color = color_and_velocity(W.particles[i].vel.len());
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
    Mouse mouse(R, M);

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
