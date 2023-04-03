#include "gtest/gtest.h"
#include "tree.hpp"


namespace tree
{

namespace testing
{

TEST(Node_t_test, ctor_test)
{
    Node_t node{10, nullptr, nullptr, nullptr, Color_t::BLACK};
    EXPECT_EQ (node.key(), 10);
    EXPECT_EQ (node.color(), Color_t::BLACK);
    EXPECT_EQ (node.size(), 1);
}

TEST(Node_t_test, move_ctor_test)
{
    Node_t node{10, nullptr, nullptr, nullptr, Color_t::BLACK};
    Node_t mv_node{std::move(node)};
    EXPECT_EQ (mv_node.key(), 10);
    EXPECT_EQ (mv_node.color(), Color_t::BLACK);
    EXPECT_EQ (mv_node.size(), 1);
}


TEST(Node_t_test, move_asign_test)
{
    Node_t node{10, nullptr, nullptr, nullptr, Color_t::BLACK};
    Node_t mv_node = std::move(node);
    EXPECT_EQ (mv_node.key(), 10);
    EXPECT_EQ (mv_node.color(), Color_t::BLACK);
    EXPECT_EQ (mv_node.size(), 1);
}

TEST(RB_tree_t_test, insert_test)
{
    RB_tree_t tree;
    tree.insert(10);
    const Node_t* root = tree.get_root();
    EXPECT_EQ (root->key(), 10);
    EXPECT_EQ (root->color(), Color_t::BLACK);
    EXPECT_EQ (root->size(), 1);
}

TEST(RB_tree_t_test, copy_ctor_test)
{
    RB_tree_t tr;
    tr.insert(17);
    tr.insert(26);
    tr.insert(41);
    RB_tree_t tree(tr);
    const Node_t* root = tree.get_root();
    EXPECT_EQ (root->key(), 26);
    EXPECT_EQ (root->color(), Color_t::BLACK);
    EXPECT_EQ (root->size(), 3);
    EXPECT_EQ (root->left->key(), 17);
    EXPECT_EQ (root->left->color(), Color_t::RED);
    EXPECT_EQ (root->left->size(), 1);
    EXPECT_EQ (root->right->key(), 41);
    EXPECT_EQ (root->right->color(), Color_t::RED);
    EXPECT_EQ (root->right->size(), 1);
}

TEST(RB_tree_t_test, copy_asign_test)
{
    RB_tree_t tr;
    tr.insert(17);
    tr.insert(26);
    tr.insert(41);
    RB_tree_t tree = tr;
    const Node_t* root = tree.get_root();
    EXPECT_EQ (root->key(), 26);
    EXPECT_EQ (root->color(), Color_t::BLACK);
    EXPECT_EQ (root->size(), 3);
    EXPECT_EQ (root->left->key(), 17);
    EXPECT_EQ (root->left->color(), Color_t::RED);
    EXPECT_EQ (root->left->size(), 1);
    EXPECT_EQ (root->right->key(), 41);
    EXPECT_EQ (root->right->color(), Color_t::RED);
    EXPECT_EQ (root->right->size(), 1);
}

TEST(RB_tree_t_test, move_ctor_test)
{
    RB_tree_t tr;
    tr.insert(17);
    tr.insert(26);
    tr.insert(41);
    RB_tree_t tree(std::move(tr));
    const Node_t* root = tree.get_root();
    EXPECT_EQ (root->key(), 26);
    EXPECT_EQ (root->color(), Color_t::BLACK);
    EXPECT_EQ (root->size(), 3);
    EXPECT_EQ (root->left->key(), 17);
    EXPECT_EQ (root->left->color(), Color_t::RED);
    EXPECT_EQ (root->left->size(), 1);
    EXPECT_EQ (root->right->key(), 41);
    EXPECT_EQ (root->right->color(), Color_t::RED);
    EXPECT_EQ (root->right->size(), 1);

}

TEST(RB_tree_t_test, move_asign_test)
{
    RB_tree_t tr;
    tr.insert(17);
    tr.insert(26);
    tr.insert(41);
    RB_tree_t tree = std::move(tr);
    const Node_t* root = tree.get_root();
    EXPECT_EQ (root->key(), 26);
    EXPECT_EQ (root->color(), Color_t::BLACK);
    EXPECT_EQ (root->size(), 3);
    EXPECT_EQ (root->left->key(), 17);
    EXPECT_EQ (root->left->color(), Color_t::RED);
    EXPECT_EQ (root->left->size(), 1);
    EXPECT_EQ (root->right->key(), 41);
    EXPECT_EQ (root->right->color(), Color_t::RED);
    EXPECT_EQ (root->right->size(), 1);
}

TEST(RB_tree_t_test, order_statistic_test)
{
    RB_tree_t tr;
    tr.insert(17);
    tr.insert(26);
    tr.insert(41);
    EXPECT_EQ(tr.order_statistic(1), 17);
    EXPECT_EQ(tr.order_statistic(2), 26);
    EXPECT_EQ(tr.order_statistic(3), 41);
}

TEST(RB_tree_t_test, nKeys_less_than_test)
{
    RB_tree_t tr;
    tr.insert(17);
    tr.insert(26);
    tr.insert(41);
    EXPECT_EQ(tr.nKeys_less_than(10), 0);
    EXPECT_EQ(tr.nKeys_less_than(20), 1);
    EXPECT_EQ(tr.nKeys_less_than(30), 2);
    EXPECT_EQ(tr.nKeys_less_than(50), 3);
}

} //testing

} //tree

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS();
}