#include "Tests.h"
#include <stdio.h>
#include <time.h>

#include "ArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"
#include "Exceptions.h"

// 1. ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (КОЛЛБЭКИ)

int MultiplyByTwo(int val) {
    return val * 2;
}

int AddIndexTest(int val, size_t index) {
    return val + index;
}

bool IsEven(int val) {
    return val % 2 == 0;
}

int SumReducerTest(int acc, int val) {
    return acc + val;
}

int ZipSumTest(int a, int b) {
    return a + b;
}

void Assert(bool condition, const char* message) {
    if (!condition) {
        printf("[ FAIL ] %s\n", message);
        throw Exception(message);
    }
}

void PrintSuccess(const char* testName) {
    printf("[ OK ] %s\n", testName);
}

// 2. ТЕСТЫ БАЗОВЫХ КОЛЛЕКЦИЙ (ARRAY SEQUENCE)

void TestArraySequence() {
    Sequence<int>* seq = new MutableArraySequence<int>();
    seq->Append(10);
    seq->Prepend(5);
    seq->InsertAt(15, 2); // [5, 10, 15]

    Assert(seq->GetLength() == 3, "ArraySequence (Pos): Ошибка длины");
    Assert(seq->GetFirst() == 5, "ArraySequence (Pos): Ошибка GetFirst");
    Assert(seq->GetLast() == 15, "ArraySequence (Pos): Ошибка GetLast");
    Assert(seq->Get(1) == 10, "ArraySequence (Pos): Ошибка Get");

    // Тест оператора [] (чтение и запись по ссылке)
    Assert((*seq)[1] == 10, "ArraySequence (Pos): Ошибка чтения через operator[]");
    (*seq)[1] = 25; // [5, 25, 15]
    Assert(seq->Get(1) == 25, "ArraySequence (Pos): Ошибка записи через operator[]");
    (*seq)[1] = 10; // возвращаем [5, 10, 15]

    // Тест подпоследовательности
    Sequence<int>* sub = seq->GetSubsequence(1, 2); // [10, 15]
    Assert(sub->GetLength() == 2, "ArraySequence (Pos): Длина подпоследовательности");
    Assert(sub->Get(0) == 10 && sub->Get(1) == 15, "ArraySequence (Pos): Значения подпоследовательности");

    // Тест конкатенации
    Sequence<int>* seq2 = new MutableArraySequence<int>();
    seq2->Append(99);
    Sequence<int>* concat = seq->Concat(seq2); // [5, 10, 15, 99]
    Assert(concat->GetLength() == 4, "ArraySequence (Pos): Длина после Concat");
    Assert(concat->GetLast() == 99, "ArraySequence (Pos): Значение после Concat");

    // Негативные тесты
    bool caught = false;
    try {
        seq->Get(-1);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ArraySequence (Neg): Нет исключения при Get(-1)");

    caught = false;
    try {
        seq->InsertAt(99, 100);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ArraySequence (Neg): Нет исключения при InsertAt(100)");

    caught = false;
    try {
        seq->GetSubsequence(2, 1);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ArraySequence (Neg): Нет исключения при startIndex > endIndex");

    caught = false;
    try {
        seq->Concat(nullptr);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ArraySequence (Neg): Нет исключения при Concat(nullptr)");

    Sequence<int>* emptySeq = new MutableArraySequence<int>();
    caught = false;
    try {
        emptySeq->GetFirst();
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ArraySequence (Neg): Нет исключения при GetFirst() на пустом массиве");

    delete seq;
    delete sub;
    delete seq2;
    delete concat;
    delete emptySeq;

    PrintSuccess("ArraySequence (Все операции + operator[])");
}

// 3. ТЕСТЫ БАЗОВЫХ КОЛЛЕКЦИЙ (LIST SEQUENCE)

void TestListSequence() {
    Sequence<int>* seq = new MutableListSequence<int>();
    seq->Append(10);
    seq->Prepend(5);
    seq->InsertAt(15, 2); // [5, 10, 15]

    Assert(seq->GetLength() == 3, "ListSequence (Pos): Ошибка длины");
    Assert(seq->GetFirst() == 5, "ListSequence (Pos): Ошибка GetFirst");
    Assert(seq->GetLast() == 15, "ListSequence (Pos): Ошибка GetLast");

    // Тест оператора [] (чтение и запись по ссылке в узел списка)
    Assert((*seq)[1] == 10, "ListSequence (Pos): Ошибка чтения через operator[]");
    (*seq)[1] = 77;
    Assert(seq->Get(1) == 77, "ListSequence (Pos): Ошибка записи через operator[]");
    (*seq)[1] = 10;

    // Тест подпоследовательности
    Sequence<int>* sub = seq->GetSubsequence(1, 2);
    Assert(sub->GetLength() == 2 && sub->GetFirst() == 10 && sub->GetLast() == 15, "ListSequence (Pos): Ошибка GetSubsequence");

    // Тест конкатенации
    Sequence<int>* seq2 = new MutableListSequence<int>();
    seq2->Append(99);
    Sequence<int>* concat = seq->Concat(seq2);
    Assert(concat->GetLength() == 4 && concat->GetLast() == 99, "ListSequence (Pos): Ошибка Concat");

    // Негативные тесты
    bool caught = false;
    try {
        seq->Get(-5);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ListSequence (Neg): Нет исключения при отрицательном индексе");

    caught = false;
    try {
        seq->Concat(nullptr);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ListSequence (Neg): Нет исключения при Concat(nullptr)");

    delete seq;
    delete sub;
    delete seq2;
    delete concat;

    PrintSuccess("ListSequence (Все операции + operator[])");
}

// 4. ТЕСТЫ НЕИЗМЕНЯЕМЫХ КОЛЛЕКЦИЙ (IMMUTABLE)

void TestImmutable() {
    Sequence<int>* oldArr = new ImmutableArraySequence<int>();
    Sequence<int>* newArr = oldArr->Append(10);
    Assert(oldArr->GetLength() == 0, "ImmutableArray: Оригинал изменился при Append!");
    Assert(newArr->GetLength() == 1 && newArr->Get(0) == 10, "ImmutableArray: Ошибка создания копии");

    Sequence<int>* oldList = new ImmutableListSequence<int>();
    Sequence<int>* newList = oldList->Prepend(42);
    Assert(oldList->GetLength() == 0, "ImmutableList: Оригинал изменился при Prepend!");
    Assert(newList->GetLength() == 1 && newList->Get(0) == 42, "ImmutableList: Ошибка создания копии");

    // Тест Concat для Immutable
    Sequence<int>* concatRes = newArr->Concat(newList);
    Assert(concatRes->GetLength() == 2, "Immutable Concat: Ошибка длины");
    Assert(concatRes->Get(0) == 10 && concatRes->Get(1) == 42, "Immutable Concat: Ошибка значений");
    Assert(newArr->GetLength() == 1, "Immutable Concat: Оригинал изменился!");

    // Негативные тесты
    bool caught = false;
    try {
        newArr->Get(100);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ImmutableArray (Neg): Нет защиты границ при Get()");

    delete oldArr;
    delete newArr;
    delete oldList;
    delete newList;
    delete concatRes;

    PrintSuccess("Неизменяемые коллекции (Immutable)");
}

// 5. ТЕСТЫ ИТЕРАТОРОВ И БИТОВЫХ ПОСЛЕДОВАТЕЛЬНОСТЕЙ

void TestIterators() {
    Sequence<int>* seq = new MutableArraySequence<int>();
    seq->Append(1);
    seq->Append(2);
    seq->Append(3);

    IEnumerator<int>* it = seq->GetEnumerator();
    int sum = 0;
    while (it->MoveNext()) {
        sum += it->GetCurrent();
    }
    Assert(sum == 6, "Итератор: неверная сумма элементов");

    it->Reset();
    Assert(it->MoveNext() == true && it->GetCurrent() == 1, "Итератор: сброс (Reset) не удался");

    delete it;
    delete seq;

    PrintSuccess("Паттерн Итератор");
}

void TestBitSequence() {
    BitSequence seq1;
    seq1.Append(Bit(1));
    seq1.Append(Bit(0));

    BitSequence seq2;
    seq2.Append(Bit(1));
    seq2.Append(Bit(1));

    // Побитовые операции: AND, OR, XOR, NOT
    BitSequence* andRes = seq1.And(seq2); // [1, 0] & [1, 1] = [1, 0]
    Assert(andRes->Get(0).Get() == 1 && andRes->Get(1).Get() == 0, "BitSequence: Ошибка AND");

    BitSequence* orRes = seq1.Or(seq2);   // [1, 0] | [1, 1] = [1, 1]
    Assert(orRes->Get(0).Get() == 1 && orRes->Get(1).Get() == 1, "BitSequence: Ошибка OR");

    BitSequence* xorRes = seq1.Xor(seq2); // [1, 0] ^ [1, 1] = [0, 1]
    Assert(xorRes->Get(0).Get() == 0 && xorRes->Get(1).Get() == 1, "BitSequence: Ошибка XOR");

    BitSequence* notRes = seq1.Not();     // ~[1, 0] = [0, 1]
    Assert(notRes->Get(0).Get() == 0 && notRes->Get(1).Get() == 1, "BitSequence: Ошибка NOT");

    // Проверка общих методов Sequence для BitSequence
    Sequence<Bit>* concatRes = seq1.Concat(&seq2); // [1, 0, 1, 1]
    Assert(concatRes->GetLength() == 4 && (*concatRes)[3].Get() == 1, "BitSequence: Ошибка Concat или operator[]");

    Sequence<Bit>* subRes = concatRes->GetSubsequence(1, 2); // [0, 1]
    Assert(subRes->GetLength() == 2 && subRes->GetFirst().Get() == 0 && subRes->GetLast().Get() == 1, "BitSequence: Ошибка GetSubsequence");

    // Негативный тест
    bool caught = false;
    try {
        seq1.Get(100);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "BitSequence (Neg): Нет исключения при выходе за границы");

    delete andRes;
    delete orRes;
    delete xorRes;
    delete notRes;
    delete concatRes;
    delete subRes;

    PrintSuccess("Битовые последовательности (AND, OR, XOR, NOT, Concat, Subseq)");
}

// 6. ТЕСТЫ АЛГОРИТМОВ (Map, Map с индексом, Where, Reduce, Zip)

void TestAlgorithms() {
    Sequence<int>* seq = new MutableArraySequence<int>();
    seq->Append(10);
    seq->Append(5);
    seq->Append(20);
    seq->Append(15);
    seq->Append(30); // [10, 5, 20, 15, 30]

    // 1. Обычный Map (x * 2) -> [20, 10, 40, 30, 60]
    Sequence<int>* mappedSeq = seq->Map(MultiplyByTwo);
    Assert(mappedSeq->GetLength() == 5 && mappedSeq->Get(0) == 20 && mappedSeq->Get(4) == 60, "Map (Pos): ошибка преобразования");

    // 2. Контекстный Map с индексом (x + index) -> [10+0, 5+1, 20+2, 15+3, 30+4] = [10, 6, 22, 18, 34]
    Sequence<int>* indexedMappedSeq = seq->Map(AddIndexTest);
    Assert(indexedMappedSeq->Get(0) == 10 && indexedMappedSeq->Get(1) == 6 && indexedMappedSeq->Get(4) == 34, "Map с контекстом (Pos): ошибка");

    // 3. Where (четные) -> [10, 20, 30]
    Sequence<int>* filteredSeq = seq->Where(IsEven);
    Assert(filteredSeq->GetLength() == 3 && filteredSeq->Get(1) == 20, "Where (Pos): ошибка фильтрации");

    // 4. Reduce (сумма) -> 10 + 5 + 20 + 15 + 30 = 80
    Assert(seq->Reduce(SumReducerTest, 0) == 80, "Reduce (Pos): ошибка свертки");

    // 5. Zip (поэлементная сумма двух последовательностей)
    Sequence<int>* seq2 = new MutableListSequence<int>();
    seq2->Append(1);
    seq2->Append(2);
    seq2->Append(3); // Длина 3 (меньше, чем у seq)

    // Zip должен корректно сшить по длине меньшей последовательности: [10+1, 5+2, 20+3] = [11, 7, 23]
    Sequence<int>* zippedSeq = seq->Zip(seq2, ZipSumTest);
    Assert(zippedSeq->GetLength() == 3, "Zip (Pos): ошибка длины");
    Assert(zippedSeq->Get(0) == 11 && zippedSeq->Get(1) == 7 && zippedSeq->Get(2) == 23, "Zip (Pos): ошибка значений");

    // Негативный тест для Zip(nullptr)
    bool caught = false;
    try {
        seq->Zip(nullptr, ZipSumTest);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "Zip (Neg): нет исключения при передаче nullptr");

    delete mappedSeq;
    delete indexedMappedSeq;
    delete filteredSeq;
    delete seq2;
    delete zippedSeq;
    delete seq;

    PrintSuccess("Функции высшего порядка (Map, Map+Index, Where, Reduce, Zip)");
}

// 7. ТЕСТЫ ПРОИЗВОДИТЕЛЬНОСТИ

void ComparePerformance() {
    printf("\n--- СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ (10000 элементов) ---\n");
    int count = 10000;

    // 1. Изменяемый массив (Mutable Array)
    clock_t start = clock();
    Sequence<int>* mutArray = new MutableArraySequence<int>();
    for (int i = 0; i < count; ++i) {
        Sequence<int>* next = mutArray->Append(i);
        if (next != mutArray) {
            delete mutArray;
            mutArray = next;
        }
    }
    clock_t end = clock();
    printf("Изменяемая (Mutable Array):     %.2f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);
    delete mutArray;

    // 2. Неизменяемый массив (Immutable Array)
    start = clock();
    Sequence<int>* immutArray = new ImmutableArraySequence<int>();
    for (int i = 0; i < count; ++i) {
        Sequence<int>* next = immutArray->Append(i);
        if (next != immutArray) {
            delete immutArray;
            immutArray = next;
        }
    }
    end = clock();
    printf("Неизменяемая (Immutable Array): %.2f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);
    delete immutArray;

    // 3. Изменяемый список (Mutable List)
    start = clock();
    Sequence<int>* mutList = new MutableListSequence<int>();
    for (int i = 0; i < count; ++i) {
        Sequence<int>* next = mutList->Append(i);
        if (next != mutList) {
            delete mutList;
            mutList = next;
        }
    }
    end = clock();
    printf("Изменяемая (Mutable List):      %.2f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);
    delete mutList;

    // 4. Неизменяемый список (Immutable List)
    start = clock();
    Sequence<int>* immutList = new ImmutableListSequence<int>();
    for (int i = 0; i < count; ++i) {
        Sequence<int>* next = immutList->Append(i);
        if (next != immutList) {
            delete immutList;
            immutList = next;
        }
    }
    end = clock();
    printf("Неизменяемая (Immutable List):  %.2f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);
    delete immutList;
}

void RunCoreTests() {
    printf("\n======================================================\n");
    printf("         ЗАПУСК БАЗОВЫХ ТЕСТОВ КОЛЛЕКЦИЙ              \n");
    printf("======================================================\n");
    try {
        TestArraySequence();
        TestListSequence();
        TestImmutable();
        TestIterators();
        printf("\n=> Все базовые тесты успешно выполнены!\n");
    } catch (const Exception& e) {
        printf("\n=> [КРИТИЧЕСКАЯ ОШИБКА] Прервано: %s\n", e.GetMessage());
    } catch (...) {
        printf("\n=> [КРИТИЧЕСКАЯ ОШИБКА] Базовое тестирование прервано.\n");
    }
}

void RunBonusTests() {
    printf("\n======================================================\n");
    printf("         ЗАПУСК ТЕСТОВ АЛГОРИТМОВ И БОНУСОВ           \n");
    printf("======================================================\n");
    try {
        TestBitSequence();
        TestAlgorithms();
        printf("\n=> Все тесты алгоритмов успешно выполнены!\n");
    } catch (const Exception& e) {
        printf("\n=> [КРИТИЧЕСКАЯ ОШИБКА] Прервано: %s\n", e.GetMessage());
    } catch (...) {
        printf("\n=> [КРИТИЧЕСКАЯ ОШИБКА] Прервано из-за ошибки.\n");
    }
}