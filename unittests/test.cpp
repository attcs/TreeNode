#include "pch.h"

#include <memory>
#include "../treenode.h"

#include <vector>

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
  #include <execution>
#endif

struct DbEntity
{
  int val = 0;
};

bool operator==(DbEntity const& l, DbEntity const& r)
{
  return l.val == r.val;
}

namespace TreeNodeTests
{
  using namespace std;

  TEST(TreeNode, add_child_parent_eq)
  {
    TreeNode<DbEntity> root;
    auto const node = root.add_child(DbEntity{ 1 });

    EXPECT_EQ(&root, node->parent());
  }

  TEST(TreeNode, get_1)
  {
    TreeNode<DbEntity> root;
    auto const node = root.add_child(DbEntity{ 1 });
    EXPECT_EQ(1, node->get().val);
  }

  TEST(TreeNode, add_child_1child_PrevIsNull)
  {
    TreeNode<DbEntity> root;
    auto const node = root.add_child(DbEntity{ 1 });

    EXPECT_EQ(nullptr, node->prev());
  }

  TEST(TreeNode, add_child_1child_NextIsNull)
  {
    TreeNode<DbEntity> root;
    auto const node = root.add_child(DbEntity{ 1 });

    EXPECT_EQ(nullptr, node->next());
  }

  TEST(TreeNode, add_child_1child_next_bfsIsNullAndNode)
  {
    TreeNode<DbEntity> root;
    auto const node = root.add_child(DbEntity{ 1 });

    EXPECT_EQ(nullptr, node->next_bfs());
    EXPECT_EQ(node, root.next_bfs());
  }


  TEST(TreeNode, add_child_2child_PrevNextParent)
  {
    TreeNode<DbEntity> root;
    auto const node1 = root.add_child(DbEntity{ 1 });
    auto const node2 = root.add_child(DbEntity{ 2 });

    EXPECT_EQ(1, node1->get().val);
    EXPECT_EQ(2, node2->get().val);

    EXPECT_EQ(&root, node1->parent());
    EXPECT_EQ(&root, node2->parent());

    EXPECT_EQ(nullptr, node1->prev());
    EXPECT_EQ(node2, node1->next());
    EXPECT_EQ(nullptr, node2->next());
  }

  TEST(TreeNode, add_child_2child_parentIsRoot)
  {
    TreeNode<DbEntity> root;
    auto const node1 = root.add_child(DbEntity{ 1 });
    auto const node2 = root.add_child(DbEntity{ 2 });

    EXPECT_EQ(&root, node1->parent());
    EXPECT_EQ(&root, node2->parent());
  }

  TEST(TreeNode, add_child_2child_valsAreOk)
  {
    TreeNode<DbEntity> root;
    auto const node1 = root.add_child(DbEntity{ 1 });
    auto const node2 = root.add_child(DbEntity{ 2 });

    EXPECT_EQ(1, node1->get().val);
    EXPECT_EQ(2, node2->get().val);
  }


  TEST(TreeNode, add_child_2child_prevAndnextAreOk)
  {
    TreeNode<DbEntity> root;
    auto const node1 = root.add_child(DbEntity{ 1 });
    auto const node2 = root.add_child(DbEntity{ 2 });

    EXPECT_EQ(nullptr, node1->prev());
    EXPECT_EQ(node2, node1->next());
    EXPECT_EQ(nullptr, node2->next());
  }

  TEST(TreeNode, add_child_2child_next_bfsIsOk)
  {
    TreeNode<DbEntity> root;
    auto const node1 = root.add_child(DbEntity{ 1 });
    auto const node2 = root.add_child(DbEntity{ 2 });

    EXPECT_EQ(node1, root.next_bfs());
    EXPECT_EQ(node2, node1->next_bfs());
    EXPECT_EQ(nullptr, node2->next_bfs());
  }


  TEST(TreeNode, add_child_2child1grandchild1_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto child1 = root.add_child(DbEntity{ 1 });
    auto child2 = root.add_child(DbEntity{ 2 });

    auto grandchild11 = child1->add_child(DbEntity{ 11 });

    EXPECT_EQ(child2, child1->next_bfs());
    EXPECT_EQ(grandchild11, child2->next_bfs());
    EXPECT_EQ(nullptr, grandchild11->next_bfs());
  }


