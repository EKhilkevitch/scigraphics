
project( graphicsQt )

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

set ( graphics_qt_source_files
        qt4plotsettings.cpp
        qt4plotmanager.cpp
        qt4plot.cpp
    )

set( moc_headers 
        qt4plotsettings.h
        qt4plotmanager.h
        qt4plot.h
   )

qt4_wrap_cpp( moc_sources ${moc_headers} )

add_library( scigraphicsQt ${library_type} ${graphics_qt_source_files} ${moc_sources} )
target_link_libraries( scigraphicsQt ${QT_LIBRARIES} scigraphics )

