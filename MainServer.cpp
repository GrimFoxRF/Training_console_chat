//��� ����-�������
#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include "Chat.h"

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	Chat chat; //��� ������
	NetworkServer server(PORT); //������ ������

	chat.start(); //������ ����
	chat.loadUsersFromFile(); //�������� ������ �������������
	chat.loadMessageFromFile(); //�������� ������ ���������
	chat.adminCreation(); //�������� ������������ � ������� ������, ���� ������ ������������ ���

	server.startServer(); //������ ������� �������

	while (chat.chatWork()) //�������� ���� ���������
	{
		chat.showMainMenu(); //������� ����
		while (chat.getCurrentUser())
		{
			chat.showChatMenu(); //���� ���������, ���
		}
	}

	chat.saveUsersToFile(); //���������� ������ �������������
	chat.saveMessageToFile(); //���������� ���������
	server.stopServer(); //�������� �������

	return 0;
}