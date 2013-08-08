#ifndef NINJAPARTY_RANDOM_HPP
#define NINJAPARTY_RANDOM_HPP

#include <limits>
#include <random>

namespace NinjaParty
{
	class Random
	{
	public:
		Random();
		Random(unsigned int seed);
		Random(const Random &random);
		Random& operator=(const Random &random);
		~Random() = default;
		
		void Seed(unsigned int seed);
		
		int NextInt(int maxValue = std::numeric_limits<int>::max());
		int NextInt(int minValue, int maxValue);
		
		float NextFloat(float maxValue = 1.0f);
		float NextFloat(float minValue, float maxValue);
		
	private:
		std::mt19937 generator;
		std::uniform_int_distribution<int> intDistribution;
		std::uniform_real_distribution<float> floatDistribution;
	};
}

#endif//NINJAPARTY_RANDOM_HPP

