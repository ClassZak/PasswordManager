﻿cmake_minimum_required(VERSION 3.28)



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


# Горячая перезагрузка для MSVC
if(MSVC)
	# Активация политики для Edit and Continue
	cmake_policy(SET CMP0141 NEW)

	target_compile_options(PasswordManager PRIVATE "/std:c11")

	set_property(TARGET PasswordManager PROPERTY
		MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug>:EditAndContinue>"
	)
	set_target_properties(PasswordManager PROPERTIES
		LINK_FLAGS_DEBUG "/INCREMENTAL"
	)
endif()
# Установка флагов отключения оптимизации для всех конфигураций
if(MSVC)
	# Для MSVC используем /Od и отключаем другие оптимизации
	target_compile_options(PasswordManager PRIVATE
		"$<$<CONFIG:Debug>:/Od>"
	)
else()
	# Для GCC/Clang используем -O0
	target_compile_options(PasswordManager PRIVATE -O0)
endif()