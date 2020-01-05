#pragma once

#include "../src/common/singleton.h"
#include "../src/events/event.h"
#include "../src/events/applicationevent.h"
#include "../src/core/layerstack.h"
#include "../src/core/input.h"
#include "../src/renderer/orthocamera.h"

#include "../src/memory/memoryleak.h"

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
		void PreUpdate();
		void Render();
		void GUI();
		void PostUpdate();

		void OnEvent(Event & e);
		bool OnWindowClose(WindowCloseEvent & e);
		bool OnWindowResize(WindowResizeEvent & e);
	private:
		bool m_IsWindowOpen;
		LayerStack m_LayerStack;
		StrongHandle<OrthoCamera> m_Camera;
	};

}
