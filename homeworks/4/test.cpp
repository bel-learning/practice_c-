#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */
#define COUNT 10;

namespace my
{
 
  class string
  {
  public:
    char *_data;
    size_t _size;
    size_t _cap;
    string() : _data(nullptr),  _size(0), _cap(0), _ref(1) {};
    ~string() {
      if(_ref == 0)
        delete [] _data;
    }
    
    void increaseRef() {
      (_ref)++;
    }
    void decreaseRef() {
      (_ref)--;
    }
    int getRef() {
      return _ref;
    }
    private:
      int _ref;

  };


  class sstring {
    private:
    string * m_Ptr;
    public:

    sstring(const char * str = ""): m_Ptr(nullptr) {
      m_Ptr = new string;
      size_t len = strlen(str);
      size_t cap = len+1;
      m_Ptr->_data = new char[cap];
      m_Ptr->_cap = cap;
      m_Ptr->_size = len;
      memcpy(m_Ptr->_data, str, cap);
      m_Ptr->_data[len] = '\0';
    }
    sstring(const sstring & other) {
      m_Ptr = other.m_Ptr;
      m_Ptr->increaseRef();
    }
    sstring & operator = (const sstring &other)
    {
      if(this == &other) return *this;
      deleteData();
      m_Ptr = other.m_Ptr;
      m_Ptr->increaseRef();
      return *this;
    }
    ~sstring() {
      deleteData();
    }
    

    bool operator==(const my::sstring &other) const
    {
      if (strcmp(m_Ptr->_data, other.m_Ptr->_data) == 0)
        return true;
      return false;
    }
    bool operator!=(const my::sstring &other) const
    {
      return !(*this == other);
    }
    bool operator>(const my::sstring &other) const
    {
      return (strcmp(m_Ptr->_data, other.m_Ptr->_data) > 0);
    }
    bool operator<(const my::sstring &other) const
    {
      return (strcmp(m_Ptr->_data, other.m_Ptr->_data) < 0);
    }

    friend ostream & operator << (ostream & out, const  sstring & me) {
        out << me.m_Ptr->_data;
        return out;
    }

    void deleteData() {
      m_Ptr->decreaseRef();
      if(m_Ptr->getRef() == 0) {
        delete m_Ptr;
      }
    }
  
  };

  template <typename T>
  class vector
  {
    
  public:
    T *_arr;
    size_t _cap;
    size_t _size;
    vector(): _arr(nullptr),  _cap(0) ,_size(0), _ref(1) {}
    ~vector()
    {
      if(_ref == 0)
        delete [] _arr;
    }
    
    void increaseRef() {
      _ref++;
    }
    void decreaseRef() {
      _ref--;
    }
    int getCount() {
      return _ref;
    }

    

   private:
   int _ref;
  };
  
  
   class SNode {
    public:
        my::sstring val;
        SNode * left;
        SNode * right;
        SNode(): left(nullptr), right(nullptr) {};
        SNode(const my::sstring & str): val(str), left(nullptr), right(nullptr) {}; 
    };

    class SBST {
    public:
        SNode* root;
        int _ref;
        SBST() : root(nullptr), _ref(1) {}
        ~SBST() {
            if(_ref == 0)
                freeItems(root);
        }
    
   
        void freeItems(SNode * current) {
            if(current == nullptr) {
            return;
            }
            freeItems(current->left);
            freeItems(current->right);
            delete current;
        }
        SNode * insert(const my::sstring & val) {
            SNode * newNode = new SNode(val);
            if (root == NULL) {
                root = newNode;
                return newNode;
            }
            SNode* cur = root;
            while (true) {
                if (val < cur->val) {
                    if (cur->left == NULL) {
                        cur->left = newNode;
                        return newNode;
                    }
                    cur = cur->left;
                } else {
                    if (cur->right == NULL) {
                        cur->right = newNode;
                        return newNode;
                    }
                    cur = cur->right;
                }
            }
        }
    
        SNode * search(const my::sstring & val) {
            SNode* cur = root;
            while (cur != NULL) {
                if (val == cur->val) {
                    return cur;
                } else if (val < cur->val) {
                    cur = cur->left;
                } else {
                    cur = cur->right;
                }
            }
            return nullptr;
        }
        SNode * stringToNode(const my::sstring & val) {
            SNode * searchFound = search(val);
            if(searchFound == nullptr) {
                return insert(val);
            }
            else {
                // cout << "Returning found string node" << endl;
                return searchFound;
            }
         }

    };


