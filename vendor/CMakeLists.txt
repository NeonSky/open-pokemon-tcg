# glad
file(GLOB_RECURSE vendor_files "glad/*.c" "glad/*.h" "glad/*.cpp" "glad/*.hpp")
add_library(glad "${vendor_files}")
target_include_directories(glad PUBLIC "glad/include")
