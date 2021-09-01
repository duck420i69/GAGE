#pragma once

#include "Drawable.h"

template<typename T>
class DrawableBase : public Drawable {
	static std::vector<std::unique_ptr<Bindable>> s_binds;
protected:
	static bool IsStaticInited() noexcept {
		return !s_binds.empty();
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBufferObject> ibuf) noexcept {
		mIndexBuffer = ibuf.get();
		s_binds.push_back(std::move(ibuf));
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept {
		s_binds.push_back(std::move(bind));
	}

	void SetIndexFromStatic() noexcept {
		for (const auto& b : s_binds) {
			if (const auto p = dynamic_cast<IndexBufferObject*>(b.get())) {
				mIndexBuffer = p;
				return;
			}
		}
	}

private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBind() const noexcept override {
		return s_binds;
	}
};

template<typename T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::s_binds;