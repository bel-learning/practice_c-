#ifndef __PROGTEST__
#include <cstring>
using namespace std;

class CLinkedSetTester;
#endif /* __PROGTEST__ */


class CLinkedSet
{
private:
    struct CNode
    {
        CNode * m_Next;
        char * m_Data;
        const char * Value () const {
            return m_Data;
        };
        void addValue(const char * value) {
            size_t len = strlen(value) + 1;
            m_Data = new char[len];
            strcpy(m_Data, value);
        }
      
    };

    CNode * m_Begin;

    

public:
    // default constructor
    CLinkedSet() {
        m_Begin = nullptr;
    }

    // copy constructor
    CLinkedSet(const CLinkedSet & list) {
        if(list.m_Begin == nullptr) {
            m_Begin = nullptr;
            return;
        }
        CNode * tmp = list.m_Begin;
        CNode * newHead = new CNode;
        CNode * newCurrent = newHead;
       
        while(tmp->m_Next != nullptr) {
            newCurrent->m_Next = new CNode;
            newCurrent->addValue(tmp->m_Data);
            newCurrent = newCurrent->m_Next;
            tmp = tmp->m_Next;
        }
        newCurrent->addValue(tmp->m_Data);
        newCurrent->m_Next = nullptr;
        m_Begin = newHead;
    }
   
    // operator=
    CLinkedSet & operator = (const CLinkedSet & list) {
        if(this == &list) {
            return *this;
        }
        while (m_Begin != nullptr) {
            CNode* current = m_Begin;
            m_Begin = m_Begin->m_Next;
            delete []current->m_Data;
            delete current;
        }
        CNode* src = list.m_Begin;
        CNode* dest = nullptr;
        CNode* prev = nullptr;
        while (src != nullptr) {
            CNode* node = new CNode;
            node->addValue(src->m_Data);
            node->m_Next = nullptr;
            if (prev == nullptr) {
                dest = node;
            } else {
                prev->m_Next = node;
            }
            prev = node;
            src = src->m_Next;
        }
        m_Begin = dest;
            
        return *this;
    }
    // destructor
    ~CLinkedSet() {
        CNode * cur = m_Begin;
        while(cur != nullptr) {
            CNode * tmp = cur;
            cur = cur->m_Next;
            delete [] tmp->m_Data;
            delete tmp;
        }
        cur = nullptr;

    }


    bool Insert ( const char * value ) {
        CNode * current = m_Begin;
        if(current == nullptr) {
            current = new CNode;
            current->addValue(value);
            current->m_Next = nullptr;
            m_Begin = current;
            return true;
        }
        
        // at the start
        if(strcmp(value, current->m_Data) == 0)
            return false;
        if(strcmp(value, current->m_Data) < 0) {
            CNode * new_node = new CNode;
            new_node->addValue(value);

            new_node->m_Next = current;
            m_Begin = new_node;
            return true;
        }
        CNode * previous = current;
        while (current != nullptr) {
            int state = strcmp(value, current->m_Data);
            if (state == 0)
                return false;
            if (state < 0) {
                CNode* new_node = new CNode;
                new_node->addValue(value);
                new_node->m_Next = current;
                previous->m_Next = new_node;
                return true;
            }
            previous = current;
            current = current->m_Next;
        }

        CNode* new_node = new CNode;
        new_node->addValue(value);
        new_node->m_Next = nullptr;
        previous->m_Next = new_node;
        return true;
    };
    
    bool Remove ( const char * value ) {
        if(m_Begin == nullptr) return false;
        CNode * head = m_Begin;
        CNode* current = head; // the first valid node
        CNode* prev = current; // empty header
        while(current != nullptr) {
            if(strcmp(current->m_Data,value) == 0) { 
                break; 
            }
            else {
                prev = current; 
                current = current->m_Next; // go to next value
            }
        }
        if(current == nullptr) { // if we reached end of list or the list is empty
            return false;
        } else {
            prev->m_Next = current->m_Next; 
            delete [] current->m_Data;
            delete current; // delete the node
            return true;
        }
    };

    bool Empty () const {
        if(m_Begin == nullptr) return true;
        return false;
    };

    size_t Size () const {
        size_t counter = 0;
        CNode * cur = m_Begin;
        while(cur != nullptr) {
            counter++;
            cur = cur->m_Next;
        }
        return counter;
    };

    bool Contains ( const char * value ) const {
        CNode * tmp = m_Begin;
        while(tmp != nullptr) {
            if(strcmp(tmp->m_Data, value) == 0) {
                return true;
            }
            tmp = tmp->m_Next;
        } 
        return false;
    };

    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__
#include <cassert>

struct CLinkedSetTester
{
    static void test0()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );

        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        assert( !x0 . Empty() );

    }

    static void test1()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CLinkedSet x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CLinkedSet x0;
        CLinkedSet x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }
    static void test3()
    {
        CLinkedSet x0;
        assert(! x0.Remove("Blablbal"));
        assert( x0.Size() == 0);
        assert( x0.Insert("c"));
        assert( !x0.Remove("b"));
        assert (x0.Insert("b"));
        assert (x0.Insert("a"));

        CLinkedSet x1;
        CLinkedSet x2;
        assert(x2.Insert("a"));
        assert(x1.Insert("p"));
        x2 = x1 = x2;
        assert(!x2.Empty());
        assert(!x1.Empty());

        assert(x2.Contains("a"));
        assert(x1.Contains("a"));
        
        assert(x1.m_Begin != x2.m_Begin);
        x1 = x1;
    }

};

int main ()
{
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    CLinkedSetTester::test3();


    return 0;
}
#endif /* __PROGTEST__ */
