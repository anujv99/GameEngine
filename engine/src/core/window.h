#pragma once

#include <functional>

#include "../common/types.h"
#include "../common/singleton.h"

#include "../events/event.h"

namespace prev {

	typedef std::function<void(Event & e)> EventFunc;

	class WindowProps {
	public:
		WindowProps(pvint width, pvint height, const pvstring & title) : m_Width(width), m_Height(height), m_Title(title) {}
		~WindowProps() {}
	public:
		inline pvint GetWidth() const { return m_Width; }
		inline pvint GetHeight() const { return m_Height; }

		inline const pvstring & GetTitle() const { return m_Title; }
	private:
		pvint m_Width, m_Height;
		pvstring m_Title;
	};

	class Window : public Singleton<Window> {
		friend class Singleton<Window>;
	protected:
		Window() {}
		virtual ~Window() {}
	public:
		virtual void Create(const WindowProps & props) = 0;
		virtual void Update() = 0;
		virtual void SetEventFunction(EventFunc eventFunc) = 0;
		virtual pvptr GetRawWindowPtr() = 0;
	private:
		static Window * CreateWindow();
	};

	template<>
	inline void Singleton<Window>::CreateInst() {
		if (!s_Instance) s_Instance = Window::CreateWindow();
	}

}