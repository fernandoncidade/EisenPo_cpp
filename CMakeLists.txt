cmake_minimum_required(VERSION 3.28)

project(EisenPo LANGUAGES CXX)

include(GNUInstallDirs)
include(CheckIPOSupported)
include(CheckCXXCompilerFlag)
include(CheckLinkerFlag)

if(POLICY CMP0069)
    cmake_policy(SET CMP0069 NEW)
endif()
if(POLICY CMP0138)
    cmake_policy(SET CMP0138 NEW)
endif()

option(EISENHOWER_BUILD_HOST_APP "Build the EisenPo desktop application" ON)
option(EISENHOWER_BUILD_MOCKS "Build development and translation utility tools from mocks" OFF)

option(EISENHOWER_ENABLE_IPO "Enable interprocedural/link-time optimization for optimized builds" ON)
option(EISENHOWER_ENABLE_NATIVE_OPTIMIZATIONS "Optimize Release binaries for this machine's CPU instruction set" OFF)
option(EISENHOWER_ENABLE_FAST_MATH "Allow aggressive floating-point optimizations in optimized builds" OFF)
option(EISENHOWER_ENABLE_AGGRESSIVE_INLINING "Enable additional favor-speed and inlining optimizations" OFF)
option(EISENHOWER_STRIP_RELEASE_BINARIES "Strip MinGW Release binaries after linking" ON)
option(EISENHOWER_PRUNE_OPTIONAL_ROOT_RUNTIME_DLLS "Remove optional root runtime DLLs that are not imported by the deployed products" ON)

set(EISENHOWER_QT_DEPLOY_TRANSLATIONS "pt_BR,en" CACHE STRING "Comma-separated Qt translation languages deployed by windeployqt")

set(BUILD_TARGET "EISENHOWER" CACHE STRING "Application build target: EISENHOWER")
set_property(CACHE BUILD_TARGET PROPERTY STRINGS EISENHOWER)
string(TOUPPER "${BUILD_TARGET}" BUILD_TARGET)

set(APP_ASSETS_SUBDIR "eisenhower")
set(APP_WINDOWS_ICON_NAME "eisenpo.ico")
set(APP_EXECUTABLE_NAME "EisenPo")
set(APP_EXPORT_DIR_BASENAME "EisenPo_cpp")
set(APP_EXPORT_ROOT_DIR "D:/MISCELANEAS/VSCode/EisenPo")

set(EISENHOWER_MSVC_ARCH "OFF" CACHE STRING "MSVC /arch value for native optimized builds: OFF, AVX, AVX2, or AVX512")
set_property(CACHE EISENHOWER_MSVC_ARCH PROPERTY STRINGS OFF AVX AVX2 AVX512)
set(EISENHOWER_GNU_ARCH "x86-64" CACHE STRING "GCC/MinGW -march value used when native optimizations are enabled")
set(EISENHOWER_GNU_TUNE "generic" CACHE STRING "GCC/MinGW -mtune value used when native optimizations are enabled")

set(EISENHOWER_PGO_MODE "OFF" CACHE STRING "Profile-guided optimization mode: OFF, GENERATE, or USE")
set_property(CACHE EISENHOWER_PGO_MODE PROPERTY STRINGS OFF GENERATE USE)
string(TOUPPER "${EISENHOWER_PGO_MODE}" EISENHOWER_PGO_MODE)
if(NOT EISENHOWER_PGO_MODE STREQUAL "OFF"
   AND NOT EISENHOWER_PGO_MODE STREQUAL "GENERATE"
   AND NOT EISENHOWER_PGO_MODE STREQUAL "USE")
    message(FATAL_ERROR "EISENHOWER_PGO_MODE must be OFF, GENERATE, or USE.")
endif()

set(EISENHOWER_PGO_DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}/pgo" CACHE PATH "Directory used by PGO generate/use builds")
set(EISENHOWER_MSVC_ARCH_VALUE "${EISENHOWER_MSVC_ARCH}")
string(TOUPPER "${EISENHOWER_MSVC_ARCH_VALUE}" EISENHOWER_MSVC_ARCH_VALUE)
if(NOT EISENHOWER_MSVC_ARCH_VALUE STREQUAL "OFF"
   AND NOT EISENHOWER_MSVC_ARCH_VALUE STREQUAL "AVX"
   AND NOT EISENHOWER_MSVC_ARCH_VALUE STREQUAL "AVX2"
   AND NOT EISENHOWER_MSVC_ARCH_VALUE STREQUAL "AVX512")
    message(FATAL_ERROR "EISENHOWER_MSVC_ARCH must be OFF, AVX, AVX2, or AVX512.")
endif()

set(EISENHOWER_BUILD_TOOLCHAIN_SUFFIX "generic" CACHE STRING "Suffix used for toolchain-specific build directories")

if(WIN32)
    enable_language(RC)

    if(MSVC)
        add_compile_options(/MP)
        set(EISENHOWER_DETECTED_TOOLCHAIN_SUFFIX "msvc")
    elseif(MINGW)
        set(EISENHOWER_DETECTED_TOOLCHAIN_SUFFIX "mingw")
    else()
        set(EISENHOWER_DETECTED_TOOLCHAIN_SUFFIX "windows")
    endif()

    if(EISENHOWER_BUILD_TOOLCHAIN_SUFFIX STREQUAL "generic")
        set(EISENHOWER_BUILD_TOOLCHAIN_SUFFIX "${EISENHOWER_DETECTED_TOOLCHAIN_SUFFIX}" CACHE STRING "Suffix used for toolchain-specific build directories" FORCE)
    endif()
endif()

set(EISENHOWER_DEFAULT_EXPORT_DIR "${APP_EXPORT_ROOT_DIR}/${APP_EXPORT_DIR_BASENAME}")
if(WIN32)
    if(EISENHOWER_BUILD_TOOLCHAIN_SUFFIX MATCHES "^msvc")
        set(EISENHOWER_DEFAULT_EXPORT_DIR "${APP_EXPORT_ROOT_DIR}/${APP_EXPORT_DIR_BASENAME}_msvc")
    elseif(EISENHOWER_BUILD_TOOLCHAIN_SUFFIX STREQUAL "mingw")
        set(EISENHOWER_DEFAULT_EXPORT_DIR "${APP_EXPORT_ROOT_DIR}/${APP_EXPORT_DIR_BASENAME}_mingw")
    endif()
endif()

