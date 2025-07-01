
include( FetchContent )

add_compile_options( /wd5286 )
FetchContent_Declare( SDL3 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                           GIT_TAG        release-3.2.16 )
FetchContent_MakeAvailable( SDL3 )

