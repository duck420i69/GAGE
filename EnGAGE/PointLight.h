#pragma once

#include "UniformBufferObject.h"

#include <glm/vec3.hpp>
#include <imgui.h>

class PointLight {

	struct PointLightCBuf {
		alignas(16) glm::vec3 light_pos;
		alignas(16) glm::vec3 ambient_color;
		alignas(16) glm::vec3 diffuse_color;
		float diffuse_intensity;
		float att_const;
		float att_linear;
		float att_exponent;
	};

	PointLightCBuf buf;
	UniformBufferObject< PointLightCBuf> cbuf; //Uses slot 1
public:
	PointLight() noexcept : cbuf(1) {
		cbuf.Update(buf);
		Reset();
	}

	inline void Reset() noexcept {
		buf.light_pos = { 0, 0, 0 };
		buf.ambient_color = { 0.5f, 0.5f, 0.5f };
		buf.diffuse_color = { 1.0f, 1.0f, 1.0f };
		buf.diffuse_intensity = 1.0f;
		buf.att_const = 1.0f;
		buf.att_linear = 0.045f;
		buf.att_exponent = 0.0075f;
	}

	inline void SpawnControlWindow() noexcept {
		ImGui::Begin("Light");

		ImGui::DragFloat3("Position", &buf.light_pos.x, 0.3f);
		ImGui::ColorEdit3("Ambient Color", &buf.ambient_color.x);
		ImGui::ColorEdit3("Diffuse Color", &buf.diffuse_color.x);
		ImGui::SliderFloat("Diffuse Intensity", &buf.diffuse_intensity, 0, 1);
		ImGui::SliderFloat("Attenuation constant", &buf.att_const, 0, 1);
		ImGui::SliderFloat("Attenuation linear", &buf.att_linear, 0, 0.1f, "%.3f");
		ImGui::SliderFloat("Attenuation exponent", &buf.att_exponent, 0, 0.01f, "%.7f");

		if (ImGui::Button("Reset")) {
			Reset();
		}

		ImGui::End();
	}

	inline void Bind(const glm::mat4& view) noexcept {
		auto copy = buf;

		copy.light_pos = view * glm::vec4(copy.light_pos, 1);

		cbuf.Update(copy);
		cbuf.Bind();
	}
};