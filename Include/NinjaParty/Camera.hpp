#ifndef NINJAPARTY_CAMERA_HPP
#define NINJAPARTY_CAMERA_HPP

#include <NinjaParty/Vector2.hpp>
#include <NinjaParty/Matrix3.hpp>

namespace NinjaParty
{
	class Camera
	{
	public:
		Camera(int screenWidth, int screenHeight);
		
		Vector2 GetScreenDimensions() const { return screenDimensions; }
		void SetScreenDimensions(const Vector2 &screenDimensions);
		
		Vector2 GetPosition() const { return position; }
		void SetPosition(const Vector2 &position);
		
		float GetRotation() const { return angle; }
		void SetRotation(const float &angle);
		
		float GetZoom() const { return zoom; }
		void SetZoom(const float &zoom);
		
		Vector2 GetTopLeft() const;
		Vector2 GetBottomRight() const;
		
		Matrix3 GetTransform() const;
		
	protected:
		Vector2 screenDimensions;
		Vector2 position;
		float angle;
		float zoom;
	};
}

#endif//NINJAPARTY_CAMERA_HPP
