#include "collections.h"

#pragma region TreeNode

template<class T>
TreeNode<T>::TreeNode(Tree<T>* tree) 
{
    childCount = 0;
    capacity = 2;
    childPtrs = new TreeNode *[2]();
    this->tree = tree;
}

template<class T>
TreeNode<T>::TreeNode(Tree<T>* tree,const T& data) 
{
    childCount = 0;
    capacity = 2;
    this->data = data;
    this->tree = tree;
    childPtrs = new TreeNode *[2]();
}

template<class T>
TreeNode<T>::~TreeNode() 
{
    for (int i = 0; i < capacity; i++)
    {
        if(childPtrs[i])
        {
            delete[] childPtrs[i];
            childPtrs[i] = NULL;
        }
    }
    
}

//添加一个孩子，返回孩子结点
template<class T>
TreeNode<T>* TreeNode<T>::AddChild(const T& data)
{
    childCount++;
    if(childCount > capacity)
    {
        capacity *= 2;
        TreeNode<T>** newArray = new TreeNode<T>*[capacity];
        memcpy(newArray, childPtrs, capacity * sizeof(TreeNode<T>*));
        childPtrs = newArray;
    }
    TreeNode<T> *child = new TreeNode<T>(tree,data);
    childPtrs[childCount -1 ] = child;
    tree->m_count++;
    return child;
}

//添加一个孩子，返回本结点
template<class T>
TreeNode<T>* TreeNode<T>::AddChildNode(TreeNode<T>* child)
{
    childCount++;
    if(childCount > capacity)
    {
        capacity *= 2;
        TreeNode<T>** newArray = new TreeNode<T>*[capacity];
        memcpy(newArray, childPtrs, capacity * sizeof(TreeNode<T>*));
        childPtrs = newArray;
    }
    childPtrs[childCount -1 ] = child;
    tree->m_count++;
    return this;
}

//清空
template<class T>
void TreeNode<T>::Clear()
{
    for (int i = 0; i < childCount; i++)
    {
        delete[] childPtrs[i];
        childPtrs[i] = NULL;
    }
    tree->m_count -= childCount;
    childCount = 0;
}

//获得索引孩子的值
template<class T>
T& TreeNode<T>::GetData(int index)
{
    if(index < 0 || index >= capacity)
    {
        error("Error index");
    }
    return childPtrs[index]->data;
}

//获得索引孩子
template<class T>
TreeNode<T>* TreeNode<T>::GetNode(int index)
{
    if(index < 0)
    {
        return NULL;
    }
    return childPtrs[index];
}

//设置索引孩子
template<class T>
void TreeNode<T>::SetNode(int index,TreeNode<T>* node)
{
    if(index > capacity - 1)
    {
        capacity = index + 1;
        TreeNode<T>** newArray = new TreeNode<T>*[capacity];
        memcpy(newArray, childPtrs, capacity * sizeof(TreeNode<T>*));
        childPtrs = newArray;
    }
    if(childPtrs[index] == NULL && node != NULL)
    {
        childCount++;
        tree->m_count++;
    }
    if(node == NULL)
    {

        childCount--;
        tree->m_count--;
    }
    childPtrs[index] = node;
}

//孩子数量
template<class T>
int TreeNode<T>::ChildCount()
{
    return childCount;
}

#pragma endregion

#pragma region Tree

template <class T>
Tree<T>::Tree()
{
    m_currentNode = NULL;
    m_root = NULL;
    m_currentTraversalType = TraversalType::Sequence;
    m_count = 0;
}

template <class T>
Tree<T>::Tree(const T& rootData)
{
    m_currentNode = NULL;
    m_root = new TreeNode<T>(this,rootData);
    m_currentTraversalType = TraversalType::Sequence;
    m_count = 1;
}

template <class T>
Tree<T>::Tree(const TreeNode<T>* root)
{
    m_currentNode = NULL;
    m_root = root;
    m_currentTraversalType = TraversalType::Sequence;
    m_count = 1;
}

template <class T>
Tree<T>::~Tree()
{
    if(m_currentNode)
    {
        delete[] m_currentNode;
        m_currentNode = NULL;
    }
}


template <class T>
//元素数量
inline unsigned long Tree<T>::Count()
{
    return m_count;
}

//返回当前遍历类型
template <class T>
typename Tree<T>::TraversalType Tree<T>::CurrentTraversalType()
{
    return m_currentTraversalType;
}

//设置当前遍历类型
template <class T>
void Tree<T>::SetTraversalType(TraversalType traversalType)
{
    m_currentTraversalType = traversalType;
}

//返回根结点
template <class T>
TreeNode<T>* Tree<T>::GetRoot()
{
    return m_root;
}

//重新建立树
template <class T>
void Tree<T>::RebuildTree(TreeNode<T>* root)
{
    root->tree = this;
    m_root = root;
    m_count = 1;
}

//重新建立树
template <class T>
void Tree<T>::RebuildTree(const T& rootData)
{
    m_count = 1;
    m_root = new TreeNode<T>(this,rootData);
}

//清空树
template <class T>
void Tree<T>::Clear()
{
    m_count = 0;
    m_root = NULL;
}

