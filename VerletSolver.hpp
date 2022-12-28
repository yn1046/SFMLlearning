#pragma once
#include <SFML/Graphics.hpp>

using vec2 = sf::Vector2f;

struct VerletObject {
	vec2 position;
	vec2 position_previous;
	vec2 acceleration;
	float radius = 10.0f;
	sf::Color color = sf::Color::White;

	VerletObject() = default;
	VerletObject(sf::Vector2f position, float radius) :
		position{ position },
		position_previous{ position },
		acceleration{ 0.0f, 0.0f },
		radius{ radius }
	{}
	VerletObject(sf::Vector2f position, float radius, sf::Color color) :
		position{ position },
		position_previous{ position },
		acceleration{ 0.0f, 0.0f },
		radius{ radius },
		color{color}
	{}

	void update(float dt) {
		// How much it moved
		const vec2 displacement = position - position_previous;
		// Save current position
		position_previous = position;
		// Perform Verlet integration
		position = position + displacement + acceleration * dt * dt;
		// Reset acceleration
		acceleration = {};
	}

	void accelerate(vec2 a) {
		acceleration += a;
	}

	void set_velocity(vec2 v, float dt) {
		position_previous = position - (v * dt);
	}

	void add_velocity(vec2 v, float dt) {
		position_previous -= v * dt;
	}
};

struct VerletSolver {

	VerletObject& addObject(sf::Vector2f position, float radius)
	{
		return m_objects.emplace_back(position, radius);
	}
	VerletObject& addObject(sf::Vector2f position, float radius, sf::Color color)
	{
		return m_objects.emplace_back(position, radius, color);
	}

	void update(float dt) {
		apply_gravity();
		apply_constraint();
		update_positions(dt);
	}

	void update_positions(float dt) {
		for (auto& obj : m_objects) {
			obj.update(dt);
		}
	}

	void apply_gravity() {
		for (auto& obj : m_objects) {
			obj.accelerate(m_gravity);
		}
	}

	void apply_constraint() {
		for (auto& obj : m_objects) {
			const vec2 to_obj = obj.position - m_constraint_center;
			const float dist = to_obj.x * to_obj.x + to_obj.y * to_obj.y;

			// 50 is the default
			if (dist > m_constraint_radius - obj.radius) {
				const vec2 n = to_obj / dist;
				obj.position = m_constraint_center + n * (dist - obj.radius);
			}
		}
	}


	void setConstraint(sf::Vector2f position, float radius)
	{
		m_constraint_center = position;
		m_constraint_radius = radius;
	}

	[[nodiscard]]
	const std::vector<VerletObject>& getObjects() const
	{
		return m_objects;
	}

	[[nodiscard]]
	sf::Vector3f getConstraint() const
	{
		return { m_constraint_center.x, m_constraint_center.y, m_constraint_radius };
	}

private:
	std::vector<VerletObject> m_objects;
	sf::Clock m_clock;
	vec2 m_gravity = { 0.0f, 1.0f };
	vec2 m_constraint_center;
	float m_constraint_radius = 100.f;
};