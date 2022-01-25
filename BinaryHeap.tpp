#include "collections.h"

#define BINARYHEAP
#define leftchild(index) 2*index+1
#define rightchild(index) 2*index+2
#define parent(index) (unsigned long)((index-1)/2)

//无参构造
template<class T>
BinaryHeap<T>::BinaryHeap()
{
    m_heap = new List<T>();
}

//有参构造
template<class T>
BinaryHeap<T>::BinaryHeap(unsigned long capacity)
{
    m_heap = new List<T>(capacity);
}

//有参构造,传入的数组必须是满元素的数组
template<class T>
BinaryHeap<T>::BinaryHeap(T* array,unsigned long size,bool heapify)
{
    m_heap = new List<T>(array,size);
}

//析构
template<class T>
BinaryHeap<T>::~BinaryHeap()
{
    if(m_heap != NULL)
    {
        delete m_heap;
        m_heap = NULL;
    }
}

//返回堆长度
template<class T>
inline unsigned long BinaryHeap<T>::Count()
{
    return m_heap->Count();
}

//交换
template<class T>
void BinaryHeap<T>::Swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

//向上调整
template<class T>
void BinaryHeap<T>::ShiftUp(unsigned long index)
{
    unsigned long parentIndex;
    while(index > 0)
    {
        parentIndex = parent(index);
        if(Compare((*m_heap)[index],(*m_heap)[parentIndex]))
        {
            Swap((*m_heap)[index], (*m_heap)[parentIndex]);
        }
        else
            break;
        index = parentIndex;
    }
}

//向下调整
template<class T>
void BinaryHeap<T>::ShiftDown(unsigned long index)
{
    unsigned long leftChldIndex;
    unsigned long rightChldIndex;
    unsigned long resultIndex;
    unsigned long count = Count();
    while(index < count)
    {
        leftChldIndex = leftchild(index);
        rightChldIndex = rightchild(index);
        if(rightChldIndex>=count)
        {
            if(leftChldIndex>=count)
                break;
            resultIndex = leftChldIndex;
        }
        else
        {
            if(Compare((*m_heap)[leftChldIndex],(*m_heap)[rightChldIndex]))
                resultIndex = leftChldIndex;
            else
                resultIndex = rightChldIndex;
        }

        if(!Compare((*m_heap)[index],(*m_heap)[resultIndex]))
        {
            Swap((*m_heap)[index], (*m_heap)[resultIndex]);
        }
        else
            break;
        index = resultIndex;
    }
}
 
//返回堆顶结点
template<class T>
T& BinaryHeap<T>::Peek()
{
    return (*m_heap)[0];
}

//堆化
template<class T>
void BinaryHeap<T>::Heapify()
{
    unsigned long index = (Count()-1) / 2;
    while (index>0)
    {
        ShiftDown(index);
        index--;
        if(index == 0)
        {
            ShiftDown(index);
            break;
        }   
    }
}

//清空堆
template<class T>
void BinaryHeap<T>::Clear()
{
    m_heap->Clear();
}

//向堆中添加一个元素，并自动调整堆
template<class T>
void BinaryHeap<T>::Add(const T& item)
{
    m_heap->Add(item);
    ShiftUp(m_heap->Count() - 1);
}

//向堆中添加一个元素，不调整堆
template<class T>
void BinaryHeap<T>::AddWithoutShift(const T &item)
{
    m_heap->Add(item);
}

//移除堆顶结点
template<class T>
bool BinaryHeap<T>::RemoveTop()
{
    unsigned long count = Count();
    if(count > 0)
    {
        (*m_heap)[0] = (*m_heap)[count - 1];
        m_heap->RemoveAt(count - 1);
        ShiftDown(0);
        return true;
    }
    return false;
}

//返回并移除堆顶元素
template<class T>
T& BinaryHeap<T>::ExtractTop()
{
    unsigned long count = Count();
    if(count > 0)
    {
        T top = (*m_heap)[0];
        T& result = top;
        (*m_heap)[0] = (*m_heap)[count - 1];
        m_heap->RemoveAt(count - 1);
        ShiftDown(0);
        return result;
    }
    error("Empty heap !");
}

//堆排序
template<class T>
T* BinaryHeap<T>::Sort()
{
    unsigned int count = Count();
    if(count <= 1)
        return NULL;
    unsigned int index = count - 1;
    unsigned int newIndex = 0;
    T result[count];
    while (index>0)
    {
        index--;
        result[newIndex++] = ExtractTop();
        if(index==0)
        {
            result[newIndex] = ExtractTop();
        }
    }
    m_heap->Clear();
    m_heap->Add(result,count);
    return result;
}

//小顶堆比较
template<class T>
bool MinHeap<T>::Compare(T &left, T &right)
{
    if(left < right)
        return true;
    else
        return false;
}

//大顶堆比较
template<class T>
bool MaxHeap<T>::Compare(T &left, T &right)
{
    if(left > right)
        return true;
    else
        return false;
}

#pragma region 枚举器实现
template <class T>
bool BinaryHeap<T>::MoveNext()
{
    m_index++;
    if(m_index > Count() - 1)
        return false;
    
    this->SetCurrent((*m_heap)[m_index]);
    return true;
}
template <class T>
void BinaryHeap<T>::Reset()
{
    m_index = 0;
    if(Count()>0)
    {
        this->SetCurrent((*m_heap)[0]);
    }
}
#pragma endregion

#undef  BINARYHEAP