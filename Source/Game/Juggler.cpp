#include <NinjaParty/Juggler.hpp>

namespace NinjaParty
{
	Jugglable::Jugglable()
	{
		juggler = nullptr;
	}
	
	Jugglable::~Jugglable()
	{
		if(juggler != nullptr)
			juggler->Remove(this);
	}
}
