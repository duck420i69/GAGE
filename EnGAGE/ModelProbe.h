#pragma once

class ModelProbe {
public:
	virtual bool PushNode(class Node& node) noexcept = 0;
	virtual void PopNode(class Node& node) noexcept = 0;
};