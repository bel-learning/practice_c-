#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
using namespace std;

//=================================================================================================
// a class to represent one student
// The class is essentially a container that holds the name and grades
class CStudent
{
  public:
    // a simple constructor to fill-in the members
                   CStudent                                ( string            name,
                                                             char              pa1,
                                                             char              uos );
    // comparison operator, compare students by name, used by library std::sort
    bool           operator <                              ( const CStudent  & x ) const;
    // output operator
    friend ostream & operator <<                           ( ostream         & os,
                                                             const CStudent  & x );
    // input operator
    friend istream & operator >>                           ( istream         & is,
                                                             CStudent        & x );
  private:
    string         m_Name;
    char           m_PA1;
    char           m_UOS;
    friend class StudentComparator;
};
//=================================================================================================
// a class to implement so-called function object. The function objects will be
// used to compare students by various criteria.
// the instance is called with () operator to compare two
// CStudent instances (operator() parameters)
class StudentComparator
{
  public:
    static const bool ASCENDING  = false;
    static const bool DESCENDING = true;
    static const int  NAME       = 0;
    static const int  PA1        = 1;
    static const int  UOS        = 2;


                   StudentComparator                       ( int               module,
                                                             bool              sortOrder )
      : m_Module ( module ), 
        m_Order ( sortOrder ) 
    { 
    }
    // the overloaded operator - comparator
    bool           operator ()                             ( const CStudent  & a,
                                                             const CStudent  & b ) const
    {
      switch ( m_Module )
      {
        default:
        case NAME:
          return ( a . m_Name < b . m_Name ) ^ m_Order;
        case PA1:
          return ( a . m_PA1 < b . m_PA1 ) ^ m_Order;
        case UOS:
          return ( a . m_UOS < b . m_UOS ) ^ m_Order;
      }
    }
  private:
    int            m_Module;
    bool           m_Order;
};
//=================================================================================================
                   CStudent::CStudent                      ( string            name,
                                                             char              pa1,
                                                             char              uos )
  : m_Name ( move ( name ) ), 
    m_PA1 ( pa1 ), 
    m_UOS ( uos )
{
}
//-------------------------------------------------------------------------------------------------
bool               CStudent::operator <                    ( const CStudent  & x ) const
{
  return m_Name < x . m_Name;
}
//-------------------------------------------------------------------------------------------------
ostream      & operator <<                                 ( ostream         & os,
                                                             const CStudent  & x )
{
  os << x . m_Name << " " << x . m_PA1 << ", " << x . m_UOS << endl;
  return os;
}
//-------------------------------------------------------------------------------------------------
istream      & operator >>                                 ( istream         & is,
                                                             CStudent        & x )
{
  string n;
  char pa1, uos, x2;
  // read the input into temporary variables. Once read, teh variables
  // will be copied into the instance. If reading fails for any reason,
  // the initial object content remains consistent (unchanged).
  is >> n >> pa1 >> x2 >> uos;

  if ( is . fail ()
       || pa1 < 'A' || pa1 > 'F'
       || x2 != ','
       || uos < 'A' || uos > 'F' )
  {
    is . setstate ( ios::failbit );
    return is;
  }
  x . m_Name = n;
  x . m_PA1 = pa1;
  x . m_UOS = uos;
  return is;
}
//=================================================================================================
template <typename T>
void               PrintAll                                ( const vector<T> & list )
{
  // C++98 version, note the typename keyword. This is needed here to inform the compiler
  // that vector<T>::const_iterator is a type name.
  for ( typename vector<T>::const_iterator it = list . begin (); it != list . end (); ++it )
    cout << *it;
  // C++0x11 version, requires C++-0x11 compatible compiler
  // for ( const auto & x : list )
  //   cout << x;
}
//-------------------------------------------------------------------------------------------------------
int main ( void )
{
  ifstream ifs ( "list.txt" );
  vector <CStudent> list;

  // No default constructor in CStudent, create "some" instance and overwrite it
  for ( CStudent s ( "", 'A', 'A' ); ifs >> s; )
    list . push_back ( s );

  if ( ! ifs . eof () )
  {
    cout << "Error reading input file" << endl;
    return 1;
  }

  cout << "Unsorted" << endl;
  PrintAll ( list );

  cout << "Sort by name" << endl;
  // sort using operator < in CStudent
  sort ( list . begin (), list . end () );
  PrintAll ( list );

  cout << "Sort by PA1, ascending" << endl;
  // sort using the comparator - function object
  sort ( list . begin (), list . end (), StudentComparator ( StudentComparator::PA1, StudentComparator::ASCENDING ) );
  PrintAll ( list );

  cout << "Sort by UOS, descending" << endl;
  sort ( list . begin (), list . end (), StudentComparator ( StudentComparator::UOS, StudentComparator::DESCENDING ) );
  PrintAll ( list );

  return 0;
}
