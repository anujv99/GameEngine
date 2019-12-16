#pragma once

#include "../common/handledobject.h"
#include "../common/singleton.h"
#include "../common/log.h"
#include "../common/types.h"

#include "helper.h"

namespace prev {

	enum class GraphicsAPI {
		API_OPENGL,
		API_DIRECTX,
	};

	class GraphicsContext : public Singleton<GraphicsContext> {
		friend class Singleton<GraphicsContext>;
	protected:
		GraphicsContext() {}
		virtual ~GraphicsContext() {}
	public:
		virtual void CreateContext(pvptr rawWindowPtr) = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
	public:
		static void SetGraphicsAPI(GraphicsAPI api);
		static GraphicsAPI GetAPI() { return s_GraphicsAPI; }
	private:
		IF_OPENGL(static GraphicsContext * CreateOpenGLContext());
		IF_DIRECTX(static GraphicsContext * CreateDirectXContext());
	private:
		static GraphicsAPI s_GraphicsAPI;
	};

	template<>
	inline void Singleton<GraphicsContext>::CreateInst() {
		if (InstExists()) {
			LOG_ERROR("Graphics context already created.");
			return;
		}

		IF_OPENGL(
			if (GraphicsContext::s_GraphicsAPI == GraphicsAPI::API_OPENGL) {
				s_Instance = GraphicsContext::CreateOpenGLContext();
				return;
			}
		);
		IF_DIRECTX(
			if (GraphicsContext::s_GraphicsAPI == GraphicsAPI::API_DIRECTX) {
				s_Instance = GraphicsContext::CreateDirectXContext();
				return;
			}
		)
	}

}
