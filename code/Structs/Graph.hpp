#ifndef CGraph_HPP_INCLUDED
#define CGraph_HPP_INCLUDED
#include "Graph.h"

template <class F, class S, class T>
ostream& operator<< (ostream& s, trine<F,S,T> const & rhs){
	s<<"trine ("<<rhs.first<<" "<<rhs.second<<" "<<rhs.third<<")";
	return s;
}

template <class E>
istream& operator>> (istream& s, CEdge<E>& rhs){
	s>>rhs.first>>rhs.second>>rhs.value;
	return s;
}
template <class E>
ostream& operator<< (ostream& s, CEdge<E> const & rhs){
	s<<rhs.index<<" "<<"("<<rhs.first<<" ; "<<rhs.second<<")"<<" : "<<rhs.value;
	return s;
}

template <class N>
istream& operator>> (istream& s, CNode<N>& rhs){
	s>>rhs.N;
	return s;
}
template <class N>
ostream& operator<< (ostream& s, CNode<N> const & rhs){
	s<<rhs.index<<" "<<rhs.value;
	return s;
}

template <class E, class N>
struct CGraph<E,N>::iterator : std::iterator<ptrdiff_t, CNode<N>, CNode<N>*, CNode<N>&, bidirectional_iterator_tag>{
	typedef iterator          Self;
	CGraph<E,N>* _CGraph;
	CNode<N>* _node;

	iterator() : _CGraph(), _node() {}
	explicit iterator(CNode<N>* __x, CGraph<E,N>* __g) :  _CGraph(__g), _node(__x){}

	CNode<N> operator*() const { return *(_node); }
	CNode<N>* operator->() const { return _node; }
	Self& up(){
		set<int> parents=_CGraph->getNodesIndexToNode(_node->index);
		if (!parents.empty())
			(*this)=iterator(&_CGraph->getNode(*(parents.begin())), _CGraph);
		return (*this);
	}
	Self& down(){
		set<int> children=_CGraph->getNodesIndexFromNode(_node->index);
		if (!children.empty())
			(*this)=iterator(&_CGraph->getNode(*(children.begin())), _CGraph);
		return (*this);
	}
	Self& right(){
		set<int> parents=_CGraph->getNodesIndexToNode(_node->index);
		if (!parents.empty()){
			set<int> children;
			for (auto i : parents){
				set<int> temp=_CGraph->getNodesIndexFromNode(i);
				children.insert(temp.begin(), temp.end());
			}
			(*this)=iterator(&(_CGraph->getNode(*(++children.find(_node->index)))), _CGraph);
		}
		return (*this);
	}
	Self& left(){
		set<int> parents=_CGraph->getNodesIndexToNode(_node->index);
		if (!parents.empty()){
			set<int> children;
			for (auto i : parents){
				set<int> temp=_CGraph->getNodesIndexFromNode(i);
				children.insert(temp.begin(), temp.end());
			}
			(*this)=iterator(&(_CGraph->getNode(*(--children.find(_node->index)))), _CGraph);
		}
		return (*this);
	}
	bool operator==(const Self& __x) const { return (_node==__x._node); }
	bool operator!=(const Self& __x) const { return (_node!=__x._node); }
};

///Adds & removes
template <class E, class N>
int CGraph<E,N>::addNode(N value){
	if ( !nodes.empty() )
		nodes.push_back(CNode<N>(nodes.back().index+1, value));
	else
		nodes.push_back(CNode<N>(0, value));
	return nodes.back().index;
}

template <class E, class N>
void CGraph<E,N>::addEdge(int __f, int __s, E __weight){
	if (edges.empty()==false)
		edges.push_back(CEdge<E>(edges.back().index+1, __f, __s, __weight));
	else
		edges.push_back(CEdge<E>(0, __f, __s, __weight));
}

template <class E, class N>
void CGraph<E,N>::addEdgeOnValue(N _f, N _s, E __weight){
	int __f=0, __s=0;
	for (auto i : nodes)
		if (i.value==_f)
			__f=i.index;
		else if(i.value==_s)
			__s=i.index;
	addEdge(__f, __s, __weight);
}

template <class E, class N>
void CGraph<E,N>::addBiEdge(int __f, int __s, E __weight){
	if (edges.empty()==false)
		edges.push_back(CEdge<E>(edges.back().index+1, __f, __s, __weight));
	else
		edges.push_back(CEdge<E>(0, __f, __s, __weight));
	edges.push_back(CEdge<E>(edges.back().index+1, __s, __f, __weight));
}

