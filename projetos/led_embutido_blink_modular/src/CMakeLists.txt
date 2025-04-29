# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(led_embutido_blink_modular C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(led_embutido_blink_modular
    app/main.c
    drivers/led_embutido.c
    hal/hal_led.c
)

pico_set_program_name(led_embutido_blink_modular "led_embutido_blink_modular")
pico_set_program_version(led_embutido_blink_modular "0.1")

target_sources(led_embutido_blink_modular PRIVATE app/main.c)



# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(led_embutido_blink_modular 0)
pico_enable_stdio_usb(led_embutido_blink_modular 0)

# Add the standard library to the build
# Add any user requested libraries
target_link_libraries(led_embutido_blink_modular 
        pico_stdlib
        )

target_include_directories(led_embutido_blink_modular PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/drivers
    ${CMAKE_CURRENT_LIST_DIR}/hal
)

# Add any user requested libraries
target_link_libraries(led_embutido_blink_modular 
        pico_cyw43_arch_lwip_threadsafe_background
        )

pico_add_extra_outputs(led_embutido_blink_modular)
