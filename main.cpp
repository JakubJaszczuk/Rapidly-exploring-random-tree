#include "ArrayRRT.h"
#include "includeGlew.hpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "shader.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "RenderArrayRRT.h"

int main()
{
	RenderArrayRRT rrt(ArrayRRT(0.05, true));
	while(rrt.size() < 10000)
	{
		rrt.addPoint();
	}
	// Ustaw okno
	int w = 512;
	int h = 512;
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.attributeFlags = settings.Core;
	sf::Window window(sf::VideoMode(w, h), "Szybkorosnące losowe drzewo", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	rrt.initBuffers();
	// Ustawienia widoku
	float scale = 0.5f;
	float transX = 0.5f;
	float transY = 0.5f;
	bool dragView = false;
	sf::Vector2i previousPosition;
	glm::mat4 view = glm::ortho(transX - scale, transX + scale, transY - scale, transY + scale);
	// Koniec widoku
	std::size_t from{0}, to{0};
	bool running = true;
	while(running)
	{
		// handle events
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				// end the program
				running = false;
			}
			else if(event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
				w =  event.size.width;
				h =  event.size.height;
			}
			else if(event.type == sf::Event::MouseWheelScrolled)
			{
				if(event.mouseWheelScroll.delta > 0)
				{
					scale *= 0.8;
				}
				else if(event.mouseWheelScroll.delta < 0)
				{
					scale /= 0.8;
				}
				view = glm::ortho(transX-scale, transX+scale, transY-scale, transY+scale);
			}
			else if(event.type == sf::Event::MouseButtonPressed)
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					dragView = true;
					previousPosition = sf::Mouse::getPosition(window);
				}
			}
			else if(event.type == sf::Event::MouseButtonReleased)
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					dragView = false;
				}
			}
			else if(event.type == sf::Event::MouseMoved && dragView)
			{
				
				transX -= scale * 0.003 * (event.mouseMove.x - previousPosition.x);
				transY += scale * 0.003 * (event.mouseMove.y - previousPosition.y);				
				view = glm::ortho(transX-scale, transX+scale, transY-scale, transY+scale);
				previousPosition.x = event.mouseMove.x;
				previousPosition.y = event.mouseMove.y;
			}
			else if(event.type == sf::Event::KeyPressed)
			{
				// Obliczanie współrzędnych kliknięcia
				const auto mousePosition = sf::Mouse::getPosition(window);
				const auto x = mousePosition.x / static_cast<float>(w/2) - 1;
				const auto y = (h-mousePosition.y) / static_cast<float>(h/2) - 1;
				const glm::vec2 position = glm::inverse(view) * glm::vec4(x, y, 0, 1);
				// Wybranie punktu
				auto closest = rrt.rrt.findClosestPoint(position);
				if(glm::distance(rrt.rrt.points[closest], position) < 0.01)
				{
					if(event.key.code == sf::Keyboard::Q)
					{
						from = closest;
					}
					else if(event.key.code == sf::Keyboard::W)
					{
						to = closest;
					}
				rrt.updatePathBuffer(rrt.rrt.pathBetweenPoints(from, to));
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		rrt.draw(glm::value_ptr(view));

		window.display();
	}
	return 0;
}
