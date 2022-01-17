#include <cstddef>
#include <string>

#define ERRORMESSAGE
#define error(message) throw runtime_error(message)


#ifndef COLLECTIONS
#define COLLECTIONS

#pragma region Enumerator

template <class T> class Enumerator
{
    private:
        T* m_current;
        bool m_start = false;
    protected:
        void SetCurrent(T& item) { m_current = &item; }
        virtual bool MoveNext() = 0;
        virtual void Reset() = 0;
    public:
        Enumerator() { m_current = NULL; }
        ~Enumerator(){if ( m_current != NULL ) {m_current = NULL;}}
        T** GetCurrentRef() { return &m_current; }
        T* GetCurrent() { return m_current; }
        T GetCurrentValue() { return *m_current; }
        void SetCurrentValue(const T& item) {*m_current = item; }
        bool AutoMoveNext()
        {
            if(!m_start)
            {
                Reset();
                if(m_current == NULL)
                    return false;
                m_start = true;
                return true;
            }
            if(MoveNext())
            {
                return true;
            }
            else
            {
                m_start = false;
                return false;
            }
        }
};

template<typename T>
void foreach(Enumerator<T>& enumerator,void (*callback)(T*))
{
    while(enumerator.AutoMoveNext())
    {
        callback(enumerator.GetCurrent());
    }
}

#ifndef ENMERATE
#define ENMERATE
#define enumerate(enumerator) while(enumerator.AutoMoveNext())
#endif

#pragma endregion

#pragma region Collection
    template <class T> class Collection : public Enumerator<T>
    {
        protected:
            virtual bool MoveNext() = 0;
            virtual void Reset() = 0;
        public:
            virtual inline unsigned long Count() = 0;
            virtual void Clear() = 0;
    };
#pragma endregion

#pragma region DS

template <class T> class List;
template <class T> class LinkedList;
template <class T> class Stack;
template <class T> class Queue;
template <class T> class BinaryHeap;
template <class T> class TreeNode;
template <class T> class Tree;
template <class K,class V> class KeyValuePair;
template <class K,class V> class HashTable;

