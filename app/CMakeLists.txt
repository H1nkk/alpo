set(target ${MP2_APP})

# Find includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
#set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
#set(CMAKE_AUTOUIC ON)

#find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

#file(GLOB ui "${CMAKE_CURRENT_SOURcE_DIR}/ui/*.ui")
file(GLOB hdrs "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*" "${MP2_INCLUDE}/*.h*")
file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${target} ${hdrs} ${srcs})
target_link_libraries(${target} PUBLIC ${MP2_LIBRARY}) #Qt5::Core Qt5::Gui Qt5::Widgets)

#macro(qt5_copy_dll APP DLL)
#    # find the release *.dll file
#    get_target_property(Qt5_${DLL}Location Qt5::${DLL} LOCATION)
#    # find the debug *d.dll file
#    get_target_property(Qt5_${DLL}LocationDebug Qt5::${DLL} IMPORTED_LOCATION_DEBUG)
#
#    add_custom_command(TARGET ${APP} POST_BUILD
#       COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_${DLL}LocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_${DLL}Location}> $<TARGET_FILE_DIR:${APP}>)
#endmacro()

#qt5_copy_dll(${PROJECT_NAME} Core)
#qt5_copy_dll(${PROJECT_NAME} Gui)
#qt5_copy_dll(${PROJECT_NAME} Widgets)
