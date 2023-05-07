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
       for(auto & it : graph) {
        for(auto & it1: it.second) {
          delete it1;
        }
       }
    }
    // addMatch ( contestant1, contestant2, result )
    CContest & addMatch(const string & c1, const string & c2, const M_ & match) {
        if(!addMatchToGraph(c1,c2, match)) 
          throw std::logic_error("Duplicate match");
      return *this;
    }
    // isOrdered ( comparator )
    bool isOrdered(const function<int (const M_ &)> & cmp) const noexcept {
    
      // if(hasCycle(cmp)) {
      //   cout << "detected cycle" << endl;
      //   return false; 
      // };
      // unordered_map<string, int> winTable;
      vector<string> sorted = topologicalSort();
      if(sorted.empty()) {
        return false;
      }
      else {
        return true;
      }
      // for(const auto & it : graph) {
      //   // winTable[it.first] = BFS(it.first, cmp, winTable);
    
      //   // cout << it.first << "  : " << winTable[it.first] << endl; 

      // }

      // // cout << endl;

      //  // see how many did we win for each players with memoization
      // unordered_map<int, bool> duplicateTable;

      // // Should look like 5 4 3 2 1 so no duplicates
      // for(auto & it : winTable) {
      //   if(duplicateTable.find(it.second) != duplicateTable.end()) {
      //     return false;
      //   }
      //   else {
      //     duplicateTable[it.second] = true;
      //   }
      // }
      // // cout << "Order exists" << endl;

      // return true;
    }

   
   
    // results ( comparator )
    list<string> results(const function<int (const M_ &)> & cmp) const {
       if(!isOrdered(cmp)) throw std::logic_error("List is unambiguous");
        unordered_map<string, int> winTable;

        for(auto & it : graph) {
          // see how many did we win
          winTable[it.first] = BFS(it.first, cmp, winTable);
          // O(contestants * matches)
        }
        vector<pair<string, int> > playersWin;
        for(auto & it : winTable) {
          playersWin.push_back(make_pair(it.first, it.second));
        }
        sort(playersWin.begin(), playersWin.end(), [] (const pair<string, int> & a, const pair<string, int> & b) {
          return a.second < b.second;
        });
        // O(contestants * (log(contestants))
        list<string> ans;
        for(const pair<string, int> & it : playersWin) {
          ans.push_front(it.first);
        }
        
        return ans;
    }
 
   
  private:
  struct Weight {
    M_ match;
    bool firstPlayer;
  };
    // todo
  unordered_map<string, vector<pair< string, Weight > * > > graph;
 
    vector<string> topologicalSort () const {
      queue<string> q;
      unordered_map<string, int> in_degree;
      for(auto & q : graph) {
        cout << q.first << endl;
        int degree = 0;
        for(auto &v : q.second) {
          
        }
        in_degree[q.first]
      }
      for (auto &kv : in_degree) {
          if (kv.second == 0) {
              q.push(kv.first);
          }
      }
      vector<string> result;
      while (!q.empty()) {
          string u = q.front();
          q.pop();
          result.push_back(u);
          for (auto &v : graph.at(u)) {
              in_degree[v->first]--;
              if (in_degree[v->first] == 0) {
                  q.push(v->first);
              }
          }
      }
      
      if (result.size() != in_degree.size()) {
          return vector<string>();
      } else {
          return result;
      }
    }

    int BFS(const string & start, const function<int (const M_ &)> & cmp, unordered_map<string, int> & winTable) const {
      unordered_map<string, bool> visited;
      // unordered_map<string, bool> wonOpponent;
      int winCount = 0;
      queue<pair<string, int>> q;
      q.push(make_pair(start, 1));
      while(!q.empty()) {
        const string & cur = q.front().first; 
        const int & state = q.front().second;
        q.pop();
        if(visited[cur])
          continue;
        if(state > 0)
          winCount++;
        // if(winTable[cur] && !wonOpponent[cur]) {
        //   winCount += winTable.at(cur);
        //   visited[cur] = true;
        //   continue;
        // }
        visited[cur] = true;
        vector< pair< string, Weight > * > neighbours;
        if(graph.find(cur) != graph.end())
          neighbours = graph.at(cur);

        for(auto & neigh : neighbours) {
          int state = cmp(neigh->second.match);
          if(!neigh->second.firstPlayer) state = -(state);
          if(state >= 0) {
            
            q.push(make_pair(neigh->first,state));
            // if(wonOpponent[neigh->first])
              // continue;
            // winCount++;
            // wonOpponent[neigh->first] = true;
          }
        }
      }
      return winCount;
    }

    bool addMatchToGraph(const string & c1, const string & c2, const M_ & match) {
      if(matchExists(c1,c2, match)) return false;
      if(matchExists(c2,c1, match)) return false;
      graph[c1].push_back(new pair<string, Weight>(c2, Weight{match, true}));
      graph[c2].push_back(new pair<string, Weight>(c1, Weight{match, false}));

      return true;

    }

    bool matchExists (const string & c1, const string & c2, const M_ & match) const {
      if(graph.find(c1) != graph.end()) {
        for (auto & neigh : graph.at(c1)) {
          if(neigh->first == c2)
            return true;
        }
      }
      return false;
    }
    bool hasCycle(const function<int (const M_ &)> & cmp) const {
    unordered_map<string, bool> visited;
    unordered_map<string, bool> onStack;

     for(const auto & p : graph) {
        // cout << "Checking: " << p.first << endl;
        if(checkCycleUtil(cmp, p.first, visited, onStack)) {
          return true;
        }
      }
    return false; // no cycle detected
    }
    bool checkCycleUtil(const function<int (const M_ &)> & cmp, const string & cur, unordered_map<string, bool>  & visited, unordered_map<string, bool> & onStack) const {
      if(onStack[cur])
        return true;
      if(visited[cur])
        return false;
      visited[cur] = true;
      onStack[cur] = true;
      vector<pair<string, Weight > * > neighbours;
      if(graph.find(cur) != graph.end())
        neighbours = graph.at(cur);

      for(auto & it : neighbours) {
        int state = cmp(it->second.match);
        if(!it->second.firstPlayer) state = -(state);
        if(state <= 0) continue;
        if(checkCycleUtil(cmp, it->first, visited, onStack)) {
          return true;
        }
      }
      onStack[cur] = false;
      return false;
    }
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

  assert ( x . isOrdered ( HigherScore ));
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

  CContest<CMatch> z;

  z.addMatch("1", "2", CMatch(10,1));
  z.addMatch("2", "3", CMatch(10,1));
  z.addMatch("2", "4", CMatch(10,1));
  z.addMatch("3", "4", CMatch(10,1));
  z.addMatch("4", "5", CMatch(10,1));
  z.addMatch("2", "5", CMatch(10,1));

  assert(z.isOrdered(HigherScore) );

  // list<string> res = z.results(HigherScore);
  // assert ( ( res == list<string>{"a", "c", "b"}) );


  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
