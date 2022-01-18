#pragma once
#define _USE_MATH_DEFINES
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

class particlesystem
{
public:
	struct system
	{
		glm::vec2 pos;
	};


	particlesystem(const particlesystem&) = delete;
	particlesystem& operator=(particlesystem) = delete;

	particlesystem();
	~particlesystem();

	std::vector<system> GetSystems();
	void Tick(float deltaTime, boids& _boids);
	void AddSystem(glm::vec2 pos);

	std::vector<system> _systems;
	float Force = 1.f;
	float Radius = 2.f;
	float Angle = 0.f;
};