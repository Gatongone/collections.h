# collections.h

  准备找工作复习数据结构时候写的一些简单容器，无线程安全，所以建议写项目的时候还是用 STL，该库仅刷题和学习使用。

  因为从 C# 转回 C++，用的还不太熟练,所以 emmmmmmm…………。主要还是因为习惯了C# system.collections.generic，又没时间重新学STL库，所以就自己写了个习惯上和 C# 差不多的库。

* ## 包含容器

  容器|描述
  ----|----
  List|动态扩容数组
  LinkedList|双向链表
  Stack|栈
  Queue|队列
  HashTable|哈希表
  MaxHeap|大根堆
  MinHeap|小根堆
  Tree|n叉树
  BinarySearchTree|二叉搜索树
  AdjacencyList|邻接表（十字邻接表 + 多重邻接表）
  AdjacencyMatrix|邻接矩阵

* ## 使用方法：

  ```cpp
  #include "collections.h"
  ```

* ## 枚举器 Enumerator

  总觉得实现 STL 迭代器接口很麻烦，所以自己写了个类似 C# 枚举器接口，只需要实现下面纯虚函数：
  
  * `MoveNext`：遍历时每一次成功返回 true，失败返回 false
  * `Reset`：开始枚举时的重置方法

  需要设置当前枚举值时，可以调用 `this->SetCurrent()` 方法



* ## 容器 Collection

  继承至 `Enumerator`，需要重写两个纯虚函数：
  
  * `Clear`：清空容器
  * `Count`：当前容器内元素数量

* ## 枚举遍历
  
  ### 方法一：
  
  ```cpp
  BinarySearchTree<int> tree;
  tree.Add(5);
  tree.Add(3);
  tree.Add(7);
  tree.Add(10);
  tree.Add(11);
  tree.Add(2);

  //设置遍历类型为后序遍历
  //tree.SetTraversalType(Tree<int>::TraversalType::Postorde);
  
  //枚举遍历
  enumerate(tree)
  {
      cout << tree.GetCurrentValue() << endl;
  }
  ```
  
  ### 方法二：
  
  ```cpp
  BinarySearchTree<int> tree;
  tree.Add(5);
  tree.Add(3);
  tree.Add(7);
  tree.Add(10);
  tree.Add(11);
  tree.Add(2);

  //设置遍历类型为后序遍历
  //tree.SetTraversalType(Tree<int>::TraversalType::Postorde);

  //枚举遍历
  foreach<int>(tree, [](auto item)
  {
      cout << *item << endl;
  });
  ```
