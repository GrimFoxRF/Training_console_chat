#include <iostream>
#include <Windows.h>
#include "Chat.h"

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Chat chat; //��� ������

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