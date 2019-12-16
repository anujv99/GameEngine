#pragma once

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

// --------------------------------- HANDLED ---------------------------------

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

// --------------------------------- SINGLETON ---------------------------------

#define DECLARE_SINGLE_OBJECT(X, ...)	private:\
											IF_OPENGL(static X * CreateOpenGL##X(__VA_ARGS__));\
											IF_DIRECTX(static X * CreateDirectX##X(__VA_ARGS__));

#define DEFINE_SINGLE_OBJECT(X)\
	template<>\
	inline void Singleton<X>::CreateInst() {\
		if (!s_Instance) {\
			switch (GraphicsContext::Ref().GetAPI()) {\
			case GraphicsAPI::API_OPENGL:\
			{\
				IFNOT_OPENGL(ASSERT(false, "Failed to create " #X ". Inavlid GraphicsAPI"));\
				IF_OPENGL(s_Instance = X::CreateOpenGL##X());\
				break;\
			}\
			case GraphicsAPI::API_DIRECTX:\
			{\
				IFNOT_DIRECTX(ASSERT(false, "Failed to create "#X". Inavlid GraphicsAPI"));\
				IF_DIRECTX(s_Instance = X::CreateDirectX##X());\
				break;\
			}\
			default:\
			{\
				IFNOT_DIRECTX(ASSERT(false, "Failed to create "#X". Inavlid GraphicsAPI"));\
				break;\
			}\
			}\
			\
		}\
	}

#define DEFINE_SINGLE_OBJECT_OPENGL(X) X * X::CreateOpenGL##X()
#define DEFINE_SINGLE_OBJECT_DIRECTX(X) X * X::CreateDirectX##X()