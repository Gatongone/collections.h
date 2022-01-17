#include "collections.h"

//构造函数
template <class T>
Queue<T>::Queue()
{
    m_count = 0;
    m_head = m_tail = m_currentNode = NULL;
}

//析构
template <class T>
Queue<T>::~Queue()
{
    Node *temp;
    if(m_head!=NULL)
    {
        while(m_head->next != NULL)
        {
            temp = m_head->next;
            delete[] m_head;
            m_head = temp;
        }
        delete[] m_head;
    }
}

template <class T>
inline unsigned long Queue<T>::Count()
{
    return m_count;
}

//出队列
template <class T>
T& Queue<T>::Dequeue()
{
    if(m_count == 0)
        error("Empty queue !"); 
    else if(m_head == NULL)
        error("Queue front got a null pointer !");
    else
    {
        m_count--;
        T& data = m_head->data;
        Node *temp = m_head->next;
        m_head = temp;
        return data;
    }
}

//入队列
template <class T>
void Queue<T>::Enqueue(const T& item)
{
    Node *node = new Node(item);
    m_count++;
    if(m_count == 1)
    {
        m_head = m_tail = node;
        return;
    }
    m_tail->next = node;
    m_tail = node;
}

//返回队头
template <class T>
T& Queue<T>::Peek()
{
    if(m_count == 0)
        error("Empty stack !");
    else if(m_head == NULL)
        error("Stack top got a null pointer !");
    else
    {
        return m_head->data;
    }
}

//清空队列
template <class T>
void Queue<T>::Clear()
{
    Node *temp;
    if(m_head!=NULL)
    {
        while(m_head->next != NULL)
        {
            temp = m_head->next;
            delete[] m_head;
            m_head = temp;
        }
    }
    m_head = m_tail = NULL;
    m_count = 0;
}

#pragma region 枚举器实现

template <class T>
bool Queue<T>::MoveNext()
{
    if(m_head == NULL)
        return false;
    if(m_currentNode->next == NULL)
        return false;
    m_currentNode = m_currentNode->next;
    this->SetCurrent(m_currentNode->data);
    return true;
}
template <class T>
void Queue<T>::Reset()
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