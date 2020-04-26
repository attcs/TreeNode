
// #include <memory>

template<typename T> 
class TreeNode;

template<typename T, typename TValueType, typename TRefAndPointerBase, typename TNode, typename StepManager>
class IteratorNodeTreeBase;


struct StepManagerSegment;

template<typename T, typename T_or_NodePtr = T>
using IteratorSegment = IteratorNodeTreeBase<T, T_or_NodePtr, T_or_NodePtr, TreeNode<T>, StepManagerSegment>;

template<typename T, typename T_or_NodePtr = T>
using IteratorSegmentConst = IteratorNodeTreeBase<T, T_or_NodePtr, T_or_NodePtr const, TreeNode<T> const, StepManagerSegment>;


struct StepManagerDfs;

template<typename T, typename T_or_NodePtr = T>
using IteratorDfs = IteratorNodeTreeBase<T, T_or_NodePtr, T_or_NodePtr, TreeNode<T>, StepManagerDfs>;

template<typename T, typename T_or_NodePtr = T>
using IteratorDfsConst = IteratorNodeTreeBase<T, T_or_NodePtr, T_or_NodePtr const, TreeNode<T> const, StepManagerDfs>;



struct StepManagerBfs;

template<typename T, typename T_or_NodePtr = T>
using IteratorBfs = IteratorNodeTreeBase<T, T_or_NodePtr, T_or_NodePtr, TreeNode<T>, StepManagerBfs>;

template<typename T, typename T_or_NodePtr = T>
using IteratorBfsConst = IteratorNodeTreeBase<T, T_or_NodePtr, T_or_NodePtr const, TreeNode<T> const, StepManagerBfs>;



template<typename T>
class TreeNode
{
private:
  T data{};

  TreeNode* _parent = nullptr;

  TreeNode* _prev = nullptr;
  std::unique_ptr<TreeNode> _next{};

  std::unique_ptr<TreeNode> _child_first{};
  TreeNode* _child_last = nullptr;

  TreeNode* _prev_bfs = nullptr;
  TreeNode* _next_bfs = nullptr;

public:
  TreeNode() = default;
  TreeNode(const TreeNode<T>&) = default;
  TreeNode(TreeNode<T>&&) = default;
  TreeNode<T>& operator=(TreeNode<T>&&) = default;
  TreeNode<T>& operator=(TreeNode<T> const&) = default;

  explicit TreeNode(T const& d) : data(d) {}
  explicit TreeNode(T && d) : data(std::forward<T>(d)) {}

  // Get data
  T const& get() const noexcept { return data; }
  T& get() noexcept { return data; }

  // Segment navigation
  inline TreeNode* prev() noexcept { return _prev; }
  inline TreeNode const* prev() const noexcept { return _prev; }

  inline TreeNode* next() noexcept { return _next.get(); }
  inline TreeNode const* next() const noexcept { return _next.get(); }

  // Subsegment navigation
  inline TreeNode* parent() { return _parent; }
  inline TreeNode const* parent() const { return _parent; }

  inline TreeNode* child_first() noexcept { return _child_first.get(); }
  inline TreeNode const* child_first() const noexcept { return _child_first.get(); }

  inline TreeNode* child_last() noexcept { return _child_last; }
  inline TreeNode const* child_last() const noexcept { return _child_last; }

  // Breadth first search
  inline TreeNode * prev_bfs() noexcept { return _prev_bfs; }
  inline TreeNode const* prev_bfs() const noexcept { return _prev_bfs; }

  inline TreeNode * next_bfs() noexcept { return _next_bfs; }
  inline TreeNode const* next_bfs() const noexcept { return _next_bfs; }