template <class E, class N>
void CGraph<E,N>::removeEdge(int __index){
	for (auto i : edges)
		if (i.index==__index){
			edges.remove(i);
			break;
		}
}

template <class E, class N>
void CGraph<E,N>::removeEdge(int __f, int __s){
	for (typename list< CEdge<E> >::iterator it=edges.begin(); it!=edges.end(); it++)
		if ((*it).first==__f && (*it).second==__s){
			typename list< CEdge<E> >::iterator temp_it(it);
			it--;
			edges.remove((*temp_it));
		}
}

template <class E, class N>
void CGraph<E,N>::removeNode(int __index){
	if (__index>=0){
		for (typename list< CEdge<E> >::iterator it=edges.begin(); it!=edges.end(); it++)
			if ((*it).first==__index || (*it).second==__index){
				typename list< CEdge<E> >::iterator temp_it(it);
				it--;
				edges.remove((*temp_it));
			}
		for (auto i : nodes)
			if (i.index==__index){
				nodes.remove(i);
				break;
			}
	}
}

///Input & output
template <class E, class N>
ostream& operator<< (ostream& s, CGraph<E,N> const & m){
	for (auto i : m.nodes)
		s<<i<<endl;
	s<<endl;
	for (auto i : m.edges)
		s<<i<<endl;
	s<<endl;
	return s;
}

template <class E, class N>
istream& operator>> (istream& stream, CGraph<E,N> & m){
	string str;
	while(str!="exit" && str!="end" && str!="q"){
		stream>>str;
		if (str=="biedge"){
			int f, s;
			E data;
			stream>>f>>s>>data;
			m.addBiEdge(f,s,data);
			str="";
		}
		if (str=="node" || str=="Node"){
			N data;
			stream>>data;
			m.addNode(data);
			str="";
		}
		if (str=="edge" || str=="Edge"){
			int f, s;
			E data;
			stream>>f>>s>>data;
			m.addEdge(f,s,data);
			str="";
		}
	}
	return stream;
}

///Getters
template <class E, class N>
bool CGraph<E,N>::isNodeExists(N _node) const{
	for (auto i : nodes)
		if (i.value==_node)
			return true;
	return false;
}

template <class E, class N>
CNode<N>& CGraph<E,N>::getNode(int __index){
	for (auto& i : nodes)
		if (i.index==__index)
			return (i);
	cout<<"Not found"<<endl;
	return (*nodes.begin());
}

template <class E, class N>
CNode<N>& CGraph<E,N>::getNode(N value){
	for (auto& i : nodes)
		if (i.value==value)
			return (i);
	cout<<"Not found"<<endl;
	return (*nodes.begin());
}


template <class E, class N>
CEdge<E>& CGraph<E,N>::getEdge(int lhs, int rhs){
	for (auto& i : edges)
		if (i.first==lhs && i.second==rhs)
			return i;
	cout<<"Not found"<<endl;
	return (*edges.begin());
}

template <class E, class N>
set<int> CGraph<E,N>::getEdgesIndexFromNode(int __index) const{
	set<int> res;
	for (auto i : edges)
		if (i.first==__index)
			res.insert(i.index);
	return res;
}

template <class E, class N>
set<CEdge<E>> CGraph<E,N>::getEdgesCopyFromNode(int __index) const{
	set<CEdge<E>> res;
	for (auto i : edges)
		if (i.first==__index)
			res.insert(i);
	return res;
}

template <class E, class N>
set<int> CGraph<E,N>::getNodesIndexFromNode(int __index) const{
	set<int> res;
	for (auto i : edges)
		if (i.first==__index)
			res.insert(i.second);
	return res;
}

template <class E, class N>
set<CNode<N>> CGraph<E,N>::getNodesCopyFromNode(int __index) const{
	set<CNode<N>> res;
	for (auto i : edges)
		if (i.first==__index)
			res.insert(i);
	return res;
}

template <class E, class N>
set<int> CGraph<E,N>::getEdgesIndexToNode(int __index) const{
	set<int> res;
	for (auto i : edges)
		if (i.second==__index)
			res.insert(i.index);
	return res;
}

template <class E, class N>
set<CEdge<E>> CGraph<E,N>::getEdgesCopyToNode(int __index) const{
	set<CEdge<E>> res;
	for (auto i : edges)
		if (i.second==__index)
			res.insert(i);
	return res;
}

