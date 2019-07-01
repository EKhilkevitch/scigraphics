
cmake_minimum_required( VERSION 2.6 )

if ( ENABLE_EXAMPLES )
  
  message( "Examples enabled" )

  find_package(ImageMagick COMPONENTS Magick++)
  find_package(Qt4 REQUIRED)
  include( ${QT_USE_FILE} )

  if(WIN32)
    set( gui_type WIN32 )
  endif(WIN32)

  if ( WIN32 )
    set( QtLibs ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY} )
  else()
    set( QtLibs ${QT_LIBRARIES} )
  endif()

  message( "Qt libraries are ${QT_LIBRARIES}" )

  project( examapleSimple )
  add_executable( exampleSimple ${gui_type} exampleSimple.cpp )
  target_link_libraries( exampleSimple ${QtLibs} scigraphicsQt )
  target_include_directories( exampleSimple PRIVATE "${PROJECT_SOURCE_DIR}/../" )

  project( examapleGeneral )
  add_executable( exampleGeneral ${gui_type} exampleGeneral.cpp )
  target_link_libraries( exampleGeneral ${QtLibs} scigraphicsQt )
  target_include_directories( exampleGeneral PRIVATE "${PROJECT_SOURCE_DIR}/../" )

  project( exampleManager )
  add_executable( exampleManager ${gui_type} exampleManager.cpp )
  target_link_libraries( exampleManager ${QtLibs} scigraphicsQt )
  target_include_directories( exampleManager PRIVATE "${PROJECT_SOURCE_DIR}/../" )

  project( exampleAnimation )
  qt4_wrap_cpp( exampleAnimation_moc_sources exampleAnimation.h )
  add_executable( exampleAnimation exampleAnimation.cpp ${exampleAnimation_moc_sources} )
  target_link_libraries( exampleAnimation ${QtLibs} scigraphicsQt )
  target_include_directories( exampleAnimation PRIVATE "${PROJECT_SOURCE_DIR}/../" )

  project( exampleMap )
  add_executable( exampleMap ${gui_type} exampleMap.cpp )
  target_link_libraries( exampleMap ${QtLibs} scigraphicsQt )
  target_include_directories( exampleMap PRIVATE "${PROJECT_SOURCE_DIR}/../" )

  project( exampleQtImage )
  add_executable( exampleQtImage ${gui_type} exampleQtImage.cpp )
  target_link_libraries( exampleQtImage ${QtLibs} scigraphicsQt )
  target_include_directories( exampleQtImage PRIVATE "${PROJECT_SOURCE_DIR}/../" )

  project( exampleImagemagick )
  if ( ImageMagick_FOUND AND IMAGEMAGICK_ENABLED )
    add_executable( exampleImagemagick exampleImagemagick.cpp )
    target_link_libraries( exampleImagemagick scigraphicsIm )
    target_include_directories( exampleImagemagick PRIVATE "${PROJECT_SOURCE_DIR}/../" )
  endif()

else()
  message( "Examples disabed (use -DENABLE_EXAMPLES=1)" )
endif()  