  template<typename T>
  class svector {
    public:
    my::vector<T> * m_Ptr;
    svector(): m_Ptr(new my::vector<T>) {}
    svector(const svector & other) {
      m_Ptr = other.m_Ptr;
      m_Ptr->increaseRef();
    }
    svector & operator = (const svector & other) {
      if(this == &other) return *this;
      deleteValue();

      m_Ptr = other.m_Ptr;
      m_Ptr->increaseRef();

      return *this;
    }
    ~svector() {
      deleteValue();
    }
    size_t size() const
    {
      return m_Ptr->_size;
    }
    T * begin() const {
      return m_Ptr->_arr;
    }
    
    void push_back(const T & data)
    {
      detach();

      if (m_Ptr->_size >= m_Ptr->_cap)
      {
        size_t old_cap = m_Ptr->_cap;
        if(m_Ptr->_cap == 0)
          m_Ptr->_cap = 1;
        T * temp = new T[m_Ptr->_cap * 2];
        // copying old array elements to new array
        
        copy(m_Ptr->_arr, m_Ptr->_arr + old_cap, temp);
        // for(size_t i = 0; i < _size; i++) {
        //   temp[i] = _arr[i];
        // }
        delete[] m_Ptr->_arr;

        m_Ptr->_arr = temp;
        m_Ptr->_cap *= 2;
      }
      m_Ptr->_arr[m_Ptr->_size] = data;
     
      m_Ptr->_size++;
    }
    void detach() {
      if(m_Ptr->getCount() == 1)
        return;
      deleteValue();
      my::vector<T> * tmp = new my::vector<T>();
      tmp->_arr = new T[m_Ptr->_cap];
      tmp->_size = m_Ptr->_size;
      tmp->_cap = m_Ptr->_cap;

      copy(m_Ptr->_arr,m_Ptr->_arr + m_Ptr->_cap, tmp->_arr);

      m_Ptr = tmp;
    }
    void deleteValue() {
      m_Ptr->decreaseRef();
      if(m_Ptr->getCount() == 0) 
        delete m_Ptr;
    }
  };


}

my::SBST string_storage;

class CMail
{
public:
  CMail() : _from(nullptr), _to(nullptr), _body(nullptr){};
  
  CMail(const char *from,
        const char *to,
        const char *body)
      {
        _from = string_storage.stringToNode(my::sstring(from));
        _to = string_storage.stringToNode(my::sstring(to));
        _body = string_storage.stringToNode(my::sstring(body));
      };

  CMail(const CMail &x) {
    _from = x._from;
    _to = x._to;
    _body = x._body;
  };
  
  CMail & operator = (const CMail &x) {
    if(this == &x) return *this;
    _to = x._to;
    _from = x._from;
    _body = x._body;
    return *this;
  };
  
  bool operator==(const CMail &x) const
  {
    if (x._body != _body)
      return false;
    if (x._from != _from)
      return false;
    if (x._to != _to)
      return false;
    return true;
  };
  friend ostream &operator<<(ostream &os,
                             const CMail &m){
      
      os << "From: " << m._from->val << ", To: " << m._to->val << ", Body: " << m._body->val;
      return os;
  };
  my::sstring getFrom() const
  {
    return _from->val;
  }
  my::sstring getTo() const
  {
    return _to->val;
  }
  my::sstring getBody() const
  {
    return _body->val;
  }

private:
  // todo
  my::SNode * _from;
  my::SNode * _to;
  my::SNode * _body;
};

class CMailIterator
{
public:
  CMailIterator(const my::svector<CMail> & vec) {
    _data = new CMail[vec.size()];
    copy(vec.begin(), vec.begin() + vec.size(), _data);
    _index = 0;
    _size = vec.size();
  };
  
  CMailIterator operator = (const CMailIterator & other) {
    return CMailIterator(other);
  }
  ~CMailIterator() {
    delete [] _data;
  }
  
  explicit operator bool(void) const {
    return _index < _size;
  };
  bool operator ! (void) const {
    return _index >= _size;
  };
  const CMail & operator*(void) const {
    return _data[_index];
  };
  CMail & operator * (void) {
    return _data[_index];
  };
  CMailIterator &operator++(void){
    _index++;
    return *this;
  };
  

private:
  // todo
  CMail * _data;
  size_t _size;
  size_t _index;
};

