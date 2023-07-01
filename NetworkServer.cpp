#include "NetworkServer.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

NetworkServer::NetworkServer(int port)
    : port_(port), serverSocket_(INVALID_SOCKET), clientSocket_(INVALID_SOCKET)
{
    // Конструктор класса
}

NetworkServer::~NetworkServer()
{
    closeServerSocket();
    WSACleanup();
}

void NetworkServer::startServer()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации" << std::endl;
        return;
    }

    if (!createSocket()) {
        WSACleanup();
        return;
    }

    if (!bindSocket()) {
        closeServerSocket();
        WSACleanup();
        return;
    }

    if (!listenForConnections()) {
        closeServerSocket();
        WSACleanup();
        return;
    }

    std::cout << "Сервер запущен. Ожидание соединения..." << std::endl;

    if (!acceptClientConnection()) {
        closeServerSocket();
        WSACleanup();
        return;
    }

    std::cout << "Клиент подключился к серверу" << std::endl;

    handleClient(clientSocket_);

    closeServerSocket();
    WSACleanup();
}

bool NetworkServer::createSocket()
{
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ == INVALID_SOCKET) {
        std::cerr << "ОШИБКА: сокет не создан" << std::endl;
        return false;
    }
    return true;
}

bool NetworkServer::bindSocket()
{
    SOCKADDR_IN serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port_);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Не удалось привязать сокет" << std::endl;
        return false;
    }
    return true;
}

bool NetworkServer::listenForConnections()
{
    if (listen(serverSocket_, 1) == SOCKET_ERROR) {
        std::cerr << "Не удалось прослушать сокет" << std::endl;
        return false;
    }
    return true;
}

bool NetworkServer::acceptClientConnection()
{
    clientSocket_ = accept(serverSocket_, NULL, NULL);
    if (clientSocket_ == INVALID_SOCKET) {
        std::cerr << "Не удалось принять соединение клиента" << std::endl;
        return false;
    }
    return true;
}

void NetworkServer::closeServerSocket()
{
    if (serverSocket_ != INVALID_SOCKET) {
        closesocket(serverSocket_);
        serverSocket_ = INVALID_SOCKET;
    }
}
//прием сообщения от клиента и отправка ответа
void NetworkServer::handleClient(SOCKET clientSocket)
{
    char buffer[MESSAGE_LENGTH] = { 0 };
    recv(clientSocket, buffer, MESSAGE_LENGTH, 0);
    std::string message(buffer);
    std::cout << "\nСообщение от клиента: " << message << "\n" << std::endl;
    std::cout << "Отправьте приветствие для клиента: ";
    std::string response;
    std::getline(std::cin, response);

    send(clientSocket, response.c_str(), response.length(), 0);
    std::cout << "\nОтвет отправлен клиенту: " << response << "\n" << std::endl;
}
//закрытие сервера
void NetworkServer::stopServer() {
    // Закрытие сокета сервера
    if (serverSocket_ != INVALID_SOCKET) {
        closesocket(serverSocket_);
        serverSocket_ = INVALID_SOCKET;
    }

    // Закрытие сокета клиента
    if (clientSocket_ != INVALID_SOCKET) {
        closesocket(clientSocket_);
        clientSocket_ = INVALID_SOCKET;
    }
    
    WSACleanup(); // Вызов WSACleanup() для освобождения ресурсов Winsock
}
//В РАЗРАБОТКЕ:
// Отправка файла списка пользователей клиенту
void NetworkServer::sendUserFile(SOCKET clientSocket, const std::string& filename)
{
    std::ifstream userFileRead("userFile.txt", std::ios::in | std::ios::binary);
    if (userFileRead.is_open()) {
        std::string fileContent((std::istreambuf_iterator<char>(userFileRead)), std::istreambuf_iterator<char>());
        userFileRead.close();

        send(clientSocket, fileContent.c_str(), fileContent.length(), 0);
        std::cout << "Файл " << filename << " отправлен клиенту" << std::endl;
    }
    else {
        std::cout << "Не удалось открыть userFile.txt" << std::endl;
    }
}
// Получение обновленного содержимого файла списка пользователей от клиента
void NetworkServer::receiveUpdatedUserFile(SOCKET clientSocket, const std::string& filename)
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
            std::cout << "Файл " << filename << " успешно получен и сохранен на сервере" << std::endl;
        }
        else {
            std::cout << "Не удалось открыть userFile.txt для записи" << std::endl;
        }
    }
    else {
        std::cout << "Ошибка при получении файла от клиента" << std::endl;
    }
}


