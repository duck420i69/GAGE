#pragma once

#include <string>
#include <typeinfo>

class Bindable {
public:
	virtual ~Bindable() = default;
	virtual void Bind() const noexcept = 0;

	virtual std::string GetUID() const noexcept {
		assert(false);
		return "";
	}
	virtual void InitParent(const class Drawable&) noexcept {};
	virtual void Accept(class TechniqueProbe&) noexcept {};
};