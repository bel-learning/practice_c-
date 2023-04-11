// zoo database, various animals, located in pavilions,
//  - need to compute water/meat/hay requirements in pavilions,
//  - different species have different nutrition requirements
// basic solution with abstract base class and several subclasses
//  - objects representing animasl use counted references

#include <iostream>
#include <iomanip>
#include <typeinfo>
using namespace std;

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
      if ( --m_RefCnt == 0 ) 
        delete this;
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
   protected:
    int                 m_AnimalsNr;
    int                 m_AnimalsMax;
    CAnimal          ** m_Animals;
    friend ostream    & operator <<                        ( ostream         & os, 
                                                             const CPavilion & x );
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
void                    CPavilion::Add                     ( CAnimal         * x )
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
  double sum = 0;

  for ( int i = 0; i < m_AnimalsNr; i ++ )
    sum += m_Animals[i] -> Water ();
  return sum;
}
//-------------------------------------------------------------------------------------------------
double                  CPavilion::Hay                     ( void ) const
{
  double sum = 0;

  for ( int i = 0; i < m_AnimalsNr; i ++ )
    sum += m_Animals[i] -> Hay ();
  return sum;
}
//-------------------------------------------------------------------------------------------------
double                  CPavilion::Meat                    ( void ) const
{
  double sum = 0;

  for ( int i = 0; i < m_AnimalsNr; i ++ )
    sum += m_Animals[i] -> Meat ();
  return sum;
}
//-------------------------------------------------------------------------------------------------
ostream    & operator <<                                   ( ostream         & os, 
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
int main ( int argc, char * argv [] )
 {
   CAnimal   * a1, * a2;
   CPavilion    p;

   a1 = new CLion ( "Elsa", 300 );
   a2 = new CElephant ( "Bimbo", 3500 );

   p . Add ( a1 );
   p . Add ( a2 );

   a1 -> Release ();
   a2 -> Release ();

   cout << p;
   return 0;
 }
//-------------------------------------------------------------------------------------------------
