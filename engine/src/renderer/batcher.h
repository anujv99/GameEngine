#pragma once

#include <unordered_map>
#include <vector>

#include "../math/math.h"
#include "../graphics/renderstate.h"
#include "../common/handledobject.h"

namespace prev {

	struct VertexColor {
		pvubyte r, g, b, a;
		VertexColor() : r(0), g(0), b(0), a(0) {}
		VertexColor(pvubyte val) : r(val), g(val), b(val), a(val) {}
		VertexColor(pvubyte red, pvubyte green, pvubyte blue, pvubyte alpha) :
			r(red), g(green), b(blue), a(alpha) {
		}
		VertexColor(pvfloat red, pvfloat green, pvfloat blue, pvfloat alpha) : r(0), g(0), b(0), a(0) {
			r = static_cast<pvubyte>(red	* 255.0f);
			g = static_cast<pvubyte>(green	* 255.0f);
			b = static_cast<pvubyte>(blue	* 255.0f);
			a = static_cast<pvubyte>(alpha	* 255.0f);
		}
		VertexColor(pvfloat val) : r(0), g(0), b(0), a(0) {
			r = static_cast<pvubyte>(val * 255.0f);
			g = static_cast<pvubyte>(val * 255.0f);
			b = static_cast<pvubyte>(val * 255.0f);
			a = static_cast<pvubyte>(val * 255.0f);
		}
	};

	struct Vertex {
		Vertex() : Position(0.0f), Color() {}
		Vertex(Vec2 pos, VertexColor col) : Position(pos), Color(col) {}

		inline void Reset(const Vec2 & pos, const VertexColor & color) { Position = pos; Color = color; }

		Vec2 Position;
		VertexColor Color;
	};

	class Batcher {
	protected:
		Batcher(pvsizet maxVertices);
		~Batcher();
	public:
		inline void Color(VertexColor color) { m_CurrentColor = color; }
		inline void Color(pvubyte color) { m_CurrentColor = VertexColor(color); }
		inline void Color(pvfloat color) { m_CurrentColor = VertexColor(color); }
		inline void Color(pvubyte r, pvubyte g, pvubyte b, pvubyte a) { m_CurrentColor = VertexColor(r, g, b, a); }
		inline void Color(pvfloat r, pvfloat g, pvfloat b, pvfloat a) { m_CurrentColor = VertexColor(r, g, b, a); }

		void PolygonBegin(PrimitiveTopology topology);
		void PolygonEnd();

		void Vertex(Vec2 pos);
	protected:
		struct Batch : HandledObject<Batch> {
			friend class Batcher;
			friend class StrongHandle<Batch>;
			
			inline const void * GetData() const { return reinterpret_cast<const void *>(Vertices.data()); }
			inline pvsizet GetDataSize() const { return Index * sizeof(prev::Vertex); }
			inline pvsizet GetNumElements() const { return Index; }
		private:
			Batch(pvsizet maxSize) : Vertices(), Index(0) { Vertices.resize(maxSize); }
		private:
			std::vector<prev::Vertex> Vertices;
			pvsizet Index;
		};

		virtual void Flush(PrimitiveTopology prim, const StrongHandle<Batch> batch) = 0;

		void FlushAll();
	private:
		StrongHandle<Batch> GetBatch(PrimitiveTopology topology);
	private:
		std::unordered_map<PrimitiveTopology, StrongHandle<Batch>> m_Batches;
		pvsizet m_MaxVertices;
		pvsizet m_NumVertices;
		VertexColor m_CurrentColor;
		StrongHandle<Batch> m_CurrentBatch;
	};

}