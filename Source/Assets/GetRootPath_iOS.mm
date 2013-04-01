#include <string>

#include <NinjaParty/AssetManager.hpp>

namespace NinjaParty
{
	std::string AssetManager::GetRootPath() const
	{
		NSString *path = [[NSBundle mainBundle] bundlePath];
		
		return std::string(static_cast<const char*>([path UTF8String])) + "/";
	}
}
