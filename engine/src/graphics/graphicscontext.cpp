#include "graphicscontext.h"

namespace prev {

	void GraphicsContext::SetGraphicsAPI(GraphicsAPI api) {
		if (GraphicsContext::InstExists()) {
			LOG_ERROR("Graphics context already created. Please restart the engine to change GraphicsAPI");
			return;
		}

		IFNOT_OPENGL(if (api == GraphicsAPI::API_OPENGL) {
			LOG_ERROR("Unable to create OpenGL context. Please define ENGINE_OPENGL to use opengl");
			return;
		});

		#if !defined(ENGINE_WINDOWS)
			if (api == GraphicsAPI::API_DIRECTX) {
				LOG_ERROR("Unable to create DirectX context. DirectX is only available in windows os");
				return;
			}
		#endif

		IFNOT_DIRECTX(if (api == GraphicsAPI::API_DIRECTX) {
			LOG_ERROR("Unable to create DirectX context. Please define ENGINE_DIRECTX to use directx");
			return;
		});

		s_GraphicsAPI = api;
	}

}