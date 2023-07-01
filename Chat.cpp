#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include "Chat.h"
#include "Utility.h"
#include "NetworkServer.h"


#if defined(max)
#undef max
#endif

void Chat::start()
{
	_chatWork = true;
}

bool Chat::chatWork() const
{
	return _chatWork;
}

std::shared_ptr<User> Chat::getUserByLogin(const std::string& login) const //�������� ������� ������������� �� ���������� ������
{
	for (auto& user : _users)
	{
		if (login == user.getUserLogin()) {
			return std::make_shared<User>(user);
		}
	}
	return nullptr;
}

std::shared_ptr<User> Chat::getUserByName(const std::string& name) const //�������� ������� ������������� �� ���������� �����
{
	for (auto& user : _users)
	{
		if (name == user.getUserName()) {
			return std::make_shared<User>(user);
		}
	}
	return nullptr;
}
//������� �������� ������ � ������ ��� ����� � ���
void Chat::showLoginMenu()
{
	bool menu = true;
	char choice;
	std::string login;
	std::string password;

	while (menu)
	{
		std::cout << "\t������� �����:\n" << std::endl;
		std::cin >> login;

		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr)
		{
			std::cout << "\n����������� ������ �����\n" << std::endl;
			std::cout << "������� ����� ������ ��� ���������� ����� ��� \"0\" ��� ������\n" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cin >> choice;

			if (choice == '0')
			{
				menu = false;
				break;
			}
		}
		else
		{
			std::cout << "\t������� ������:\n" << std::endl;
			std::cin >> password;

			if (password != _currentUser->getUserPassword())
			{
				std::cout << "\n����������� ������ ������\n" << std::endl;
				std::cout << "������� ����� ������ ��� ���������� ����� ��� \"0\" ��� ������\n" << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cin >> choice;

				if (choice == '0')
				{
					menu = false;
					_currentUser = nullptr;
					break;
				}

				_currentUser = nullptr;
				break;
			}
			break;
		}
	}
}
//������� �������� ������ ������������ ����������� ��������� �����, ������ � ��� ������������ � �������� � ����� ������� users
void Chat::showRegistrationMenu()
{
	bool menu = true;
	std::string login;
	std::string password;
	std::string name;

	std::cout << "\t����������� ������ ������������:\n" << std::endl;

	while (menu)
	{
		std::cout << "���������� �����:\n" << std::endl;
		std::cin >> login;

		if (getUserByLogin(login) || login == "all" || login == "All")
		{
			throw loginException();
		}

		std::cout << "���������� ������: \n" << std::endl;
		std::cin >> password;

		std::cout << "���������� ��� ������������: \n" << std::endl;
		std::cin >> name;

		if (getUserByName(name) || name == "all" || name == "All")
		{
			throw nameException();
		}

		User user = User(login, password, name);
		_users.push_back(user);
		
		saveUsersToFile();

		menu = false;
		break;
	}
}
//������� ���� � ������� �����, ����������� ��� ������ �� ���������, ���������� ������� ����
void Chat::showMainMenu()
{
	char choice;

	_currentUser = nullptr;

	std::cout << "\t������� ����\n" << std::endl;
	std::cout << "[" << getCurrentTime() << "]" << std::endl; //���� � ����� � ������� ����
	while(!_currentUser && _chatWork)
	{ 
		std::cout << "\n1 - ���� � ���\n2 - ����������� ������������\n3 - �����\n" << std::endl;
		std::cin >> choice;
		switch (choice) 
		{
		case('1'):
			showLoginMenu();
			break;
		case('2'):
			try 
			{
				showRegistrationMenu();
			}
			catch (std::exception& e)
			{
				std::cout << e.what();
			}
			break;
		case('3'):
			std::cout << "\n\t�����\n" << std::endl;
			_chatWork = false;
			system("pause");
			break;
		default:
			std::cout << "������� 1, 2 ��� 3" << std::endl;
			break;
		}
		
	}

}

//����� � ������� ��������� ��� ���� � ��� �������� ������������
void Chat::showChat()
{
	std::string from;
	std::string to;

	std::cout << "\t< ��� >\n" << std::endl;

	for (const auto& message : _messages) 
	{
		if (_currentUser->getUserLogin() != message.getFrom() && _currentUser->getUserLogin() != message.getTo() && message.getTo() != "all")
		{
			continue; // ���������� ���������, ���� ������� ������������ �� �������� ������������, ����������� ��� ��������� �� ���������� ����
		}

		from = (_currentUser->getUserLogin() == message.getFrom()) ? "����" : getUserByLogin(message.getFrom())->getUserName();

		if (message.getTo() == "all") 
		{ 
			to = "����"; 
		}
		else 
		{ 
			to = (_currentUser->getUserLogin() == message.getTo()) ? "���" : getUserByLogin(message.getTo())->getUserName(); 
		}

		std::cout << "��������� �� " << from << " ��� " << to << ": " << std::endl;
		std::cout << "\"" << "[" << message.getTime() << "] " << message.getText() << "\"" << std::endl;
	}
}
//���� ���� �� ���� ������������, ��� ������������ Admin �������� ������� ���������� ������ ���� �������������
void Chat::showChatMenu() 
{
	char choice;

	//client.receiveUpdatedUserFile(clientSocket_, "userFile.txt");

	std::cout << "\n������������: " << _currentUser->getUserName() << std::endl;

	while (_currentUser) 
	{
		if (_currentUser->getUserName() == "Admin") //������ ���� ��� ������
		{
			std::cout << "\n1 - �������� ���������\n2 - �������� ���������\n3 - �������� ���� �������������\n4 - (Admin) �������� ������ ���� �������������\n5 - ����� �� ����\n" << std::endl;

			std::cin >> choice;

			switch (choice)
			{
			case ('1'):
				showChat();
				break;
			case ('2'):
				addMessage();
				break;
			case ('3'):
				showAllUsers();
				break;
			case ('4'):
				showAllUsersInfo();
				break;
			case ('5'):
				std::cout << "\t�����\n" << std::endl;
				_currentUser = nullptr;
				break;
			}
		}
		else //������ ���� ��� �������� ������������
		{
			std::cout << "\n1 - �������� ���������\n2 - �������� ���������\n3 - �������� ���� �������������\n4 - ����� �� ����\n" << std::endl;

			std::cin >> choice;

			switch (choice)
			{
			case ('1'):
				showChat();
				break;
			case ('2'):
				addMessage();
				break;
			case ('3'):
				showAllUsers();
				break;
			case ('4'):
				std::cout << "\t�����\n" << std::endl;
				_currentUser = nullptr;
				break;
			}
		}
	}
}
//���������� ����� ���� �������������, "<- �" ��������� �� �������� ������������
void Chat::showAllUsers()
{
	std::cout << "\t������������ � ����:" << std::endl;

	for (auto& user : _users) 
	{
		std::cout << user.getUserName();
		if (user.getUserLogin() == _currentUser->getUserLogin()) 
		{
			std::cout << "<-- �";
		}
		std::cout << std::endl;
	}
}

