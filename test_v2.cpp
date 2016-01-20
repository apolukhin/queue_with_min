#include "queue_with_min_v2.hpp"

#include <memory>
#include <deque>

#include "gtest/gtest.h"

using namespace examples_v2;


TEST(qwm2, basic) {
    queue_with_min<int> q;
    ASSERT_TRUE(q.size() == 0);
    ASSERT_TRUE(q.empty());

    q.push_back(777);
    ASSERT_TRUE(q.size() == 1);
    ASSERT_TRUE(q.min() == 777);
    ASSERT_TRUE(q.back() == 777);
    ASSERT_TRUE(q.front() == 777);

    q.pop_front();
    ASSERT_TRUE(q.size() == 0);
    ASSERT_TRUE(q.empty());
}

TEST(qwm2, min_increasing) {
    queue_with_min<unsigned int> q;
    const unsigned int values_count = 10;

    for (unsigned int i = 0; i < values_count; ++i) {
        q.emplace_back(i);
        ASSERT_TRUE(q.size() == i + 1);
        ASSERT_TRUE(q.min() == 0);
        ASSERT_TRUE(q.back() == i);
        ASSERT_TRUE(q.front() == 0);
    }

    for (unsigned int i = 0; i < values_count; ++i) {
        ASSERT_TRUE(q.size() == values_count - i);
        ASSERT_TRUE(q.min() == i);
        ASSERT_TRUE(q.back() == values_count - 1);
        ASSERT_TRUE(q.front() == i);
        q.pop_front();
        ASSERT_TRUE(q.size() == values_count - i - 1);
    }
    ASSERT_TRUE(q.empty());



    for (unsigned int i = 0; i < values_count; ++i) {
        q.emplace_back(i);
        q.emplace_back(i);
        ASSERT_TRUE(q.size() == (i + 1) * 2);
        ASSERT_TRUE(q.min() == 0);
        ASSERT_TRUE(q.back() == i);
        ASSERT_TRUE(q.front() == 0);
    }

    for (unsigned int i = 0; i < values_count - 1; ++i) {
        ASSERT_TRUE(q.size() == (values_count - i) * 2);
        ASSERT_TRUE(q.min() == i);
        ASSERT_TRUE(q.back() == values_count - 1);
        ASSERT_TRUE(q.front() == i);
        q.pop_front();

        ASSERT_TRUE(q.size() == (values_count - i) * 2 - 1);
        ASSERT_TRUE(q.min() == i);
        ASSERT_TRUE(q.back() == values_count - 1);
        ASSERT_TRUE(q.front() == i);
        q.pop_front();
    }

}


TEST(qwm2, min_decreasing) {
    queue_with_min<unsigned int> q;
    const unsigned int values_count = 10;

    for (unsigned int i = values_count; !!i; --i) {
        q.emplace_back(i);
        ASSERT_TRUE(q.size() == values_count - i + 1);
        ASSERT_TRUE(q.min() == i);
        ASSERT_TRUE(q.back() == i);
        ASSERT_TRUE(q.front() == values_count);
    }

    for (unsigned int i = 0; i < values_count; ++i) {
        ASSERT_TRUE(q.size() == values_count - i);
        ASSERT_TRUE(q.min() == 1);
        ASSERT_TRUE(q.back() == 1);
        ASSERT_TRUE(q.front() == values_count - i);
        q.pop_front();
        ASSERT_TRUE(q.size() == values_count - i - 1);
    }
    ASSERT_TRUE(q.empty());
}


TEST(qwm2, min_random) {
    const std::size_t values_count = 500;

    std::deque<unsigned int> v(values_count);
    std::generate(v.begin(), v.end(), std::rand);

    queue_with_min<unsigned int> q(v.begin(), v.end());

    for (std::size_t i = 0; i < values_count; ++i) {
        ASSERT_TRUE(q.size() == v.size());
        ASSERT_TRUE(q.back() == v.back());
        ASSERT_TRUE(q.front() == v.front());
        ASSERT_TRUE(q.min() == *std::min_element(v.cbegin(), v.cend()) );

        q.pop_front();
        v.pop_front();
    }

    ASSERT_TRUE(q.empty());
}


TEST(qwm2, move_only) {
    queue_with_min<std::unique_ptr<int> > q;
    q.push_back(nullptr);
    q.push_back(nullptr);
    q.push_back(std::unique_ptr<int>(
        new int (10)
    ));
}

TEST(qwm2, middle_states1) {
    queue_with_min<int> q({2, 1, 3, 4, 5});
    ASSERT_TRUE(q.min() == 1);
    q.pop_front();
    ASSERT_TRUE(q.min() == 1);
    q.push_back(0);
    ASSERT_TRUE(q.min() == 0);
    q.pop_front();
    ASSERT_TRUE(q.min() == 0);

}

