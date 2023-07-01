//�������� ���������� ����
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "User.h"
#include "Message.h"
#include "NetworkServer.h"
#include "NetworkClient.h"


class Chat
{
protected:
	bool _chatWork = false;
	std::vector<User> _users;
	std::vector<Message> _messages;
	std::shared_ptr<User> _currentUser = nullptr;	

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

	std::shared_ptr<User> getUserByLogin(const std::string& login) const; //�������� ������� ������������� �� ���������� ������
	std::shared_ptr<User> getUserByName(const std::string& name) const;  //�������� ������� ������������� �� ���������� �����


	void adminCreation(); //�������� ������������ admin, ���� ������ ������������ ���
	void showAllUsersInfo(); //���������� ������ ���� �������������

	std::shared_ptr<User>getCurrentUser() const { return _currentUser; }

	void loadUsersFromFile(); //�������� ������ ������������� �� �����
	void saveUsersToFile() const; //���������� ������ ������������� � ����

	void loadMessageFromFile(); //�������� ������ ��������� �� �����
	void saveMessageToFile() const; //���������� ��������� � ����

};


