#pragma once
#include <string>

template<typename T>class Message //Шаблонный класс
{
	const T _from;
	const T _to;
	const T _text;

public:
	Message(const T& from, const T& to, const T& text) : _from(from), _to(to), _text(text){}
	~Message() = default;

	const T& getFrom() const { return _from; }
	const T& getTo() const { return _to; }
	const T& getText() const { return _text; }
};

