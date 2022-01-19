#pragma once
#define _USE_MATH_DEFINES
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include "boids.h"
#include "object.h"

class particlesystem
{
public:
	struct system : public object
	{
		system(glm::vec2 _pos, glm::vec4 _color)
			: object({ _pos, _color }) {}
	};


	particlesystem(const particlesystem&) = delete;
	particlesystem& operator=(particlesystem) = delete;

	particlesystem();
	~particlesystem();

	std::vector<system> GetSystems();
	std::vector<object*> GetSystemsPtr();
	void Tick(float deltaTime, boids& _boids);
	void AddSystem(glm::vec2 pos);

	std::vector<system> _systems;
	std::vector<object*> _systems_ptr;
	float Force = 1.f;
	float Radius = 2.f;
	float Angle = 0.f;
};