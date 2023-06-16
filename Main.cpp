#include <iostream>
#include <Windows.h>
#include "Chat.h"

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	Chat chat; //��� ������

	chat.start();
	chat.loadUsersFromFile(); //�������� ������ �������������
	chat.loadMessageFromFile(); //�������� ������ ���������
	chat.adminCreation(); //�������� ������������ � ������� ������, ���� ������ ������������ ���

	while (chat.chatWork())
	{
		chat.showMainMenu(); //������� ����
		while (chat.getCurrentUser())
		{
			chat.showChatMenu(); //���� ���������, ���
		}
	}

	chat.saveUsersToFile(); //���������� ������ �������������
	chat.saveMessageToFile(); //���������� ���������

	return 0;
}