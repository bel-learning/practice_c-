#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
#endif /* __PROGTEST__ */

void printBools(vector<bool> ba) {
  if(ba.empty()) return;
    for(size_t i = 0; i < ba.size(); i++) 
      cout << ba[i];
    cout << endl;
}

class Fib {
public:
  // Fib(vector<bool>&new_data) { data = new_data; };
  Fib() {};
  bool fail = false;
  vector<bool> data;
  
  vector<bool> intToFib(int num) {
    vector<int> fib;
    fib.push_back(1);
    fib.push_back(2);
    num += 1;
    int cur = 2;
    int fi = 0;
    
    while (cur <= num) {
      fib.push_back(fib[fi]+ fib[fi+1]);
      cur = fib[fi+2];
      fi++;
    }

    vector<bool> ans;
    fi += 2;
    for(int i = 0; i < fi; i++) {
      ans.push_back(false);
    }
    for(int i = fi - 1; i >= 0; i--) {
      if(num == 0) break;
      if(num - fib[i] >= 0) {
        num -= fib[i];
        ans[i] = true;
      }
    }
    ans[fi-1] = true;
    // printBools(ans);  
    return ans;
  }
  void appendToData(vector<bool> new_bools, ofstream& out ) {
    for(size_t i = 0; i < new_bools.size(); i++) {
      if(data.size() >= 8) {
        writeToFile(out);
      }
      data.push_back(new_bools[i]);
    }
  }
  void writeToFile(ofstream& out) {
    // cout << "here" << endl;;
    int num = 0;
    int mul = 1;
    reverseData();
    // printBools(data);
    for(int i = data.size() - 1; i >= 0; i--) {
      // cout << "i: " << i << endl;
      if(data[i]) {
        num += mul;
      }
      mul *= 2;
    }
    data.clear();
    char byte = (char) num;
    out.write(&byte, 1);
    if(out.fail()) {
      fail = true;
      return;
    }
    // cout << hex << num << endl;
  }
  void writeLeftover(ofstream& out) {
    while(data.size() < 8) {
      data.push_back(false);
    }
    writeToFile(out);
  }
  void reverseData() {
    // Reversing will always be 1 bit
    if(data.size() != 8) {
      cout << "Trying to reverse half-byte" << endl; 
      return;
    }
    bool tmp;
    int j = 7;
    for(size_t i = 0; i < 4; i++) {
      tmp = data[i];
      data[i] = data[j];
      data[j] = tmp;
      j--;
    }
  }
  
  
};




vector<bool> ByteToBools(char byte, int last) {
    vector<bool> bits;
    for (int i = 0; i < 8; i++) {
        // Extract the i-th bit using a bitwise AND operation
        bool bit = byte & (1 << i);
        // Add the bit to the vector
        bits.push_back(bit);
    }
    
    vector<bool> new_bits;
    for(int i = last-1; i >= 0; i --) {
      new_bits.push_back(bits[i]);
    }
    return new_bits;
  }



int binBoolsToInt(vector<bool> vec) {
  int mul = 1;
  int sum = 0;
  for(int i = vec.size() - 1; i >= 0 ; i--) {
    sum += (vec[i] * mul);
    mul *= 2;
  }
  return sum;
}

void reverseByte(vector<bool>& bits) {
  bool tmp;
  int j = bits.size() - 1;
  for(size_t i = 0; i < bits.size() / 2; i ++) {
    tmp = bits[i];
    bits[i] = bits[j];
    bits[j] = tmp;
    j--;
  }
}

void mergeTwoBooleans(vector<bool> & vec1, vector<bool> & vec2) {

  for(size_t i = 0; i < vec2.size(); i++) {
    vec1.push_back(vec2[i]);
  }
}

