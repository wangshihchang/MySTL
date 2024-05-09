#ifndef _MYSTL_QUEUE_H_
#define _MYSTL_QUEUE_H_

// 这个头文件包含了两个模板类 queue 和 priority_queue
// queue          : 队列
// priority_queue : 优先队列

#include "deque.h"
#include "functional.h"
#include "heap_algo.h"
#include "vector.h"

namespace MySTL {
// 模板类 queue
// 参数一代表数据类型，参数二代表底层容器类型，缺省使用 MySTL::deque 作为底层容器
template <class T, class Container = MySTL::deque<T>>
class queue {
   public:
    typedef Container container_type;
    // 使用底层容器的型别
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

    static_assert(std::is_same<T, value_type>::value, "the value_type of Container should be same with T");

   private:
    container_type container_;  // 用底层容器表现 queue

   public:
    // 构造、复制、移动函数
    queue() = default;
    explicit queue(size_type n) : container_(n) {}
    queue(size_type n, const value_type& value) : container_(n, value) {}
    queue(std::initializer_list<T> ilist) : container_(ilist.begin(), ilist.end()) {}
    queue(const Container& c) : container_(c) {}
    queue(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value)
        : container_(MySTL::move(c)) {}
    queue(const queue& rhs) : container_(rhs.container_) {}
    queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
        : container_(MySTL::move(rhs.container_)) {}

    queue& operator=(const queue& rhs) {
        container_ = rhs.container_;
        return *this;
    }

    queue& operator=(queue&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value) {
        container_ = MySTL::move(rhs.container_);
        return *this;
    }

    queue& operator=(std::initializer_list<T> ilist) {
        container_ = ilist;
        return *this;
    }

    ~queue() = default;

    // 访问元素相关操作
    reference front() { return container_.front(); }
    const_reference front() const { return container_.front(); }
    reference back() { return container_.back(); }
    const_reference back() const { return container_.back(); }

    // 容量相关操作
    bool empty() const noexcept { return container_.empty(); }
    size_type size() const noexcept { return container_.size(); }

    // 修改容器相关操作
    template <class... Args>
    void emplace(Args&&... args) { container_.emplace_back(MySTL::forward<Args>(args)...); }

    void push(const value_type& value) { container_.push_back(value); }
    void push(value_type&& value) { container_.emplace_back(MySTL::move(value)); }
    void pop() { container_.pop_front(); }

    void clear() {
        while (!empty())
            pop();
    }

    void swap(queue& rhs) noexcept(noexcept(MySTL::swap(container_, rhs.container_))) { MySTL::swap(container_, rhs.container_); }

