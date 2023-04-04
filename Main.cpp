#include <iostream>
#include "Chat.h"

int main() 
{
	setlocale(LC_ALL, "Russian");

	Chat chat; //Чат создан

	chat.start(); 

	while (chat.chatWork())
	{
		chat.showMainMenu();
		while (chat.getCurrentUser())
		{
			chat.showChatMenu();
		}
	}
	
	return 0;
}