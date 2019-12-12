#pragma once

#include "event.h"

namespace prev {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(pvint x, pvint y) :
			m_MousePosX(x), m_MousePosY(y) {}

		inline pvint GetMousePosX() const { return m_MousePosX; }
		inline pvint GetMousePosY() const { return m_MousePosY; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MousePosX << ", " << m_MousePosY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		pvint m_MousePosX, m_MousePosY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(pvint xOffset, pvint yOffset) :
			m_ScrollOffsetX(xOffset), m_ScrollOffsetY(yOffset) {}

		inline pvint GetScrollOffsetX() const { return m_ScrollOffsetX; }
		inline pvint GetScrollOffsetY() const { return m_ScrollOffsetY; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_ScrollOffsetX << ", " << m_ScrollOffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		pvint m_ScrollOffsetX, m_ScrollOffsetY;
	};

	class MouseButtonEvent : public Event {
	public:
		inline pvint GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(pvint button) :
			m_Button(button) {}

		pvint m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(pvint button) :
			MouseButtonEvent(button) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(pvint button) :
			MouseButtonEvent(button) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}