class UTF8 {
  int fib_int;
public:
  UTF8() {fib_int = -1; fail = false;};
  bool fail;
  vector<bool> data;
  void insertToData(vector<bool> bits, ofstream & out) {
    for(size_t i = 0; i < bits.size(); i++) {
      data.push_back(bits[i]);
    }
    bool codepoint = true;
    while(codepoint) {
        codepoint = false;
        // cout << "stuck here" << endl;
        if(data.empty()) return;
        if(fail) return;
        for(size_t i = 0; i < data.size() - 1; i++) {
          if(data[i] && data[i+1]) {
              codepoint = true;
              removeCodePoint(out);
              break;
          }
        }
    }
  }
  void removeCodePoint(ofstream & out) {
    int counter = 0;
    for(size_t i = 0; i < data.size() - 1; i++) {
      if(data[i] && data[i+1]) {
        counter = i + 1;
        break; 
      }
    }
    vector<int> fib;
    fib.push_back(1);
    fib.push_back(2);
    int sum = -1;
    for(int i = 0; i < counter; i++) {
      fib.push_back(fib[i] + fib[i+1]);
    }
    for(int i = 0; i < counter; i++) {
      if(sum > 0x10ffff) {
          fail = true;
          return;
        }
      if(data[i]) {
        sum += fib[i];
        // cout << sum << " ";
      }
    }
    // printBools(data);
    data.erase(data.begin(), data.begin() + counter + 1);
    // printBools(data);

    fib_int = sum;
    writeToFile(out);
    return;
  }
  void writeToFile(ofstream & out) {
    if(fib_int == -1) {
      cout << "Error somewhere" << endl;
      fail = true;
      return ;
    } 
    // cout << "fib num too big: " << fib_int << endl;

    if(fib_int > 0x10ffff) {
      fail = true; 
      // cout << "fib num too big: " << fib_int << endl;
     return;
    };
    char byte;
    if (fib_int <= 0x7f) {
      byte = static_cast<char>(fib_int);
      out.write(&byte, 1);
    }
    else if (fib_int <= 0x7ff)
    {
        byte = static_cast<char>(0xc0 | ((fib_int >> 6) & 0x1f));
        out.write(&byte, 1);
        byte = static_cast<char>(0x80 | (fib_int & 0x3f));
        out.write(&byte, 1);
    }
    else if (fib_int <= 0xffff)
    {
        byte = static_cast<char>(0xe0 | ((fib_int >> 12) & 0x0f));
        out.write(&byte, 1);
        byte = static_cast<char>(0x80 | ((fib_int >> 6) & 0x3f));
        out.write(&byte, 1);
        byte = static_cast<char>(0x80 | (fib_int & 0x3f));
        out.write(&byte, 1);
    }
    else
    {
        byte = static_cast<char>(0xf0 | ((fib_int >> 18) & 0x07));
        out.write(&byte,1);
        byte = static_cast<char>(0x80 | ((fib_int >> 12) & 0x3f));
        out.write(&byte,1);
        byte = static_cast<char>(0x80 | ((fib_int >> 6) & 0x3f));
        out.write(&byte,1);
        byte = static_cast<char>(0x80 | (fib_int & 0x3f));
        out.write(&byte,1);
    }
    if(out.fail()) {
      fail = true;
      return;
    }
    fail = false;
    return;
  }
};


