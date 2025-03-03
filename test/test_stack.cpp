#include <gtest/gtest.h>
#include "stack.h"

TEST(StackTest, can_create_with_default_constructor)
{
    Stack<int> stack;
    EXPECT_EQ(stack.Size(), 0);
    EXPECT_EQ(stack.Capacity(), 0);
}

TEST(StackTest, can_create_with_positive_length)
{
    Stack<int> stack(10);
    EXPECT_EQ(stack.Size(), 0);
    EXPECT_EQ(stack.Capacity(), 10);
}

TEST(StackTest, cant_create_too_big)
{
    EXPECT_ANY_THROW(Stack<int> stack(LONG_MAX));
}

TEST(StackTest, can_create_a_copy)
{
    Stack<int> stack1;
    stack1.Push(1);
    stack1.Push(2);

    Stack<int> stack2(stack1);
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2.Top(), 2);

    stack2.Pop();
    EXPECT_EQ(stack2.Top(), 1);
}

TEST(StackTest, can_create_with_move_constructor)
{
    Stack<int> stack1;
    stack1.Push(1);
    stack1.Push(2);

    Stack<int> stack2(std::move(stack1));
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2.Top(), 2);

    EXPECT_EQ(stack1.Size(), 0);
    EXPECT_EQ(stack1.Capacity(), 0);
}

TEST(StackTest, can_assign)
{
    Stack<int> stack1;
    stack1.Push(1);
    stack1.Push(2);

    Stack<int> stack2;
    stack2 = stack1;
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2.Top(), 2);

    stack2.Pop();
    EXPECT_EQ(stack2.Top(), 1);
}

TEST(StackTest, capacity_doesnt_change_on_assign_shorter)
{
    Stack<int> stack1;
    stack1.Push(1);

    Stack<int> stack2;
    stack2.Push(1);
    stack2.Push(2);
    stack2.Push(3);
    size_t cap = stack2.Capacity();
    stack2 = stack1;
    EXPECT_EQ(stack2.Capacity(), cap);
}

TEST(StackTest, can_perform_move_assignment)
{
    Stack<int> stack1;
    stack1.Push(1);
    stack1.Push(2);

    Stack<int> stack2;
    stack2 = std::move(stack1);
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2.Top(), 2);

    EXPECT_EQ(stack1.Size(), 0);
    EXPECT_EQ(stack1.Capacity(), 0);
}

TEST(StackTest, can_push)
{
    Stack<int> stack;
    stack.Push(1);
    EXPECT_EQ(stack.Size(), 1);
    EXPECT_EQ(stack.Top(), 1);
    stack.Push(2);
    EXPECT_EQ(stack.Size(), 2);
    EXPECT_EQ(stack.Top(), 2);
}

TEST(StackTest, can_pop)
{
    Stack<int> stack;
    stack.Push(1);
    stack.Push(2);
    stack.Pop();
    EXPECT_EQ(stack.Size(), 1);
    EXPECT_EQ(stack.Top(), 1);
    stack.Pop();
    EXPECT_TRUE(stack.Empty());
}

TEST(StackTest, cant_pop_from_empty)
{
    Stack<int> stack;
    EXPECT_ANY_THROW(stack.Pop());
}

TEST(StackTest, cant_get_top_from_empty)
{
    Stack<int> stack;
    EXPECT_ANY_THROW(stack.Top());
}

TEST(StackTest, can_reserve_memory)
{
    Stack<int> stack;
    stack.Reserve(10);
    EXPECT_EQ(stack.Capacity(), 10);
}

TEST(StackTest, doesnt_reserve_less_than_current)
{
    Stack<int> stack;
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);
    size_t cap = stack.Capacity();
    stack.Reserve(2);
    EXPECT_EQ(stack.Capacity(), cap);
}

TEST(StackTest, cant_reserve_too_many_elements)
{
    Stack<int> stack;
    EXPECT_ANY_THROW(stack.Reserve(LONG_MAX));
}

TEST(StackTest, test_empty)
{
    Stack<int> stack;
    EXPECT_TRUE(stack.Empty());
    stack.Push(1);
    EXPECT_FALSE(stack.Empty());
}