
#include <prevengine.h>

using namespace prev;

class SandboxLayer : public Layer {
	virtual void OnUpdate(pvfloat deltaTime) override {
		Renderer2D::Ref().Color(1.0f, 0.0f, 0.0f, 1.0f);
		Renderer2D::Ref().DrawLineThick(Vec2(-0.5f), Vec2(0.5f), 0.02f);
		Renderer2D::Ref().Color(0.0f, 1.0f, 0.0f, 1.0f);
		Renderer2D::Ref().DrawTriangleWire(Vec2(0.0f, 0.5f), Vec2(0.5f, -0.5f), Vec2(-0.5f, -0.5f));
	}
};

int main() {

	Application::CreateInst();

	Application::Ref().GetLayerStack().PushLayer(new SandboxLayer());

	Application::Ref().Run();
	
	Application::DestroyInst();

	return 0;
}