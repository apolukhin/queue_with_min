#ifndef EXAMPLES_QUEUE_WITH_MIN_HPP
#define EXAMPLES_QUEUE_WITH_MIN_HPP

#include <boost/config.hpp>
#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#include <deque>
#include <boost/heap/fibonacci_heap.hpp>


namespace examples {

template <class T>
class queue_with_min {
    typedef const T* data_ptr_t;

    struct comparator {
        bool operator()(data_ptr_t lhs, data_ptr_t rhs) const noexcept {
            return *rhs < *lhs;
        }
    };

    std::deque<T>                   data_;
    // fibonacci_heap - O(1): top(), push()
    boost::heap::fibonacci_heap<data_ptr_t, boost::heap::compare<comparator> > min_;

    data_ptr_t pointer_to_last() const noexcept {
        return std::addressof(data_.back());
    }

    data_ptr_t pointer_to_first() const {
        return std::addressof(data_.front());
    }

public:
    typedef T value_type;

    /// \b Complexity: O(1)
    ///
    /// \b Note:
    /// fibonacci_heap() does not throw, but is not marked with `noexcept`.
    /// Need to define move contructor/assignments and default constructors, instead of writing `= default`
    ///
    /// https://github.com/boostorg/heap/issues/5
    queue_with_min() noexcept {};

    /**
    \b Complexity: O(1)

    \b Note: After deque move assignment/construction (unless elementwise move assignment is forced by incompatible allocators)
    references, pointers, and iterators (other than the end iterator) to other remain valid, but refer to elements that
    are now in *this. The current standard makes this guarantee via the blanket statement in
    §23.2.1[container.requirements.general]/12, and a more direct guarantee is under consideration via LWG 2321
    */
    queue_with_min(queue_with_min&& q) noexcept
        : data_(std::move(q.data_))
        , min_(std::move(q.min_))
    {}

    queue_with_min(const queue_with_min& q)
        : data_(q.data_)
    {
        const auto end = data_.cend();
        for (auto it = data_.cbegin(); it != end; ++it) {
            min_.push(std::addressof(*it));
        }
    }

    queue_with_min(std::initializer_list<value_type> il) {
        for (auto&& v : il) {
            push_back(v);
        }
    }

    queue_with_min& operator=(queue_with_min&& q) noexcept {
        if (this == &q) {
            return *this;
        }

        data_ = std::move(q.data_);
        min_ = std::move(q.min_);
        return *this;
    }

    queue_with_min& operator=(const queue_with_min& q) {
        if (this == &q) {
            return *this;
        }

        clear();
        for (auto&& v : q.data_) {
            push_back(v);
        }

        return *this;
    }

    queue_with_min& operator=(std::initializer_list<value_type> il) {
        clear();
        for (auto&& v : il) {
            push_back(v);
        }

        return *this;
    }

    // back

    /// \b Complexity: O(1)
    void push_back(value_type&& v) {
        emplace_back(std::move(v));
    }

    /// \b Complexity: O(1)
    void push_back(const value_type& v) {
        emplace_back(v);
    }

    /// \b Complexity: O(1)
    template <class... Args>
    void emplace_back(Args&&... args) {
        data_.emplace_back(std::forward<Args>(args)...);
        min_.push(pointer_to_last());
    }

    /// \b Complexity: O(N)
    void pop_back() {
        auto it = std::find(min_.begin(), min_.end(), pointer_to_last());
        min_.erase(
            min_.s_handle_from_iterator(it)
        );
        data_.pop_back();
    }

    /// \b Complexity: O(1)
    const value_type& back() const {
        return data_.back();
    }


    // front

    /// \b Complexity: O(1)
    void push_front(value_type&& v) {
        emplace_front(std::move(v));
    }

    /// \b Complexity: O(1)
    void push_front(const value_type& v) {
        emplace_front(v);
    }

    /// \b Complexity: O(1)
    template <class... Args>
    void emplace_front(Args&&... args) {
        data_.emplace_front(std::forward<Args>(args)...);
        min_.push(pointer_to_first());
    }

    /// \b Complexity: O(N)
    void pop_front() {
        auto it = std::find(min_.begin(), min_.end(), pointer_to_first());
        min_.erase(
            min_.s_handle_from_iterator(it)
        );
        data_.pop_front();
    }

    /// \b Complexity: O(1)
    const value_type& front() const {
        return data_.front();
    }


    // misc
    /// \b Complexity: O(1)
    std::size_t size() const noexcept {
        return data_.size();
    }

    /// \b Complexity: O(1)
    bool empty() const noexcept {
        return data_.empty();
    }

    /// \b Complexity: O(1)
    const value_type& min() const {
        return *min_.top();
    }

    /// \b Complexity: O(N)
    bool equal(const queue_with_min<T>& q) const noexcept {
        return data_ == q.data_;
    }

    /// \b Complexity: O(N), in case of POD type up to O(1)
    void clear() noexcept {
        min_.clear();
        data_.clear();
    }
};

template <class T>
inline bool operator==(const queue_with_min<T>& lhs, const queue_with_min<T>& rhs) noexcept {
    return lhs.equal(rhs);
}

} // namespace examples

#endif // EXAMPLES_QUEUE_WITH_MIN_HPP
