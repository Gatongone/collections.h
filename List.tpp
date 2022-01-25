#include "collections.h"
#include <cstddef>
#include<iostream>

using namespace std;

//无参构造
template <class T>
List<T>::List()
{
    m_index = 0;
    m_count = 0;
    m_capacity = 0;
    m_list = NULL;
};

//有参构造,传入的数组必须是满元素的数组
template <class T>
List<T>::List(T* m_listay,unsigned long size)
{
    m_index = 0;
    m_count = m_capacity = size;
    m_list = m_listay;
};

//有参构造
template <class T>
List<T>::List(unsigned long capacity)
{
    m_index = 0;
    m_count = 0;
    m_capacity = capacity;
    m_list = new T[capacity];
};

//析构
template <class T>
List<T>::~List()
{
    if (m_list != NULL)
    {
        delete[] m_list;
        m_list = NULL;
    }
}

//下标重载
template <class T>
T& List<T>::operator[](unsigned long index)
{
    if(index > m_count-1)
        error("Index Out Of Range");
    return m_list[index];
}

//返回当前容量
template <class T>
inline unsigned long List<T>::Count()
{
    return m_count;
}

//自动扩容
template <class T>
inline void List<T>::AutoExpand()
{
    if(m_capacity == 0)
    {
        m_capacity = 4;
        m_list = new T[m_capacity];
    }
    else if (m_count == m_capacity)
    {
        m_capacity *= 2; //扩容两倍

        T *newList = new T[m_capacity];
        memcpy(newList, m_list, m_capacity * sizeof(T));
        m_list = newList;
    }
}

//插入到尾端
template <class T>
inline void List<T>::Add(const T& item)
{
    m_count++;
    AutoExpand();
    m_list[m_count - 1] = item;
}

//插入数组到尾端
template <class T>
inline void List<T>::Add(const T targets[],unsigned long size)
{
    for (unsigned long i= 0; i < size;i++)
    {
        m_count++;
        AutoExpand();
        m_list[m_count - 1] = targets[i];
    }
}

//插入列表到尾端
template <class T>
inline void List<T>::Add(List<T>& targets)
{
    enumerate(targets)
    {
        m_count++;
        AutoExpand();
        m_list[m_count - 1] = targets.GetCurrentValue();
    };
}

//插入到指定位置
template <class T>
inline void List<T>::Insert(unsigned long index,const T& item)
{
    m_count++;
    AutoExpand();
    //从指定位置之后的全部右移
    for (unsigned long i = m_count - 1; i > index; i--)
        m_list[i] = m_list[i - 1];
    m_list[index] = item;
}

//是否包含指定元素
template <class T>
bool List<T>::Contains(const T& item)
{
    for (unsigned long i = 0; i < m_count; i++)
    {
        if (m_list[i] == item)
        {
            return true;
        }
    }
    return false;
}

//删除指定元素
template <class T>
inline bool List<T>::Remove(const T& item)
{
    if (m_count == 0)
        return false;

    for (unsigned long i = 0; i < m_count; i++)
    {
        if (m_list[i] == item)
        {
            //从指定位置之后的全部左移
            for (unsigned long j = i; j < m_count; j--)
            {
                if (i + 1 <= m_count - 1)
                    m_list[i] = m_list[i + 1];
            }
            m_count--;
            return true;
        }
    }
    return false;
}

//删除指定元素
template <class T>
inline bool List<T>::RemoveAt(unsigned long index)
{
    if (index > m_count - 1)
        return false;

    //从指定位置之后的全部左移
    for (unsigned long j = index; j < m_count; j--)
    {
        if (index + 1 <= m_count - 1)
            m_list[index] = m_list[index + 1];
    }
    m_count--;
    return true;
}

//清空列表
template <class T>
void List<T>::Clear()
{
    m_count = 0;
}

//希尔排序
template <class T>
void List<T>::ShellSort(bool isReverse)
{
    long long i, j, interval;
    T current;

    //分组减半
    for (interval = m_count / 2; interval != 0; interval /= 2)
    {
        //做插排
        for (i = interval; i < m_count; i += interval)
        {
            current = m_list[i];

            for (j = i - interval; j >=0 ; j -= interval)
            {
                if(isReverse)
                {
                    if (m_list[j] > current)
                    {
                        break;
                    }
                }
                else
                {
                    if (m_list[j] < current)
                    {
                        break;
                    }
                }

                m_list[j + interval] = m_list[j];
            }
            m_list[j + interval] = current;
        }
    }
}

//局部排序
template <class T>
unsigned long List<T>::PartitionSort(unsigned long left, unsigned long right)
{
    T pivot = m_list[left];
    while(left < right)
    {
        while(left < right && m_list[right] >= pivot)
        {
            right--;
        }
        m_list[left] = m_list[right];
        while(left < right && m_list[left] < pivot)
        {
            left++;
        }
        m_list[right] = m_list[left];
    }
    m_list[left] = pivot;
    return left;
}

//快速排序
template <class T>
void List<T>::QuickSort(bool isReverse)
{
    Stack<unsigned long> stack;
    //第一次分割
    unsigned long pvoitIndex = PartitionSort(0, m_count - 1);
    unsigned long left = 0;
    unsigned long right = m_count - 1;
    
    //确保小的在左端，大的在右端
    if(left < pvoitIndex - 1)
    {
        stack.Push(left);
        stack.Push(pvoitIndex - 1);
    }
    if(pvoitIndex + 1 < right)
    {
        stack.Push(pvoitIndex + 1);
        stack.Push(right);
    }
    
    while(stack.Count() > 0)
    {
        //之前是大的最后入栈，所以先取右端
        right = stack.Pop();
        left = stack.Pop();
        pvoitIndex = PartitionSort(left, right);

        if(left < pvoitIndex-1)
        {
            stack.Push(left);
            stack.Push(pvoitIndex - 1);
        }
        if(pvoitIndex + 1 < right)
        {
            stack.Push(pvoitIndex + 1);
            stack.Push(right);
        }
    }
}

//对数组内元素排序，默认为从小到大排序
template <class T>
void List<T>::Sort(bool isReverse)
{
    if(m_count > 100)
        QuickSort(isReverse);
    else
        ShellSort(isReverse);
}            

//返回并移除指定位置元素
template <class T>
T& List<T>::GetAndRemove(unsigned long index)
{
    if(index > m_count-1)
        error("Index Out Of Range");

    T result = m_list[index];
    RemoveAt(index);
    return result;
}

//转换成数组
template <class T>
T* List<T>::ToArray()
{
    T* newList = new T[m_count];
    memcpy(newList, m_list, m_count * sizeof(T));
    return newList;
}

#pragma region 枚举器实现

template <class T>
bool List<T>::MoveNext()
{
    m_index++;
    if(m_index > m_count - 1)
        return false;
    
    this->SetCurrent(m_list[m_index]);
    return true;
}

template <class T>
void List<T>::Reset()
{
    m_index = 0;
    if(m_count>0)
    {
        this->SetCurrent(m_list[0]);
    }
}
#pragma endregion