//枚举器移动
template <class T>
bool Tree<T>::MoveNext()
{
    if((m_currentNode== NULL && m_nodeQueue.Count() == 0 && m_nodeStack.Count() == 0) || m_count<=0)
        return false;
    TreeNode<T>* top = NULL;
    switch(m_currentTraversalType)
    {
        //前序遍历 根、左、右
        case TraversalType::Preorder :
            if(m_nodeStack.Count() != 0)
                m_currentNode = m_nodeStack.Pop();
            this->SetCurrent(m_currentNode->data);

            for (int i = m_currentNode->capacity - 1; i >= 0; i--)
            {
                if(m_currentNode->GetNode(i))
                    m_nodeStack.Push(m_currentNode->GetNode(i));
            }
            if(m_nodeStack.Count() == 0)
                m_currentNode = NULL;
            break;
        //中序遍历 左、根、右
        case TraversalType::Inorder :
            while (m_currentNode)
            {
                if(m_currentNode)
                    m_nodeStack.Push(m_currentNode);
                m_currentNode = m_currentNode->GetNode(0);
            }
            if(!m_currentNode)
                m_currentNode = m_nodeStack.Pop();
            this->SetCurrent(m_currentNode->data);
            m_currentNode = m_currentNode->GetNode(1);
            break;
        //后序遍历 左、右、根
        case TraversalType::Postorde:
            while (m_nodeStack.Count() > 0)
            {
                top = m_nodeStack.Peek();
                //判断下一个节点是否访问过
                for (int i = 0; i < top->capacity; i++)
                {
                    if(m_currentNode == top->GetNode(i))
                    {
                        this->SetCurrent(top->data);
                        m_currentNode = m_nodeStack.Pop();
                        if(m_nodeStack.Count() == 0)
                            m_currentNode = NULL;
                        return true;
                    }
                }
                m_currentNode = top;
                if (m_currentNode->childCount == 0 )
                {
                    this->SetCurrent(m_currentNode->data);
                    m_nodeStack.Pop();
                    if(m_nodeStack.Count() == 0)
                        m_currentNode = NULL;
                    return true;
                }
                else
                {
                    for (int i = m_currentNode->capacity - 1; i >=0; i--)
                    {
                        if(m_currentNode->GetNode(i))
                            m_nodeStack.Push(m_currentNode->GetNode(i));
                    }
                }
            }
            break;
        //层序遍历
        default:
            if(m_nodeQueue.Count() != 0)
                m_currentNode = m_nodeQueue.Dequeue();
            this->SetCurrent(m_currentNode->data);
            for (int i = 0; i < m_currentNode->capacity; i++)
            {
                if(m_currentNode->GetNode(i))
                    m_nodeQueue.Enqueue(m_currentNode->GetNode(i));
            }
            if(m_nodeQueue.Count() == 0)
                m_currentNode = NULL;
            break;
    }
    return true;
}

//枚举器重置
template <class T>
void Tree<T>::Reset()
{
    m_currentNode = NULL;
    m_nodeStack.Clear();
    m_nodeQueue.Clear();
    if(m_root!=NULL)
    {
        m_currentNode = m_root;
        switch (m_currentTraversalType)
        {
            //前序遍历
            case TraversalType::Preorder :
                this->SetCurrent(m_currentNode->data);
                for (int i = m_currentNode->capacity - 1; i >= 0; i--)
                {
                    if(m_currentNode->GetNode(i))
                        m_nodeStack.Push(m_currentNode->GetNode(i));
                }
                if(m_nodeStack.Count() == 0)
                    m_currentNode = NULL;
                break;
            //中序遍历
            case TraversalType::Inorder :
                while (m_currentNode)
                {
                    m_nodeStack.Push(m_currentNode);
                    m_currentNode = m_currentNode->GetNode(0);
                }
                if(!m_currentNode)
                    m_currentNode = m_nodeStack.Pop();
                this->SetCurrent(m_currentNode->data);
                m_currentNode = m_currentNode->GetNode(1);
                break;
            //后序遍历
            case TraversalType::Postorde :
                m_nodeStack.Push(m_root);
                while (m_nodeStack.Count() > 0)
                {
                    m_currentNode = m_nodeStack.Peek();
                    if (m_currentNode->childCount == 0)
                    {
                        this->SetCurrent(m_currentNode->data);
                        m_nodeStack.Pop();
                        if(m_nodeStack.Count() == 0)
                            m_currentNode = NULL;
                        return;
                    }
                    else
                    {
                        for (int i = m_currentNode->capacity - 1; i >=0; i--)
                        {
                            if(m_currentNode->GetNode(i))
                                m_nodeStack.Push(m_currentNode->GetNode(i));
                        }
                    }
                }
                break;
            //层序遍历
            default:
                this->SetCurrent(m_currentNode->data);
                for (int i = 0; i < m_currentNode->capacity; i++)
                {
                    if(m_currentNode->GetNode(i))
                        m_nodeQueue.Enqueue(m_currentNode->GetNode(i));
                }
                if(m_nodeQueue.Count() == 0)
                    m_currentNode = NULL;
                break;
        }
    }
}
#pragma endregion