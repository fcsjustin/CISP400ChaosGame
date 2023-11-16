#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>

using namespace sf;
using namespace std;

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int MENU_FONT_SIZE = 30;

enum FractalType {
    TRIANGLE,
    PENTAGON
};

// Function to calculate the midpoint
Vector2f calculateMidpoint(const Vector2f& p1, const Vector2f& p2)
{
    return (p1 + p2) / 2.0f;
}

// Recursive function for the triangle fractal
void generateTriangleFractal(vector<Vector2f>& points, const vector<Vector2f>& vertices, int depth)
{
    if (depth <= 0)
        return;

    int randomVertexIndex = rand() % vertices.size();
    Vector2f lastPoint = points.back();
    Vector2f selectedVertex = vertices[randomVertexIndex];
    Vector2f triangleMidpoint = calculateMidpoint(lastPoint, selectedVertex);

    points.push_back(triangleMidpoint);

    generateTriangleFractal(points, vertices, depth - 1);
}

//Menu for user selection
void drawMenu(RenderWindow& window, Font& font, FractalType& currentFractalType) 
{
    Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(MENU_FONT_SIZE);
    menuText.setFillColor(Color::Green);
    menuText.setPosition(10, 130);
    menuText.setString("Press 'T' for Triangle, 'P' for Pentagon");

    window.draw(menuText);

    if (currentFractalType == TRIANGLE) 
    {
        menuText.setString("Current Fractal: Triangle");
    }
    else 
    {
        menuText.setString("Current Fractal: Pentagon");
    }

    menuText.setPosition(10, 170);
    window.draw(menuText);
}

int main()
{
    // Create a video mode object
    VideoMode vm(1920, 1080);
    // Create and open a window for the game
    RenderWindow window(vm, "Chaos Game", Style::Default);

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

    FractalType currentFractalType = TRIANGLE;

    Vector2f triangleMidpoint;
    Vector2f pentagonMidpoint;

    int previousVertexIndex = -1;  // Initialize with an invalid index

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
                        if (vertices.size() < (currentFractalType == TRIANGLE ? 3 : 5)) 
                        {
                            Vector2f clickCoordinate(event.mouseButton.x, event.mouseButton.y);
                            vertices.push_back(clickCoordinate);
                            cout << "Vertex " << vertices.size() << " clicked at: (" << clickCoordinate.x << ", " << clickCoordinate.y << ")\n";

                            if (vertices.size() == (currentFractalType == TRIANGLE ? 3 : 5)) 
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
                            algorithmText3.setString("1 Click = 25 points (esc to end)");
                        }
                    }
                }
                else 
                {
                    for (int i = 0; i < 25; i++) 
                    {
                        int randomVertexIndex;
                        do 
                        {
                            randomVertexIndex = rand() % vertices.size();
                        } while (randomVertexIndex == previousVertexIndex);

                        Vector2f lastPoint = points.empty() ? startCoordinate : points.back();
                        Vector2f selectedVertex;

                        if (currentFractalType == TRIANGLE) 
                        {
                            selectedVertex = vertices[randomVertexIndex];
                            generateTriangleFractal(points, { lastPoint, selectedVertex }, 1);
                        }
                        else 
                        {
                            selectedVertex = vertices[randomVertexIndex];
                            pentagonMidpoint = (1.25f * lastPoint + selectedVertex) / 3.0f;  //Distance factor
                        }

                        points.push_back((currentFractalType == TRIANGLE) ? triangleMidpoint : pentagonMidpoint);

                        previousVertexIndex = randomVertexIndex;
                    }
                }
            }
            if (event.type == Event::KeyPressed) 
            {
                if (event.key.code == Keyboard::Escape) 
                {
                    window.close();
                }
                else if (event.key.code == Keyboard::T) 
                {
                    currentFractalType = TRIANGLE;
                    vertices.clear();
                    points.clear();
                    algorithmStarted = false;
                    instructionText.setString("Click on any three points to create the vertices for the triangle.");
                }
                else if (event.key.code == Keyboard::P) 
                {
                    currentFractalType = PENTAGON;
                    vertices.clear();
                    points.clear();
                    algorithmStarted = false;
                    instructionText.setString("Click on any FIVE points to create the vertices for the PENTAGON.");
                }
            }
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

        drawMenu(window, font, currentFractalType);

        window.display();
    }

    return 0;
}
