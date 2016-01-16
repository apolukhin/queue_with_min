#ifndef EXAMPLES_QUEUE_WITH_MIN_HPP
#define EXAMPLES_QUEUE_WITH_MIN_HPP

#include <boost/config.hpp>
#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#include <list>
#include <algorithm>


namespace examples {

template <class T>
class queue_with_min {
    typedef typename std::list<T>::const_iterator data_ptr_t;
    std::list<T>    data_;
    data_ptr_t      min_;

    data_ptr_t pointer_to_last() const noexcept {
        return std::prev(data_.end());
    }

    data_ptr_t pointer_to_first() const noexcept {
        return data_.cbegin();
    }

public:
    typedef T value_type;

    /// \b Complexity: O(1)
    queue_with_min() noexcept
        : data_()
        , min_(data_.cend())
    {}

    /**
    \b Complexity: O(1)
    */
    queue_with_min(queue_with_min&& q) = default;

    queue_with_min(const queue_with_min& q)
        : data_(q.data_)
        , min_( std::min_element(data_.cbegin(), data_.cend()) )
    {}

    queue_with_min(std::initializer_list<value_type> il)
        : data_(il)
       , min_( std::min_element(data_.cbegin(), data_.cend()) )
   {}

    queue_with_min& operator=(queue_with_min&& q) = default;

    queue_with_min& operator=(const queue_with_min& q) {
        if (this == &q) {
            return *this;
        }

        data_ = q.data_;
        min_ = std::min_element(data_.cbegin(), data_.cend());
        return *this;
    }

    queue_with_min& operator=(std::initializer_list<value_type> il) {
        data_ = il;
        min_ = std::min_element(data_.cbegin(), data_.cend());
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
        const bool need_reinit = (min_ == data_.cend());
        data_.emplace_back(std::forward<Args>(args)...);

        if (need_reinit || data_.back() < *min_) {
            min_ = pointer_to_last();
        }
    }

    /// \b Complexity: amort O(1)
    void pop_back() {
        const bool need_reinit = (min_ == pointer_to_last());
        data_.pop_back();
        if (need_reinit) {
            min_ = std::min_element(data_.cbegin(), data_.cend());
        }
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
        const bool need_reinit = (min_ == data_.cend());
        data_.emplace_front(std::forward<Args>(args)...);
        if (need_reinit || data_.front() < *min_) {
            min_ = pointer_to_first();
        }
    }

    /// \b Complexity: amort O(1)
    void pop_front() {
        const bool need_reinit = (min_ == pointer_to_first());
        data_.pop_front();
        if (need_reinit) {
            min_ = std::min_element(data_.cbegin(), data_.cend());
        }
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
        return *min_;
    }

    /// \b Complexity: O(N)
    bool equal(const queue_with_min<T>& q) const noexcept {
        return data_ == q.data_;
    }

    /// \b Complexity: O(N), in case of POD type up to O(1)
    void clear() noexcept {
        data_.clear();
        min_ = data_.cend();
    }
};

template <class T>
inline bool operator==(const queue_with_min<T>& lhs, const queue_with_min<T>& rhs) noexcept {
    return lhs.equal(rhs);
}

} // namespace examples

#endif // EXAMPLES_QUEUE_WITH_MIN_HPP
