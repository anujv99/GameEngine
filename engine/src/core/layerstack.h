#pragma once

#include <list>

#include "layer.h"

#include "../common/stronghandle.h"
#include "../common/types.h"

namespace prev {

	class LayerStack : public HandledObject<LayerStack> {
	public:
		LayerStack() = default;
		~LayerStack();
	public:
		void PushLayer(StrongHandle<Layer> layer);
		void PushOverlay(StrongHandle<Layer> layer);
		void PopLayer(StrongHandle<Layer> layer);
		void PopOverlay(StrongHandle<Layer> layer);

		std::list<StrongHandle<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::list<StrongHandle<Layer>>::iterator end() { return m_Layers.end(); }
		std::list<StrongHandle<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::list<StrongHandle<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

		std::list<StrongHandle<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
		std::list<StrongHandle<Layer>>::const_iterator end()	const { return m_Layers.end(); }
		std::list<StrongHandle<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::list<StrongHandle<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::list<StrongHandle<Layer>> m_Layers;
		pvuint m_LastLayerIndex = 0;
	};

}