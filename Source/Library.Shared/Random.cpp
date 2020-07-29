#include "pch.h"
#include "Random.h"

using namespace std;

namespace Library
{
	bool Random::mIsSeeded = false;

	float Random::RandomFloat()
	{
		Seed();
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	}
	float Random::RandomFloat(float max)
	{
		Seed();
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX / max);
	}
	float Random::RandomFloat(float min, float max)
	{
		Seed();
		return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max-min));
	}

	float Random::RandomFloat(glm::vec2 range)
	{
		Seed();
		return RandomFloat(range.x, range.y);
	}

	void Random::Seed()
	{
		if (!mIsSeeded)
		{
			srand(static_cast<unsigned int>(time(nullptr)));
			mIsSeeded = true;
		}
	}

}