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
		std::cin >> login;

		_currentUser = getUserByLogin(login);

		if (_currentUser == nullptr)
		{
			std::cout << "\nНеправильно введен логин\n" << std::endl;
			std::cout << "Нажмите любую кнопку для повторного ввода или \"0\" для выхода\n" << std::endl;
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
			std::cout << "\tВведите пароль:\n" << std::endl;
			std::cin >> password;

			if (password != _currentUser->getUserPassword())
			{
				std::cout << "\nНеправильно введен пароль\n" << std::endl;
				std::cout << "Нажмите любую кнопку для повторного ввода или \"0\" для выхода\n" << std::endl;
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
		
		saveUsersToFile();

		menu = false;
		break;
	}
}
//Главное меню с выбором входа, регистрации или выхода из программы, отображает текущую дату
void Chat::showMainMenu()
{
	char choice;

	_currentUser = nullptr;

	std::cout << "\tГлавное Меню\n" << std::endl;
	std::cout << "[" << getCurrentTime() << "]" << std::endl; //Дата и время в главном меню
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

	for (const auto& message : _messages) 
	{
		if (_currentUser->getUserLogin() != message.getFrom() && _currentUser->getUserLogin() != message.getTo() && message.getTo() != "all")
		{
			continue; // Пропустить сообщение, если текущий пользователь не является отправителем, получателем или сообщение не отправлено всем
		}

		from = (_currentUser->getUserLogin() == message.getFrom()) ? "меня" : getUserByLogin(message.getFrom())->getUserName();

		if (message.getTo() == "all") 
		{ 
			to = "всех"; 
		}
		else 
		{ 
			to = (_currentUser->getUserLogin() == message.getTo()) ? "мне" : getUserByLogin(message.getTo())->getUserName(); 
		}

		std::cout << "Сообщение от " << from << " для " << to << ": " << std::endl;
		std::cout << "\"" << "[" << message.getTime() << "] " << message.getText() << "\"" << std::endl;
	}
}
//Меню чата со всем функционалом, для пользователя Admin доступна функция посмотреть данные всех пользователей
void Chat::showChatMenu() 
{
	char choice;

	//client.receiveUpdatedUserFile(clientSocket_, "userFile.txt");

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
	std::string timeCurr = getCurrentTime();

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
		_messages.push_back(Message(_currentUser->getUserLogin(), "all", timeCurr, text));
	}
	else 
	{
		_messages.push_back(Message(_currentUser->getUserLogin(), getUserByName(to)->getUserLogin(), timeCurr, text));
	}
}
//Создает пользователя Admin
void Chat::adminCreation()
{
	if (!getUserByLogin("admin")) {
		User user = User("admin", "admin", "Admin");
		_users.push_back(user);
	}
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
//Загрузка списка пользователей из файла
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
		std::cout << "Не удалось открыть userFile.txt" << std::endl;
	}
	SetFilePermissions("userFile.txt");
}
//Сохранение списка пользователей в файл
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
		std::cout << "Не удалось открыть userFile.txt для записи" << std::endl;
	}
	SetFilePermissions("userFile.txt");
}
//Звгрузка списка сообщений из файла
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
		std::cout << "Не удалось открыть messageFile.txt" << std::endl;
	}
	SetFilePermissions("messageFile.txt");
}
//Сохранение сообщений в файл
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
		std::cout << "Не удалось открыть messageFile.txt для записи" << std::endl;
	}
	SetFilePermissions("messageFile.txt");
}

