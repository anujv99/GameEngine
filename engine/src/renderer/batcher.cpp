#include "batcher.h"

namespace prev {

	Batcher::Batcher(pvsizet maxVertices) :
		m_Batches(),
		m_MaxVertices(maxVertices),
		m_NumVertices(0u), m_CurrentColor(1.0f),
		m_CurrentBatch(nullptr) {}

	Batcher::~Batcher() {}

	void Batcher::PolygonBegin(PrimitiveTopology topology) {
		StrongHandle<Batch> batch = GetBatch(topology);
		ASSERT(batch != nullptr);
		m_CurrentBatch = batch;
	}

	void Batcher::PolygonEnd() {
		m_CurrentBatch = nullptr;
	}

	void Batcher::Vertex(Vec2 pos) {
		ASSERTM(m_CurrentBatch != nullptr, "Forgot to call PolygonBegin");

		m_CurrentBatch->Vertices[m_CurrentBatch->Index].Reset(pos, m_CurrentColor);
		m_CurrentBatch->Index++;
	}

	void Batcher::FlushAll() {
		ASSERTM(m_CurrentBatch == nullptr, "Forgot to call PolygonEnd");
		for (auto & batch : m_Batches) {
			if (batch.second->GetNumElements() == 0) continue;
			Flush(batch.first, batch.second);
			batch.second->Index = 0;
		}
	}

	StrongHandle<Batcher::Batch> Batcher::GetBatch(PrimitiveTopology topology) {
		auto batch = m_Batches.find(topology);
		if (batch == m_Batches.end()) {
			StrongHandle<Batch> newBatch = new Batch(m_MaxVertices);
			m_Batches.insert(std::make_pair(topology, newBatch));
			return newBatch;
		}
		return batch->second;
	}

}