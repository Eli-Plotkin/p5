#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"



TEST(bst_check_empty_true) {
  BinarySearchTree<int> bst;
  ASSERT_TRUE(bst.empty());
}

TEST(bst_insert_check_sorting) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(15);
  ASSERT_TRUE(bst.check_sorting_invariant());
  ASSERT_TRUE(bst.size() == 3);
}

TEST(bst_find_after_insert) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  ASSERT_TRUE(*bst.find(10) == 10);
  ASSERT_TRUE(bst.find(20) == bst.end());
}

TEST(bst_size_after_multiple_inserts) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(15);
  ASSERT_TRUE(bst.size() == 3);
}

TEST(bst_height_after_inserts) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(15);
  ASSERT_TRUE(bst.height() == 2);
  bst.insert(20);
  ASSERT_TRUE(bst.height() == 3);
}

TEST(bst_traverse_inorder) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(15);
  std::ostringstream os;
  bst.traverse_inorder(os);
  ASSERT_TRUE(os.str() == "5 10 15 ");
}

TEST(bst_traverse_preorder) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(15);
  std::ostringstream os;
  bst.traverse_preorder(os);
  ASSERT_TRUE(os.str() == "10 5 15 ");
}

TEST(bst_min_max_element) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(15);
  ASSERT_TRUE(*bst.min_element() == 5);
  ASSERT_TRUE(*bst.max_element() == 15);
}

TEST(bst_min_greater_than) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(5);
  bst.insert(15);
  ASSERT_TRUE(*bst.min_greater_than(10) == 15);
}

TEST(bst_height_single_element) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  ASSERT_TRUE(bst.height() == 1);
}

TEST(bst_height_linear_tree) {
  BinarySearchTree<int> bst;
  for (int i = 0; i < 10; ++i) {
    bst.insert(i);
  }
  ASSERT_TRUE(bst.height() == 10); 
}

TEST(bst_min_greater_than_nonexistent) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  bst.insert(11);
  ASSERT_TRUE(bst.min_greater_than(15) == bst.end());
}

TEST(bst_iterator_increment_past_end) {
  BinarySearchTree<int> bst;
  bst.insert(10);
  auto it = bst.begin();
  ++it;
  ASSERT_TRUE(it == bst.end());
}

TEST(bst_to_string_empty_tree) {
  BinarySearchTree<int> bst;
  ASSERT_TRUE(bst.to_string() == "( )");
}

TEST(bst_copy_constructor_deep_copy) {
  BinarySearchTree<int> bst1;
  bst1.insert(10);
  bst1.insert(5);
  bst1.insert(15);

  BinarySearchTree<int> bst2(bst1);
  bst2.insert(20);

  ASSERT_TRUE(bst1.size() == 3); 
  ASSERT_TRUE(bst2.size() == 4);
}



//testing behavior on empty tree and after an insertion
TEST(bst_empty_then_insert) {
  BinarySearchTree<int> bst;
  ASSERT_TRUE(bst.empty());
  bst.insert(5);
  ASSERT_FALSE(bst.empty());
}

//testing the dynamic change in height with insertions
TEST(bst_dynamic_height_check_insertions) {
  BinarySearchTree<int> bst;
  bst.insert(3);
  bst.insert(4);
  bst.insert(5);
  ASSERT_TRUE(bst.height() == 3);
  bst.insert(6);
  ASSERT_TRUE(bst.height() == 4);
}

//testing size under a large number of insertions
TEST(bst_lots_of_insertions) {
  BinarySearchTree<int> bst;
  for (int i = 1; i <= 144; ++i) {
    bst.insert(i);
    ASSERT_TRUE(bst.size() == i);
  }
}

//testing correctness of iterators
TEST(bst_iterator_correctness) {
  BinarySearchTree<int> bst;
  bst.insert(3);
  bst.insert(2);
  bst.insert(4);
  auto it = bst.begin();
  ASSERT_TRUE(*it == 2);
  ++it;
  ASSERT_TRUE(*it == 3);
  ++it;
  ASSERT_TRUE(*it == 4);
}

