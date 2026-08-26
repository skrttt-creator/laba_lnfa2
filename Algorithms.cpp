#include "Algorithms.h"

Stats StatsReducer(const Stats& acc, const int& val) {
    Stats res = acc;

    if (val < res.min) {
        res.min = val;
    }

    if (val > res.max) {
        res.max = val;
    }

    res.sum += val;
    res.count += 1;
    res.avg = (res.sum * 1.0) / res.count;

    return res;
}

Stats GetSequenceStats(const ArraySequence<int>& seq) {
    if (seq.GetLength() == 0) {
        throw IndexOutOfRange("Пустая последовательность");
    }

    Stats initial;
    initial.min = seq.Get(0);
    initial.max = seq.Get(0);
    initial.sum = 0;
    initial.count = 0;
    initial.avg = 0.0;

    return Reduce(&seq, StatsReducer, initial);
}

void QuickSort(ArraySequence<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                // Ручной обмен элементов (swap), так как std::swap использовать нельзя
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;

        QuickSort(arr, low, pi - 1);
        QuickSort(arr, pi + 1, high);
    }
}

double GetMedian(const ArraySequence<int>& seq) {
    int len = seq.GetLength();

    if (len == 0) {
        throw IndexOutOfRange("Пустая последовательность");
    }

    ArraySequence<int> copy(seq);

    QuickSort(copy, 0, len - 1);

    if (len % 2 == 1) {
        return copy[len / 2] * 1.0;
    }

    return (copy[len / 2 - 1] + copy[len / 2]) / 2.0;
}

int MirrorSumMapper(const int& val, int index, const Sequence<int>* seq) {
    return val + seq->Get(seq->GetLength() - 1 - index);
}

ArraySequence<int>* GetMirrorSums(const ArraySequence<int>& seq) {
    return (ArraySequence<int>*)MapContext(&seq, MirrorSumMapper);
}