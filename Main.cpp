#include <iostream>
#include <Windows.h>
#include "Chat.h"

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	Chat chat; //Чат создан

	chat.start();
	chat.loadUsersFromFile(); //загрузка списка пользователей
	chat.loadMessageFromFile(); //загрузка списка сообщений
	chat.adminCreation(); //Создание пользователя с правами Админа, если такого пользователя нет

	while (chat.chatWork())
	{
		chat.showMainMenu(); //Главное меню
		while (chat.getCurrentUser())
		{
			chat.showChatMenu(); //меню переписки, Чат
		}
	}

	chat.saveUsersToFile(); //сохранение списка пользователей
	chat.saveMessageToFile(); //сохранение сообщений

	return 0;
}