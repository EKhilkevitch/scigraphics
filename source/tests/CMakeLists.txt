
cmake_minimum_required( VERSION 2.6 )
<<<<<<< HEAD

if ( ENABLE_TESTS )

  project( selfTest ) 

  set ( tests_source_files
          gtest_geometry.cpp
          gtest_interval.cpp
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
          gtest_settings.cpp
          mockpainter.cpp
      )

  include_directories( "${PROJECT_SOURCE_DIR}/../scigraphics/" )

  find_library( gtest  NAMES gtest )
  find_library( gtest_main  NAMES gtest_main )

  if ( NOT ${gtest} MATCHES gtest-NOTFOUND )
    message( "Tests enabled" )
    add_executable( selfTest-scigraphics ${tests_source_files} )
    target_link_libraries( selfTest-scigraphics ${gtest} )
    target_link_libraries( selfTest-scigraphics ${gtest_main} )
    target_link_libraries( selfTest-scigraphics scigraphics )
    target_link_libraries( selfTest-scigraphics pthread )
    target_include_directories( selfTest-scigraphics PRIVATE "${PROJECT_SOURCE_DIR}/../" )
    target_include_directories( selfTest-scigraphics PRIVATE "${PROJECT_SOURCE_DIR}/../scigraphics/" )
  else()
    message( "gtest not found, test not build" )
  endif()  
else()
    message( "Tests disabed (use -DENABLE_TESTS=1)" )
endif()

