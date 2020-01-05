#pragma once

#include "../math/math.h"

#include "../events/event.h"
#include "../events/keyevent.h"
#include "../events/mouseevent.h"

#include "pvkeycodes.h" // Contains defines for buttons

#define MAX_KEYS					0xFFF
#define MAX_MOUSE_BUTTONS			5
#define MAX_CHARACTER_PER_FRAME		16

namespace prev {

	class Input {
		friend class Application;
	public:
		static bool IsKeyDown(unsigned short keyCode);
		static bool IsKeyPressed(unsigned short keyCode);
		static bool IsKeyUp(unsigned short keyCode);
		static bool IsKeyReleased(unsigned short keyCode);

		static bool IsMouseButtonDown(unsigned short keyCode);
		static bool IsMouseButtonPressed(unsigned short keyCode);
		static bool IsMouseButtonUp(unsigned short keyCode);
		static bool IsMouseButtonReleased(unsigned short keyCode);

		static bool IsCharacterPressed();

		static inline const std::vector<char> & GetPressedCharacterBuffer() { return m_CharacterPressedBuffer; }

		static inline Vec2 GetMousePosition() { return m_MousePos; }
		static inline Vec2 GetMouseDeltaPosition() { return m_MousePos - m_PrevMousePos; }
		static inline Vec2 GetMouseScrollDelta() { return m_MouseScrollDelta; }
	private:
		static void OnEvent(Event & e);
		static bool KeyPressed(KeyPressedEvent & e);
		static bool KeyReleased(KeyReleasedEvent & e);
		static bool MouseMoved(MouseMovedEvent & e);
		static bool MouseScrolled(MouseScrolledEvent & e);
		static bool MousePressed(MouseButtonPressedEvent & e);
		static bool MouseReleased(MouseButtonReleasedEvent & e);
		static bool CharacterPressed(CharacterEvent & e);

		static void Update();
	private:
		static bool m_Keys[MAX_KEYS];
		static bool m_PrevKeys[MAX_KEYS]; //Previous Key States

		static bool m_MouseButtons[MAX_MOUSE_BUTTONS];
		static bool m_PrevMouseButtons[MAX_MOUSE_BUTTONS]; //Previous Key States

		static Vec2 m_MousePos;
		static Vec2 m_PrevMousePos;
		static Vec2 m_MouseScrollDelta;

		static std::vector<char> m_CharacterPressedBuffer;
	};

}
