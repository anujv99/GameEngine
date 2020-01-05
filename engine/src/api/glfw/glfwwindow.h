#pragma once

#include "core/window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace prev {

	class GLFWWindow : public Window {
	public:
		GLFWWindow();
		~GLFWWindow();
	public:
		virtual void Create(const WindowProps & props) override;
		virtual void Update() override;
		virtual void SetEventFunction(EventFunc eventFunc) override;

		virtual pvptr GetRawWindowPtr() override { return reinterpret_cast<pvptr>(m_WindowProps.Window); }

		virtual pvuint GetWidth() const override { return m_WindowProps.Width; };
		virtual pvuint GetHeight() const override { return m_WindowProps.Height; };
		virtual pvstring GetTitle() const override { return m_WindowProps.Title; };

		virtual void SetWidth(pvuint width) override;
		virtual void SetHeight(pvuint height) override;
		virtual void SetTitle(const pvstring & title) override;
	private:
		void SetCallbacks();
	public:
		struct GLFWWindowProps {
			pvuint Width, Height;
			pvstring Title;

			EventFunc EventFunction;
			::GLFWwindow * Window;
		};

		GLFWWindowProps m_WindowProps;
	};

}