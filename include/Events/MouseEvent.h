#pragma once

#include "Event.h"

class MouseMovedEvent : public Event {

public:
	MouseMovedEvent(float x, float y) : _mouseX(x), _mouseY(y) {}
	inline float GetX() const { return _mouseX; }
	inline float GetY() const { return _mouseY; }
	std::string ToString() const override {
		std::stringstream sstream;
		sstream << "MouseMovedEvent: " << _mouseX << ", " << _mouseY;
		return sstream.str();
	}
	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
	float _mouseX;
	float _mouseY;

};

class MouseScrolledEvent : public Event {

public:
	MouseScrolledEvent(float x, float y) : _xOffset(x), _yOffset(y) {}
	inline float GetXOffset() const { return _xOffset; }
	inline float GetYOffset() const { return _yOffset; }
	std::string ToString() const override {
		std::stringstream sstream;
		sstream << "MouseScrolledEvent: " << _xOffset << ", " << _yOffset;
		return sstream.str();
	}
	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
	float _xOffset;
	float _yOffset;

};

class MouseButtonEvent : public Event {

public:
	MouseButtonEvent(int mouseButton) : _mouseButton(mouseButton) {}
	inline int GetMouseButton() const { return _mouseButton; }
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

protected:
	int _mouseButton;

};

class MouseButtonPressedEvent : public MouseButtonEvent {

public:
	MouseButtonPressedEvent(int mouseButton) : MouseButtonEvent(mouseButton) {}
	std::string ToString() const override {
		std::stringstream sstream;
		sstream << "MouseButtonPressedEvent: " << _mouseButton;
		return sstream.str();
	}
	EVENT_CLASS_TYPE(MouseButtonPressed)

};

class MouseButtonReleasedEvent : public MouseButtonEvent {

public:
	MouseButtonReleasedEvent(int mouseButton) : MouseButtonEvent(mouseButton) {}
	inline int GetMouseButton() const { return _mouseButton; }
	std::string ToString() const override {
		std::stringstream sstream;
		sstream << "MouseButtonReleasedEvent: " << _mouseButton;
		return sstream.str();
	}
	EVENT_CLASS_TYPE(MouseButtonReleased)

};