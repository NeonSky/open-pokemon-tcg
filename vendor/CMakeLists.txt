# eventpp
file(GLOB_RECURSE vendor_files "eventpp/*.c" "eventpp/*.h" "eventpp/*.cpp" "eventpp/*.hpp")
add_library(eventpp "${vendor_files}")
target_include_directories(eventpp PUBLIC "eventpp")
set_target_properties(eventpp PROPERTIES LINKER_LANGUAGE CXX)

# glad
file(GLOB_RECURSE vendor_files "glad/*.c" "glad/*.h" "glad/*.cpp" "glad/*.hpp")
add_library(glad "${vendor_files}")
target_include_directories(glad PUBLIC "glad/include")

# imgui_glfw
file(GLOB_RECURSE vendor_files "imgui_glfw/*.c" "imgui_glfw/*.h" "imgui_glfw/*.cpp" "imgui_glfw/*.hpp")
add_library(imgui_glfw "${vendor_files}")

target_compile_definitions(imgui_glfw PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(imgui_glfw PUBLIC "imgui_glfw/include")
target_link_libraries(imgui_glfw PRIVATE imgui::imgui)
target_link_libraries(imgui_glfw PRIVATE glad)
