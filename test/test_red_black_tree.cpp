#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include "red_black_tree.h"

using namespace std;

TEST(RedBlackTreeTest, default_constructor)
{
    RedBlackTree<string, int> tree;

    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.find("a"), std::nullopt);
}

TEST(RedBlackTreeTest, can_insert_data)
{
    RedBlackTree<string, int> tree;

    tree.insert("a", 1);
    tree.insert("ab", 12);
    tree.insert("A", -1);
    tree.insert("c", 3);
    EXPECT_EQ(tree.find("a"), 1);
    EXPECT_EQ(tree.find("ab"), 12);
    EXPECT_EQ(tree.find("A"), -1);
    EXPECT_EQ(tree.find("c"), 3);
    EXPECT_EQ(tree.find("d"), std::nullopt);
}

TEST(RedBlackTreeTest, can_rewrite_data)
{
    RedBlackTree<string, int> tree;

    tree.insert("a", 1);
    tree.insert("ab", 12);
    tree.insert("A", -1);
    tree.insert("c", 3);
    EXPECT_EQ(tree.find("a"), 1);
    EXPECT_EQ(tree.find("ab"), 12);
    EXPECT_EQ(tree.find("A"), -1);
    EXPECT_EQ(tree.find("c"), 3);
    tree.insert("ab", -20);
    EXPECT_EQ(tree.find("a"), 1);
    EXPECT_EQ(tree.find("ab"), -20);
    EXPECT_EQ(tree.find("A"), -1);
    EXPECT_EQ(tree.find("c"), 3);
}

TEST(RedBlackTree, insertion_search_stress_test)
{
    RedBlackTree<int, int> tree;

    const int n = 10000000;
    vector<int> data(n);
    iota(data.begin(), data.end(), 0);
    for (auto v : data) tree.insert(v, v + 1);
    reverse(data.begin(), data.end());
    for (auto v : data)
    {
        EXPECT_EQ(tree.find(v), v + 1);
    }
}