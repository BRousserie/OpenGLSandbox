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

	boids(float _AttractForce = 10, float _AttractRadius = 20, float _RepulseForce = 15, float _RepulseRadius = 3, float _AligneForce = 5, float _AlignRadius = 7);
	~boids();

	boids(const& boids) = delete;
	boids& operator=(boids) = delete;

	std::vector<boid> GetBoids();
	void Tick();
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