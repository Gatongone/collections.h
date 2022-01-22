#include "collections.h"

template <class NodeType,class EdgeType>
AdjacencyMatrix<NodeType,EdgeType>::AdjacencyMatrix(bool isOriented)
{
    this->m_isOriented = isOriented;
    this->m_edgeCount = this->m_edgeCount = m_nodeCapacity = 0;
}

template <class NodeType,class EdgeType>
AdjacencyMatrix<NodeType,EdgeType>::~AdjacencyMatrix()
{
    for (unsigned long i = 0; i < m_nodeCapacity; i++)
    {
        // for (unsigned long j = 0; j < m_nodeCapacity; j++)
        // {
        //     if(m_matrix[i][j])
        //     {
        //         delete[] m_matrix[i][j];
        //         m_matrix[i][j] = NULL;
        //     }
        // }
        delete[] m_matrix[i];
    }
    delete[] m_matrix;
}

//清空所有点和边
template <class NodeType,class EdgeType>
void AdjacencyMatrix<NodeType,EdgeType>::Clear()
{
    this->m_nodeCount = this->m_edgeCount = m_nodeCapacity = 0;
    m_matrix = new EdgeType**[m_nodeCapacity];
    m_indexMapping.Clear();
    m_nodeList.Clear();
}

//结点添加一个结点
template <class NodeType,class EdgeType>
void AdjacencyMatrix<NodeType,EdgeType>::AddNode(const NodeType& node)
{
    if(m_indexMapping.Count()>0 && m_indexMapping.ContainsKey(node))
    {
        error("Node already existed ! ");
    }
    
    this->m_nodeCount++;

    if(this->m_nodeCount > m_nodeCapacity)
    {
        unsigned long originedCapacity = m_nodeCapacity;
        m_nodeCapacity = 2 * this->m_nodeCount;
        EdgeType*** newMatrix = new EdgeType**[m_nodeCapacity];

        for (unsigned long i = 0; i < m_nodeCapacity;i++)
        {
            newMatrix[i] = new EdgeType*[m_nodeCapacity];
            for (unsigned long j = 0; j < m_nodeCapacity; j++)
            {
                newMatrix[i][j] = NULL;
            }
        }
        for (unsigned long i = 0; i < originedCapacity;i++)
        {
            for (unsigned long j = 0; j < originedCapacity;j++)
            {
                if(m_matrix[i][j])
                    newMatrix[i][j] = m_matrix[i][j];
            }
        }

        m_matrix = newMatrix;
    }

    m_nodeList.Add(node);
    m_indexMapping.Add(node, m_nodeList.Count() - 1);
}

//在两结点中添加一条边
template <class NodeType,class EdgeType>                             
void AdjacencyMatrix<NodeType,EdgeType>::AddEdge(const NodeType& begin, const NodeType& end,EdgeType&& weight)
{
    this->m_edgeCount++;
    unsigned long x = m_indexMapping[begin];
    unsigned long y = m_indexMapping[end];
    if(m_matrix[x][y])
    {
        error("Edge already existed ! ");
    }
    m_matrix[x][y] = &weight;
    if(!this->m_isOriented)
    {
        if(m_matrix[y][x])
        {
            error("Edge already existed ! ");
        }
        m_matrix[y][x] = &weight;
    }
}

//在两结点中添加一条边
template <class NodeType,class EdgeType>                             
void AdjacencyMatrix<NodeType,EdgeType>::AddEdge(const NodeType& begin, const NodeType& end,EdgeType& weight)
{
    this->m_edgeCount++;
    unsigned long x = m_indexMapping[begin];
    unsigned long y = m_indexMapping[begin];
    if(m_matrix[x][y])
    {
        error("Edge already existed ! ");
    }
    m_matrix[x][y] = &weight;
    if(!this->m_isOriented)
    {
        if(m_matrix[y][x])
        {
            error("Edge already existed ! ");
        }
        m_matrix[y][x] = &weight;
    }
}

