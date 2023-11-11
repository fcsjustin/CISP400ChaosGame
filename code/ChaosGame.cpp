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

    Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        cerr << "Error loading font." << endl;
        return -1;
    }

    Text instructionText;
    instructionText.setFont(font);
    instructionText.setCharacterSize(30);
    instructionText.setFillColor(Color::White);
    instructionText.setPosition(10, 10);
    instructionText.setString("Click on any three points to create the vertices for the triangle.");

    Text algorithmText1;
    algorithmText1.setFont(font);
    algorithmText1.setCharacterSize(30);
    algorithmText1.setFillColor(Color::Blue);
    algorithmText1.setPosition(10, 50);

    Text algorithmText2;
    algorithmText2.setFont(font);
    algorithmText2.setCharacterSize(30);
    algorithmText2.setFillColor(Color::Magenta);
    algorithmText2.setPosition(10, 90);
   
    Text algorithmText3;
    algorithmText3.setFont(font);
    algorithmText3.setCharacterSize(40);
    algorithmText3.setFillColor(Color::Cyan);
    algorithmText3.setPosition(10, 300);

    bool algorithmStarted = false;
    Vector2f startCoordinate;

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
                if (!algorithmStarted)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        if (vertices.size() < 3)
                        {
                            Vector2f clickCoordinate(event.mouseButton.x, event.mouseButton.y);
                            vertices.push_back(clickCoordinate);
                            cout << "Vertex " << vertices.size() << " clicked at: (" << clickCoordinate.x << ", " << clickCoordinate.y << ")\n";

                            if (vertices.size() == 3)
                            {
                                startCoordinate = clickCoordinate;
                                algorithmText1.setString("Choose your starting point to begin the algorithm.");
                            }
                        }
                        else
                        {
                            algorithmStarted = true;
                            points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                            
                            algorithmText2.setString("Algorithm started. Click to generate points.");
                            algorithmText3.setString("1 Click = 25 points (ctrl to end)");
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < 100; i++)
                    {
                        int randomVertexIndex = rand() % vertices.size();
                        Vector2f lastPoint = points.empty() ? startCoordinate : points.back();
                        Vector2f selectedVertex = vertices[randomVertexIndex];
                        Vector2f midpoint = (lastPoint + selectedVertex) / 2.0f;
                        points.push_back(midpoint);
                    }
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        window.clear();

        for (size_t i = 0; i < vertices.size(); i++)
        {
            CircleShape vertex(5);
            vertex.setPosition(vertices[i]);
            vertex.setFillColor(Color::Blue);
            window.draw(vertex);
        }

        for (size_t i = 0; i < points.size(); i++)
        {
            CircleShape point(2);
            point.setPosition(points[i]);
            point.setFillColor(Color::Red);
            window.draw(point);
        }

        window.draw(instructionText);
        window.draw(algorithmText1);
        window.draw(algorithmText2);
        window.draw(algorithmText3);
        window.display();
    }

    return 0;
}
