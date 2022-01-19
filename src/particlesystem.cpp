#include "particlesystem.h"
#include "boids.h"

particlesystem::particlesystem()
{
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

	for (object system : _systems)
	{
		for (object boid : _boids.GetBoids())
		{
			glm::vec2 dir = system.pos - boid.pos;
			if (dir.length() < Radius)
			{
				glm::vec2 rotatedVector = 
				{
					cos(Rad * dir.x) - sin(Rad * dir.y), 
					sin(Rad * dir.x) + cos(Rad * dir.y) 
				};
				boid.pos += rotatedVector * Force * deltaTime;
			}
		}
	}
}

void particlesystem::AddSystem(glm::vec2 pos)
{
	_systems.push_back({ pos, { 1,0,0,1 } });
	_systems_ptr.push_back(&_systems.back());
}
