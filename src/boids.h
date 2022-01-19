#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <list>
#include "object.h"

class boids 
{
public:

	struct boid : public object
	{
		boid (glm::vec2 _pos, glm::vec4 _color, glm::vec2 _velocity, glm::vec2 _force)
			: object({ _pos, _color }), velocity(_velocity), force(_force) {}

		boid(const boid& other)
		{
			pos = other.pos;
			color = other.color;
			velocity = other.velocity;
			force = other.force;
		}

		glm::vec2 velocity;
		glm::vec2 force;
	};

	boids(float _AttractForce = 0.f, float _AttractRadius = 0.f, float _RepulseForce = 0.f, float _RepulseRadius = 0.f, float _AligneForce = 0.f, float _AlignRadius = 0.f, int _BoidsNumber = 0);
	~boids();

	boids(const boids& ) = delete;
	boids& operator=(boids) = delete;

	std::list<boid>& GetBoids();
	std::vector<object*> GetBoidsPtr();
	void Tick(float deltaTime);
	void AddBoids(unsigned int amount);
	void AddBoidAt(glm::vec2 pos);
	void RemoveBoids(unsigned int amount);
	void UpdatePosition(float deltaTime);

	std::list<boid> _boids;
	std::vector<object*> _boids_ptr;
	float AttractForce;
	float AttractRadius;
	float RepulseForce;
	float RepulseRadius;
	float AlignForce;
	float AlignRadius;
	float SpeedMax = 1;
	int TargetBoidsNumber;
};