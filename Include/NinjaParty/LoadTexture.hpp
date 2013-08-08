#ifndef NINJAPARTY_LOADTEXTURE_HPP
#define NINJAPARTY_LOADTEXTURE_HPP

#include <string>

namespace NinjaParty
{
	int LoadTexture(const std::string &fileName, int &width, int &height, bool resample = false);
	int LoadTextureFromBuffer(const unsigned char *buffer, int length, int &width, int &height, bool resample = false);
}

#endif//NINJAPARTY_LOADTEXTURE_HPP
