#pragma once

#include "Event.h"

class KeyEvent : public Event {

public:
	inline int GetKeyCode() const { return _keyCode; }
	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
		
protected:
	KeyEvent(int keyCode) : _keyCode(keyCode) {}
	int _keyCode;

};

class KeyPressedEvent : public KeyEvent {

public:
	KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), _repeatCount(repeatCount) {}
	inline int GetRepeatCount() { return _repeatCount; }
	std::string ToString() const override {
		std::stringstream sstream;
		sstream << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
		return sstream.str();
	}
	EVENT_CLASS_TYPE(KeyPressed)

private:
	int _repeatCount;

};

class KeyReleasedEvent : public KeyEvent {

public:
	KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}
	std::string ToString() const override {
		std::stringstream sstream;
		sstream << "KeyReleasedEvent: " << _keyCode;
		return sstream.str();
	}
	EVENT_CLASS_TYPE(KeyReleased)

};

class KeyTypedEvent : public KeyEvent {

public:
	KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {}
	std::string ToString() const override {
		std::stringstream sstream;
		sstream << "KeyTypedEvent: " << _keyCode;
		return sstream.str();
	}
	EVENT_CLASS_TYPE(KeyTyped)

};