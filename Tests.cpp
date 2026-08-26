#include "Tests.h"
#include <stdio.h>
#include <time.h>

#include "ArraySequence.h"
#include "ImmutableArraySequence.h"
#include "ListSequence.h"
#include "ImmutableListSequence.h"
#include "BitSequence.h"
#include "UniquePtr.h"
#include "Exceptions.h"
#include "Algorithms.h"




int MultiplyByTwo(const int& val) {
    return val * 2;
}

bool IsEven(const int& val) {
    return val % 2 == 0;
}

bool IsZero(const int& val) {
    return val == 0;
}

int SumReducerTest(const int& acc, const int& val) {
    return acc + val;
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

void TestArraySequence() {
    UniquePtr<Sequence<int>> seq(new ArraySequence<int>());
    seq->Append(10);
    seq->Prepend(5);
    seq->InsertAt(15, 2); // [5, 10, 15]

    Assert(seq->GetLength() == 3, "ArraySequence (Pos): Ошибка длины");
    Assert(seq->GetFirst() == 5, "ArraySequence (Pos): Ошибка GetFirst");
    Assert(seq->GetLast() == 15, "ArraySequence (Pos): Ошибка GetLast");
    Assert(seq->Get(1) == 10, "ArraySequence (Pos): Ошибка Get");

    UniquePtr<Sequence<int>> sub(seq->GetSubsequence(1, 2)); // [10, 15]
    Assert(sub->GetLength() == 2, "ArraySequence (Pos): Длина подпоследовательности");
    Assert(sub->Get(0) == 10, "ArraySequence (Pos): Значение подпоследовательности");

    UniquePtr<Sequence<int>> seq2(new ArraySequence<int>());
    seq2->Append(99);
    UniquePtr<Sequence<int>> concat(seq->Concat(seq2.Get())); // [5, 10, 15, 99]
    Assert(concat->GetLength() == 4, "ArraySequence (Pos): Длина после Concat");
    Assert(concat->GetLast() == 99, "ArraySequence (Pos): Значение после Concat");

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

    UniquePtr<Sequence<int>> emptySeq(new ArraySequence<int>());
    caught = false;
    try {
        emptySeq->GetFirst();
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ArraySequence (Neg): Нет исключения при GetFirst() на пустом массиве");

    PrintSuccess("ArraySequence (Все операции)");
}

void TestListSequence() {
    UniquePtr<Sequence<int>> seq(new ListSequence<int>());
    seq->Append(10);
    seq->Prepend(5);
    seq->InsertAt(15, 2);

    Assert(seq->GetLength() == 3, "ListSequence (Pos): Ошибка длины");
    Assert(seq->GetFirst() == 5, "ListSequence (Pos): Ошибка GetFirst");

    UniquePtr<Sequence<int>> sub(seq->GetSubsequence(1, 2));
    Assert(sub->GetLength() == 2 && sub->GetFirst() == 10, "ListSequence (Pos): Ошибка GetSubsequence");

    UniquePtr<Sequence<int>> seq2(new ListSequence<int>());
    seq2->Append(99);
    UniquePtr<Sequence<int>> concat(seq->Concat(seq2.Get()));
    Assert(concat->GetLength() == 4 && concat->GetLast() == 99, "ListSequence (Pos): Ошибка Concat");

    bool caught = false;
    try {
        seq->Get(-5);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ListSequence (Neg): Нет исключения при отрицательном индексе");

    caught = false;
    try {
        seq->GetSubsequence(2, 1);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ListSequence (Neg): Нет исключения при startIndex > endIndex");

    UniquePtr<Sequence<int>> emptySeq(new ListSequence<int>());
    caught = false;
    try {
        emptySeq->GetFirst();
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ListSequence (Neg): Нет исключения при GetFirst() на пустом списке");

    PrintSuccess("ListSequence (Все операции)");
}


void TestImmutable() {
    UniquePtr<Sequence<int>> oldArr(new ImmutableArraySequence<int>());
    UniquePtr<Sequence<int>> newArr(oldArr->Append(10));
    Assert(oldArr->GetLength() == 0, "ImmutableArray: Оригинал изменился!");
    Assert(newArr->GetLength() == 1 && newArr->Get(0) == 10, "ImmutableArray: Ошибка создания копии");

    UniquePtr<Sequence<int>> oldList(new ImmutableListSequence<int>());
    UniquePtr<Sequence<int>> newList(oldList->Prepend(42));
    Assert(oldList->GetLength() == 0, "ImmutableList: Оригинал изменился!");

    UniquePtr<Sequence<int>> concatRes(newArr->Concat(newList.Get()));
    Assert(concatRes->GetLength() == 2, "Immutable Concat: Ошибка длины");
    Assert(concatRes->Get(1) == 42, "Immutable Concat: Ошибка значения");
    Assert(newArr->GetLength() == 1, "Immutable Concat: Оригинал изменился!");

    bool caught = false;
    try {
        newArr->Get(100);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ImmutableArray (Neg): Нет защиты границ при Get()");

    caught = false;
    try {
        oldList->GetFirst();
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "ImmutableList (Neg): Нет исключения при GetFirst() на пустом");

    PrintSuccess("Неизменяемые коллекции (Immutable)");
}


void TestIterators() {
    UniquePtr<Sequence<int>> seq(new ArraySequence<int>());
    seq->Append(1);
    seq->Append(2);
    seq->Append(3);

    UniquePtr<Iterator<int>> it(seq->GetIterator());
    int sum = 0;
    while (it->HasNext()) {
        sum += it->Next();
    }
    Assert(sum == 6, "Итератор: неверная сумма элементов");

    it->Reset();
    Assert(it->HasNext() == true && it->Next() == 1, "Итератор: сброс (Reset) не удался");

    PrintSuccess("Паттерн Итератор");
}

void TestBitSequence() {
    BitSequence seq1;
    seq1.Append(Bit(1));
    seq1.Append(Bit(0));

    BitSequence seq2;
    seq2.Append(Bit(1));
    seq2.Append(Bit(1));

    UniquePtr<BitSequence> andResult(seq1.And(seq2));
    Assert(andResult->Get(0).Get() == 1 && andResult->Get(1).Get() == 0, "BitSequence: Ошибка логического AND");

    bool caught = false;
    try {
        seq1.Get(100);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "BitSequence (Neg): Нет исключения при выходе за границы");

    PrintSuccess("Битовые последовательности (BitSequence)");
}

void TestAlgorithms() {
    UniquePtr<ArraySequence<int>> seq(new ArraySequence<int>());
    seq->Append(10);
    seq->Append(5);
    seq->Append(20);
    seq->Append(15);
    seq->Append(30);

    ArraySequence<int> emptySeq;
    bool caught = false;

    Stats s = GetSequenceStats(*seq);
    Assert(s.min == 5 && s.max == 30 && s.avg == 16.0f, "GetSequenceStats (Pos): неверные расчеты");

    caught = false;
    try {
        GetSequenceStats(emptySeq);
    } catch (const Exception&) {
        caught = true;
    }
    Assert(caught, "GetSequenceStats (Neg): Нет исключения на пустом списке");

    Assert(GetMedian(*seq) == 15.0, "GetMedian (Pos): неверная медиана (нечет)");

    UniquePtr<ArraySequence<int>> evenSeq(new ArraySequence<int>());
    evenSeq->Append(10);
    evenSeq->Append(20);
    Assert(GetMedian(*evenSeq) == 15.0, "GetMedian (Pos): неверная медиана (чет)");

    UniquePtr<ArraySequence<int>> mirror(GetMirrorSums(*seq));
    Assert(mirror->Get(0) == 40 && mirror->Get(2) == 40, "GetMirrorSums (Pos): ошибка расчета");

    UniquePtr<ArraySequence<int>> seq2(new ArraySequence<int>());
    seq2->Append(1);
    seq2->Append(2);
    UniquePtr<ArraySequence<Tuple<int, int>>> zipped(ZipSequences(*seq, *seq2));
    Assert(zipped->GetLength() == 2, "ZipSequences: не обрезает по короткому списку");

    UniquePtr<ArraySequence<int>> itemsToInsert(new ArraySequence<int>());
    itemsToInsert->Append(99);
    UniquePtr<Sequence<int>> sliced(Slice<int>(seq.Get(), 1, 2, itemsToInsert.Get()));
    Assert(sliced->GetLength() == 4 && sliced->Get(1) == 99, "Slice: элемент не вставился");

    UniquePtr<Sequence<int>> mappedSeq(Map<int, int>(seq.Get(), MultiplyByTwo));
    Assert(mappedSeq->Get(0) == 20, "Map (Pos): ошибка преобразования");

    UniquePtr<Sequence<int>> filteredSeq(Where<int>(seq.Get(), IsEven));
    Assert(filteredSeq->GetLength() == 3, "Where (Pos): ошибка фильтрации");

    Assert(Reduce<int, int>(seq.Get(), SumReducerTest, 0) == 80, "Reduce (Pos): ошибка свертки");

    UniquePtr<ArraySequence<int>> splitBase(new ArraySequence<int>());
    splitBase->Append(1);
    splitBase->Append(0);
    splitBase->Append(2);
    UniquePtr<Sequence<Sequence<int>*>> splitResult(Split<int>(*splitBase, IsZero));
    Assert(splitResult->GetLength() == 2, "Split (Pos): неверное количество частей");

    for (int i = 0; i < splitResult->GetLength(); ++i) {
        delete splitResult->Get(i);
    }

    PrintSuccess("Алгоритмы (Map, Reduce, Where, Zip, Slice, Split, Stats, Median)");
}


void ComparePerformance() {
    printf("\n--- СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ (10000 элементов) ---\n");
    int count = 10000;

    clock_t start = clock();
    UniquePtr<Sequence<int>> mutArray(new ArraySequence<int>());
    for (int i = 0; i < count; ++i) {
        Sequence<int>* next = mutArray->Append(i);
        if (next != mutArray.Get()) {
            mutArray.Reset(next);
        }
    }
    clock_t end = clock();
    printf("Изменяемая (Mutable Array):    %.2f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);

    start = clock();
    UniquePtr<Sequence<int>> immutArray(new ImmutableArraySequence<int>());
    for (int i = 0; i < count; ++i) {
        Sequence<int>* next = immutArray->Append(i);
        if (next != immutArray.Get()) {
            immutArray.Reset(next);
        }
    }
    end = clock();
    printf("Неизменяемая (Immutable Array): %.2f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);

    start = clock();
    UniquePtr<Sequence<int>> mutList(new ListSequence<int>());
    for (int i = 0; i < count; ++i) {
        Sequence<int>* next = mutList->Append(i);
        if (next != mutList.Get()) {
            mutList.Reset(next);
        }
    }
    end = clock();
    printf("Изменяемая (Mutable List):     %.2f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);

    start = clock();
    UniquePtr<Sequence<int>> immutList(new ImmutableListSequence<int>());
    for (int i = 0; i < count; ++i) {
        Sequence<int>* next = immutList->Append(i);
        if (next != immutList.Get()) {
            immutList.Reset(next);
        }
    }
    end = clock();
    printf("Неизменяемая (Immutable List):  %.2f мс\n", ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0);
}

void RunCoreTests() {
    printf("         ЗАПУСК БАЗОВЫХ ТЕСТОВ КОЛЛЕКЦИЙ              \n");
    try {
        TestArraySequence();
        TestListSequence();
        TestImmutable();
        TestIterators();
        printf("\n=> Все базовые тесты успешно выполнены!\n");
    } catch (...) {
        printf("\n=> [КРИТИЧЕСКАЯ ОШИБКА] Базовое тестирование прервано.\n");
    }
}

void RunBonusTests() {
    printf("         ЗАПУСК ТЕСТОВ АЛГОРИТМОВ И БОНУСОВ           \n");
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