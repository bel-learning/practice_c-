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
    
    string(const char *str = "")
    {
      size_t len = strlen(str);
      _cap = len + 1;
      _size = len;
      _data = new char[_cap];
      memcpy(_data, str, len);
      _data[len] = '\0';
    }
     string(const my::string& other)
    {
       _data = other._data;
      _size = other._size;
      _cap = other._cap;
    }
   
    ~string() {
      // delete [] _data;
      // _data = nullptr;
     // _size = 0;
      // _cap = 0;
    }
    size_t size() const
    {
      return _size;
    }
    const char * data()
    {
      return _data;
    }

    my::string & operator = (const my::string &other)
    {
      if(this == &other) return *this;
      _data = other._data;
      _size = other._size;
      _cap = other._cap;
      return *this;
    }
    bool operator==(const my::string &other) const
    {
      if (strcmp(_data, other._data) == 0)
        return true;
      return false;
    }
    bool operator!=(const my::string &other) const
    {
      return !(*this == other);
    }
    bool operator>(const my::string &other) const
    {
      return (strcmp(_data, other._data) > 0);
    }
    bool operator<(const my::string &other) const
    {
      return (strcmp(_data, other._data) < 0);
    }
    bool operator>=(const my::string &other) const
    {
      return (strcmp(_data, other._data) >= 0);
    }
    bool operator<=(const my::string &other) const
    {
      return (strcmp(_data, other._data) <= 0);
    }
    friend ostream & operator << (ostream & out, my::string me) {
        out << me.data();
        return out;
    }
    char &operator[](int index)
    {
      return _data[index];
    }
    const char &operator[](int index) const
    {
      return _data[index];
    }
  private:
    
    char *_data;
    size_t _size;
    size_t _cap;
  };


  template <typename T>
  class vector
  {
    T *_arr;
    size_t _cap;
    size_t _size;

  public:
    vector()
    {
      _arr = new T[1];
      _cap = 1;
      _size = 0;
    }
    ~vector()
    {
      delete[] _arr;
      _size = 0;
      _cap = 0;
      _arr = nullptr;
    }
    // Copy constructor
    vector(const vector &other)
    {
        _arr = new T[other._cap];
        _cap = other._cap;
        _size = other._size;
        for (size_t i = 0; i < _size; i++)
        {
            _arr[i] = other._arr[i];
        }
    }

    // Move constructor
    vector(vector &&other) noexcept
    {
        _arr = other._arr;
        _cap = other._cap;
        _size = other._size;
        other._arr = nullptr;
        other._cap = 0;
        other._size = 0;
    }
    void push_back(const T & data)
    {

      // if the number of elements is equal to the
      // capacity, that means we don't have space to
      // accommodate more elements. We need to double the
      // capacity
      if (_size + 1>= _cap)
      {
        T * temp = new T[_cap * 2];

        // copying old array elements to new array
        for (size_t i = 0; i < _size; i++)
        {
          temp[i] = _arr[i];
        }

        // deleting previous array
        delete[] _arr;
        _arr = temp;
        _cap *= 2;
      }

      // Inserting data
      _arr[_size] = data;
      _arr[_size+1] = NULL;
      _size++;
    }
    T &at(int index)
    {
      if (index < _size)
        return _arr[index];
    }
    size_t size() const
    {
      return _size;
    }
    void print()
    {
      for (int i = 0; i < _size; i++)
      {
        cout << _arr[i] << " ";
      }
      cout << endl;
    }
    T &operator[](int index)
    {
      return _arr[index];
    }
    const T &operator[](int index) const
    {
      return _arr[index];
    }
    T * begin() {
      if(_size == 0) return nullptr;
      return (&_arr[0]);
    }
    T * end() {
      if(_size == 0) return nullptr;
      return (&_arr[_size]);
    }

    vector& operator=(const vector& other) {
        if (this != &other) {
            _cap = other._cap;
            _size = other._size;
            delete[] _arr;
            _arr = new T[_cap];
            for (size_t i = 0; i < _size; i++) {
                _arr[i] = other._arr[i];
            }
        }
        return *this;
    }

    
  };

}

class CMail
{
public:
  CMail() : _from(""), _to(""), _body(""){};
  
  CMail(const CMail &x) : _from(x._from), _to(x._to), _body(x._body){};
  CMail(const char *from,
        const char *to,
        const char *body)
      : _from(from), _to(to), _body(body){};
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
      
      os << "From: " << m._from << ", To: " << m._to << ", Body: " << m._body;
      return os;
  };
  my::string getFrom() const
  {
    return _from;
  }
  my::string getTo() const
  {
    return _to;
  }
  my::string getBody() const
  {
    return _body;
  }

