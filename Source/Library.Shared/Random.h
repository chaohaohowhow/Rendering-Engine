#pragma once
namespace Library
{
	class Random final
	{
	public:
		Random() = delete;
		Random(const Random&) = delete;
		Random& operator=(const Random&) = delete;
		Random(Random&&) = delete;
		Random& operator=(Random&&) = delete;
		~Random() = default;

		static float RandomFloat();
		static float RandomFloat(float max);
		static float RandomFloat(float min, float max);
	private:
		static void Seed();
		static bool mIsSeeded;
	};
}