template <class E, class N>
set<int> CGraph<E,N>::getNodesIndexToNode(int __index) const{
	set<int> res;
	for (auto i : edges)
		if (i.second==__index)
			res.insert(i.first);
	return res;
}

template <class E, class N>
set<CNode<N>&> CGraph<E,N>::getNodesFromNode(int __index){
	set<CNode<N>&> res;
	for (auto i : edges)
		if (i.first==__index)
			res.insert( getNode(i.second) );
	return res;
}

template <class E, class N>
set<CNode<N>&> CGraph<E,N>::getNodesToNode(int __index){
	set<CNode<N>&> res;
	for (auto i : edges)
		if (i.second==__index)
			res.insert( getNode(i.first) );
	return res;
}

template <class E, class N>
set<CNode<N>> CGraph<E,N>::getNodesCopyToNode(int __index) const{
	set<CNode<N>> res;
	for (auto i : edges)
		if (i.second==__index)
			res.insert(i);
	return res;
}

template <class E, class N>
list< CEdge<E> > CGraph<E,N>::getAllEdgesCopy() const{
	return(list< CEdge<E> >(edges));
}

template <class E, class N>
list< CNode<N> > CGraph<E,N>::getAllNodesCopy() const{
	return(list< CNode<N> >(nodes));
}

/// Algorithms
template <class E, class N>
vector<E> CGraph<E,N>::Dijkstra(int __index, E max_distance){
	vector<bool> visited=vector<bool>(nodes.back().index+1, true);
	for (auto i : nodes)
		visited[i.index]=false;
	vector<E> distance=vector<E>(nodes.back().index+1, max_distance);
	distance[__index]=0;
	bool loop_exit_flag=false;
	while(loop_exit_flag!=true){
		E min_distance=max_distance;
		int min_distance_node=__index;
		loop_exit_flag=true;
		for(int i=0; i<visited.size(); i++)
			if (visited[i]==false){
				loop_exit_flag=false;
				if (distance[i]<=min_distance){
					min_distance=distance[i];
					min_distance_node=i;
				}
			}
		if(loop_exit_flag==true)
			break;
		visited[min_distance_node]=true;

		set<CEdge<E>> nbh=getEdgesCopyFromNode(min_distance_node);
		for (auto i : nbh)
			if (visited[i.second])
				nbh.erase(i);
		for (auto i : nbh)
			if (distance[i.first]+i.value<distance[i.second])
				distance[i.second]=distance[i.first]+i.value;
	}
	return distance;
}

template <class E, class N>
CGraph<E,N> CGraph<E,N>::BFS(int __index){
	vector<int> colors(nodes.size(), 0);
	CGraph<E, int> tree;
	tree.addNode(__index);
	colors[__index]=1;
	list<int> gray_nodes;
	gray_nodes.push_back(__index);

	while(!gray_nodes.empty()){
		int current=gray_nodes.back();
		set<int> children=getNodesIndexFromNode(current);
		gray_nodes.pop_back();
		for (auto i : children){
			if (colors[i]==0){
				colors[i]=1;
				tree.addNode(i);
				tree.addEdge(current, i, getEdge(current, i).value);
				gray_nodes.push_back(i);
			}
		}
		colors[current]=2;
	}
	return tree;
}

template <class E, class N>
void CGraph<E,N>::DFS(bool& cycled, list<CNode<N>*>& ordered){
	vector<int> colors(nodes.size(), 0);
	int time=0;
	for (auto i : nodes)
		if (colors[i.index]==0){
			DFS_visit(i.index, colors, time, cycled, ordered);
		}
}
template <class E, class N>
void CGraph<E,N>::DFS_visit(int current, vector<int>& colors, int& time, bool& cycled, list<CNode<N>*>& ordered){
	colors[current]=1;
	time++;
	set<int> children=getNodesIndexFromNode(current);
	for (auto i : children){
		if (colors[i]==0)
			DFS_visit(i, colors, time, cycled, ordered);
		if (colors[i]==1)
			cycled=true;
	}
	ordered.push_back(&getNode(current));
	colors[current]=2;

	time++;
}
template <class E, class N>
pair<bool, list<CNode<N> > > CGraph<E,N>::TSort(){
	list<CNode<N>> ordered;
	bool cycled=false;
	DFS(cycled, ordered);
	return make_pair(cycled, ordered);
}

#endif // GRAPH_HPP_INCLUDED