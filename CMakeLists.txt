# --------------------------------------------------------------------------------------------------

cmake_minimum_required( VERSION 4.0.0 )

project( GameLatex CXX )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

# --------------------------------------------------------------------------------------------------

if( MSVC )
    set( CMAKE_VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/assets )
    add_compile_options( /ZI  # 
                         /W4  # warning level 4
                         /WX  # treat linker warnings as errors
                         /MP  # multi processor compilation
                         )
endif()


set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )
include( MicroTeX )
include( sdl3 )
include( imgui )

set( CMAKE_CXX_STANDARD 23 )

add_subdirectory( src )
add_subdirectory( assets )



