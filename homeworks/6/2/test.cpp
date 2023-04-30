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
    COMBOBOX,
    PANEL
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
  void setRPos(const CRect & pos) {
    m_rPos = pos;
  }
  void setAPos(const CRect & pos) {
    m_aPos = pos;
  }
  
  virtual Type getType() const = 0;
  virtual ostream & print(ostream & out, int space = 0, string prefix = "", bool horizontalLine = false) const = 0;
  virtual CComponent * createNewCopy() const = 0;
  virtual void changeAbsPos(const CRect & absPos) {
    m_aPos = CRect(
    m_rPos.m_X * absPos.m_W + absPos.m_X, 
    m_rPos.m_Y * absPos.m_H + absPos.m_Y,
    m_rPos.m_W * absPos.m_W,
    m_rPos.m_H * absPos.m_H
    );
  }
  virtual CComponent * findID(int id, CComponent * self) const {
    if(self->m_Id == id) return self;
    return nullptr;
  };
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
                                                             : m_Id(id), m_Title(title), m_Pos(absPos) {};
                            CWindow ( const CWindow & other): m_Pos(other.m_Pos) {
                              m_Id = other.m_Id;
                              m_Title = other.m_Title;
                              // cout << "CWindow copy constructor called" << endl;
                              for(size_t i = 0; i < other.objects.size(); i++) {
                                objects.push_back(other.objects[i]->createNewCopy());
                              }
                              // cout << "Copied things" << endl;
                            }
                            CWindow & operator = (const CWindow & other) {
                              if(this == &other) return *this;
                              m_Pos = other.m_Pos;
                              m_Id = other.m_Id;
                              m_Title = other.m_Title;
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
      CComponent * obj = nullptr;
      for(CComponent * comp : objects) {
        obj = comp->findID(id, comp);
        if(obj != nullptr) {
          return obj; 
        }
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
    vector<CComponent *> objects;
};
// The toppest in the hierarchy deserves its definition wide and open.
ostream & operator << (ostream & out, const CWindow & window) {
  out << "[" << window.m_Id << "]" << " Window \"" << window.m_Title << "\" " << window.m_Pos << "\n";
  string prefix;
  for(size_t i = 0; i < window.objects.size(); i++) {
    CComponent * obj = window.objects[i];
    if(obj->getType() == CComponent::Type::COMBOBOX || obj->getType() == CComponent::Type::PANEL)  {
      if(i == window.objects.size() - 1) {
        obj->print(out, 1, prefix, false);
        continue;
      }
      else {
        obj->print(out, 1, prefix, true);
        continue;
      }
    }
      
    obj->print(out, 1, prefix, false);
  }
  return out;
}

class CPanel : public CComponent
{
  vector<CComponent *> m_Objects;
  public:
                             CPanel                        ( int               id,
                                                             const CRect     & relPos )
                        : CComponent(id, relPos) {};
                            CPanel                        ( int               id,
                                                             const CRect     & relPos,
                                                             const CRect    & absPos
                                                              )
                        : CComponent(id, relPos, absPos) {};
                           
    ~CPanel() {
      for(CComponent * obj : m_Objects) {
        delete obj;
      }
    }
    // add
    CComponent * createNewCopy() const override {
      CPanel * newPanel = new CPanel(this->getId(), this->getRPos(), this->getAPos());
      // cout << " Creating new copy of panel " << endl;
      for(CComponent * obj : m_Objects) {
        newPanel->m_Objects.push_back(obj->createNewCopy());
      }
      // cout << "Copied the objects inside panel" << endl;
      return newPanel;
    }
    
    CPanel & add(const CComponent & comp) {
      
      CComponent * new_comp = comp.createNewCopy();
      new_comp->changeAbsPos(this->getAPos());
      // if(new_comp->getType() == CComponent::Type::PANEL) {
      // }

      m_Objects.push_back(new_comp);
      return *this;
    }
    ostream & print(ostream & out, int space = 0, string prefix = "", bool horizontalLine = false) const override {
      out << prefix << (space != 0 ? "+- " : "") << "[" << this->getId() << "]" << " Panel " << this->getAPos() << "\n";
      int len= static_cast<int> (prefix.size());
      for(int j = 0; j < space * 3 - len; j++) {
          if(horizontalLine && j == 0) {
            prefix.push_back('|');
          } else {
            prefix.push_back(' ');
          }
      }
      
      for(size_t i = 0; i < m_Objects.size(); i++) {
        CComponent * obj = m_Objects[i];

        if(obj->getType() == CComponent::Type::COMBOBOX || obj->getType() == CComponent::Type::PANEL)  {
          if(i == m_Objects.size() - 1) {
            obj->print(out, space + 1, prefix, false);
            continue;
          }
          else {
            obj->print(out, space + 1, prefix, true);
            continue;
          }
        }
          obj->print(out, space , prefix, false);
       }
    
      return out;
    
    };
    Type getType() const override {
      return CComponent::Type::PANEL;
    }
    void changeAbsPos(const CRect & absPos) override {
      // Changing it's abs position for children to know its parent's abs
       this->setAPos(CRect(
        this->getRPos().m_X * absPos.m_W + absPos.m_X, 
        this->getRPos().m_Y * absPos.m_H + absPos.m_Y,
        this->getRPos().m_W * absPos.m_W,
        this->getRPos().m_H * absPos.m_H
      ));
      for(CComponent * obj: m_Objects) {
        obj->changeAbsPos(this->getAPos());
      }
    }

    CComponent * findID(int id, CComponent * self) const override {
      if(self->getId() == id) return self;
      for(CComponent * obj : m_Objects) {
        CComponent * found = this->findID(id, obj);
        if(found != nullptr)
          return found;
      }
      return nullptr;
    };

};


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
    ostream & print(ostream & out, int space = 0, string prefix="", bool horizontalLine = false) const override {
      out << prefix <<  (space != 0 ? "+- " : "") << "["  << this->getId() << "]" << " Button \"" << m_Name << "\" " << this->getAPos() << "\n";
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
    ostream & print(ostream & out, int space = 0, string prefix="",  bool horizontalLine = false) const override {
      out << prefix << (space != 0 ? "+- " : "") << "[" << this->getId() << "]" << " Input \"" << m_Value << "\" " << this->getAPos() << "\n";
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
  ostream & print(ostream & out,  int space = 0, string prefix = "", bool horizontalLine = false) const override {
      out << prefix << (space != 0 ? "+- " : "") << "[" << this->getId() << "]" << " Label \"" << m_Label << "\" " << this->getAPos() << "\n";
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
      return new CComboBox(this->getId(), this->getRPos(), this->getAPos(), m_List, m_Selected);
    }
    
    ostream & print(ostream & out, int space = 0, string prefix = "",  bool horizontalLine = false) const override {
      // out << "+-  [" << this->getId() << "]" << " Button \"" << m_Label << "\" " << this->changeAbsPos(absPos) << "\n";
      out << prefix << (space != 0 ? "+- " : "") << "[" << this->getId() << "]" << " ComboBox " << this->getAPos() << "\n";
        for(size_t i = 0; i < m_List.size(); i++) {
          out << prefix;
          if(space != 0)
          {
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
template <typename T_>
string toString ( const T_ & x )
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
  assert ( sizeof ( CButton ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CPanel ( 12, CRect ( 0.1, 0.3, 0.8, 0.7 ) ) . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) ) );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  CWindow b = a;

  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (118,254.8,384,33.6)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );

  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  CPanel & p = dynamic_cast<CPanel &> ( *b . search ( 12 ) );
  p . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (118,322,384,33.6)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  assert ( toString ( p ) ==
    "[12] Panel (70,154,480,336)\n"
    "+- [20] ComboBox (118,254.8,384,33.6)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (118,322,384,33.6)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [12] Panel (84,186,512,364)\n"
    "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|     +->PA2<\n"
    "|     +- OSY\n"
    "|     +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "         +->PA2<\n"
    "         +- OSY\n"
    "         +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|     +->PA2<\n"
    "|     +- OSY\n"
    "|     +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|  |  +->PA2<\n"
    "|  |  +- OSY\n"
    "|  |  +- Both\n"
    "|  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "|     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "|     |  +- Karate\n"
    "|     |  +- Judo\n"
    "|     |  +- Box\n"
    "|     |  +->Progtest<\n"
    "|     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "|        +->PA2<\n"
    "|        +- OSY\n"
    "|        +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "   |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "   |     +->PA2<\n"
    "   |     +- OSY\n"
    "   |     +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "      |  +->PA2<\n"
    "      |  +- OSY\n"
    "      |  +- Both\n"
    "      +- [12] Panel (208.928,425.148,262.144,124.852)\n"
    "         +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
    "         |  +- Karate\n"
    "         |  +- Judo\n"
    "         |  +- Box\n"
    "         |  +->Progtest<\n"
    "         +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
    "            +->PA2<\n"
    "            +- OSY\n"
    "            +- Both\n" );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [12] Panel (84,186,512,364)\n"
    "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |     +->PA2<\n"
    "   |     +- OSY\n"
    "   |     +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  |  +->PA2<\n"
    "   |  |  +- OSY\n"
    "   |  |  +- Both\n"
    "   |  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "   |     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "   |     |  +- Karate\n"
    "   |     |  +- Judo\n"
    "   |     |  +- Box\n"
    "   |     |  +->Progtest<\n"
    "   |     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "   |        +->PA2<\n"
    "   |        +- OSY\n"
    "   |        +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "      +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "      |  +->PA2<\n"
    "      |  +- OSY\n"
    "      |  +- Both\n"
    "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  |  +- Karate\n"
    "      |  |  +- Judo\n"
    "      |  |  +- Box\n"
    "      |  |  +->Progtest<\n"
    "      |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "      |     +->PA2<\n"
    "      |     +- OSY\n"
    "      |     +- Both\n"
    "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "         +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "         |  +- Karate\n"
    "         |  +- Judo\n"
    "         |  +- Box\n"
    "         |  +->Progtest<\n"
    "         +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "         |  +->PA2<\n"
    "         |  +- OSY\n"
    "         |  +- Both\n"
    "         +- [12] Panel (208.928,425.148,262.144,124.852)\n"
    "            +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
    "            |  +- Karate\n"
    "            |  +- Judo\n"
    "            |  +- Box\n"
    "            |  +->Progtest<\n"
    "            +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
    "               +->PA2<\n"
    "               +- OSY\n"
    "               +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
