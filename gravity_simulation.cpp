#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

float metersToPixels(float x)
{ 
    // the scale of our world is : 100 pixels on the screen is equal to 1 meter in real life
    return x*100;
}

int main()
{ 
    // create a window first
    unsigned int x_size = 800;
    unsigned int y_size = 600;
    sf::RenderWindow window (sf::VideoMode({x_size,y_size}), "Physics Simulation");

    // create a circle
    float radius = 30;
    sf::CircleShape circle (radius);
    circle.setFillColor(sf::Color::Cyan);
    circle.setOrigin({radius,radius});
    circle.setPosition({x_size/2.f, radius+1});

    // boundary conditions of the circle to implement collision
    struct shapeBoundary
    { float left;
        float right;
        float top;
        float bottom;
    };

    struct shapeBoundary circle_boundary 
    { 
        radius,  
        x_size-radius,
        radius,
        y_size-radius,
    }; 

    // define the velocity and gravity in meters per second and meters per second squared respectively
    float gravity=metersToPixels(9.81);
    float velocity = metersToPixels(0); 

    // create a clock before the loop
    sf::Clock clock;


    bool sim_status=false;

    // outer loop to keep the application alive
    while (window.isOpen())
    { 
        // handle events
        while (const std::optional event = window.pollEvent())
        {

            if (event->is <sf::Event::Closed>())
                window.close();

            if (const auto* key = event-> getIf<sf::Event::KeyPressed>())
            { 
                if (key->code == sf::Keyboard::Key::W)
                { 
                    std::cout <<"Exiting the simulation";
                    window.close();
                }
                else if (key->code ==sf::Keyboard::Key::Enter)
                { 
                    sim_status=!sim_status;
                    if (sim_status) std::cout<<"Enter pressed ... resuming the simulation... \n";
                    else std::cout<<"Enter pressed ... pausing the simulation... \n";
                }
            }
        }

        // find the time elapsed since the last frame
        float time_elapsed = clock.restart().asSeconds();
        if (sim_status)
        {

            velocity += gravity * time_elapsed;

            sf::Vector2f position_check = circle.getPosition();

            float x_position = position_check.x + 0;
            float y_position = position_check.y + velocity * time_elapsed + 0.5f * gravity * time_elapsed * time_elapsed;

            if (y_position>circle_boundary.bottom)
                y_position=circle_boundary.bottom;
            else if (y_position<circle_boundary.top)
                y_position=circle_boundary.top;

            circle.setPosition({x_position,y_position});


            // COLLISION CHECK
            // if it hits the floor
            if (y_position >=circle_boundary.bottom && velocity>0)
                velocity = -0.8*velocity; 
            // if it hits the ceiling
            if (y_position <=circle_boundary.top && velocity<0)
                velocity = -0.8*velocity; 

        }

        // render the frame
        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display(); 
    }
}