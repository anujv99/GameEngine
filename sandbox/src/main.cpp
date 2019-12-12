
#include <prevengine.h>

int main() {

	prev::Application::CreateInst();

	prev::Application::Ref().Run();

	LOG_TRACE("Hello Sandbox");

	prev::Application::DestroyInst();

	return 0;
}