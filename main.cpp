#include <stdio.h>
#include <iostream>
#include <cstddef>

#ifdef _WIN32
#include <windows.h>
#undef GetMessage
#endif

#include "ArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"
#include "Tests.h"
#include "Exceptions.h"






int MapMultiplyBy2(int x) { return x * 2; }
int MapSquare(int x) { return x * x; }


int MapAddIndex(int x, size_t index) { return x + (int)index; }
int MapMultiplyByIndex(int x, size_t index) { return x * (int)index; }


bool WhereIsEven(int x) { return x % 2 == 0; }
bool WhereIsPositive(int x) { return x > 0; }


int ReduceSum(int acc, int x) { return acc + x; }
int ReduceProduct(int acc, int x) { return acc * x; }


int ZipSum(int a, int b) { return a + b; }
int ZipMultiply(int a, int b) { return a * b; }





template <class T>
void PrintSequence(const Sequence<T>& seq) {
    printf("Текущая последовательность (len=%zu): [ ", seq.GetLength());
    
    IEnumerator<T>* it = seq.GetEnumerator();
    bool first = true;
    while (it->MoveNext()) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << it->GetCurrent();
        first = false;
    }
    delete it;
    
    printf(" ]\n");
}

void PrintBitSequence(const Sequence<Bit>& seq) {
    printf("Битовая последовательность (len=%zu): [ ", seq.GetLength());
    
    IEnumerator<Bit>* it = seq.GetEnumerator();
    bool first = true;
    while (it->MoveNext()) {
        if (!first) {
            printf(", ");
        }
        printf("%d", it->GetCurrent().Get() ? 1 : 0);
        first = false;
    }
    delete it;
    
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

int ReadBitInt(const char* prompt) {
    int val = ReadInt(prompt);
    while (val != 0 && val != 1) {
        printf("Ошибка! Бит может быть только 0 или 1.\n");
        val = ReadInt(prompt);
    }
    return val;
}





void SequenceOperationsMenu(Sequence<int>*& current) {
    int op = -1;
    while (op != 0) {
        printf("\n ОПЕРАЦИИ НАД ПОСЛЕДОВАТЕЛЬНОСТЬЮ \n");
        PrintSequence(*current);
        printf("1.  Append (в конец)\n");
        printf("2.  Prepend (в начало)\n");
        printf("3.  InsertAt (в позицию)\n");
        printf("4.  GetFirst / GetLast\n");
        printf("5.  Get(index) / [index]\n");
        printf("6.  GetSubsequence (подпоследовательность)\n");
        printf("7.  Map (обычный: x*2 или x*x)\n");
        printf("8.  Map с контекстом (x + index или x * index)\n");
        printf("9.  Where (фильтр: четные или > 0)\n");
        printf("10. Reduce (сумма или произведение)\n");
        printf("11. Zip (слияние со второй последовательностью)\n");
        printf("0.  Вернуться в главное меню\n");

        op = ReadInt("Выберите операцию: ");

        try {
            switch (op) {
                case 1: {
                    int val = ReadInt("Значение: ");
                    Sequence<int>* next = current->Append(val);
                    if (next != current) {
                        delete current;
                        current = next;
                    }
                    break;
                }
                case 2: {
                    int val = ReadInt("Значение: ");
                    Sequence<int>* next = current->Prepend(val);
                    if (next != current) {
                        delete current;
                        current = next;
                    }
                    break;
                }
                case 3: {
                    int idx = ReadInt("Индекс: ");
                    int val = ReadInt("Значение: ");
                    Sequence<int>* next = current->InsertAt(val, idx);
                    if (next != current) {
                        delete current;
                        current = next;
                    }
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
                    Sequence<int>* sub = current->GetSubsequence(start, end);
                    printf("Результат: ");
                    PrintSequence(*sub);
                    delete sub;
                    break;
                }
                case 7: {
                    printf("Выберите функцию Map:\n1. Умножить на 2 (x * 2)\n2. Возвести в квадрат (x * x)\n");
                    int mode = ReadInt("Ваш выбор: ");
                    Sequence<int>* mapped = (mode == 2) ? current->Map(MapSquare) : current->Map(MapMultiplyBy2);
                    delete current;
                    current = mapped;
                    printf("Map успешно применен!\n");
                    break;
                }
                case 8: {
                    printf("Выберите функцию Map с контекстом (индексом):\n1. Прибавить индекс (x + i)\n2. Умножить на индекс (x * i)\n");
                    int mode = ReadInt("Ваш выбор: ");
                    Sequence<int>* mapped = (mode == 2) ? current->Map(MapMultiplyByIndex) : current->Map(MapAddIndex);
                    delete current;
                    current = mapped;
                    printf("Контекстный Map успешно применен!\n");
                    break;
                }
                case 9: {
                    printf("Выберите условие фильтрации Where:\n1. Оставить только четные\n2. Оставить только положительные (> 0)\n");
                    int mode = ReadInt("Ваш выбор: ");
                    Sequence<int>* filtered = (mode == 2) ? current->Where(WhereIsPositive) : current->Where(WhereIsEven);
                    delete current;
                    current = filtered;
                    printf("Фильтрация Where выполнена!\n");
                    break;
                }
                case 10: {
                    if (current->GetLength() == 0) throw IndexOutOfRange("Последовательность пуста");
                    printf("Выберите операцию Reduce:\n1. Сумма всех элементов\n2. Произведение всех элементов\n");
                    int mode = ReadInt("Ваш выбор: ");
                    int res = (mode == 2) ? current->Reduce(ReduceProduct, 1) : current->Reduce(ReduceSum, 0);
                    printf("Результат Reduce: %d\n", res);
                    break;
                }
                case 11: {
                    int count = ReadInt("Сколько элементов будет во второй последовательности? ");
                    if (count < 0) throw InvalidArgument("Количество не может быть отрицательным");
                    
                    MutableArraySequence<int> other;
                    for (int i = 0; i < count; ++i) {
                        printf("[%d] ", i);
                        int val = ReadInt("Значение: ");
                        other.Append(val);
                    }
                    
                    printf("Выберите комбинатор Zip:\n1. Поэлементная сумма (a + b)\n2. Поэлементное произведение (a * b)\n");
                    int mode = ReadInt("Ваш выбор: ");
                    Sequence<int>* zipped = (mode == 2) ? current->Zip(&other, ZipMultiply) : current->Zip(&other, ZipSum);
                    delete current;
                    current = zipped;
                    printf("Zip успешно выполнен!\n");
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





void BitSequenceOperationsMenu(BitSequence*& current) {
    int op = -1;
    while (op != 0) {
        printf("\n ОПЕРАЦИИ НАД BIT SEQUENCE \n");
        PrintBitSequence(*current);
        printf("1. Append (добавить бит в конец)\n");
        printf("2. Prepend (добавить бит в начало)\n");
        printf("3. InsertAt (вставить бит в позицию)\n");
        printf("4. Get(index) / [index]\n");
        printf("5. GetSubsequence (подпоследовательность)\n");
        printf("6. Побитовое НЕ (NOT / инверсия)\n");
        printf("7. Побитовое И (AND со второй последовательностью)\n");
        printf("8. Побитовое ИЛИ (OR со второй последовательностью)\n");
        printf("9. Побитовое ИСКЛ. ИЛИ (XOR со второй последовательностью)\n");
        printf("0. Вернуться в главное меню\n");

        op = ReadInt("Выберите операцию: ");

        try {
            switch (op) {
                case 1: {
                    int b = ReadBitInt("Введите бит (0 или 1): ");
                    Sequence<Bit>* next = current->Append(Bit(b));
                    if (next != current) {
                        delete current;
                        current = static_cast<BitSequence*>(next);
                    }
                    break;
                }
                case 2: {
                    int b = ReadBitInt("Введите бит (0 или 1): ");
                    Sequence<Bit>* next = current->Prepend(Bit(b));
                    if (next != current) {
                        delete current;
                        current = static_cast<BitSequence*>(next);
                    }
                    break;
                }
                case 3: {
                    int idx = ReadInt("Индекс: ");
                    int b = ReadBitInt("Введите бит (0 или 1): ");
                    Sequence<Bit>* next = current->InsertAt(Bit(b), idx);
                    if (next != current) {
                        delete current;
                        current = static_cast<BitSequence*>(next);
                    }
                    break;
                }
                case 4: {
                    int idx = ReadInt("Индекс: ");
                    printf("Бит по индексу %d: %d\n", idx, (*current)[idx].Get() ? 1 : 0);
                    break;
                }
                case 5: {
                    int start = ReadInt("Начальный индекс: ");
                    int end = ReadInt("Конечный индекс: ");
                    Sequence<Bit>* sub = current->GetSubsequence(start, end);
                    printf("Результат: ");
                    PrintBitSequence(*sub);
                    delete sub;
                    break;
                }
                case 6: {
                    BitSequence* inverted = current->Not();
                    delete current;
                    current = inverted;
                    printf("Инверсия (NOT) выполнена!\n");
                    break;
                }
                case 7:
                case 8:
                case 9: {
                    int count = ReadInt("Сколько бит во второй последовательности? ");
                    if (count < 0) throw InvalidArgument("Количество не может быть отрицательным");
                    
                    BitSequence other;
                    for (int i = 0; i < count; ++i) {
                        printf("Бит [%d] ", i);
                        int b = ReadBitInt("(0 или 1): ");
                        other.Append(Bit(b));
                    }
                    
                    BitSequence* res = nullptr;
                    if (op == 7) res = current->And(other);
                    else if (op == 8) res = current->Or(other);
                    else res = current->Xor(other);

                    delete current;
                    current = res;
                    printf("Побитовая операция выполнена!\n");
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

    Sequence<int>* current = nullptr;
    BitSequence* currentBits = nullptr;
    int choice = -1;

    while (choice != 0) {
        printf("              Меню работы                  \n");
        printf("1. Создать MutableArraySequence (Изменяемая)\n");
        printf("2. Создать MutableListSequence (Изменяемая)\n");
        printf("3. Создать ImmutableArraySequence (НЕизменяемая)\n");
        printf("4. Создать ImmutableListSequence (НЕизменяемая)\n");
        printf("5. Создать и работать с BitSequence\n");
        printf("------------------------------------------------------\n");
        printf("6. Запустить Обязательные тесты\n");
        printf("7. Запустить Битовые тесты (BitSequence)\n");
        printf("8. Тест производительности (Mutable vs Immutable)\n");
        printf("9. Тест бонусных алгоритмов (Map/Reduce/Where)\n");
        printf("0. Выход\n");

        choice = ReadInt("Ваш выбор: ");

        try {
            switch (choice) {
                case 1:
                    delete current;
                    current = new MutableArraySequence<int>();
                    SequenceOperationsMenu(current);
                    break;
                case 2:
                    delete current;
                    current = new MutableListSequence<int>();
                    SequenceOperationsMenu(current);
                    break;
                case 3:
                    delete current;
                    current = new ImmutableArraySequence<int>();
                    SequenceOperationsMenu(current);
                    break;
                case 4:
                    delete current;
                    current = new ImmutableListSequence<int>();
                    SequenceOperationsMenu(current);
                    break;
                case 5:
                    delete currentBits;
                    currentBits = new BitSequence();
                    BitSequenceOperationsMenu(currentBits);
                    break;
                case 6: RunCoreTests(); break;
                case 7: TestBitSequence(); break;
                case 8: ComparePerformance(); break;
                case 9: RunBonusTests(); break;
                case 0: 
                    printf("Выход из программы...\n"); 
                    break;
                default: printf("Неверный пункт.\n");
            }
        } catch (const Exception& e) {
            printf("\nКритическая ошибка: %s\n", e.GetMessage());
        }
    }

    delete current;
    delete currentBits;

    return 0;
}