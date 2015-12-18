
project( graphicsQt )

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

set ( graphics_qt_source_files
        plotview.cpp
        drawer.cpp
        mousecallback.cpp
        plot.cpp
        labeledline.cpp
        settingsbox.cpp
        settingscomposer.cpp
        settings.cpp
        manager.cpp
        debugprint.cpp
    )

set( moc_headers 
        plotview.h
        plot.h
        labeledline.h
        settingsbox.h
        settings.h
        manager.h
   )

qt4_wrap_cpp( moc_sources ${moc_headers} )

add_library( scigraphicsQt ${LIBRARY_TYPE} ${graphics_qt_source_files} ${moc_sources} )
target_link_libraries( scigraphicsQt ${QT_LIBRARIES} scigraphics )

