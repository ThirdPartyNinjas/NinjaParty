#include <NinjaParty/Random.hpp>

namespace NinjaParty
{
	Random::Random()
	{
	}
	
	Random::Random(unsigned int seed)
		: generator(seed)
	{
	}
	
	Random::Random(const Random &random)
		: generator(random.generator), intDistribution(random.intDistribution), floatDistribution(random.floatDistribution)
	{
	}
	
	Random& Random::operator=(const NinjaParty::Random &rhs)
	{
		generator = rhs.generator;
		intDistribution = rhs.intDistribution;
		floatDistribution = rhs.floatDistribution;
		return *this;
	}
	
	void Random::Seed(unsigned int seed)
	{
		generator.seed(seed);
	}
	
	int Random::NextInt(int maxValue)
	{
		return intDistribution(generator, std::uniform_int_distribution<int>::param_type(0, maxValue));
	}
	
	int Random::NextInt(int minValue, int maxValue)
	{
		return intDistribution(generator, std::uniform_int_distribution<int>::param_type(minValue, maxValue));
	}
	
	float Random::NextFloat(float maxValue)
	{
		return floatDistribution(generator, std::uniform_real_distribution<float>::param_type(0, maxValue));
	}
	
	float Random::NextFloat(float minValue, float maxValue)
	{
		return floatDistribution(generator, std::uniform_real_distribution<float>::param_type(minValue, maxValue));
	}
}
