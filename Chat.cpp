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

std::shared_ptr<User> Chat::getUserByLogin(const std::string& login) const //Проверка массива пользователей на совпадения логина
{
	for (auto& user : _users)
	{
		if (login == user.getUserLogin()) {
			return std::make_shared<User>(user);
		}
	}
	return nullptr;
}

std::shared_ptr<User> Chat::getUserByName(const std::string& name) const //Проверка массива пользователей на совпадение имени
{
	for (auto& user : _users)
	{
		if (name == user.getUserName()) {
			return std::make_shared<User>(user);
		}
	}
	return nullptr;
}
//Функция проверки логина и пароля при входе в чат
void Chat::showLoginMenu()
{
	bool menu = true;
	char choice;
	std::string login;
	std::string password;
	while (menu)
	{
		std::cout << "\tВведите логин:\n" << std::endl;
		std::cin.ignore();
		getline(std::cin, login);
		
		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr)
		{
			std::cout << "\nНеправильно введен логин\n" << std::endl;
			std::cout << "Нажмите любую кнопку для повторного ввода или \"0\" для выхода\n" << std::endl;
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
			
			std::cin >> password;

			if (password != _currentUser->getUserPassword())
			{
				std::cout << "\nНеправильно введен пароль\n" << std::endl;
				std::cout << "Нажмите любую кнопку для повторного ввода или \"0\" для выхода\n" << std::endl;
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
//Функция создания нового пользователя запрашивает придумать логин, пароль и имя пользователя и добавляе в конец вектора users
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
//функции для отображения даты в главном меню
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
//Главное меню с выбором входа, регистрации или выхода из программы, отображает текущую дату
void Chat::showMainMenu()
{
	char choice;

	_currentUser = nullptr;

	std::cout << "\tГлавное Меню\n" << std::endl;
	std::cout << "Дата: " << getDay() << "." << getMonth() << "." << getYear() << std::endl;
	std::cout << "Время: " << "(" << getHour() << ":" << getMin() << ")" << std::endl;
	while(!_currentUser && _chatWork)
	{ 
		std::cout << "\n1 - Вход в чат\n2 - Регистрация пользователя\n3 - Выход\n" << std::endl;
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
			std::cout << "\n\tВыход\n" << std::endl;
			_chatWork = false;
			system("pause");
			break;
		default:
			std::cout << "Введите 1, 2 или 3" << std::endl;
			break;
		}
		
	}

}

//Вывод в консоль сообщений для всех и для текущего пользователя
void Chat::showChat()
{
	std::string from;
	std::string to;

	std::cout << "\t< Чат >\n" << std::endl;

	for (auto& message : _messages) 
	{
		if (_currentUser->getUserLogin() == message.getFrom() || _currentUser->getUserLogin() == message.getTo() || message.getTo() == "all") 
		{
			from = (_currentUser->getUserLogin() == message.getFrom() ? "меня" : getUserByLogin(message.getFrom())->getUserName());

			if (message.getTo() == "all") 
			{ 
				to = "всех"; 
			}
			else 
			{ 
				to = _currentUser->getUserLogin() == message.getFrom() ? "мне" : getUserByLogin(message.getTo())->getUserName(); 
			}

			std::cout << "Сообщение от " << from << " для " << to <<": " << std::endl;
			std::cout << "\"" << message.getText() << "\"" << std::endl;
		}
	}
}
//Меню чата со всем функционалом, для пользователя Admin доступна функция посмотреть данные всех пользователей
void Chat::showChatMenu() 
{
	char choice;

	std::cout << "\nПользователь: " << _currentUser->getUserName() << std::endl;

	while (_currentUser) 
	{
		if (_currentUser->getUserName() == "Admin") //версия меню для админа
		{
			std::cout << "\n1 - Показать сообщения\n2 - Написать сообщение\n3 - Показать всех пользователей\n4 - (Admin) Показать данные всех пользователей\n5 - Выйти из чата\n" << std::endl;

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
				std::cout << "\tВыход\n" << std::endl;
				_currentUser = nullptr;
				break;
			}
		}
		else //версия меню для обычного пользователя
		{
			std::cout << "\n1 - Показать сообщения\n2 - Написать сообщение\n3 - Показать всех пользователей\n4 - Выйти из чата\n" << std::endl;

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
}
//Показывает имена всех пользователей, "<- Я" указывает на текущего пользователя
void Chat::showAllUsers()
{
	std::cout << "\tПользователи в сети:" << std::endl;

	for (auto& user : _users) 
	{
		std::cout << user.getUserName();
		if (user.getUserLogin() == _currentUser->getUserLogin()) 
		{
			std::cout << "<-- Я";
		}
		std::cout << std::endl;
	}
}

//Функция позволяет написать сообщение конкретному пользователю или всем, сообщение добавляется в конец вектора messages
void Chat::addMessage()
{
	std::string to;
	std::string text;

	std::cout << "\nВведите имя пользователя или all - отпрвить сообщение Всем:\n" << std::endl;
	std::cin >> to;
	std::cout << "\nНапишите текст сообщения: \n" << std::endl;
	std::cin.ignore();
	getline(std::cin, text);

	if (!(to == "all" || getUserByName(to))) 
	{
		std::cout << "\nПользователь не найден\n" << std::endl;
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
//Создает пользователя Admin
void Chat::adminCreation()
{
	User user = User("admin", "admin", "Admin");
	_users.push_back(user);
}
//Выводит в консоль данные всех пользователей
void Chat::showAllUsersInfo()
{
	std::cout << "\tПользователи:" << std::endl;

	for (auto& user : Chat::_users)
	{
		std::cout << "Логин: " << user.getUserLogin() << " / ";
		std::cout << "Пароль: " << user.getUserPassword() << " / ";
		std::cout << "Имя: "<<user.getUserName();
		std::cout << std::endl;
	}
}