set(
    EISENHOWER_EXPORT_DIR
    "${EISENHOWER_DEFAULT_EXPORT_DIR}"
    CACHE PATH
    "Additional directory that receives the deployed application runtime"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(EISENHOWER_QT_DEPLOY_TRANSLATIONS)
    set(EISENHOWER_WINDEPLOYQT_TRANSLATION_ARGS --translations "${EISENHOWER_QT_DEPLOY_TRANSLATIONS}")
else()
    set(EISENHOWER_WINDEPLOYQT_TRANSLATION_ARGS --no-translations)
endif()

set(EISENHOWER_WINDEPLOYQT_COMPILER_RUNTIME_ARGS --no-compiler-runtime)
set(EISENHOWER_WINDEPLOYQT_NO_COMPILER_RUNTIME ON)

set(EISENHOWER_MSVC_RUNTIME_ARCH "")
set(EISENHOWER_MSVC_REDIST_ROOT "")
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(EISENHOWER_MSVC_RUNTIME_ARCH "x64")
    else()
        set(EISENHOWER_MSVC_RUNTIME_ARCH "x86")
    endif()

    if(DEFINED ENV{VCToolsRedistDir} AND EXISTS "$ENV{VCToolsRedistDir}")
        set(EISENHOWER_MSVC_REDIST_ROOT "$ENV{VCToolsRedistDir}")
    elseif(DEFINED ENV{VCINSTALLDIR} AND EXISTS "$ENV{VCINSTALLDIR}/Redist/MSVC")
        set(EISENHOWER_MSVC_REDIST_ROOT "$ENV{VCINSTALLDIR}/Redist/MSVC")
    elseif(CMAKE_CXX_COMPILER MATCHES "/VC/Tools/MSVC/")
        get_filename_component(EISENHOWER_MSVC_COMPILER_ARCH_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
        get_filename_component(EISENHOWER_MSVC_COMPILER_HOST_DIR "${EISENHOWER_MSVC_COMPILER_ARCH_DIR}" DIRECTORY)
        get_filename_component(EISENHOWER_MSVC_COMPILER_BIN_DIR "${EISENHOWER_MSVC_COMPILER_HOST_DIR}" DIRECTORY)
        get_filename_component(EISENHOWER_MSVC_VERSION_DIR "${EISENHOWER_MSVC_COMPILER_BIN_DIR}" DIRECTORY)
        get_filename_component(EISENHOWER_MSVC_TOOLS_MSVC_DIR "${EISENHOWER_MSVC_VERSION_DIR}" DIRECTORY)
        get_filename_component(EISENHOWER_MSVC_TOOLS_DIR "${EISENHOWER_MSVC_TOOLS_MSVC_DIR}" DIRECTORY)
        get_filename_component(EISENHOWER_MSVC_VC_DIR "${EISENHOWER_MSVC_TOOLS_DIR}" DIRECTORY)
        if(EXISTS "${EISENHOWER_MSVC_VC_DIR}/Redist/MSVC/14.52.37025")
            set(EISENHOWER_MSVC_REDIST_ROOT "${EISENHOWER_MSVC_VC_DIR}/Redist/MSVC/14.52.37025")
        elseif(EXISTS "${EISENHOWER_MSVC_VC_DIR}/Redist/MSVC")
            set(EISENHOWER_MSVC_REDIST_ROOT "${EISENHOWER_MSVC_VC_DIR}/Redist/MSVC")
        endif()
    endif()

    if(EISENHOWER_MSVC_REDIST_ROOT STREQUAL "")
        foreach(EISENHOWER_MSVC_REDIST_EXPLICIT IN ITEMS
            "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Redist/MSVC/14.52.37025"
            "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Redist/MSVC/14.52.37025"
        )
            if(EXISTS "${EISENHOWER_MSVC_REDIST_EXPLICIT}")
                set(EISENHOWER_MSVC_REDIST_ROOT "${EISENHOWER_MSVC_REDIST_EXPLICIT}")
                break()
            endif()
        endforeach()
    endif()

    if(EISENHOWER_MSVC_REDIST_ROOT STREQUAL "")
        file(GLOB EISENHOWER_MSVC_REDIST_ROOT_CANDIDATES LIST_DIRECTORIES true
            "C:/Program Files (x86)/Microsoft Visual Studio/*/*/VC/Redist/MSVC"
            "C:/Program Files/Microsoft Visual Studio/*/*/VC/Redist/MSVC"
        )
        if(EISENHOWER_MSVC_REDIST_ROOT_CANDIDATES)
            list(SORT EISENHOWER_MSVC_REDIST_ROOT_CANDIDATES COMPARE NATURAL ORDER DESCENDING)
            list(GET EISENHOWER_MSVC_REDIST_ROOT_CANDIDATES 0 EISENHOWER_MSVC_REDIST_ROOT)
        endif()
    endif()
endif()

if(MSVC AND EISENHOWER_ENABLE_AGGRESSIVE_INLINING)
    foreach(EISENHOWER_MSVC_CONFIG RELEASE RELWITHDEBINFO)
        set(EISENHOWER_MSVC_FLAGS_VAR "CMAKE_CXX_FLAGS_${EISENHOWER_MSVC_CONFIG}")
        if(DEFINED ${EISENHOWER_MSVC_FLAGS_VAR})
            string(
                REGEX REPLACE
                "(^|[ \t]+)/Ob[0-2]($|[ \t])"
                " "
                ${EISENHOWER_MSVC_FLAGS_VAR}
                "${${EISENHOWER_MSVC_FLAGS_VAR}}"
            )
            string(STRIP "${${EISENHOWER_MSVC_FLAGS_VAR}}" ${EISENHOWER_MSVC_FLAGS_VAR})
        endif()
    endforeach()
endif()

if(WIN32)
    set(EISENHOWER_SELECTED_QT_CMAKE_DIR "")
    set(EISENHOWER_QT_CMAKE_CANDIDATE_DIRS "")

    foreach(EISENHOWER_QT_DIR_VAR Qt6_DIR QT_DIR)
        if(DEFINED ${EISENHOWER_QT_DIR_VAR})
            get_filename_component(EISENHOWER_QT_DIR_VALUE "${${EISENHOWER_QT_DIR_VAR}}" ABSOLUTE)
            if(EXISTS "${EISENHOWER_QT_DIR_VALUE}/Qt6Config.cmake")
                get_filename_component(EISENHOWER_QT_CMAKE_DIR_FROM_VAR "${EISENHOWER_QT_DIR_VALUE}" DIRECTORY)
                list(APPEND EISENHOWER_QT_CMAKE_CANDIDATE_DIRS "${EISENHOWER_QT_CMAKE_DIR_FROM_VAR}")
            elseif(EXISTS "${EISENHOWER_QT_DIR_VALUE}/Qt6/Qt6Config.cmake")
                list(APPEND EISENHOWER_QT_CMAKE_CANDIDATE_DIRS "${EISENHOWER_QT_DIR_VALUE}")
            endif()
        endif()
    endforeach()

    foreach(EISENHOWER_PREFIX_PATH IN LISTS CMAKE_PREFIX_PATH)
        get_filename_component(EISENHOWER_PREFIX_PATH_ABSOLUTE "${EISENHOWER_PREFIX_PATH}" ABSOLUTE)
        if(EXISTS "${EISENHOWER_PREFIX_PATH_ABSOLUTE}/Qt6/Qt6Config.cmake")
            list(APPEND EISENHOWER_QT_CMAKE_CANDIDATE_DIRS "${EISENHOWER_PREFIX_PATH_ABSOLUTE}")
        elseif(EXISTS "${EISENHOWER_PREFIX_PATH_ABSOLUTE}/lib/cmake/Qt6/Qt6Config.cmake")
            list(APPEND EISENHOWER_QT_CMAKE_CANDIDATE_DIRS "${EISENHOWER_PREFIX_PATH_ABSOLUTE}/lib/cmake")
        elseif(EXISTS "${EISENHOWER_PREFIX_PATH_ABSOLUTE}/Qt6Config.cmake")
            get_filename_component(EISENHOWER_QT_CMAKE_DIR_FROM_PREFIX "${EISENHOWER_PREFIX_PATH_ABSOLUTE}" DIRECTORY)
            list(APPEND EISENHOWER_QT_CMAKE_CANDIDATE_DIRS "${EISENHOWER_QT_CMAKE_DIR_FROM_PREFIX}")
        endif()
    endforeach()

    set(EISENHOWER_QT_COMPILER_DIR "")
    if(MSVC)
        set(EISENHOWER_QT_COMPILER_DIR "msvc2022_64")
    elseif(MINGW)
        set(EISENHOWER_QT_COMPILER_DIR "mingw_64")
    endif()

    if(EISENHOWER_QT_COMPILER_DIR)
        file(GLOB EISENHOWER_QT_CONFIG_CANDIDATES "C:/Qt/*/${EISENHOWER_QT_COMPILER_DIR}/lib/cmake/Qt6/Qt6Config.cmake")
        list(SORT EISENHOWER_QT_CONFIG_CANDIDATES COMPARE NATURAL ORDER DESCENDING)
        foreach(EISENHOWER_QT_CONFIG_CANDIDATE IN LISTS EISENHOWER_QT_CONFIG_CANDIDATES)
            get_filename_component(EISENHOWER_QT6_CONFIG_DIR "${EISENHOWER_QT_CONFIG_CANDIDATE}" DIRECTORY)
            get_filename_component(EISENHOWER_QT_CMAKE_DIR "${EISENHOWER_QT6_CONFIG_DIR}" DIRECTORY)
            list(APPEND EISENHOWER_QT_CMAKE_CANDIDATE_DIRS "${EISENHOWER_QT_CMAKE_DIR}")
        endforeach()
    endif()

    list(REMOVE_DUPLICATES EISENHOWER_QT_CMAKE_CANDIDATE_DIRS)
    foreach(EISENHOWER_QT_CMAKE_DIR_CANDIDATE IN LISTS EISENHOWER_QT_CMAKE_CANDIDATE_DIRS)
        if(EXISTS "${EISENHOWER_QT_CMAKE_DIR_CANDIDATE}/Qt6/Qt6Config.cmake")
            set(EISENHOWER_SELECTED_QT_CMAKE_DIR "${EISENHOWER_QT_CMAKE_DIR_CANDIDATE}")
            break()
        endif()
    endforeach()

    if(EISENHOWER_SELECTED_QT_CMAKE_DIR)
        get_cmake_property(EISENHOWER_CACHE_VARIABLES CACHE_VARIABLES)
        foreach(EISENHOWER_CACHE_VAR IN LISTS EISENHOWER_CACHE_VARIABLES)
            if(EISENHOWER_CACHE_VAR MATCHES "^Qt6.*_DIR$" OR EISENHOWER_CACHE_VAR STREQUAL "Qt6_DIR" OR EISENHOWER_CACHE_VAR STREQUAL "QT_DIR")
                unset(${EISENHOWER_CACHE_VAR} CACHE)
            endif()
        endforeach()

        list(PREPEND CMAKE_PREFIX_PATH "${EISENHOWER_SELECTED_QT_CMAKE_DIR}")
        set(QT_DIR "${EISENHOWER_SELECTED_QT_CMAKE_DIR}/Qt6" CACHE PATH "Qt6 package directory" FORCE)
        set(Qt6_DIR "${EISENHOWER_SELECTED_QT_CMAKE_DIR}/Qt6" CACHE PATH "Qt6 package directory" FORCE)

        foreach(EISENHOWER_QT_COMPONENT Core Gui Widgets Multimedia Network Sql LinguistTools)
            if(EXISTS "${EISENHOWER_SELECTED_QT_CMAKE_DIR}/Qt6${EISENHOWER_QT_COMPONENT}/Qt6${EISENHOWER_QT_COMPONENT}Config.cmake")
                set(
                    Qt6${EISENHOWER_QT_COMPONENT}_DIR
                    "${EISENHOWER_SELECTED_QT_CMAKE_DIR}/Qt6${EISENHOWER_QT_COMPONENT}"
                    CACHE PATH "Qt6${EISENHOWER_QT_COMPONENT} package directory" FORCE
                )
            endif()
        endforeach()
    endif()
endif()

find_package(Qt6 6.12 REQUIRED COMPONENTS Core Gui Widgets Multimedia Sql Network)



set(EISENHOWER_FORCE_IPO_FOR_PGO OFF)
if(MSVC AND NOT EISENHOWER_PGO_MODE STREQUAL "OFF")
    set(EISENHOWER_FORCE_IPO_FOR_PGO ON)
endif()

set(EISENHOWER_IPO_REQUESTED OFF)
if(EISENHOWER_ENABLE_IPO OR EISENHOWER_FORCE_IPO_FOR_PGO)
    set(EISENHOWER_IPO_REQUESTED ON)
endif()

set(EISENHOWER_IPO_SUPPORTED OFF)
if(EISENHOWER_IPO_REQUESTED)
    check_ipo_supported(RESULT EISENHOWER_IPO_SUPPORTED OUTPUT EISENHOWER_IPO_ERROR LANGUAGES CXX)
    if(NOT EISENHOWER_IPO_SUPPORTED)
        message(STATUS "IPO/LTO não suportado nesta toolchain (utilizando otimizações padrão sem IPO).")
    endif()
endif()

if(EISENHOWER_FORCE_IPO_FOR_PGO AND NOT EISENHOWER_IPO_SUPPORTED)
    message(FATAL_ERROR "MSVC PGO requires IPO/LTCG support. Disable EISENHOWER_PGO_MODE or use a toolchain with IPO support.")
endif()

if(EISENHOWER_PGO_MODE STREQUAL "USE" AND NOT EXISTS "${EISENHOWER_PGO_DATA_DIR}")
    message(WARNING "EISENHOWER_PGO_MODE=USE was selected, but EISENHOWER_PGO_DATA_DIR does not exist yet: ${EISENHOWER_PGO_DATA_DIR}")
endif()

message(
    STATUS
    "Eisenhower performance profile: IPO=${EISENHOWER_IPO_SUPPORTED}; native=${EISENHOWER_ENABLE_NATIVE_OPTIMIZATIONS}; fast-math=${EISENHOWER_ENABLE_FAST_MATH}; PGO=${EISENHOWER_PGO_MODE}"
)

set(EISENHOWER_OPTIMIZED_CONFIG_EXPR "$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>")

function(eisenhower_flag_cache_var PREFIX FLAG OUT_VAR)
    string(MAKE_C_IDENTIFIER "${PREFIX}_${CMAKE_CXX_COMPILER_ID}_${CMAKE_CXX_COMPILER_VERSION}_${FLAG}" FLAG_VAR)
    string(TOUPPER "${FLAG_VAR}" FLAG_VAR)
    set("${OUT_VAR}" "${FLAG_VAR}" PARENT_SCOPE)
endfunction()

function(eisenhower_add_optimized_compile_options TARGET_NAME)
    foreach(FLAG IN LISTS ARGN)
        target_compile_options("${TARGET_NAME}" PRIVATE "$<${EISENHOWER_OPTIMIZED_CONFIG_EXPR}:${FLAG}>")
    endforeach()
endfunction()

function(eisenhower_add_optimized_link_options TARGET_NAME)
    foreach(FLAG IN LISTS ARGN)
        target_link_options("${TARGET_NAME}" PRIVATE "$<${EISENHOWER_OPTIMIZED_CONFIG_EXPR}:${FLAG}>")
    endforeach()
endfunction()

function(eisenhower_add_optimized_compile_option_if_supported TARGET_NAME FLAG)
    eisenhower_flag_cache_var("EISENHOWER_HAS_CXX_FLAG" "${FLAG}" FLAG_VAR)
    check_cxx_compiler_flag("${FLAG}" "${FLAG_VAR}")
    if(${FLAG_VAR})
        eisenhower_add_optimized_compile_options("${TARGET_NAME}" "${FLAG}")
    endif()
endfunction()

function(eisenhower_add_optimized_link_option_if_supported TARGET_NAME FLAG)
    eisenhower_flag_cache_var("EISENHOWER_HAS_LINK_FLAG" "${FLAG}" FLAG_VAR)
    check_linker_flag(CXX "${FLAG}" "${FLAG_VAR}")
    if(${FLAG_VAR})
        eisenhower_add_optimized_link_options("${TARGET_NAME}" "${FLAG}")
    endif()
endfunction()

function(eisenhower_target_pgo_dir TARGET_NAME OUT_VAR)
    file(TO_CMAKE_PATH "${EISENHOWER_PGO_DATA_DIR}/${TARGET_NAME}" EISENHOWER_TARGET_PGO_DIR)
    file(MAKE_DIRECTORY "${EISENHOWER_TARGET_PGO_DIR}")
    set("${OUT_VAR}" "${EISENHOWER_TARGET_PGO_DIR}" PARENT_SCOPE)
endfunction()

function(eisenhower_apply_app_identity TARGET_NAME)
    if(NOT TARGET "${TARGET_NAME}")
        return()
    endif()

    target_compile_definitions("${TARGET_NAME}" PRIVATE "APP_${BUILD_TARGET}")
endfunction()

function(eisenhower_apply_performance_options TARGET_NAME)
    if(NOT TARGET "${TARGET_NAME}")
        return()
    endif()

    eisenhower_apply_app_identity("${TARGET_NAME}")

    target_compile_definitions(
        "${TARGET_NAME}"
        PRIVATE
            $<$<CONFIG:Release>:QT_NO_DEBUG_OUTPUT>
            $<$<CONFIG:Release>:QT_NO_INFO_OUTPUT>
            $<$<CONFIG:RelWithDebInfo>:QT_NO_DEBUG_OUTPUT>
            $<$<CONFIG:RelWithDebInfo>:QT_NO_INFO_OUTPUT>
    )

    if(MSVC)
        set_property(TARGET "${TARGET_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

        eisenhower_add_optimized_compile_options(
            "${TARGET_NAME}"
            /O2
            /Oi
            /Gy
            /Gw
            /GF
            /Ot
            /Zc:inline
        )

        if(EISENHOWER_ENABLE_FAST_MATH)
            eisenhower_add_optimized_compile_options("${TARGET_NAME}" /fp:fast)
        endif()

        if(EISENHOWER_ENABLE_AGGRESSIVE_INLINING)
            eisenhower_add_optimized_compile_option_if_supported("${TARGET_NAME}" "/Ob3")
        endif()

        if(EISENHOWER_ENABLE_NATIVE_OPTIMIZATIONS AND NOT EISENHOWER_MSVC_ARCH_VALUE STREQUAL "OFF")
            eisenhower_add_optimized_compile_option_if_supported("${TARGET_NAME}" "/arch:${EISENHOWER_MSVC_ARCH_VALUE}")
        endif()

        eisenhower_add_optimized_link_options(
            "${TARGET_NAME}"
            /OPT:REF
            /OPT:ICF
            /INCREMENTAL:NO
            /RELEASE
        )

        if(EISENHOWER_PGO_MODE STREQUAL "GENERATE")
            eisenhower_target_pgo_dir("${TARGET_NAME}" EISENHOWER_TARGET_PGO_DIR)
            set(EISENHOWER_TARGET_PGD "${EISENHOWER_TARGET_PGO_DIR}/${TARGET_NAME}.pgd")
            target_link_options(
                "${TARGET_NAME}"
                PRIVATE
                    "$<${EISENHOWER_OPTIMIZED_CONFIG_EXPR}:/GENPROFILE:PGD=${EISENHOWER_TARGET_PGD}>"
            )
        elseif(EISENHOWER_PGO_MODE STREQUAL "USE")
            eisenhower_target_pgo_dir("${TARGET_NAME}" EISENHOWER_TARGET_PGO_DIR)
            set(EISENHOWER_TARGET_PGD "${EISENHOWER_TARGET_PGO_DIR}/${TARGET_NAME}.pgd")
            target_link_options(
                "${TARGET_NAME}"
                PRIVATE
                    "$<${EISENHOWER_OPTIMIZED_CONFIG_EXPR}:/USEPROFILE:PGD=${EISENHOWER_TARGET_PGD}>"
            )
        endif()
    elseif(MINGW)
        set(EISENHOWER_GNU_OPTIMIZATION_FLAG "-O3")
        if(EISENHOWER_ENABLE_FAST_MATH)
            set(EISENHOWER_GNU_OPTIMIZATION_FLAG "-Ofast")
        endif()

        eisenhower_add_optimized_compile_options(
            "${TARGET_NAME}"
            "${EISENHOWER_GNU_OPTIMIZATION_FLAG}"
            -ffunction-sections
            -fdata-sections
            -fomit-frame-pointer
        )

        if(EISENHOWER_ENABLE_AGGRESSIVE_INLINING)
            eisenhower_add_optimized_compile_option_if_supported("${TARGET_NAME}" "-finline-functions")
            eisenhower_add_optimized_compile_option_if_supported("${TARGET_NAME}" "-fipa-pta")
            eisenhower_add_optimized_compile_option_if_supported("${TARGET_NAME}" "-fdevirtualize-at-ltrans")
        endif()

        if(EISENHOWER_ENABLE_NATIVE_OPTIMIZATIONS)
            if(EISENHOWER_GNU_ARCH)
                eisenhower_add_optimized_compile_option_if_supported("${TARGET_NAME}" "-march=${EISENHOWER_GNU_ARCH}")
            endif()
            if(EISENHOWER_GNU_TUNE)
                eisenhower_add_optimized_compile_option_if_supported("${TARGET_NAME}" "-mtune=${EISENHOWER_GNU_TUNE}")
            endif()
        endif()

        eisenhower_add_optimized_link_options(
            "${TARGET_NAME}"
            -Wl,--gc-sections
            -Wl,-O1
        )

        if(EISENHOWER_STRIP_RELEASE_BINARIES)
            target_link_options("${TARGET_NAME}" PRIVATE $<$<CONFIG:Release>:-Wl,--strip-all>)
        endif()

        if(EISENHOWER_IPO_SUPPORTED)
            eisenhower_add_optimized_link_option_if_supported("${TARGET_NAME}" "-fuse-linker-plugin")
            eisenhower_add_optimized_link_option_if_supported("${TARGET_NAME}" "-Wl,--icf=safe")
        endif()

        if(EISENHOWER_PGO_MODE STREQUAL "GENERATE")
            eisenhower_target_pgo_dir("${TARGET_NAME}" EISENHOWER_TARGET_PGO_DIR)
            eisenhower_add_optimized_compile_options("${TARGET_NAME}" "-fprofile-generate=${EISENHOWER_TARGET_PGO_DIR}")
            eisenhower_add_optimized_link_options("${TARGET_NAME}" "-fprofile-generate=${EISENHOWER_TARGET_PGO_DIR}")
        elseif(EISENHOWER_PGO_MODE STREQUAL "USE")
            eisenhower_target_pgo_dir("${TARGET_NAME}" EISENHOWER_TARGET_PGO_DIR)
            eisenhower_add_optimized_compile_options(
                "${TARGET_NAME}"
                "-fprofile-use=${EISENHOWER_TARGET_PGO_DIR}"
                -fprofile-correction
            )
            eisenhower_add_optimized_link_options("${TARGET_NAME}" "-fprofile-use=${EISENHOWER_TARGET_PGO_DIR}")
        endif()
    endif()

    if(EISENHOWER_IPO_SUPPORTED)
        set_property(TARGET "${TARGET_NAME}" PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        set_property(TARGET "${TARGET_NAME}" PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)
    endif()
endfunction()



function(eisenhower_append_app_assets_deploy OUT_VAR TARGET_ASSETS_ROOT PRODUCT_ASSETS_DIR LICENSES_DIR PRODUCT_ASSETS_SUBDIR)
    set(EISENHOWER_DEPLOY_COMMANDS ${${OUT_VAR}})
    list(
        APPEND
        EISENHOWER_DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E rm -rf "${TARGET_ASSETS_ROOT}/assets"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${TARGET_ASSETS_ROOT}/assets"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${LICENSES_DIR}"
            "${TARGET_ASSETS_ROOT}/assets/LICENSES"
    )
    foreach(EISENHOWER_DOC_SUBDIR IN ITEMS ABOUT CLC COPYRIGHT EULA NOTICES PRIVACY_POLICY RELEASE)
        if(EXISTS "${PRODUCT_ASSETS_DIR}/${EISENHOWER_DOC_SUBDIR}")
            list(
                APPEND
                EISENHOWER_DEPLOY_COMMANDS
                COMMAND "${CMAKE_COMMAND}" -E copy_directory
                    "${PRODUCT_ASSETS_DIR}/${EISENHOWER_DOC_SUBDIR}"
                    "${TARGET_ASSETS_ROOT}/assets/${PRODUCT_ASSETS_SUBDIR}/${EISENHOWER_DOC_SUBDIR}"
            )
        endif()
    endforeach()
    list(
        APPEND
        EISENHOWER_DEPLOY_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${TARGET_ASSETS_ROOT}/assets/${PRODUCT_ASSETS_SUBDIR}/icones"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${PRODUCT_ASSETS_DIR}/icones/eisenpo.ico"
            "${PRODUCT_ASSETS_DIR}/icones/eisenpo.png"
            "${PRODUCT_ASSETS_DIR}/icones/calendar.png"
            "${TARGET_ASSETS_ROOT}/assets/${PRODUCT_ASSETS_SUBDIR}/icones/"
    )
    set("${OUT_VAR}" ${EISENHOWER_DEPLOY_COMMANDS} PARENT_SCOPE)
endfunction()

if(EISENHOWER_BUILD_MOCKS)
    add_subdirectory(mocks)
endif()

set(EISENHOWER_SHARED_UTIL_SOURCES
    source/utils/ApplicationPathUtils.cpp
    source/utils/CaminhoPersistenteUtils.cpp
    source/utils/FeynmanLabelsConfig.cpp
    source/utils/IconUtils.cpp
    source/utils/LinkedTitleUnderlineDelegate.cpp
    source/utils/LogManager.cpp
    source/utils/PluginRegistry.cpp
    source/utils/UnifiedTaskStore.cpp
)



set(EISENHOWER_HOST_SOURCES
    main.cpp
    source/eisenhower_app.cpp
    source/app/app_01_eisenhower_matrix_app.hpp
    source/app/app_01_eisenhower_matrix_app.cpp
    source/app/EisenhowerGanttHostAdapter.hpp
    source/app/EisenhowerGanttHostAdapter.cpp
    source/app/app_02_get_text.hpp
    source/app/app_02_get_text.cpp
    source/app/app_03_initUI.hpp
    source/app/app_03_initUI.cpp
    source/app/app_04_refresh_module_tabs.hpp
    source/app/app_04_refresh_module_tabs.cpp
    source/app/app_05_load_feynman_plugin.hpp
    source/app/app_05_load_feynman_plugin.cpp
    source/app/app_06_send_task_to_feynman.hpp
    source/app/app_06_send_task_to_feynman.cpp
    source/app/app_07_set_task_layout.hpp
    source/app/app_07_set_task_layout.cpp
    source/app/app_08_apply_theme_palette.hpp
    source/app/app_08_apply_theme_palette.cpp
    source/app/app_09_add_task.hpp
    source/app/app_09_add_task.cpp
    source/app/app_10_atualizar_textos.hpp
    source/app/app_10_atualizar_textos.cpp
    source/utils/AppConfig.hpp
    source/utils/ApplicationPathUtils.cpp
    source/utils/BarraTitulosUtils.cpp
    source/utils/CaminhoPersistenteUtils.cpp
    source/utils/FeynmanLabelsConfig.cpp
    source/utils/IconUtils.cpp
    source/utils/LinkedTitleUnderlineDelegate.cpp
    source/utils/LogManager.cpp
    source/utils/PluginRegistry.cpp
    source/utils/TrialManager.cpp
    source/utils/UnifiedTaskStore.cpp
    source/models/model_01_TaskRecordListModel.cpp
    source/language/tr_01_gerenciadorTraducao.cpp
    source/services/agregador_01_services.cpp
    source/services/incore_15_ProductivityFeatures.cpp
    source/services/RecursosProdutividade/recpro_01_utilitarios_compartilhados.cpp
    source/services/RecursosProdutividade/recpro_02_busca_painel.cpp
    source/services/RecursosProdutividade/recpro_03_recorrencias.cpp
    source/services/RecursosProdutividade/recpro_04_lembretes_locais.cpp
    source/services/RecursosProdutividade/recpro_05_relatorios_fluxos.cpp
    source/services/RecursosProdutividade/recpro_06_revisoes_plugins.cpp
    source/services/RecursosProdutividade/recpro_07_filtros.hpp
    source/services/RecursosProdutividade/recpro_07_filtros.cpp
    source/services/incore_01_initUI.cpp
    source/services/incore_02_add_placeholder.cpp
    source/services/incore_03_criar_menu_configuracoes.cpp
    source/services/incore_04_definir_idioma.cpp
    source/services/incore_05_atualizar_textos.cpp
    source/services/incore_06_atualizar_placeholders.cpp
    source/services/incore_07_MostrarMenuContexto.cpp
    source/services/incore_08_exibir_sobre.cpp
    source/services/incore_09_Arquivo.cpp
    source/services/incore_10_task_list_widget.cpp
    source/services/incore_11_atualizar_itens_tarefas.cpp
    source/services/incore_12_Manual.cpp
    source/services/incore_13_prioridade_display.cpp
    source/services/incore_14_EditTaskDialog.cpp
    source/services/incore_16_manual_pt-BR.cpp
    source/services/incore_17_manual_en-US.cpp
    source/services/Arquivo/arq_01_clear_all_lists.cpp
    source/services/Arquivo/arq_02_add_placeholders_after_clear.cpp
    source/services/Arquivo/arq_03_novo.cpp
    source/services/Arquivo/arq_04_limpar_tudo.cpp
    source/services/Arquivo/arq_05_parse_text_date_time.cpp
    source/services/Arquivo/arq_06_abrir_arquivo.cpp
    source/services/Arquivo/arq_07_salvar_como.cpp
    source/services/MostrarMenuContexto/mmc_01_compute_widget_height.cpp
    source/services/MostrarMenuContexto/mmc_02_apply_widget_min_height.cpp
    source/services/MostrarMenuContexto/mmc_03_effective_locale.cpp
    source/services/MostrarMenuContexto/mmc_04_apply_locale_to_dateedit.cpp
    source/services/MostrarMenuContexto/mmc_05_is_completed_list.cpp
    source/services/MostrarMenuContexto/mmc_06_quadrant_options.cpp
    source/services/MostrarMenuContexto/mmc_07_target_list_for_quadrant.cpp
    source/services/MostrarMenuContexto/mmc_08_find_source_item_for_calendar.cpp
    source/services/MostrarMenuContexto/mmc_09_base_text_from_item.cpp
    source/services/MostrarMenuContexto/mmc_10_build_display_and_tooltip.cpp
    source/services/MostrarMenuContexto/mmc_11_edit_task_dialog.cpp
    source/services/MostrarMenuContexto/mmc_12_move_to_quadrant.cpp
    source/services/MostrarMenuContexto/mmc_13_mostrar_menu_contexto.cpp
    source/ui/agregador_01_ui.cpp
    source/ui/ui_01_SobreDialog.cpp
    source/ui/ui_02_OpcoesSobre.cpp
    source/ui/ui_03_add_task.cpp
    source/ui/ui_04_handle_item_checked.cpp
    source/ui/ui_05_move_item_between_lists.cpp
    source/ui/ui_06_remove_task.cpp
    source/ui/ui_07_save_tasks.cpp
    source/ui/ui_08_load_tasks.cpp
    source/ui/ui_09_Calendar.cpp
    source/ui/ui_10_BadgeCalendarWidget.cpp
    source/ui/ui_11_NativeLineEditFrame.cpp
    source/ui/ui_12_CalendarDialog.cpp
    source/ui/CalendarDialog/agregador_01_CalendarDialog.cpp
    source/ui/CalendarDialog/caldial_01_closeEvent.cpp
    source/ui/CalendarDialog/caldial_02_open_linked_file.cpp
    source/ui/CalendarDialog/caldial_03_collect_tasks.cpp
    source/ui/CalendarDialog/caldial_04_update_task_list.cpp
    source/ui/CalendarDialog/caldial_05_on_calendar_context_menu.cpp
    source/ui/CalendarDialog/caldial_06_prompt_edit_task_for_date.cpp
    source/ui/CalendarDialog/caldial_07_prompt_remove_task_for_date.cpp
    source/ui/CalendarDialog/caldial_08_update_badge_counts.cpp
    source/ui/CalendarDialog/caldial_09_on_language_changed.cpp
    source/ui/CalendarDialog/caldial_10_apply_locale_to_calendar.cpp
    source/ui/CalendarDialog/caldial_11_get_task_dates.cpp
    source/ui/CalendarDialog/caldial_12_apply_highlighted_dates.cpp
    source/ui/ui_13_RotatedTabButton.cpp
    source/ui/ui_14_CalendarPanel.cpp
    source/ui/ui_15_edit_task.cpp
    source/ui/ui_16_YearCalendarWidget.cpp
)

file(
    GLOB_RECURSE
    EISENHOWER_POMODORO_MODULE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/modules/pomodoro/*.cpp"
)

file(
    GLOB_RECURSE
    EISENHOWER_FEYNMAN_MODULE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/modules/feynman/*.cpp"
)

file(
    GLOB_RECURSE
    EISENHOWER_AGENDA_MODULE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/modules/agenda/*.cpp"
)

file(
    GLOB_RECURSE
    EISENHOWER_GANTT_MODULE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/modules/gantt/*.cpp"
)

list(
    APPEND
    EISENHOWER_HOST_SOURCES
    ${EISENHOWER_POMODORO_MODULE_SOURCES}
    ${EISENHOWER_FEYNMAN_MODULE_SOURCES}
    ${EISENHOWER_AGENDA_MODULE_SOURCES}
    ${EISENHOWER_GANTT_MODULE_SOURCES}
)
list(REMOVE_DUPLICATES EISENHOWER_HOST_SOURCES)

if(EISENHOWER_BUILD_HOST_APP)
    add_executable(EisenPo WIN32 ${EISENHOWER_HOST_SOURCES})
    set_target_properties(EisenPo PROPERTIES OUTPUT_NAME "${APP_EXECUTABLE_NAME}")
    add_executable(EisenhowerOrganizer ALIAS EisenPo)

    target_include_directories(EisenPo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(EisenPo
        PRIVATE
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
            Qt6::Multimedia
            Qt6::Sql
            Qt6::Network
            Advapi32
            User32
    )

    eisenhower_apply_performance_options(EisenPo)

    if(WIN32)
        set(EISENHOWER_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/assets")
        set(EISENHOWER_APP_ASSETS_DIR "${EISENHOWER_ASSETS_DIR}/${APP_ASSETS_SUBDIR}")
        set(EISENHOWER_LICENSES_DIR "${EISENHOWER_ASSETS_DIR}/LICENSES")
        set(EISENHOWER_APP_ICON "${EISENHOWER_APP_ASSETS_DIR}/icones/${APP_WINDOWS_ICON_NAME}")
        set(EISENHOWER_TRANSLATIONS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/language/translations")
        set(EISENHOWER_POMODORO_SOUND_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/modules/pomodoro/Pomodoro/Alarme/sound")

        if(NOT EXISTS "${EISENHOWER_APP_ASSETS_DIR}")
            message(FATAL_ERROR "Application assets directory was not found at ${EISENHOWER_APP_ASSETS_DIR}.")
        endif()
        if(NOT EXISTS "${EISENHOWER_LICENSES_DIR}")
            message(FATAL_ERROR "Application licenses directory was not found at ${EISENHOWER_LICENSES_DIR}.")
        endif()
        if(EXISTS "${EISENHOWER_APP_ICON}")
            file(TO_NATIVE_PATH "${EISENHOWER_APP_ICON}" EISENHOWER_APP_ICON_NATIVE)
            string(REPLACE "\\" "\\\\" EISENPO_APP_ICON_RC_PATH "${EISENHOWER_APP_ICON_NATIVE}")
            set(EISENHOWER_APP_ICON_RC_PATH "${EISENPO_APP_ICON_RC_PATH}")
            configure_file(
                "${CMAKE_CURRENT_SOURCE_DIR}/source/windows_app_icon.rc.in"
                "${CMAKE_CURRENT_BINARY_DIR}/EisenPo_app_icon.rc"
                @ONLY
            )
            target_sources(
                EisenPo
                PRIVATE
                    "${CMAKE_CURRENT_BINARY_DIR}/EisenPo_app_icon.rc"
            )
        else()
            message(WARNING "Application icon was not found at ${EISENHOWER_APP_ICON}. The Windows executable will be built without the requested icon.")
        endif()

        file(TO_CMAKE_PATH "${EISENHOWER_EXPORT_DIR}" EISENHOWER_EXPORT_DIR_NORMALIZED)

        set(_eisenpo_qmake_executable "")
        foreach(_eisenpo_qt_loc_prop IN ITEMS
            IMPORTED_LOCATION_RELEASE
            IMPORTED_LOCATION_RELWITHDEBINFO
            IMPORTED_LOCATION
            IMPORTED_LOCATION_DEBUG
        )
            get_target_property(_eisenpo_qt_loc_candidate Qt6::qmake ${_eisenpo_qt_loc_prop})
            if(_eisenpo_qt_loc_candidate AND NOT _eisenpo_qt_loc_candidate MATCHES "-NOTFOUND$" AND EXISTS "${_eisenpo_qt_loc_candidate}")
                set(_eisenpo_qmake_executable "${_eisenpo_qt_loc_candidate}")
                break()
            endif()
        endforeach()

        if(_eisenpo_qmake_executable)
            get_filename_component(QT6_BIN_DIR "${_eisenpo_qmake_executable}" DIRECTORY)
        else()
            get_filename_component(QT6_BIN_DIR "${Qt6_DIR}/../../../bin" ABSOLUTE)
        endif()
        get_filename_component(QT6_PREFIX_DIR "${QT6_BIN_DIR}/.." ABSOLUTE)

        find_program(WINDEPLOYQT_EXECUTABLE
            NAMES windeployqt windeployqt6
            HINTS "${QT6_BIN_DIR}"
            NO_DEFAULT_PATH
        )
        if(NOT WINDEPLOYQT_EXECUTABLE)
            find_program(WINDEPLOYQT_EXECUTABLE
                NAMES windeployqt windeployqt6
                HINTS "${QT6_BIN_DIR}"
            )
        endif()

        find_program(QTPATHS_EXECUTABLE
            NAMES qtpaths6 qtpaths
            HINTS "${QT6_BIN_DIR}"
            NO_DEFAULT_PATH
        )
        if(NOT QTPATHS_EXECUTABLE)
            find_program(QTPATHS_EXECUTABLE
                NAMES qtpaths6 qtpaths
                HINTS "${QT6_BIN_DIR}"
            )
        endif()

        if(WINDEPLOYQT_EXECUTABLE AND QTPATHS_EXECUTABLE AND EXISTS "${WINDEPLOYQT_EXECUTABLE}" AND EXISTS "${QTPATHS_EXECUTABLE}")
            file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")
            configure_file(
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake/qt.conf.in"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/qt.conf"
                @ONLY
            )

            set(EISENHOWER_BUILD_TREE_OPTIONAL_ROOT_RUNTIME_PRUNE_COMMANDS)
            set(EISENHOWER_EXPORT_OPTIONAL_ROOT_RUNTIME_PRUNE_COMMANDS)
            set(EISENHOWER_PRUNE_ROOT_MSVC_RUNTIME_DLLS OFF)
            set(EISENHOWER_PRUNE_ROOT_MINGW_RUNTIME_DLLS OFF)
            if(MSVC)
                set(EISENHOWER_PRUNE_ROOT_MSVC_RUNTIME_DLLS OFF)
                set(EISENHOWER_PRUNE_ROOT_MINGW_RUNTIME_DLLS ON)
            elseif(MINGW)
                set(EISENHOWER_PRUNE_ROOT_MSVC_RUNTIME_DLLS ON)
                set(EISENHOWER_PRUNE_ROOT_MINGW_RUNTIME_DLLS OFF)
            endif()

            set(EISENHOWER_MINGW_BIN_DIR "")
            if(MINGW AND CMAKE_CXX_COMPILER)
                get_filename_component(EISENHOWER_MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
            endif()

            set(EISENHOWER_BUILD_TREE_ASSET_COPY_COMMANDS)
            eisenhower_append_app_assets_deploy(
                EISENHOWER_BUILD_TREE_ASSET_COPY_COMMANDS
                "$<TARGET_FILE_DIR:EisenPo>"
                "${EISENHOWER_APP_ASSETS_DIR}"
                "${EISENHOWER_LICENSES_DIR}"
                "${APP_ASSETS_SUBDIR}"
            )
            set(EISENHOWER_EXPORT_ASSET_COPY_COMMANDS)
            if(EISENHOWER_EXPORT_DIR_NORMALIZED)
                eisenhower_append_app_assets_deploy(
                    EISENHOWER_EXPORT_ASSET_COPY_COMMANDS
                    "${EISENHOWER_EXPORT_DIR_NORMALIZED}"
                    "${EISENHOWER_APP_ASSETS_DIR}"
                    "${EISENHOWER_LICENSES_DIR}"
                    "${APP_ASSETS_SUBDIR}"
                )
            endif()
            if(EISENHOWER_PRUNE_OPTIONAL_ROOT_RUNTIME_DLLS)
                list(
                    APPEND
                    EISENHOWER_BUILD_TREE_OPTIONAL_ROOT_RUNTIME_PRUNE_COMMANDS
                    COMMAND "${CMAKE_COMMAND}"
                        "-DDEPLOY_ROOT=$<TARGET_FILE_DIR:EisenPo>"
                        "-DPRUNE_MSVC_RUNTIME=${EISENHOWER_PRUNE_ROOT_MSVC_RUNTIME_DLLS}"
                        "-DPRUNE_MINGW_RUNTIME=${EISENHOWER_PRUNE_ROOT_MINGW_RUNTIME_DLLS}"
                        "-DCONFIG=$<CONFIG>"
                        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/prune_optional_root_runtime_dlls.cmake"
                )
                if(EISENHOWER_EXPORT_DIR_NORMALIZED)
                    list(
                        APPEND
                        EISENHOWER_EXPORT_OPTIONAL_ROOT_RUNTIME_PRUNE_COMMANDS
                        COMMAND "${CMAKE_COMMAND}"
                            "-DDEPLOY_ROOT=${EISENHOWER_EXPORT_DIR_NORMALIZED}"
                            "-DPRUNE_MSVC_RUNTIME=${EISENHOWER_PRUNE_ROOT_MSVC_RUNTIME_DLLS}"
                            "-DPRUNE_MINGW_RUNTIME=${EISENHOWER_PRUNE_ROOT_MINGW_RUNTIME_DLLS}"
                            "-DCONFIG=$<CONFIG>"
                            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/prune_optional_root_runtime_dlls.cmake"
                    )
                endif()
            endif()

            set(EISENHOWER_BUILD_TREE_TOOLCHAIN_RUNTIME_COMMANDS)
            set(EISENHOWER_EXPORT_TOOLCHAIN_RUNTIME_COMMANDS)
            if(MSVC)
                list(
                    APPEND
                    EISENHOWER_BUILD_TREE_TOOLCHAIN_RUNTIME_COMMANDS
                    COMMAND "${CMAKE_COMMAND}"
                        "-DDEPLOY_ROOT=$<TARGET_FILE_DIR:EisenPo>"
                        "-DMSVC_REDIST_ROOT=${EISENHOWER_MSVC_REDIST_ROOT}"
                        "-DMSVC_RUNTIME_ARCH=${EISENHOWER_MSVC_RUNTIME_ARCH}"
                        "-DCONFIG=$<CONFIG>"
                        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_msvc_runtime.cmake"
                )
                if(EISENHOWER_EXPORT_DIR_NORMALIZED)
                    list(
                        APPEND
                        EISENHOWER_EXPORT_TOOLCHAIN_RUNTIME_COMMANDS
                        COMMAND "${CMAKE_COMMAND}"
                            "-DDEPLOY_ROOT=${EISENHOWER_EXPORT_DIR_NORMALIZED}"
                            "-DMSVC_REDIST_ROOT=${EISENHOWER_MSVC_REDIST_ROOT}"
                            "-DMSVC_RUNTIME_ARCH=${EISENHOWER_MSVC_RUNTIME_ARCH}"
                            "-DCONFIG=$<CONFIG>"
                            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_msvc_runtime.cmake"
                    )
                endif()
            elseif(MINGW)
                list(
                    APPEND
                    EISENHOWER_BUILD_TREE_TOOLCHAIN_RUNTIME_COMMANDS
                    COMMAND "${CMAKE_COMMAND}"
                        "-DDEPLOY_ROOT=$<TARGET_FILE_DIR:EisenPo>"
                        "-DMINGW_BIN_DIR=${EISENHOWER_MINGW_BIN_DIR}"
                        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_mingw_runtime.cmake"
                )
                if(EISENHOWER_EXPORT_DIR_NORMALIZED)
                    list(
                        APPEND
                        EISENHOWER_EXPORT_TOOLCHAIN_RUNTIME_COMMANDS
                        COMMAND "${CMAKE_COMMAND}"
                            "-DDEPLOY_ROOT=${EISENHOWER_EXPORT_DIR_NORMALIZED}"
                            "-DMINGW_BIN_DIR=${EISENHOWER_MINGW_BIN_DIR}"
                            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_mingw_runtime.cmake"
                    )
                endif()
            endif()

            set(EISENHOWER_WINDEPLOYQT_COMMON_FLAGS
                --no-opengl-sw
                --no-system-d3d-compiler
                --no-system-dxc-compiler
                --no-quick-import
                --no-svg
                --no-pdf
                --skip-plugin-types generic,iconengines
                --exclude-plugins qgif,qicns,qjpeg,qpdf,qsvg,qtga,qtiff,qwbmp,qwebp,qsqlibase,qsqlmimer,qsqloci,qsqlodbc,qsqlpsql,qopensslbackend
                ${EISENHOWER_WINDEPLOYQT_COMPILER_RUNTIME_ARGS}
                ${EISENHOWER_WINDEPLOYQT_TRANSLATION_ARGS}
                --qtpaths "${QTPATHS_EXECUTABLE}"
            )

            set(EISENHOWER_EXPORT_DEPLOY_COMMANDS)
            if(EISENHOWER_EXPORT_DIR_NORMALIZED)
                list(
                    APPEND
                    EISENHOWER_EXPORT_DEPLOY_COMMANDS
                    COMMAND "${CMAKE_COMMAND}" -E make_directory "${EISENHOWER_EXPORT_DIR_NORMALIZED}"
                    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                        "$<TARGET_FILE:EisenPo>"
                        "${EISENHOWER_EXPORT_DIR_NORMALIZED}/"
                    COMMAND "${CMAKE_COMMAND}" -E rm -rf
                        "${EISENHOWER_EXPORT_DIR_NORMALIZED}/plugins"
                        "${EISENHOWER_EXPORT_DIR_NORMALIZED}/qml"
                        "${EISENHOWER_EXPORT_DIR_NORMALIZED}/translations"
                        "${EISENHOWER_EXPORT_DIR_NORMALIZED}/language"
                        "${EISENHOWER_EXPORT_DIR_NORMALIZED}/qt.conf"
                    COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                        "${EISENHOWER_EXPORT_DIR_NORMALIZED}/$<TARGET_FILE_NAME:EisenPo>"
                        --dir "${EISENHOWER_EXPORT_DIR_NORMALIZED}"
                        --libdir "${EISENHOWER_EXPORT_DIR_NORMALIZED}"
                        --plugindir "${EISENHOWER_EXPORT_DIR_NORMALIZED}/plugins"
                        --translationdir "${EISENHOWER_EXPORT_DIR_NORMALIZED}/translations"
                        $<IF:$<CONFIG:Debug>,--debug,--release>
                        ${EISENHOWER_WINDEPLOYQT_COMMON_FLAGS}
                    COMMAND "${CMAKE_COMMAND}"
                        "-DDEPLOY_ROOT=${EISENHOWER_EXPORT_DIR_NORMALIZED}"
                        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/remove_compiler_runtime_installer.cmake"
                    ${EISENHOWER_EXPORT_TOOLCHAIN_RUNTIME_COMMANDS}
                    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                        "${CMAKE_CURRENT_BINARY_DIR}/generated/qt.conf"
                        "${EISENHOWER_EXPORT_DIR_NORMALIZED}/qt.conf"
                    ${EISENHOWER_EXPORT_ASSET_COPY_COMMANDS}
                    COMMAND "${CMAKE_COMMAND}"
                        "-DSOURCE_DIR=${EISENHOWER_TRANSLATIONS_DIR}"
                        "-DDEST_DIR=${EISENHOWER_EXPORT_DIR_NORMALIZED}/translations"
                        "-DCLEAN_DESTINATION=OFF"
                        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_qm_translations.cmake"
                    COMMAND "${CMAKE_COMMAND}"
                        "-DSOURCE_DIR=${EISENHOWER_POMODORO_SOUND_DIR}"
                        "-DDEST_DIR=${EISENHOWER_EXPORT_DIR_NORMALIZED}/Alarme/sound"
                        "-DCLEAN_DESTINATION=ON"
                        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_optional_directory.cmake"
                    ${EISENHOWER_EXPORT_OPTIONAL_ROOT_RUNTIME_PRUNE_COMMANDS}
                    COMMAND "${CMAKE_COMMAND}"
                        "-DDEPLOY_ROOT=${EISENHOWER_EXPORT_DIR_NORMALIZED}"
                        "-DCONFIG=$<CONFIG>"
                        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/prune_optional_qt_sqldrivers.cmake"
                )
            endif()

            add_custom_target(
                EisenPoDeploy
                ALL
                DEPENDS EisenPo
                COMMAND "${CMAKE_COMMAND}" -E rm -rf
                    "$<TARGET_FILE_DIR:EisenPo>/plugins"
                    "$<TARGET_FILE_DIR:EisenPo>/qml"
                    "$<TARGET_FILE_DIR:EisenPo>/translations"
                    "$<TARGET_FILE_DIR:EisenPo>/language"
                    "$<TARGET_FILE_DIR:EisenPo>/qt.conf"
                COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    "$<TARGET_FILE:EisenPo>"
                    --dir "$<TARGET_FILE_DIR:EisenPo>"
                    --libdir "$<TARGET_FILE_DIR:EisenPo>"
                    --plugindir "$<TARGET_FILE_DIR:EisenPo>/plugins"
                    --translationdir "$<TARGET_FILE_DIR:EisenPo>/translations"
                    $<IF:$<CONFIG:Debug>,--debug,--release>
                    ${EISENHOWER_WINDEPLOYQT_COMMON_FLAGS}
                COMMAND "${CMAKE_COMMAND}"
                    "-DDEPLOY_ROOT=$<TARGET_FILE_DIR:EisenPo>"
                    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/remove_compiler_runtime_installer.cmake"
                ${EISENHOWER_BUILD_TREE_TOOLCHAIN_RUNTIME_COMMANDS}
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_BINARY_DIR}/generated/qt.conf"
                    "$<TARGET_FILE_DIR:EisenPo>/qt.conf"
                ${EISENHOWER_BUILD_TREE_ASSET_COPY_COMMANDS}
                COMMAND "${CMAKE_COMMAND}"
                    "-DSOURCE_DIR=${EISENHOWER_TRANSLATIONS_DIR}"
                    "-DDEST_DIR=$<TARGET_FILE_DIR:EisenPo>/translations"
                    "-DCLEAN_DESTINATION=OFF"
                    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_qm_translations.cmake"
                COMMAND "${CMAKE_COMMAND}"
                    "-DSOURCE_DIR=${EISENHOWER_POMODORO_SOUND_DIR}"
                    "-DDEST_DIR=$<TARGET_FILE_DIR:EisenPo>/Alarme/sound"
                    "-DCLEAN_DESTINATION=ON"
                    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_optional_directory.cmake"
                ${EISENHOWER_BUILD_TREE_OPTIONAL_ROOT_RUNTIME_PRUNE_COMMANDS}
                COMMAND "${CMAKE_COMMAND}"
                    "-DDEPLOY_ROOT=$<TARGET_FILE_DIR:EisenPo>"
                    "-DCONFIG=$<CONFIG>"
                    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/prune_optional_qt_sqldrivers.cmake"
                ${EISENHOWER_EXPORT_DEPLOY_COMMANDS}
                COMMENT "Deploying EisenPo minimal runtime to build tree and export directory"
                VERBATIM
            )
            add_custom_target(
                EisenhowerOrganizerDeploy
                DEPENDS EisenPoDeploy
            )
        else()
            message(WARNING "windeployqt.exe or qtpaths6.exe was not found. The build tree executable may miss Qt runtime DLLs.")
        endif()
    endif()
endif()

