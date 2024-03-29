
project( graphicsQt )

if( POLICY CMP0012 )
  cmake_policy(SET CMP0012 NEW)
endif()

find_package( Qt5 COMPONENTS Widgets LinguistTools QUIET )
if ( NOT ${Qt5_FOUND} )
  find_package( Qt4 COMPONENTS QtGui QtCore QUIET )
endif()

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

if ( ${Qt4_FOUND} )
  message( "Qt4 found" )
  include( ${QT_USE_FILE} )

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
elseif ( ${Qt5_FOUND} )
  
  message( "Qt5 found" )

  set( translations_qrc "${CMAKE_CURRENT_BINARY_DIR}/translations.qrc" )
  file( WRITE ${translations_qrc} "<RCC>\n\t<qresource prefix=\"/scigraphics\">" )
  foreach( qm_file ${translations} )
    get_filename_component( qm_file_name ${qm_file} NAME )
    get_filename_component( qm_file_base ${qm_file_name} NAME_WE )
    file( APPEND ${translations_qrc} "\n\t\t<file alias=\"${qm_file_base}.qm\">${CMAKE_CURRENT_BINARY_DIR}/${qm_file_base}.qm</file>" )
  endforeach()
  file( APPEND ${translations_qrc} "\n\t</qresource>\n</RCC>" )
  list( APPEND resources ${translations_qrc} )

  qt5_wrap_cpp( moc_sources ${moc_headers} )
  qt5_add_translation( qm_sources ${translations} )
  qt5_add_resources( qrc_sources ${resources} ) 

  add_library( scigraphicsQt ${LIBRARY_TYPE} ${graphics_qt_source_files} ${moc_sources} ${qm_sources} ${qrc_sources} )
  target_link_libraries( scigraphicsQt Qt5::Widgets scigraphics )
  target_include_directories( scigraphicsQt PRIVATE "${PROJECT_SOURCE_DIR}/../../" )
  target_include_directories( scigraphicsQt PRIVATE ${Qt5Widgets_INCLUDE_DIRS} )

else()
  message( "No Qt found" )
endif()