  size_t get_depth() const noexcept
  {
    size_t d = 0;
    for (auto p = _parent; p; p = p->_parent)
      ++d;

    return d;
  }

private:
  TreeNode<T>* _setup_child(std::unique_ptr<TreeNode<T>>&& node) noexcept
  {
    TreeNode<T>* child = nullptr;
    if (child_last())
    {
      auto last = child_last();
      last->_next.swap(node);
      child = last->next();

      child->_prev = last;
      child->_prev_bfs = last;
      child->_next_bfs = last->_next_bfs;
      if (child->_next_bfs)
        child->_next_bfs->_prev_bfs = child;

      last->_next_bfs = child;
    }
    else
    {
      _child_first.swap(node);
      child = child_first();

      auto const depth_this = get_depth();

      auto prev_bfs_node = this;
      auto next_bfs_node = this->_next_bfs;
      while (next_bfs_node)
      {
        auto const depth_p = next_bfs_node->get_depth();
          
        if (depth_p == depth_this && next_bfs_node->_child_last)
        {
          prev_bfs_node = next_bfs_node;
          next_bfs_node = next_bfs_node->child_first();
          break;
        }
          
        if (depth_p > depth_this + 1)
          break;

        prev_bfs_node = next_bfs_node;
        next_bfs_node = next_bfs_node->_parent->_child_last;
        if (depth_p == depth_this + 1 && !next_bfs_node)
          break;

        prev_bfs_node = next_bfs_node;
        next_bfs_node = next_bfs_node->_next_bfs;
      }


      if (next_bfs_node)
      {
        child->_next_bfs = next_bfs_node;
        child->_prev_bfs = next_bfs_node->_prev_bfs;
        next_bfs_node->_prev_bfs->_next_bfs = child;
        next_bfs_node->_prev_bfs = child;
      }
      else
      {
        prev_bfs_node->_next_bfs = child;
        child->_prev_bfs = prev_bfs_node;
      }
    }

    _child_last = child;
    child->_parent = this;

    return child;
  }

public:
  TreeNode<T>* add_child(T const& d) noexcept
  {
    return _setup_child(std::make_unique<TreeNode<T>>(d));
  }
  
  TreeNode<T>* add_child(T&& d) noexcept
  {
    return _setup_child(std::make_unique<TreeNode<T>>(d));
  }


  static inline void swap(TreeNode<T>* node1, TreeNode<T>* node2) { std::swap(node1->get(), node2->get()); }
  inline void swap(TreeNode<T>* node2) { swap(this, node2); }

  void clear() noexcept
  {
    if (_parent)
      for (auto n = child_first(); n != nullptr; n = n->next())
        n->clear();

    if (!_child_last)
      return;

    if (_child_last->_next_bfs)
      _child_last->_next_bfs->_prev_bfs = child_first()->_prev_bfs;

    if (child_first()->_prev_bfs)
      child_first()->_prev_bfs->_next_bfs = _child_last->_next_bfs;

    _child_first.reset(nullptr);
    _child_last = nullptr;
  }

  void remove() noexcept
  {
    if (!_parent)
    {
      assert(("Root cannot be removed!", false));
      return;
    }

    clear();

    // Bfs rewire
    if (_next_bfs)
      _next_bfs->_prev_bfs = _prev_bfs;

    if (_prev_bfs)
      _prev_bfs->_next_bfs = _next_bfs;

    // Container reset
    auto& container = _prev ? _prev->_next : _parent->_child_first ;
    if (_next)
    {
      _next->_prev = _prev;
      container.reset(_next.release());
    }
    else
    {
      container.reset(nullptr);
    }
  }

 
  //!TODO Separate the subtree, BFS will be update on both tree
  // std::unique_ptr<TreeNode<T>> deattach_child(TreeNode<T> const& child)  { return {}; }

public:
  TreeNode<T> const* child_begin_in_depth(size_t depth) const
  {
    size_t depth_current = 0;
    auto node = this;

    while (depth_current < depth)
    {
      while (node->child_first() && depth_current < depth)
      {
        node = node->child_first();
        ++depth_current;
      }

      if (depth_current < depth)
      {
        if (node->next())
          node = node->next();
        else
        {
          node = node->_next_bfs;
          if (!node)
            return nullptr;

          depth_current = node->get_depth();
        }
      }
    }
    return node;
  }
  
  TreeNode<T>* child_begin_in_depth(size_t depth)
  {
    return const_cast<TreeNode<T>*>(static_cast<const TreeNode<T>*>(this)->child_begin_in_depth(depth)); // Scott Meyers
  }


  TreeNode<T> const* child_end_in_depth(size_t depth) const
  {
    if (depth == -1)
      return nullptr;

    return child_begin_in_depth(depth + 1); // end: element after the last of level, so the next level begin element
  }

