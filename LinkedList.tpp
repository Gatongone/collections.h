#include "collections.h"

//构造函数
template<class T>
LinkedList<T>::LinkedList()
{
    m_currentNode = NULL;
    m_head = NULL;
    m_tail = NULL;
    m_count = 0;
}

//析构
template<class T>
LinkedList<T>::~LinkedList()
{
    if(m_head == NULL)
        return;
    Node* currentNode = m_head;
    while (m_head->next != NULL)
    {
        currentNode = m_head->next;
        delete[] m_head;
        m_head = currentNode;
    }
    if(m_tail != NULL)
    {
        delete[] m_tail;
    }
    m_head = m_tail = NULL;

    m_count = 0;
}

//下标重载
template<class T>
T& LinkedList<T>::operator[](unsigned long index)
{
    return GetNode(index)->data;
}

//获得指定位置结点
template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned long index)
{
    if(index > m_count-1 || index < 0)
    {
        error("Error index");
    }
    Node *currentNode = m_head;
    for (unsigned long i = 0; i < m_count;i++)
    {
        if(i == index)
        {
            return currentNode;
        }
        if(currentNode->next != NULL)
            currentNode = currentNode->next;
    }
    error("GetNode failed");
}

//返回当前容量
template<class T>
inline unsigned long LinkedList<T>::Count()
{
    return this->m_count;
}

//将元素插入尾部
template<class T>
typename LinkedList<T>::Node* LinkedList<T>::AddLast(const T& item)
{
    Node* node = new Node(item);
    if(m_count == 0)
    {
        m_head = m_tail = node;
    }
    else
    {
        m_tail->next = node;
        node->prev = m_tail;
        node->next = NULL;
        m_tail = node;
    }
    m_count++;
    return node;
}

//将元素插入头部
template<class T>
typename LinkedList<T>::Node* LinkedList<T>::AddFirst(const T& item)
{
    Node* node = new Node(item);
    if(m_count == 0)
    {
        m_head = m_tail = node;
    }
    else
    {
        node->next = m_head;
        m_head->prev = node;
        m_head = node;
    }
    m_count++;
    return node;
}

//插入到某元素前面
template<class T>
typename LinkedList<T>::Node* LinkedList<T>::InsertFront(unsigned long index, const T& item)
{
    if(index > m_count-1 || index < 0)
    {
        error("Error index");
        return NULL;
    }
    if(index == 0)
    {
        return AddFirst(item);
    }

    Node* node = new Node(item);
    Node* targetNode = GetNode(index);
    node->next = targetNode;
    node->prev = targetNode->prev;
    targetNode->prev->next = node;
    targetNode->prev = node;
    m_count++; 
    return node;
    
}

//插入到某元素后面
template<class T>
typename LinkedList<T>::Node* LinkedList<T>::InsertBehind(unsigned long index, const T& item)
{
    if(index > m_count-1 || index < 0)
    {
        error("Error index");
        return NULL;
    }
    if(index == m_count-1)
    {
        return AddLast(item);
    }

    Node* node = new Node(item);
    Node* targetNode = GetNode(index);
    node->prev = targetNode;
    node->next = targetNode->next;
    targetNode->next->prev = node;
    targetNode->next = node;
    m_count++;
    return node;
}

//移除头部元素
template<class T>
bool LinkedList<T>::RemoveFirst()
{
    if(m_count == 0)
        return false;
    else if(m_count == 1)
    {
        delete[] m_head;
        delete[] m_tail;
        m_head = m_tail = NULL;
    }
    else
    {
        Node* tempNode = m_head->next;
        tempNode->prev = NULL;
        delete[] m_head;
        m_head = tempNode;
    }
    m_count--;
    return true;
}

//移除尾部元素
template<class T>
bool LinkedList<T>::RemoveLast()
{
    if(m_count == 0)
        return false;
    else if(m_count == 1)
    {
        delete[] m_head;
        delete[] m_tail;
        m_head = m_tail = NULL;
    }
    else
    {
        Node* tempNode = m_tail->prev;
        tempNode->next = NULL;
        delete[] m_tail;
        m_tail = tempNode;
    }
    m_count--;
    return true;
}

//移除指定位置元素
template<class T>
bool LinkedList<T>::RemoveAt(unsigned long index)
{
    if(index < 0 || index > m_count)
        return false;
    
    if(index == 0)
        return RemoveFirst();
    else if(index == m_count - 1)
        return RemoveLast();

    Node *targetNode = GetNode(index);
    targetNode->prev->next = targetNode->next;
    targetNode->next->prev = targetNode->prev;
    delete[] targetNode;
    targetNode = NULL;

    return true;
}

//移除元素
template<class T>
bool LinkedList<T>::Remove(const T& item)
{
    if(m_count == 0)
        return false;
    Node *p_node = m_head;
    while (p_node)
    {
        if(p_node->data == item)
        {
            if(p_node->prev == NULL)
            {
                RemoveFirst();
            }
            else if(p_node->next == NULL)
            {
                RemoveLast();
            }
            else
            {
                p_node->prev->next = p_node->next;
                p_node->next->prev = p_node->prev;
                delete[] p_node;
                p_node = NULL;
            }
            return true;
        }
        p_node = p_node->next;
    }
    

}

//连接链表
template<class T>
void LinkedList<T>::Link(const LinkedList<T>& target)
{
    if(m_tail == NULL)
        *this = target;
    if(target.m_head == NULL)
        return;

    m_tail->next = target.m_head;
    target.m_head->prev = m_tail;
    m_tail = target.m_tail;
    m_count += target.m_count;
}

//清空链表
template<class T>
void LinkedList<T>::Clear()
{
    if(m_head == NULL)
        return;
    Node* currentNode = m_head;
    while (m_head->next != NULL)
    {
        currentNode = m_head->next;
        delete[] m_head;
        m_head = currentNode;
    }
    if(m_tail != NULL)
    {
        delete[] m_tail;
    }
    m_count = 0;
    m_head = m_tail = NULL;
}

#pragma region 枚举器实现
template <class T>
bool LinkedList<T>::MoveNext()
{
    if(m_head == NULL)
        return false;
    if(m_currentNode->next == NULL)
    {
        return false;
    }

    m_currentNode = m_currentNode->next;
    this->SetCurrent(m_currentNode->data);
    return true;
}
template <class T>
void LinkedList<T>::Reset()
{
    if(m_head != NULL)
    {
        m_currentNode = m_head;
        this->SetCurrent(m_currentNode->data);
    }
    else
    {
        m_currentNode = NULL;
    }
        
}
#pragma endregion