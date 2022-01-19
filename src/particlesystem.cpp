#include "particlesystem.h"
#include "boids.h"

particlesystem::particlesystem()
{
	_systems.reserve(100);
}

particlesystem::~particlesystem()
{
}

std::vector<particlesystem::system> particlesystem::GetSystems()
{
	return _systems;
}

std::vector<object*> particlesystem::GetSystemsPtr()
{
	return _systems_ptr;
}

void particlesystem::Tick(float deltaTime, boids& _boids)
{
	float Rad = Angle * M_PI / 180;

	
	for (boids::boid& boid : _boids.GetBoids())
	{
		boid.force = { 0,0 };
		for (const object& system : _systems)
		{
			glm::vec2 dir = system.pos - boid.pos;
			float length = glm::length(dir);
			dir = glm::normalize(dir);
			if (length < Radius)
			{
				glm::vec2 rotatedVector = 
				{
					cos(Rad) * dir.x - sin(Rad) * dir.y, 
					sin(Rad) * dir.x + cos(Rad) * dir.y 
				};
				boid.velocity *= 0.99;
				boid.force += rotatedVector * Force * (Radius - length) / Radius;
			}
		}
	}
}

void particlesystem::AddSystem(glm::vec2 pos)
{
	if (_systems.size() < 100)
	{
		_systems.push_back({ pos, { 1,1,1,1 } });
		_systems_ptr.push_back(&_systems.back());
	}
}