  TreeNode<T>* child_end_in_depth(size_t depth)
  {
    return const_cast<TreeNode<T>*>(static_cast<const TreeNode<T>*>(this)->child_end_in_depth(depth)); // Scott Meyers
  }


public: // Iterators

  // Segment iterator begin
  template<typename T_or_NodePtr = T>
  IteratorSegment<T, T_or_NodePtr> begin_segment() noexcept { return IteratorSegment<T, T_or_NodePtr>(child_first()); }

  // Segment iterator end
  template<typename T_or_NodePtr = T>
  IteratorSegment<T, T_or_NodePtr> end_segment() noexcept  { return IteratorSegment<T, T_or_NodePtr>(nullptr); }

  // Segment iterator begin
  template<typename T_or_NodePtr = T>
  IteratorSegmentConst<T, T_or_NodePtr> begin_segment() const noexcept { return IteratorSegmentConst<T, T_or_NodePtr>(child_first()); }

  // Segment iterator end
  template<typename T_or_NodePtr = T>
  IteratorSegmentConst<T, T_or_NodePtr> end_segment() const noexcept { return IteratorSegmentConst<T, T_or_NodePtr>(nullptr); }


  // Depth first search begin iterator
  template<typename T_or_NodePtr = T>
  IteratorDfs<T, T_or_NodePtr> begin_dfs() noexcept { return IteratorDfs<T, T_or_NodePtr>(this); }

  // Depth first search end iterator
  template<typename T_or_NodePtr = T>
  IteratorDfs<T, T_or_NodePtr> end_dfs() noexcept { return IteratorDfs<T, T_or_NodePtr>(nullptr); }

  // Depth first search begin iterator
  template<typename T_or_NodePtr = T>
  IteratorDfsConst<T, T_or_NodePtr> begin_dfs() const noexcept { return IteratorDfsConst<T, T_or_NodePtr>(this); }

  // Depth first search end iterator
  template<typename T_or_NodePtr = T>
  IteratorDfsConst<T, T_or_NodePtr> end_dfs() const noexcept { return IteratorDfsConst<T, T_or_NodePtr>(nullptr); }


  // Breadth first search begin iterator
  template<typename T_or_NodePtr = T>
  IteratorBfs<T, T_or_NodePtr> begin_bfs(size_t depth = 0) { return IteratorBfs<T, T_or_NodePtr>(child_begin_in_depth(depth)); }

  // Breadth first search end iterator
  template<typename T_or_NodePtr = T>
  IteratorBfs<T, T_or_NodePtr> end_bfs(size_t depth = -1) { return IteratorBfs<T, T_or_NodePtr>(child_end_in_depth(depth)); }

  // Breadth first search begin iterator
  template<typename T_or_NodePtr = T>
  IteratorBfsConst<T, T_or_NodePtr> begin_bfs(size_t depth = 0) const noexcept { return IteratorBfsConst<T, T_or_NodePtr>(child_begin_in_depth(depth)); }

  // Breadth first search end iterator
  template<typename T_or_NodePtr = T>
  IteratorBfsConst<T, T_or_NodePtr> end_bfs(size_t depth = -1) const noexcept { return IteratorBfsConst<T, T_or_NodePtr>(child_end_in_depth(depth)); }


  // Begin iterator (using Breadth first traversal)
  IteratorBfs<T> begin() noexcept { return IteratorBfs<T>(this); }

  // End iterator (using Breadth first traversal)
  IteratorBfs<T> end() noexcept { return IteratorBfs<T>(nullptr); }
  
  // Begin iterator (using Breadth first traversal)
  IteratorBfsConst<T> begin() const noexcept { return IteratorBfsConst<T>(this); }

  // End iterator (using Breadth first traversal)
  IteratorBfsConst<T> end() const noexcept { return IteratorBfsConst<T>(nullptr); }
};


template<typename T, typename TValueType, typename TRefAndPointerBase, typename TNode, typename StepManager>
class IteratorNodeTreeBase
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = TValueType;
  using difference_type = ptrdiff_t;
  using pointer = TRefAndPointerBase*;
  using reference = TRefAndPointerBase&;

protected:
  TNode* _node;