  TEST(TreeNode, add_child_2child1grandchild2_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto child1 = root.add_child(DbEntity{ 1 });
    auto child2 = root.add_child(DbEntity{ 2 });

    auto grandchild21 = child2->add_child(DbEntity{ 21 });

    EXPECT_EQ(child2, child1->next_bfs());
    EXPECT_EQ(grandchild21, child2->next_bfs());
    EXPECT_EQ(nullptr, grandchild21->next_bfs());
  }

  TEST(TreeNode, add_child_2child2grandchildInOrder_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto child1 = root.add_child(DbEntity{ 1 });
    auto child2 = root.add_child(DbEntity{ 2 });

    auto grandchild11 = child1->add_child(DbEntity{ 11 });
    auto grandchild21 = child2->add_child(DbEntity{ 21 });

    EXPECT_EQ(child2, child1->next_bfs());
    EXPECT_EQ(grandchild11, child2->next_bfs());
    EXPECT_EQ(grandchild21, grandchild11->next_bfs());
    EXPECT_EQ(nullptr, grandchild21->next_bfs());
  }

  TEST(TreeNode, add_child_2child2grandchildInReverseOrder_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto child1 = root.add_child(DbEntity{ 1 });
    auto child2 = root.add_child(DbEntity{ 2 });

    auto grandchild21 = child2->add_child(DbEntity{ 21 });
    auto grandchild11 = child1->add_child(DbEntity{ 11 });

    EXPECT_EQ(child2, child1->next_bfs());
    EXPECT_EQ(grandchild11, child2->next_bfs());
    EXPECT_EQ(grandchild21, grandchild11->next_bfs());
    EXPECT_EQ(nullptr, grandchild21->next_bfs());
  }

  TEST(TreeNode, add_child_3child2grandchildInOrder_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto child1 = root.add_child(DbEntity{ 1 });
    auto child2 = root.add_child(DbEntity{ 2 });
    auto child3 = root.add_child(DbEntity{ 3 });

    auto grandchild11 = child1->add_child(DbEntity{ 11 });
    auto grandchild31 = child3->add_child(DbEntity{ 31 });

    EXPECT_EQ(child2, child1->next_bfs());
    EXPECT_EQ(child3, child2->next_bfs());
    EXPECT_EQ(grandchild11, child3->next_bfs());
    EXPECT_EQ(grandchild31, grandchild11->next_bfs());
    EXPECT_EQ(nullptr, grandchild31->next_bfs());
  }

  TEST(TreeNode, add_child_3child2grandchild3Atlast_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto child1 = root.add_child(DbEntity{ 1 });
    auto child2 = root.add_child(DbEntity{ 2 });
    auto grandchild11 = child1->add_child(DbEntity{ 11 });

    auto child3 = root.add_child(DbEntity{ 3 });
    auto grandchild31 = child3->add_child(DbEntity{ 31 });

    EXPECT_EQ(child2, child1->next_bfs());
    EXPECT_EQ(child3, child2->next_bfs());
    EXPECT_EQ(grandchild11, child3->next_bfs());
    EXPECT_EQ(grandchild31, grandchild11->next_bfs());
    EXPECT_EQ(nullptr, grandchild31->next_bfs());
  }

  TEST(TreeNode, add_child_3child3grandchild_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto child1 = root.add_child(DbEntity{ 1 });
    auto child2 = root.add_child(DbEntity{ 2 });
    auto child3 = root.add_child(DbEntity{ 3 });

    auto grandchild11 = child1->add_child(DbEntity{ 11 });
    auto grandchild12 = child1->add_child(DbEntity{ 12 });
    auto grandchild31 = child3->add_child(DbEntity{ 31 });

    EXPECT_EQ(child2, child1->next_bfs());
    EXPECT_EQ(child3, child2->next_bfs());
    EXPECT_EQ(grandchild11, child3->next_bfs());
    EXPECT_EQ(grandchild12, grandchild11->next_bfs());
    EXPECT_EQ(grandchild31, grandchild12->next_bfs());
    EXPECT_EQ(nullptr, grandchild31->next_bfs());
  }

