#ifndef NINJAPARTY_ASSETMANAGER_HPP
#define NINJAPARTY_ASSETMANAGER_HPP

#include <string>
#include <map>
#include <memory>
#include <utility>

namespace spine
{
    struct Animation;
    struct SkeletonData;
}

namespace NinjaParty
{
	class Song;
	class SoundEffect;
	
	struct Font;
	class Texture;
	class TextureDictionary;
	
	struct DeminaAnimation;
	struct SpriteAnimation;

	struct GleedLevel;
	
	typedef spine::Animation SpineAnimation;
    typedef spine::SkeletonData SpineSkeletonData;
	
	class Foobar
	{
	public:
		Foobar();
	};
	
	class AssetManager
	{
	public:
		AssetManager(const std::string &assetPath = "Assets/", const std::string &assetZipPath = "");
		~AssetManager();
		
		std::string GetAssetPath() const { return assetPath; }
		std::string GetAssetRootPath() const { return assetRootPath; }
		
		Texture* LoadTexture(const std::string &fileName, bool forceReload = false);
		TextureDictionary* LoadTextureDictionary(const std::string &fileName);
		
		Song* LoadSong(const std::string &fileName);
		SoundEffect* LoadSoundEffect(const std::string &fileName);

		Font* LoadFont(const std::string &fileName);

		DeminaAnimation* LoadDeminaAnimation(const std::string &fileName, Texture *texture, TextureDictionary *textureDictionary);
		SpriteAnimation* LoadSpriteAnimation(const std::string &fileName, Texture *texture, TextureDictionary *textureDictionary);

		GleedLevel* LoadGleedLevel(const std::string &fileName);
		
		SpineSkeletonData* LoadSpineSkeletonData(const std::string &fileName, TextureDictionary *textureDictionary);
		SpineAnimation* LoadSpineAnimation(const std::string &animationName, SpineSkeletonData *skeletonData);

	private:
		std::string GetRootPath() const; // get the platform specific path
        void CreateArchiveMap();
        void DestroyArchiveMap();
		
		std::string assetPath;
        std::string assetRootPath;
		std::string assetZipPath;
        
        unsigned char *mappedAssetArchive;
        int mappedMemoryLength;
        std::map<std::string, std::pair<int, int>> archiveInfo;

		std::map<std::string, Song*> songs;
		std::map<std::string, SoundEffect*> soundEffects;

		std::map<std::string, Texture*> textures;
		std::map<std::string, TextureDictionary*> textureDictionaries;

		std::map<std::string, Font*> fonts;

		std::map<std::string, DeminaAnimation*> deminaAnimations;
		std::map<std::string, SpriteAnimation*> spriteAnimations;

		std::map<std::string, GleedLevel*> gleedLevels;

		std::map<std::string, SpineSkeletonData*> spineSkeletons;
	};
}

#endif//NINJAPARTY_ASSETMANAGER_HPP
