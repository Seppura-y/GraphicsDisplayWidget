cmake_minimum_required(VERSION 3.22)

project(GraphicsDisplayWidget LANGUAGES CXX)



set(RUNTIME_DIR ${CMAKE_CURRENT_LIST_DIR}/bin)
set(LIBRARY_DIR ${CMAKE_CURRENT_LIST_DIR}/lib)

#===================================== CONFIGURATION TYPES OF THE PROJECT ==================================#
set(CONFIG_TYPE_DEBUG TRUE CACHE BOOL "Debug" )
set(CONFIG_TYPE_RELEASE FALSE CACHE BOOL "Release" )
set(CONFIG_TYPE_RELWITHDEBINFO FALSE CACHE BOOL "RelWithDebInfo" )
set(CONFIG_TYPE_MINSIZEREL FALSE CACHE BOOL "MinSizeRel")

if(${CONFIG_TYPE_DEBUG})
    list(APPEND CONFIG_TYPES "Debug;")
endif()

if(${CONFIG_TYPE_RELEASE})
    list(APPEND CONFIG_TYPES "Release;")
endif()

if(${CONFIG_TYPE_RELWITHDEBINFO})
    list(APPEND CONFIG_TYPES "RelWithDebInfo;")
endif()

if(${CONFIG_TYPE_MINSIZEREL})
    list(APPEND CONFIG_TYPES "MinSizeRel")
endif()

set(CMAKE_CONFIGURATION_TYPES ${CONFIG_TYPES})


#============================================== INCLUSION OF Qt ==============================================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{Qt515_64})
# set(AUTOGEN_BUILD_DIR ${CMAKE_CURRENT_LIST_DIR}/gens)

find_package(Qt5 COMPONENTS 
    Core
    Widgets 
    OpenGL
    OpenGLExtensions
    REQUIRED
)

#========================================= INCLUSION OF Project Files =========================================#
set(FORMS_DIR ${CMAKE_CURRENT_LIST_DIR}/forms)

LIST(APPEND SOURCES_DIR 
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/config
    ${CMAKE_CURRENT_LIST_DIR}/src/camera        
    ${CMAKE_CURRENT_LIST_DIR}/src/canvas        
    ${CMAKE_CURRENT_LIST_DIR}/src/capture       
    ${CMAKE_CURRENT_LIST_DIR}/src/capture_ui    
    ${CMAKE_CURRENT_LIST_DIR}/src/move_item_test
    ${CMAKE_CURRENT_LIST_DIR}/src/test          
    ${CMAKE_CURRENT_LIST_DIR}/src/tools         
    ${CMAKE_CURRENT_LIST_DIR}/src/widget        
)

include_directories(${FORMS_DIR})
include_directories(${SOURCE_DIR})

FILE(GLOB ${PROJECT_NAME}_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
)