TEST(qwm2, middle_states2) {
    queue_with_min<int> q({2, 1, 3, 4, 5});
    ASSERT_TRUE(q.min() == 1);
    q.pop_front();
    ASSERT_TRUE(q.min() == 1);
    q.push_back(110);
    ASSERT_TRUE(q.min() == 1);
    q.pop_front();
    ASSERT_TRUE(q.min() == 3);
}

TEST(qwm2, middle_states3) {
    queue_with_min<int> q({2, 1, 3, 4, 5});
    ASSERT_TRUE(q.min() == 1);
    q.pop_front();
    ASSERT_TRUE(q.min() == 1);
    q.push_back(1);
    ASSERT_TRUE(q.min() == 1);
    q.pop_front();
    ASSERT_TRUE(q.min() == 1);
}


TEST(qwm2, move_and_copy1) {
    queue_with_min<int> q({2, 1, 3, 4, 5});

    queue_with_min<int> q_moved( std::move(q) );
    ASSERT_TRUE(q_moved.size() == 5);
    ASSERT_TRUE(q_moved.min() == 1);
    ASSERT_TRUE(q_moved.back() == 5);
    ASSERT_TRUE(q_moved.front() == 2);
    ASSERT_TRUE(q.empty());

    q = q_moved;
    ASSERT_TRUE(q == q_moved);

    ASSERT_TRUE(q_moved.size() == 5);
    ASSERT_TRUE(q_moved.min() == 1);
    ASSERT_TRUE(q_moved.back() == 5);
    ASSERT_TRUE(q_moved.front() == 2);

    ASSERT_TRUE(q.size() == 5);
    ASSERT_TRUE(q.min() == 1);
    ASSERT_TRUE(q.back() == 5);
    ASSERT_TRUE(q.front() == 2);

    q_moved.pop_front();
    q_moved.pop_front();
    ASSERT_TRUE(q_moved.size() == 3);
    ASSERT_TRUE(q_moved.min() == 3);
    ASSERT_TRUE(q_moved.back() == 5);
    ASSERT_TRUE(q_moved.front() == 3);
    q_moved.push_back(998);
    q_moved.push_back(999);
    q_moved.push_back(1000);
    q_moved.push_back(1001);

    ASSERT_TRUE(q_moved.size() == 7);
    ASSERT_TRUE(q_moved.min() == 3);
    ASSERT_TRUE(q_moved.back() == 1001);
    ASSERT_TRUE(q_moved.front() == 3);

    q_moved.pop_front();
    q_moved.push_back(0);
    ASSERT_TRUE(q_moved.size() == 7);
    ASSERT_TRUE(q_moved.min() == 0);
    ASSERT_TRUE(q_moved.back() == 0);
    ASSERT_TRUE(q_moved.front() == 4);

    q = {4, 5, 998, 999, 1000, 1001, 0};
    ASSERT_TRUE(q == q_moved);
    q.pop_front();
    q_moved = {5, 998, 999, 1000, 1001, 0};
    ASSERT_TRUE(q == q_moved);

}

TEST(qwm2, move_and_copy2) {
    queue_with_min<int> q1({2, 1, 3, 4, 5});

    queue_with_min<int> q_moved;
    q_moved = std::move(q1);
    ASSERT_TRUE(q_moved.size() == 5);
    ASSERT_TRUE(q_moved.min() == 1);
    ASSERT_TRUE(q_moved.back() == 5);
    ASSERT_TRUE(q_moved.front() == 2);
    ASSERT_TRUE(q1.empty());

    queue_with_min<int> q(q_moved);
    ASSERT_TRUE(q == q_moved);

    ASSERT_TRUE(q_moved.size() == 5);
    ASSERT_TRUE(q_moved.min() == 1);
    ASSERT_TRUE(q_moved.back() == 5);
    ASSERT_TRUE(q_moved.front() == 2);

    ASSERT_TRUE(q.size() == 5);
    ASSERT_TRUE(q.min() == 1);
    ASSERT_TRUE(q.back() == 5);
    ASSERT_TRUE(q.front() == 2);

    q_moved.pop_front();
    q_moved.pop_front();
    ASSERT_TRUE(q_moved.size() == 3);
    ASSERT_TRUE(q_moved.min() == 3);
    ASSERT_TRUE(q_moved.back() == 5);
    ASSERT_TRUE(q_moved.front() == 3);

    q.clear();
    ASSERT_TRUE(q.size() == 0);
    q_moved.clear();
    ASSERT_TRUE(q == q_moved);

}

TEST(qwm2, noexcepts) {
    ASSERT_TRUE( std::is_nothrow_constructible<queue_with_min<int> >() );
    //ASSERT_TRUE( std::is_nothrow_move_assignable<queue_with_min<int> >() );
    //ASSERT_TRUE( std::is_nothrow_move_constructible<queue_with_min<int> >() );
}

