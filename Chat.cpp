#include <iostream>
#include "Chat.h"

void Chat::start()
{
	_chatWork = true;
}

bool Chat::chatWork() const
{
	return _chatWork;
}

std::shared_ptr<User> Chat::getUserByLogin(const std::string& login) const
{
	for (auto& user : _users)
	{
		if (login == user.getUserLogin()) {
			return std::make_shared<User>(user);
		}
	}
	return nullptr;
}

std::shared_ptr<User> Chat::getUserByName(const std::string& name) const
{
	for (auto& user : _users)
	{
		if (name == user.getUserName()) {
			return std::make_shared<User>(user);
		}
	}
	return nullptr;
}

void Chat::showLoginMenu()
{
	bool menu = true;
	char choice;
	std::string login;
	std::string pass;
	while (menu)
	{
		std::cout << "\t������� �����:\n" << std::endl;
		std::cin >> login;

		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr)
		{
			std::cout << "����������� ������ �����" << std::endl;
			std::cout << "������� ����� ������ ��� ���������� ����� ��� \"0\" ��� ������" << std::endl;
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
			std::cin >> pass;

			if (pass != _currentUser->getUserPassword())
			{
				std::cout << "����������� ������ ������" << std::endl;
				std::cout << "������� ����� ������ ��� ���������� ����� ��� \"0\" ��� ������" << std::endl;
				std::cin >> choice;
				if (choice == '0')
				{
					menu = false;
					break;
				}
				break;
			}
			break;
		}
	}
}

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

void Chat::showMainMenu()
{
	char choice;

	_currentUser = nullptr;

	std::cout << "\t������� ����\n" << std::endl;

	while(!_currentUser && _chatWork)
	{ 
		std::cout << "1 - ���� � ���\n2 - ����������� ������������\n3 - �����" << std::endl;
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
			std::cout << "\n\t�����" << std::endl;
			_chatWork = false;
			system("pause");
			break;
		}
		
	}

}

void Chat::showChat()
{
	std::string from;
	std::string to;

	std::cout << "\t���\n" << std::endl;

	for (auto& message : _messages) 
	{
		if (_currentUser->getUserLogin() == message.getFrom() || _currentUser->getUserLogin() == message.getTo() || message.getTo() == "all") 
		{
			from = (_currentUser->getUserLogin() == message.getFrom() ? "me" : getUserByLogin(message.getFrom())->getUserName());

			if (message.getTo() == "all" || message.getTo() == "All") 
			{ 
				to = "All"; 
			}
			else 
			{ 
				to = _currentUser->getUserLogin() == message.getFrom() ? "me" : getUserByLogin(message.getTo())->getUserName(); 
			}

			std::cout << "��������� �� " << from << " ��� " << to << std::endl;
			std::cout << "\"" << message.getText() << "\"" << std::endl;
		}
	}
}

void Chat::showChatMenu()
{
	char choice;

	std::cout << "������������: " << _currentUser->getUserName() << std::endl;

	while (_currentUser) 
	{
		std::cout << "1 - �������� ���������\n2 - �������� ���������\n3 - �������� ���� �������������\n4 - ����� �� ����\n" << std::endl;
		
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

void Chat::showAllUsers()
{
	std::cout << "\t������������ � ����:" << std::endl;

	for (auto& user : _users) 
	{
		std::cout << user.getUserName();
		if (user.getUserLogin() == _currentUser->getUserLogin()) 
		{
			std::cout << "<--";
		}
		std::cout << std::endl;
	}
}

void Chat::addMessage()
{
	std::string to;
	std::string text;

	std::cout << "������� ��� ������������ ��� All - �������� ��������� ����:" << std::endl;
	std::cin >> to;
	std::cout << "�������� ����� ���������: " << std::endl;
	std::cin.ignore();
	getline(std::cin, text);

	if (!(to == "all" || to == "All" || getUserByName(to))) 
	{
		std::cout << "������������ �� ������" << std::endl;
		return;
	}
	if (to == "All" || to == "all") 
	{
		_messages.push_back(Message(_currentUser->getUserLogin(), "All", text));
	}
	else 
	{
		_messages.push_back(Message(_currentUser->getUserLogin(), getUserByName(to)->getUserLogin(), text));
	}
}

