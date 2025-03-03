set(target ${MP2_TESTS})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

add_executable(${target} ${srcs} ${hdrs})
add_test(NAME ${target} COMMAND ${target})
target_link_libraries(${target} PUBLIC ${MP2_LIBRARY} gtest)
