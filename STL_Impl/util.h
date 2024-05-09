#ifndef _MYSTL_UTIL_H_
#define _MYSTL_UTIL_H_

// 这个文件包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等

#include <cstddef>
#include <type_traits>
namespace MySTL {
// move
template <class T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

// forward
template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
    return static_cast<T&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward!");
    return static_cast<T&&>(arg);
}

// swap
template <class T>
void swap(T& lhs, T& rhs) {
    auto tmp(MySTL::move(lhs));
    lhs = MySTL::move(rhs);
    rhs = MySTL::move(tmp);
}

template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2) {
    for (; first1 != last1; ++first1, (void)++first2) {
        MySTL::swap(*first1, *first2);
    }
    return first2;
}

template <class T, size_t n>
void swap(T (&a)[n], T (&b)[n]) {
    MySTL::swap_range(a, a + n, b);
}

// pair
template <class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    first_type first;
    second_type second;

    // 默认构造
    template <class Other1 = T1, class Other2 = T2,
              typename = typename std::enable_if<
                  std::is_default_constructible<Other1>::value &&
                      std::is_default_constructible<Other2>::value,
                  void>::type>
    constexpr pair() : first(), second() {}

    // 当前类型的隐式构造
    template <class U1 = T1, class U2 = T2,
              typename std::enable_if<
                  std::is_copy_constructible<U1>::value &&
                      std::is_copy_constructible<U2>::value &&
                      std::is_convertible<const U1&, T1>::value &&
                      std::is_convertible<const U2&, T2>::value,
                  int>::type = 0>
    constexpr pair(const T1& a, const T2& b) : first(a), second(b) {}

    // 当前类型的显式构造
    template <class U1 = T1, class U2 = T2,
              typename std::enable_if<
                  std::is_copy_constructible<U1>::value &&
                      std::is_copy_constructible<U2>::value &&
                      (!std::is_convertible<const U1&, T1>::value ||
                       !std::is_convertible<const U2&, T2>::value),
                  int>::type = 0>
    explicit constexpr pair(const T1& a, const T2& b) : first(a), second(b) {}

    pair(const pair& rhs) = default;
    pair(pair&& rhs) = default;

    // 其他类型的隐式构造
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<T1, Other1>::value &&
                      std::is_constructible<T2, Other2>::value &&
                      std::is_convertible<Other1&&, T1>::value &&
                      std::is_convertible<Other2&&, T2>::value,
                  int>::type = 0>
    constexpr pair(Other1&& a, Other2&& b) : first(MySTL::forward<Other1>(a)), second(MySTL::forward<Other2>(b)) {}

    // 其他类型的显式构造
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<T1, Other1>::value &&
                      std::is_constructible<T2, Other2>::value &&
                      (!std::is_convertible<Other1, T1>::value ||
                       !std::is_convertible<Other2, T2>::value),
                  int>::type = 0>
    explicit constexpr pair(Other1&& a, Other2&& b) : first(MySTL::forward<Other1>(a)), second(MySTL::forward<Other2>(b)) {}

    // 其他类型pair的隐式构造
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<T1, const Other1&>::value &&
                      std::is_constructible<T2, const Other2&>::value &&
                      std::is_convertible<const Other1&, T1>::value &&
                      std::is_convertible<const Other2&, T2>::value,
                  int>::type = 0>
    constexpr pair(const pair<Other1, Other2>& other) : first(other.first), second(other.second) {}

    // 其他类型pair的显式构造
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<T1, const Other1&>::value &&
                      std::is_constructible<T2, const Other2&>::value &&
                      (!std::is_convertible<const Other1&, T1>::value ||
                       !std::is_convertible<const Other2&, T2>::value),
                  int>::type = 0>
    explicit constexpr pair(const pair<Other1, Other2>& other) : first(other.first), second(other.second) {}

    // 其他类型pair的隐式构造
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<T1, Other1>::value &&
                      std::is_constructible<T2, Other2>::value &&
                      std::is_convertible<Other1, T1>::value &&
                      std::is_convertible<Other2, T2>::value,
                  int>::type = 0>
    constexpr pair(pair<Other1, Other2>&& other) : first(MySTL::forward<Other1>(other.first)), second(MySTL::forward<Other2>(other.second)) {}

    // 其他类型pair的显式构造
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<T1, Other1>::value &&
                      std::is_constructible<T2, Other2>::value &&
                      (!std::is_convertible<Other1, T1>::value ||
                       !std::is_convertible<Other2, T2>::value),
                  int>::type = 0>
    explicit constexpr pair(pair<Other1, Other2>&& other) : first(MySTL::forward<Other1>(other.first)), second(MySTL::forward<Other2>(other.second)) {}

    // 同类型pair的拷贝赋值
    pair& operator=(const pair& rhs) {
        if (this != &rhs) {
            first = rhs.first;
            second = rhs.second;
        }
        return *this;
    }

    // 同类型pair的移动赋值
    pair& operator=(pair&& rhs) {
        if (this != &rhs) {
            first = MySTL::move(rhs.first);
            second = MySTL::move(rhs.second);
        }
        return *this;
    }

    // 其他类型pair的拷贝赋值
    template <class Other1, class Other2>
    pair& operator=(const pair<Other1, Other2>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    // 其他类型pair的移动赋值
    template <class Other1, class Other2>
    pair& operator=(pair<Other1, Other2>&& other) {
        first = MySTL::forward<Other1>(other.first);
        second = MySTL::forward<Other1>(other.second);
        return *this;
    }

    ~pair() = default;

    void swap(pair& other) {
        if (this != &other) {
            MySTL::swap(first, other.first);
            MySTL::swap(second, other.second);
        }
    }
};

// 重载比较操作符
template <class T1, class T2>
bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class T1, class T2>
bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
}

template <class T1, class T2>
bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs == rhs);
}

template <class T1, class T2>
bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return rhs < lhs;
}

template <class T1, class T2>
bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs > rhs);
}

template <class T1, class T2>
bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs < rhs);
}

// 重载 MySTL 的 swap
template <class T1, class T2>
void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs) {
    lhs.swap(rhs);
}

// make_pair
template <class T1, class T2>
pair<T1, T2> make_pair(T1&& first, T2&& second) {
    return pair<T1, T2>(MySTL::forward<T1>(first), MySTL::forward<T2>(second));
}
}  // namespace MySTL
#endif