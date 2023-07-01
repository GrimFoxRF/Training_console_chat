//Код чата-клиента
#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include "Chat.h"

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	Chat chat; //Чат создан
	NetworkClient client("127.0.0.1", PORT); //клиент создан

	chat.start(); //запуск чата
	chat.loadUsersFromFile(); //загрузка списка пользователей
	chat.loadMessageFromFile(); //загрузка списка сообщений
	chat.adminCreation(); //Создание пользователя с правами Админа, если такого пользователя нет

	client.startClient(); //запуск сетевых функций

	while (chat.chatWork()) //основной цикл программы
	{
		chat.showMainMenu(); //Главное меню
		while (chat.getCurrentUser())
		{
			chat.showChatMenu(); //меню переписки, Чат
		}
	}

	chat.saveUsersToFile(); //сохранение списка пользователей
	chat.saveMessageToFile(); //сохранение сообщений
	client.stopClient(); //закрытие сервера

	return 0;
}