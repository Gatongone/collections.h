#include "collections.h"

//构造函数
template <class T>
Stack<T>::Stack() 
{
    m_count = 0;
    m_top = m_currentNode = NULL;
}

//析构
template <class T>
Stack<T>::~Stack() 
{
    Node *temp;
    if(m_top!=NULL)
    {
        while(m_top->next != NULL)
        {
            temp = m_top->next;
            delete[] m_top;
            m_top = temp;
        }
        delete[] m_top;
    }
}

template<class T>
inline unsigned long Stack<T>::Count()
{
    return m_count;
}

//出栈
template<class T>
T& Stack<T>::Pop()
{
    if(m_count == 0)
        error("Empty stack !");
    else if(m_top == NULL)
        error("Queue front got a null pointer !");
    else
    {
        m_count--;
        T& data = m_top->data;
        m_top = m_top->next;
        return data;
    }

}

//入栈
template<class T>
void Stack<T>::Push(const T& item)
{
    Node *node = new Node(item);
    m_count++;
    if(m_count == 1)
    {
        m_top = node;
        return;
    }
    node->next = m_top;
    m_top = node;
}

//返回栈顶元素
template<class T>
T& Stack<T>::Peek()
{
    if(m_count == 0)
        error("Empty stack !");
    else if(m_top == NULL)
        error("Stack top got a null pointer !");
    else
    {
        return m_top->data;
    }
    
}

//清空栈
template<class T>
void Stack<T>::Clear()
{
    Node *temp;
    if(m_top!=NULL)
    {
        while(m_top->next != NULL)
        {
            temp = m_top->next;
            delete[] m_top;
            m_top = temp;
        }
    }
    m_top = NULL;
    m_count = 0;
}

#pragma region 枚举器实现

template <class T>
bool Stack<T>::MoveNext()
{
    if(m_top == NULL)
        return false;
    if(m_currentNode->next == NULL)
        return false;
    m_currentNode = m_currentNode->next;
    this->SetCurrent(m_currentNode->data);
    return true;
}
template <class T>
void Stack<T>::Reset()
{
    if(m_top != NULL)
    {
        m_currentNode = m_top;
        this->SetCurrent(m_currentNode->data);
    }
    else
    {
        m_currentNode = NULL;
    }
}
#pragma endregion