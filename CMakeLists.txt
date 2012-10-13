
cmake_minimum_required( VERSION 2.6 )
project( graphics ) 

set( CMAKE_BUILD_TYPE Release )

set( EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/" )
set( LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib/" )

include_directories( "./source/" )

if ( WIN32 )
  set( library_type STATIC )
else()
  set( library_type SHARED )
endif()

add_subdirectory( ./source/scigraphics/ )
add_subdirectory( ./source/scigraphics/qt4/ )
add_subdirectory( ./source/scigraphics/imagemagick/ )
add_subdirectory( ./source/example/ )
add_subdirectory( ./source/tests/ )

if ( WIN32 )
  add_custom_command( COMMAND "sed -i 's/machine:[a-z0-9]*/machine:x86/g" TARGET Makefile )
endif()

