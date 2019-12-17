
#include <prevengine.h>

using namespace prev;

class SandboxLayer : public Layer {
	virtual void OnUpdate(pvfloat deltaTime) override {
		Renderer2D::Ref().Color(1.0f, 0.0f, 0.0f, 1.0f);
		Renderer2D::Ref().PolygonBegin(PrimitiveTopology::TOPOLOGY_TRIANGLE);
		Renderer2D::Ref().Vertex(Vec2(0.0f, 0.5f));
		Renderer2D::Ref().Vertex(Vec2(0.5f,-0.5f));
		Renderer2D::Ref().Vertex(Vec2(-0.5f, -0.5f));
		Renderer2D::Ref().PolygonEnd();
	}
};

int main() {

	Application::CreateInst();

	Application::Ref().GetLayerStack().PushLayer(new SandboxLayer());

	Application::Ref().Run();
	
	Application::DestroyInst();

	return 0;
}