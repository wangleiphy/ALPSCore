/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <alps/graph/embedding.hpp>
#include <map>
#include <iostream>

template <class subgraph_type, class graph_type>
class embedding_counter
{
  private:
    typedef std::map<
        typename boost::graph_traits<subgraph_type>::vertex_descriptor,
        typename boost::graph_traits<graph_type>::vertex_descriptor
        > map_type;
    typedef boost::associative_property_map<map_type> property_map_type;

  public:
    embedding_counter(subgraph_type const& subgraph, graph_type const& graph)
      : subgraph_(subgraph), graph_(graph)
    {}

    inline std::size_t count()
    {
      embedding_iterator<property_map_type, subgraph_type, graph_type> emb_it, emb_end;
      map_type map_store;
      property_map_type mapping(map_store);
      std::size_t count_emb = 0;
      boost::tie(emb_it, emb_end) = embedding(mapping, subgraph_, graph_);
      for (; emb_it != emb_end; ++emb_it)
        ++count_emb;
      return count_emb;
    }

    inline std::size_t count(
        typename boost::graph_traits<graph_type>::vertex_descriptor graph_vertex,
        typename boost::graph_traits<subgraph_type>::vertex_descriptor subgraph_vertex
    )
    {
      embedding_iterator<
          property_map_type,
          subgraph_type,
          graph_type,
          typename boost::graph_traits<subgraph_type>::vertex_descriptor,
          typename boost::graph_traits<graph_type>::vertex_descriptor
      > emb_it, emb_end;
      map_type map_store;
      property_map_type mapping(map_store);
      std::size_t count_emb = 0;
      boost::tie(emb_it, emb_end) = embedding(mapping, subgraph_, graph_, subgraph_vertex, graph_vertex);
      for (; emb_it != emb_end; ++emb_it)
        ++count_emb;
      return count_emb;
    }

  private:
    
    subgraph_type const & subgraph_;
    graph_type const & graph_;
};


void print(std::ostream& o, boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> const& graph)
{
    boost::graph_traits<boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> >:: edge_iterator it, end;
    boost::tie(it,end) = edges(graph);
    for(;it != end; ++it)
        o<<"("<<source(*it,graph)<<","<<target(*it,graph)<<")";
    o<<std::endl;
    return;
}

template <typename Graph>
void print_embedding(Graph const& subgraph, Graph const& graph)
{
    print(std::cout,subgraph);
    embedding_counter<Graph,Graph> emb(subgraph,graph);
    std::cout<<emb.count()<<std::endl;
}

void embedding_test1()
{
    // Embedding test 1
    // We count how many times we can embed each (connected) subgraph of g in g
    //
    //  2--4
    //  |  |
    //  0--1
    //  |
    //  3
    //

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph_type;
    graph_type g;
    add_edge(0,1,g);
    add_edge(0,2,g);
    add_edge(0,3,g);
    add_edge(1,4,g);
    add_edge(2,4,g);

    print(std::cout,g);

    std::cout<<"------------"<<std::endl;

    graph_type ha;
    add_edge(0,1,ha);
    
    print_embedding(ha,g);

    graph_type hb;
    add_edge(0,1,hb);
    add_edge(1,2,hb);
    
    print_embedding(hb,g);
    
    graph_type hc;
    add_edge(0,1,hc);
    add_edge(1,2,hc);
    add_edge(2,3,hc);
    
    print_embedding(hc,g);

    graph_type hd;
    add_edge(0,1,hd);
    add_edge(1,2,hd);
    add_edge(1,3,hd);
    
    print_embedding(hd,g);

    graph_type he;
    add_edge(0,1,he);
    add_edge(1,2,he);
    add_edge(2,3,he);
    add_edge(3,4,he);
    
    print_embedding(he,g);
    
    graph_type hg;
    add_edge(0,1,hg);
    add_edge(1,2,hg);
    add_edge(2,3,hg);
    add_edge(2,4,hg);
    
    print_embedding(hg,g);
    
    graph_type hh;
    add_edge(0,1,hh);
    add_edge(1,2,hh);
    add_edge(1,3,hh);
    add_edge(1,4,hh);
    
    print_embedding(hh,g);

    graph_type hi;
    add_edge(0,1,hi);
    add_edge(0,2,hi);
    add_edge(1,3,hi);
    add_edge(2,3,hi);
    
    print_embedding(hi,g);
}

void embedding_test2()
{

    // Embedding test 2
    // We try to embed the graphs
    //
    // 0--1--2      1--0--3
    //    |            |
    //    3            2
    //
    // in the graph
    //
    //  2--4
    //  |  |
    //  0--1
    //  |
    //  3
    //
    // and look if the result is the same.
    //

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph_type;
    graph_type g;
    add_edge(0,1,g);
    add_edge(0,2,g);
    add_edge(0,3,g);
    add_edge(1,4,g);
    add_edge(2,4,g);

    print(std::cout,g);

    std::cout<<"------------"<<std::endl;

    graph_type hd;
    add_edge(0,1,hd);
    add_edge(1,2,hd);
    add_edge(1,3,hd);
    
    print_embedding(hd,g);
    
    graph_type he;
    add_edge(0,1,he);
    add_edge(0,2,he);
    add_edge(0,3,he);
    
    print_embedding(he,g);
}

int main()
{
    embedding_test1();
    embedding_test2();
    return 0;
}

