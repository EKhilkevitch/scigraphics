
cmake_minimum_required( VERSION 2.6 )
project( graphics ) 

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set( EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/" )
set( LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib/" )

if ( NOT LIBRARY_TYPE )
  if ( WIN32 )
    set( LIBRARY_TYPE STATIC )
  else()
    set( LIBRARY_TYPE SHARED )
  endif()
endif()

if ( NOT IMAGEMAGICK_ENABLED )
  if ( WIN32 )
    set( IMAGEMAGICK_ENABLED 0 )
  else()
    set( IMAGEMAGICK_ENABLED 1 )
  endif()  
endif()

message( "LIBRARY_TYPE = ${LIBRARY_TYPE}" )
message( "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
message( "IMAGEMAGICK_ENABLED = ${IMAGEMAGICK_ENABLED}" )

#Profiling 
#set( CMAKE_BUILD_TYPE Release )
#set( library_type STATIC )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -g")
#set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -g")

if ( UNIX )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC") 
  set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC") 
endif()

add_subdirectory( ./source/scigraphics/ )
add_subdirectory( ./source/scigraphics/qt4/ )
add_subdirectory( ./source/scigraphics/imagemagick/ )
add_subdirectory( ./source/example/ )
add_subdirectory( ./source/tests/ )

