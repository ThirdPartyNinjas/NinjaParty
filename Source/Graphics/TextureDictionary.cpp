#include <stdio.h>

#include <json/json.h>

#include <NinjaParty/TextureDictionary.hpp>

namespace NinjaParty
{

TextureDictionary::TextureDictionary()
{
}

TextureDictionary* TextureDictionary::FromFile(const std::string &filename)
{
	FILE *file = fopen(filename.c_str(), "r");
	if(!file)
		return nullptr;
	
	fseek(file, 0, SEEK_END);
	int fileLength = static_cast<int>(ftell(file));
	fseek(file, 0, SEEK_SET);
	
	unsigned char *buffer = new unsigned char[fileLength];
	fread(buffer, 1, fileLength, file);
	fclose(file);

	TextureDictionary *textureDictionary = FromBuffer(buffer, fileLength);

	delete[] buffer;
    
    return textureDictionary;
}

TextureDictionary* TextureDictionary::FromBuffer(const unsigned char *buffer, int size)
{
	Json::Value root;
	Json::Reader reader;
	
	bool parsingSuccessful = reader.parse(reinterpret_cast<const char*>(buffer), root);
	
	if(!parsingSuccessful)
		return nullptr;

	Json::Value frames = root["frames"];
	
	TextureDictionary *textureDictionary = new TextureDictionary();
	
	for(int index = 0; index < (int)frames.size(); ++index)
	{
		std::string name = frames[index]["filename"].asString();
		
		TextureRegion textureRegion;
		textureRegion.bounds.x = frames[index]["frame"]["x"].asInt();
		textureRegion.bounds.y = frames[index]["frame"]["y"].asInt();
		textureRegion.bounds.width = frames[index]["frame"]["w"].asInt();
		textureRegion.bounds.height = frames[index]["frame"]["h"].asInt();
		textureRegion.rotated = frames[index]["rotated"].asBool();
        
        float spriteSourceSizeX = frames[index]["spriteSourceSize"]["x"].asFloat();
        float spriteSourceSizeY = frames[index]["spriteSourceSize"]["y"].asFloat();
        float spriteSourceSizeW = frames[index]["spriteSourceSize"]["w"].asFloat();
        float spriteSourceSizeH = frames[index]["spriteSourceSize"]["h"].asFloat();
        
        float sourceSizeW = frames[index]["sourceSize"]["w"].asFloat();
        float sourceSizeH = frames[index]["sourceSize"]["h"].asFloat();
        
        textureRegion.originTopLeft.X() = -spriteSourceSizeX / spriteSourceSizeW;
        textureRegion.originTopLeft.Y() = -spriteSourceSizeY / spriteSourceSizeH;
        
        textureRegion.originBottomRight.X() = 1 + (sourceSizeW - (spriteSourceSizeX + spriteSourceSizeW)) / spriteSourceSizeW;
        textureRegion.originBottomRight.Y() = 1 + (sourceSizeH - (spriteSourceSizeY + spriteSourceSizeH)) / spriteSourceSizeH;
        
        textureRegion.originCenter = (textureRegion.originTopLeft + textureRegion.originBottomRight) / 2;
        
        textureRegion.input.x = (int)spriteSourceSizeX;
        textureRegion.input.y = (int)spriteSourceSizeY;
        textureRegion.input.width = (int)sourceSizeW;
        textureRegion.input.height = (int)sourceSizeH;
		
		textureDictionary->dictionary[name] = textureRegion;
	}

	return textureDictionary;
}

bool TextureDictionary::ContainsTexture(const std::string &textureName)
{
	return dictionary.count(textureName) == 1;
}

TextureRegion& TextureDictionary::GetRegion(const std::string &file)
{
	return dictionary[file];
}

}
