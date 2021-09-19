#pragma once

namespace DynamicUniform {
	class Buffer;
}

class TechniqueProbe {
protected:
	class Technique* mTech = nullptr;
	class Step* mStep = nullptr;
	size_t mTechID = 0u;
	size_t mStepID = 0u;
	size_t mBufferID = 0u;
public:
	inline void SetTechnique(class  Technique* in) noexcept {
		mTech = in; OnSetTechnique(in); mTechID++;
	}
	inline void SetStep(class  Step* in) noexcept {  
		mStep = in; OnSetStep(in); mStepID++;
	}

	bool VisitBuffer(class DynamicUniform::Buffer& buf) noexcept {
		mBufferID++;
		return OnVisitBuffer(buf);
	}
protected:
	virtual void OnSetTechnique(class  Technique*) noexcept  {};
	virtual void OnSetStep(class  Step*) noexcept  {};
	virtual bool OnVisitBuffer(class DynamicUniform::Buffer&) noexcept { return false; };
};