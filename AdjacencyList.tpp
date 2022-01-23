#include "collections.h"

template <class NodeType,class EdgeType>
AdjacencyList<NodeType,EdgeType>::AdjacencyList(bool isOriented)
{
    this->m_isOriented = isOriented;
    this->m_edgeCount = this->m_nodeCount = 0;
}

//清空所有点和边
template <class NodeType,class EdgeType>
void AdjacencyList<NodeType,EdgeType>::Clear()
{
    if(this->m_isOriented)
    {
        m_orientedNodes.Clear();
    }
    else
    {
        m_unorientedNodes.Clear();
    }
    m_indexMapping.Clear();
    this->m_edgeCount = this->m_nodeCount = 0;
}

//结点添加一个结点
template <class NodeType,class EdgeType>
void AdjacencyList<NodeType,EdgeType>::AddNode(const NodeType& node)
{
    if(this->m_nodeCount!=0 && m_indexMapping.ContainsKey(node))
    {
        error("Node already existed ! ");
    }
    if(this->m_isOriented)
    {
        OrientedNode* p_node = new OrientedNode(node);
        m_orientedNodes.Add(p_node);
        m_indexMapping.Add(node, m_orientedNodes.Count() - 1);
    }
    else
    {
        UnorientedNode *p_node = new UnorientedNode(node);
        m_unorientedNodes.Add(p_node);
        m_indexMapping.Add(node, m_unorientedNodes.Count() - 1);
    }
    
    this->m_nodeCount++;
}

//在两结点中添加一条边
template <class NodeType,class EdgeType>                             
void AdjacencyList<NodeType,EdgeType>::AddEdge(const NodeType& begin, const NodeType& end,const EdgeType& weight,bool checkRepeatedEdge)
{
    if(!m_indexMapping.ContainsKey(begin)||!m_indexMapping.ContainsKey(end))
        error("Node dosen't exist !");

    unsigned long beginIndex = m_indexMapping[begin];
    unsigned long endIndex = m_indexMapping[end];

    if(this->m_isOriented)
    {
        OrientedEdge* p_edge_out = m_orientedNodes[beginIndex]->out;
        OrientedEdge* newEdge = new OrientedEdge(weight, beginIndex, endIndex);

        if(p_edge_out)
        {
            newEdge->out = p_edge_out;
            if(checkRepeatedEdge)
            {
                while (p_edge_out)
                {
                    if(p_edge_out->beginVex == beginIndex && p_edge_out->endVex == endIndex)
                        error("Edge already existed ! ");
                    p_edge_out = p_edge_out->out;
                }
            }
            
        }
        m_orientedNodes[beginIndex]->out = newEdge;

        //设置终点入边
        OrientedEdge *p_edge_in = m_orientedNodes[endIndex]->in;
        if(p_edge_in)
        {
            newEdge->in = m_orientedNodes[endIndex]->in;
        }
        m_orientedNodes[endIndex]->in = newEdge;

        m_orientedNodes[beginIndex]->outDegree++;
        m_orientedNodes[endIndex]->inDegree++; 
    }
    else
    {
        UnorientedEdge* newEdge = new UnorientedEdge(weight,beginIndex,endIndex);
        UnorientedEdge *p_edge_begin = m_unorientedNodes[beginIndex]->firstEdge;
        UnorientedEdge *p_edge_end = m_unorientedNodes[endIndex]->firstEdge;
        if(p_edge_begin)
        {
            newEdge->beginLink = p_edge_begin;
            if(checkRepeatedEdge)
            {
                while (p_edge_begin)
                {
                    if((p_edge_begin->beginVex == beginIndex && p_edge_begin->endVex == endIndex)
                     ||(p_edge_begin->endVex == beginIndex && p_edge_begin->beginVex == endIndex))
                        error("Edge already existed ! ");
                    p_edge_begin = p_edge_begin->beginLink;
                }
            }
        }
        m_unorientedNodes[beginIndex]->firstEdge = newEdge;
        
        if(p_edge_end)
        {
            newEdge->endLink = p_edge_end;
        }
         m_unorientedNodes[endIndex]->firstEdge = newEdge;

        
        m_unorientedNodes[beginIndex]->degree++;
        m_unorientedNodes[endIndex]->degree++;
    }
    this->m_edgeCount++;
}

