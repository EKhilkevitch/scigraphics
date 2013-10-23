
cmake_minimum_required( VERSION 2.6 )
project( graphicsLib ) 

set( graphics_source_files 
        graphsequence.cpp
        graphmap.cpp
        numlimits.cpp
        datasequence.cpp
        datamap.cpp
        plot.cpp
        color.cpp
        numberstyle.cpp
        drawer.cpp
        painter.cpp
        axis.cpp
        scale.cpp
        marker.cpp
        plotlimits.cpp
        graphviewsequence.cpp
        graphviewmap.cpp
        graphviewcollection.cpp
        axisticks.cpp
        axislabels.cpp
        axistitle.cpp
        axisset.cpp
        grid.cpp
        mouse.cpp
        zoomrect.cpp
        floatrect.cpp
        legend.cpp
        cursorpoint.cpp
        selection.cpp
        selectioncollection.cpp
        plotelementcollection.cpp
        settings.cpp
   )

add_library( scigraphics ${LIBRARY_TYPE} ${graphics_source_files} )

