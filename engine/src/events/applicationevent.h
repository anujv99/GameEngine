#pragma once

#include <sstream>
#include <functional>

#include "event.h"

namespace prev {

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(pvint width, pvint height) :
			m_WindowSizeX(width), m_WindowSizeY(height) {}
		inline pvint GetWindowSizeX() const { return m_WindowSizeX; };
		inline pvint GetWindowSizeY() const { return m_WindowSizeY; };
		
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_WindowSizeX << ", " << m_WindowSizeY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		pvint m_WindowSizeX, m_WindowSizeY;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowMoveEvent : public Event {
	public:
		WindowMoveEvent(pvint xpos, pvint ypos) :
			m_WindowPosX(xpos), m_WindowPosY(ypos) {}

		inline pvint GetWindowPosX() const { return m_WindowPosX; }
		inline pvint GetWindowPosY() const { return m_WindowPosY; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_WindowPosX << ", " << m_WindowPosY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		pvint m_WindowPosX, m_WindowPosY;
	};

	class AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}