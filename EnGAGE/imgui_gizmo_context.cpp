#include "pch.h"
#include "imgui_gizmo_context.h"

#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"

void ImGuiGizmoContext::init() noexcept
{
	ImGuizmo::Enable(true);
}

void ImGuiGizmoContext::prepare(const Camera& camera) noexcept
{
	ImGuizmo::BeginFrame();

}
