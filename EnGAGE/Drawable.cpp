#include "pch.h"
#include "Drawable.h"

#include "RenderQueue.h"

Drawable::~Drawable()
{
}

void Drawable::Submit(RenderQueue& queue) const noexcept
{
	for (const Technique& t : mTechniques) {
		t.Submit(queue, *this);
	}
}

void Drawable::Bind() const noexcept
{
	mVertexData->Bind();
}

void Drawable::Accept(TechniqueProbe& probe) noexcept
{
	for (Technique& t : mTechniques) {
		t.Accept(probe);
	}
}

void Drawable::AddTechnique(Technique technique) noexcept
{
	technique.InitParent(*this);
	mTechniques.push_back(technique);
}
