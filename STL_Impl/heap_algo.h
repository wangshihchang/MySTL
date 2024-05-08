#ifndef _MYSTL_HEAP_ALGO_H_
#define _MYSTL_HEAP_ALGO_H_

// 这个头文件包含 heap 的四个算法 : push_heap, pop_heap, sort_heap, make_heap

#include "iterator.h"

namespace MySTL {

/*****************************************************************************************/
// push_heap
// 该函数接受两个迭代器，表示一个 heap 容器的首尾，并且新元素已经插入到底部容器的最尾端，调整 heap
/*****************************************************************************************/
template <class RandomIter, class Distance, class T>
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value) {
    auto parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + parent) < value) {
        // 使用 operator<，所以 heap 为 max-heap
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class RandomIter, class Distance>
void push_heap_d(RandomIter first, RandomIter last, Distance*) {
    MySTL::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
}

template <class RandomIter>
void push_heap(RandomIter first, RandomIter last) {
    // 新元素应该已置于底部容器的最尾端
    MySTL::push_heap_d(first, last, distance_type(first));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Distance, class T, class Compare>
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value, Compare comp) {
    auto parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class RandomIter, class Compare, class Distance>
void push_heap_d(RandomIter first, RandomIter last, Distance*, Compare comp) {
    MySTL::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1), comp);
}

template <class RandomIter, class Compare>
void push_heap(RandomIter first, RandomIter last, Compare comp) {
    MySTL::push_heap_d(first, last, distance_type(first), comp);
}

/*****************************************************************************************/
// pop_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，将 heap 的根节点取出放到容器尾部，调整 heap
/*****************************************************************************************/
template <class RandomIter, class T, class Distance>
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value) {
    // 先进行下溯(percolate down)过程
    auto topIndex = holeIndex;
    auto rchild = 2 * holeIndex + 2;
    while (rchild < len) {
        if (*(first + rchild) < *(first + rchild - 1))
            --rchild;
        *(first + holeIndex) = *(first + rchild);
        holeIndex = rchild;
        rchild = 2 * (rchild + 1);
    }
    if (rchild == len) {  // 如果没有右子节点
        *(first + holeIndex) = *(first + (rchild - 1));
        holeIndex = rchild - 1;
    }
    // 再执行一次上溯(percolate up)过程
    MySTL::push_heap_aux(first, holeIndex, topIndex, value);
}

template <class RandomIter, class T, class Distance>
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*) {
    // 先将首值调至尾节点，然后调整[first, last - 1)使之重新成为一个 max-heap
    *result = *first;
    MySTL::adjust_heap(first, static_cast<Distance>(0), last - first, value);
}

template <class RandomIter>
void pop_heap(RandomIter first, RandomIter last) {
    MySTL::pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class T, class Distance, class Compare>
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value, Compare comp) {
    // 先进行下溯(percolate down)过程
    auto topIndex = holeIndex;
    auto rchild = 2 * holeIndex + 2;
    while (rchild < len) {
        if (comp(*(first + rchild), *(first + rchild - 1))) --rchild;
        *(first + holeIndex) = *(first + rchild);
        holeIndex = rchild;
        rchild = 2 * (rchild + 1);
    }
    if (rchild == len) {
        *(first + holeIndex) = *(first + (rchild - 1));
        holeIndex = rchild - 1;
    }
    // 再执行一次上溯(percolate up)过程
    MySTL::push_heap_aux(first, holeIndex, topIndex, value, comp);
}

template <class RandomIter, class T, class Distance, class Compare>
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result,
                  T value, Distance*, Compare comp) {
    *result = *first;  // 先将尾指设置成首值，即尾指为欲求结果
    MySTL::adjust_heap(first, static_cast<Distance>(0), last - first, value, comp);
}

template <class RandomIter, class Compare>
void pop_heap(RandomIter first, RandomIter last, Compare comp) {
    MySTL::pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first), comp);
}

/*****************************************************************************************/
// sort_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，不断执行 pop_heap 操作，直到首尾最多相差1
/*****************************************************************************************/
template <class RandomIter>
void sort_heap(RandomIter first, RandomIter last) {
    // 每执行一次 pop_heap，最大的元素都被放到尾部，直到容器最多只有一个元素，完成排序
    while (last - first > 1) {
        MySTL::pop_heap(first, last--);
    }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Compare>
void sort_heap(RandomIter first, RandomIter last, Compare comp) {
    while (last - first > 1) {
        MySTL::pop_heap(first, last--, comp);
    }
}

/*****************************************************************************************/
// make_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，把容器内的数据变为一个 heap
/*****************************************************************************************/
template <class RandomIter, class Distance>
void make_heap_aux(RandomIter first, RandomIter last, Distance*) {
    if (last - first < 2) return;
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;
    while (true) {
        // 重排以 holeIndex 为首的子树
        MySTL::adjust_heap(first, holeIndex, len, *(first + holeIndex));
        if (holeIndex == 0) return;
        holeIndex--;
    }
}

template <class RandomIter>
void make_heap(RandomIter first, RandomIter last) {
    MySTL::make_heap_aux(first, last, distance_type(first));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Distance, class Compare>
void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compare comp) {
    if (last - first < 2) return;
    auto len = last - first;
    auto holeIndex = (len - 2) / 2;
    while (true) {
        // 重排以 holeIndex 为首的子树
        MySTL::adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
        if (holeIndex == 0) return;
        holeIndex--;
    }
}

template <class RandomIter, class Compare>
void make_heap(RandomIter first, RandomIter last, Compare comp) {
    MySTL::make_heap_aux(first, last, distance_type(first), comp);
}
}  // namespace MySTL
#endif