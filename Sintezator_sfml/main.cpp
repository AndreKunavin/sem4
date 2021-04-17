#include <iostream>
#include "sintezator.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SINTEZATOR");

    Sintezator S;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                S.process(event);
            }
        }
        S.update_tex();
        window.clear();
        window.draw(S.sprite);
        window.display();
    }

    return 0;
}