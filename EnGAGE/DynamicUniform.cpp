#include "pch.h"
#include "DynamicUniform.h"


namespace DynamicUniform {

	Layout& Layout::PushBack(const std::string& name, const Type& type) noexcept
	{
		assert(mElements.count(name) == 0 && "Element already exited !");
		mElements[name] = Element{ type, 0 };
		return *this;
	}

	Type& Layout::operator[](const std::string& name) noexcept
	{
		assert(mElements.count(name) == 0 && "Element already exited !");
		return mElements[name].type;
	}

	std::string Layout::GenerateUID() const noexcept
	{
		std::stringstream ss;

		for (const auto& [name, element] : mElements) {
			ss << Element::GetTypeUID(element.type);
		}

		return ss.str();
	}

	DynamicUniform::Buffer::Buffer(const Layout& layout) noexcept :
		mLayout(layout)
	{
		size_t offset = 0;
		size_t prev_offset = 0;
		for (const auto& [name, element] : layout.mElements) {
			mBakedElements[name] = Element{ element.type, offset == 0 ? offset : offset - 1};
			size_t size = Element::GetSize(element.type);
			for (size_t i = 0; i < size; i++) {

				if (offset != 0 && (offset % 16) == 0) {
					i = 0;
					mBakedElements[name].offset += offset - prev_offset;
				}
				offset++;
			}
			prev_offset = offset;
		}
		mBuffer.resize(offset);
	}

	Buffer::ElementRef Buffer::operator[](const std::string& name) noexcept
	{
		Element& ele = mBakedElements[name];
		return { ele.type, &mBuffer[ele.offset], Element::GetSize(ele.type), mBuffer.data(),  mBuffer.data() + mBuffer.size() };
	}



}
