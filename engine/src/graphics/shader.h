#pragma once

#include "../common/handledobject.h"

#include "graphicscontext.h"

namespace prev {

	// ------------- VERTEX SHADER -------------

	class VertexShader : public HandledObject<VertexShader> {
		friend class StrongHandle<VertexShader>;
	protected:
		VertexShader() {}
		virtual ~VertexShader() {}
	public:
		DECLARE_OBJECT(VertexShader, const pvstring & source);
	};

	// ------------- FRAGMENT SHADER -------------

	class FragmentShader : public HandledObject<FragmentShader> {
		friend class StrongHandle<FragmentShader>;
	protected:
		FragmentShader() {}
		virtual ~FragmentShader() {}
	public:
		DECLARE_OBJECT(FragmentShader, const pvstring & source);
	};

	// ------------- SHADER PROGRAM -------------

	class ShaderProgram : public HandledObject<ShaderProgram> {
		friend class StrongHandle<ShaderProgram>;
	protected:
		ShaderProgram() {}
		virtual ~ShaderProgram() {}
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
	public:
		DECLARE_OBJECT(ShaderProgram, StrongHandle<VertexShader> vShader, StrongHandle<FragmentShader> fShader);
	};

}