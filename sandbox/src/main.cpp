
#include <prevengine.h>

using namespace prev;

#include "boidsdemo.h"

class SandboxLayer : public Layer {
public:
	virtual void OnAttach() override {
		m_Label.Text = "Test String";
		m_Label.Scale = Vec2(0.5f);
		m_Label.Color = Vec4(0.0f);
	}

	virtual void OnUpdate(pvfloat deltaTime) override {
		auto imguiFont = ImGuiManager::Ref().GetFont();
		Renderer2D::Ref().DrawText(imguiFont, m_Label);
	}

	virtual void OnImGuiUpdate() override {
		ImGui::Begin("C++ menu");
		ImGui::TextInput("String", m_Label.Text, 100.0f);
		ImGui::SliderRGBA("Text Color", m_Label.Color);
		ImGui::End();
	}

private:
	Label m_Label;
};

int main() {

	Application::CreateInst();

	Application::Ref().GetLayerStack().PushLayer(new BoidsDemo());
	Application::Ref().GetLayerStack().PushLayer(new SandboxLayer());

	Application::Ref().Run();

	Application::DestroyInst();

	return 0;

}