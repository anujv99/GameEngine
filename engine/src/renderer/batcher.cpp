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

	void Batcher::Vertex(const Vec2 & pos) {
		ASSERTM(m_CurrentBatch != nullptr, "Forgot to call PolygonBegin");

		m_CurrentBatch->Vertices[m_CurrentBatch->Index].Reset(pos, m_CurrentColor);
		m_CurrentBatch->Index++;
	}

	void Batcher::FlushAll() {
		PV_PROFILE_FUNCTION();
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

	void Batcher::DrawLine(const Vec2 & start, const Vec2 & end) {
		PolygonBegin(PrimitiveTopology::TOPOLOGY_LINE);
		Vertex(start);
		Vertex(end);
		PolygonEnd();
	}

	void Batcher::DrawLineThick(const Vec2 & start, const Vec2 & end, pvfloat thickness) {
		Vec2 dir = end - start;
		dir = Vec2::Perpendicular(dir);
		dir = Vec2::Normalize(dir) * thickness * 0.5f;

		DrawQuad(start + dir, end + dir, end - dir, start - dir);
	}

	void Batcher::DrawLineThickWire(const Vec2 & start, const Vec2 & end, pvfloat thickness) {
		Vec2 dir = end - start;
		dir = Vec2::Perpendicular(dir);
		dir = Vec2::Normalize(dir) * thickness * 0.5f;

		DrawQuadWire(start + dir, end + dir, end - dir, start - dir);
	}

	void Batcher::DrawLineDashed(const Vec2 & start, const Vec2 & end, pvfloat stepSize) {
		const Vec2 diff = end - start;
		const pvfloat len = Vec2::Length(diff);

		if (len < 0.01f || stepSize < 0.01f) return;

		const Vec2 dir = diff / len;
		const Vec2 dir_step = dir * stepSize;
		const pvint num_steps = static_cast<pvint>(len / stepSize);

		PolygonBegin(PrimitiveTopology::TOPOLOGY_LINE);

		for (pvint i = 0; i < num_steps; i += 2) {
			pvfloat index = static_cast<pvfloat>(i);

			Vec2 pos0 = start + dir_step * index;
			Vec2 pos1 = start + dir_step * (index + 1.0f);

			Vertex(pos0);
			Vertex(pos1);
		}

		PolygonEnd();
	}

	void Batcher::DrawTriangle(const Vec2 & a, const Vec2 & b, const Vec2 & c) {
		PolygonBegin(PrimitiveTopology::TOPOLOGY_TRIANGLE);
		Vertex(a);
		Vertex(b);
		Vertex(c);
		PolygonEnd();
	}

	void Batcher::DrawTriangleWire(const Vec2 & a, const Vec2 & b, const Vec2 & c) {
		PolygonBegin(PrimitiveTopology::TOPOLOGY_LINE);
		Vertex(a);
		Vertex(b);
		Vertex(b);
		Vertex(c);
		Vertex(c);
		Vertex(a);
		PolygonEnd();
	}

	void Batcher::DrawQuad(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d) {
		PolygonBegin(PrimitiveTopology::TOPOLOGY_TRIANGLE);
		Vertex(a);
		Vertex(b);
		Vertex(c);
		Vertex(a);
		Vertex(c);
		Vertex(d);
		PolygonEnd();
	}

	void Batcher::DrawQuadWire(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d) {
		PolygonBegin(PrimitiveTopology::TOPOLOGY_LINE);
		Vertex(a);
		Vertex(b);
		Vertex(b);
		Vertex(c);
		Vertex(c);
		Vertex(d);
		Vertex(d);
		Vertex(a);
		PolygonEnd();
	}

}