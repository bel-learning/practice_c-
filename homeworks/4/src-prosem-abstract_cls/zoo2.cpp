// manual AddRef/Release manipulation is error prone:
//  - we develop a smart pointer class that does the manipulation automatically
//  - accumulation in CPavilion is redundant. We use method pointers to implement 
//     accumulations only once

#include <iostream>
#include <iomanip>
using namespace std;

//=================================================================================================
template <class T>
class CSmartPtr
{
  public:
                        CSmartPtr                          ( T               * x ) 
                        : m_Val ( x )
    { 
    }
                        CSmartPtr                          ( const CSmartPtr<T> & src ) 
    { 
      m_Val = src . m_Val; 
      m_Val -> AddRef (); 
    } 
                       ~CSmartPtr                          ( void )
    { 
      m_Val -> Release (); 
    }
    CSmartPtr<T>      & operator =                         ( const CSmartPtr<T> & src )
    { 
      src . m_Val -> AddRef ();  // no need to check this != &src: add then release
      m_Val -> Release (); 
      m_Val = src . m_Val; 
      return *this; 
    }                   
    T                 & operator *                         ( void )
    {
      return *m_Val;
    } 
                        operator T *                       ( void )
    {
      return m_Val;
    } 
    T *                 operator ->               ( void )
    {
      return m_Val;
    } 
  private:  
    T                 * m_Val; 
 };
//=================================================================================================
class CAnimal
{
  public:
    //---------------------------------------------------------------------------------------------

                        CAnimal                            ( string            name,
                                                             double            weight )
                        : m_Name ( name ), 
                          m_Weight ( weight ), 
                          m_RefCnt ( 1 )
    {
    }
    //---------------------------------------------------------------------------------------------
    void                AddRef                             ( void )
    {
      m_RefCnt ++;
    }
    //---------------------------------------------------------------------------------------------
    void                Release                            ( void )
    {
      if ( --m_RefCnt == 0 ) delete this;
    }
    //---------------------------------------------------------------------------------------------
    virtual double      Hay                                ( void ) const = 0;
    virtual double      Meat                               ( void ) const = 0;
    virtual double      Water                              ( void ) const = 0;
    //---------------------------------------------------------------------------------------------
  protected:
    string              m_Name;
    double              m_Weight;
    int                 m_RefCnt;
    //---------------------------------------------------------------------------------------------
    virtual             ~CAnimal                           ( void )
    {
    }
    //---------------------------------------------------------------------------------------------
    virtual void        print                              ( ostream         & os ) const = 0;
    //---------------------------------------------------------------------------------------------
    friend ostream    & operator <<                        ( ostream         & os, 
                                                             const CAnimal   & x );
};
//=================================================================================================
class CPredator : public CAnimal
{
  public:
    //---------------------------------------------------------------------------------------------
                        CPredator                          ( string            name,
                                                             double            weight )
                        : CAnimal ( name, weight )
    {
    }
    //---------------------------------------------------------------------------------------------
    virtual double      Hay                                ( void ) const
    {
      return 0;
    }
    //---------------------------------------------------------------------------------------------
    virtual double      Water                              ( void ) const
    {
      return m_Weight * 0.01;
    }
    //---------------------------------------------------------------------------------------------
};
//=================================================================================================
class CHerbivore : public CAnimal
{
  public:
    //---------------------------------------------------------------------------------------------
                        CHerbivore                         ( string            name,
                                                             double            weight )
                        : CAnimal ( name, weight )
    {
    }                                   
    //---------------------------------------------------------------------------------------------
    virtual double      Meat                               ( void ) const
    {
      return 0;
    }
    //---------------------------------------------------------------------------------------------
    virtual double      Water                              ( void ) const
    {
      return m_Weight * 0.02;
    }
    //---------------------------------------------------------------------------------------------
};
//=================================================================================================
class CLion : public CPredator
{
  public:
    //---------------------------------------------------------------------------------------------
                        CLion                              ( string            name,
                                                             double            weight )
                        : CPredator ( name, weight )
    {   
    }
    //---------------------------------------------------------------------------------------------
    virtual double      Meat                               ( void ) const
    {
      return 3;
    }
    //---------------------------------------------------------------------------------------------
   protected:
    //---------------------------------------------------------------------------------------------
    virtual void        print                              ( ostream         & os ) const
    {
      os << "Lion " << m_Name << " " << m_Weight << "kg" << endl;
    }
    //---------------------------------------------------------------------------------------------
 };
