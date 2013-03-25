#!/usr/bin/perl
use strict;
use warnings;

sub readFile 
{
  my $FileName = shift;
  open my $FD, "<", $FileName
    or die "Can not open $FileName : $!";
  my @Content = map { chomp; $_; } <$FD>;
  close $FD;
  return @Content;
}

my %FileNames;

for my $File ( @ARGV )
{
  $File =~ s/\.(h|cpp)$//;
  $FileNames{$File}++;
}

for my $Name ( keys %FileNames )
{
  my @FileH = readFile $Name . ".h";
  my @FileCpp = readFile $Name . ".cpp";

  my @TestSuites = grep { defined; } map { /CPPUNIT_TEST_SUITE\(\s*(.*?)\s*\)/ ? $1 : undef; } @FileH;
  my @RegisteredSuites = grep { defined; } map { /CPPUNIT_TEST_SUITE_REGISTRATION\(\s*(.*?)\s*\)/ ? $1 : undef; } @FileCpp;
  my @Declarations = grep { /^(typedef|using) /; } @FileCpp; 
  my @TestFunctions = grep { defined; } map { /CPPUNIT_TEST\(\s*(.*?)\s*\)/ ? $1 : undef; } @FileH;
  my @TestBodyes = grep { my $F = $_; grep { index($F,$_) >= 0; } @TestFunctions; } 
                       ( join "\n", @FileCpp ) =~ /^((?:template\s*<.*?>\s*)?void\s*scigraphics::tests::test\w+(?:<\s*\w+\s*>)?::\w+\(\s*\)\s*^{.*?^})/msg;
  my @HelperBodyes = grep { my $F = $_; ! grep { $F eq $_ } @TestBodyes; } 
                       ( ( join "\n", @FileCpp ) =~ /((?:^(?:(?:template[^{(]*?)?(?:class|struct|static|enum\s+.*?\)))|(?:[a-z][a-zA-Z_<>0-9 :]+\s*scigraphics::.*?\))).*?^{.*?^}(?:;)?)/msg );
  my @Includes = ( join "\n", @FileCpp ) =~ /^#include\s*(.*)\s*$/mg;

  for ( @TestBodyes, @HelperBodyes )
  {
    s/CPPUNIT_ASSERT_EQUAL\(/ASSERT_EQ(/g;
    s/CPPUNIT_ASSERT_DOUBLES_EQUAL\(/ASSERT_NEAR(/g;
    s/CPPUNIT_ASSERT\(/ASSERT_TRUE(/g;

    my $MsgArgs = '\(\s*(.*?)\s*,\s*(.*?)\s*\);';
    s/CPPUNIT_ASSERT_MESSAGE$MsgArgs/ASSERT_TRUE( $2 ) << $1;/go;
    s/CPPUNIT_ASSERT_EQUAL_MESSAGE$MsgArgs/ASSERT_EQ( $2 ) << $1;/go;
    s/CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE$MsgArgs/ASSERT_NEAR( $2 ) << $1;/go;

    s/CPPUNIT_FAIL\(\s*(.*?)\s*\)/FAIL() << $1;/g;
    
    die "Used CPPUNIT at $_" if /CPPUNIT/;
  };

  for ( @TestBodyes )
  {
    s/^(template\s*<.*?>\s+)?void\s*scigraphics::tests::test_(\w+)(?:<.*?>)?::(\w+)\(\s*\)/($1 ? "TYPED_TEST" : "TEST_F" ) . "( test_$2, $3 )"/gme;
    s/{/{\n  typedef TypeParam T;\n/ if /TYPED_TEST/;
  }

  for ( @HelperBodyes )
  {
    s/^(.*?)\s+scigraphics::tests::test_\w+::/static $1 /gm;
  }

  #print "Suits: @TestSuites\n";
  #print "Functions: @TestFunctions\n";
  #print "Bodyes: ", ( join "\n++++++\n", map { $_; } @FunctionsBodyes ), "\n";
  #exit 0;
  #print "Includes: @Includes\n"; 

  my $Line = "// =========================================================";
  my $SubLine = $Line;
  $SubLine =~ s/=/-/g;

  open my $FD, ">", "g$Name.cpp"
    or die "Can not open gtest file for name $Name for writing : $!";

  print $FD "\n$Line\n\n";
  print $FD "#include $_\n" for ( grep { /^</ } ( @Includes ) );
  print $FD "\n#include <gtest/gtest.h>\n";
  print $FD "\n";
  print $FD "#define private public\n";
  print $FD "#define protected public\n";
  print $FD "\n";
  print $FD "#include $_\n" for ( grep { /^"/ } grep { ! m{tests/test_}; } @Includes );
  print $FD "\nnamespace scigraphics {};\nusing namespace scigraphics;\n";
  print $FD "\n$Line\n\n";
  
  for ( @Declarations )
  {
    print $FD "$_\n";
  }
  print $FD "\n$Line\n\n" if @Declarations;

  for my $Suite ( @TestSuites )
  {
    my $TemplateType = ( $Suite =~ /<(.*?)>/ ? $1 : undef );
    $Suite =~ s/<.*>//;
    #print $FD "namespace scigraphics\n{\n\n";
    print $FD ( $TemplateType ? "template <class $TemplateType> " : "" ) . "class $Suite : public testing::Test\n";
    print $FD "{\n";
    print $FD "};\n";
    if ( $TemplateType )
    {
      print $FD "\n";
      my $SBase = ( $Suite =~ /^(.*?)</ ? $1 : $Suite );
      print $FD "typedef testing::Types< ";
      print $FD join ", ", map { m/<(.*?)>/; $1; } grep { index($_,$SBase) >= 0; } @RegisteredSuites;
      print $FD " > ${SBase}Types;\n";
      print $FD "TYPED_TEST_CASE( $SBase, ${SBase}Types );\n";
    }
    #print $FD "\n}\n";
    print $FD "\n$Line\n\n";
  }

  
  for my $i ( 0 .. $#HelperBodyes )
  {
    print $FD "\n$SubLine\n\n" if $i != 0;
    print $FD $HelperBodyes[$i], "\n";
  }

  print $FD "\n$Line\n\n" if @HelperBodyes;

  for my $i ( 0 .. $#TestBodyes )
  {
    print $FD "\n$SubLine\n\n" if $i != 0;
    print $FD $TestBodyes[$i], "\n";
  }
  
  print $FD "\n$Line\n\n";

  close $FD;

}

exit 0;

