
// =====================================================

#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestListener.h>
#include <cppunit/TestResult.h>

#include <iostream>

// =========================================================

bool runAllTests()
{

  CppUnit::Test *Suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  CppUnit::TextUi::TestRunner Runner;
  Runner.addTest( Suite );

  Runner.setOutputter( new CppUnit::CompilerOutputter( &Runner.result(), std::cerr ) );

  bool Sucess = Runner.run();

  return Sucess;
}

// =========================================================

int main()
{
  return runAllTests() ? 0 : 1;
}

// =====================================================