//删除一个结点
template <class NodeType,class EdgeType>
bool AdjacencyList<NodeType,EdgeType>::RemoveNode(const NodeType& node)
{
    if(!m_indexMapping.ContainsKey(node))
        return false;
    unsigned long beginIndex = m_indexMapping[node];

    if(this->m_isOriented)
    {
        OrientedEdge* p_edge_out = m_orientedNodes[beginIndex]->out;
        OrientedEdge* p_edge_in = NULL;
        while (p_edge_out)
        {
            p_edge_in = m_orientedNodes[p_edge_out->endVex]->in;
            //删除入边
            if(p_edge_in->beginVex == beginIndex)
            {
                m_orientedNodes[p_edge_out->endVex]->in = p_edge_in->in;
                this->m_edgeCount--;
                continue;
            }
            while (p_edge_in)
            {
                if(p_edge_in->in)
                {
                    if (p_edge_in->in->beginVex == beginIndex)
                    {

                        p_edge_in->in = p_edge_in->in->in;
                        this->m_edgeCount--;
                        break;
                    }
                }
                p_edge_in = p_edge_in->in;
            }
            this->m_edgeCount--;
            p_edge_out = p_edge_out->out;
        }
        m_orientedNodes.RemoveAt(beginIndex);
    }
    else
    {
        UnorientedEdge *p_edge = m_unorientedNodes[beginIndex]->firstEdge;
        while(p_edge)
        {
            RemoveEdge(m_unorientedNodes[p_edge->beginVex]->data, m_unorientedNodes[p_edge->endVex]->data);
            p_edge = p_edge->beginLink;
        }
        m_unorientedNodes.RemoveAt(beginIndex);
    }
    m_indexMapping.Remove(node);
    this->m_nodeCount--;
    return true;
}

