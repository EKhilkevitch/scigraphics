
cmake_minimum_required( VERSION 2.6 )
project( graphicsIm ) 

find_package(ImageMagick COMPONENTS Magick++)

set( graphics_im_source_files 
      implot.cpp
   )

include_directories(${ImageMagick_INCLUDE_DIRS})

if ( ImageMagick_FOUND AND IMAGEMAGICK_ENABLED )
  add_library( scigraphicsIm ${LIBRARY_TYPE} ${graphics_im_source_files} )
  target_link_libraries( scigraphicsIm ${ImageMagick_LIBRARIES} scigraphics )
endif()


