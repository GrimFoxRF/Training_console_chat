#include <iostream>
#include <Windows.h>
#include "Chat.h"
#include "Main.h"

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Chat chat; //��� ������

	chat.start(); 
	chat.adminCreation();

	while (chat.chatWork())
	{
		chat.showMainMenu(); //������� ����
		while (chat.getCurrentUser())
		{
			chat.showChatMenu();
		}
	}
	
	return 0;
}