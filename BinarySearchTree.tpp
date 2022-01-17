#include "collections.h"

template <class T>
BinarySearchTree<T>::BinarySearchTree()
{
    this->m_currentNode = NULL;
    this->m_currentTraversalType = Tree<T>::TraversalType::Inorder;
    this->m_count = 0;
}

//插入
template <class T>
void BinarySearchTree<T>::Add(const T& item)
{
    TreeNode<T>* node = new TreeNode<T>(this, item);
    if(this->m_count == 0)
    {
        this->m_root = node;
        this->m_count = 1;
    }
    else
    {
        TreeNode<T>* p_preNode = NULL;
        TreeNode<T>* p_node = this->m_root;
        while (p_node)
        {
            p_preNode = p_node;
            if(item < p_node->data)
            {
                p_node = p_node->GetNode(0);
            }
            else
            {
                p_node = p_node->GetNode(1);
            }
        }
        if(p_preNode)
        {
            if(item < p_preNode->data)
            {
                p_preNode->SetNode(0,node);
            }
            else
            {
                p_preNode->SetNode(1,node);
            }
        }

    }
}

//删除
template <class T>
bool BinarySearchTree<T>::Remove(const T& item)
{
    TreeNode<T>* p_preNode = this->m_root;
    TreeNode<T>* p_node = this->m_root;
    int leftOrRight = 0;
    while (p_node)
    {
        if(p_node->data == item)
        {
            break;
        }
        p_preNode = p_node;
        if(item < p_node->data)
        {
            p_node = p_node->GetNode(0);
            leftOrRight = 0;
        }
        else
        {
            p_node = p_node->GetNode(1);
            leftOrRight = 1;
        }
    }
    if(!p_node)
    {
        return false;
    }

    //1. 没有孩子时，直接删除
    if(p_node->childCount == 0)
    {
        if(p_preNode)
        {
            p_preNode->SetNode(leftOrRight,NULL);
        }
        else
        {
            this->m_root = NULL;
            this->m_count = 0;
        }
    }
    //2. 只有一个孩子时，和孩子交换指针并
    else if (p_node->childCount == 1)
    {
        if(p_node->GetNode(0))
        {
            TreeNode<T>* temp = p_node->GetNode(0);
            p_node->SetNode(0,NULL);
            *p_node = *(temp);
        }
        else
        {
            TreeNode<T>* temp = p_node->GetNode(1);
            p_node->SetNode(0,NULL);
            *p_node = *(temp);
        }
    }
    //3. 有两个孩子时，从左子树中找到最大叶子和它交换值
    else
    {
        TreeNode<T>* p_max = p_node->GetNode(0);
        TreeNode<T> *p_maxPre = p_node;
        while (p_max)
        {
            if(p_max->GetNode(1))
            {
                p_maxPre = p_max;
                p_max = p_max->GetNode(1);
            }
            else
                break;
        }
        p_preNode->GetNode(leftOrRight)->data = p_max->data;

        if(p_max == p_node->GetNode(0))
            p_maxPre->SetNode(0, NULL);
        else
            p_maxPre->SetNode(1, NULL);
    }
    return true;
}

//是否包含元素
template <class T>
bool BinarySearchTree<T>::Contains(const T &item)
{
    TreeNode<T>* p_preNode = this->m_root;
    TreeNode<T>* p_node = this->m_root;
    int leftOrRight = 0;
    while (p_node)
    {
        if(p_node->data == item)
        {
            return true;
        }
        p_preNode = p_node;
        if(item < p_node->data)
        {
            p_node = p_node->GetNode(0);
            leftOrRight = 0;
        }
        else
        {
            p_node = p_node->GetNode(1);
            leftOrRight = 1;
        }
    }
    return false;
}