  TEST(TreeNode, add_child_3child3grandchild3Atlast_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto child1 = root.add_child(DbEntity{ 1 });
    auto child2 = root.add_child(DbEntity{ 2 });
    auto grandchild11 = child1->add_child(DbEntity{ 11 });
    auto grandchild12 = child1->add_child(DbEntity{ 12 });

    auto child3 = root.add_child(DbEntity{ 3 });
    auto grandchild31 = child3->add_child(DbEntity{ 31 });

    EXPECT_EQ(child2, child1->next_bfs());
    EXPECT_EQ(child3, child2->next_bfs());
    EXPECT_EQ(grandchild11, child3->next_bfs());
    EXPECT_EQ(grandchild12, grandchild11->next_bfs());
    EXPECT_EQ(grandchild31, grandchild12->next_bfs());
    EXPECT_EQ(nullptr, grandchild31->next_bfs());
  }



  TEST(TreeNode, add_child_3_3_1_111_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto c1 = root.add_child(DbEntity{ 1 });
    auto c2 = root.add_child(DbEntity{ 2 });
    auto c3 = root.add_child(DbEntity{ 3 });

    auto c11 = c1->add_child(DbEntity{ 11 });
    auto c12 = c1->add_child(DbEntity{ 12 });
    auto c31 = c3->add_child(DbEntity{ 31 });

    auto c111 = c11->add_child(DbEntity{ 111 });

    EXPECT_EQ(c2, c1->next_bfs());
    EXPECT_EQ(c3, c2->next_bfs());
    EXPECT_EQ(c11, c3->next_bfs());
    EXPECT_EQ(c12, c11->next_bfs());
    EXPECT_EQ(c31, c12->next_bfs());
    EXPECT_EQ(c111, c31->next_bfs());
    EXPECT_EQ(nullptr, c111->next_bfs());
  }

  TEST(TreeNode, add_child_3_3_1_111_r_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto c1 = root.add_child(DbEntity{ 1 });
    auto c2 = root.add_child(DbEntity{ 2 });

    auto c11 = c1->add_child(DbEntity{ 11 });
    auto c12 = c1->add_child(DbEntity{ 12 });
    auto c111 = c11->add_child(DbEntity{ 111 });

    auto c3 = root.add_child(DbEntity{ 3 });
    auto c31 = c3->add_child(DbEntity{ 31 });

    EXPECT_EQ(c2, c1->next_bfs());
    EXPECT_EQ(c3, c2->next_bfs());
    EXPECT_EQ(c11, c3->next_bfs());
    EXPECT_EQ(c12, c11->next_bfs());
    EXPECT_EQ(c31, c12->next_bfs());
    EXPECT_EQ(c111, c31->next_bfs());
    EXPECT_EQ(nullptr, c111->next_bfs());
  }


  TEST(TreeNode, add_child_3_3_1_121_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto c1 = root.add_child(DbEntity{ 1 });
    auto c2 = root.add_child(DbEntity{ 2 });
    auto c3 = root.add_child(DbEntity{ 3 });

    auto c11 = c1->add_child(DbEntity{ 11 });
    auto c12 = c1->add_child(DbEntity{ 12 });
    auto c31 = c3->add_child(DbEntity{ 31 });

    auto c121 = c12->add_child(DbEntity{ 121 });

    EXPECT_EQ(c2, c1->next_bfs());
    EXPECT_EQ(c3, c2->next_bfs());
    EXPECT_EQ(c11, c3->next_bfs());
    EXPECT_EQ(c12, c11->next_bfs());
    EXPECT_EQ(c31, c12->next_bfs());
    EXPECT_EQ(c121, c31->next_bfs());
    EXPECT_EQ(nullptr, c121->next_bfs());
  }

  TEST(TreeNode, add_child_3_3_1_121_r_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto c1 = root.add_child(DbEntity{ 1 });
    auto c2 = root.add_child(DbEntity{ 2 });
    auto c3 = root.add_child(DbEntity{ 3 });

    auto c11 = c1->add_child(DbEntity{ 11 });
    auto c12 = c1->add_child(DbEntity{ 12 });
    auto c121 = c12->add_child(DbEntity{ 121 });

    auto c31 = c3->add_child(DbEntity{ 31 });


    EXPECT_EQ(c2, c1->next_bfs());
    EXPECT_EQ(c3, c2->next_bfs());
    EXPECT_EQ(c11, c3->next_bfs());
    EXPECT_EQ(c12, c11->next_bfs());
    EXPECT_EQ(c31, c12->next_bfs());
    EXPECT_EQ(c121, c31->next_bfs());
    EXPECT_EQ(nullptr, c121->next_bfs());
  }


