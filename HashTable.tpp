#include "collections.h"
#include <cmath>
#include<string>
using namespace std;

//无参构造
template<class K,class V>
HashTable<K,V>::HashTable()
{
    m_index = 0;
    m_capacity = 0;
    m_count = 0;
    m_container = new KeyValuePair<K, V>*[0]();
    m_currentNode = NULL;
}

//有参构造
template<class K,class V>
HashTable<K,V>::HashTable(unsigned long capacity)
{
    m_index = 0;
    m_count = 0;
    m_capacity = capacity;
    m_container = new KeyValuePair<K, V>*[capacity]();
    m_currentNode = NULL;
};

//析构
template<class K,class V>
HashTable<K,V>::~HashTable()
{
    if (m_container != NULL)
    {
        KeyValuePair<K, V>* before = NULL;
        for (unsigned int i = 0; i < m_capacity;i++)
        {
            while (m_container[i])
            {
                if(before)
                {
                    delete[] before;
                }
                before = m_container[i];
                m_container[i] = m_container[i]->next;
            }
            before = NULL;
        }
        delete[] m_container;
    }
    m_count = 0;
}

//获得小于等于num的最大质数
template<class K,class V>
unsigned long HashTable<K,V>::GetMaxPrime(unsigned long num)
{
    if(IsPrime(num))
        return num;
    while(num--)
    {
        if(IsPrime(num))
            break;
    }
    return num;
}

//是否是质数
template<class K,class V>
bool HashTable<K,V>::IsPrime(unsigned long num){
	
	if(num==3||num==5||num==2)
		return true;
	if(num%2==0 || num%3==0 || num%5==0)//排除掉2,3,5的倍数 
		return false;
	unsigned long c = 7;
	unsigned long Cmax = (unsigned long) sqrt(num);
    //排除2,3,5的倍数直接排除
	while(c<Cmax)
	{
		if(num%c==0)
		return false;
		c+=4;//11 
		if(num%c==0)
		return false;
		c+=2;// 13 
		if(num%c==0)
		return false;
		c+=4;//17
		if(num%c==0)
		return false;
		c+=2;//19
		if(num%c==0)
		return false;
		c+=4;//23
		if(num%c==0)
		return false;
		c+=6;//29
		if(num%c==0)
		return false;
		c+=2;//31 
		if(num%c==0)
		return false;
		c+=6;//37
	} 
	return true;
	
}

//重载下标运算，通过键获取值
template<class K,class V>
V& HashTable<K,V>::operator[](const K& key)
{
    unsigned long hashCode = GetHashCode(key);
    if(m_container[hashCode])
    {
        KeyValuePair<K, V> *p_node = m_container[hashCode];
        if(p_node->key == key)
            return p_node->value;
        while (p_node->next != NULL)
        {
            if(p_node->key == key)
                return p_node->value;
            p_node = p_node->next;
        }
    }
    error("Key does't exist !");
}

//哈希表表长
template<class K,class V>
inline unsigned long HashTable<K,V>::Count()
{
    return m_count;
}

//生成哈希值
template<class K,class V>
inline unsigned long HashTable<K,V>::GetHashCode(const K& key)
{
    unsigned long hashCode = std::hash<K>()(key);
    hashCode %= m_capacity;
    return hashCode;
}

//自动扩容
template<class K,class V>
void HashTable<K,V>::AutoExpand()
{
    if(m_count == 1)
    {
        m_capacity = s_primes[0];
        m_container = new KeyValuePair<K, V>*[m_capacity]();
    }
    //如果超过质数数组上限
    else if(m_count > m_capacity)
    {
        unsigned long originCapacity = m_capacity;

        if(m_primesIndex > c_primesSize)
        {
            m_capacity = GetMaxPrime(m_capacity * 2);
        }
        else
        {
            m_capacity = s_primes[++m_primesIndex];
        }

        //重建哈希表
        unsigned long newIndex = 0;

        KeyValuePair<K, V>** newList = new KeyValuePair<K, V>*[m_capacity]();
        KeyValuePair<K, V>* p_temp;
        //转移所有结点
        for (unsigned long i = 0; i < originCapacity; i++)
        {
            if(m_container[i])
            {
                //遍历重新计算每一个结点在新表的哈希值
                while(m_container[i])
                {
                    newIndex = GetHashCode(m_container[i]->key);
                    p_temp = m_container[i]->next;
                    if(!newList[newIndex])
                    {
                        newList[newIndex] = m_container[i];
                        newList[newIndex]->next = NULL;
                    }
                    else
                    {
                        m_container[i]->next = newList[newIndex];
                        newList[newIndex] = m_container[i];
                    }
                    m_container[i] = p_temp;
                }
            }
        }
        m_container = newList;
    }
}