private:
  // todo
  my::string _from;
  my::string _to;
  my::string _body;
};

class CMailIterator
{
public:
  CMailIterator(CMail ** start, CMail ** current, CMail ** end): head(start), ptr(current), end_ptr(end) {};

  CMailIterator(const CMailIterator & other) : head(other.head), ptr(other.ptr), end_ptr(other.end_ptr) {}

  // Move constructor
  CMailIterator(CMailIterator && other) noexcept : head(other.head), ptr(other.ptr), end_ptr(other.end_ptr) {
    other.head = nullptr;
    other.ptr = nullptr;
    other.end_ptr = nullptr;
  }
  CMailIterator operator = (const CMailIterator & other) {
    return CMailIterator(other);
  }
  
  explicit operator bool(void) const {
    return end_ptr != ptr;
  };
  bool operator ! (void) const {
    return end_ptr == ptr;
  };
  const CMail & operator*(void) const {
    return **ptr;
  };
  CMail & operator * (void) {
    return **ptr;
  };
  CMailIterator &operator++(void){
    if(end_ptr != ptr)
      ptr++;
    return *this;
  };
  
 
  CMailIterator begin() const {
    return CMailIterator(head, head, end_ptr);
  }
  CMailIterator end() const {
    return CMailIterator(head, end_ptr, end_ptr);
  }


private:
  // todo
  CMail ** head;
  CMail ** ptr;
  CMail ** end_ptr;
};

struct Node
  {
    Node *left;
    Node *right;
    my::string email;
    my::vector<CMail *> inbox;
    my::vector<CMail *> outbox;
    Node(): left(nullptr), right(nullptr) {};
  };

class BST
{
  
  Node *root;
public:
  int _ref;

  BST(void) : root(nullptr), _ref(1) {};
  // ~BST() {
  //   cout << "Tree destructor: " <<  _ref << endl;

  //   if(_ref == 0)
  //     freeItems(root);
  // }
  BST(const BST & other) {
    cout << "copy constructor on tree" << endl;
    root = other.root;
  }
  BST & operator = (const BST & other) {
    cout << "assignment constructor on tree" << endl;
    root = other.root;
    return *this;
  }

  void freeItems(Node * current) {
    if(current == nullptr) {
      return;
    }
    freeItems(current->left);
    freeItems(current->right);
    delete current;
  }
  void Insert(CMail * x)
  {
    Node *node = root;

    Node *parent = nullptr;
    bool exists = false;
    bool left = false;
    my::string sender = x->getFrom();
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
    
    const my::string receiver = x->getTo();

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

  Node *Find(const my::string &email) const
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

  void populateTree(const BST * src) {
    root = copyTree(src->root);
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

  void attach() {
    _ref++;
    cout << "Attached to the tree: " << _ref << endl;
  };
  void detach() {
    _ref--;
  };

};

class CMailServer
{
public:
  CMailServer(void) : tree(new BST) {};
  CMailServer(const CMailServer &src) {
    tree = src.tree;
    tree->attach();
    // tree = std::move(src.tree);
    // tree->attach();
  };

  CMailServer &operator=(const CMailServer &src){
    if(this == &src) return *this;
    if(--tree->_ref == 0)
      delete tree;
    tree = src.tree;
    tree->attach();
    return *this;
  };
   
  ~CMailServer(void){
    tree->detach();
    if(tree->_ref == 0)
      delete tree;
  };
  void sendMail(const CMail &m)
  {
    CMail * mail = new CMail(m);
    BST * new_tree = new BST;
    new_tree->populateTree(tree);
    tree->detach();
    if(tree->_ref == 0)
      delete tree;
    tree = new_tree;
    tree->Insert(mail);
    // tree->print2D();
    // cout << "------------------------------" << endl;
  };
  CMailIterator outbox(const char *email) const {
    my::string searchFor(email);
    Node * el = tree->Find(searchFor);
   
    if(el == nullptr) 
      return CMailIterator(nullptr, nullptr, nullptr);
    return CMailIterator(el->outbox.begin(), el->outbox.begin(), el->outbox.end());
  };

  CMailIterator inbox(const char *email) const {
    my::string searchFor(email);
    Node * el = tree->Find(searchFor);
    if(el == nullptr) 
      return CMailIterator(nullptr, nullptr, nullptr);

    return CMailIterator(el->inbox.begin(), el->inbox.begin(), el->inbox.end());
  };
  
private:
  // todo
  
  BST * tree;
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
  s6 = s6;
  cout << "Everything finished" << endl;
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
