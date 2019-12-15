#pragma once

#include "../common/handledobject.h"
#include "../common/singleton.h"
#include "../common/log.h"
#include "../common/types.h"

#ifdef ENGINE_OPENGL
	#define IF_OPENGL(X) X
	#define IFNOT_OPENGL(X)
#else
	#define IF_OPENGL(X)
	#define IFNOT_OPENGL(X) X
#endif

#ifdef ENGINE_DIRECTX
	#define IF_DIRECTX(X) X
	#define IFNOT_DIRECTX(X)
#else
	#define IF_DIRECTX(X)
	#define IFNOT_DIRECTX(X) X
#endif

#define DECLARE_OBJECT(X, ...)	static StrongHandle<X> Create(__VA_ARGS__);\
								private:\
									IF_OPENGL(static X * CreateOpenGL##X(__VA_ARGS__));\
									IF_DIRECTX(static X * CreateDirectX##X(__VA_ARGS__));

#define DEFINE_OBJECT_START(X, ...) StrongHandle<X> X::Create(__VA_ARGS__) {
#define DEFINE_OBJECT_BODY(X, ...)\
	switch (GraphicsContext::Ref().GetAPI()) {\
		case GraphicsAPI::API_OPENGL:\
		{\
			IFNOT_OPENGL(ASSERT(false, "Failed to create " #X ". Inavlid GraphicsAPI"));\
			IF_OPENGL(return X::CreateOpenGL##X(__VA_ARGS__));\
		}\
		case GraphicsAPI::API_DIRECTX:\
		{\
			IFNOT_DIRECTX(ASSERT(false, "Failed to create "#X". Inavlid GraphicsAPI"));\
			IF_DIRECTX(return X::CreateDirectX##X(__VA_ARGS__));\
		}\
		default:\
		{\
			IFNOT_DIRECTX(ASSERT(false, "Failed to create "#X". Inavlid GraphicsAPI"));\
			break;\
		}\
		}\
		\
		return nullptr;
#define DEFINE_OBJECT_END(X) }

#define DEFINE_OBJECT_OPENGL(X, ...) X * X::CreateOpenGL##X(__VA_ARGS__)
#define DEFINE_OBJECT_DIRECTX(X, ...) X * X::CreateDirectX##X(__VA_ARGS__)

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
