# Training_console_chat
Проект тренировочного консольного чата

Группа 1007

Максим Данченко

-----------------------------------------
Консольный чат:

Чат может регистрировать новых пользователей и поддерживает вход с помощью логина и пароля.

В чате реализованы сетевые функции: сервер и клиент при запуске могут отправлять друг другу сообщения.

Списки пользователей и сообщений сохраняются в отдельные текстовые файлы и загружаются при запуске программы.

Пользователи могут обмениваться сообщениями между собой используя имена пользователя или писать сообщения всем используя all как имя пользователя
В меню чата можно посмотреть список всех пользоватлей, с указанием на текущего
Есть функция отображения всех сообщений

Дополнительный функционал:

При запуске программы создается пользователь Admin, для которого в меню чата есть отдельная функция - посмотреть данные всех пользователей

В главном меню отображается текущая дата и время

-------------------------------------------------
Чат разделен на две программы - сервер и клиент, MainServer и MainClient соответственно

--- Сетевые функции ---

Сетевой функционал сервера реализован в NetworkServer
Сетевой функционал клиента реализован в NetworkClient

client.startClient(); //запуск сетевых функций клиента
server.startClient(); //запуск сетевых функций сервера
client.stopClient(); //закрытие соединеия
server.stopClient(); //закрытие соединеия

--- Основной функционал чата ---

Пользовательский класс User имеет поля _login, _password и _name
Пользовательский шаблонный класс Message имеет поля _from, _to, и _text

Основной функционал чата реализован в классе Chat:
Класс содержит вектора _users (пользователи) и _messages (сообщения)
shared_ptr<User> getUserByLogin(const std::string& login) // проверка массива пользователей на совпадение логина
shared_ptr<User> getUserByName(const std::string& name) // проверка массива пользователей на совпадение имени

void showMainMenu(); // Главное меню
void showLoginMenu(); // Меню входа пользователя
void showRegistrationMenu(); // Меню регистрации нового пользователя
void showChat(); // Лента чата с пользователями и сообщениями
void showChatMenu(); //Меню чата, с выбором действий
void showAllUsers(); //Показать список пользователей
void addMessage(); //Добавить сообщение


void adminCreation(); //Создание пользователя Admin
void showAllUsersInfo(); //Функция для админа, позволяет посмотреть данные всех пользователей

--- Хранение информации пользователей ---

void loadUsersFromFile(); //Загрузка списка пользователей из файла
void saveUsersToFile() const; //Сохранение списка пользователей в файл
void loadMessageFromFile(); //Звгрузка списка сообщений из файла
void saveMessageToFile() const; //Сохранение сообщений в файл

--- Служебные методы ---

Utility содержит служебные методы:

class loginException обрабатывает исключение в случае повторяющегося логина при регистрации
class nameException обрабатывает исключение в случае повторяющегося имени пользователя при регистрации

void SetFilePermissions(const std::string& filePath); //Установка разрешения доступа к файлам

string getCurrentTime(); //методы получения системной даты и времени

--- В РАЗРАБОТКЕ ---

Файл HashTable содержит хэш-функцию, для хеширования пароля, пока не используется в программе

Методы для синхронизации списка пользователей, пока не дорработаны:
void sendUserFile(SOCKET clientSocket, const std::string& filename); 
void receiveUpdatedUserFile(SOCKET clientSocket, const std::string& filename);

-----------------------------------------------------------

