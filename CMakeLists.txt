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

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/ktrayshortcut DESTINATION bin)
# for packing purposes:

SET(MAJOR_VERSION "0")
SET(MINOR_VERSION "1")
SET(PATCH_VERSION "2")

IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
INCLUDE(InstallRequiredSystemLibraries)
 
SET(CPACK_SET_DESTDIR "on")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
SET(CPACK_GENERATOR "DEB")

SET(CPACK_PACKAGE_DESCRIPTION "Send any window to the system tray and assign them global shortcuts.")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Send any window to the system tray and assign them global shortcuts.")
SET(CPACK_PACKAGE_VENDOR "rosenfeld")
SET(CPACK_PACKAGE_CONTACT "rr.rosas@gmail.com")
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
 
SET(CPACK_DEBIAN_PACKAGE_DEPENDS " libxmu6 , libkdeui5 ")
 
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "kde")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
 
SET(CPACK_COMPONENTS_ALL Libraries ApplicationData)
INCLUDE(CPack)
 
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
