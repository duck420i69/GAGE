#include "pch.h"
#include "Player.h"

#include "Events.h"
#include "Window.h"
#include "Logger.h"
#include "TileType.h"
#include "TileMap.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <imgui/imgui.h>


void Player::Update(float delta) noexcept
{
	UpdateZoom(delta);
	UpdateMovement(delta);
	UpdateCursor();
}


void Player::UpdateZoom(float delta) noexcept
{
	float scroll = Events::GetDScroll();
	mZoom -= ZOOM_SPEED * delta * scroll;

	if (mZoom < 1.0f) mZoom = 1.0f;
}

void Player::UpdateMovement(float delta) noexcept
{
	glm::vec2 dir = { 0, 0 };

	if (Events::IsKeyDown(Events::KEY_W)) {
		dir.y += 1;
	}
	if (Events::IsKeyDown(Events::KEY_S)) {
		dir.y -= 1;
	}
	if (Events::IsKeyDown(Events::KEY_D)) {
		dir.x += 1;
	}
	if (Events::IsKeyDown(Events::KEY_A)) {
		dir.x -= 1;
	}

	dir = glm::length2(dir) != 0 ? glm::normalize(dir) : glm::vec2(0, 0);
	mPos += dir * MOVE_SPEED * delta;
}

void Player::UpdateCursor() noexcept
{

	float real_x = (2 * Events::GetX() - Window::GetWidth()) * (mZoom / Window::GetHeight()) + mPos.x;
	float real_y = (1 - (2 * Events::GetY()) / Window::GetHeight()) * mZoom + mPos.y;


	mCursorPos.x = real_x < 0.0f ? (int)glm::floor(real_x) : (int)real_x;
	mCursorPos.y = real_y < 0.0f ? (int)glm::floor(real_y) : (int)real_y;
}
