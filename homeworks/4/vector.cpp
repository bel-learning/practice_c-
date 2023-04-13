#include<iostream>

using namespace std;

namespace my
{
  template <typename T>
  class vector
  {
    T *_arr;
    size_t _cap;
    size_t _size;
    int *_ref;
  public:
    vector()
    {
      _arr = new T[1];
      _cap = 1;
      _size = 0;
      _ref = new int(1);
    }
    ~vector()
    {
        decreaseRef();
        deleteData();
    }
    // Copy constructor
    vector(const vector &other)
    {
        _arr = other._arr;
        _cap = other._cap;
        _size = other._size;
        _ref = other._ref;
        increaseRef();
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
        _arr = returnDeepCopy();
        decreaseRef();
        _ref = new int(1);
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

    T * returnDeepCopy() {
        T * newArr = new T[_cap];
        for (size_t i = 0; i < _size; i++)
        {
            newArr[i] = _arr[i];
        }
        return newArr;
    }

    vector& operator=(const vector& other) {
        if (this != &other) {
            cout << "Operator just shallow copy" << endl;

            decreaseRef();
            deleteData();
            _cap = other._cap;
            _size = other._size;
            _arr = other._arr;
            _ref = other._ref;
            increaseRef();
            
        }
        return *this;
    }

    void decreaseRef() {
        (*_ref)--;
    }
    void increaseRef() {
        (*_ref)++;
    }
    int getCount() {
        return *_ref;
    }
    void deleteData() {
        if(*_ref == 0) {
            cout << "Deleting data" << endl;
            delete [] _arr;
            delete _ref;
            _size = 0;
            _cap = 0;
        }
    }
  };
}

int main() {
    my::vector<int> a;
    a.push_back(1);
   
    a.push_back(2);
     for(auto it : a) {
        cout << it << endl;
    }

    my::vector<int> c = a;
    my::vector<int> b;
    b = a;

    for(auto it : b) {
        cout << it << endl;
    }
    cout << "instance count: " << b.getCount() << endl;
    b.push_back(3);
    cout << "instance count: " << b.getCount() << endl;
    
    cout << "Previous a array count: " << a.getCount() << endl;
}