  TEST(TreeNode, add_child_3_3_1_131_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto c1 = root.add_child(DbEntity{ 1 });
    auto c2 = root.add_child(DbEntity{ 2 });
    auto c3 = root.add_child(DbEntity{ 3 });

    auto c11 = c1->add_child(DbEntity{ 11 });
    auto c12 = c1->add_child(DbEntity{ 12 });
    auto c31 = c3->add_child(DbEntity{ 31 });

    auto c131 = c31->add_child(DbEntity{ 131 });

    EXPECT_EQ(c2, c1->next_bfs());
    EXPECT_EQ(c3, c2->next_bfs());
    EXPECT_EQ(c11, c3->next_bfs());
    EXPECT_EQ(c12, c11->next_bfs());
    EXPECT_EQ(c31, c12->next_bfs());
    EXPECT_EQ(c131, c31->next_bfs());
    EXPECT_EQ(nullptr, c131->next_bfs());
  }

  TEST(TreeNode, add_child_3_3_1_211_131_bfsOk)
  {
    TreeNode<DbEntity> root;
    auto c1 = root.add_child(DbEntity{ 1 });
    auto c2 = root.add_child(DbEntity{ 2 });
    auto c21 = c2->add_child(DbEntity{ 21 });
    auto c211 = c21->add_child(DbEntity{ 211 });
    auto c212 = c21->add_child(DbEntity{ 212 });

    auto c3 = root.add_child(DbEntity{ 3 });

    auto c11 = c1->add_child(DbEntity{ 11 });
    auto c12 = c1->add_child(DbEntity{ 12 });
    auto c31 = c3->add_child(DbEntity{ 31 });

    auto c131 = c31->add_child(DbEntity{ 131 });

    EXPECT_EQ(c2, c1->next_bfs());
    EXPECT_EQ(c3, c2->next_bfs());
    EXPECT_EQ(c11, c3->next_bfs());
    EXPECT_EQ(c12, c11->next_bfs());
    EXPECT_EQ(c21, c12->next_bfs());
    EXPECT_EQ(c31, c21->next_bfs());
    EXPECT_EQ(c211, c31->next_bfs());
    EXPECT_EQ(c212, c211->next_bfs());
    EXPECT_EQ(c131, c212->next_bfs());
    EXPECT_EQ(nullptr, c131->next_bfs());
  }

  TEST(TreeNode, child_begin_in_depth__empty__0__root)
  {
    TreeNode<int> root(0);

    auto node = root.child_begin_in_depth(0);
    EXPECT_EQ(&root, node);
  }

  TEST(TreeNode, child_begin_in_depth__2level__0__root)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c11 = c1->add_child(11);

