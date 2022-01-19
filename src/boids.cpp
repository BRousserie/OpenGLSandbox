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
	return unif(rng) * 2 - 1;
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

std::list<boids::boid>& boids::GetBoids()
{
	return _boids;
}

std::vector<object*> boids::GetBoidsPtr()
{
	return _boids_ptr;
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

	int x = 0;
	//Calculate forces
	for (boid& boidLooked : _boids)
	{
		glm::vec2 attractionForce = { 0,0 };
		glm::vec2 attractionPosition = { 0,0 };
		int neighbour = 0;

		glm::vec2 repulseForce = { 0,0 };
		int repulseCount = 0;

		glm::vec2 alignementVelocity = { 0,0 };
		int alignmentCount = 0;

		int y = 0;
		for (boid boidCompared : _boids)
		{
			if (x == y) continue;

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
				neighbour++;
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

			y++;
		}
		
		//Average forces
		attractionPosition = (neighbour == 0) ? boidLooked.pos: attractionPosition/(float)neighbour;
		attractionForce = (attractionPosition - boidLooked.pos) * AttractForce;

		repulseForce *= RepulseForce;

		alignementVelocity /= (alignmentCount == 0) ? 1 : alignmentCount;

		//Sum forces
		boidLooked.force = attractionForce + repulseForce * RepulseForce + alignementVelocity * AlignForce;
		x++;
	}

	UpdatePosition(deltaTime);
}

void boids::UpdatePosition(float deltaTime)
{
	for (boid& boid : _boids)
	{
		//Calculate velocity
		boid.velocity = boid.force * deltaTime + boid.velocity;

		//Clamp speed
		float speed = glm::length(boid.velocity);
		if (speed > SpeedMax)
		{
			boid.velocity /= (speed / SpeedMax);
		}

		//Apply velocity
		boid.pos += boid.velocity * deltaTime;

		//Wrap screen
		if (boid.pos.x > 1)
		{
			boid.pos.x -= 2;
		}
		if (boid.pos.y > 1)
		{
			boid.pos.y -= 2;
		}

		if (boid.pos.x < -1)
		{
			boid.pos.x += 2;
		}
		if (boid.pos.y < -1)
		{
			boid.pos.y += 2;
		}
	}
}

void boids::AddBoids(unsigned int amount)
{
	for (int i = 0; i < amount; i++)
	{
		_boids.push_back(
			{
					glm::vec2{RandomNormalize(),RandomNormalize()},
					glm::vec4{RandomZeroOrOne(), RandomZeroOrOne(), RandomZeroOrOne(), 1},
					glm::vec2{RandomNormalize(),RandomNormalize()},
					glm::vec2{0, 0}
			});
		_boids_ptr.push_back(&_boids.back());
	}
}

void boids::AddBoidAt(glm::vec2 pos)
{
	_boids.push_back({ pos, { 1, 0, 0, 1}, { RandomNormalize(), RandomNormalize() }, { 0, 0 } });
	_boids_ptr.push_back(&_boids.back());

}

void boids::RemoveBoids(unsigned int amount)
{
	for (int i = 0; i < amount; i++)
	{
		if (_boids.size() == 0) return;
		_boids.pop_back();
		_boids_ptr.pop_back();
	}
}

