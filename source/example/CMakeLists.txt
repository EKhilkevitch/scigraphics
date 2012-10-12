
cmake_minimum_required( VERSION 2.6 )

find_package(ImageMagick COMPONENTS Magick++)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

project( examapleSimple )
add_executable( exampleSimple exampleSimple.cpp )
target_link_libraries( exampleSimple ${QT_LIBRARIES} scigraphicsQt )

project( examapleGeneral )
add_executable( exampleGeneral exampleGeneral.cpp )
target_link_libraries( exampleGeneral ${QT_LIBRARIES} scigraphicsQt )

project( exampleManager )
add_executable( exampleManager exampleManager.cpp )
target_link_libraries( exampleManager ${QT_LIBRARIES} scigraphicsQt )
if ( UNIX )
  set_target_properties( exampleManager PROPERTIES COMPILE_FLAGS "-std=c++0x")
endif()

project( exampleAnimation )
qt4_wrap_cpp( exampleAnimation_moc_sources exampleAnimation.h )
add_executable( exampleAnimation exampleAnimation.cpp ${exampleAnimation_moc_sources} )
target_link_libraries( exampleAnimation ${QT_LIBRARIES} scigraphicsQt )
if ( UNIX )
  set_target_properties( exampleAnimation PROPERTIES COMPILE_FLAGS "-std=c++0x")
endif()

project( exampleMap )
add_executable( exampleMap exampleMap.cpp )
target_link_libraries( exampleMap ${QT_LIBRARIES} scigraphicsQt )
if ( UNIX )
  set_target_properties( exampleMap PROPERTIES COMPILE_FLAGS "-std=c++0x")
endif()

project( exampleImagemagick )
if ( ImageMagick_FOUND )
  add_executable( exampleImagemagick exampleImagemagick.cpp )
  target_link_libraries( exampleImagemagick scigraphicsIm )
endif()

