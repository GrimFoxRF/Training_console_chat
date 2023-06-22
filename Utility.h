//Вспомогательные методы
#pragma once
#include <exception>
#include <string>

//Исключения неверный логин и имя
class loginException : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "ОШИБКА: логин занят";
	}
};
class nameException : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "ОШИБКА: имя занято";
	}
};

void SetFilePermissions(const std::string& filePath); //Установка разрешения доступа к файлам

//Служебные методы для получения даты и времени системы
std::string getCurrentTime();
int getDay();
int getMonth();
int getYear();
int getHour();
int getMin();