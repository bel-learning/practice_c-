#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */


template <typename M_>
class CContest
{
  
  public:

    // default constructor
    CContest() {};
    // destructor
    ~CContest() {
       for(auto it = graph.begin(); it != graph.end(); it++) {
        delete it->second;
       }
    }
    // addMatch ( contestant1, contestant2, result )
    CContest & addMatch(const string & c1, const string & c2, const M_ & match) {
        if(!addMatchToGraph(c1,c2, match)) 
          throw std::logic_error("Duplicate match");

      return *this;
    }
    // isOrdered ( comparator )
    template <typename cmp_>
    bool isOrdered(cmp_ cmp) const {

      unordered_map<string, int> table;   

       for(auto it = graph.begin(); it != graph.end(); it++) {
          // it -> first = string
          // it -> second = Node *
          vector<pair <Node *, Edge> > & neighbours = it->second->neighbours;
          for(pair < Node *, Edge > & it1 : neighbours) {
            Edge & e = it1.second;
            int state = cmp(e.match);
            if(e.firstPlayer)
              e.state = state;
            else
              e.state = -(state);
            it1.second = e;
            // cout << e.c1 << " - " << e.c2 <<  " state: " << it1.second.state << endl;
          }
       }
      unordered_map<string, int> wins;
      vector<int> totalWins;
      for(auto it = graph.begin(); it != graph.end(); it++) {
        // it -> first = string
          // it -> second = Node *
        Node * cur = it->second;
        vector<string> parents;
        pair <int , bool> values = cur->calculateWin(wins, parents);
        // cout << it->first << " : ";
        // cout << values.first << " " << values.second << endl;

        if(!values.second) return false;
        totalWins.push_back(values.first);
      }
      sort(totalWins.begin(), totalWins.end());
      for(size_t i = 0; i < totalWins.size() -1; i++) {
        if(totalWins[i] == totalWins[i+1]) {
          return false;
        }
      }

      return true;
    }
   
      // results ( comparator )
    template <typename cmp_>
    list<string> results(cmp_ cmp) {
      if(!isOrdered(cmp)) throw std::logic_error("List is unambiguous");
      unordered_map<string, int> wins;
      vector<pair<string, int>> totalWins;
      for(auto it = graph.begin(); it != graph.end(); it++) {
        // it -> first = string
          // it -> second = Node *
        Node * cur = it->second;
        vector<string> parents;
        pair <int , bool> values = cur->calculateWin(wins, parents);
        totalWins.push_back(make_pair(it->first, values.first));
      }
      sort(totalWins.begin(), totalWins.end(), [] (const pair<string, int> & a, const pair<string, int> & b) {
        return a.second < b.second;
      });
      list<string> ans;
      for(size_t i = 0; i < totalWins.size(); i++) {
        // cout << totalWins[i].first << " ";
        ans.push_front(totalWins[i].first);
      }
      // cout << endl;
      return ans;
    }
  
      
    bool addMatchToGraph(const string & c1, const string & c2, const M_ & match) {
      if(matchExists(c1,c2, match)) return false;
      if(matchExists(c2,c1, match)) return false;

      if(graph.find(c1) == graph.end()) {
        graph[c1] = new Node(c1);
      }
      if(graph.find(c2) == graph.end()) {
        graph[c2] = new Node(c2);
      }
      graph[c1]->neighbours.push_back(make_pair(graph[c2], Edge{c1, c2, 0, match, true}));
      graph[c2]->neighbours.push_back(make_pair(graph[c1], Edge{c2, c1, 0, match, false}));

      return true;

    }
    bool matchExists (const string & c1,const string & c2, const M_ & match) {
      if(graph.find(c1) != graph.end()) {
        vector<pair <Node *, Edge >> & neighbours = graph[c1]->neighbours;
        for(size_t i = 0; i < neighbours.size(); i++) {
          if(neighbours[i].second.c2 == c2 ) {
            return true;
          }
        }
      }
      return false;
    }
  private:
    // todo

  struct Edge {
    string c1;
    string c2;
    int state;
    M_ match;
    bool firstPlayer;
  };
  class Node {
  public:
    string c1;
    int won;
    vector< pair<Node *, Edge> > neighbours;
    Node(const string & str): c1(str), won(-1) {};
    pair<int, bool> calculateWin(unordered_map<string, int> & wins, vector<string> parents) const {
      // cout << c1 << " ";
      // cout << "iterating" << endl;
      for(const string & parent : parents) {
        if(c1 == parent) {
          // cout << "Found same parent" << endl;
          return make_pair(won, false);
        }
      }
      if(wins.find(c1) != wins.end()) return make_pair(wins.at(c1), true);

      int childCount = 0;
      for(const pair<Node *, Edge> & it : neighbours) {
         if(it.second.state > 0) {
          childCount++;
        }
      }

      // propogate backward if it's the leaf node
      if(childCount == 0) return make_pair(0, true);
      int sum = 0;
      // cout << "Trying to calculate win" << endl;
      for(const pair<Node *, Edge> & it : neighbours) {
        // cout << "Iterating through neighbours" << endl;
        if(it.second.state > 0) {
          parents.push_back(c1);
          pair<int, bool > values = it.first->calculateWin(wins, parents);
          if(!values.second) return values;
          sum += values.first + 1;
        }
      }
      return make_pair(sum , true);
    }
  };

  unordered_map<string, Node * > graph;
   
  
};


#ifndef __PROGTEST__
struct CMatch
{
  public:
                             CMatch                        ( int               a, 
                                                             int               b )
      : m_A ( a ), 
        m_B ( b )
    {
      }
    
    int                      m_A;
    int                      m_B;
};

class HigherScoreThreshold
{
  public:
                             HigherScoreThreshold          ( int diffAtLeast )
      : m_DiffAtLeast ( diffAtLeast )
    {
    }
    int                      operator ()                   ( const CMatch & x ) const
    {
      return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
  private:
    int            m_DiffAtLeast;    
};

int                HigherScore                             ( const CMatch    & x )
{
  return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}


ostream & operator << (ostream & out, const CMatch & m) {
  return out << m.m_A << " " << m.m_B;
}

int                main                                    ( void )
{
  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  
  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  
  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
  
  CContest<bool>  y;
  
  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
    
  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  CContest<CMatch>  z;
  z.addMatch("a", "b", CMatch(10,1));
  z.addMatch("b", "c", CMatch(10,10));
  assert(!z.isOrdered(HigherScore));



  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