//数组列表
template <class T> class List : public Collection<T>
{
    private:
        T *m_list;                       //元素列表
        unsigned long m_index;            //枚举编号
        unsigned long m_count;            //当前容量
        unsigned long m_capacity;         //最大容量
        inline void AutoExpand();        //自动扩容
    protected:
        bool MoveNext();
        void Reset();    
    public:
        List();
        List(T* array,unsigned long size);
        List(unsigned long capacity);
        ~List();
        T& operator[](unsigned long index);

        inline bool Contains(const T& item);                  //是否包含元素
        inline void Add(const T& item);                       //添加元素
        inline void Add(const T targets[],unsigned long size);//添加数组到列表
        inline void Add(List<T>& targets);                    //添加数组到列表
        inline void Insert(unsigned long index,const T& item);//插入元素
        inline bool Remove(const T& item);                    //移除元素
        inline bool RemoveAt(unsigned long index);            //移除指定位置元素
        void Clear();                                         //清空列表
        T* ToArray();                                         //列表转换为数组
        inline unsigned long Count();                         //返回元素数量
};
//双向链表
template <class T> class LinkedList : public Collection<T>
{
    protected:
        bool MoveNext();
        void Reset();
    public:        
        struct Node
        {
            T data;         //数据
            Node* next;     //下一个元素
            Node* prev;     //上一个元素
            Node(const T& data)
            {
                next = NULL;
                prev = NULL;
                this->data = data;
            }
        };
        LinkedList();
        ~LinkedList();
        T& operator[](unsigned long index);
        Node* GetNode(unsigned long index);                       //返回下标结点
        Node* AddLast(const T& item);                             //添加元素至链表尾端
        Node* AddFirst(const T& item);                            //添加元素至链表首端
        Node* InsertFront(unsigned long index, const T& item);    //插入元素到下标元素前
        Node* InsertBehind(unsigned long index, const T& item);   //插入元素到下标元素后
        bool RemoveLast();                                        //移除链表尾端
        bool RemoveFirst();                                       //移除链表首端
        bool RemoveAt(unsigned long index);                       //移除指定下标元素
        bool Remove(const T &item);                               //移除元素
        void Clear();                                             //清空链表
        void Link(const LinkedList<T>&);                          //与指定链表链接，返回链接后的链表
        inline unsigned long Count();                             //返回元素数量
    private:
        Node* m_currentNode;
        Node* m_head;
        Node* m_tail;
        unsigned long m_count;
};
//栈
template <class T> class Stack : public Collection<T>
{
    private:
        struct Node
        {
            T data;
            Node* next;
            Node(const T& data)
            {
                this->data = data;
                next = NULL;
            }
        };
        unsigned long m_count;
        Node *m_top;
        Node *m_currentNode;

    protected:
        bool MoveNext();
        void Reset();
    public:
        Stack();
        ~Stack();
        T& Pop();                       //出栈
        void Push(const T& item);       //入栈
        T& Peek();                      //返回栈顶元素
        void Clear();                   //清空
        inline unsigned long Count();   //返回元素数量
};
//队列
template <class T> class Queue : public Collection<T>
{
    private:
        struct Node
        {
            T data;
            Node* next;
            Node(const T& data)
            {
                this->data = data;
                next = NULL;
            }
        };
        unsigned long m_count;
        Node *m_head;
        Node *m_tail;
        Node *m_currentNode;
    protected:
        bool MoveNext();
        void Reset();
    public:
        Queue();
        ~Queue();
        T& Dequeue();                   //出队列
        void Enqueue(const T& item);    //入队列
        T& Peek();                      //返回队头
        void Clear();                   //清空队列
        inline unsigned long Count();   //返回元素数量
};
//键值对
template <class K,class V> struct KeyValuePair
{
    public:
        K key;
        V value;
        KeyValuePair<K, V>* next;
        KeyValuePair(void) {next  = NULL;}
        KeyValuePair(const K& key,const V& value) : key(key), value(value) { next = NULL; }
        KeyValuePair(const K& key,const V& value, KeyValuePair<K, V>* next) : key(key), value(value), next(next){}
};
//哈希表
static const unsigned long s_primes[30] = {
    11ul,23ul,53ul, 97ul, 193ul, 389ul, 769ul,
    1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
    49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
    1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
    50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
    1610612741ul, 3221225473ul, 4294967291ul
};
template <class K,class V> class HashTable : public Collection<KeyValuePair<K,V>>
{
    private:
        unsigned long m_index;            //当前索引
        KeyValuePair<K, V> *m_currentNode;//当前结点
        const int c_primesSize = 30;      //质数最大索引
        int m_primesIndex = 0;            //质素索引
        unsigned long m_count;            //当前容量
        unsigned long m_capacity;         //最大容量
        KeyValuePair<K, V>** m_container; //实际的数据指针数组

        inline unsigned long GetHashCode(const K& key);                        //返回哈希值
        unsigned long GetMaxPrime(unsigned long num);                          //获得最大质数
        bool IsPrime(unsigned long num);                                       //是否是质数
        void DealHashCollision(unsigned long hashCode,KeyValuePair<K, V>* kv); //处理哈希冲突
        void AutoExpand();                                                     //自动扩容

    protected:
        bool MoveNext();
        void Reset();
    public:
        HashTable();
        HashTable(unsigned long capacity);
        ~HashTable();
        V& operator[](const K& key);
        void Add(const K& key,const V& value);          //添加一对键值
        bool Remove(const K& key);                      //移除键
        bool ContainsKey(const K& key);                 //是否包含键
        bool ContainsValue(const V& value);             //是否包含值
        bool TryGetValue(const K &key, V &outValue);    //如果键存在则将值赋予outValue
        void Clear();                                   //清空
        inline unsigned long Count();                   //返回元素数量
};
//二叉堆
template <class T> class BinaryHeap : public Collection<T>
{
    private:
        unsigned int m_index;
        List<T>* m_heap;                     //堆数组
        void ShiftUp(unsigned long index);   //向上调整堆
        void ShiftDown(unsigned long index); //向下调整堆
        void Swap(T &a, T &b);               //交换元素

    protected:
        bool MoveNext();
        void Reset();
        virtual bool Compare(T& left,T& right) = 0;    //比较两元素大小，成功返回1
    public:
        BinaryHeap();
        BinaryHeap(unsigned long capacity);
        BinaryHeap(T* array,unsigned long size,bool heapify);
        ~BinaryHeap();
        void Heapify();                     //堆化
        void Add(const T& item);            //向堆中添加一个元素，并自动调整堆
        void AddWithoutShift(const T &item);//向堆中添加一个元素，不调整堆
        T& ExtractTop();                    //返回并移除堆顶元素
        T& Peek();                          //返回堆顶结点
        bool RemoveTop();                   //移除堆顶结点
        void Sort();                        //堆排序
        void Clear();                       //清空堆
        inline unsigned long Count();       //返回堆长度
};
//小顶堆
template <class T> class MinHeap : public BinaryHeap<T>
{
    protected:
    bool Compare(T &left, T &right);
    public:
        MinHeap() : BinaryHeap<T>(){}
        MinHeap(unsigned long capacity) : BinaryHeap<T>(capacity){}
        MinHeap(T* array,unsigned long size,bool heapify) : BinaryHeap<T>(array,size,heapify){if(heapify){this->Heapify();}}
};
//大顶堆
template <class T> class MaxHeap : public BinaryHeap<T>
{
    protected:
    bool Compare(T &left, T &right);
    public:
        MaxHeap() : BinaryHeap<T>(){}
        MaxHeap(unsigned long capacity) : BinaryHeap<T>(capacity){}
        MaxHeap(T* array,unsigned long size,bool heapify) : BinaryHeap<T>(array,size,heapify){if(heapify){this->Heapify();}}
};
//树结点
template <class T> struct TreeNode
{
    private:
        friend class Tree<T>;
        TreeNode** childPtrs;//当前结点指向孩子结点的指针
        int capacity;        //孩子容量
        Tree<T>* tree;       //当前节点所在树
    public:
        T data;              //数据
        int childCount;      //孩子数量
        TreeNode(Tree<T> *tree);
        TreeNode(Tree<T> *tree, const T &data);
        ~TreeNode();
        TreeNode<T>* AddChild(const T& data);
        TreeNode<T>* AddChildNode(TreeNode<T>* child);
        void Clear();
        int ChildCount();
        T& GetData(int index);
        void SetNode(int index, TreeNode<T>* node);
        TreeNode* GetNode(int index);
};
//树
template <class T> class Tree : public Collection<T>
{
    private:
        friend class TreeNode<T>;
        Queue<TreeNode<T>*> m_nodeQueue;
        Stack<TreeNode<T>*> m_nodeStack;
    public:
        Tree();
        Tree(const T& rootData);
        Tree(const TreeNode<T>* root);
        ~Tree();
        enum TraversalType//遍历类型
        {
            Preorder,//前序遍历，遍历顺序为：根、孩子
            Inorder, //中序遍历，遍历顺序为：第一个孩子、根、第二个孩子（仅限于二叉树使用）
            Postorde,//后序遍历，遍历顺序为：孩子、根
            Sequence //层序遍历
        };
        TraversalType CurrentTraversalType();
        void SetTraversalType(TraversalType traversalType);
        void Clear();
        inline unsigned long Count();
        TreeNode<T> *GetRoot();
        void RebuildTree(TreeNode<T>* root);
        void RebuildTree(const T& rootData);
    protected:
        unsigned long m_count;
        TreeNode<T>* m_root;
        TreeNode<T>* m_currentNode;
        TraversalType m_currentTraversalType; //当前遍历类型
        bool MoveNext();
        void Reset();
};
//二叉搜索树
template <class T> class BinarySearchTree : public Tree<T>
{
    public:
        BinarySearchTree();
        void Add(const T &item);
        bool Remove(const T &item);
        bool Contains(const T &item);
};
#pragma endregion

#include "List.tpp"
#include "LinkedList.tpp"
#include "Stack.tpp"
#include "Queue.tpp"
#include "HashTable.tpp"
#include "BinaryHeap.tpp"
#include "Tree.tpp"
#include "BinarySearchTree.tpp"

#undef ERRORMESSAGE
#endif