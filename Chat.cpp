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
		std::cout << "\tВведите логин:\n" << std::endl;
		std::cin >> login;

		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr)
		{
			std::cout << "Неправильно введен логин" << std::endl;
			std::cout << "Нажмите любую кнопку для повторного ввода или \"0\" для выхода" << std::endl;
			std::cin >> choice;
			if (choice == '0')
			{
				menu = false;
				break;
			}
		}
		else
		{
			std::cout << "\tВведите пароль:\n" << std::endl;
			std::cin >> pass;

			if (pass != _currentUser->getUserPassword())
			{
				std::cout << "Неправильно введен пароль" << std::endl;
				std::cout << "Нажмите любую кнопку для повторного ввода или \"0\" для выхода" << std::endl;
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

	std::cout << "\tРегистрация нового пользователя:\n" << std::endl;

	while (menu)
	{
		std::cout << "Придумайте логин:\n" << std::endl;
		std::cin >> login;

		if (getUserByLogin(login) || login == "all" || login == "All")
		{
			throw loginException();
		}

		std::cout << "Придумайте пароль: \n" << std::endl;
		std::cin >> password;

		std::cout << "Придумайте имя пользователя: \n" << std::endl;
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

	std::cout << "\tГлавное Меню\n" << std::endl;

	while(!_currentUser && _chatWork)
	{ 
		std::cout << "1 - Вход в чат\n2 - Регистрация пользователя\n3 - Выход" << std::endl;
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
			std::cout << "\n\tВыход" << std::endl;
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

	std::cout << "\tЧат\n" << std::endl;

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

			std::cout << "Сообщение от " << from << " для " << to << std::endl;
			std::cout << "\"" << message.getText() << "\"" << std::endl;
		}
	}
}

void Chat::showChatMenu()
{
	char choice;

	std::cout << "Пользователь: " << _currentUser->getUserName() << std::endl;

	while (_currentUser) 
	{
		std::cout << "1 - Показать сообщения\n2 - Написать сообщение\n3 - Показать всех пользователей\n4 - Выйти из чата\n" << std::endl;
		
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
			std::cout << "\tВыход\n" << std::endl;
			_currentUser = nullptr;
			break;
		}
		
	}
}

void Chat::showAllUsers()
{
	std::cout << "\tПользователи в сети:" << std::endl;

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

	std::cout << "Введите имя пользователя или All - отпрвить сообщение Всем:" << std::endl;
	std::cin >> to;
	std::cout << "Напишите текст сообщения: " << std::endl;
	std::cin.ignore();
	getline(std::cin, text);

	if (!(to == "all" || to == "All" || getUserByName(to))) 
	{
		std::cout << "Пользователь не найден" << std::endl;
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

