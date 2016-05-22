#ifndef CGraph_H_INCLUDED
#define CGraph_H_INCLUDED
#include "../common.h"
template <class F, class S, class T> struct trine;
template <class F, class S, class T> ostream& operator<< (ostream&, trine<F,S,T> const &);
template <class F, class S, class T>
struct trine{
    F first;
    S second;
    T third;
    trine() : first(F()), second(S()), third(T()) {}
    trine(F _f, S _s, T _t) : first(_f), second(_s), third(_t){}
    bool operator==(const trine& rhs){
        return (rhs.first==first && rhs.second==second && rhs.third==third);
    }
    friend ostream& operator<< <>(ostream& s, trine<F,S,T> const & rhs);
};


template <class E> struct CEdge;
template <class E> istream& operator>> (istream&, CEdge<E>&);
template <class E> ostream& operator<< (ostream&, CEdge<E> const &);
template <class E>
struct CEdge{
    int index;
    int first;
    int second;
    E value;
    CEdge() : index(0), first(0), second(0), value(E()) {}
    CEdge(int __index, int __f, int __s, E __val) : index(__index), first(__f), second(__s), value(__val) {}
    bool operator==(const CEdge& rhs) const{ return (index==rhs.index); }
    bool operator< (const CEdge& rhs) const{ return (index<rhs.index); }
    CEdge<E>& operator=(const CEdge<E>& rhs){
        index=rhs.index;
        first=rhs.first;
        second=rhs.second;
        value=rhs.value;
        return (*this);
    }
    friend istream& operator>> <>(istream& s, CEdge<E>& rhs);
    friend ostream& operator<< <>(ostream& s, CEdge<E> const & rhs);
};

template <class N> struct CGraphNode;
template <class N> istream& operator>> (istream&, CGraphNode<N>&);
template <class N> ostream& operator<< (ostream&, CGraphNode<N> const &);
template <class N>
struct CGraphNode{
    int index;
    N value;
    CGraphNode(int __index, N __val) : index(__index), value(__val) {}
    bool operator==(const CGraphNode& rhs) const{ return (index==rhs.index && value==rhs.value); }
    bool operator< (const CGraphNode& rhs) const{ return (index<rhs.index); }
    friend istream& operator>> <>(istream& s, CGraphNode<N>& rhs);
    friend ostream& operator<< <>(ostream& s, CGraphNode<N> const & rhs);
};


template <class E, class N> class CGraph;
template <class E, class N> istream& operator>> (istream&, CGraph<E,N>&);
template <class E, class N> ostream& operator<< (ostream&, CGraph<E,N> const &);

template <class E, class N>
class CGraph{
    typedef pair<bool, E> EdgeProp;
private:
    list< CEdge<E> > edges;
    list< CGraphNode<N> > nodes;
public:
    CGraph() : edges(list< CEdge<E> >(0, CEdge<E>(0, 0, 0, E()))), nodes(list< CGraphNode<N> >(0, CGraphNode<N>(0, N()))) {}


    int addNode(N);
    void addEdge(int, int, E=E());
    void addEdgeOnValue(N, N, E=E());
    void addBiEdge(int, int, E=E());
    void removeEdge(int);
    void removeEdge(int, int);
    void removeNode(int);

    bool isNodeExists(N) const;

    CEdge<E>& getEdge(int, int);
    set<int> getEdgesIndexFromNode(int) const;
    set<CEdge<E>> getEdgesCopyFromNode(int) const;
    set<CEdge<E>> getEdgesCopyToNode(int) const;
    set<int> getEdgesIndexToNode(int) const;
    list<CEdge<E>> getAllEdgesCopy() const;

    CGraphNode<N>& getNode(int);
    CGraphNode<N>& getNode(N);
    set<int> getNodesIndexFromNode(int) const;
    set<int> getNodesIndexToNode(int) const;
    set<CGraphNode<N>> getNodesCopyFromNode(int) const;
    set<CGraphNode<N>> getNodesCopyToNode(int) const;
    set<CGraphNode<N>&> getNodesFromNode(int);
    set<CGraphNode<N>&> getNodesToNode(int);
    list<CGraphNode<N>> getAllNodesCopy() const;

    vector<E> Dijkstra(int, E);
    CGraph<E,N> BFS(int);
    void DFS(bool&, list<CGraphNode<N>*>&);
    void DFS_visit(int , vector<int>&, int&, bool&, list<CGraphNode<N>*>&);
    pair<bool, list<CGraphNode<N>>> TSort();

    friend istream& operator>> <>(istream&, CGraph<E,N>&);
    friend ostream& operator<< <>(ostream&, CGraph<E,N> const &);

    struct iterator;

    typename CGraph<E,N>::iterator begin(){
        return CGraph<E,N>::iterator(&(*nodes.begin()), this);
    }
    typename CGraph<E,N>::iterator findNode(N n){
        for (auto& i: nodes)
            if (n==i.value)
                return CGraph<E,N>::iterator(&i, this);
        cout<<"Not found"<<endl;
        return CGraph<E,N>::iterator();
    }
};
#include "Graph.hpp"

#endif // CGraph_H_INCLUDED
