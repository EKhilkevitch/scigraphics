
project( graphicsQt )

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

set ( graphics_qt_source_files
        plotview.cpp
        drawer.cpp
        mousecallback.cpp
        plot.cpp
        plotimage.cpp
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

set( translations 
        settingsbox_ru.ts 
   )

set( translations_qrc "${CMAKE_CURRENT_BINARY_DIR}/translations.qrc" )
file( WRITE ${translations_qrc} "<RCC>\n\t<qresource prefix=\"/scigraphics\">" )
foreach( qm_file ${translations} )
  get_filename_component( qm_file_name ${qm_file} NAME )
  get_filename_component( qm_file_base ${qm_file_name} NAME_WE )
  file( APPEND ${translations_qrc} "\n\t\t<file alias=\"${qm_file_base}.qm\">${CMAKE_CURRENT_BINARY_DIR}/${qm_file_base}.qm</file>" )
endforeach()
file( APPEND ${translations_qrc} "\n\t</qresource>\n</RCC>" )
list( APPEND resources ${translations_qrc} )

qt4_wrap_cpp( moc_sources ${moc_headers} )
qt4_add_translation( qm_sources ${translations} )
qt4_add_resources( qrc_sources ${resources} ) 

add_library( scigraphicsQt ${LIBRARY_TYPE} ${graphics_qt_source_files} ${moc_sources} ${qm_sources} ${qrc_sources} )
target_link_libraries( scigraphicsQt ${QT_LIBRARIES} scigraphics )
target_include_directories( scigraphicsQt PRIVATE "${PROJECT_SOURCE_DIR}/../../" )

