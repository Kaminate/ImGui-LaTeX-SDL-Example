add_executable( GameLaTeX )
target_sources( GameLaTeX PRIVATE
  main.cpp
  microtex_imgui_graphics.cpp
  microtex_imgui_graphics.h
  demo.cpp
  demo.h
)

target_link_libraries( GameLaTeX
  microtex
  SDL3::SDL3
  imgui
)


