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
	NetworkClient client("127.0.0.1", PORT); //������ ������

	chat.start(); //������ ����
	chat.loadUsersFromFile(); //�������� ������ �������������
	chat.loadMessageFromFile(); //�������� ������ ���������
	chat.adminCreation(); //�������� ������������ � ������� ������, ���� ������ ������������ ���

	client.startClient(); //������ ������� �������

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
	client.stopClient(); //�������� �������

	return 0;
}