//删除一条边
template <class NodeType,class EdgeType>                                   
bool AdjacencyList<NodeType,EdgeType>::RemoveEdge(const NodeType& begin, const NodeType& end)
{
    if(!m_indexMapping.ContainsKey(begin)||!m_indexMapping.ContainsKey(end))
        return false;

    bool isSucess = false;
    unsigned long beginIndex = m_indexMapping[begin];
    unsigned long endIndex = m_indexMapping[end];

    if(this->m_isOriented)
    {
        
        OrientedEdge* p_edge_out = m_orientedNodes[beginIndex]->out;
        OrientedEdge* p_edge_in = m_orientedNodes[endIndex]->in;
        OrientedEdge* p_next = NULL;
        //删除出边
        if(p_edge_out->beginVex == beginIndex)
        {
            m_orientedNodes[beginIndex]->out = p_edge_out->out;
            isSucess = true;
        }
        else
        {
            while (p_edge_out)
            {
                p_next = p_edge_out->out;
                if(p_next)
                {
                    if (p_next->beginVex == beginIndex)
                    {
                        p_edge_out->out = p_next->out;
                        isSucess = true;
                        break;
                    }
                }
                p_edge_out = p_next;
            }
        }
        //删除入边
        if(!isSucess)
            return false;
        if(p_edge_in->beginVex == beginIndex)
        {
            m_orientedNodes[endIndex]->in = p_edge_in->in;
        }
        else
        {
            while (p_edge_in)
            {
                p_next = p_edge_in->in;
                if(p_next)
                {
                    if (p_next->beginVex == beginIndex)
                    {
                        p_edge_in->in = p_next->in;
                        break;
                    }
                }
                p_edge_in = p_next;
            }
        }
        m_orientedNodes[beginIndex]->outDegree--;
        m_orientedNodes[endIndex]->inDegree--;
    }
    else
    {
        UnorientedEdge* p_edge = m_unorientedNodes[beginIndex]->firstEdge;
        UnorientedEdge* p_pre = p_edge;
        unsigned int count = 0;
        //删掉 起点到终点
        while(p_edge)
        {
            count++;
            if((p_edge->beginVex == beginIndex && p_edge->endVex == endIndex)
            ||(p_edge->endVex == beginIndex && p_edge->beginVex == endIndex))
            {
                isSucess = true;
                break;
            }
            p_pre = p_edge;
            if(p_edge->beginVex == beginIndex)
                p_edge = p_edge->beginLink;
            else
                p_edge = p_edge->endLink;
        }
        if(!isSucess)
            return false;
        if(count == 1)
        {
            if(p_edge->beginVex == beginIndex)
                m_unorientedNodes[beginIndex]->firstEdge = p_edge->beginLink;
            else
                m_unorientedNodes[beginIndex]->firstEdge = p_edge->endLink;
        }
        else
        {
            if(p_pre->beginVex == beginIndex)
            {
                if(p_edge->beginVex == beginIndex)
                    p_pre->beginLink = p_edge->beginLink;
                else
                    p_pre->beginLink = p_edge->endLink;
            }
            else
            {
                if(p_edge->beginVex == beginIndex)
                    p_pre->endLink = p_edge->beginLink;
                else
                    p_pre->endLink = p_edge->endLink;
            }
        }
        
        //删掉终点到起点
        p_edge = m_unorientedNodes[endIndex]->firstEdge;
        p_pre = p_edge;
        count = 0;
        while(p_edge)
        {
            count++;
            if((p_edge->beginVex == beginIndex && p_edge->endVex == endIndex)
            ||(p_edge->endVex == beginIndex && p_edge->beginVex == endIndex))
            {
                break;
            }
            p_pre = p_edge;
            if(p_edge->beginVex == endIndex)
                p_edge = p_edge->beginLink;
            else
                p_edge = p_edge->endLink;
        }
        if(count == 1)
        {
            if(p_edge->beginVex == endIndex)
                m_unorientedNodes[endIndex]->firstEdge = p_edge->beginLink;
            else
                m_unorientedNodes[endIndex]->firstEdge = p_edge->endLink;
        }
        else
        {
            if(p_pre->beginVex == endIndex)
            {
                if(p_edge->beginVex == endIndex)
                    p_pre->beginLink = p_edge->beginLink;
                else
                    p_pre->beginLink = p_edge->endLink;
            }
            else
            {
                if(p_edge->beginVex == endIndex)
                    p_pre->endLink = p_edge->beginLink;
                else
                    p_pre->endLink = p_edge->endLink;
            }
        }

        m_unorientedNodes[beginIndex]->degree--;
        m_unorientedNodes[endIndex]->degree--;
    }
    if(isSucess)
        this->m_edgeCount--;
    return isSucess;
}

//获得边权
template <class NodeType,class EdgeType>                                     
EdgeType& AdjacencyList<NodeType,EdgeType>::GetEdgeweight(const NodeType& begin, const NodeType& end)
{
    if(!m_indexMapping.ContainsKey(begin)||!m_indexMapping.ContainsKey(end))
        error("Node dosen't exist !");
    
    unsigned long beginIndex = m_indexMapping[begin];
    unsigned long endIndex = m_indexMapping[end];

    if(this->m_isOriented)
    {

        OrientedEdge* p_edge_out = m_orientedNodes[beginIndex]->out;
        while (p_edge_out)
        {
            if(p_edge_out->beginVex == beginIndex && p_edge_out->endVex == endIndex)
            {
                return p_edge_out->weight;
            }
            p_edge_out = p_edge_out->out;
        }
    }
    else
    {
        UnorientedEdge* p_edge = m_unorientedNodes[beginIndex]->firstEdge;
        while(p_edge)
        {
            if((p_edge->beginVex == beginIndex && p_edge->endVex == endIndex)
            ||(p_edge->endVex == beginIndex && p_edge->beginVex == endIndex))
            {
                return p_edge->weight;
            }
            if(p_edge->beginVex == beginIndex)
                p_edge = p_edge->beginLink;
            else
                p_edge = p_edge->endLink;
        }
    }
    error("Edge dosen't exist !");
    
}

