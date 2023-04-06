#include <iostream>
#include <ctime>
#include "Chat.h"


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
		std::cin.ignore();
		getline(std::cin, login);
		
		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr)
		{
			std::cout << "\n����������� ������ �����\n" << std::endl;
			std::cout << "������� ����� ������ ��� ���������� ����� ��� \"0\" ��� ������\n" << std::endl;
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
				std::cin >> choice;
			
				if (choice == '0')
				{
					menu = false;
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
		
		menu = false;
	}
}
//������� ��� ����������� ���� � ������� ����
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
//������� ���� � ������� �����, ����������� ��� ������ �� ���������, ���������� ������� ����
void Chat::showMainMenu()
{
	char choice;

	_currentUser = nullptr;

	std::cout << "\t������� ����\n" << std::endl;
	std::cout << "����: " << getDay() << "." << getMonth() << "." << getYear() << std::endl;
	std::cout << "�����: " << "(" << getHour() << ":" << getMin() << ")" << std::endl;
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

	for (auto& message : _messages) 
	{
		if (_currentUser->getUserLogin() == message.getFrom() || _currentUser->getUserLogin() == message.getTo() || message.getTo() == "all") 
		{
			from = (_currentUser->getUserLogin() == message.getFrom() ? "����" : getUserByLogin(message.getFrom())->getUserName());

			if (message.getTo() == "all") 
			{ 
				to = "����"; 
			}
			else 
			{ 
				to = _currentUser->getUserLogin() == message.getFrom() ? "���" : getUserByLogin(message.getTo())->getUserName(); 
			}

			std::cout << "��������� �� " << from << " ��� " << to <<": " << std::endl;
			std::cout << "\"" << message.getText() << "\"" << std::endl;
		}
	}
}
//���� ���� �� ���� ������������, ��� ������������ Admin �������� ������� ���������� ������ ���� �������������
void Chat::showChatMenu() 
{
	char choice;

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
		_messages.push_back(Message<std::string>(_currentUser->getUserLogin(), "all", text));
	}
	else 
	{
		_messages.push_back(Message<std::string>(_currentUser->getUserLogin(), getUserByName(to)->getUserLogin(), text));
	}
}
//������� ������������ Admin
void Chat::adminCreation()
{
	User user = User("admin", "admin", "Admin");
	_users.push_back(user);
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
