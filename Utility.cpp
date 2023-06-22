//Реализация вспомогательных методов
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem> //требуется С++17 и выше
#include "Utility.h"

namespace fs = std::filesystem;

//Установка разрешения доступа к файлам
void SetFilePermissions(const std::string& filePath)
{
	fs::perms permissions = fs::perms::owner_read | fs::perms::owner_write;

	fs::file_status status = fs::status(filePath);
	if (status.permissions() != permissions) {
		fs::permissions(filePath, permissions);
	}
}

//функции для отображения даты в главном меню
std::string getCurrentTime() 
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	int day = newtime.tm_mday;
	int month = 1 + newtime.tm_mon;
	int year = 1900 + newtime.tm_year;
	int hour = newtime.tm_hour;
	int min = newtime.tm_min;

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << day << "." << std::setw(2) << month << "." << year << " "
		<< std::setw(2) << hour << ":" << std::setw(2) << min;

	return oss.str();
}


int getDay()
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	int day = newtime.tm_mday;
	return day;
}
int getMonth()
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	int month = 1 + newtime.tm_mon;
	return month;
}
int getYear()
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	int year = 1900 + newtime.tm_year;
	return year;
}
int getHour()
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	int hour = newtime.tm_hour;
	return hour;
}
int getMin()
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	int min = newtime.tm_min;
	return min;
}
