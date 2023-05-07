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
    }
    // addMatch ( contestant1, contestant2, result )
    CContest & addMatch(const string & c1, const string & c2, const M_ & match) {
        if(matchExists(c1,c2, match)) 
          throw std::logic_error("Duplicate match");
        
        matches.push_back(Match{c1,c2,match});
        contestants.insert(c1);
        contestants.insert(c2);
      return *this;
    }
    // isOrdered ( comparator )
    bool isOrdered(const function<int (const M_ &)> & cmp) const noexcept {
        unordered_map<string, vector<string>> graph;
        for(const Match & match : matches) {
            // Loser knows it's winners
            // cout << match.c1 << " " << match.c2 << endl;
            if(cmp(match.m) > 0) 
                graph[match.c1].push_back(match.c2);
            if(cmp(match.m) < 0)
                graph[match.c2].push_back(match.c1);
        }
        unordered_map<string, int> in_degrees;
        
        for(const auto & g : graph) {
            in_degrees[g.first] = 0;
        }

        for(const auto & g : graph) {
            for(const auto & neigh : graph[g.first]) {
                in_degrees[neigh]++;
            }
        }

        queue<string> q;

        for(const auto & it : in_degrees) {
            // cout << it.first << " has: " << it.second << endl;
            if(it.second == 0)
                q.push(it.first);
        }
        // cout << q.size() << endl;
        if(q.size() != 1) return false;
        
        vector<string> scoreboard;
        while(!q.empty()) {
            string cur = q.front();
            q.pop();
            scoreboard.push_back(cur);
            vector<string> tmp;
            for(const auto & neighbor : graph[cur]) {
                in_degrees[neighbor]--;
                if(in_degrees[neighbor] == 0) {
                    // q.push(neighbor);
                    tmp.push_back(neighbor);
                }
            }

            if(tmp.size() == 1) {
                q.push(tmp[0]);
            } 
            else if(tmp.size() == 0) continue;
            else {
                return false;
            }
        }

        if(scoreboard.size() != contestants.size()) 
            return false;
       
        return true;
    }

   
   
    // results ( comparator )
    list<string> results(const function<int (const M_ &)> & cmp) const {
       if(!isOrdered(cmp)) {
            throw std::logic_error("Can't be ordered");
       }
       unordered_map<string, vector<string>> graph;
        for(const Match & match : matches) {
            // Loser knows it's winners
            // cout << match.c1 << " " << match.c2 << endl;
            if(cmp(match.m) > 0) 
                graph[match.c1].push_back(match.c2);
            if(cmp(match.m) < 0)
                graph[match.c2].push_back(match.c1);
        }
        unordered_map<string, int> in_degrees;
        
        for(const auto & g : graph) {
            in_degrees[g.first] = 0;
        }

        for(const auto & g : graph) {
            for(const auto & neigh : graph[g.first]) {
                in_degrees[neigh]++;
            }
        }

        queue<string> q;

        for(const auto & it : in_degrees) {
            // cout << it.first << " has: " << it.second << endl;
            if(it.second == 0)
                q.push(it.first);
        }
        // cout << q.size() << endl;
        
        vector<string> scoreboard;
        while(!q.empty()) {
            string cur = q.front();
            q.pop();
            scoreboard.push_back(cur);
            for(const auto & neighbor : graph[cur]) {
                in_degrees[neighbor]--;
                if(in_degrees[neighbor] == 0) {
                    // q.push(neighbor);
                    q.push(neighbor);
                }
            }
        }
        // cout << "Results" << endl;
        list<string> ans;
        for(const string & s : scoreboard) {
            // cout << s << " ";
            ans.push_back(s);
        }
        // cout << endl;
       return ans;
    }
 
   
  private:
    struct Match {
        string c1;
        string c2;
        M_ m;
    };

    vector<Match > matches;
    set<string> contestants;
    
   
    bool matchExists (const string & c1, const string & c2, const M_ & match) const {
      for(const Match & match : matches) {
        if(match.c1 == c1 && match.c2 == c2) return true;
        if(match.c2 == c1 && match.c1 == c2) return true;
      }
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
