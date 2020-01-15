
#include <prevengine.h>

using namespace prev;

class SandboxLayer : public Layer {
	virtual void OnUpdate(pvfloat deltaTime) override {}

	virtual void OnImGuiUpdate() override {
		ImGui::Begin("Sandbox Window");
		ImGui::End();
	}
};

int main() {

	Application::CreateInst();

	Application::Ref().GetLayerStack().PushLayer(new SandboxLayer());

	Application::Ref().Run();

	Application::DestroyInst();

	return 0;

}