#pragma once
#include <string>

class Message 
{
	const std::string _from;
	const std::string _to;
	const std::string _currentTime;
	const std::string _text;

public:
	Message(const std::string& from, const std::string& to, const std::string& currentTime, const std::string& text) : _from(from), _to(to), _currentTime(currentTime), _text(text){}
	~Message() = default;

	const std::string& getFrom() const { return _from; }
	const std::string& getTo() const { return _to; }
	const std::string& getTime() const { return _currentTime; }
	const std::string& getText() const { return _text; }
};

