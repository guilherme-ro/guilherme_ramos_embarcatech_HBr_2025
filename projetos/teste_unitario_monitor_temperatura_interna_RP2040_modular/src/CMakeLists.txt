cmake_minimum_required(VERSION 3.13)

# Defina o caminho do Pico SDK
set(PICO_SDK_PATH "/home/guilherme/pico/pico-sdk" CACHE PATH "Path to the Pico SDK")

# Inclua o arquivo de importação do SDK
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Nome do projeto
project(test_adc_to_celsius C CXX ASM)

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
include(../pico-sdk/pico_sdk_init.cmake)

pico_sdk_init()

set(UNITY_PATH /home/guilherme/unity)

# Adicione os diretórios de include para todos os arquivos
include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/hal
    ${CMAKE_CURRENT_LIST_DIR}/drivers
    ${CMAKE_CURRENT_LIST_DIR}/app
    ${CMAKE_CURRENT_LIST_DIR}/tests
    ${UNITY_PATH}/src
)

# Adicione os arquivos fonte
add_executable(${PROJECT_NAME}
    app/main.c
    hal/hal_display.c
    hal/hal_temp_sensor.c
    drivers/ssd1306.c
)

# Vincule as bibliotecas
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_adc
    hardware_i2c
)

# Configuração de STDIO
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PICO_SDK_PATH}/include  # Diretório de cabeçalhos do SDK
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${UNITY_PATH}/src
)

add_executable(test_runner
    tests/test_adc_to_celsius.c  # Inclua o arquivo de teste, se for parte do build
    hal/hal_temp_sensor.c
    ${UNITY_PATH}/src/unity.c    
)

# Garanta que o test_runner tenha todas as dependências do SDK
target_link_libraries(test_runner
    pico_stdlib
    hardware_adc  
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_add_extra_outputs(${PROJECT_NAME})



# Gere arquivos extras
pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(test_runner 1)
pico_enable_stdio_uart(test_runner 0)
pico_add_extra_outputs(test_runner)
