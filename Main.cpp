#include <iostream>
#include "Chat.h"

int main() 
{
	setlocale(LC_ALL, "Russian");

	Chat chat; //��� ������

	chat.start(); 

	while (chat.chatWork()) 
	{

	}


	return 0;
}