#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
  public:
                             CRect                         ( double            x,
                                                             double            y,
                                                             double            w,
                                                             double            h )
      : m_X ( x ),
        m_Y ( y ),
        m_W ( w ),
        m_H ( h )
    {
    }
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CRect     & x )
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double                   m_X;
    double                   m_Y;
    double                   m_W;
    double                   m_H;
};
#endif /* __PROGTEST__ */

class CComponent {
  public:
  CComponent() = delete;
  
  CComponent(int id, const CRect & pos): m_Id(id), m_rPos(pos), m_aPos(pos) { };
  CComponent(int id, const CRect & rPos, const CRect & aPos): m_Id(id), m_rPos(rPos), m_aPos(aPos) { };
  
  virtual ~CComponent()  {};
  enum class Type {
    LABEL,
    INPUT,
    BUTTON,
    COMBOBOX
  };
  int getId() const {
    return m_Id;
  }
  CRect getRPos() const {
    return m_rPos;
  }
  CRect getAPos() const {
    return m_aPos;
  }

  void setId(int id) {
    m_Id = id;
  }
  void setRPos(CRect pos) {
    m_rPos = pos;
  }
  
  virtual Type getType() const = 0;
  virtual ostream & print(ostream & out, int space = 0,  bool horizontalLine = false) const = 0;
  virtual CComponent * createNewCopy() const = 0;
  virtual void changeAbsPos(const CRect & absPos) {
    m_aPos = CRect(
    m_rPos.m_X * absPos.m_W + absPos.m_X, 
    m_rPos.m_Y * absPos.m_H + absPos.m_Y,
    m_rPos.m_W * absPos.m_W,
    m_rPos.m_H * absPos.m_H
    );
  }
  private:
  int m_Id;
  CRect m_rPos;
  CRect m_aPos;
};
ostream & operator << (ostream & out, const CComponent & comp) {
  return comp.print(out);
}


class CWindow 
{
  public:
                             CWindow                       ( int               id,
                                                             const string    & title,
                                                             const CRect     & absPos )
                                                             : m_Id(id), m_Title(title), m_Pos(absPos), m_BoxCount(0) {};
                            CWindow ( const CWindow & other): m_Pos(other.m_Pos) {
                              m_Id = other.m_Id;
                              m_Title = other.m_Title;
                              m_BoxCount = other.m_BoxCount;
                              // cout << "CWindow copy constructor called" << endl;
                              for(size_t i = 0; i < other.objects.size(); i++) {
                                objects.push_back(other.objects[i]->createNewCopy());
                              }
                            }
                            CWindow & operator = (const CWindow & other) {
                              if(this == &other) return *this;
                              m_Pos = other.m_Pos;
                              m_Id = other.m_Id;
                              m_Title = other.m_Title;
                              m_BoxCount = other.m_BoxCount;
                              freeItems();
                              objects.clear();
                              // objects = other.objects;
                               for(size_t i = 0; i < other.objects.size(); i++) {
                              
                                objects.push_back(other.objects[i]->createNewCopy());
                              }
                              return *this;
                            }
    // add
    CWindow & add(const CComponent & comp) {
      CComponent * new_comp = comp.createNewCopy();
      new_comp->changeAbsPos(m_Pos);
      if(new_comp->getType() == CComponent::Type::COMBOBOX)
        m_BoxCount++;

      objects.push_back(new_comp);
      return *this;
    }
    ~CWindow() {
      freeItems();
    }
    void freeItems() {
      for(size_t i = 0; i < objects.size(); i++)
        delete objects[i];   
    }
    // search
    CComponent * search(int id) {
      for(CComponent * comp : objects) {
        if(id == comp->getId())
          return comp;
      }
      return nullptr;
    }
    // setPosition
    void setPosition(const CRect & pos) {
      m_Pos = pos;
      for(size_t i = 0; i < objects.size(); i++) {
        objects[i]->changeAbsPos(m_Pos);
      }
    }
    friend ostream & operator << (ostream & out, const CWindow & window);

  private:
    int m_Id;
    string m_Title;
    CRect m_Pos;
    int m_BoxCount;
    vector<CComponent *> objects;
};

ostream & operator << (ostream & out, const CWindow & window) {
  out << "[" << window.m_Id << "]" << " Window \"" << window.m_Title << "\" " << window.m_Pos << "\n";
  for(size_t i = 0; i < window.objects.size(); i++) {
    CComponent * obj = window.objects[i];
    if(obj->getType() == CComponent::Type::COMBOBOX)  {
      if(i == window.objects.size() - 1) {
        obj->print(out, 1, false);
        continue;
      }
      else {
        obj->print(out, 1, true);
        continue;
      }
    }
      
    obj->print(out, 1, false);
  }
  return out;
}

class CButton : public CComponent
{
  public:
                             CButton                       ( int               id,
                                                             const CRect     & relPos,
                                                             const string    & name )
                   : CComponent(id, relPos), m_Name(name)     {}  ;
                            CButton                       ( int               id,
                                                             const CRect     & relPos,
                                                             const CRect     & absPos,
                                                             const string    & name )
                   : CComponent(id, relPos, absPos), m_Name(name)     {}  ;
                    

