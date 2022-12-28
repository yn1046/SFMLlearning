#pragma once
#include "VerletSolver.hpp"

class Renderer {
public:
	explicit Renderer(sf::RenderTarget& target)
		: m_target{ target } { }


	void render(const VerletSolver& solver) const {
		const sf::Vector3f constraint = solver.getConstraint();
		sf::CircleShape constraint_background{ constraint.z };
		constraint_background.setOrigin(constraint.z, constraint.z);
		constraint_background.setFillColor(sf::Color::Black);
		constraint_background.setPosition(constraint.x, constraint.y);
		constraint_background.setPointCount(128);
		m_target.draw(constraint_background);

		// Render objects
		sf::CircleShape circle{ 1.0f };
		circle.setPointCount(32);
		circle.setOrigin(1.0f, 1.0f);
		const auto& objects = solver.getObjects();
		for (const auto& obj : objects) {
			circle.setOrigin(obj.radius, obj.radius);
			circle.setPosition(obj.position);
			circle.setRadius(obj.radius);
			circle.setFillColor(obj.color);
			m_target.draw(circle);
		}

	}

private:
	sf::RenderTarget& m_target;
};