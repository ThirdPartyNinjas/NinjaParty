#include <NinjaParty/Camera.hpp>

namespace NinjaParty
{	
	Camera::Camera(int screenWidth, int screenHeight)
	{
		SetPosition(Vector2(0, 0));
		SetRotation(0);
		SetZoom(1);
		SetScreenDimensions(Vector2(static_cast<float>(screenWidth), static_cast<float>(screenHeight)));
	}

	void Camera::SetScreenDimensions(const Vector2 &screenDimensions)
	{
		this->screenDimensions = screenDimensions;
	}

	void Camera::SetPosition(const Vector2 &position)
	{
		this->position = position;
	}

	void Camera::SetRotation(const float &angle)
	{
		this->angle = angle;
	}

	void Camera::SetZoom(const float &zoom)
	{
		this->zoom = zoom;
	}

	Matrix3 Camera::GetTransform() const
	{
		return CreateTranslationMatrix(screenDimensions.X() / 2, screenDimensions.Y() / 2) *
		CreateRotationMatrix(angle) *
		CreateScaleMatrix(zoom, zoom) *
		CreateTranslationMatrix(-position.X(), -position.Y());
	}

	Vector2 Camera::GetTopLeft() const
	{
		return Vector2(position.X() - screenDimensions.X() / 2 / zoom, position.Y() - screenDimensions.Y() / 2 / zoom);
	}

	Vector2 Camera::GetBottomRight() const
	{
		return Vector2(position.X() + screenDimensions.X() / 2 / zoom, position.Y() + screenDimensions.Y() / 2 / zoom);
	}	
}