    auto node = root.child_begin_in_depth(0);
    EXPECT_EQ(&root, node);
  }

  TEST(TreeNode, child_begin_in_depth__2level__1__c1)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c11 = c1->add_child(11);

    auto node = root.child_begin_in_depth(1);
    EXPECT_EQ(c1, node);
  }

  TEST(TreeNode, child_begin_in_depth__2level__2__c11)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c11 = c1->add_child(11);

    auto node = root.child_begin_in_depth(2);
    EXPECT_EQ(c11, node);
  }

  TEST(TreeNode, child_begin_in_depth__2level__2__c21)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c21 = c2->add_child(21);

    auto node = root.child_begin_in_depth(2);
    EXPECT_EQ(c21, node);
  }

  TEST(TreeNode, child_begin_in_depth__3level__3__c211)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c21 = c2->add_child(21);
    auto c211 = c21->add_child(211);

    auto node = root.child_begin_in_depth(3);
    EXPECT_EQ(c211, node);
  }

  TEST(TreeNode, child_begin_in_depth__3level__4__null)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c21 = c2->add_child(21);
    auto c211 = c21->add_child(211);

    auto node = root.child_begin_in_depth(4);
    EXPECT_EQ(nullptr, node);
  }

  TEST(TreeNode, child_end_in_depth__empty__0__null)
  {
    TreeNode<int> root(0);

    auto node = root.child_end_in_depth(0);
    EXPECT_EQ(nullptr, node);
  }

  TEST(TreeNode, child_end_in_depth__2level__0__c1)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c11 = c1->add_child(11);

    auto node = root.child_end_in_depth(0);
    EXPECT_EQ(c1, node);
  }

  TEST(TreeNode, child_end_in_depth__2level__1__c11)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c11 = c1->add_child(11);

    auto node = root.child_end_in_depth(1);
    EXPECT_EQ(c11, node);
  }

  TEST(TreeNode, child_end_in_depth__2level11__2__null)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c11 = c1->add_child(11);

    auto node = root.child_end_in_depth(2);
    EXPECT_EQ(nullptr, node);
  }

  TEST(TreeNode, child_end_in_depth__2level21__2__null)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c21 = c2->add_child(21);

    auto node = root.child_end_in_depth(2);
    EXPECT_EQ(nullptr, node);
  }

  TEST(TreeNode, child_end_in_depth__3level__2__c211)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c21 = c2->add_child(21);
    auto c211 = c21->add_child(211);

    auto node = root.child_end_in_depth(2);
    EXPECT_EQ(c211, node);
  }

  TEST(TreeNode, child_end_in_depth__3level__4__null)
  {
    TreeNode<int> root(0);
    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);
    auto c21 = c2->add_child(21);
    auto c211 = c21->add_child(211);

    auto node = root.child_end_in_depth(4);
    EXPECT_EQ(nullptr, node);
  }

  TEST(TreeNode, clear_root_DoesNothing)
  {
    TreeNode<int> root(0);
    root.clear();

    EXPECT_EQ(0, root.get());
    EXPECT_EQ(nullptr, root.child_first());
    EXPECT_EQ(nullptr, root.next());
  }

  TEST(TreeNode, clear_root_ChildsAreNull)
  {
    TreeNode<int> root(0);
    root.add_child(1)->add_child(11);
    root.add_child(2);
    root.add_child(3);

    root.clear();

    EXPECT_EQ(nullptr, root.child_first());
    EXPECT_EQ(nullptr, root.child_last());
  }


  TEST(TreeNode, clear_c21_complex)
  {
    TreeNode<int> root(0);
    root.add_child(1)
      ->add_child(11)
      ->add_child(111)
      ->parent()
      ->add_child(112)
      ->parent()
      ->add_child(113)
      ->add_child(1131)
      ->add_child(11311);

    auto c21 = root.add_child(2)
      ->add_child(21);

    c21->add_child(211)
      ->add_child(2111);

    c21->add_child(212)
      ->add_child(2121);
    c21->add_child(213)
      ->add_child(2131);

    root.add_child(3);

    vector<int> vals_before;
    copy(root.begin(), root.end(), back_inserter(vals_before));
    vector<int> const expected_before = { 0, 1, 2, 3, 11, 21, 111, 112, 113, 211, 212, 213, 1131, 2111, 2121, 2131, 11311 };
    EXPECT_EQ(expected_before, vals_before);

    c21->clear();
    vector<int> vals_after;
    copy(root.begin(), root.end(), back_inserter(vals_after));

    vector<int> const expected_after = { 0, 1, 2, 3, 11, 21, 111, 112, 113, 1131, 11311 };
    EXPECT_EQ(expected_after, vals_after);
  }

  TEST(TreeNode, remove_c21_1level)
  {
    TreeNode<int> root(0);
    root.add_child(1);
    auto c2 = root.add_child(2);
    root.add_child(3);

    c2->remove();

    vector<int> vals;
    copy(root.begin(), root.end(), back_inserter(vals));

    vector<int> expected = { 0, 1, 3 };

    EXPECT_EQ(expected, vals);
  }

  TEST(TreeNode, remove_c21_complex)
  {
    TreeNode<int> root(0);
    root.add_child(1)
      ->add_child(11)
      ->add_child(111)
      ->parent()
      ->add_child(112)
      ->parent()
      ->add_child(113)
      ->add_child(1131)
      ->add_child(11311);

    auto c21 = root.add_child(2)
      ->add_child(21);

    c21->add_child(211)
      ->add_child(2111);

    c21->add_child(212)
      ->add_child(2121);
    c21->add_child(213)
      ->add_child(2131);

    root.add_child(3);

    vector<int> vals_before;
    copy(root.begin(), root.end(), back_inserter(vals_before));
    vector<int> const expected_before = { 0, 1, 2, 3, 11, 21, 111, 112, 113, 211, 212, 213, 1131, 2111, 2121, 2131, 11311 };
    EXPECT_EQ(expected_before, vals_before);

    c21->remove();
    vector<int> vals_after;
    copy(root.begin(), root.end(), back_inserter(vals_after));

    vector<int> const expected_after = { 0, 1, 2, 3, 11, 111, 112, 113, 1131, 11311 };
    EXPECT_EQ(expected_after, vals_after);
  }

  TEST(TreeNode, begin_segment_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.begin_segment();
  }

  TEST(TreeNode, end_segment_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.end_segment();
  }

  TEST(TreeNode, begin_segment_nodeptr_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.begin_segment<TreeNode<int> const*>();
  }

  TEST(TreeNode, end_segment_nodeptr_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.end_segment<TreeNode<int> const*>();
  }

  TEST(TreeNode, begin_dfs_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.begin_dfs();
  }

  TEST(TreeNode, end_dfs_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.end_dfs();
  }

  TEST(TreeNode, begin_dfs_nodeptr_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.begin_dfs<TreeNode<int> const*>();
  }

  TEST(TreeNode, end_dfs_nodeptr_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.end_dfs<TreeNode<int> const*>();
  }

  TEST(TreeNode, begin_bfs_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.begin_bfs();
  }

  TEST(TreeNode, end_bfs_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.end_bfs();
  }

  TEST(TreeNode, begin_bfs_nodeptr_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.begin_bfs<TreeNode<int> const*>();
  }

  TEST(TreeNode, end_bfs_nodeptr_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.end_bfs<TreeNode<int> const*>();
  }

  TEST(TreeNode, begin_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.begin();
  }

  TEST(TreeNode, end_const_ShouldCompile)
  {
    TreeNode<int> const root;
    root.end();
  }
}



