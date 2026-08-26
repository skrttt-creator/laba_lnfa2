#include <stdio.h>
#include <iostream>
#ifdef _WIN32
#endif
#include <windows.h>
#undef GetMessage
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"
#include "UniquePtr.h"
#include "Tests.h"
#include "Exceptions.h"
#include "Algorithms.h"
template <class T>
void PrintSequence(const Sequence<T>& seq) {
    printf("Текущая последовательность (len=%d): [ ", seq.GetLength());
    for (int i = 0; i < seq.GetLength(); i++) {
        std::cout << seq.Get(i) << (i == seq.GetLength() - 1 ? "" : ", ");
    }
    printf(" ]\n");
}

int ReadInt(const char* prompt) {
    printf("%s", prompt);
    int value = 0;
    while (scanf("%d", &value) != 1) {
        while (getchar() != '\n');
        printf("Ошибка! Введите целое число: ");
    }
    return value;
}

void SequenceOperationsMenu(UniquePtr<Sequence<int>>& current) {
    int op = -1;
    while (op != 0) {
        printf("\n ОПЕРАЦИИ НАД ПОСЛЕДОВАТЕЛЬНОСТЬЮ \n");
        PrintSequence(*current);
        printf("1. Append (в конец)\n");
        printf("2. Prepend (в начало)\n");
        printf("3. InsertAt (в позицию)\n");
        printf("4. GetFirst / GetLast\n");
        printf("5. Get(index) / [index]\n");
        printf("6. GetSubsequence (подпоследовательность)\n");
        printf("0. Вернуться в главное меню\n");

        op = ReadInt("Выберите операцию: ");

        try {
            switch (op) {
                case 1: {
                    int val = ReadInt("Значение: ");
                    Sequence<int>* next = current->Append(val);
                    if (next != current.Get()) current.Reset(next);
                    break;
                }
                case 2: {
                    int val = ReadInt("Значение: ");
                    Sequence<int>* next = current->Prepend(val);
                    if (next != current.Get()) current.Reset(next);
                    break;
                }
                case 3: {
                    int idx = ReadInt("Индекс: ");
                    int val = ReadInt("Значение: ");
                    Sequence<int>* next = current->InsertAt(val, idx);
                    if (next != current.Get()) current.Reset(next);
                    break;
                }
                case 4: {
                    if (current->GetLength() == 0) throw IndexOutOfRange("Последовательность пуста");
                    printf("First: %d, Last: %d\n", current->GetFirst(), current->GetLast());
                    break;
                }
                case 5: {
                    int idx = ReadInt("Индекс: ");
                    printf("Элемент по индексу %d: %d\n", idx, (*current)[idx]);
                    break;
                }
                case 6: {
                    int start = ReadInt("Начальный индекс: ");
                    int end = ReadInt("Конечный индекс: ");
                    UniquePtr<Sequence<int>> sub(current->GetSubsequence(start, end));
                    printf("Результат: ");
                    PrintSequence(*sub);
                    break;
                }
                case 0: break;
                default: printf("Неверная операция.\n");
            }
        } catch (const Exception& e) {
            printf("\n Ошибка: %s\n", e.GetMessage());
        }
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    UniquePtr<Sequence<int>> current;
    int choice = -1;

    while (choice != 0) {
        printf("1. Создать ArraySequence (Изменяемая)\n");
        printf("2. Создать ListSequence (Изменяемая)\n");
        printf("3. Создать ImmutableArraySequence (НЕизменяемая)\n");
        printf("4. Создать ImmutableListSequence (НЕизменяемая)\n");
        printf("5. Запустить Обязательные тесты\n");
        printf("6. Запустить Битовые тесты (BitSequence)\n");
        printf("7. Тест производительности (Mutable vs Immutable)\n");
        printf("8. Тест бонусных алгоритмов (Map/Reduce/Zip...)\n");
        printf("0. Выход\n");

        choice = ReadInt("Ваш выбор: ");

        try {
            switch (choice) {
                case 1:
                    current.Reset(new ArraySequence<int>());
                    SequenceOperationsMenu(current);
                    break;
                case 2:
                    current.Reset(new ListSequence<int>());
                    SequenceOperationsMenu(current);
                    break;
                case 3:
                    current.Reset(new ImmutableArraySequence<int>());
                    SequenceOperationsMenu(current);
                    break;
                case 4:
                    current.Reset(new ImmutableListSequence<int>());
                    SequenceOperationsMenu(current);
                    break;
                case 5: RunCoreTests(); break;
                case 6: TestBitSequence(); break;
                case 7: ComparePerformance(); break;
                case 8: RunBonusTests(); break;
                case 0: printf("Выход из программы...\n"); break;
                default: printf("Неверный пункт.\n");
            }
        } catch (const Exception& e) {
            printf("\nКритическая ошибка: %s\n", e.GetMessage());
        }
    }

    return 0;
}