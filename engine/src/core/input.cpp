#include "input.h"

namespace prev {

	bool Input::m_Keys[MAX_KEYS]							= {};
	bool Input::m_PrevKeys[MAX_KEYS]						= {};

	bool Input::m_MouseButtons[MAX_MOUSE_BUTTONS]			= {};
	bool Input::m_PrevMouseButtons[MAX_MOUSE_BUTTONS]		= {};

	Vec2 Input::m_MousePos									= Vec2(0.0f);
	Vec2 Input::m_PrevMousePos								= Vec2(0.0f);
	Vec2 Input::m_MouseScrollDelta							= Vec2(0.0f);

	std::vector<char> Input::m_CharacterPressedBuffer;

	bool Input::IsKeyDown(unsigned short keyCode) {
		return m_Keys[keyCode];
	}

	bool Input::IsKeyPressed(unsigned short keyCode) {
		return m_Keys[keyCode] && !m_PrevKeys[keyCode];
	}

	bool Input::IsKeyUp(unsigned short keyCode) {
		return !m_Keys[keyCode];
	}

	bool Input::IsKeyReleased(unsigned short keyCode) {
		return !m_Keys[keyCode] && m_PrevKeys[keyCode];
	}

	bool Input::IsMouseButtonDown(unsigned short keyCode) {
		return m_MouseButtons[keyCode];
	}

	bool Input::IsMouseButtonPressed(unsigned short keyCode) {
		return m_MouseButtons[keyCode] && !m_PrevMouseButtons[keyCode];
	}

	bool Input::IsMouseButtonUp(unsigned short keyCode) {
		return !m_MouseButtons[keyCode];
	}

	bool Input::IsMouseButtonReleased(unsigned short keyCode) {
		return !m_MouseButtons[keyCode] && m_PrevMouseButtons[keyCode];
	}

	bool Input::IsCharacterPressed() {
		return !m_CharacterPressedBuffer.empty();
	}

	void Input::OnEvent(Event & e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(Input::KeyPressed);
		dispatcher.Dispatch<KeyReleasedEvent>(Input::KeyReleased);
		dispatcher.Dispatch<MouseMovedEvent>(Input::MouseMoved);
		dispatcher.Dispatch<MouseScrolledEvent>(Input::MouseScrolled);
		dispatcher.Dispatch<MouseButtonPressedEvent>(Input::MousePressed);
		dispatcher.Dispatch<MouseButtonReleasedEvent>(Input::MouseReleased);
		dispatcher.Dispatch<CharacterEvent>(Input::CharacterPressed);
	}

	bool Input::KeyPressed(KeyPressedEvent & e) {
		m_Keys[e.GetKeyCode()] = true;
		return false;
	}

	bool Input::KeyReleased(KeyReleasedEvent & e) {
		m_Keys[e.GetKeyCode()] = false;
		return false;
	}

	bool Input::MouseMoved(MouseMovedEvent & e) {
		m_MousePos = Vec2(e.GetMousePosX(), e.GetMousePosY());
		return false;
	}

	bool Input::MouseScrolled(MouseScrolledEvent & e) {
		m_MouseScrollDelta = Vec2(e.GetScrollOffsetX(), e.GetScrollOffsetY());
		return false;
	}

	bool Input::MousePressed(MouseButtonPressedEvent & e) {
		m_MouseButtons[e.GetMouseButton()] = true;
		return false;
	}

	bool Input::MouseReleased(MouseButtonReleasedEvent & e) {
		m_MouseButtons[e.GetMouseButton()] = false;
		return false;
	}

	bool Input::CharacterPressed(CharacterEvent & e) {
		m_CharacterPressedBuffer.push_back(e.GetPressedChar());
		return false;
	}

	void Input::Update() {
		m_PrevMousePos = m_MousePos;
		m_MouseScrollDelta = Vec2(0);
		std::memcpy(m_PrevKeys, m_Keys, sizeof(m_Keys));
		std::memcpy(m_PrevMouseButtons, m_MouseButtons, sizeof(m_MouseButtons));

		m_CharacterPressedBuffer.clear();
		m_CharacterPressedBuffer.reserve(MAX_CHARACTER_PER_FRAME);
	}

}