//处理哈希冲突
template<class K,class V>
void HashTable<K,V>::DealHashCollision(unsigned long hashCode,KeyValuePair<K, V>* newNode)
{
    KeyValuePair<K, V> *p_node = m_container[hashCode];
    while (p_node->next != NULL)
    {
        if(p_node->key == newNode->key)
            error("Key already exists");
        p_node = p_node->next;
    }
    //头插法插入
    newNode->next = m_container[hashCode];
    m_container[hashCode] = newNode;
}

//添加一对键值
template<class K,class V>
void HashTable<K,V>::Add(const K& key,const V& value)
{
    m_count++;
    AutoExpand();
    unsigned long hashCode = GetHashCode(key);
    KeyValuePair<K, V>* kv = new KeyValuePair<K,V>(key , value);
    //空槽
    if(m_container[hashCode] == NULL)
        m_container[hashCode] = kv;
    //否则发生哈希冲突
    else
        DealHashCollision(hashCode, kv);
}

//移除一对键值
template<class K,class V>
bool HashTable<K,V>::Remove(const K& key)
{
    KeyValuePair<K, V> *current;
    KeyValuePair<K, V> *before = NULL;
    unsigned long hashCode = GetHashCode(key);

    if(m_container[hashCode])
    {
        current = m_container[hashCode];
        if(current->key == key)
        {
            m_container[hashCode] = current->next;
            m_count--;
            return true;
        }
        current = current->next;
        while (current)
        {                
            if(current->key == key)
            {
                before->next = current->next;
                m_count--;
                return true;
            }
            before = current;
            current = current->next;
        }
    }
    
    return false;
}

//是否存在key
template<class K,class V>
bool HashTable<K,V>::ContainsKey(const K& key)
{
    KeyValuePair<K, V> *p_node = m_container[GetHashCode(key)];
    while (p_node)
    {
        if(p_node->key == key)
            return true;
        p_node = p_node->next;
    }   
    return false;
}

//是否存在value
template<class K,class V>
bool HashTable<K,V>::ContainsValue(const V& value)
{
    KeyValuePair<K, V> *p_node;
    for (unsigned long i = 0; i < m_capacity; i++)
    {
        p_node = m_container[i];
        while (p_node)
        {
            if(p_node->value == value)
                return true;
            p_node = p_node->next;
        }
    }
       
    return false;
}

//如果键存在则将值赋予outValue
template<class K,class V>
bool HashTable<K,V>::TryGetValue(const K& key,V& outValue)
{
    KeyValuePair<K, V> *p_node = m_container[GetHashCode(key)];
    while (p_node)
    {
        if(p_node->key == key)
        {
            outValue = p_node->value;
            return true;
        }
        p_node = p_node->next;
    }   
    return false;
}

//清空哈希表
template<class K,class V>
void HashTable<K,V>::Clear()
{
    for (unsigned long i = 0; i < m_capacity; i++)
    {
        m_container[i] = NULL;
    }
    m_count = 0;
}

#pragma region 枚举器实现

template<class K,class V>
bool HashTable<K,V>::MoveNext()
{
    if(m_container == NULL)
        return false;
    if(m_currentNode!=NULL)
    {
        if(m_currentNode->next == NULL)
        {
            m_currentNode == NULL;
        }
        else
        {
            m_currentNode = m_currentNode->next;
            this->SetCurrent(*m_currentNode);
            return true;
        }
    }

    m_index++;
    if(m_index > m_capacity-1)
        return false;
    while (!m_container[m_index])
    {
        if(m_index > m_count-1)
            return false;
        m_index++;
    }

    m_currentNode = m_container[m_index];
    this->SetCurrent(*m_currentNode);
    return true;
}

template<class K,class V>
void HashTable<K,V>::Reset()
{
    m_index = 0;
    if(m_count>0)
    {
        m_currentNode = NULL;
        while (!m_container[m_index])
        {
            m_index++;
        }
        this->SetCurrent(*m_container[m_index]);
        return;
    }
}
#pragma endregion