   public:
    friend bool operator==(const queue& lhs, const queue& rhs) { return lhs.container_ == rhs.container_; }
    friend bool operator<(const queue& lhs, const queue& rhs) { return lhs.container_ < rhs.container_; }
};

// 重载比较操作符
template <class T, class Container>
bool operator==(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs) {
    return lhs == rhs;
}

template <class T, class Container>
bool operator!=(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(const queue<T, Container>& lhs,
               const queue<T, Container>& rhs) {
    return lhs < rhs;
}

template <class T, class Container>
bool operator>(const queue<T, Container>& lhs,
               const queue<T, Container>& rhs) {
    return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs) {
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const queue<T, Container>& lhs,
                const queue<T, Container>& rhs) {
    return !(lhs < rhs);
}

// 重载 MySTL 的 swap
template <class T, class Container>
void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

/*****************************************************************************************/
// 模板类 priority_queue
// 参数一代表数据类型，参数二代表容器类型，缺省使用 MySTL::vector 作为底层容器
// 参数三代表比较权值的方式，缺省使用 MySTL::less 作为比较方式
template <class T, class Container = MySTL::vector<T>, class Compare = MySTL::less<typename Container::value_type>>
class priority_queue {
   public:
    typedef Container container_type;
    typedef Compare value_compare;

    // 使用底层容器的型别
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

    static_assert(std::is_same<T, value_type>::value, "the value_type of Container should be same with T");

   private:
    container_type container_;  // 用底层容器来表现 priority_queue
    value_compare comp_;        // 权值比较的标准

   public:
    // 构造、复制、移动函数
    priority_queue() = default;
    priority_queue(const Compare& c) : container_(), comp_(c) {}
    explicit priority_queue(size_type n) : container_(n) { MySTL::make_heap(container_.begin(), container_.end(), comp_); }
    template <class Iter>
    priority_queue(Iter first, Iter last) : container_(first, last) { MySTL::make_heap(container_.begin(), container_.end(), comp_); }
    priority_queue(std::initializer_list<T> ilist) : container_(ilist) { MySTL::make_heap(container_.begin(), container_.end(), comp_); }
    priority_queue(const Container& s) : container_(s) { MySTL::make_heap(container_.begin(), container_.end(), comp_); }
    priority_queue(Container&& s) : container_(MySTL::move(s)) { MySTL::make_heap(container_.begin(), container_.end(), comp_); }
    priority_queue(const priority_queue& rhs) : container_(rhs.container_), comp_(rhs.comp_) {
        MySTL::make_heap(container_.begin(), container_.end(), comp_);
    }
    priority_queue(priority_queue&& rhs) : container_(MySTL::move(rhs.container_)), comp_(rhs.comp_) {
        MySTL::make_heap(container_.begin(), container_.end(), comp_);
    }

    priority_queue& operator=(const priority_queue& rhs) {
        container_ = rhs.container_;
        comp_ = rhs.comp_;
        MySTL::make_heap(container_.begin(), container_.end(), comp_);
        return *this;
    }

    priority_queue& operator=(priority_queue&& rhs) {
        container_ = MySTL::move(rhs.container_);
        comp_ = rhs.comp_;
        MySTL::make_heap(container_.begin(), container_.end(), comp_);
        return *this;
    }

    priority_queue& operator=(std::initializer_list<T> ilist) {
        container_ = ilist;
        comp_ = value_compare();
        MySTL::make_heap(container_.begin(), container_.end(), comp_);
        return *this;
    }

    ~priority_queue() = default;

   public:
    // 访问元素相关操作
    const_reference top() const noexcept { return container_.front(); }

    // 容量相关操作
    bool empty() const noexcept { return container_.empty(); }
    size_type size() const noexcept { return container_.size(); }

    // 修改容器相关操作
    template <class... Args>
    void emplace(Args&&... args) {
        container_.emplace_back(MySTL::forward<Args>(args)...);
        MySTL::push_heap(container_.begin(), container_.end(), comp_);
    }

    void push(const value_type& value) {
        container_.push_back(value);
        MySTL::push_heap(container_.begin(), container_.end(), comp_);
    }

    void push(value_type&& value) {
        container_.push_back(MySTL::move(value));
        MySTL::push_heap(container_.begin(), container_.end(), comp_);
    }

    void pop() {
        MySTL::pop_heap(container_.begin(), container_.end(), comp_);
        container_.pop_back();
    }

    void clear() {
        while (!empty())
            pop();
    }

    void swap(priority_queue& rhs) noexcept(noexcept(MySTL::swap(container_, rhs.container_)) &&
                                            noexcept(MySTL::swap(comp_, rhs.comp_))) {
        MySTL::swap(container_, rhs.container_);
        MySTL::swap(comp_, rhs.comp_);
    }

   public:
    friend bool operator==(const priority_queue& lhs, const priority_queue& rhs) { return lhs.container_ == rhs.container_; }
    friend bool operator!=(const priority_queue& lhs, const priority_queue& rhs) { return lhs.container_ != rhs.container_; }
};

// 重载比较操作符
template <class T, class Container, class Compare>
bool operator==(priority_queue<T, Container, Compare>& lhs,
                priority_queue<T, Container, Compare>& rhs) {
    return lhs == rhs;
}

template <class T, class Container, class Compare>
bool operator!=(priority_queue<T, Container, Compare>& lhs,
                priority_queue<T, Container, Compare>& rhs) {
    return lhs != rhs;
}

// 重载 mystl 的 swap
template <class T, class Container, class Compare>
void swap(priority_queue<T, Container, Compare>& lhs,
          priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

}  // namespace MySTL
#endif