#include "queue_with_min.hpp"

#include <memory>

#include "gtest/gtest.h"

using namespace examples;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



TEST(qwm, basic) {
    queue_with_min<int> q;
    ASSERT_TRUE(q.size() == 0);
    ASSERT_TRUE(q.empty());

    q.push_back(777);
    ASSERT_TRUE(q.size() == 1);
    ASSERT_TRUE(q.min() == 777);
    ASSERT_TRUE(q.back() == 777);
    ASSERT_TRUE(q.front() == 777);

    q.pop_back();
    ASSERT_TRUE(q.size() == 0);
    ASSERT_TRUE(q.empty());
}

TEST(qwm, min_front) {
    queue_with_min<int> q;

    q.emplace_front(10);
    ASSERT_TRUE(q.size() == 1);
    ASSERT_TRUE(q.min() == 10);
    ASSERT_TRUE(q.back() == 10);
    ASSERT_TRUE(q.front() == 10);

    q.emplace_front(9);
    ASSERT_TRUE(q.size() == 2);
    ASSERT_TRUE(q.min() == 9);
    ASSERT_TRUE(q.back() == 10);
    ASSERT_TRUE(q.front() == 9);

    q.emplace_front(8);
    ASSERT_TRUE(q.size() == 3);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 10);
    ASSERT_TRUE(q.front() == 8);

    q.emplace_front(18);
    ASSERT_TRUE(q.size() == 4);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 10);
    ASSERT_TRUE(q.front() == 18);

    q.emplace_front(7);
    ASSERT_TRUE(q.size() == 5);
    ASSERT_TRUE(q.min() == 7);
    ASSERT_TRUE(q.back() == 10);
    ASSERT_TRUE(q.front() == 7);

    q.emplace_front(7);
    ASSERT_TRUE(q.size() == 6);
    ASSERT_TRUE(q.min() == 7);
    ASSERT_TRUE(q.back() == 10);
    ASSERT_TRUE(q.front() == 7);

    // popping data
    q.pop_front();
    ASSERT_TRUE(q.size() == 5);
    ASSERT_TRUE(q.min() == 7);
    ASSERT_TRUE(q.back() == 10);
    ASSERT_TRUE(q.front() == 7);

    q.pop_front();
    ASSERT_TRUE(q.size() == 4);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 10);
    ASSERT_TRUE(q.front() == 18);

    q.pop_back();
    ASSERT_TRUE(q.size() == 3);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 9);
    ASSERT_TRUE(q.front() == 18);

    q.pop_front();
    ASSERT_TRUE(q.size() == 2);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 9);
    ASSERT_TRUE(q.front() == 8);

    q.pop_front();
    ASSERT_TRUE(q.size() == 1);
    ASSERT_TRUE(q.min() == 9);
    ASSERT_TRUE(q.back() == 9);
    ASSERT_TRUE(q.front() == 9);

    q.pop_front();
    ASSERT_TRUE(q.size() == 0);
}


TEST(qwm, min_back) {
    queue_with_min<int> q;

    q.emplace_back(10);
    ASSERT_TRUE(q.size() == 1);
    ASSERT_TRUE(q.min() == 10);
    ASSERT_TRUE(q.back() == 10);
    ASSERT_TRUE(q.front() == 10);

    q.emplace_back(9);
    ASSERT_TRUE(q.size() == 2);
    ASSERT_TRUE(q.min() == 9);
    ASSERT_TRUE(q.back() == 9);
    ASSERT_TRUE(q.front() == 10);

    q.emplace_back(8);
    ASSERT_TRUE(q.size() == 3);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 8);
    ASSERT_TRUE(q.front() == 10);

    q.emplace_back(18);
    ASSERT_TRUE(q.size() == 4);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 18);
    ASSERT_TRUE(q.front() == 10);

    q.emplace_back(7);
    ASSERT_TRUE(q.size() == 5);
    ASSERT_TRUE(q.min() == 7);
    ASSERT_TRUE(q.back() == 7);
    ASSERT_TRUE(q.front() == 10);

    q.pop_back();
    ASSERT_TRUE(q.size() == 4);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 18);
    ASSERT_TRUE(q.front() == 10);

    q.pop_front();
    ASSERT_TRUE(q.size() == 3);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 18);
    ASSERT_TRUE(q.front() == 9);

    q.pop_back();
    ASSERT_TRUE(q.size() == 2);
    ASSERT_TRUE(q.min() == 8);
    ASSERT_TRUE(q.back() == 8);
    ASSERT_TRUE(q.front() == 9);

    q.pop_back();
    ASSERT_TRUE(q.size() == 1);
    ASSERT_TRUE(q.min() == 9);
    ASSERT_TRUE(q.back() == 9);
    ASSERT_TRUE(q.front() == 9);

    q.pop_back();
    ASSERT_TRUE(q.size() == 0);
}

TEST(qwm, move_only) {
    queue_with_min<std::unique_ptr<int> > q;
    q.push_back(nullptr);
    q.push_back(nullptr);
    q.push_back(std::unique_ptr<int>(
        new int (10)
    ));
}

TEST(qwm, move_and_copy1) {
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
    q_moved.push_front(998);
    q_moved.push_front(999);

    ASSERT_TRUE(q.size() == 5);
    ASSERT_TRUE(q.min() == 1);
    ASSERT_TRUE(q.back() == 5);
    ASSERT_TRUE(q.front() == 2);

    q = {999, 998, 3, 4, 5};
    ASSERT_TRUE(q == q_moved);
}

TEST(qwm, move_and_copy2) {
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
    q_moved.push_front(998);
    q_moved.push_front(999);

    ASSERT_TRUE(q.size() == 5);
    ASSERT_TRUE(q.min() == 1);
    ASSERT_TRUE(q.back() == 5);
    ASSERT_TRUE(q.front() == 2);

    q = {999, 998, 3, 4, 5};
    ASSERT_TRUE(q == q_moved);

    q.clear();
    ASSERT_TRUE(q.size() == 0);

}

TEST(qwm, noexcepts) {
    ASSERT_TRUE( std::is_nothrow_constructible<queue_with_min<int> >() );
    //ASSERT_TRUE( std::is_nothrow_move_assignable<queue_with_min<int> >() );
    //ASSERT_TRUE( std::is_nothrow_move_constructible<queue_with_min<int> >() );
}
