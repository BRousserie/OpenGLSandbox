#pragma once

#include <glm/glm.hpp>
#include <vector>

class boids 
{
public:

	struct boid
	{
		glm::vec2 pos;
		glm::vec2 forward;
	};

	boids(float _AttractForce = 0.1f, float _AttractRadius = 2.f, float _RepulseForce = 0.15f, float _RepulseRadius = 0.5f, float _AligneForce = 0.5f, float _AlignRadius = 0.7f);
	~boids();

	boids(const boids& ) = delete;
	boids& operator=(boids) = delete;

	std::vector<boid> GetBoids();
	void Tick(float deltaTime);
	void AddBoids(unsigned int amount);
	void RemoveBoids(unsigned int amount);

private:
	std::vector<boid> _boids;
	float AttractForce;
	float AttractRadius;
	float RepulseForce;
	float RepulseRadius;
	float AlignForce;
	float AlignRadius;
};