FILE(GLOB ${PROJECT_NAME}_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/*.h
)

FILE(GLOB ${PROJECT_NAME}_CONFIG_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/config/*.cpp
)

FILE(GLOB ${PROJECT_NAME}_CONFIG_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/config/*.h
)

FILE(GLOB ${PROJECT_NAME}_CAMERA_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/camera/*.cpp
)
FILE(GLOB ${PROJECT_NAME}_CAMERA_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/camera/*.h
)


FILE(GLOB ${PROJECT_NAME}_CANVAS_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/canvas/*.cpp
)
FILE(GLOB ${PROJECT_NAME}_CANVAS_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/canvas/*.h
)


FILE(GLOB ${PROJECT_NAME}_CAPTURE_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/capture/*.cpp
)
FILE(GLOB ${PROJECT_NAME}_CAPTURE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/capture/*.h
)


FILE(GLOB ${PROJECT_NAME}_CAPTURE_UI_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/capture_ui/*.cpp
)
FILE(GLOB ${PROJECT_NAME}_CAPTURE_UI_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/capture_ui/*.h
)


FILE(GLOB ${PROJECT_NAME}_MOVE_ITEM_TEST_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/move_item_test/*.cpp
)
FILE(GLOB ${PROJECT_NAME}_MOVE_ITEM_TEST_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/move_item_test/*.h
)


FILE(GLOB ${PROJECT_NAME}_TEST_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/test/*.cpp
)
FILE(GLOB ${PROJECT_NAME}_TEST_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/test/*.h
)



FILE(GLOB ${PROJECT_NAME}_TOOLS_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/tools/*.cpp
)
FILE(GLOB ${PROJECT_NAME}_TOOLS_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/tools/*.h
)



FILE(GLOB ${PROJECT_NAME}_WIDGET_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/widget/*.cpp
)
FILE(GLOB ${PROJECT_NAME}_WIDGET_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/src/widget/*.h
)



FILE(GLOB ${PROJECT_NAME}_CSS_FILES
    ${CMAKE_CURRENT_LIST_DIR}/forms/res/css/*.css
)

FILE(GLOB ${PROJECT_NAME}_UI 
    ${CMAKE_CURRENT_LIST_DIR}/forms/*.ui
)

FILE(GLOB ${PROJECT_NAME}_QRC_FILES
    ${CMAKE_CURRENT_LIST_DIR}/forms/*.qrc
)

qt5_wrap_ui(${PROJECT_NAME}_UI_HEADERS ${${PROJECT_NAME}_UI})
qt5_add_resources(${PROJECT_NAME}_QRC ${${PROJECT_NAME}_QRC_FILES})

source_group("forms"                        FILES ${${PROJECT_NAME}_UI})
source_group("qrc"                          FILES ${${PROJECT_NAME}_QRC_FILES} ${${PROJECT_NAME}_QRC})

source_group("qrc//css"                     FILES ${${PROJECT_NAME}_CSS_FILES} )

source_group("src//ui_gens"                 FILES ${${PROJECT_NAME}_UI_HEADERS})

source_group("src//"                        FILES ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
source_group("src//config"                  FILES ${${PROJECT_NAME}_CONFIG_HEADERS} ${${PROJECT_NAME}_CONFIG_SOURCES})
source_group("src//camera"                  FILES ${${PROJECT_NAME}_CAMERA_SOURCES} ${${PROJECT_NAME}_CAMERA_SOURCES})
source_group("src//canvas"                  FILES ${${PROJECT_NAME}_CANVAS_HEADERS} ${${PROJECT_NAME}_CANVAS_SOURCES})
source_group("src//capture"                 FILES ${${PROJECT_NAME}_CAPTURE_HEADERS} ${${PROJECT_NAME}_CAPTURE_SOURCES})
source_group("src//capture_ui"              FILES ${${PROJECT_NAME}_CAPTURE_UI_HEADERS} ${${PROJECT_NAME}_CAPTURE_UI_SOURCES})
source_group("src//move_item_test"          FILES ${${PROJECT_NAME}_MOVE_ITEM_TEST_HEADERS} ${${PROJECT_NAME}_MOVE_ITEM_TEST_SOURCES})
source_group("src//test"                    FILES ${${PROJECT_NAME}_TEST_HEADERS} ${${PROJECT_NAME}_TEST_SOURCES})
source_group("src//tools"                   FILES ${${PROJECT_NAME}_TOOLS_HEADERS} ${${PROJECT_NAME}_TOOLS_SOURCES})
source_group("src//widget"                  FILES ${${PROJECT_NAME}_WIDGET_HEADERS} ${${PROJECT_NAME}_WIDGET_SOURCES})



#========================================= SETUP EXECTUABLE =========================================#
# Enable debug logging on RELWITHDEBINFO configuration
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Set the startup project for visual studio
set_property(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Add the executable
if (WIN32) 
    add_executable(${PROJECT_NAME} WIN32 
        ${${PROJECT_NAME}_HEADERS}
        ${${PROJECT_NAME}_SOURCES}

        ${${PROJECT_NAME}_CONFIG_HEADERS}
        ${${PROJECT_NAME}_CONFIG_SOURCES}

        ${${PROJECT_NAME}_CONFIG_HEADERS} ${${PROJECT_NAME}_CONFIG_SOURCES}
        ${${PROJECT_NAME}_CAMERA_SOURCES} ${${PROJECT_NAME}_CAMERA_SOURCES}
        ${${PROJECT_NAME}_CANVAS_HEADERS} ${${PROJECT_NAME}_CANVAS_SOURCES}
        ${${PROJECT_NAME}_CAPTURE_HEADERS} ${${PROJECT_NAME}_CAPTURE_SOURCES}
        ${${PROJECT_NAME}_CAPTURE_UI_HEADERS} ${${PROJECT_NAME}_CAPTURE_UI_SOURCES}
        ${${PROJECT_NAME}_MOVE_ITEM_TEST_HEADERS} ${${PROJECT_NAME}_MOVE_ITEM_TEST_SOURCES}
        ${${PROJECT_NAME}_TEST_HEADERS} ${${PROJECT_NAME}_TEST_SOURCES}
        ${${PROJECT_NAME}_TOOLS_HEADERS} ${${PROJECT_NAME}_TOOLS_SOURCES}
        ${${PROJECT_NAME}_WIDGET_HEADERS} ${${PROJECT_NAME}_WIDGET_SOURCES}

        ${${PROJECT_NAME}_CSS_FILES}

        ${${PROJECT_NAME}_UI_HEADERS}
        ${${PROJECT_NAME}_UI}
        ${${PROJECT_NAME}_QRC}
    )
elseif(UNIX)
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

# set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "frontend")

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup" )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../bin
    )
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    DN_PLATFORM_WINDOWS
    DN_BUILD_STATIC
)


target_compile_features(${PROJECT_NAME} PRIVATE
    cxx_std_17
)

# Add the target includes for the project 
target_include_directories(${PROJECT_NAME} PRIVATE ${FORMS_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCES_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/include/
)

#========================================= LINKING LIBRARIES =========================================#
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lib
)

target_link_libraries(${PROJECT_NAME} 
    Qt5::Core
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::OpenGLExtensions
    user32.lib
    dwmapi.lib

    avcodec.lib
    avutil.lib
    swscale.lib


)

foreach(type IN LISTS CONFIG_TYPES)
    if(type)
        string(TOUPPER _${type} config)
        message("config = ${config}")
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY${config} ${RUNTIME_DIR}   #dll exe 
        LIBRARY_OUTPUT_DIRECTORY${config} ${LIBRARY_DIR}   #so dylib
        ARCHIVE_OUTPUT_DIRECTORY${config} ${LIBRARY_DIR}   #lib a
        PDB_OUTPUT_DIRECTORY${config}     ${RUNTIME_DIR}   #pdb
    )
endforeach()

set(INSTALL_PATH 
${CMAKE_INSTALL_PREFIX}/${CMAKE_VS_PLATFORM_NAME})


set(windeployqt "$ENV{Qt515_64}/bin/windeployqt")

install( CODE 
        "execute_process(COMMAND 
        ${windeployqt} $<TARGET_FILE:${PROJECT_NAME}>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )"
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    PRIVATE_HEADER DESTINATION inc
)

install(EXPORT ${PROJECT_NAME} FILE "${PROJECT_NAME}Config.cmake" DESTINATION config)

install(DIRECTORY 
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DESTINATION
    ${INSTALL_PATH}
)