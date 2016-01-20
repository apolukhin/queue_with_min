#ifndef EXAMPLES_QUEUE_WITH_MIN_V2_HPP
#define EXAMPLES_QUEUE_WITH_MIN_V2_HPP

#include <boost/config.hpp>
#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#include <list>
#include <vector>
#include <algorithm>
#include <cassert>


namespace examples_v2 {

template <class T>
class queue_with_min {
    typedef std::list<T> data_t;
    typedef typename data_t::const_iterator data_ptr_t;

    data_t                  data_raw_;
    data_ptr_t              min_raw_;

    data_t                  data_ready_;
    std::vector<data_ptr_t> min_ready_;


    data_ptr_t pointer_to_last_raw() const noexcept {
        return std::prev(data_raw_.end());
    }

    void setup_min_ready() {
        assert(min_ready_.empty());

        const auto end = data_ready_.crend();
        for (auto it = data_ready_.crbegin(); it != end; ++it) {
            if (min_ready_.empty() || *it < *min_ready_.back()) {
                min_ready_.push_back( std::prev(it.base()) );
            }

        }
    }

    void make_ready() {
        assert(data_ready_.empty());

        data_raw_.swap(data_ready_);
        min_raw_ = data_raw_.cbegin();

        setup_min_ready();
    }

public:
    typedef T value_type;

    /// \b Complexity: O(1)
    queue_with_min() noexcept
        : data_raw_()
        , min_raw_(data_raw_.cend())
    {}

    /**
    \b Complexity: O(1)
    */
    queue_with_min(queue_with_min&& q) = default;


    queue_with_min(const queue_with_min& q)
        : data_raw_(q.data_raw_)
        , min_raw_( std::min_element(data_raw_.cbegin(), data_raw_.cend()) )
        , data_ready_(q.data_ready_)
        , min_ready_()
    {
        setup_min_ready();
    }

    queue_with_min(std::initializer_list<value_type> il)
        : data_raw_(il)
        , min_raw_( std::min_element(data_raw_.cbegin(), data_raw_.cend()) )
    {}

    template <class It>
    queue_with_min(It begin, It end)
        : data_raw_(begin, end)
        , min_raw_( std::min_element(data_raw_.cbegin(), data_raw_.cend()) )
    {}

    queue_with_min& operator=(queue_with_min&& q) = default;

    queue_with_min& operator=(const queue_with_min& q) {
        if (this == &q) {
            return *this;
        }

        data_raw_ = q.data_raw_;
        min_raw_ = std::min_element(data_raw_.cbegin(), data_raw_.cend());
        data_ready_ = q.data_ready_;
        min_ready_.clear();
        setup_min_ready();

        return *this;
    }

    queue_with_min& operator=(std::initializer_list<value_type> il) {
        clear();

        data_raw_ = il;
        min_raw_ = std::min_element(data_raw_.cbegin(), data_raw_.cend());
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
        const bool need_reinit = (min_raw_ == data_raw_.cend());
        data_raw_.emplace_back(std::forward<Args>(args)...);

        if (need_reinit || data_raw_.back() < *min_raw_) {
            min_raw_ = pointer_to_last_raw();
        }
    }

    /// \b Complexity: O(1)
    const value_type& back() const {
        return !data_raw_.empty() ? data_raw_.back() : data_ready_.back();
    }


    // front

    /// \b Complexity: amort O(1) [Up to O(N) additional memory and O(N) in worst case].
    void pop_front() {
        if (data_ready_.empty()) {
            make_ready();
        }

        if (data_ready_.cbegin() == min_ready_.back()) {
            min_ready_.pop_back();
        }

        data_ready_.pop_front();
    }

    /// \b Complexity: O(1)
    const value_type& front() const {
        return data_ready_.empty() ? data_raw_.front() : data_ready_.front();
    }


    // misc
    /// \b Complexity: same as for std::list.size() [O(1) or O(N) for old libstdc++]
    std::size_t size() const noexcept {
        return data_raw_.size() + data_ready_.size();
    }

    /// \b Complexity: O(1)
    bool empty() const noexcept {
        return data_ready_.empty() && data_raw_.empty();
    }

    /// \b Complexity: O(1)
    const value_type& min() const {
        if (!data_ready_.empty() && !data_raw_.empty()) {
            return *min_raw_ < *min_ready_.back() ? *min_raw_ : *min_ready_.back();
        } else if (!data_ready_.empty()) {
            return *min_ready_.back();
        }

        return *min_raw_;
    }

   /// \b Complexity: O(N)
    bool equal(const queue_with_min<T>& q) const noexcept {
        if (size() != q.size()) {
            return false;
        }

        auto it = data_ready_.cbegin();
        const auto end1 = data_ready_.cend();

        auto q_it1 = q.data_ready_.cbegin();
        const auto q_end1 = q.data_ready_.cend();
        auto q_it2 = q.data_raw_.cbegin();

        while (it != end1) {
            auto& q_it = (q_it1 == q_end1 ? q_it2 : q_it1);
            if (*it != *q_it) {
                return false;
            }

            ++ it;
            ++ q_it;
        }


        it = data_raw_.cbegin();
        const auto end2 = data_raw_.cend();
        while (it != end2) {
            auto& q_it = (q_it1 == q_end1 ? q_it2 : q_it1);
            if (*it != *q_it) {
                return false;
            }

            ++ it;
            ++ q_it;
        }

        return true;
    }

    /// \b Complexity: O(N), in case of POD type up to O(1)
    void clear() noexcept {
        data_raw_.clear();
        min_raw_ = data_raw_.cend();

        data_ready_.clear();
        min_ready_.clear();
    }
};

template <class T>
inline bool operator==(const queue_with_min<T>& lhs, const queue_with_min<T>& rhs) noexcept {
    return lhs.equal(rhs);
}

} // namespace examples_v2

#endif // EXAMPLES_QUEUE_WITH_MIN_V2_HPP