//删除一个结点
template <class NodeType,class EdgeType>
bool AdjacencyMatrix<NodeType,EdgeType>::RemoveNode(const NodeType& node)
{
    if(!m_nodeList.RemoveAt(m_indexMapping[node]))
    {
        return false;
    }
    for (unsigned long i = 0; i < m_nodeCapacity; i++)
    {
        if(m_matrix[m_indexMapping[node]][i])
        {
            m_matrix[m_indexMapping[node]][i] = NULL;
            this->m_edgeCount--;
        }
        if(m_matrix[m_indexMapping[i]][node])
        {
            m_matrix[m_indexMapping[i]][node] = NULL;
            this->m_edgeCount--;
        }
    } 
    this->m_nodeCount--;
    return m_indexMapping.Remove(node);
}

//删除一条边
template <class NodeType,class EdgeType>                                   
bool AdjacencyMatrix<NodeType,EdgeType>::RemoveEdge(const NodeType& begin, const NodeType& end)
{
    this->m_edgeCount--;
    if(m_matrix[m_indexMapping[begin]][m_indexMapping[end]])
        m_matrix[m_indexMapping[begin]][m_indexMapping[end]] = NULL;
    else
        return false;

    if(!this->m_isOriented)
    {
        if(m_matrix[m_indexMapping[end]][m_indexMapping[begin]])
            m_matrix[m_indexMapping[end]][m_indexMapping[begin]] = NULL;
        else
            return false;
    }
    return true;
}

//获得边权
template <class NodeType,class EdgeType>                                     
EdgeType& AdjacencyMatrix<NodeType,EdgeType>::GetEdgeweight(const NodeType& begin, const NodeType& end)
{
    if(!m_indexMapping.ContainsKey(begin)||!m_indexMapping.ContainsKey(end))
        error("Edge dosen't exist !");

    unsigned long beginIndex = m_indexMapping[begin];
    unsigned long endIndex = m_indexMapping[end];

    EdgeType* result = m_matrix[beginIndex][endIndex];
    if(result)
        return *result;
    else
        error("Edge dosen't exist !");
}

//设置边权
template <class NodeType,class EdgeType>                                     
void AdjacencyMatrix<NodeType,EdgeType>::SetEdgeweight(const NodeType& begin, const NodeType& end,EdgeType&& weight)
{
    if(!m_indexMapping.ContainsKey(begin)||!m_indexMapping.ContainsKey(end))
        error("Edge dosen't exist !");

    unsigned long beginIndex = m_indexMapping[begin];
    unsigned long endIndex = m_indexMapping[end];

    if(m_matrix[beginIndex][endIndex])
        m_matrix[beginIndex][endIndex] = &weight;
    else
        error("Edge dosen't exist !");
}

//设置边权
template <class NodeType,class EdgeType>                                     
void AdjacencyMatrix<NodeType,EdgeType>::SetEdgeweight(const NodeType& begin, const NodeType& end,EdgeType& weight)
{
    if(m_matrix[m_indexMapping[begin]][m_indexMapping[end]])
        return m_matrix[m_indexMapping[begin]][m_indexMapping[end]] = &weight;
    else
        error("Edge dosen't exist !");
}


#pragma region 枚举器实现

template <class NodeType,class EdgeType>                                     
bool AdjacencyMatrix<NodeType,EdgeType>::MoveNext()
{
    if(m_nodeList.Count() == 0 || m_currentIndex == m_nodeList.Count()-1)
        return false;
    this->SetCurrent(m_nodeList[++m_currentIndex]);
    return true;
}

template <class NodeType,class EdgeType>                                     
void AdjacencyMatrix<NodeType,EdgeType>::Reset()
{
    m_currentIndex = 0;
    if(m_nodeList.Count() != 0 )
        this->SetCurrent(m_nodeList[m_currentIndex]);
}
#pragma endregion