#include<cstring>
#include<iostream>
using namespace std;

namespace my
{
  class string
  {
  public:
    int m_ref;
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
      _size = other._size;
      _cap = _size + 1;
      _data = new char[_cap];
      memcpy(_data, other._data, _size);
      _data[_size] = '\0';
    }
    
    ~string() {
      if(m_ref == 0) {
        delete [] _data;
        _data = nullptr;
        _size = 0;
        _cap = 0;
      }
    }
    void attach() {
      m_ref++;
    }
    void detach() {
      m_ref--;
    }
    
    size_t size()
    {
      return _size;
    }
    const char * data() const
    {
      return _data;
    }
   
    my::string & operator = (const my::string &other)
    {
      if(this == &other)
        return *this;
      delete [] _data;
      _size = other._size;
      _cap = _size+1;
      _data = new char[_cap];
      memcpy(_data, other._data, _size);
      _data[_size] = '\0';
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
}
class CMail {
  public:
  CMail(const char * body): _body(body) {
    _body.attach();
  }
  private:
  my::string _body;
};

int main() {
    my::string a("a");
    my::string b = a;
    b = "d";
    cout << "a:" << a << endl;
    cout << "b:" << b << endl;
}