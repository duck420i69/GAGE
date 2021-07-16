#pragma once

#include "components.h"

class Loader
{
	static std::vector<uint32_t> sVAO, sVBO;
public:
	static MeshComponent loadMesh(const std::vector<float>& data) noexcept;
	static void shutdown();

};