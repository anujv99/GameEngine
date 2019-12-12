#pragma once

#include "../src/common/singleton.h"

namespace prev {

	class Application : public Singleton<Application> {
		friend class Singleton<Application>;
	private:
		Application();
		virtual ~Application();
	public:
		void Run();
	};

}