namespace IteratorSegmentTests
{
  using namespace std;
  TEST(IteratorSegment, begin_segment_RootEmptySg_Null)
  {

    TreeNode<DbEntity> root;
    EXPECT_EQ(IteratorSegment<DbEntity>(nullptr), root.begin_segment());
  }

  TEST(IteratorSegment, end_segment_RootEmptySg_Null)
  {

    TreeNode<DbEntity> root;
    EXPECT_EQ(IteratorSegment<DbEntity>(nullptr), root.end_segment());
  }

  TEST(IteratorSegment, begin_segment_1Child_Is1)
  {
    TreeNode<DbEntity> root;
    auto const node = root.add_child(DbEntity{ 1 });

    EXPECT_EQ(node->get().val, (*root.begin_segment()).val);
  }

  TEST(IteratorSegment, end_segment_1Child_IsNull)
  {
    TreeNode<DbEntity> root;
    auto const node = root.add_child(DbEntity{ 1 });

    EXPECT_EQ(IteratorSegment<DbEntity>(nullptr), root.end_segment());
  }

  TEST(IteratorSegment, pointer_operator_T)
  {
    TreeNode<DbEntity> root;
    auto const node = root.add_child(DbEntity{ 1 });

    EXPECT_EQ(node->get().val, root.begin_segment()->val);
  }

  TEST(IteratorSegment, usability_copy_value123)
  {
    TreeNode<DbEntity> root;
    root.add_child(DbEntity{ 1 });
    root.add_child(DbEntity{ 2 });
    root.add_child(DbEntity{ 3 });

    vector<DbEntity> vals;
    copy(root.begin_segment(), root.end_segment(), back_inserter(vals));

    auto const expected = vector<DbEntity>{ DbEntity{1}, DbEntity{2}, DbEntity{3} };
    EXPECT_EQ(expected, vals);
  }
  
  TEST(IteratorSegment, usability_copy_node123)
  {
    TreeNode<DbEntity> root;
    auto node1 = root.add_child(DbEntity{ 1 });
    auto node2 = root.add_child(DbEntity{ 2 });
    auto node3 = root.add_child(DbEntity{ 3 });

    vector<TreeNode<DbEntity>*> vals;
    copy(root.begin_segment<TreeNode<DbEntity>*>(), root.end_segment<TreeNode<DbEntity>*>(), back_inserter(vals));
    
    auto const expected = vector<TreeNode<DbEntity>*>{ node1, node2, node3 };
    EXPECT_EQ(expected, vals);
  }


#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
  TEST(IteratorSegment, usability_copy_par_val10)
  {
    constexpr int n = 10;
    TreeNode<DbEntity> root;

    vector<DbEntity> expected;
    for (int i = 0; i < n; ++i)
      root.add_child(expected.emplace_back(DbEntity{ i + 1 }));

    vector<DbEntity> vals(n);
    copy(execution::par_unseq, root.begin_segment(), root.end_segment(), vals.begin());

    EXPECT_EQ(expected, vals);
  }
  
#endif
}

