#ifndef NINJAPARTY_JUGGLER_HPP
#define NINJAPARTY_JUGGLER_HPP

#include <algorithm>
#include <list>
#include <memory>

namespace NinjaParty
{
	struct IJugglable
	{
		virtual ~IJugglable() { }
		virtual void Update(float deltaSeconds) = 0;
		virtual bool IsComplete() const = 0;
	};
	
	class Juggler
	{
	public:
		void Add(IJugglable *item) { Add(std::shared_ptr<IJugglable>(item)); }
		void Add(std::shared_ptr<IJugglable> item) { items.push_back(item); }
		
		void Update(float deltaSeconds)
		{
			for(auto &i : items)
				i->Update(deltaSeconds);
			
			items.remove_if([](std::shared_ptr<IJugglable> p){return p->IsComplete();});
		}
		
	protected:
		std::list<std::shared_ptr<IJugglable>> items;
	};
}

#endif//NINJAPARTY_JUGGLER_HPP
