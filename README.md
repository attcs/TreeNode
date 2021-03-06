# TreeNode with  Breadth-first-search iterator
TreeNode is a self-contained linked list which represents a tree data structure.<br>
Implemented bidirectional iterators
* Segment iterator (`IteratorSegment`, `IteratorSegmentConst`: iterates over only the child elements)
* Depth-first-search iterator (`IteratorDfs`, `IteratorDfsConst`: iterates over the tree)
* Breadth-first-search iterator (`IteratorBfs`, `IteratorBfsConst`: iterates over the tree)

## Requirements
* Language standard: C++14 or above
* STL Headers: \<memory\>

## Motivation
A general software database hierarchy is often handled by a heterogenous container in a tree representation. In this case, the TreeNode solution could be very helpful:
* Iterators make available the standard algorithms for the database
* Using different traversal types, search and selector algorithms can be optimized (e.g.: finding a knowingly shallow element in a deep tree structure is much faster with the BFS traversal)

## Notes
* Header only implementation (treenode.h and required standard headers are needed to be included).
* Recommended if the data purposely stored in a tree structure and element removal is needed, otherwise a contiguous container could be a better choice.
* The solution is based on multiple double linked lists, along with all its pros and cons. 
* Homogenous container (heterogeneous elements based in a common ancestor can be stored by smart ptrs: `TreeNode<unique_ptr<DbEntityBase>> root`)
* Navigator element are available: `next()`, `prev()`, `parent()`, `child_first()`, `child_last()`, `next_bfs()`, `prev_bfs()`
* Level begin and end element can be found by `child_begin_in_depth()`/`child_end_in_depth()` function
* C++17 execution policies are supported.
* Bidirectional iterators could be implemented in a very compact way (12in1), using static polymorphism.
* Elements are stored by `std::unique_ptr`-s.
* Special `begin_*()` and `end_*()` can be templated with TreeNode, if hierarchy information is needed.
* Unittest is attached. (GTEST)

## Basic examples
```C++
    ...
    // Initialization
    
    const TreeNode<int> list = { 0, 1, 2, 3, 4 }; // Initializer list: root: 0; childs: 1, 2, 3, 4
    auto list_copy = list; // Copy ctor and Copy-assignment use deep-copy semantic
    
    TreeNode<int> root(0); // Value init using move


    // Adding and removing elements
    
    auto c1 = root.add_child(1);
    auto c11 = c1->add_child(11);
        
    c11->add_child(111)
        ->add_child(1111);
    c11->add_child(112);

    c11->clear();  // remove all subelement of c11: 111, 1111, 112
    c11->remove(); // remove c11 itself and all subelement

    auto c2 = root.add_child(2);
    c2->add_child(21)
        ->add_child(211);
      
    auto c3 = root.add_child(3);
    auto c4 = root.add_child(4);


    // Traversals
    
    std::vector<int> vIntBfs; // Values in order of BFS
    std::copy(root.begin(), root.end(), back_inserter(vIntBfs)); // { 0, 1, 2, 3, 4, 21, 211 }
    
    std::vector<int> vIntBfsForeach;
    for (autoc const& v : root)
      vIntBfsForeach.push_back(v);  // { 0, 1, 2, 3, 4, 21, 211 }

    std::vector<int> vIntBfsL12; // Values on the 1. and 2. level in order of BFS
    std::copy(root.begin_bfs(1), root.end_bfs(2), back_inserter(vIntBfsL12)); // { 1, 2, 3, 4, 21 }
       
    std::vector<int> vIntDfs; // Values in order of DFS
    std::copy(root.begin_dfs(), root.end_dfs(), back_inserter(vIntDfs)); // { 0, 1, 2, 21, 211, 3, 4 }
       
    std::vector<int> vIntChild; // Values of root's childs
    std::copy(root.begin_segment(), root.end_segment(), back_inserter(vIntChild)); // { 1, 2, 3, 4 }
    
    using TN = TreeNode<int>*;
    std::vector<TN> vPtrChild; // TreeNode pointers of root's child
    std::copy(root.begin_segment<TN>(), root.end_segment<TN>(), back_inserter(vPtrChild)); // { c1, c2, c3, c4 }
    
    // C++17 execution policies
    std::vector<int> vIntBfsPar(root.size());
    std::transform(std::execution::par, root.begin(), root.end(), vIntBfsPar.begin(), [](auto const& v)
    { 
        return v;
    });
    
```
    
