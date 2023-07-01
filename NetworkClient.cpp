#include "NetworkClient.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ws2tcpip.h>

bool NetworkClient::createSocket()
{
    clientSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket_ == INVALID_SOCKET) {
        std::cerr << "������: ����� �� ������" << std::endl;
        return false;
    }
    return true;
}

bool NetworkClient::connectToServer()
{
    SOCKADDR_IN serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port_);
    inet_pton(AF_INET, ipAddress_.c_str(), &(serverAddress.sin_addr));

    if (connect(clientSocket_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "�� ������� ���������� ���������� � ��������" << std::endl;
        return false;
    }
    return true;
}

void NetworkClient::closeSocket()
{
    if (clientSocket_ != INVALID_SOCKET) {
        closesocket(clientSocket_);
        clientSocket_ = INVALID_SOCKET;
    }
}

NetworkClient::NetworkClient(const std::string& ipAddress, int port)
    : ipAddress_(ipAddress), port_(port), clientSocket_(INVALID_SOCKET)
{
    // ����������� ������
}

NetworkClient::~NetworkClient()
{
    stopClient();
    WSACleanup();
}

void NetworkClient::startClient()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "������ �������������" << std::endl;
        return;
    }

    if (!createSocket()) {
        WSACleanup();
        return;
    }

    if (!connectToServer()) {
        closeSocket();
        WSACleanup();
        return;
    }

    std::cout << "���������� � �������� �����������" << std::endl;
    std::cout << "\n��������������� ������: ";
    std::string greeting;
    std::getline(std::cin, greeting);
    // �������������� � ��������, �������� � ����� ���������
    sendMessage(clientSocket_, greeting);

    closeSocket();
    WSACleanup();
}

void NetworkClient::stopClient()
{
    closeSocket();
    WSACleanup();
}

void NetworkClient::sendMessage(SOCKET clientSocket, const std::string& message) //�������� ��������� ��������� �� ������
{
    send(clientSocket, message.c_str(), message.length(), 0);
    std::cout << "\n��������� ����������" << std::endl;

    char buffer[MESSAGE_LENGTH] = { 0 };
    recv(clientSocket, buffer, MESSAGE_LENGTH, 0);
    std::string response(buffer);
    std::cout << "\n����� �������: " << response << "\n" << std::endl;
}
//� ����������:
// �������� ����� ������ ������������� �������
void NetworkClient::sendUserFile(SOCKET clientSocket, const std::string& filename)
{
    std::ifstream userFileRead("userFile.txt", std::ios::in | std::ios::binary);
    if (userFileRead.is_open()) {
        std::string fileContent((std::istreambuf_iterator<char>(userFileRead)), std::istreambuf_iterator<char>());
        userFileRead.close();

        send(clientSocket, fileContent.c_str(), fileContent.length(), 0);
        std::cout << "���� " << filename << " ��������� �������" << std::endl;
    }
    else {
        std::cout << "�� ������� ������� userFile.txt" << std::endl;
    }
}
// ��������� ������������ ����������� ����� ������ ������������� �� �������
void NetworkClient::receiveUpdatedUserFile(SOCKET clientSocket, const std::string& filename)
{
    char buffer[MESSAGE_LENGTH];
    std::string fileContent;

    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, MESSAGE_LENGTH, 0)) > 0) {
        fileContent += std::string(buffer, bytesRead);
    }

    if (bytesRead == 0) {
        std::ofstream userFileWrite("userFile.txt", std::ios::out | std::ios::binary);
        if (userFileWrite.is_open()) {
            userFileWrite << fileContent;
            userFileWrite.close();
            std::cout << "���� " << filename << " ������� ������� � �������� � �������" << std::endl;
        }
        else {
            std::cout << "�� ������� ������� userFile.txt ��� ������" << std::endl;
        }
    }
    else {
        std::cout << "������ ��� ��������� ����� �� �������" << std::endl;
    }
}
