
cmake_minimum_required( VERSION 2.6 )

find_package(ImageMagick COMPONENTS Magick++)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

if(WIN32)
  add_definitions( -DQT_DLL )
  link_libraries( ${QT_QTMAIN_LIBRARY} )
  set( gui_type WIN32 )
endif(WIN32)

project( examapleSimple )
add_executable( exampleSimple ${gui_type} exampleSimple.cpp )
target_link_libraries( exampleSimple ${QT_LIBRARIES} scigraphicsQt )

project( examapleGeneral )
add_executable( exampleGeneral ${gui_type} exampleGeneral.cpp )
target_link_libraries( exampleGeneral ${QT_LIBRARIES} scigraphicsQt )

project( exampleManager )
add_executable( exampleManager ${gui_type} exampleManager.cpp )
target_link_libraries( exampleManager ${QT_LIBRARIES} scigraphicsQt )

project( exampleAnimation )
qt4_wrap_cpp( exampleAnimation_moc_sources exampleAnimation.h )
add_executable( exampleAnimation exampleAnimation.cpp ${exampleAnimation_moc_sources} )
target_link_libraries( exampleAnimation ${QT_LIBRARIES} scigraphicsQt )

project( exampleMap )
add_executable( exampleMap ${gui_type} exampleMap.cpp )
target_link_libraries( exampleMap ${QT_LIBRARIES} scigraphicsQt )

project( exampleImagemagick )
if ( ImageMagick_FOUND )
  add_executable( exampleImagemagick exampleImagemagick.cpp )
  target_link_libraries( exampleImagemagick scigraphicsIm )
endif()

