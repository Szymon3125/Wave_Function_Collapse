#pragma once

class DeltaTime {

public:
	DeltaTime(float time = 0.0f) : _time(time) {}

	float GetSeconds() const { return _time; }
	float GetMiliseconds() const { return _time * 1000.0f; }

	operator float() const { return _time; }

private:
	// licaba sekund
	float _time;

};
