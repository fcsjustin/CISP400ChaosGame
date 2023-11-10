#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>

using namespace sf;
using namespace std;

int main()
{
    // Create a video mode object
    VideoMode vm(1920, 1080);
    // Create and open a window for the game
    RenderWindow window(vm, "Triangle Fractal", Style::Default);

    vector<Vector2f> vertices;
    vector<Vector2f> points;
    srand(static_cast<unsigned>(time(NULL)));

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    cout << "the left button was pressed" << endl;
                    cout << "mouse x: " << event.mouseButton.x << endl;
                    cout << "mouse y: " << event.mouseButton.y << endl;

                    if (vertices.size() < 3)
                    {
                        vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                    else if (points.empty())
                    {
                        // Fourth click, push back to points vector
                        points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (points.size() > 0)
        {
            // Generate more points within the triangle
            int randomVertexIndex = rand() % 3; // Randomly select one of the three vertices
            Vector2f lastPoint = points.back();
            Vector2f selectedVertex = vertices[randomVertexIndex];
            Vector2f midpoint = (lastPoint + selectedVertex) / 2.0f;
            points.push_back(midpoint);
        }

        window.clear();

        for (size_t i = 0; i < vertices.size(); i++)
        {
            RectangleShape rect(Vector2f(10, 10));
            rect.setPosition(vertices[i]);
            rect.setFillColor(Color::Blue);
            window.draw(rect);
        }

        for (size_t i = 0; i < points.size(); i++)
        {
            CircleShape circle(5);
            circle.setPosition(points[i]);
            circle.setFillColor(Color::Red);
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}
