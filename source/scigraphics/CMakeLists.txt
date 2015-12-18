
cmake_minimum_required( VERSION 2.6 )
project( graphicsLib ) 

set( graphics_source_files 
        color.cpp
        numlimits.cpp
        numberstyle.cpp
        linestyle.cpp
        pointstyle.cpp
        brushstyle.cpp
        textstyle.cpp
        plotelement.cpp
        plotelementcollection.cpp
        graph.cpp
        graphsequence.cpp
        graphmap.cpp
        datasequence.cpp
        datamap.cpp
        plot.cpp
        selectionstyle.cpp
        errorbarstyle.cpp
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
        mousecallback.cpp
        zoomrect.cpp
        floatrect.cpp
        legend.cpp
        cursorpoint.cpp
        selection.cpp
        selectioncollection.cpp
        settings.cpp
   )

add_library( scigraphics ${LIBRARY_TYPE} ${graphics_source_files} )

