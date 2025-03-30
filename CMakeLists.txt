cmake_minimum_required(VERSION 3.28)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()



project(PasswordManager	VERSION 1.0
		DESCRIPTION "Password Manager on C"
		LANGUAGES C
)

set(CMAKE_C_STANDART 11)

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

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O0")