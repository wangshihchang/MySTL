#ifndef _MYSTL_ITERATOR_H_
#define _MYSTL_ITERATOR_H_

#include <cstddef>

#include "type_traits.h"

namespace MySTL {
// 五种迭代器类型
struct input_iterator_tag {};                                              // 输入迭代器，只读不写，单遍扫描，只能递增(++)
struct output_iterator_tag {};                                             // 输出迭代器，只写不读，单遍扫描，只能递增(++)
struct forward_iterator_tag : public input_iterator_tag {};                // 前向迭代器，可读可写，多遍扫描，只能递增(++)
struct bidirectional_iterator_tag : public forward_iterator_tag {};        // 双向迭代器，可读可写，多遍扫描，可递增或递减(++/--)
struct random_access_iterator_tag : public bidirectional_iterator_tag {};  // 随机访问迭代器，可读可写，多遍扫描，支持迭代器全部运算

// 迭代器模板
template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
struct iterator {
    typedef T value_type;                // 迭代器指向元素类型
    typedef Pointer pointer;             // 迭代器所指向元素的指针类型，默认为 T*
    typedef Category iterator_category;  // 迭代器类别
    typedef Distance difference_type;    // 两个迭代器之间的距离的类型，通常是 ptrdiff_t，用于表示两个指针之间的差异，是一个有符号整数类型
    typedef Reference reference;         // 迭代器解引用操作的结果类型，即通过迭代器访问元素的引用类型，默认为T&
};

// iterator traits
template <class T>
struct has_iterator_cat {
   private:
    struct two {
        char a;
        char b;
    };
    template <class U>
    static two test(...);
    template <class U>
    static char test(typename U::iterator_category* = 0);

   public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true> {
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::reference reference;
};

template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true> : public iterator_traits_impl<Iterator,
                                                                            std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
                                                                                std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value> {};

// 萃取迭代器特性
template <class Iterator>
struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

// 针对原生指针的偏特化版本
template <class T>
struct iterator_traits<T*> {
    typedef T value_type;
    typedef T* pointer;
    typedef random_access_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
};

template <class T>
struct iterator_traits<const T*> {
    typedef T value_type;
    typedef const T* pointer;
    typedef random_access_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    typedef const T& reference;
};

template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of : public m_bool_constant<
                                 std::is_convertible<typename iterator_traits<T>::iterator_category, U>::value> {};

// 萃取某种迭代器
template <class T, class U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

template <class Iter>
struct is_input_iterator : has_iterator_cat_of<Iter, input_iterator_tag> {};

template <class Iter>
struct is_output_iterator : has_iterator_cat_of<Iter, output_iterator_tag> {};

template <class Iter>
struct is_forward_iterator : has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator : has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator : has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <class Iterator>
struct is_iterator : public m_bool_constant<
                         is_input_iterator<Iterator>::value ||
                         is_output_iterator<Iterator>::value> {};

// 萃取某个迭代器的 category
template <class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();
}

// 萃取某个迭代器的 distance_type
template <class Iterator>
typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 萃取某个迭代器的 value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 以下函数用于计算迭代器间的距离
// distance 的 input_iterator_tag 的版本
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

// distance 的 random_access_iterator_tag 的版本
template <class RandomIterator>
typename iterator_traits<RandomIterator>::difference_type
distance_dispatch(RandomIterator first, RandomIterator last, random_access_iterator_tag) {
    return last - first;
}

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    return distance_dispatch(first, last, iterator_category(first));
}

// 以下函数用于让迭代器前进 n 个距离
// advance 的 input_iterator_tag 的版本
template <class InputIterator, class Distance>
void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
    while (n--)
        ++i;
}

// advance 的 bidirectional_iterator_tag 的版本
template <class BidirectionalIterator, class Distance>
void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
    if (n >= 0) {
        while (n--)
            ++i;
    } else {
        while (n++)
            --i;
    }
}

// advance 的 random_access_iterator_tag 的版本
template <class RandomIterator, class Distance>
void advance_dispatch(RandomIterator& i, Distance n, random_access_iterator_tag) {
    i += n;
}

template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n) {
    advance_dispatch(i, n, iterator_category(i));
}

/*****************************************************************************************/

// 模板类 : reverse_iterator
// 代表反向迭代器，使前进为后退，后退为前进
template <class Iterator>
class reverse_iterator {
   private:
    Iterator current;  // 记录对应的正向迭代器
   public:
    // 反向迭代器的五种相应型别
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::pointer pointer;
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::reference reference;

    typedef Iterator iterator_type;
    typedef reverse_iterator<Iterator> self;

   public:
    // 构造函数
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type i) : current(i) {}
    reverse_iterator(const self& rhs) : current(rhs.current) {}

   public:
    // 取出对应的正向迭代器
    iterator_type base() const { return current; }

    // 重载操作符
    reference operator*() const {
        auto tmp = current;
        return *--tmp;
    }

    pointer operator->() const { return &(operator*()); }

    // 前进(++)变为后退(--)
    self& operator++() {
        --current;
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        --current;
        return tmp;
    }

    // 后退(--)变为前进(++)
    self& operator--() {
        ++current;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        ++current;
        return tmp;
    }

    self& operator+=(difference_type n) {
        current -= n;
        return *this;
    }

    self operator+(difference_type n) const { return self(current - n); }

    self& operator-=(difference_type n) {
        current += n;
        return *this;
    }

    self operator-(difference_type n) const { return self(current + n); }

    reference operator[](difference_type n) const { return *(*this + n); }
};

// 重载 operator-
template <class Iterator>
typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& lhs,
          const reverse_iterator<Iterator>& rhs) {
    return rhs.base() - lhs.base();
}

// 重载比较操作符
template <class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
    return lhs.base() == rhs.base();
}

template <class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs) {
    return lhs.base > rhs.base();
}

template <class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
    return !(lhs == rhs);
}

template <class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs) {
    return lhs > rhs;
}

template <class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
    return !(rhs < lhs);
}

template <class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
    return !(lhs < rhs);
}
}  // namespace MySTL
#endif