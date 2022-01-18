#include "boids.h"
#include <chrono>
#include <random>
#include "drawbuffer.h"
#include <cmath>


#define COUNTOF(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

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

boids::boids(float _AttractForce, float _AttractRadius, float _RepulseForce, float _RepulseRadius, float _AligneForce, float _AlignRadius)
	: AttractForce(_AttractForce), AttractRadius(_AttractRadius), RepulseForce(_RepulseForce), RepulseRadius(_RepulseRadius), AlignForce(_AligneForce), AlignRadius(_AlignRadius)
{
	AddBoids(5);
}

boids::~boids()
{

}

std::vector<boids::boid> boids::GetBoids()
{
	return _boids;
}

void boids::Tick(float deltaTime)
{
	for (int x = 0; x < _boids.size(); x++)
	{

		boid boidLooked = { _boids[x].pos, _boids[x].forward };

		for (int y = 0; y < _boids.size(); y++)
		{
			if (x == y) continue;
			glm::vec2 force = { 0,0 };

			boid boidCompared = { _boids[y].pos, _boids[y].forward };
 
			if (abs(boidCompared.pos.x - boidLooked.pos.x) > 1)
			{
				boidCompared.pos.x += (boidCompared.pos.x > boidLooked.pos.x)?-2:2;
			}

			if (abs(boidCompared.pos.y - boidLooked.pos.y) > 1)
			{
				boidCompared.pos.y += (boidCompared.pos.y > boidLooked.pos.y) ? -2 : 2;
			}

			glm::vec2 difference = boidCompared.pos - boidLooked.pos;
			float distance = sqrtf(difference.x * difference.x + difference.y * difference.y);
			


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

			_boids[x].pos += force * deltaTime;

			if (_boids[x].pos.x > 1)
			{
				_boids[x].pos.x -= 2;
			}
			if (_boids[x].pos.y > 1)
			{
				_boids[x].pos.y -= 2;
			}

			if (_boids[x].pos.x < -1)
			{
				_boids[x].pos.x += 2;
			}
			if (_boids[x].pos.y < -1)
			{
				_boids[x].pos.y += 2;
			}

			glm::vec2 newForward = glm::normalize(force);

			if (!isnan(newForward.x))
			{
				_boids[y].forward = newForward;
			}
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

