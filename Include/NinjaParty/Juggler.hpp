#ifndef NINJAPARTY_JUGGLER_HPP
#define NINJAPARTY_JUGGLER_HPP

#include <algorithm>
#include <list>
#include <memory>

namespace NinjaParty
{
	class Juggler;
	
	class Jugglable
	{
	public:
		Jugglable();
		virtual ~Jugglable();
		
		virtual void Update(float deltaSeconds) = 0;
		virtual bool IsComplete() const = 0;

	protected:
		friend class Juggler;
		Juggler *juggler;
	};
	
	class Juggler
	{
	public:
		void Add(Jugglable *item) { Add(std::shared_ptr<Jugglable>(item)); }
		void Add(std::shared_ptr<Jugglable> item) { item->juggler = this; items.push_back(item); }
		
		void Remove(Jugglable *item)
		{
			items.remove_if([&](std::shared_ptr<Jugglable> p){return p.get() == item;});
		}
		
		void Update(float deltaSeconds)
		{
			for(auto &i : items)
				i->Update(deltaSeconds);
			
			items.remove_if([](std::shared_ptr<Jugglable> p){return p->IsComplete();});
		}
		
	protected:
		std::list<std::shared_ptr<Jugglable>> items;
	};
}

#endif//NINJAPARTY_JUGGLER_HPP