//设置边权
template <class NodeType,class EdgeType>                                     
void AdjacencyList<NodeType,EdgeType>::SetEdgeweight(const NodeType& begin, const NodeType& end,const EdgeType& weight)
{
    if(!m_indexMapping.ContainsKey(begin)||!m_indexMapping.ContainsKey(end))
        error("Node dosen't exist !");

    unsigned long beginIndex = m_indexMapping[begin];
    unsigned long endIndex = m_indexMapping[end];

    if(this->m_isOriented)
    {
        OrientedEdge* p_edge_out = m_orientedNodes[beginIndex]->out;
        while (p_edge_out)
        {
            if(p_edge_out->beginVex == beginIndex && p_edge_out->endVex == endIndex)
            {
                p_edge_out->weight = weight;
                return;
            }
            p_edge_out = p_edge_out->out;
        }
    }
    else
    {
        UnorientedEdge* p_edge = m_unorientedNodes[beginIndex]->firstEdge;
        while(p_edge)
        {
            if((p_edge->beginVex == beginIndex && p_edge->endVex == endIndex)
            ||(p_edge->endVex == beginIndex && p_edge->beginVex == endIndex))
            {
                p_edge->weight = weight;
            }
            if(p_edge->beginVex == beginIndex)
                p_edge = p_edge->beginLink;
            else
                p_edge = p_edge->endLink;
        }
    }
    error("Edge dosen't exist !");
}

//获得结点的度
template <class NodeType,class EdgeType>                                     
unsigned int  AdjacencyList<NodeType,EdgeType>::GetNodeDegree(const NodeType& node)
{
    if(m_indexMapping.ContainsKey(node))
    {
        error("Node dosen't exist !");
    }
    if(this->m_isOriented)
    {
        unsigned long index = m_indexMapping[node];
        return m_orientedNodes[index]->inDegree + m_orientedNodes[index]->outDegree;
    }
    else
    {
        return m_unorientedNodes[m_indexMapping[node]]->degree;
    }
    
}

//获得结点的入度
template <class NodeType,class EdgeType>                                     
unsigned int  AdjacencyList<NodeType,EdgeType>::GetNodeInDegree(const NodeType& node)
{
    if(m_indexMapping.ContainsKey(node))
    {
        error("Node dosen't exist !");
    }
    if(this->m_isOriented)
    {
        return m_orientedNodes[m_indexMapping[node]]->inDegree;
    }
    else
    {
        return m_unorientedNodes[m_indexMapping[node]]->degree;
    }
    
}

//获得结点的出度
template <class NodeType,class EdgeType>                                     
unsigned int  AdjacencyList<NodeType,EdgeType>::GetNodeOutDegree(const NodeType& node)
{
    if(m_indexMapping.ContainsKey(node))
    {
        error("Node dosen't exist !");
    }
    if(this->m_isOriented)
    {
        return m_orientedNodes[m_indexMapping[node]]->outDegree;
    }
    else
    {
        return m_unorientedNodes[m_indexMapping[node]]->degree;
    }
    
}


#pragma region 枚举器实现

template <class NodeType,class EdgeType>                                     
bool AdjacencyList<NodeType,EdgeType>::MoveNext()
{
    if(this->m_isOriented)
    {
        if(m_orientedNodes.Count() == 0 || m_currentIndex == m_orientedNodes.Count()-1)
            return false;
        this->SetCurrent(m_orientedNodes[++m_currentIndex]->data);
    }
    else
    {
        if(m_unorientedNodes.Count() == 0 || m_currentIndex == m_unorientedNodes.Count()-1)
            return false;
        this->SetCurrent(m_unorientedNodes[++m_currentIndex]->data);
    }
    return true;
}

template <class NodeType,class EdgeType>                                     
void AdjacencyList<NodeType,EdgeType>::Reset()
{
    m_currentIndex = 0;
    if(this->m_isOriented)
    {
        if(m_orientedNodes.Count() != 0 )
            this->SetCurrent(m_orientedNodes[m_currentIndex]->data);
    }
    else
    {
        if(m_unorientedNodes.Count() != 0 )
            this->SetCurrent(m_unorientedNodes[m_currentIndex]->data);
    }
}
#pragma endregion