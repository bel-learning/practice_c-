/* Generic class implementing a set of elements, the element type is generic parameter of the template.
 * The implementation requires comparisons (operators < and ==) for T and correct copy cons/op=/delete
 * for T.
 *
 * This implementation avoids both uses double star pointers and recursion. The readability without double
 * star pointers is questionable. We must remeber our parent node and distinguish three cases:
 *  - there is no parent (i.e. we are inserting/deleting in root),
 *  - there is a parent node and our inserted / deleted node is a left child,
 *  - there is a parent node and our inserted / deleted node is a right child.
 *
 * The tests are not difficult, however, they are annoying.
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
using namespace std;

template <class T>
class CBST
{
public:
  CBST(void);
  CBST(const CBST<T> &src);
  ~CBST(void);
  CBST<T> &operator=(const CBST<T> &src);
  bool Insert(const T &x);
  bool Delete(const T &x);
  bool IsSet(const T &x) const;
  int Size(void) const;
  void Dump(ostream &os) const;

private:
  struct TNode
  {
    TNode(const T &x);
    ~TNode(void);
    TNode *Clone(void) const;
    void Print(ostream &os,
               bool first) const;
    void Dump(ostream &os,
              string prefix,
              string childern) const;
    TNode *m_L;
    TNode *m_R;
    T m_Val;
  };

  TNode *m_Root;
  int m_Count;
  template <class X>
  friend ostream &operator<<(ostream &os,
                             const CBST<X> &x);
};
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>::CBST(void)
    : m_Root(nullptr),
      m_Count(0)
{
}
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>::CBST(const CBST<T> &src)
    : m_Root(src.m_Root ? src.m_Root->Clone() : nullptr),
      m_Count(src.m_Count)
{
}
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>::~CBST(void)
{
  delete m_Root;
}
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T> &CBST<T>::operator=(const CBST<T> &src)
{
  if (this != &src)
  {
    delete m_Root;
    if (src.m_Root)
      m_Root = src.m_Root->Clone();
    else
      m_Root = nullptr;
    m_Count = src.m_Count;
  }
  return *this;
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool CBST<T>::Insert(const T &x)
{
  TNode *node = m_Root, *parent = nullptr;
  bool left;

  while (node)
  {
    if (x == node->m_Val)
      return false;
    if (x < node->m_Val)
    {
      parent = node;
      node = node->m_L;
      left = true;
    }
    else
    {
      parent = node;
      node = node->m_R;
      left = false;
    }
  }
  node = new TNode(x);
  /* distinguish the three cases
   */
  if (parent)
  {
    if (left)
      parent->m_L = node;
    else
      parent->m_R = node;
  }
  else
    m_Root = node;
  m_Count++;
  return true;
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool CBST<T>::Delete(const T &x)
{
  TNode *node = m_Root, *parent = nullptr, *toDel;
  bool left;

  while (node)
  {
    if (x == node->m_Val)
    {
      toDel = node;
      if (node->m_L && node->m_R)
      {
        parent = node;
        left = true;
        node = node->m_L;
        while (node->m_R)
        {
          parent = node;
          node = node->m_R;
          left = false;
        }

        toDel->m_Val = node->m_Val;
        toDel = node;
      }

      node = toDel->m_L ? toDel->m_L : toDel->m_R;

      /* and again distunguish three possible parents
       */
      if (parent)
      {
        if (left)
          parent->m_L = node;
        else
          parent->m_R = node;
      }
      else
        m_Root = node;

      toDel->m_L = toDel->m_R = nullptr;
      delete toDel;
      m_Count--;
      return true;
    }
    if (x < node->m_Val)
    {
      parent = node;
      node = node->m_L;
      left = true;
    }
    else
    {
      parent = node;
      node = node->m_R;
      left = false;
    }
  }
  return false;
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool CBST<T>::IsSet(const T &x) const
{
  TNode *n = m_Root;

  while (n)
  {
    if (x == n->m_Val)
      return true;
    if (x < n->m_Val)
      n = n->m_L;
    else
      n = n->m_R;
  }
  return false;
}
//-------------------------------------------------------------------------------------------------
template <class T>
int CBST<T>::Size(void) const
{
  return m_Count;
}
//-------------------------------------------------------------------------------------------------
template <class T>
void CBST<T>::Dump(ostream &os) const
{
  if (m_Root)
    m_Root->Dump(os, "", "  ");
}
//=================================================================================================
template <class T>
CBST<T>::TNode::TNode(const T &x)
    : m_L(nullptr),
      m_R(nullptr),
      m_Val(x)
{
}
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>::TNode::~TNode(void)
{
  delete m_L;
  delete m_R;
}
//-------------------------------------------------------------------------------------------------
template <class T>
typename CBST<T>::TNode *CBST<T>::TNode::Clone(void) const
{
  TNode *n;

  n = new TNode(m_Val);
  if (m_L)
    n->m_L = m_L->Clone();
  if (m_R)
    n->m_R = m_R->Clone();
  return n;
}
//-------------------------------------------------------------------------------------------------
template <class T>
void CBST<T>::TNode::Print(ostream &os,
                           bool first) const
{
  if (m_L)
  {
    m_L->Print(os, first);
    first = false;
  }
  if (!first)
    os << ", ";

  os << m_Val;
  if (m_R)
    m_R->Print(os, false);
}
//-------------------------------------------------------------------------------------------------
template <class T>
void CBST<T>::TNode::Dump(ostream &os,
                          string prefix,
                          string children) const
{
  os << prefix << "+-" << m_Val << endl;
  if (m_L)
    m_L->Dump(os, prefix + children, m_R ? "| " : "  ");
  if (m_R)
    m_R->Dump(os, prefix + children, "  ");
}
//-------------------------------------------------------------------------------------------------
template <class T>
ostream &operator<<(ostream &os,
                    const CBST<T> &x)
{
  os << "[";
  if (x.m_Root)
    x.m_Root->Print(os, true);
  os << "]";
  return os;
}
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  CBST<int> a;

  a.Insert(100);
  a.Insert(120);
  a.Insert(80);
  a.Insert(85);
  a.Insert(90);
  a.Insert(88);
  a.Insert(150);

  a.Dump(cout);

  cout << a << endl;

  a.Delete(100);

  a.Dump(cout);
  cout << a << endl;
  a.Insert(100);
  a.Insert(120);
  a.Insert(80);
  a.Insert(85);
  a.Insert(90);
  a.Insert(88);

  a.Dump(cout);

  cout << a << endl;

  a.Delete(100);

  a.Dump(cout);
  cout << a << endl;

  a.Delete(85);
  a.Dump(cout);
  a.Delete(120);
  a.Delete(80);
  a.Delete(90);
  a.Delete(150);
  a.Delete(88);

  a.Dump(cout);
  cout << a << endl;
  return 0;
}
//-------------------------------------------------------------------------------------------------
