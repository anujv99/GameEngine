#pragma once

#include "event.h"

namespace prev {

	class KeyEvent : public Event {
	public:
		inline pvint GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(pvint keyCode) :
			m_KeyCode(keyCode) {}

		pvint m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(pvint keycode, pvbool repeatCount) :
			KeyEvent(keycode), m_Repeat(repeatCount) {}

		inline pvbool IsRepeating() const { return m_Repeat; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_Repeat << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		pvbool m_Repeat;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(pvint keycode) :
			KeyEvent(keycode) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class CharacterEvent : public Event {
	public:
		CharacterEvent(pvuchar pressedChar) :
			m_PressedChar(pressedChar) {}

		inline pvuchar GetPressedChar() const { return m_PressedChar; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "CharacterEvent: " << m_PressedChar;
			return ss.str();
		}

		EVENT_CLASS_TYPE(CharacterInput)
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	private:
		pvuchar m_PressedChar;
	};

}