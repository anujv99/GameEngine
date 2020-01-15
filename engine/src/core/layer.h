#pragma once

#include "../events/event.h"

#include "../common/types.h"
#include "../common/handledobject.h"

namespace prev {

	class Layer : public HandledObject<Layer> {
	public:
		Layer(const std::string & name = "Layer");
		virtual ~Layer();
	public:
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(pvfloat deltaTime) {}
		virtual void OnImGuiUpdate() {}
		virtual void OnEvent(Event & event) {}

		inline const std::string & GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};

}