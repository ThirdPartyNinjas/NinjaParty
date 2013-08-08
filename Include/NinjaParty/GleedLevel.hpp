#ifndef NINJAPARTY_GLEEDLEVEL_HPP
#define NINJAPARTY_GLEEDLEVEL_HPP

#include <map>
#include <memory>
#include <vector>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/Vector2.hpp>

namespace NinjaParty
{
	enum class GleedItemType
	{
		Texture,
		Path,
		Rectangle,
		Circle,
	};

	struct GleedItem
	{
		virtual ~GleedItem() { }
		virtual GleedItemType GetItemType() const = 0;

		std::string name;
		std::map<std::string, std::string> customProperties;
		bool visible;
		NinjaParty::Vector2 position;
	};

	struct GleedTextureItem : public GleedItem
	{
		GleedItemType GetItemType() const { return GleedItemType::Texture; }

		bool flipHorizontally;
		bool flipVertically;
		NinjaParty::Vector2 origin;
		float rotation;
		NinjaParty::Vector2 scale;
		std::string textureFileName;
		NinjaParty::Color tintColor;
	};

	struct GleedPathItem : public GleedItem
	{
		GleedItemType GetItemType() const { return GleedItemType::Path; }

		bool isPolygon;
		NinjaParty::Color lineColor;
		float lineWidth;
		std::vector<NinjaParty::Vector2> localPoints;
		std::vector<NinjaParty::Vector2> worldPoints;
	};

	struct GleedRectangleItem : public GleedItem
	{
		GleedItemType GetItemType() const { return GleedItemType::Rectangle; }

		NinjaParty::Color fillColor;
		float height;
		float width;
	};

	struct GleedCircleItem : public GleedItem
	{
		GleedItemType GetItemType() const { return GleedItemType::Circle; }

		NinjaParty::Color fillColor;
		float radius;
	};

	struct GleedLayer
	{
		std::string name;
		bool visible;
		NinjaParty::Vector2 scrollSpeed;
		std::vector<std::shared_ptr<GleedItem> > items;
		std::map<std::string, std::string> customProperties;
	};

	struct GleedLevel
	{
		std::map<std::string, std::string> customProperties;
		std::vector<GleedLayer> layers;
	};
}

#endif//NINJAPARTY_GLEEDLEVEL_HPP
