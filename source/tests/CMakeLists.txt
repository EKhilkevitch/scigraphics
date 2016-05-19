
cmake_minimum_required( VERSION 2.6 )
project( selfTest ) 

set ( tests_source_files
        gtest_geometry.cpp
        gtest_graphcollection.cpp
        gtest_graphviewsequence.cpp
        gtest_graph.cpp
        gtest_graphdata.cpp
        gtest_legend.cpp
        gtest_marker.cpp
        gtest_numberlimits.cpp
        gtest_painter.cpp
        gtest_plotlimits.cpp
        gtest_scale.cpp
        gtest_color.cpp
        mockpainter.cpp
    )

include_directories( "${PROJECT_SOURCE_DIR}/../scigraphics/" )

find_library( gtest  NAMES gtest )
find_library( gtest_main  NAMES gtest_main )

if ( NOT ${gtest} MATCHES gtest-NOTFOUND AND ENABLE_TESTS )

  #find_program( ICPC_PROGRAM icpc )
  #if ( ICPC_PROGRAM )
  #  set( CMAKE_CXX_COMPILER icpc ) 
  #  set( CMAKE_C_COMPILER icc ) 
  #endif()

  message( "Tests enabled" )
  add_executable( selfTest ${tests_source_files} )
  target_link_libraries( selfTest ${gtest} )
  target_link_libraries( selfTest ${gtest_main} )
  target_link_libraries( selfTest scigraphics )
  target_link_libraries( selfTest pthread )
  set_target_properties( selfTest PROPERTIES COMPILE_FLAGS "-std=c++0x")
else()
  message( "Tests disabed (use -DENABLE_TESTS=1)" )
endif()  

