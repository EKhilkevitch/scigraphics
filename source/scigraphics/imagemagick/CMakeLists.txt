
cmake_minimum_required( VERSION 2.6 )
project( graphicsIm ) 

find_package(ImageMagick COMPONENTS Magick++)

set( graphics_im_source_files 
      plot.cpp
      drawer.cpp
   )

if ( ImageMagick_FOUND AND IMAGEMAGICK_ENABLED )
  add_library( scigraphicsIm ${LIBRARY_TYPE} ${graphics_im_source_files} )
  target_link_libraries( scigraphicsIm ${ImageMagick_LIBRARIES} scigraphics )
  target_include_directories( scigraphicsIm PRIVATE ${ImageMagick_INCLUDE_DIRS} )
  target_include_directories( scigraphicsIm PRIVATE "${PROJECT_SOURCE_DIR}/../../" )
endif()