//=================================================================================================
class CTiger : public CPredator
{
  public:
    //---------------------------------------------------------------------------------------------
                        CTiger                             ( string            name,
                                                             double            weight )
                        : CPredator ( name, weight )
    {
    }
    //---------------------------------------------------------------------------------------------
    virtual double      Meat                               ( void ) const
    {
      return 2;
    }
    //---------------------------------------------------------------------------------------------
  protected:
    //---------------------------------------------------------------------------------------------
    virtual void        print                              ( ostream         & os ) const
    {
      os << "Tiger " << m_Name << " " << m_Weight << "kg" << endl;
    }
    //---------------------------------------------------------------------------------------------
};
//=================================================================================================
class CElephant : public CHerbivore
{
  public:
    //---------------------------------------------------------------------------------------------
                        CElephant                          ( string            name,
                                                             double            weight )
                        : CHerbivore ( name, weight )
    {
    }
    //---------------------------------------------------------------------------------------------
    virtual double      Hay                                ( void ) const
    {
      return 20;
    }
    //---------------------------------------------------------------------------------------------
  protected:
    //---------------------------------------------------------------------------------------------
    virtual void        print                              ( ostream         & os ) const
    {
      os << "Elephant " << m_Name << " " << m_Weight << "kg" << endl;
    }
    //---------------------------------------------------------------------------------------------
};
//=================================================================================================
class CZebra : public CHerbivore
{
  public:
    //---------------------------------------------------------------------------------------------
                        CZebra                             ( string            name,
                                                             double            weight )
                        : CHerbivore ( name, weight )
    {
    }
    //---------------------------------------------------------------------------------------------
    virtual double      Hay                                ( void ) const
    {
      return 3;
    }
    //---------------------------------------------------------------------------------------------
  protected:
    //---------------------------------------------------------------------------------------------
    virtual void        print                              ( ostream         & os ) const
    {
      os << "Zebra " << m_Name << " " << m_Weight << "kg" << endl;
    }
    //---------------------------------------------------------------------------------------------
};
//=================================================================================================
class CPavilion 
{
  public:
                        CPavilion                          ( void );
                       ~CPavilion                          ( void );
    void                Add                                ( CAnimal         * x );
    double              Water                              ( void ) const;
    double              Hay                                ( void ) const;
    double              Meat                               ( void ) const;
  private:
    int                 m_AnimalsNr;
    int                 m_AnimalsMax;
    CAnimal          ** m_Animals;
    double              accumulate                         ( double (CAnimal::*methPtr) ( void ) const ) const;
    friend ostream    & operator <<                        ( ostream & os, const CPavilion & x );
};
//=================================================================================================
                        CPavilion::CPavilion               ( void )
{
  m_AnimalsNr = 0;
  m_AnimalsMax = 0;
  m_Animals = nullptr;
}                        
//-------------------------------------------------------------------------------------------------
                        CPavilion::~CPavilion              ( void )
{
  for ( int i = 0; i < m_AnimalsNr; i ++ )
    m_Animals[i] -> Release ();
  delete [] m_Animals;
}                        
//-------------------------------------------------------------------------------------------------
void                    CPavilion::Add                     ( CAnimal * x ) 
{
  if ( m_AnimalsNr >= m_AnimalsMax )
  {
    m_AnimalsMax += 100;
    CAnimal ** tmp = new CAnimal * [m_AnimalsMax];
    for ( int i = 0; i < m_AnimalsNr; i ++ )
      tmp[i] = m_Animals[i];
    delete [] m_Animals;
    m_Animals = tmp;
  }
  m_Animals[m_AnimalsNr ++] = x;
  x -> AddRef ();
}
//-------------------------------------------------------------------------------------------------
double                  CPavilion::Water                   ( void ) const
{
  return accumulate ( &CAnimal::Water );
}
//-------------------------------------------------------------------------------------------------
double                  CPavilion::Hay                     ( void ) const
{
  return accumulate ( &CAnimal::Hay );
}
//-------------------------------------------------------------------------------------------------
double                  CPavilion::Meat                    ( void ) const
{
  return accumulate ( &CAnimal::Meat );
}
//-------------------------------------------------------------------------------------------------
double                  CPavilion::accumulate              ( double (CAnimal::*methPtr) ( void ) const ) const
{
  double sum = 0;
   
  for ( int i = 0; i < m_AnimalsNr; i ++ )
    sum += (m_Animals[i] ->* methPtr) ( );
  return sum; 
}    
//-------------------------------------------------------------------------------------------------
ostream               & operator <<                        ( ostream         & os, 
                                                             const CPavilion & x )
{
  for ( int i = 0; i < x . m_AnimalsNr; i ++ )
    os << (*x . m_Animals[i] );
   
  os << "Hay "   << x . Hay () << endl;
  os << "Meat "  << x . Meat () << endl;
  os << "Water " << x . Water () << endl;
  return os;   
}
//=================================================================================================
ostream               & operator <<                        ( ostream         & os, 
                                                             const CAnimal   & x )
{
  x . print ( os );
  return os;
}
//=================================================================================================
int                      main                              ( int               argc, 
                                                             char            * argv [] )
{
  CSmartPtr<CAnimal> a1 ( new CLion ( "Elsa", 300 ));
  CSmartPtr<CAnimal> a2 ( new CElephant ( "Bimbo", 3500 ));
  CPavilion    p;

  p . Add ( a1 );
  p . Add ( a2 );
  
  cout << p;
 
  return 0;  
}
                      