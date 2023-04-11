// add operator ==, i.e. compare two instances are the same species (and attributes)
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
using namespace std;

//=================================================================================================
class CAnimal
{
  public:
    //---------------------------------------------------------------------------------------------

                        CAnimal                            ( string            name,
                                                             double            weight )
                        : m_Name ( name ), 
                          m_Weight ( weight )
    {
    }
    //---------------------------------------------------------------------------------------------
    virtual             ~CAnimal                           ( void )
    {
    }
    //---------------------------------------------------------------------------------------------
    bool                operator ==                        ( const CAnimal   & x ) const
    {
      return compare ( x ) && x . compare ( *this ); // need both comparisons, see Lion vs Lion2 below
    }
    //---------------------------------------------------------------------------------------------
    virtual double      Hay                                ( void ) const = 0;
    virtual double      Meat                               ( void ) const = 0;
    virtual double      Water                              ( void ) const = 0;
    //---------------------------------------------------------------------------------------------
  protected:
    string              m_Name;
    double              m_Weight;
    //---------------------------------------------------------------------------------------------
    virtual void        print                              ( ostream         & os ) const = 0;
    virtual bool        compare                            ( const CAnimal   & x ) const = 0;
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
    virtual bool        compare                            ( const CAnimal & x ) const
    {
      const CLion * rhs = dynamic_cast<const CLion *> ( &x );
      return rhs && m_Name == rhs -> m_Name && m_Weight == rhs -> m_Weight;
    }
    //---------------------------------------------------------------------------------------------
    virtual void        print                              ( ostream         & os ) const
    {
      os << "Lion " << m_Name << " " << m_Weight << "kg" << endl;
    }
    //---------------------------------------------------------------------------------------------
 };
//=================================================================================================
class CLion2 : public CLion
{
  public:
    //---------------------------------------------------------------------------------------------
                        CLion2                              ( string            name,
                                                             double            weight )
                        : CLion ( name, weight )
    {   
    }
    //---------------------------------------------------------------------------------------------
   protected:
    //---------------------------------------------------------------------------------------------
    virtual bool        compare                            ( const CAnimal & x ) const
    {
      const CLion2 * rhs = dynamic_cast<const CLion2 *> ( &x );
      return rhs && m_Name == rhs -> m_Name && m_Weight == rhs -> m_Weight;
    }
    //---------------------------------------------------------------------------------------------
    virtual void        print                              ( ostream         & os ) const
    {
      os << "Lion2 " << m_Name << " " << m_Weight << "kg" << endl;
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
    virtual bool        compare                            ( const CAnimal & x ) const 
    {
      const CTiger * rhs = dynamic_cast<const CTiger *> ( &x );
      return rhs && m_Name == rhs -> m_Name && m_Weight == rhs -> m_Weight;
    }
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
    virtual bool        compare                            ( const CAnimal & x ) const 
    {
      const CElephant * rhs = dynamic_cast<const CElephant *> ( &x );
      return rhs && m_Name == rhs -> m_Name && m_Weight == rhs -> m_Weight;
    }
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
    virtual bool        compare                            ( const CAnimal & x ) const 
    {
      const CZebra * rhs = dynamic_cast<const CZebra *> ( &x );
      return rhs && m_Name == rhs -> m_Name && m_Weight == rhs -> m_Weight;
    }
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
    void                Add                                ( unique_ptr<CAnimal> && x );
    double              Water                              ( void ) const;
    double              Hay                                ( void ) const;
    double              Meat                               ( void ) const;
  private:
    vector<unique_ptr<CAnimal> >  m_Animals;
    double              accumulate                         ( double (CAnimal::*methPtr) ( void ) const ) const;
    friend ostream    & operator <<                        ( ostream         & os, 
                                                             const CPavilion & x );
};
//=================================================================================================
void                    CPavilion::Add                     ( unique_ptr<CAnimal> && x )
{
  m_Animals . push_back ( move(x) );
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

  for ( auto & x : m_Animals )
    sum += ((*x) .* methPtr) ( );
  return sum;
}
//-------------------------------------------------------------------------------------------------
ostream    & operator <<                                   ( ostream         & os, 
                                                             const CPavilion & x )
{
  for ( auto & y : x . m_Animals )
    os << *y;

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
  CPavilion    p;

  p . Add ( unique_ptr<CAnimal> ( new CLion ( "Elsa", 300 ) ) );
  p . Add ( unique_ptr<CAnimal> ( new CElephant ( "Bimbo", 3500 ) ) );

  cout << p;


  unique_ptr<CAnimal> a ( new CLion  ( "Test", 300 ) );
  unique_ptr<CAnimal> b ( new CLion  ( "Test", 300 ) );
  unique_ptr<CAnimal> c ( new CTiger ( "Test", 300 ) );
  unique_ptr<CAnimal> d ( new CLion2 ( "Test", 300 ) );
  
  

  cout << "a == b " << (*a==*b) << endl;
  cout << "a == c " << (*a==*c) << endl;
  cout << "a == d " << (*a==*d) << endl;
  cout << "d == a " << (*d==*a) << endl;

  return 0;
}
