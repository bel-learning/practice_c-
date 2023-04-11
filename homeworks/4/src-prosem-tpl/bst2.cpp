/* Generic class implementing a set of elements, the element type is generic parameter of the template.
 * The implementation requires comparisons (operators < and ==) for T and correct copy cons/op=/delete
 * for T.
 *
 * The implementation uses double star pointers and recursion
 * to implement insert/delete operations.
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;


template <class T>
class CBST
{
  public:
                             CBST                          ( void );
                             CBST                          ( const CBST<T>   & src );
                            ~CBST                          ( void );
    CBST<T>                & operator =                    ( const CBST<T>   & src );
    bool                     Insert                        ( const T         & x );
    bool                     Delete                        ( const T         & x );
    bool                     IsSet                         ( const T         & x ) const;
    int                      Size                          ( void ) const;
    void                     Dump                          ( ostream         & os ) const;

  private:
    struct TNode
    {
                             TNode                         ( const T         & x );
                            ~TNode                         ( void );
      TNode                * Clone                         ( void ) const;
      void                   Print                         ( ostream         & os,
                                                             bool              first ) const;
      void                   Dump                          ( ostream         & os,
                                                             string            prefix,
                                                             string            childern ) const;
      TNode                * m_L;
      TNode                * m_R;
      T                      m_Val;
    };

    TNode                  * m_Root;
    int                      m_Count;
    bool                     addNode                       ( TNode          ** node,
                                                               const T         & x );
    bool                     delNode                       ( TNode          ** node,
                                                             const T         & x );
    template <class X>
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CBST<X>   & x );
};
//-------------------------------------------------------------------------------------------------
template <class T>
                   CBST<T>::CBST                           ( void )
  : m_Root ( nullptr ),
    m_Count ( 0 )
{
}
//-------------------------------------------------------------------------------------------------
template <class T>
                   CBST<T>::CBST                           ( const CBST<T>   & src )
  : m_Root ( src . m_Root ? src . m_Root -> Clone () : nullptr ),
    m_Count ( src . m_Count )
{
  /* Copy constructor, deep copy of the tree. The recursion is achieved using Clone () method in the node
   */
}
//-------------------------------------------------------------------------------------------------
template <class T>
                            CBST<T>::~CBST                 ( void )
{
  /* Tree delete. The recursion is inherently implemented in the destructors.
   */
  delete m_Root;
}
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>          & CBST<T>::operator =                     ( const CBST<T>   & src )
{
  if ( this != &src )
  {
    /* the usual destructor/copy constructor way
     */
    delete m_Root;
    if ( src . m_Root )
      m_Root = src . m_Root -> Clone ();
    else
      m_Root = nullptr;
    m_Count = src . m_Count;
  }
  return *this;
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool               CBST<T>::Insert                         ( const T         & x )
{
  if ( addNode ( &m_Root, x ) )
  {
    /* success - increment
     */
    m_Count ++;
    return true;
  }
  return false;
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool               CBST<T>::Delete                         ( const T         & x )
{
  if ( delNode ( &m_Root, x ) )
  {
    /* successful delete
     */
    m_Count --;
    return true;
  }
  return false;
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool               CBST<T>::IsSet                          ( const T         & x ) const
{
  TNode * n = m_Root;

  while ( n )
  {
    if ( x == n -> m_Val )
      return true;
    if ( x < n -> m_Val )
      n = n -> m_L;
    else
      n = n -> m_R;
  }
  return false;
}
//-------------------------------------------------------------------------------------------------
template <class T>
int                CBST<T>::Size                           ( void ) const
{
  return m_Count;
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool               CBST<T>::addNode                        ( TNode          ** node,
                                                             const T         & x )
{
  TNode * n;

  /*  The recursion traverses the tree and finds the position in the tree, where the new value
   * belongs to. However, we shall remember the position "where to put the address of the newly added
   * node". Thus, only the address of the pointer is insufficient, we need to use ** pointer.
   *
   * Please note this works fine for the root node to.
   */

  if ( ! *node )
  {
    /* We have reached the leaf. The branch does not continue in our direction, i.e. the inserted value
     * was not present in the tree. Create the new node and replace the nullptr with the newly created node.
     * Return success.
     */
    n = new TNode ( x );
   *node = n;
    return true;
  }

  /* Duplicate (value already present)
   */
  if ( x == (*node) -> m_Val )
    return false;
  /* Recursive traversal of the left/right subtree
   */
  if ( x < (*node) -> m_Val )
    return addNode ( &(*node) -> m_L, x );
  else
    return addNode ( &(*node) -> m_R, x );
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool               CBST<T>::delNode                        ( TNode          ** node,
                                                             const T         & x )

{
  TNode * toDel;

  /* Recursive search/delete. We have to find the node to delete, however, we shall remember
   * the place "where the delete node was referenced". We need to replace the adderss (e.g. with nullptr).
   * Thus again, double star pointer is used.
   */

  /* We reached end of the tree (leaf), however, the searched value was not present.
   */
  if ( ! *node ) return false;
  if ( x == (*node) -> m_Val )
  {
    /* we found the value
     */
    toDel = *node;

    if ( toDel -> m_L && toDel -> m_R )
    {
      /* The node to delete (toDel) has two childs. We must find the rightmost node in the left subtree (node).
       * (Equivalently, we could find the leftmost node in the right subtree). Copy the value from the node (node)
       * to the (toDel) node. Now, we need to delete the (node) node, which has at most one child.
       */
      node = &toDel -> m_L;
      while ( (*node) -> m_R )
        node = &(*node) -> m_R;

      toDel -> m_Val = (*node) -> m_Val;
      toDel = *node;
    }

    /* The node to delete has at most one child. Replace the link to our node (toDel) with the link to the child.
     * The child may be either left or right, or there may be zero childs. If both childs are nullptr, we must replace
     * the link to (toDel) node with nullptr.
     */

    if ( toDel -> m_L )
      *node = toDel -> m_L;
    else
      *node = toDel -> m_R;

    /* Now cut the links to our childs. If we deleted the node without links disconnected, the recursive delete
     * would destroy our subtree (which is still in use).
     */
    toDel -> m_L = toDel -> m_R = nullptr;
    delete toDel;
    return true;
  }
  /* Traverse the tree
   */
  if ( x < (*node) -> m_Val )
    return delNode ( &(*node) -> m_L, x );
  else
    return delNode ( &(*node) -> m_R, x );
}
//-------------------------------------------------------------------------------------------------
template <class T>
void               CBST<T>::Dump                           ( ostream         & os ) const
{
  if ( m_Root )
    m_Root -> Dump ( os, "", "  " );
}
//=================================================================================================
template <class T>
                   CBST<T>::TNode::TNode                   ( const T         & x )
  : m_L ( nullptr ),
    m_R ( nullptr ),
    m_Val ( x )
{
}
//-------------------------------------------------------------------------------------------------
template <class T>
                   CBST<T>::TNode::~TNode                  ( void )
{
  delete m_L;
  delete m_R;
}
//-------------------------------------------------------------------------------------------------
template <class T>
typename CBST<T>::TNode  * CBST<T>::TNode::Clone           ( void ) const
{
  TNode  * n;

  n = new TNode ( m_Val );
  if ( m_L )
    n -> m_L = m_L -> Clone ();
  if ( m_R )
    n -> m_R = m_R -> Clone ();
  return n;
}
//-------------------------------------------------------------------------------------------------
template <class T>
void               CBST<T>::TNode::Print                   ( ostream         & os,
                                                             bool              first ) const
{
  if ( m_L )
  {
    m_L -> Print ( os, first );
    first = false;
  }
  if ( !first ) // suppress comma before the first value printed
    os << ", ";

  os << m_Val;
  if ( m_R )
    m_R -> Print ( os, false );
}
//-------------------------------------------------------------------------------------------------
template <class T>
void               CBST<T>::TNode::Dump                    ( ostream         & os,
                                                             string            prefix,
                                                             string            childern ) const
{
  os << prefix << "+-" << m_Val << endl;
  if ( m_L )
    m_L -> Dump ( os, prefix + childern, m_R ? "| " : "  " );
  if ( m_R )
    m_R -> Dump ( os, prefix + childern, "  " );
}
//-------------------------------------------------------------------------------------------------
template <class T>
ostream         & operator <<                              ( ostream         & os,
                                                             const CBST<T>   & x )
{
  os << "[";
  if ( x . m_Root )
    x . m_Root -> Print ( os, true );
  os << "]";
  return os;
}
//-------------------------------------------------------------------------------------------------
int main ( int argc, char * argv [] )
{
  CBST<int>  a;

  a . Insert ( 100 );
  a . Insert ( 120 );
  a . Insert ( 80 );
  a . Insert ( 85 );
  a . Insert ( 90 );
  a . Insert ( 88 );
  a . Insert ( 150 );

  a . Dump ( cout );

  cout << a << endl;

  a . IsSet ( 150 );

  a . Delete ( 100 );

  a . Dump ( cout );
  cout << a << endl;
  a . Insert ( 100 );
  a . Insert ( 120 );
  a . Insert ( 80 );
  a . Insert ( 85 );
  a . Insert ( 90 );
  a . Insert ( 88 );

  a . Dump ( cout );

  cout << a << endl;

  a . Delete ( 100 );

  a . Dump ( cout );
  cout << a << endl;

  a . Delete ( 85 );
  a . Dump ( cout );
  a . Delete ( 120 );
  a . Delete ( 80 );
  a . Delete ( 90 );
  a . Delete ( 150 );
  a . Delete ( 88 );

  a . Dump ( cout );
  cout << a << endl;

  CBST<string>  b;

  b . Insert ( "this is a test" );
  cout << b << endl;

  return 0;
}
//-------------------------------------------------------------------------------------------------