bool               utf8ToFibonacci                         ( const char      * inFile, 
                                                             const char      * outFile )
{
  // todo
  ifstream file(inFile, ios::binary | ios::in);
  ofstream out(outFile, ios::binary | ios::out);
  if(file.fail() || out.fail()) return false;
 
  // Take One UTF-8 number (1-4 bytes)
  char value;
  Fib fib_state;
  while(file.read(&value, 1)) {
    // UTF8 current;
    if( (value & 0b10000000) == 0 ) {
      // Just read this
      int num = static_cast<int>(value);
      vector<bool> new_fib = fib_state.intToFib(num);
      fib_state.appendToData(new_fib, out);
      
      // out.write(&value, 1);
      continue;
    }
    if( (value & 0b11111000) == 240) {
      //  3 more byte waiting
      vector<bool> tmp_bools = ByteToBools(value, 3);
      for(size_t i = 0; i < 3; i++) {
        if(!file.read(&value, 1)) return false;
        if((value & 0b11000000) != 128) return false; 
        vector<bool> byte_vector = ByteToBools(value, 6);
        mergeTwoBooleans(tmp_bools, byte_vector);
      }

      int num  = binBoolsToInt(tmp_bools);
      vector<bool> new_fib = fib_state.intToFib(num);
      fib_state.appendToData(new_fib, out);
      new_fib.clear();
      continue;

    }
    if( (value & 0b11110000) == 224) {
      //  2 more byte waiting
      vector<bool> tmp_bools = ByteToBools(value, 4);
      for(size_t i = 0; i < 2; i++) {
        if(!file.read(&value, 1)) return false;
        if((value & 0b11000000) != 128) return false; 
        vector<bool> byte_vector = ByteToBools(value, 6);
        mergeTwoBooleans(tmp_bools, byte_vector);
      }

      int num  = binBoolsToInt(tmp_bools);
      vector<bool> new_fib = fib_state.intToFib(num);
      fib_state.appendToData(new_fib, out);
      new_fib.clear();
      continue;

    }
    if( (value & 0b11100000) == 192 ) {
      //  One more byte waiting
      
      vector<bool> tmp_bools = ByteToBools(value, 5);
      for(size_t i = 0; i < 1; i++) {
        if(!file.read(&value, 1)) return false;
        if((value & 0b11000000) != 128) return false; 
        vector<bool> byte_vector = ByteToBools(value, 6);
        mergeTwoBooleans(tmp_bools, byte_vector);
      }

      int num  = binBoolsToInt(tmp_bools);
      vector<bool> new_fib = fib_state.intToFib(num);
      fib_state.appendToData(new_fib, out);
      new_fib.clear();
      
      continue;
    }

    return false;

  }
  
  // Success.
  if(out.fail()) return false;
  fib_state.writeLeftover(out);
  file.close();
  out.close();

  return true;
}
  
bool               fibonacciToUtf8                         ( const char      * inFile, 
                                                             const char      * outFile )
{
  // todo
  ifstream f_in(inFile, ios::binary | ios::in);
  ofstream f_out(outFile, ios::binary | ios::out);
  if(f_in.fail() || f_out.fail()) return false;

  UTF8 state;
  char value;
  bool seenOne = false;
  while(f_in.read(&value , 1)) {
    vector<bool> bits = ByteToBools(value, 8);
    for(size_t i = 0; i < bits.size(); i++) {
      if(bits[i])
        seenOne = true;
    }
    reverseByte(bits);
    state.insertToData(bits, f_out);
    if(state.fail) return false;
  }
  // cout <<"Finished: " << endl;
  // printBools(state.data);
  // cout << endl;
  for(size_t i = 0; i < state.data.size(); i++) {
    if(state.data[i]) return false;
  } 
  if(!seenOne) return false;
  if(f_out.fail()) return false;

  return true;
}
  
#ifndef __PROGTEST__

bool               identicalFiles                          ( const char      * file1,
                                                             const char      * file2 )
{
  // todo
  ifstream f1(file1, ios::binary | ios::in);
  ifstream f2(file2, ios::binary | ios::in);
  if(f1.fail() || f2.fail())  return false;
  if(f1.tellg() != f2.tellg())
    return false;
  f1.seekg(0, ifstream::beg);
  f2.seekg(0, ifstream::beg);
  return std::equal(istreambuf_iterator<char>(f1.rdbuf()),
                    istreambuf_iterator<char>(),
                    istreambuf_iterator<char>(f2.rdbuf()));

}

int main ( int argc, char * argv [] )
{
  assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_0.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_1.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_2.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_3.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_4.fib" ) );
  assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );
  assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );
  assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );
  assert (! fibonacciToUtf8 ( "failing.bin", "output.utf8" ));
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