public:
  IteratorNodeTreeBase(TNode* node) noexcept : _node(node) { }

  IteratorNodeTreeBase() = default;
  IteratorNodeTreeBase(IteratorNodeTreeBase const&) = default;
  IteratorNodeTreeBase(IteratorNodeTreeBase&&) = default;

  static_assert(std::is_same<TValueType, T>::value || std::is_same<TValueType, TreeNode<T>*>::value || std::is_same<TValueType, TreeNode<T> const*>::value, "Only the type of data and TreeNode* are allowed.");

  IteratorNodeTreeBase& operator=(TNode* node)
  {
    this->_node = node;
    return *this;
  }

  bool operator==(IteratorNodeTreeBase const& r) const noexcept
  {
    return _node == r._node;
  }

  IteratorNodeTreeBase& operator++()
  {
    StepManager::next<TNode>(_node);
    return *this;
  }

  IteratorNodeTreeBase operator++(int)
  {
    auto iterator = *this;
    ++* this;
    return iterator;
  }

  IteratorNodeTreeBase& operator--()
  {
    StepManager::prev<TNode>(_node);
    return *this;
  }

  IteratorNodeTreeBase operator--(int)
  {
    auto iterator = *this;
    --* this;
    return iterator;
  }

  bool operator!=(IteratorNodeTreeBase const& iterator)
  {
    return _node != iterator._node;
  }

  IteratorNodeTreeBase& operator+=(size_t const& n)
  {
    for (int i = 0; i < n; ++i)
    {
      assert(("Out of Segment's range!", _node));
      ++* this;
    }
    return *this;
  }

  IteratorNodeTreeBase& operator-=(size_t const n)
  {
    for (int i = 0; i < n; ++i)
    {
      assert(("Out of Segment's range!", _node));
      --* this;
    }
    return *this;
  }

  IteratorNodeTreeBase operator+(size_t const n) const { auto it = *this; it += n; return it; }
  IteratorNodeTreeBase operator-(size_t const n) const { auto it = *this; it -= n; return it; }


private:

  template<typename TR>
  inline TR _get_reference(std::true_type const&) { return _node->get(); }

  template<typename TR>
  inline TR _get_reference(std::false_type const&) { return _node; }

  template<typename TP>
  inline TP _get_pointer(std::true_type const&) { return &_node->get(); }

  template<typename TP>
  inline TP _get_pointer(std::false_type const&) { return &_node; }


public:

  [[nodiscard]] reference operator*()
  {
    using typeval = std::conditional<std::is_same<value_type, T>::value, std::true_type, std::false_type>::type;
    return _get_reference<reference>(typeval{});
  }

  [[nodiscard]] pointer operator->()
  {
    using typeval = std::conditional<std::is_same<value_type, T>::value, std::true_type, std::false_type>::type;
    return _get_pointer<pointer>(typeval{});
  }
};


struct StepManagerSegment
{
  template<typename TNode>
  static inline void next(TNode*& _node)
  {
    if (_node)
      _node = _node->next();
  }

  template<typename TNode>
  static inline void prev(TNode*& _node)
  {
    if (_node)
      _node = _node->prev();
  }
};


struct StepManagerDfs
{
  template<typename TNode>
  static inline void next(TNode*& _node)
  {
    if (!_node)
      return;

    auto child = _node->child_first();
    if (child)
      _node = child;
    else
    {
      auto n = _node->next();
      if (n)
        _node = n;
      else
      {
        auto p = _node->parent();
        while (p && !p->next())
          p = p->parent();

        _node = p ? p->next() : nullptr;
      }
    }
  }

  template<typename TNode>
  static inline void prev(TNode*& _node)
  {
    if (!_node)
      return;

    auto prev = _node->prev();
    if (!prev)
      _node = _node->parent();
    else
    {
      auto child = prev->child_last();
      while (child && child->child_last())
        child = child->child_last();

      if (child)
        _node = child;
      else
        _node = prev;
    }
  }
};


struct StepManagerBfs
{
  template<typename TNode>
  static inline void next(TNode*& _node)
  {
    if (_node)
      _node = _node->next_bfs();
  }

  template<typename TNode>
  static inline void prev(TNode*& _node)
  {
    if (_node)
      _node = _node->prev_bfs();
  }
};
