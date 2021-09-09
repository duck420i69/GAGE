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
};