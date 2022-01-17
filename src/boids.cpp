#include "boids.h"
#include <chrono>
#include <random>
#include "drawbuffer.h"


#define COUNTOF(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

boids::boids(float _AttractForce, float _AttractRadius, float _RepulseForce, float _RepulseRadius, float _AligneForce, float _AlignRadius)
	: AttractForce(_AttractForce), AttractRadius(_AttractRadius), RepulseForce(_RepulseForce), RepulseRadius(_RepulseRadius), AlignForce(_AligneForce), AlignRadius(_AlignRadius)
{
}

boids::~boids()
{

}

std::vector<boids::boid> boids::GetBoids()
{
	return _boids;
}

void boids::Tick()
{
	for (int x; x < _boids.size(); x++)
	{
		for (int y; y < _boids.size(); x++)
		{
			if (x == y) continue;
			glm::vec2 force = { 0,0 };
			glm::vec2 difference = _boids[y].pos - _boids[x].pos;
			float distance = difference.length();

			if (distance < AttractRadius)
			{
				force += difference * AttractForce;
			}

			if (distance < RepulseRadius)
			{
				force -= difference * RepulseForce;
			}

			if (distance < AlignRadius)
			{
				force += _boids[y].forward * AlignForce;
			}

			_boids[x].pos += force;
			_boids[y].forward = glm::normalize(force);
		}
	}
}

void boids::AddBoids(unsigned int amount)
{
	for (int i = 0; i < amount; i++)
	{
		_boids.push_back({
					{RandomNormalize(),RandomNormalize()},
					glm::normalize(glm::vec2{RandomNormalize(),RandomNormalize()})
			});
	}
}

void boids::RemoveBoids(unsigned int amount)
{
	for (int i = 0; i < amount; i++)
	{
		if (_boids.size() == 0) return;
		_boids.pop_back();
	}
}


float RandomNormalize()
{
	std::mt19937_64 rng;
	// initialize the random number generator with time-dependent seed
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	// initialize a uniform distribution between 0 and 1
	std::uniform_real_distribution<double> unif(0, 1);
	return unif(rng);
}

