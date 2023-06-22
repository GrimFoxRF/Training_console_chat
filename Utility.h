//��������������� ������
#pragma once
#include <exception>
#include <string>

//���������� �������� ����� � ���
class loginException : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "������: ����� �����";
	}
};
class nameException : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "������: ��� ������";
	}
};

void SetFilePermissions(const std::string& filePath); //��������� ���������� ������� � ������

//��������� ������ ��� ��������� ���� � ������� �������
std::string getCurrentTime();
int getDay();
int getMonth();
int getYear();
int getHour();
int getMin();