struct Node
  {
    Node *left;
    Node *right;
    my::sstring email;
    my::svector<CMail> inbox;
    my::svector<CMail> outbox;
    Node(): left(nullptr), right(nullptr) {};
  };

class BST
{
  
public:
  Node *root;

  BST(void) : root(nullptr), _ref(1) {};
  BST(Node * new_root) : root(new_root), _ref(1) {};

  ~BST() {
    freeItems(root);
  }  

  void freeItems(Node * current) {
    if(current == nullptr) {
      return;
    }
    freeItems(current->left);
    freeItems(current->right);
    delete current;
  }
  void Insert(const CMail & x)
  {
    Node *node = root;

    Node *parent = nullptr;
    bool exists = false;
    bool left = false;
    my::sstring sender = x.getFrom();
    while (node != nullptr)
    {
      if (sender == node->email)
      {
        node->outbox.push_back(x);
        exists = true;
        break;
      }
      else if (sender < node->email)
      {
        parent = node;
        node = node->left;
        left = true;
      }
      else
      {
        parent = node;
        node = node->right;
        left = false;
      }
    }

    if (!exists) {
      node = new Node;
      node->email = sender;
      node->outbox.push_back(x);
      if (parent)
        if (left)
          parent->left = node;
        else
          parent->right = node;
      else
        root = node;
    }
   
    node = root;
    parent = nullptr;
    left = false;
    exists = false;
    
    const my::sstring receiver = x.getTo();

    while (node != nullptr)
    {
      if (receiver == node->email)
      {
        node->inbox.push_back(x);
        exists = true;
        break;
      }
      else if (receiver < node->email)
      {
        parent = node;
        node = node->left;
        left = true;
      }
      else
      {
        parent = node;
        node = node->right;
        left = false;
      }
    }
    if (!exists) {
      node = new Node;
      node->email = receiver;
      node->inbox.push_back(x);
      if (parent)
        if (left)
          parent->left = node;
        else
          parent->right = node;
      else
        root = node;
    }
  }