//������� ��������� �������� ��������� ����������� ������������ ��� ����, ��������� ����������� � ����� ������� messages
void Chat::addMessage()
{
	std::string to;
	std::string text;
	std::string timeCurr = getCurrentTime();

	std::cout << "\n������� ��� ������������ ��� all - �������� ��������� ����:\n" << std::endl;
	std::cin >> to;
	std::cout << "\n�������� ����� ���������: \n" << std::endl;
	std::cin.ignore();
	getline(std::cin, text);

	if (!(to == "all" || getUserByName(to))) 
	{
		std::cout << "\n������������ �� ������\n" << std::endl;
		return;
	}
	if (to == "all") 
	{
		_messages.push_back(Message(_currentUser->getUserLogin(), "all", timeCurr, text));
	}
	else 
	{
		_messages.push_back(Message(_currentUser->getUserLogin(), getUserByName(to)->getUserLogin(), timeCurr, text));
	}
}
//������� ������������ Admin
void Chat::adminCreation()
{
	if (!getUserByLogin("admin")) {
		User user = User("admin", "admin", "Admin");
		_users.push_back(user);
	}
}
//������� � ������� ������ ���� �������������
void Chat::showAllUsersInfo()
{
	std::cout << "\t������������:" << std::endl;

	for (auto& user : Chat::_users)
	{
		std::cout << "�����: " << user.getUserLogin() << " / ";
		std::cout << "������: " << user.getUserPassword() << " / ";
		std::cout << "���: "<<user.getUserName();
		std::cout << std::endl;
	}
}
//�������� ������ ������������� �� �����
void Chat::loadUsersFromFile()
{
	std::ifstream userFileRead("userFile.txt", std::ios::in | std::ios::binary);
	if (userFileRead.is_open()) {
		std::string login, password, name;
		while (std::getline(userFileRead, login) && std::getline(userFileRead, password) && std::getline(userFileRead, name)) {
			User user(login, password, name);
			_users.emplace_back(user);
		}
		userFileRead.close();
	}
	else {
		std::cout << "�� ������� ������� userFile.txt" << std::endl;
	}
	SetFilePermissions("userFile.txt");
}
//���������� ������ ������������� � ����
void Chat::saveUsersToFile() const
{
	std::ofstream userFileWrite("userFile.txt", std::ios::out | std::ios::binary);
	if (userFileWrite.is_open()) {
		for (const auto& user : _users) {
			userFileWrite << user.getUserLogin() << std::endl;
			userFileWrite << user.getUserPassword() << std::endl;
			userFileWrite << user.getUserName() << std::endl;
		}
		userFileWrite.close();
	}
	else {
		std::cout << "�� ������� ������� userFile.txt ��� ������" << std::endl;
	}
	SetFilePermissions("userFile.txt");
}
//�������� ������ ��������� �� �����
void Chat::loadMessageFromFile()
{
	std::ifstream messageFileRead("messageFile.txt", std::ios::in | std::ios::binary);
	if (messageFileRead.is_open()) {
		std::string from, to, timeCurr, text;
		while (std::getline(messageFileRead, from) && std::getline(messageFileRead, to) && std::getline(messageFileRead, timeCurr) && std::getline(messageFileRead, text)) {
			Message message(from, to, timeCurr, text);
			_messages.emplace_back(message);
		}
		messageFileRead.close();
	}
	else {
		std::cout << "�� ������� ������� messageFile.txt" << std::endl;
	}
	SetFilePermissions("messageFile.txt");
}
//���������� ��������� � ����
void Chat::saveMessageToFile() const
{
	std::ofstream messageFileWrite("messageFile.txt", std::ios::out | std::ios::binary);
	if (messageFileWrite.is_open()) {
		for (const auto& message : _messages) {
			messageFileWrite << message.getFrom() << std::endl;
			messageFileWrite << message.getTo() << std::endl;
			messageFileWrite << message.getTime() << std::endl;
			messageFileWrite << message.getText() << std::endl;
		}
		messageFileWrite.close();
	}
	else {
		std::cout << "�� ������� ������� messageFile.txt ��� ������" << std::endl;
	}
	SetFilePermissions("messageFile.txt");
}

