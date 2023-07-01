//������� ��� �������
#pragma once
#include <winsock2.h>
#include <iostream>

#define MESSAGE_LENGTH 1024 // ������������ ������ ������ ��� ������
#define PORT 7777 // ����� �������������� ���� ����� �����


class NetworkClient {
private:
    std::string ipAddress_;
    int port_ = PORT;
    WSADATA wsaData;
    SOCKET clientSocket_;

    bool createSocket(); //�������� ������
    bool connectToServer(); //���������� � ��������
    void closeSocket(); //�������� ������

public:
    NetworkClient(const std::string& ipAddress, int port); //����������� ������ NetworkClient
    ~NetworkClient();

    void startClient(); //������ ������� �������
    void stopClient(); //��������� ������� ������� � �������� ������
    void sendMessage(SOCKET clientSocket, const std::string& message); //�������� ��������� ��������� �������

    //������ ��� ������������� ��������� ������, ���� � ����������
    void sendUserFile(SOCKET clientSocket, const std::string& filename); 
    void receiveUpdatedUserFile(SOCKET clientSocket, const std::string& filename);
};