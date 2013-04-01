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
	
	char *buffer = new char[fileLength];
	fread(buffer, 1, fileLength, file);
	fclose(file);
	
	Json::Value root;
	Json::Reader reader;
	
	bool parsingSuccessful = reader.parse(buffer, root);
	delete[] buffer;
	
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
		textureRegion.origin.X() = ((frames[index]["sourceSize"]["w"].asFloat() / 2) - frames[index]["spriteSourceSize"]["x"].asFloat()) / frames[index]["spriteSourceSize"]["w"].asFloat();
		textureRegion.origin.Y() = ((frames[index]["sourceSize"]["h"].asFloat() / 2) - frames[index]["spriteSourceSize"]["y"].asFloat()) / frames[index]["spriteSourceSize"]["h"].asFloat();
		textureRegion.input = Vector2(frames[index]["spriteSourceSize"]["x"].asFloat(), frames[index]["spriteSourceSize"]["y"].asFloat());
		
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
