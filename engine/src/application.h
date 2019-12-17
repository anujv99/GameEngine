#pragma once

#include "../src/common/singleton.h"
#include "../src/events/event.h"
#include "../src/events/applicationevent.h"
#include "../src/core/layerstack.h"

namespace prev {

	class Application : public Singleton<Application> {
		friend class Singleton<Application>;
	private:
		Application();
		virtual ~Application();
	public:
		void Run();
	
		inline LayerStack & GetLayerStack() { return m_LayerStack; }
	private:
		void OnEvent(Event & e);
		bool OnWindowClose(WindowCloseEvent & e);
	private:
		bool m_IsWindowOpen;
		LayerStack m_LayerStack;
	};

}