  Node *Find(const my::sstring &email) const
  {
    Node *current = root;
    while (current)
    {
      if (current->email == email)
      {
        return current;
      }
      else if (email < current->email)
      {
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    return nullptr;
  }

  void print2DUtil(Node* root, int space) const
  {
      // Base case
      if (root == NULL)
          return;
  
      // Increase distance between levels
      space += COUNT;
  
      // Process right child first
      print2DUtil(root->right, space);
  
      // Print current node after space
      // count
      cout << endl;
      for (int i = 10; i < space; i++)
          cout << " ";
      cout << root->email << endl;
      // Process left child
      print2DUtil(root->left, space);
  }
  
  // Wrapper over print2DUtil()
  void print2D() const
  {
      // Pass initial space count as 0
      print2DUtil(root, 0);
  }

  Node * populateTree() {
    return copyTree(root);
  }
  Node * copyTree(Node * root) {
    if(root == nullptr) {
      return nullptr;
    }
    Node * newRoot = createNodeFrom(root);     
    newRoot->left = copyTree(root->left); 
    newRoot->right = copyTree(root->right); 
    return newRoot;
  }
 
  Node * createNodeFrom(Node * other) {
    Node * new_node = new Node();
    new_node->email = other->email;
    new_node->inbox = other->inbox;
    new_node->outbox = other->outbox;
    new_node->left = other->left;
    new_node->right = other->right;
    return new_node;
  }

  void increaseRef() {
    (_ref)++;
  };
  void decreaseRef() {
    (_ref)--;
  };
  
  int getCount() {
    return _ref;
  }
  private:
  int _ref;

};

class BBST {
  public:
    BST * m_Ptr;
    BBST() : m_Ptr(new BST) {};
    BBST(const BBST & other) {
      m_Ptr = other.m_Ptr;
      m_Ptr->increaseRef();
    };
    
    ~BBST() {
      deleteData();
    }
    BBST & operator = (const BBST & other) {
      if(this == &other) return *this;
      deleteData();
      m_Ptr = other.m_Ptr;
      m_Ptr->increaseRef();
      return *this;
    }
    BST * getValue() {
      return m_Ptr;
    }
    void detach() {
      if(m_Ptr->getCount() == 1)
        return;

      Node * new_root = m_Ptr->populateTree();
      BST * new_tree = new BST(new_root);

      deleteData();

      m_Ptr = new_tree;
    }
    void deleteData() {
      m_Ptr->decreaseRef();
      if(m_Ptr->getCount() == 0) {
        delete m_Ptr;
      }
    }

};

class CMailServer
{
public:
  CMailServer(void) {};
  CMailServer(const CMailServer &src) {
    tree = src.tree;
  };

  CMailServer &operator=(const CMailServer &src){
    if(this == &src) return *this;
    tree = src.tree;
    return *this;
  };
   
  ~CMailServer(void){
  };
  void sendMail(const CMail &m)
  {
    tree.detach();
    tree.m_Ptr->Insert(m);
    // cout << "------------------------------" << endl;
  };
  CMailIterator outbox(const char *email) const {
    my::sstring searchFor(email);
    Node * el = tree.m_Ptr->Find(searchFor);
    if(el == nullptr) 
      return CMailIterator(my::svector<CMail>());
    return CMailIterator(el->outbox);
  };

  CMailIterator inbox(const char *email) const {
    my::sstring searchFor(email);
    Node * el = tree.m_Ptr->Find(searchFor);

    if(el == nullptr) 
      return CMailIterator(my::svector<CMail>());

    return CMailIterator(el->inbox);
  };
  
private:
  // todo
  
  BBST tree;
};

#ifndef __PROGTEST__
bool matchOutput(const CMail &m,
                 const char *str)
{
  ostringstream oss;
  oss << m;
  return oss.str() == str;
}

int main(void)
{
  char from[100], to[100], body[1024];
  assert(CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
  assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));
  CMailServer s0;
  s0.sendMail(CMail("john", "peter", "some important mail"));
  strncpy(from, "john", sizeof(from));
  strncpy(to, "thomas", sizeof(to));
  strncpy(body, "another important mail", sizeof(body));

  s0.sendMail(CMail(from, to, body));
  strncpy(from, "john", sizeof(from));
  strncpy(to, "alice", sizeof(to));
  strncpy(body, "deadline notice", sizeof(body));
  s0.sendMail(CMail(from, to, body));
  s0.sendMail(CMail("alice", "john", "deadline confirmation"));
  s0.sendMail(CMail("peter", "alice", "PR bullshit"));
  CMailIterator i0 = s0.inbox("alice");
  cout << "Comparing iterator" << endl;
  assert(i0 && *i0 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i0, "From: john, To: alice, Body: deadline notice"));
  assert(++i0 && *i0 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i0, "From: peter, To: alice, Body: PR bullshit"));
  cout << "Moving i0" << endl;
  assert(!++i0);
  cout << "Here" << endl;
  CMailIterator i1 = s0.inbox("john");
  assert(i1 && *i1 == CMail("alice", "john", "deadline confirmation"));
  assert(matchOutput(*i1, "From: alice, To: john, Body: deadline confirmation"));
  assert(!++i1);

  CMailIterator i2 = s0.outbox("john");
  assert(i2 && *i2 == CMail("john", "peter", "some important mail"));
  assert(matchOutput(*i2, "From: john, To: peter, Body: some important mail"));
  assert(++i2 && *i2 == CMail("john", "thomas", "another important mail"));
  assert(matchOutput(*i2, "From: john, To: thomas, Body: another important mail"));
  assert(++i2 && *i2 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i2, "From: john, To: alice, Body: deadline notice"));
  assert(!++i2);
  cout << "Checking no outbox mails" << endl;

  CMailIterator i3 = s0.outbox("thomas");
  assert(!i3);

  CMailIterator i4 = s0.outbox("steve");
  assert(!i4);

  CMailIterator i5 = s0.outbox("thomas");
  s0.sendMail(CMail("thomas", "boss", "daily report"));
  assert(!i5);

  CMailIterator i6 = s0.outbox("thomas");
  assert(i6 && *i6 == CMail("thomas", "boss", "daily report"));
  assert(matchOutput(*i6, "From: thomas, To: boss, Body: daily report"));
  assert(!++i6);

  CMailIterator i7 = s0.inbox("alice");
  s0.sendMail(CMail("thomas", "alice", "meeting details"));
  assert(i7 && *i7 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i7, "From: john, To: alice, Body: deadline notice"));
  assert(++i7 && *i7 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i7, "From: peter, To: alice, Body: PR bullshit"));
  assert(!++i7);

  CMailIterator i8 = s0.inbox("alice");
  assert(i8 && *i8 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i8, "From: john, To: alice, Body: deadline notice"));
  assert(++i8 && *i8 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i8, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i8 && *i8 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i8, "From: thomas, To: alice, Body: meeting details"));
  assert(!++i8);

  cout << "Copying to s1" << endl;
  CMailServer s1(s0);
  
  s0.sendMail(CMail("joe", "alice", "delivery details"));
  s1.sendMail(CMail("sam", "alice", "order confirmation"));
  cout << "Going to access s0's alice's inbox" << endl;
  CMailIterator i9 = s0.inbox("alice");
  assert(i9 && *i9 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i9, "From: john, To: alice, Body: deadline notice"));
  assert(++i9 && *i9 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i9, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i9 && *i9 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i9, "From: thomas, To: alice, Body: meeting details"));
  assert(++i9 && *i9 == CMail("joe", "alice", "delivery details"));
  assert(matchOutput(*i9, "From: joe, To: alice, Body: delivery details"));
  assert(!++i9);
  cout << "Checking alice inbox, expecting 4 email inside" << endl;
  CMailIterator i10 = s1.inbox("alice");
  assert(i10 && *i10 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i10, "From: john, To: alice, Body: deadline notice"));
  assert(++i10 && *i10 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i10, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i10 && *i10 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i10, "From: thomas, To: alice, Body: meeting details"));
  assert(++i10 && *i10 == CMail("sam", "alice", "order confirmation"));
  assert(matchOutput(*i10, "From: sam, To: alice, Body: order confirmation"));
  assert(!++i10);
  cout << "Done" << endl;
  CMailServer s2;
  s2.sendMail(CMail("alice", "alice", "mailbox test"));
  CMailIterator i11 = s2.inbox("alice");
  assert(i11 && *i11 == CMail("alice", "alice", "mailbox test"));
  assert(matchOutput(*i11, "From: alice, To: alice, Body: mailbox test"));
  assert(!++i11);
  cout << "Assignment operator" << endl;
  s2 = s0;
  s0.sendMail(CMail("steve", "alice", "newsletter"));
  s2.sendMail(CMail("paul", "alice", "invalid invoice"));
  cout << "Sent mail" << endl;
  CMailIterator i12 = s0.inbox("alice");
  assert(i12 && *i12 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i12, "From: john, To: alice, Body: deadline notice"));
  assert(++i12 && *i12 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i12, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i12 && *i12 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i12, "From: thomas, To: alice, Body: meeting details"));
  assert(++i12 && *i12 == CMail("joe", "alice", "delivery details"));
  assert(matchOutput(*i12, "From: joe, To: alice, Body: delivery details"));
  assert(++i12 && *i12 == CMail("steve", "alice", "newsletter"));
  assert(matchOutput(*i12, "From: steve, To: alice, Body: newsletter"));
  assert(!++i12);

  CMailIterator i13 = s2.inbox("alice");
  assert(i13 && *i13 == CMail("john", "alice", "deadline notice"));
  assert(matchOutput(*i13, "From: john, To: alice, Body: deadline notice"));
  assert(++i13 && *i13 == CMail("peter", "alice", "PR bullshit"));
  assert(matchOutput(*i13, "From: peter, To: alice, Body: PR bullshit"));
  assert(++i13 && *i13 == CMail("thomas", "alice", "meeting details"));
  assert(matchOutput(*i13, "From: thomas, To: alice, Body: meeting details"));
  assert(++i13 && *i13 == CMail("joe", "alice", "delivery details"));
  assert(matchOutput(*i13, "From: joe, To: alice, Body: delivery details"));
  assert(++i13 && *i13 == CMail("paul", "alice", "invalid invoice"));
  assert(matchOutput(*i13, "From: paul, To: alice, Body: invalid invoice"));
  assert(!++i13);

  CMailServer s4;
  s4.sendMail(CMail("e", "b", "12"));
  s4.sendMail(CMail("a", "a", "12"));
  CMailIterator ia1 = s4.outbox("a");
  cout << *ia1 << endl;

  s4.sendMail(CMail("a", "c", "12"));
  ++ia1;
  s4.sendMail(CMail("a", "d", "12"));
  CMailServer s5 = s4;
  CMailServer s6(s4);
  s4.sendMail(CMail("k", "e", "12"));
  s6 = s4;
 

  cout << "Everything finished" << endl;
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