    CComponent * createNewCopy() const override {
      return new CButton(this->getId(), this->getRPos(), this->getAPos(), m_Name);
    }
    ostream & print(ostream & out, int space = 0, bool horizontalLine = false) const override {
      out <<  (space != 0 ? "+- " : "") << "["  << this->getId() << "]" << " Button \"" << m_Name << "\" " << this->getAPos() << "\n";
      return out;
    };
    Type getType() const override {
      return CComponent::Type::BUTTON;
    }

  private:
    string m_Name;
};

class CInput : public CComponent
{
  public:
                             CInput                        ( int               id,
                                                             const CRect     & relPos,
                                                             const string    & value ) 
                   : CComponent(id, relPos), m_Value(value)     {}  ;
                               CInput                       ( int               id,
                                                             const CRect     & relPos,
                                                             const CRect     & absPos,
                                                             const string    & value )
                   : CComponent(id, relPos, absPos), m_Value(value)     {}  ;
    // setValue 
    void setValue(string value) {
      m_Value = value;
    }
    string getValue() const {
      return m_Value;
    }
    // getValue 
    CComponent * createNewCopy() const override {
      return new CInput(this->getId(), this->getRPos(), this->getAPos(), m_Value);
    }
    ostream & print(ostream & out, int space = 0,  bool horizontalLine = false) const override {
      out << (space != 0 ? "+- " : "") << "[" << this->getId() << "]" << " Input \"" << m_Value << "\" " << this->getAPos() << "\n";
      return out;
    };
     Type getType() const override {
      return CComponent::Type::INPUT;
    }

  private:
    string m_Value;
};
class CLabel : public CComponent
{
  public:
                             CLabel                        ( int               id,
                                                             const CRect     & relPos,
                                                             const string    & label )
                  : CComponent(id, relPos), m_Label(label) {};
                                 CLabel                       ( int               id,
                                                             const CRect     & relPos,
                                                             const CRect     & absPos,
                                                             const string    & label )
                   : CComponent(id, relPos, absPos), m_Label(label)     {}  ;

  CComponent * createNewCopy() const override {
      return new CLabel(this->getId(), this->getRPos(), this->getAPos(), m_Label);
    }
  ostream & print(ostream & out,  int space = 0, bool horizontalLine = false) const override {
      out << (space != 0 ? "+- " : "") << "[" << this->getId() << "]" << " Label \"" << m_Label << "\" " << this->getAPos() << "\n";
      return out;
    };
   Type getType() const override {
      return CComponent::Type::LABEL;
    }
  private:
    string m_Label;                                                           
};

class CComboBox : public CComponent
{
  public:
                             CComboBox                     ( int               id,
                                                             const CRect     & relPos ) : CComponent(id, relPos), m_Selected(0) {};
                             CComboBox                     ( int               id,
                                                             const CRect     & relPos,
                                                             const CRect     & absPos,
                                                             const vector<string> & list,
                                                             int selected 
                                                              ): CComponent(id, relPos, absPos), m_List(list), m_Selected(selected) {}
                                                              ;
         
    // add                    
    CComboBox & add(const string & str) {
      m_List.push_back(str);
      return *this;
    }                                                        
    // setSelected
    void setSelected(int id ) {
      m_Selected = id;
    }
    int getSelected() const {
      return m_Selected;
    }
    // getSelected
    CComponent * createNewCopy() const override {
      // cout << "Creating new copy of Combobox, with selected = 0" << endl;
      return new CComboBox(this->getId(), this->getRPos(), this->getAPos(), m_List, 0);
    }
    
    ostream & print(ostream & out, int space = 0,  bool horizontalLine = false) const override {
      // out << "+-  [" << this->getId() << "]" << " Button \"" << m_Label << "\" " << this->changeAbsPos(absPos) << "\n";
      out << (space != 0 ? "+- " : "") << "[" << this->getId() << "]" << " ComboBox " << this->getAPos() << "\n";
        for(size_t i = 0; i < m_List.size(); i++) {
          if(space == 1) {
            if(horizontalLine)
              out << "|  ";
            else
              out << "   ";
          }
          out << "+-" <<(m_Selected == i ? ">" : " ") << m_List[i] <<(m_Selected == i ? "<" : "") << "\n";
        }
      
      return out;
    };
     Type getType() const override {
      return CComponent::Type::COMBOBOX;
    }
   
  private:
    vector<string> m_List;
    size_t m_Selected;
};

// output operators

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  // cout << oss.str();
  return oss . str ();
}

int main ( void )
{
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  // cout << "Should call copy constructor" << endl;
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CComboBox &> (*a.search(20)).getSelected() == 0 );

  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  CWindow c ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  c = a;
  CButton but1 = dynamic_cast<CButton &>(*b.search(1));
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