namespace IteratorDepthFirstSearchTests
{
  using namespace std;

  TEST(IteratorDfs, order_copy_value0)
  {
    TreeNode<int> root(0);

    vector<int> vals;
    copy(root.begin_dfs(), root.end_dfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0 };
    EXPECT_EQ(expected, vals);
  }
  TEST(IteratorDfs, order_copy_value0123)
  {
    TreeNode<int> root;
    root.add_child(1);
    root.add_child(2);
    root.add_child(3);

    vector<int> vals;
    copy(root.begin_dfs(), root.end_dfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 2, 3};
    EXPECT_EQ(expected, vals);
  }

  TEST(IteratorDfs, order_copy_value011123)
  {
    TreeNode<int> root;
    auto n1 = root.add_child(1);
    auto n2 = root.add_child(2);
    auto n3 = root.add_child(3);

    n1->add_child(11);

    vector<int> vals;
    copy(root.begin_dfs(), root.end_dfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 11, 2, 3 };
    EXPECT_EQ(expected, vals);
  }

  TEST(IteratorDfs, order_copy_value012213)
  {
    TreeNode<int> root;
    root.add_child(1);
    root.add_child(2)
      ->add_child(21);

    root.add_child(3);

    vector<int> vals;
    copy(root.begin_dfs(), root.end_dfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 2, 21, 3 };
    EXPECT_EQ(expected, vals);
  }


  TEST(IteratorDfs, order_copy_value012331)
  {
    TreeNode<int> root;
    root.add_child(1);
    root.add_child(2);
    
    root.add_child(3)
      ->add_child(31);

    vector<int> vals;
    copy(root.begin_dfs(), root.end_dfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 2, 3, 31 };
    EXPECT_EQ(expected, vals);
  }


  TEST(IteratorDfs, order_copy_value01112331)
  {
    TreeNode<int> root;
    root.add_child(1)
      ->add_child(11);

    root.add_child(2);

    root.add_child(3)
      ->add_child(31);

    vector<int> vals;
    copy(root.begin_dfs(), root.end_dfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 11, 2, 3, 31 };
    EXPECT_EQ(expected, vals);
  }


  TEST(IteratorDfs, order_copy_value011111123)
  {
    TreeNode<int> root;
    root.add_child(1)
      ->add_child(11)
      ->add_child(111);
    
    root.add_child(2);
    root.add_child(3);


    vector<int> vals;
    copy(root.begin_dfs(), root.end_dfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 11, 111, 2, 3 };
    EXPECT_EQ(expected, vals);
  }


  TEST(IteratorDfs, order_copy_value01111112212113)
  {
    TreeNode<int> root;
    root.add_child(1)
      ->add_child(11)
      ->add_child(111);

    auto n2 = root.add_child(2)
      ->add_child(21)
      ->add_child(211);

    auto n3 = root.add_child(3);

    vector<int> vals;
    copy(root.begin_dfs(), root.end_dfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 11, 111, 2, 21, 211, 3 };
    EXPECT_EQ(expected, vals);
  }

  TEST(StepManagerDfs, next_1_null)
  {
    TreeNode<int> root;
    TreeNode<int>* node = &root;

    StepManagerDfs::next<TreeNode<int>>(node);

    EXPECT_EQ(nullptr, node);
  }

  TEST(StepManagerDfs, prev_1_null)
  {
    TreeNode<int> root;
    TreeNode<int>* node = &root;

    StepManagerDfs::prev<TreeNode<int>>(node);

    EXPECT_EQ(nullptr, node);
  }

  TEST(StepManagerDfs, next_parent_child)
  {
    TreeNode<int> root(0);
    auto node = &root;

    auto ch = root.add_child(1);

    StepManagerDfs::next<TreeNode<int>>(node);

    EXPECT_EQ(ch, node);
  }

  TEST(StepManagerDfs, prev_parent_child)
  {
    TreeNode<int> root(0);

    auto c1 = root.add_child(1);

    auto node = c1;
    StepManagerDfs::prev<TreeNode<int>>(node);

    EXPECT_EQ(&root, node);
  }

  TEST(StepManagerDfs, next_neighbour)
  {
    TreeNode<int> root(0);

    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);

    auto node = c1;
    StepManagerDfs::next<TreeNode<int>>(node);

