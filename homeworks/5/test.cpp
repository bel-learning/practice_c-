#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;


class CDate
{
  public:
    //---------------------------------------------------------------------------------------------
                             CDate                         ( int               y,
                                                             int               m,
                                                             int               d )
      : m_Year ( y ),
        m_Month ( m ),
        m_Day ( d )
    {
    }
    //---------------------------------------------------------------------------------------------
    int                      compare                       ( const CDate     & x ) const
    {
      if ( m_Year != x . m_Year )
        return m_Year - x . m_Year;
      if ( m_Month != x . m_Month )
        return m_Month - x . m_Month;
      return m_Day - x . m_Day;
    }
    //---------------------------------------------------------------------------------------------
    int                      year                          ( void ) const 
    {
      return m_Year;
    }
    //---------------------------------------------------------------------------------------------
    int                      month                         ( void ) const 
    {
      return m_Month;
    }
    //---------------------------------------------------------------------------------------------
    int                      day                           ( void ) const 
    {
      return m_Day;
    }
    //---------------------------------------------------------------------------------------------
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CDate     & x )
    {
      char oldFill = os . fill ();
      return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Month ) << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Day ) 
                << setfill ( oldFill );
    }
    //---------------------------------------------------------------------------------------------
  private:
    int16_t                  m_Year;
    int8_t                   m_Month;
    int8_t                   m_Day;
};
#endif /* __PROGTEST__ */


std::string stripWhiteSpace(const std::string& str) {
    // Find the first non-whitespace character.
    auto first = std::find_if(str.begin(), str.end(), [](int c) {
        return !std::isspace(c);
    });

    // Find the last non-whitespace character.
    auto last = std::find_if(str.rbegin(), str.rend(), [](int c) {
        return !std::isspace(c);
    });

    // If the string is entirely whitespace, return an empty string.
    if (first == str.end() || last == str.rend())
        return "";

    // Convert the non-whitespace section of the string to a new string.
    std::string result(first, last.base());

    // Reduce all contiguous whitespace to a single space.
    auto newEnd = std::unique(result.begin(), result.end(), [](char a, char b) {
        return std::isspace(a) && std::isspace(b);
    });

    // Erase the excess whitespace from the end of the string.
    result.erase(newEnd, result.end());

    return result;
}

string to_lower(const string & name) {
  string new_str = name;
  transform(new_str.begin(), new_str.end(), new_str.begin(),
      [](unsigned char c){return std::tolower(c);});
  return new_str;
}

string normalize(const string & name)  {
    // cout << "Normalize: " << endl;
    string normal = name;
    std::transform(normal.begin(), normal.end(), normal.begin(),
      [](unsigned char c){return std::tolower(c);});

    normal = stripWhiteSpace(normal);
    // cout << "orig: " << name << " normal: >" << normal  << "<" << endl;
    // cout << "End: " << endl;
    return normal;
  }

class CInvoice
{
  public:
    int _order;

                             CInvoice                      ( const CDate     & date,
                                                             const string    & seller,
                                                             const string    & buyer,
                                                             unsigned int      amount,
                                                             double            vat ) 
    : _date(date), _seller(seller), _buyer(buyer), _amount(amount), _vat(vat) {};
                      
    CDate                    date                          ( void ) const {return _date;};
    const string       &            buyer                         ( void ) const {return _buyer;};
    const string       &            seller                        ( void ) const {return _seller;};
    unsigned int             amount                        ( void ) const {return _amount;};
    double                   vat                           ( void ) const {return _vat;};
    // string                   hashable                           ( void ) const {return _hashable;};

    friend ostream & operator << (ostream & out, const CInvoice & invoice) {
      out << invoice.date() << " " << invoice.seller()  << " " <<  invoice.buyer()  << " " << invoice.amount() << " " <<  invoice.vat() << " " << invoice._order << endl;
      return out;
    }

    void changeBuyer(const string & buyer) {
      _buyer = buyer;
    }
    void changeSeller(const string & seller) {
      _seller = seller;
    }


    bool operator == (const CInvoice & other) const {
      if(_date.compare(other._date) != 0) return false;
      if(_amount != other._amount) return false;
      if(_vat != other._vat) return false;
      if(_seller != other._seller) return false;
      if(_buyer != other._buyer) return false;
      
      return true;
    }

  private:
    // todo
    CDate _date;
    string _seller;
    string _buyer;
    unsigned int _amount;
    double _vat;
    // string _hashable;
};

template<typename T1>
struct InvoiceHash {
  auto operator()(const CInvoice & val) const {
    size_t h1 = std::hash<int>()(val.date().year());
    size_t h2 = std::hash<int>()(val.date().day());
    size_t h3 = std::hash<int>()(val.date().month());
    size_t h4 = std::hash<string>()(val.seller());
    size_t h5 = std::hash<string>()(val.buyer());
    size_t h6 = std::hash<int>()(val.amount());
    size_t h7 = std::hash<double>()(val.vat());
    return h1 ^ h2 ^ h3 ^ h4 ^ h5 ^ h6 ^ h7;
  }
};
template<typename T1>
struct InvoiceEqual {
  bool operator()(const CInvoice & a, const CInvoice & b) const {
    return a == b;
  }
};

