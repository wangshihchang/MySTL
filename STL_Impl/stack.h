#ifndef _MYSTL_STACK_H_
#define _MYSTL_STACK_H_

// 这个头文件包含了一个模板类 stack
// stack : 栈

#include "deque.h"
namespace MySTL {
// 模板类 stack
// 参数一代表数据类型，参数二代表底层容器类型，缺省使用 MySTL::deque 作为底层容器
template <class T, class Container = MySTL::deque<T>>
class stack {
    typedef Container container_type;
    // 使用底层容器的型别
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

    static_assert(std::is_same<T, value_type>::value, "the value_type of Container should be same with T");

   private:
    container_type container_;  // 用底层容器表现 stack

   public:
    // 构造、复制、移动函数
    stack() = default;
    explicit stack(size_type n) : container_(n) {}
    stack(size_type n, const value_type& value) : container_(n, value) {}
    template <class Iter>
    stack(Iter first, Iter last) : container_(first, last) {}
    stack(std::initializer_list<T> ilist) : container_(ilist.begin(), ilist.end()) {}
    stack(const Container& cotainer) : container_(cotainer) {}
    stack(Container&& cotainer) noexcept(std::is_nothrow_move_constructible<Container>::value) : container_(MySTL::move(cotainer)) {}
    stack(const stack& rhs) : container_(rhs.container_) {}
    stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value) : container_(MySTL::move(rhs.container_)) {}

    stack& operator=(const stack& rhs) {
        container_ = rhs.container_;
        return *this;
    }

    stack& operator=(stack&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value) {
        container_ = MySTL::move(rhs.container_);
        return *this;
    }

    stack& operator=(std::initializer_list<T> ilist) {
        container_ = ilist;
        return *this;
    }

    ~stack() = default;

    // 访问元素相关操作
    reference top() { return container_.back(); }
    const_reference top() const { return container_.back(); }

    // 容量相关操作
    bool empty() const noexcept { return container_.empty(); }
    size_type size() const noexcept { return container_.size(); }

    // 修改容器相关操作
    template <class... Args>
    void emplace(Args&&... args) { container_.emplace_back(MySTL::forward<Args>(args)...); }

    void push(const value_type& value) { container_.push_back(value); }
    void push(value_type&& value) { container_.push_back(MySTL::move(value)); }
    void pop() { container_.pop_back(); }

    void clear() {
        while (!empty())
            pop();
    }

    void swap(stack& rhs) noexcept(noexcept(MySTL::swap(container_, rhs.container_))) { MySTL::swap(container_, rhs.container_); }

   public:
    friend bool operator==(const stack& lhs, const stack& rhs) { return lhs.container_ == rhs.container_; }
    friend bool operator<(const stack& lhs, const stack& rhs) { return lhs.container_ < rhs.container_; }
};

// 重载比较操作符
template <class T, class Container>
bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs == rhs;
}

template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs < rhs;
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return !(lhs < rhs);
}

// 重载 MySTL 的 swap
template <class T, class Container>
void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}
}  // namespace MySTL

#endif