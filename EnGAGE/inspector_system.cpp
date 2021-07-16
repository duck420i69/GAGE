#include "pch.h"
#include "inspector_system.h"

#include "ecs.h"
#include "imgui/imgui.h"

void InspectorSystem::renderImGui() const noexcept
{
	static int selected = -1;
	ImGui::Begin("Entities from InspectorSystem");
	auto& ecs = ECS::getInstance();
	for (const auto& e : mEntities)
	{
		NameComponent& name = ecs.getComponent<NameComponent>(e);
		if (ImGui::Selectable(name.name.c_str(), selected == e))
		{
			selected = e;
		}
	}

	ImGui::End();
	ImGui::Begin("Inspector from InspectorSystem");
	if (mEntities.find(selected) != mEntities.end())
	{
		TransformComponent& transform = ecs.getComponent<TransformComponent>(selected);
		NameComponent& name = ecs.getComponent<NameComponent>(selected);
		ImGui::Text("Inspecting: %s", name.name.c_str());
		ImGui::DragFloat3("Position", &transform.position.x);
		ImGui::DragFloat4("Rotation", &transform.rotation.x);
		ImGui::DragFloat("Scale", &transform.scale);
	}
	ImGui::End();
}
