cmake_minimum_required(VERSION 3.28)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    if(MSVC)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
            "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,
            $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,
            $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
        )
    endif()
endif()

project(PasswordManager VERSION 1.0
        DESCRIPTION "Password Manager on C"
        LANGUAGES C
)

set(CMAKE_C_STANDARD 11)  # Исправлена опечатка в STANDARD

add_executable(
        PasswordManager 
        ${CMAKE_SOURCE_DIR}/src/PasswordManager.c
        ${CMAKE_SOURCE_DIR}/src/PasswordStruct.c
        ${CMAKE_SOURCE_DIR}/src/Operations.c
)

target_include_directories(
        PasswordManager
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Выберите тип сборки (Debug или Release)" FORCE)
endif()

message(STATUS "Тип сборки: ${CMAKE_BUILD_TYPE}")

# Установка флагов отключения оптимизации для всех конфигураций
if(MSVC)
    # Для MSVC используем /Od и отключаем другие оптимизации
    target_compile_options(PasswordManager PRIVATE /utf8
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Release>:/Od>
        $<$<CONFIG:RelWithDebInfo>:/Od>
        $<$<CONFIG:MinSizeRel>:/Od>
    )
else()
    # Для GCC/Clang используем -O0
    target_compile_options(PasswordManager PRIVATE -O0)
endif()