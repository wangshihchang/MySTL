#ifndef _MYSTL_CONSTRUCT_H_
#define _MYSTL_CONSTRUCT_H_

// 这个头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include <new>

#include "iterator.h"
#include "type_traits.h"

namespace MySTL {
// construct 构造对象
template <class T>
void construct(T* ptr) {
    ::new ((void*)ptr) T();
}

template <class T1, class T2>
void construct(T1* ptr, const T2& value) {
    ::new ((void*)ptr) T1(value);
}

template <class T, class... Args>
void construct(T* ptr, Args&&... args) {
    ::new ((void*)ptr) T(MySTL::forward<Args>(args)...);
}

// destroy 将对象析构
template <class T>
void destroy_one(T*, std::true_type) {}

template <class T>
void destroy_one(T* ptr, std::false_type) {
    if (ptr != nullptr)
        ptr->~T();
}

template <class ForwardIter>
void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
    for (; first != last; ++first)
        destroy(&*first);
}

template <class T>
void destroy(T* ptr) {
    destroy_one(ptr, std::is_trivially_destructible<T>{});
}

template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last) {
    destroy_cat(first, last, std::is_trivially_destructible<typename iterator_traits<ForwardIter>::value_type>{});
}
}  // namespace MySTL
#endif