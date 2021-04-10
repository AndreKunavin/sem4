#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vector2.hpp"

class Triangle {
public:
    Triangle(Vector2 v1, Vector2 v2, Vector2 v3) {
        v_1 = v1;
        v_2 = v2;
        v_3 = v3;
    };
    Vector2 v_1;
    Vector2 v_2;
    Vector2 v_3;
};

class Serpinskiy 
{
public:
    Serpinskiy(Triangle base_triangle, std::size_t fractal_depth) {  
        depth = fractal_depth;
        triangles.push_back(base_triangle);
    };
    void plot() {
        make_trio(depth, triangles[0], triangles);
    }
    std::vector<Triangle> triangles;
private:
    std::size_t depth;
    void make_trio(std::size_t n, Triangle base_triangle, std::vector<Triangle>& buffer) {
        std::vector<Triangle> trio;
        Vector2 nv_1 = (base_triangle.v_1 + base_triangle.v_3) / 2;
        Vector2 nv_2 = (base_triangle.v_1 + base_triangle.v_2) / 2;
        Vector2 nv_3 = (base_triangle.v_2 + base_triangle.v_3) / 2;
        buffer.push_back({nv_1, nv_2, nv_3});
        trio.push_back({base_triangle.v_1, nv_1, nv_2});
        trio.push_back({nv_2, base_triangle.v_2, nv_3});
        trio.push_back({nv_1, nv_3, base_triangle.v_3});
        n--;
        for (std::size_t i = 0; i < 3; i++) {
            if (n > 0) {
                make_trio(n, trio[i], buffer);
            }   
        }
    }
};

Triangle to_vector2(sf::CircleShape right_triangle) {
    sf::Vector2f v1 = right_triangle.getPoint(0); 
    sf::Vector2f v2 = right_triangle.getPoint(1);
    sf::Vector2f v3 = right_triangle.getPoint(2);
    Vector2 v_1 = {v1.x, v1.y};
    Vector2 v_2 = {v2.x, v2.y};
    Vector2 v_3 = {v3.x, v3.y};
    Triangle base_triangle = {v_1, v_2, v_3}; 
    return base_triangle;
}

void to_sfvec2(std::vector<Triangle>& triangles, sf::VertexArray& sf_triangles) {
    for (std::size_t i = 0; i < triangles.size() * 3; i += 3) {
        sf_triangles[i].position = {triangles[i/3].v_1.x, triangles[i/3].v_1.y};
        sf_triangles[i + 1].position = {triangles[i/3].v_2.x, triangles[i/3].v_2.y};
        sf_triangles[i + 2].position = {triangles[i/3].v_3.x, triangles[i/3].v_3.y};
        
        sf_triangles[i].color = sf::Color::Black;
        sf_triangles[i + 1].color = sf::Color::Black;
        sf_triangles[i + 2].color = sf::Color::Black;
    }
    sf_triangles[0].color = sf::Color::Red;
    sf_triangles[1].color = sf::Color::Green;
    sf_triangles[2].color = sf::Color::Yellow;
}


int main() {
    std::size_t depth;
    std::cout << "DEPTH: ";
    std::cin >> depth;
    sf::RenderWindow window(sf::VideoMode(1000, 800), "vertex");
    sf::CircleShape right_triangle(500.f, 3);
    Triangle base_triangle = to_vector2(right_triangle);
    Serpinskiy S(base_triangle, depth);
    S.plot();
    sf::VertexArray sf_triangles(sf::Triangles, S.triangles.size() * 3);
    to_sfvec2(S.triangles, sf_triangles);
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
        }
        window.clear();
        window.draw(sf_triangles);
        window.display();
    } 
        return 0;
}
