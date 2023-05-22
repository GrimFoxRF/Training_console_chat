#pragma once
#include <vector>
#include <string>
#include <iostream>

const int TABLE_SIZE = 100;  // ������ ���-�������

class HashTable {
    struct KeyValue {
        std::string key;
        std::string value;
    };

    std::vector<KeyValue> table;

    // ���-������� ������� ���������
    int hash(const std::string& key) {
        const double A = 0.618033;  // ���������, ������� � �������� �������
        double hashValue = 0;

        for (char ch : key) {
            hashValue += ch;
            hashValue *= A;
        }

        return static_cast<int>(TABLE_SIZE * (hashValue - static_cast<int>(hashValue)));
    }

    // ����� ������������� ������������
    int quadraticProbe(int index, int attempt) {
        return (index + attempt * attempt) % TABLE_SIZE;
    }

public:
    HashTable() : table(TABLE_SIZE) {}

    void insert(const std::string& key, const std::string& value) {
        int index = hash(key);

        // ����� ��������� ������ ��� ������ � ����������� ������
        int attempt = 0;
        while (!table[index].key.empty() && table[index].key != key) {
            attempt++;
            index = quadraticProbe(index, attempt);
        }

        // ������� ���� ����-��������
        table[index].key = key;
        table[index].value = value;
    }

    std::string getValue(const std::string& key) {
        int index = hash(key);

        // ����� ������ � ������
        int attempt = 0;
        while (!table[index].key.empty()) {
            if (table[index].key == key) {
                return table[index].value;
            }
            attempt++;
            index = quadraticProbe(index, attempt);
        }

        return "";  // ���� ���� �� ������, ���������� ������ ������
    }
};