#pragma once

#include <glm/glm.hpp>
#include <vector>

class boids 
{
public:

	struct boid
	{
		glm::vec2 pos;
		glm::vec2 velocity;
		glm::vec2 force;
		glm::vec4 color;
	};

	boids(float _AttractForce = 0.f, float _AttractRadius = 0.f, float _RepulseForce = 0.f, float _RepulseRadius = 0.f, float _AligneForce = 0.f, float _AlignRadius = 0.f, int _BoidsNumber = 0);
	~boids();

	boids(const boids& ) = delete;
	boids& operator=(boids) = delete;

	std::vector<boid> GetBoids();
	void Tick(float deltaTime);
	void AddBoids(unsigned int amount);
	void RemoveBoids(unsigned int amount);
	void UpdatePosition(float deltaTime);

	std::vector<boid> _boids;
	float AttractForce;
	float AttractRadius;
	float RepulseForce;
	float RepulseRadius;
	float AlignForce;
	float AlignRadius;
	float SpeedMax = 1;
	int TargetBoidsNumber;
};