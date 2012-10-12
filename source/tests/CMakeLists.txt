
cmake_minimum_required( VERSION 2.6 )
project( selfTest ) 

set ( tests_source_files
        testMain.cpp
        test_geometry.cpp
        test_graphcollection.cpp
        test_graph.cpp
        test_graphdata.cpp
        test_legend.cpp
        test_marker.cpp
        test_numberlimits.cpp
        test_painter.cpp
        test_plotlimits.cpp
        test_scale.cpp
        test_color.cpp
    )

set( CMAKE_CXX_COMPILER "icpc" )
set( CMAKE_C_COMPILER "icc" )

include_directories( "${PROJECT_SOURCE_DIR}/../graphics/" )

if ( UNIX )
  add_executable( selfTest ${tests_source_files} )
  target_link_libraries( selfTest cppunit graphics )
  set_target_properties( selfTest PROPERTIES COMPILE_FLAGS "-std=c++0x")
endif()

