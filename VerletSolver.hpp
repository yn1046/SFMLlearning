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
		color{ color }
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
		checkCollisions(dt);
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

	void checkCollisions(float dt)
	{
		const float    response_coef = 0.75f;
		const uint64_t objects_count = m_objects.size();
		// Iterate on all objects
		for (uint64_t i{ 0 }; i < objects_count; ++i) {
			VerletObject& object_1 = m_objects[i];
			// Iterate on object involved in new collision pairs
			for (uint64_t k{ i + 1 }; k < objects_count; ++k) {
				VerletObject& object_2 = m_objects[k];
				const sf::Vector2f v = object_1.position - object_2.position;
				const float        dist2 = v.x * v.x + v.y * v.y;
				const float        min_dist = object_1.radius + object_2.radius;
				// Check overlapping
				if (dist2 < min_dist * min_dist) {
					const float        dist = sqrt(dist2);
					const sf::Vector2f n = v / dist;
					const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
					const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
					const float delta = 0.5f * response_coef * (dist - min_dist);
					// Update positions
					object_1.position -= n * (mass_ratio_2 * delta);
					object_2.position += n * (mass_ratio_1 * delta);
				}
			}
		}
	}

	void apply_constraint() {
		for (auto& obj : m_objects) {
			const vec2 v = m_constraint_center - obj.position;
			const float        dist = sqrt(v.x * v.x + v.y * v.y);
			if (dist > (m_constraint_radius - obj.radius)) {
				const sf::Vector2f n = v / dist;
				obj.position = m_constraint_center - n * (m_constraint_radius - obj.radius);
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
	vec2 m_gravity = { 0.0f, 1000.0f };
	vec2 m_constraint_center;
	float m_constraint_radius = 100.f;

	float vec2_length(vec2 v) {
		return sqrt(v.x * v.x + v.y * v.y);
	}
};