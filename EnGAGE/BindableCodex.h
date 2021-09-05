#pragma once

#include "Bindable.h"

#include <unordered_map>
#include <string>
#include <memory>

class BindableCodex {
	std::unordered_map < std::string, std::shared_ptr<Bindable>> binds;
public:
	template<class T, typename... Params>
	static std::shared_ptr<T> Resolve(Params&&... params) noexcept {
		return Get().ResolveInternal<T>(std::forward<Params>(params)...);
	}

private:
	static BindableCodex& Get() noexcept {
		static BindableCodex instance;
		return instance;
	}

	template<class T, typename... Params>
	std::shared_ptr<T> ResolveInternal(Params&&... params) noexcept {
		const auto key = T::GenerateUID(std::forward<Params>(params)...);
		const auto& it = binds.find(key);
		if (it == binds.end()) {
			auto bind = std::make_shared<T>(std::forward<Params>(params)...);
			binds[key] = bind;
			return bind;
		}
		else {
			return std::static_pointer_cast<T>(it->second);
		}
	}

};