//testing max element in various scenarios
TEST(bst_max_element_more) {
  BinarySearchTree<int> bst;
  bst.insert(-15);
  bst.insert(-42);
  bst.insert(0);
  ASSERT_TRUE(*bst.max_element() == 0);
  bst.insert(14);
  ASSERT_TRUE(*bst.max_element() == 14);
}

//testing min element in various conditions
TEST(bst_min_element_more) {
  BinarySearchTree<int> bst;
  bst.insert(312);
  bst.insert(340);
  bst.insert(2);
  ASSERT_TRUE(*bst.min_element() == 2);
  bst.insert(1);
  ASSERT_TRUE(*bst.min_element() == 1);
  bst.insert(-11);
  ASSERT_TRUE(*bst.min_element() == -11);
}

//testing inorder and preorder traversal correctness
TEST(bst_traversal_correct) {
  BinarySearchTree<int> bst;
  bst.insert(3);
  bst.insert(1);
  bst.insert(2);
  std::ostringstream inorder, preorder;
  bst.traverse_inorder(inorder);
  bst.traverse_preorder(preorder);
  ASSERT_TRUE(inorder.str() == "1 2 3 ");
  ASSERT_TRUE(preorder.str() == "3 1 2 ");
}

TEST(bst_iterator_traversal_complete) {
  BinarySearchTree<int> bst;
  for (int i = 1; i <= 5; ++i) {
    bst.insert(i);
  }

  int count = 0;
  for (auto it = bst.begin(); it != bst.end(); ++it) {
    ++count;
  }
  ASSERT_TRUE(count == 5);
}

//testing sorting invariant violation through iterator dereferencing
TEST(bst_iterator_dereference_modification) {
  BinarySearchTree<int> bst;
  bst.insert(1);
  bst.insert(2);
  bst.insert(3);
  auto it = bst.find(2);
  if (it != bst.end()) {
    *it = 4;  }
  ASSERT_FALSE(bst.check_sorting_invariant()); 
}

//testing tree height dynamics with specific insertions
TEST(bst_height_extreme_insertion) {
  BinarySearchTree<int> bst;
  bst.insert(1000);
  for (int i = 999; i >= 1; --i) {
    bst.insert(i);
  }
  ASSERT_TRUE(bst.height() == 1000); }

//testing of iterator increment and tree structure
TEST(bst_iterator_increment_complex_structure) {
  BinarySearchTree<int> bst;	
// gotta use 7 deuce 
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(3);
  bst.insert(5);
  bst.insert(7);
  auto it = bst.begin();
  for (int expected = 2; expected <= 7; ++expected) {
    ASSERT_TRUE(*it == expected);
    ++it;
  }
  ASSERT_TRUE(it == bst.end());
}

//testing max and min elements in a tree with negative values
TEST(bst_min_max_with_negatives) {
  BinarySearchTree<int> bst;
  bst.insert(-12);
  bst.insert(-25);
  bst.insert(0);
  ASSERT_TRUE(*bst.max_element() == 0);
  ASSERT_TRUE(*bst.min_element() == -25);
}

//testing tree operations on an empty tree
TEST(bst_empty_tree_operations) {
  BinarySearchTree<int> bst;
  ASSERT_TRUE(bst.max_element() == bst.end());
  ASSERT_TRUE(bst.min_element() == bst.end());
  ASSERT_TRUE(bst.min_greater_than(0) == bst.end());
}

TEST(bst_copy_constructor_independence) {
  BinarySearchTree<int> original;
  original.insert(10);
  BinarySearchTree<int> copy = original;

  copy.insert(20); // Modifying copy should not affect original
  ASSERT_FALSE(original.find(20) != original.end());
}

TEST(bst_assignment_operator_deep_copy) {
  BinarySearchTree<int> original;
  original.insert(10);

  BinarySearchTree<int> copy;
  copy = original; // Using assignment operator for copying
  original.insert(20); // Modifying original should not affect copy

  ASSERT_TRUE(copy.find(20) == copy.end());
}



TEST_MAIN()