class Company {
  public:
  Company() {};
  Company(const string & name) : _original_name(name) {};
  Company(const Company & other):  _original_name(other._original_name), _issued(other._issued), _accepted(other._accepted) { };
  
  string original_name () const  {
    return _original_name;
  }
   string &  original_name () {
    return _original_name;
  }
  
  unordered_map<CInvoice, bool , InvoiceHash<CInvoice>, InvoiceEqual<CInvoice> >  & issued () {
    return _issued;
  }
  unordered_map<CInvoice, bool , InvoiceHash<CInvoice>, InvoiceEqual<CInvoice> >  & accepted () {
    return _accepted;
  }
  const unordered_map<CInvoice, bool , InvoiceHash<CInvoice>, InvoiceEqual<CInvoice> >  & issued () const {
    return _issued;
  }

  const unordered_map<CInvoice, bool , InvoiceHash<CInvoice>, InvoiceEqual<CInvoice> >  & accepted () const {
    return _accepted;
  }

  private:
  string _original_name;
  unordered_map<CInvoice, bool , InvoiceHash<CInvoice>,  InvoiceEqual<CInvoice> > _issued;
  unordered_map<CInvoice, bool , InvoiceHash<CInvoice>,  InvoiceEqual<CInvoice> > _accepted;
};


class CSortOpt
{
  public:
    static const int         BY_DATE                       = 0;
    static const int         BY_BUYER                      = 1;
    static const int         BY_SELLER                     = 2;
    static const int         BY_AMOUNT                     = 3;
    static const int         BY_VAT                        = 4;
                             CSortOpt                      ( void ) {};
    CSortOpt               & addKey                        ( int               key,
                                                             bool              ascending = true ) {
                                                              _key.push_back(make_pair(key, ascending));
                                                              return *this;
                                                             };

    const vector<pair<int, bool>> & key(void) const {
      return _key;
    }
  private:
    // todo
    vector<pair<int,bool> > _key;
};

class CVATRegister
{
  public:
                             CVATRegister                  ( void ) : order(0) {};
    bool                     registerCompany               ( const string    & name ) {
      string normalized_name = normalize(name);
      if(companyExists(normalized_name)) return false;
      Company new_comp(name);
      storage[normalized_name] = new_comp;
      return true;
    };


