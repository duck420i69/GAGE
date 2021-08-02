#pragma once

#include <glm/vec2.hpp>

class Player
{
	const float DEFAULT_ZOOM = 10.0f;
	const float ZOOM_SPEED = 8.0f;
	const float MOVE_SPEED = 4.0f;

	float		mZoom = DEFAULT_ZOOM;
	glm::ivec2  mCursorPos = { 0, 0 };
	glm::vec2	mPos = { 0, 0 };
public:
	Player() noexcept = default;
	void Update(float delta) noexcept;

	inline const float& GetZoom() const noexcept { return mZoom; }
	inline const glm::vec2& GetPos() const noexcept { return mPos; }
	inline const glm::ivec2& GetCursor() const noexcept { return mCursorPos; }
private:
	void UpdateZoom(float delta) noexcept;
	void UpdateMovement(float delta) noexcept;
	void UpdateCursor() noexcept;
};

