# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Definir a variável PROJECT para selecionar o projeto a ser compilado
set(PROJECT "iot_security_lab" CACHE STRING "Selecione o projeto a compilar: iot_security_lab ou mqtt_subscribe")

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

# Definir o nome do projeto baseado na variável PROJECT
project(${PROJECT} C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Configurar os arquivos-fonte com base no projeto selecionado
if(PROJECT STREQUAL "iot_security_lab")
    add_executable(${PROJECT}
        iot_security_lab.c
        src/mqtt_comm.c
        src/wifi_conn.c
        src/xor_cipher.c
    )
elseif(PROJECT STREQUAL "mqtt_subscribe")
    add_executable(${PROJECT}
        mqtt_subscribe.c
        src/mqtt_comm_subscribe.c
        src/wifi_conn.c
        src/xor_cipher.c
    )
else()
    message(FATAL_ERROR "Projeto inválido: ${PROJECT}. Escolha 'iot_security_lab' ou 'mqtt_subscribe'.")
endif()

# Definir nome e versão do programa
pico_set_program_name(${PROJECT} "${PROJECT}")
pico_set_program_version(${PROJECT} "0.1")

# Configurar saída USB/UART (valores baseados no mqtt_subscribe, que habilita USB)
if(PROJECT STREQUAL "iot_security_lab")
    pico_enable_stdio_usb(${PROJECT} 0)
    pico_enable_stdio_uart(${PROJECT} 0)
elseif(PROJECT STREQUAL "mqtt_subscribe")
    pico_enable_stdio_usb(${PROJECT} 1)
    pico_enable_stdio_uart(${PROJECT} 0)
endif()

# Adicionar bibliotecas padrão
target_link_libraries(${PROJECT}
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_mqtt
)

# Adicionar diretórios de inclusão
target_include_directories(${PROJECT} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Adicionar bibliotecas adicionais (se necessário)
target_link_libraries(${PROJECT})

# Gerar arquivos de saída adicionais (ex.: .uf2, .hex)
pico_add_extra_outputs(${PROJECT})
