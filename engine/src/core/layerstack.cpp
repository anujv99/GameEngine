#include "layerstack.h"

namespace prev {

	LayerStack::~LayerStack() {
		m_Layers.clear();
	}

	void LayerStack::PushLayer(StrongHandle<Layer> layer) {
		if (layer == nullptr) {
			LOG_ERROR("Unable to push layer in layer stack. Invalid layer");
			return;
		}

		std::list<StrongHandle<Layer>>::iterator it = m_Layers.begin();
		std::advance(it, m_LastLayerIndex);
		m_LastLayerIndex++;

		m_Layers.insert(it, layer);
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(StrongHandle<Layer> layer) {
		if (layer == nullptr) {
			LOG_ERROR("Unable to push overlay in layer stack. Invalid layer");
			return;
		}

		m_Layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(StrongHandle<Layer> layer) {
		if (layer == nullptr) {
			LOG_WARN("Unable to pop layer from layer stack. Invalid layer");
			return;
		}

		for (pvuint i = 0; i < m_LastLayerIndex; i++) {
			auto it = m_Layers.begin();
			std::advance(it, i);

			if ((*it) == layer) {
				(*it)->OnDetach();
				m_Layers.erase(it);
				m_LastLayerIndex--;
				return;
			}
		}

		LOG_ERROR("Unable to pop layer from layer stack. Layer with address \'%p\' not found", (void *)layer.Get());
	}

	void LayerStack::PopOverlay(StrongHandle<Layer> layer) {
		if (layer == nullptr) {
			LOG_WARN("Unable to pop overlay from layer stack. Invalid layer");
			return;
		}

		for (pvuint i = m_LastLayerIndex; i < m_Layers.size(); i++) {
			auto it = m_Layers.begin();
			std::advance(it, i);

			if ((*it) == layer) {
				(*it)->OnDetach();
				m_Layers.erase(it);
				m_LastLayerIndex--;
				return;
			}
		}

		LOG_ERROR("Unable to pop overlay from layer stack. Layer with address \'%p\' not found", (void *)layer.Get());
	}

}