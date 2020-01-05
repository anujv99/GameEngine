
#include <prevengine.h>

using namespace prev;

class SandboxLayer : public Layer {
	virtual void OnUpdate(pvfloat deltaTime) override {
		Renderer2D::Ref().Color(Vec4(1.0f));
		Renderer2D::Ref().DrawRectRoundedWire(Vec2(0.0f), Vec2(1.0f), 0.2f);
	}
};

int main() {

	Application::CreateInst();

	Application::Ref().GetLayerStack().PushLayer(new SandboxLayer());

	Application::Ref().Run();

	Application::DestroyInst();

	return 0;
}