    EXPECT_EQ(c2, node);
  }

  TEST(StepManagerDfs, prev_neighbour)
  {
    TreeNode<int> root(0);

    auto c1 = root.add_child(1);
    auto c2 = root.add_child(2);

    auto node = c2;
    StepManagerDfs::prev<TreeNode<int>>(node);

    EXPECT_EQ(c1, node);
  }

  TEST(StepManagerDfs, next_c1111_to_c2)
  {
    TreeNode<int> root(0);

    auto c1111 = root.add_child(1)
      ->add_child(11)
      ->add_child(111)
      ->add_child(1111);

    auto c2 = root.add_child(2);

    auto node = c1111;
    StepManagerDfs::next<TreeNode<int>>(node);

    EXPECT_EQ(c2, node);
  }

  TEST(StepManagerDfs, prev_c2_to_c1111)
  {
    TreeNode<int> root(0);

    auto c1111 = root.add_child(1)
      ->add_child(11)
      ->add_child(111)
      ->add_child(1111);

    auto c2 = root.add_child(2);

    auto node = c2;
    StepManagerDfs::prev<TreeNode<int>>(node);

    EXPECT_EQ(c1111, node);
  }

  TEST(StepManagerDfs, next_c1111_null)
  {
    TreeNode<int> root(0);

    auto c1111 = root.add_child(1)
      ->add_child(11)
      ->add_child(111)
      ->add_child(1111);

    auto node = c1111;
    StepManagerDfs::next<TreeNode<int>>(node);

    EXPECT_EQ(nullptr, node);
  }

  TEST(StepManagerDfs, prev_c1111_c111)
  {
    TreeNode<int> root(0);

    auto c111 = root.add_child(1)
      ->add_child(11)
      ->add_child(111);


    auto c1111 = c111->add_child(1111);

    auto node = c1111;
    StepManagerDfs::prev<TreeNode<int>>(node);

    EXPECT_EQ(c111, node);
  }
}



namespace IteratorBreadthFirstSearchTests
{
  using namespace std;

  TEST(IteratorBfs, order_copy_value0)
  {
    TreeNode<int> root(0);

    vector<int> vals;
    copy(root.begin_bfs(), root.end_bfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0 };
    EXPECT_EQ(expected, vals);
  }

  TEST(IteratorBfs, order_copy_value0123)
  {
    TreeNode<int> root;
    root.add_child(1);
    root.add_child(2);
    root.add_child(3);

    vector<int> vals;
    copy(root.begin_bfs(), root.end_bfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 2, 3 };
    EXPECT_EQ(expected, vals);
  }

  TEST(IteratorBfs, order_copy_value012311212231111311)
  {
    TreeNode<int> root;
    root.add_child(1)
      ->add_child(11)
      ->add_child(111);
    auto node2 = root.add_child(2);
    node2->add_child(21);

    root.add_child(3)
      ->add_child(31)
      ->add_child(311);

    node2->add_child(22);

    vector<int> vals;
    copy(root.begin_bfs(), root.end_bfs(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 2, 3, 11, 21, 22, 31, 111, 311 };
    EXPECT_EQ(expected, vals);
  }


  TEST(IteratorBfs, order_inrange_level23_copy_value11212231111311)
  {
    TreeNode<int> root;
    root.add_child(1)
      ->add_child(11)
      ->add_child(111);
    auto node2 = root.add_child(2);
    node2->add_child(21);

    root.add_child(3)
      ->add_child(31)
      ->add_child(311)
      ->add_child(3111);

    node2->add_child(22);

    vector<int> vals;
    copy(root.begin_bfs(2), root.end_bfs(3), back_inserter(vals));

    auto const expected = vector<int>{ 11, 21, 22, 31, 111, 311 };
    EXPECT_EQ(expected, vals);
  }

  TEST(IteratorBfs, begin_end_const_ShouldCompile)
  {
    TreeNode<int> root;
    root.add_child(1)
      ->add_child(11)
      ->add_child(111);
    auto node2 = root.add_child(2);
    node2->add_child(21);

    root.add_child(3)
      ->add_child(31)
      ->add_child(311);

    node2->add_child(22);


    auto const& root_const = root;
    vector<int> vals;
    copy(root_const.begin(), root_const.end(), back_inserter(vals));

    auto const expected = vector<int>{ 0, 1, 2, 3, 11, 21, 22, 31, 111, 311 };
    EXPECT_EQ(expected, vals);
  }
}