#include "pch.h"
#include "MenuScene.h"

#include "Events.h"

class TP final : public TechniqueProbe {
public:
	void OnSetTechnique(class  Technique*) noexcept override {
		ImGui::TextColored({ 0.4f, 1.0f, 0.6f, 1.0f }, mTech->GetName().c_str());
		ImGui::Checkbox(("Tech_active##" + std::to_string(mTechID)).c_str(), &mTech->GetActiveRef());
	};
	bool OnVisitBuffer(class DynamicUniform::Buffer& buf) noexcept override {
		float dirty = false;
		auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
		auto tag = [tag_scratch = std::string(), tag_string = "##" + std::to_string(mBufferID)](const char* label) mutable {
			tag_scratch = label + tag_string;
			return tag_scratch.c_str();
		};


		if (DynamicUniform::Buffer::ElementRef v = buf["specular_power"]; v.Exist()) {
			dcheck(ImGui::SliderInt(tag("specular_power"), reinterpret_cast<int*>(v.location), 16, 256));
		}

		if (DynamicUniform::Buffer::ElementRef v = buf["outline_color"]; v.Exist()) {
			dcheck(ImGui::ColorEdit3(tag("outline_color"), reinterpret_cast<float*>(v.location)));
		}

		if (DynamicUniform::Buffer::ElementRef v = buf["offset"]; v.Exist()) {
			dcheck(ImGui::SliderFloat(tag("offset"), reinterpret_cast<float*>(v.location), 0.0f, 2.0f));
		}


		if (DynamicUniform::Buffer::ElementRef v = buf["outline_scale"]; v.Exist()) {
			dcheck(ImGui::DragFloat(tag("outline_scale"), reinterpret_cast<float*>(v.location), 0.001f, 1.0f, 2.0f));
		}


		return dirty;
	}
} probe;

class MP final : public ModelProbe {
	struct TransformParams {
		glm::vec3 pos;
		glm::vec3 rot;
	};
	Node* pSelectedNode = nullptr;
	std::unordered_map<int, TransformParams> mTransformParams;
public:
	void SpawnWindow(Model& model) {
		ImGui::Begin("Model");
		ImGui::Columns(2, nullptr, true);
		model.Accept(*this);
		ImGui::NextColumn();
		if (pSelectedNode != nullptr) {
			bool dirty = false;
			const auto dcheck = [&dirty](bool changed) { dirty = dirty | changed; };
			auto& tf = ResolveTransform();
			ImGui::TextColored({ 0.4f, 1.0f, 0.6f, 1.0f }, "Translation");
			dcheck(ImGui::DragFloat("X", &tf.pos.x, 0.1f));
			dcheck(ImGui::DragFloat("Y", &tf.pos.y, 0.1f));
			dcheck(ImGui::DragFloat("Z", &tf.pos.z, 0.1f));

			ImGui::TextColored({ 0.4f, 1.0f, 0.6f, 1.0f }, "Orientation");
			dcheck(ImGui::DragFloat("PITCH", &tf.rot.x, 0.1f));
			dcheck(ImGui::DragFloat("YAW", &tf.rot.y, 0.1f));
			dcheck(ImGui::DragFloat("ROLL", &tf.rot.z, 0.1f));

			if (dirty) {
				glm::mat4x4 built;
				built = glm::translate(glm::mat4(1.0f), tf.pos);
				built = glm::rotate(built, tf.rot.x, { 1, 0, 0 });
				built = glm::rotate(built, tf.rot.y, { 0, 1, 0 });
				built = glm::rotate(built, tf.rot.z, { 0, 0, 1 });
				pSelectedNode->SetAppliedTransform(built);
			}

			TP probe;
			pSelectedNode->Accept(probe);
		}
		ImGui::End();
	}

	virtual bool PushNode(class Node& node) noexcept {
		const int selectedID = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetID();

		const auto flags = ImGuiTreeNodeFlags_OpenOnArrow |
			((node.GetID() == selectedID) ? ImGuiTreeNodeFlags_Selected : 0) |
			(node.HasChildren() ? 0 : ImGuiTreeNodeFlags_Leaf);

		const auto expanded = ImGui::TreeNodeEx((void*)(intptr_t)node.GetID(), flags, node.GetName().c_str());
		if (ImGui::IsItemClicked()) {

			class Probe : public TechniqueProbe {

			public:
				bool highlighted = false;
				void OnSetTechnique(Technique* tech) noexcept override {
					if (tech->GetName() == "outline") {
						tech->GetActiveRef() = highlighted;
					}
				}
			};
			Probe probe;
			if (pSelectedNode != nullptr) {
				pSelectedNode->Accept(probe);
			}
			probe.highlighted = true;
			node.Accept(probe);

			pSelectedNode = &node;
		}


		return expanded;
	};
	virtual void PopNode(class Node& node) noexcept {
		ImGui::TreePop();
	};
private:
	TransformParams& ResolveTransform() noexcept {
		const auto id = pSelectedNode->GetID();
		auto i = mTransformParams.find(id);
		if (i == mTransformParams.end()) {
			return LoadTransform(id);
		}

		return i->second;
	};
	TransformParams& LoadTransform(int id) noexcept {
		const auto& applied = pSelectedNode->GetAppliedTransform();
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(applied, scale, rotation, translation, skew, perspective);
		rotation = glm::conjugate(rotation);
		glm::vec3 euler = glm::eulerAngles(rotation);

		return mTransformParams.insert({ id, {translation, euler} }).first->second;
	}
};

MenuScene::MenuScene() noexcept
	:
	mCam(),
	mMuro("Assets/Models/gobber/GoblinX.obj"),
	mSponza("Assets/Models/Sponza/sponza.obj", 0.1f)
	//mBrickWall("Assets/Models/brick_wall/brick_wall.obj")
{
	Opengl::SetProjection(glm::perspective(glm::radians(75.0f), 16.0f / 9.0f, 0.05f, 1000.0f));
}

void MenuScene::Update(float delta) noexcept
{
	if (Events::IsKeyDownOnce(Events::KEY_TAB)) {
		Window::ToggleCursor();
	}

	if (!Window::IsCursorEnabled()) {
		if (Events::IsKeyDown(Events::KEY_W)) {
			mCam.MoveForward(delta);
		}
		if (Events::IsKeyDown(Events::KEY_S)) {
			mCam.MoveForward(-delta);
		}
		if (Events::IsKeyDown(Events::KEY_D)) {
			mCam.MoveRight(delta);
		}
		if (Events::IsKeyDown(Events::KEY_A)) {
			mCam.MoveRight(-delta);
		}
		mCam.Rotate(Events::GetDY(), Events::GetDX());
	}
	Opengl::SetCamera(mCam.GetMatrix());
}
void MenuScene::Render() noexcept
{
	Opengl::Clear();
	mLight.Bind(mCam.GetMatrix());
	//mPlane.Submit(mRenQueue);
	//mBox2.Submit(mRenQueue);

	mSponza.Submit(mRenQueue);
	mMuro.Submit(mRenQueue);

	mRenQueue.Execute();
	mRenQueue.Reset();
}
void MenuScene::ImGui() noexcept
{
	static MP mp;
	mp.SpawnWindow(mSponza);
	mCam.SpawnControlWindow();
	mLight.SpawnControlWindow();

};