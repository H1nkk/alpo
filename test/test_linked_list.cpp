#include "linked_list.h"
#include <exception>
#include <gtest/gtest.h>

TEST(LinkedListTest, can_create_with_default_constructor)
{
    linked_list<int> list;
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(LinkedListTest, can_create_with_initializer_list)
{
    linked_list<int> list = { 1, 2, 3 };
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(LinkedListTest, can_create_with_copy_constructor)
{
    linked_list<int> original = { 1, 2, 3 };
    linked_list<int> copy(original);

    EXPECT_EQ(copy.size(), original.size());
    auto it1 = original.begin();
    auto it2 = copy.begin();
    while (it1 != original.end() && it2 != copy.end())
    {
        EXPECT_EQ(*it1, *it2);
        ++it1;
        ++it2;
    }
}

TEST(LinkedListTest, can_create_with_move_constructor)
{
    linked_list<int> original = { 1, 2, 3 };
    linked_list<int> moved(std::move(original));

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 3);

    EXPECT_TRUE(original.is_empty());
}

TEST(LinkedListTest, can_assign_lists)
{
    linked_list<int> original = { 1, 2, 3 };
    linked_list<int> copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    auto it1 = original.begin();
    auto it2 = copy.begin();
    while (it1 != original.end() && it2 != copy.end())
    {
        EXPECT_EQ(*it1, *it2);
        ++it1;
        ++it2;
    }
}

TEST(LinkedListTest, can_perform_move_assignment)
{
    linked_list<int> original = { 1, 2, 3 };
    linked_list<int> moved;
    moved = std::move(original);

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 3);

    EXPECT_TRUE(original.is_empty());
}

TEST(LinkedListTest, self_assignment_does_nothing)
{
    linked_list<int> list = { 1, 2, 3 };
    list = list;

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(LinkedListTest, self_move_assignment_does_nothing)
{
    linked_list<int> list = { 1, 2, 3 };
    list = std::move(list);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(LinkedListTest, can_push_front)
{
    linked_list<int> list;
    list.push_front(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 10);

    list.push_front(20);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 20);
}

TEST(LinkedListTest, can_push_back)
{
    linked_list<int> list;
    list.push_back(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back(), 10);

    list.push_back(20);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 20);
}

TEST(LinkedListTest, can_pop_front)
{
    linked_list<int> list = { 1, 2, 3 };
    list.pop_front();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);

    list.pop_front();
    list.pop_front();
    EXPECT_TRUE(list.is_empty());
    EXPECT_THROW(list.pop_front(), std::out_of_range);
}

TEST(LinkedListTest, can_pop_back)
{
    linked_list<int> list = { 1, 2, 3 };
    list.pop_back();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);

    list.pop_back();
    list.pop_back();
    EXPECT_TRUE(list.is_empty());
    EXPECT_THROW(list.pop_back(), std::out_of_range);
}

TEST(LinkedListTest, can_get_front)
{
    linked_list<int> list = { 10, 20 };
    EXPECT_EQ(list.front(), 10);

    list.pop_front();
    EXPECT_EQ(list.front(), 20);

    list.pop_front();
    EXPECT_THROW(list.front(), std::out_of_range);
}

TEST(LinkedListTest, can_get_back)
{
    linked_list<int> list = { 10, 20 };
    EXPECT_EQ(list.back(), 20);

    list.pop_back();
    EXPECT_EQ(list.back(), 10);

    list.pop_back();
    EXPECT_THROW(list.back(), std::out_of_range);
}

TEST(LinkedListTest, can_insert_elements)
{
    linked_list<int> list = { 1, 3 };
    auto it = list.begin();
    list.insert_after(it, 2);
    EXPECT_EQ(list.size(), 3);

    it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
}

TEST(LinkedListTest, can_erase_elements)
{
    linked_list<int> list = { 1, 2, 3 };
    auto it = list.begin();
    list.erase_after(it);
    EXPECT_EQ(list.size(), 2);

    it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 3);
}

TEST(LinkedListTest, can_use_iterators)
{
    linked_list<int> list = { 1, 2, 3 };
    auto it = list.begin();
    EXPECT_EQ(*it, 1);

    ++it;
    EXPECT_EQ(*it, 2);

    ++it;
    EXPECT_EQ(*it, 3);

    ++it;
    EXPECT_THROW(*it, std::out_of_range);

    it = list.before_end();
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_THROW(*it, std::out_of_range);
}

TEST(LinkedListTest, can_erase_first)
{
    linked_list<int> list = { 1, 2, 3 };
    auto it = list.before_begin();
    EXPECT_EQ(list.front(), 1);
    list.erase_after(it);
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.size(), 2);
}

TEST(LinkedListTest, check_iterator_errors)
{
    linked_list<int> list;
    auto it = list.begin();
    EXPECT_THROW(++it, std::out_of_range);
    EXPECT_THROW(*it, std::out_of_range);
}

TEST(LinkedListTest, can_clear_list)
{
    linked_list<int> list = { 1, 2, 3 };
    list.clear();
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.front(), std::out_of_range);
    EXPECT_THROW(list.back(), std::out_of_range);
}