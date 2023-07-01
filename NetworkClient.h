//Сетевой код клиента
#pragma once
#include <winsock2.h>
#include <iostream>

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будет использоваться этот номер порта


class NetworkClient {
private:
    std::string ipAddress_;
    int port_ = PORT;
    WSADATA wsaData;
    SOCKET clientSocket_;

    bool createSocket(); //создание сокета
    bool connectToServer(); //соединение с сервером
    void closeSocket(); //закрытие сокета

public:
    NetworkClient(const std::string& ipAddress, int port); //конструктор класса NetworkClient
    ~NetworkClient();

    void startClient(); //запуск сетевых функций
    void stopClient(); //остановка сетевых функций и закрытие сокета
    void sendMessage(SOCKET clientSocket, const std::string& message); //отправка тестового сообщения серверу

    //методы для синхронизации текстовых файлов, пока в разработке
    void sendUserFile(SOCKET clientSocket, const std::string& filename); 
    void receiveUpdatedUserFile(SOCKET clientSocket, const std::string& filename);
};