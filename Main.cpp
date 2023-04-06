#include <iostream>
#include <Windows.h>
#include "Chat.h"
#include "Main.h"

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Chat chat; //Чат создан

	chat.start(); 
	chat.adminCreation();

	while (chat.chatWork())
	{
		chat.showMainMenu(); //Главное меню
		while (chat.getCurrentUser())
		{
			chat.showChatMenu();
		}
	}
	
	return 0;
}