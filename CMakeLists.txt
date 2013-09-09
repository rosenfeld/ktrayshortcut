project(ktrayshortcut)
cmake_minimum_required(VERSION 2.8)

find_package(KDE4 REQUIRED)
find_package(X11 REQUIRED)

include (KDE4Defaults)
include_directories(${KDE4_INCLUDES} ${X11_INCLUDE_DIR})

aux_source_directory(. SRC_LIST)

SET(ktrayshortcut_FORMS mainwindow.ui)
SET(ktrayshortcut_HEADERS mainwindow.h registeredapplication.h)
QT4_WRAP_CPP(ktrayshortcut_HEADERS_MOC ${ktrayshortcut_HEADERS})
QT4_WRAP_UI(ktrayshortcut_FORMS_HEADERS ${ktrayshortcut_FORMS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

add_executable(${PROJECT_NAME} ${SRC_LIST} ${ktrayshortcut_HEADERS_MOC}
    ${ktrayshortcut_FORMS_HEADERS})
target_link_libraries(ktrayshortcut ${KDE4_KDEUI_LIBS} ${X11_LIBRARIES} -lXmu)
