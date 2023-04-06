#pragma once
#include <string>
#include <vector>
#include <memory>
#include <exception>
#include "User.h"

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

class Chat
{
protected:
	bool _chatWork = false;
	std::vector<User> _users;
	std::vector<Message<std::string>> _messages;
	std::shared_ptr<User> _currentUser = nullptr;

	std::shared_ptr<User> getUserByLogin(const std::string& login) const;
	std::shared_ptr<User> getUserByName(const std::string& name) const;
	

public:
	void start();
	bool chatWork() const;
	
	void showMainMenu(); // ������� ����
	void showLoginMenu(); // ���� ����� ������������
	void showRegistrationMenu(); // ���� ����������� ������ ������������
	void showChat(); // ����� ���� � �������������� � �����������
	void showChatMenu(); //���� ����, � ������� ��������
	void showAllUsers(); //�������� ������ �������������
	void addMessage(); //�������� ���������

	void adminCreation(); //�������� ������������ admin
	void showAllUsersInfo(); //���������� ������ ���� �������������

	std::shared_ptr<User>getCurrentUser() const { return _currentUser; }
	
};


