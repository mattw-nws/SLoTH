#include "gtest/gtest.h"

#include <sloth.hpp>
#include <string>

class Sloth_Test {

protected:

    Sloth_Test()
    {

    }

    ~Sloth_Test() {

    }
};

//! Test that a SLoTH can be created.
TEST(Sloth_Test, TestSlothConstruction)
{
  //Test basic construction, SLoTH should have zero outputs.
  auto s = Sloth();
  ASSERT_EQ( s.GetOutputItemCount(), 0 );

}


TEST(Sloth_Test, TestSlothAddConstantDefaultsOutput)
{
  auto s = Sloth();
  double v = 42.0;
  s.SetValue( "adouble", &v );

  v = 0.0;
  s.GetValue("adouble", &v);
  ASSERT_EQ( s.GetOutputItemCount(), 1 );
  ASSERT_EQ( s.GetInputItemCount(), 0 );
  ASSERT_STREQ( s.GetVarType("adouble").c_str(), "double" );
  ASSERT_STREQ( s.GetVarUnits("adouble").c_str(), "1" );
  ASSERT_EQ( v, 42.0 );

}

TEST(Sloth_Test, TestSlothAddConstantIntOutput)
{
  auto s = Sloth();
  int v = 1138;
  s.SetValue( "anint(1,int)", &v );

  v = 0;
  s.GetValue("anint", &v);
  ASSERT_EQ( s.GetOutputItemCount(), 1 );
  ASSERT_EQ( s.GetInputItemCount(), 0 );
  ASSERT_STREQ( s.GetVarType("anint").c_str(), "int" );
  ASSERT_STREQ( s.GetVarUnits("anint").c_str(), "1" );
  ASSERT_EQ( v, 1138 );

}

TEST(Sloth_Test, TestSlothAddConstantDoubleArrayOutput)
{
  auto s = Sloth();
  double v[] = { 42.0, 43.0 };
  s.SetValue( "somedoubles(2,double)", v );

  v[0] = v[1] = 0.0;
  s.GetValue("somedoubles", v);
  ASSERT_EQ( s.GetOutputItemCount(), 1 );
  ASSERT_EQ( s.GetInputItemCount(), 0 );
  ASSERT_EQ( s.GetVarNbytes("somedoubles"), sizeof(double) * 2 );
  ASSERT_STREQ( s.GetVarType("somedoubles").c_str(), "double" );
  ASSERT_STREQ( s.GetVarUnits("somedoubles").c_str(), "1" );
  ASSERT_EQ( v[0], 42.0 );
  ASSERT_EQ( v[1], 43.0 );

}

TEST(Sloth_Test, TestSlothGetValueAtIndicesDouble){
  auto s = Sloth();
  double v[] = { 42.0, 43.0, 44.0, 45.0, 46.0 };
  s.SetValue( "somedoubles(5,double)", v );

  double v2[3] = { 0.0, 0.0, 0.0 };
  int inds[3] = { 0, 2, 4 };
  s.GetValueAtIndices("somedoubles", v2, inds, 3);
  ASSERT_EQ( s.GetVarNbytes("somedoubles"), sizeof(double) * 5 );
  ASSERT_STREQ( s.GetVarType("somedoubles").c_str(), "double" );
  ASSERT_EQ( v2[0], 42.0 );
  ASSERT_EQ( v2[1], 44.0 );
  ASSERT_EQ( v2[2], 46.0 );
}

TEST(Sloth_Test, TestSlothSetValueAtIndicesDouble){
  auto s = Sloth();
  double v[] = { 0.0, 43.0, 0.0, 45.0, 0.0 };
  s.SetValue( "somedoubles(5,double)", v );

  double v2[3] = { 42.0, 44.0, 46.0 };
  int inds[3] = { 0, 2, 4 };
  double v3[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
  s.SetValueAtIndices("somedoubles", inds, 3, v2);
  s.GetValueAtIndices("somedoubles", v3, inds, 3);
  ASSERT_EQ( s.GetVarNbytes("somedoubles"), sizeof(double) * 5 );
  ASSERT_STREQ( s.GetVarType("somedoubles").c_str(), "double" );
  ASSERT_NE( v[0], 42.0 );
  ASSERT_NE( v[1], 44.0 );
  ASSERT_NE( v[2], 46.0 );
  ASSERT_EQ( v3[0], 42.0 );
  ASSERT_EQ( v3[1], 44.0 );
  ASSERT_EQ( v3[2], 46.0 );
}

TEST(Sloth_Test, TestSlothSetInputAlias){
  auto s = Sloth();
  double v = 42.0;
  s.SetValue("adouble(1,double,K,node,alias)", &v);

  ASSERT_EQ( s.GetOutputItemCount(), 1 );
  ASSERT_EQ( s.GetInputItemCount(), 1 );
  ASSERT_EQ( s.GetVarNbytes("adouble"), sizeof(double)*1 );
  ASSERT_STREQ( s.GetVarType("adouble").c_str(), "double" );
  ASSERT_STREQ( s.GetVarUnits("adouble").c_str(), "K" );
  ASSERT_STREQ( s.GetVarLocation("adouble").c_str(), "node" );

  v = 0.0;
  s.GetValue("adouble", &v);
  ASSERT_EQ( v, 42.0 );

  v = 0.0;
  s.GetValue("alias", &v);
  ASSERT_EQ( v, 42.0 );

  ASSERT_EQ( s.GetVarNbytes("alias"), sizeof(double)*1 );
  ASSERT_STREQ( s.GetVarType("alias").c_str(), "double" );
  ASSERT_STREQ( s.GetVarUnits("alias").c_str(), "K" );
  ASSERT_STREQ( s.GetVarLocation("alias").c_str(), "node" );

}

