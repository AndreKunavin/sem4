#include <iostream>
#include "sintezator.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SINTEZATOR");
    
    Sintezator S;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                S.buffer_update(event);

            }
        }
        window.clear();
        window.display();
    }

    return 0;
}