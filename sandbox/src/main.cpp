
#include <prevengine.h>

int main() {

	prev::Application::CreateInst();

	prev::Application::Ref().Run();

	prev::Application::DestroyInst();

	return 0;
}