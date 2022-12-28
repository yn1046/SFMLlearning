#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <format>
#include <random>
#include <ctime>
#include <chrono>

#include "VerletSolver.hpp"
#include "Renderer.hpp"

using namespace sf;
using scl = std::chrono::steady_clock;


int main()
{
	// Create window
	constexpr int32_t window_width = 1000;
	constexpr int32_t window_height = 1000;
	RenderWindow window(
		VideoMode(window_width, window_height),
		"My first simulation!!",
		Style::Titlebar | Style::Close
	);
	Event ev;

	Text text;

	// Verlet stuff
	Renderer renderer{ window };
	VerletSolver solver;
	solver.setConstraint({
		static_cast<float>(window_width) * 0.5f,
		static_cast<float>(window_height) * 0.5f
		}, 450.0f);
	Clock cl;

	// Game loop
	while (window.isOpen()) {
		// Event polling
		while (window.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (ev.key.code == Keyboard::Escape)
					window.close();
				break;
			case Event::MouseButtonPressed:
				auto [button, x, y] = ev.mouseButton;
				if (ev.key.code == 0) {
					solver.addObject({ (float)x, (float)y }, 15.0f, Color(rand() % 255, rand() % 255, rand() % 255));
				}
			}
		}

		// Update
		float dt = cl.getElapsedTime().asMicroseconds() / 1000000.f;
		solver.update(dt);
		cl.restart();

		// Render
		window.clear(sf::Color::White); // Clear old frame

		// Draw here!
		auto [x, y] = Mouse::getPosition();
		text.setString(std::format("Mouse: ({}, {})", x, y));
		Font deffont;
		deffont.loadFromFile("8bo.ttf");
		text.setFont(deffont);
		text.setCharacterSize(12);
		window.draw(text);
		renderer.render(solver);

		window.display(); // The window's done drawing
	}




	return 0;
}
