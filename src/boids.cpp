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

float RandomZeroOrOne()
{
	std::mt19937_64 rng;
	// initialize the random number generator with time-dependent seed
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	// initialize a uniform distribution between 0 and 1
	std::uniform_int_distribution<> distrib(0, 1);
	return distrib(rng);
}

boids::boids(float _AttractForce, float _AttractRadius, float _RepulseForce, float _RepulseRadius, float _AligneForce, float _AlignRadius, int _BoidsNumber)
	: AttractForce(_AttractForce), AttractRadius(_AttractRadius), RepulseForce(_RepulseForce), RepulseRadius(_RepulseRadius), AlignForce(_AligneForce), AlignRadius(_AlignRadius), TargetBoidsNumber(_BoidsNumber)
{
	AddBoids(_BoidsNumber);
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
	//Update boids count
	int BoidsChange = TargetBoidsNumber - int(_boids.size());
	if (BoidsChange != 0)
	{
		if (BoidsChange < 0)
		{
			RemoveBoids(-BoidsChange);
		}
		else
		{
			AddBoids(BoidsChange);
		}
	}


	//Calculate forces
	for (int x = 0; x < _boids.size(); x++)
	{
		boid boidLooked = { _boids[x].pos, _boids[x].velocity };

		glm::vec2 attractionForce = { 0,0 };
		glm::vec2 attractionPosition = { 0,0 };
		int attractionCount = 0;

		glm::vec2 repulseForce = { 0,0 };
		int repulseCount = 0;

		glm::vec2 alignementVelocity = { 0,0 };
		int alignmentCount = 0;

		for (int y = 0; y < _boids.size(); y++)
		{
			if (x == y) continue;

			boid boidCompared = { _boids[y].pos, _boids[y].velocity };

			//Calculate distance with wrap screen
			if (abs(boidCompared.pos.x - boidLooked.pos.x) > 1)
			{
				boidCompared.pos.x += (boidCompared.pos.x > boidLooked.pos.x) ? -2 : 2;
			}
			if (abs(boidCompared.pos.y - boidLooked.pos.y) > 1)
			{
				boidCompared.pos.y += (boidCompared.pos.y > boidLooked.pos.y) ? -2 : 2;
			}

			glm::vec2 difference = boidCompared.pos - boidLooked.pos;
			float distance = sqrtf(difference.x * difference.x + difference.y * difference.y);

			//Calculate attract radius
			if (distance < AttractRadius)
			{
				attractionPosition += boidCompared.pos;
				attractionCount++;
			}

			//Calculate repulse radius
			if (distance < RepulseRadius)
			{
				repulseForce -= difference;
				repulseCount++;
			}

			//Calculate align radius
			if (distance < AlignRadius)
			{
				alignementVelocity += boidCompared.velocity;
				alignmentCount++;
			}
		}
		
		//Average forces
		attractionPosition = (attractionCount == 0) ? boidLooked.pos: attractionPosition/(float)attractionCount;
		attractionForce = (attractionPosition - boidLooked.pos);

		alignementVelocity /= (alignmentCount == 0) ? 1 : alignmentCount;
		alignementVelocity -= boidLooked.velocity;

		//Sum forces
		_boids[x].force = attractionForce * AttractForce + (repulseForce/(float)repulseCount) * RepulseForce + alignementVelocity * AlignForce;
	}
}

void boids::UpdatePosition(float deltaTime)
{
	for (int x = 0; x < _boids.size(); x++)
	{
		//Calculate velocity
		_boids[x].velocity = _boids[x].force;

		//Clamp speed
		float speed = glm::length(_boids[x].velocity);
		if (speed > SpeedMax)
		{
			_boids[x].velocity /= (speed / SpeedMax);
		}

		//Apply velocity
		_boids[x].pos += _boids[x].velocity * deltaTime;

		//Wrap screen
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
	}
}

void boids::AddBoids(unsigned int amount)
{
	for (int i = 0; i < amount; i++)
	{
		_boids.push_back({
					glm::vec2{RandomNormalize(),RandomNormalize()},
					glm::vec2{RandomNormalize(),RandomNormalize()},
					glm::vec2{0, 0},
					glm::vec4{RandomZeroOrOne(), RandomZeroOrOne(), RandomZeroOrOne(), 1}
			});
	}
}

void boids::AddBoids(glm::vec2 pos)
{
	_boids.push_back({ pos, { RandomNormalize(), RandomNormalize() } });
}

void boids::RemoveBoids(unsigned int amount)
{
	for (int i = 0; i < amount; i++)
	{
		if (_boids.size() == 0) return;
		_boids.pop_back();
	}
}

