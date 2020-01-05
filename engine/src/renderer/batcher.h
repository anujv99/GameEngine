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
		Vertex(const Vec2 & pos, VertexColor col) : Position(pos), Color(col) {}

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
		inline void Color(Vec4 col) { m_CurrentColor = VertexColor(col.x, col.y, col.z, col.w); }
		inline void Color(Vec3 col, float alpha) { m_CurrentColor = VertexColor(col.x, col.y, col.z, alpha); }

		void PolygonBegin(PrimitiveTopology topology);
		void PolygonEnd();

		void Vertex(const Vec2 & pos);
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
	public:
		void DrawLine(const Vec2 & start, const Vec2 & end);
		void DrawLineThick(const Vec2 & start, const Vec2 & end, pvfloat thickness);
		void DrawLineThickWire(const Vec2 & start, const Vec2 & end, pvfloat thickness);
		void DrawLineDashed(const Vec2 & start, const Vec2 & end, pvfloat stepSize);
		void DrawTriangle(const Vec2 & a, const Vec2 & b, const Vec2 & c);
		void DrawTriangleWire(const Vec2 & a, const Vec2 & b, const Vec2 & c);
		void DrawQuad(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d);
		void DrawQuadWire(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d);
		void DrawRect(const Vec2 & pos, const Vec2 & dimen);
		void DrawRectWire(const Vec2 & pos, const Vec2 & dimen);
		void DrawRectRounded(const Vec2 & pos, const Vec2 & dimen, pvfloat cornerRadius);
		void DrawRectRoundedWire(const Vec2 pos, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedTop(const Vec2 pos, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedTopWire(const Vec2 pos, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedBottom(const Vec2 pos, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedBottomWire(const Vec2 pos, const Vec2 dimen, float cornerRadius);
		//To Be Continued...
	private:
		std::vector<Vec2> DrawRoundRectHelper(float startAngle, float radius, Vec2 startPos);
	};

}