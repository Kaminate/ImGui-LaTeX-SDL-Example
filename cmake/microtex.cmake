include( FetchContent )

add_compile_options( /wd4099  
                     /wd4100 )
add_compile_options( /wd4101 )
add_compile_options( /wd4244 )
add_compile_options( /wd4251 )
add_compile_options( /wd4267 )
add_compile_options( /wd4305 )
add_compile_options( /wd4456 )
add_compile_options( /wd4457 )
add_compile_options( /wd4996 )

FetchContent_Declare( microtex GIT_REPOSITORY https://github.com/NanoMichael/MicroTeX.git
                               #GIT_TAG        0e3707f6dafebb121d98b53c64364d16fefe481d ) # master branch
                               GIT_TAG        086f4eb740270b28bd0c61a0a359aea9300d61ae ) # openmath branch

FetchContent_MakeAvailable( microtex )

