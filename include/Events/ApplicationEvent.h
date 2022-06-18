#pragma once

#include "Event.h"

class WindowResizeEvent : public Event {

public:
	WindowResizeEvent(unsigned int width, unsigned int height) : _width(width), _height(height) {}
	inline unsigned int GetWidth() { return _width; }
	inline unsigned int GetHeight() { return _height; }
	std::string ToString() const override {
		std::stringstream sstream;
		sstream << "WindowResizeEvent: " << _width << ", " << _height;
		return sstream.str();
	}
	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	unsigned int _width;
	unsigned int _height;

};

class WindowCloseEvent : public Event {

public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

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