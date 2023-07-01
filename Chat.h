//Основной функционал чата
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
	
	void showMainMenu(); // Главное меню
	void showLoginMenu(); // Меню входа пользователя
	void showRegistrationMenu(); // Меню регистрации нового пользователя
	void showChat(); // Лента чата с пользователями и сообщениями
	void showChatMenu(); //Меню чата, с выбором действий
	void showAllUsers(); //Показать список пользователей
	void addMessage(); //Добавить сообщение

	std::shared_ptr<User> getUserByLogin(const std::string& login) const; //Проверка массива пользователей на совпадение логина
	std::shared_ptr<User> getUserByName(const std::string& name) const;  //Проверка массива пользователей на совпадение имени


	void adminCreation(); //Создание пользователя admin, если такого пользователя нет
	void showAllUsersInfo(); //Посмотреть данные всех пользователей

	std::shared_ptr<User>getCurrentUser() const { return _currentUser; }

	void loadUsersFromFile(); //Загрузка списка пользователей из файла
	void saveUsersToFile() const; //Сохранение списка пользователей в файл

	void loadMessageFromFile(); //Звгрузка списка сообщений из файла
	void saveMessageToFile() const; //Сохранение сообщений в файл

};