    bool                     addIssued                     ( const CInvoice  & x ) {
      // cout << "Adding issues" << endl;
      string n_buyer = normalize(x.buyer());
      string n_seller = normalize(x.seller());
      if(n_buyer == n_seller) return false;
      if(!companyExists(n_buyer)) return false;
      if(!companyExists(n_seller)) return false;
     order++;
     CInvoice copy_invoice(x);
     copy_invoice.changeBuyer(storage[n_buyer].original_name());
     copy_invoice.changeSeller(storage[n_seller].original_name());

     copy_invoice._order = order;
    //  copy_invoice.makeHashable();
      if(issueExists(copy_invoice, n_buyer)) return false;
      if(issueExists(copy_invoice, n_seller)) return false;

     storage[n_buyer].issued()[copy_invoice] = true;
     storage[n_seller].issued()[copy_invoice] = true;
  
     return true;
      
    };
    bool                     addAccepted                   ( const CInvoice  & x ) {
      // cout << "Adding accepted" << endl;
      string n_buyer = normalize(x.buyer());
      string n_seller = normalize(x.seller());
      if(n_buyer == n_seller) return false;
      if(!companyExists(n_buyer)) return false;
      if(!companyExists(n_seller)) return false;
      CInvoice copy_invoice(x);
      copy_invoice.changeBuyer(storage[n_buyer].original_name());
      copy_invoice.changeSeller(storage[n_seller].original_name());
      order++;
      copy_invoice._order = order;
      // copy_invoice.makeHashable();
        if(acceptExists(copy_invoice, n_buyer)) return false;
        if(acceptExists(copy_invoice, n_seller)) return false;

      storage[n_buyer].accepted()[copy_invoice] = 1;
      storage[n_seller].accepted()[copy_invoice] = 1;
      return true;
    };
    bool                     delIssued                     ( const CInvoice  & x ) {
      string n_buyer = normalize(x.buyer());
      string n_seller = normalize(x.seller());
      if(n_buyer == n_seller) return false;
      if(!companyExists(n_buyer)) return false;
      if(!companyExists(n_seller)) return false;
      CInvoice copy_invoice(x);
      copy_invoice.changeBuyer(storage[n_buyer].original_name());
      copy_invoice.changeSeller(storage[n_seller].original_name());
    //  copy_invoice.makeHashable();
     if(!issueExists(copy_invoice, n_buyer)) return false;
      if(!issueExists(copy_invoice, n_seller)) return false;

      storage[n_buyer].issued().erase(copy_invoice);
      storage[n_seller].issued().erase(copy_invoice);
     return true;
    };
    bool                     delAccepted                   ( const CInvoice  & x ) {
      string n_buyer = normalize(x.buyer());
      string n_seller = normalize(x.seller());
      if(n_buyer == n_seller) return false;
      if(!companyExists(n_buyer)) return false;
      if(!companyExists(n_seller)) return false;
      CInvoice copy_invoice(x);
     copy_invoice.changeBuyer(storage[n_buyer].original_name());
     copy_invoice.changeSeller(storage[n_seller].original_name());
    //  copy_invoice.makeHashable();
     if(!acceptExists(copy_invoice, n_buyer)) return false;
      if(!acceptExists(copy_invoice, n_seller)) return false;

      storage[n_buyer].accepted().erase(copy_invoice);
      storage[n_seller].accepted().erase(copy_invoice);
     return true;
    };
    list<CInvoice>           unmatched                     ( const string    & company,
                                                             const CSortOpt  & sortBy ) const {
    // vector<CInvoice> arr;
      string n_company = normalize(company); 
      if(storage.find(n_company) == storage.end()) return list<CInvoice>();
      list<CInvoice> unmatched_list;
     for(auto & it : storage.at(n_company).issued()) {
        if(!acceptExists(it.first, n_company)) {
          unmatched_list.push_back(it.first);
        }
     }
     for(auto & it : storage.at(n_company).accepted()) {
        if(!issueExists(it.first, n_company)) {
          unmatched_list.push_back(it.first);
        }
     }
     vector<pair<int, bool > > keys = sortBy.key();
     
    unmatched_list.sort([keys] (const CInvoice & a, const CInvoice & b) {
      for(size_t i = 0; i < keys.size(); i++) {
        int coefficient = 1;
        int cmp = 0;
        if(keys[i].second)
          coefficient = 1;
        else
          coefficient = -1;
        switch(keys[i].first) { 
          case 0:
            cmp = a.date().compare(b.date());
            if(cmp == 0) 
              break;
            return cmp * coefficient < 0 ? true : false;
          case 1:
            cmp = to_lower(a.buyer()).compare(to_lower(b.buyer()));
            if(cmp == 0) 
              break;
            return cmp * coefficient < 0 ? true : false;
          case 2:
            cmp = to_lower(a.seller()).compare(to_lower(b.seller()));
          
            if(cmp == 0) 
              break;
            return cmp * coefficient < 0 ? true : false;
          case 3:
            cmp = a.amount() - b.amount();
            if(cmp == 0) 
              break;
            return cmp * coefficient < 0 ? true : false;
          case 4:
            cmp = a.vat() - b.vat();
            if(cmp == 0) 
              break;
            return cmp * coefficient < 0 ? true : false;
        };
      }
    
      return (a._order < b._order);
    });
      // cout << "----------------" << endl << "iterating through " << endl;

  
     return unmatched_list;
    };
    // 
    

    bool companyExists(const string & name) const {
      if(storage.find(name) != storage.end()) return true;
      return false;
    }
    Company & getCompany(const string & name) {
      string normalized_name = normalize(name);
      return storage.at(normalized_name);
    }
    bool issueExists(const CInvoice & n_invoice, const string & n_company) const {
      if(!companyExists(n_company)) return false;
      if(storage.at(n_company).issued().find(n_invoice) == storage.at(n_company).issued().end()) return false;
      // if(storage.at(n_company).issued().at(n_invoice) == 0) return false;
      return true;
    }
    bool acceptExists(const CInvoice & n_invoice, const string & n_company) const {
      if(!companyExists(n_company)) return false;
      if(storage.at(n_company).accepted().find(n_invoice) == storage.at(n_company).issued().end()) return false;
      // if(storage.at(n_company).accepted().at(n_invoice) == 0) return false;
      return true;
    }
  private:
    unordered_map<string, Company > storage;
    int order;
    // todo
};

#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{
  // todo
  if(a.size() != b.size()) return false;
  list<CInvoice>::const_iterator it1 = a.begin();
  list<CInvoice>::const_iterator it2 = b.begin();
  while(it1 != a.end() && it2 != b.end()) {
    if(!(*it1 == *it2)) return false;
    it1++;
    it2++;
  }
  return true;
}

int main ( void )
{
  CVATRegister r;
  assert ( r . registerCompany ( "first Company" ) );
  assert (! r . registerCompany ( "first Company" ) );

  assert ( r . registerCompany ( "Second     Company" ) );
  assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
  assert ( r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
  // cout << "Checking unmatched" << endl;
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
           list<CInvoice>
           {
           } ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );



  
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
