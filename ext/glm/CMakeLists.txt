ivw_project(glm)

file(GLOB ROOT_SOURCE *.cpp)
file(GLOB ROOT_INLINE *.inl)
file(GLOB ROOT_HEADER *.hpp)

file(GLOB_RECURSE CORE_SOURCE ./core/*.cpp)
file(GLOB_RECURSE CORE_INLINE ./core/*.inl)
file(GLOB_RECURSE CORE_HEADER ./core/*.hpp)

file(GLOB_RECURSE GTC_SOURCE ./gtc/*.cpp)
file(GLOB_RECURSE GTC_INLINE ./gtc/*.inl)
file(GLOB_RECURSE GTC_HEADER ./gtc/*.hpp)

file(GLOB_RECURSE GTX_SOURCE ./gtx/*.cpp)
file(GLOB_RECURSE GTX_INLINE ./gtx/*.inl)
file(GLOB_RECURSE GTX_HEADER ./gtx/*.hpp)

file(GLOB_RECURSE VIRTREV_SOURCE ./virtrev/*.cpp)
file(GLOB_RECURSE VIRTREV_INLINE ./virtrev/*.inl)
file(GLOB_RECURSE VIRTREV_HEADER ./virtrev/*.hpp)

ivw_group("Core Files" ${CORE_SOURCE})
ivw_group("Core Files" ${CORE_INLINE})
ivw_group("Core Files" ${CORE_HEADER})
ivw_group("GTC Files" ${GTC_SOURCE})
ivw_group("GTC Files" ${GTC_INLINE})
ivw_group("GTC Files" ${GTC_HEADER})
ivw_group("GTX Files" ${GTX_SOURCE})
ivw_group("GTX Files" ${GTX_INLINE})
ivw_group("GTX Files" ${GTX_HEADER})
ivw_group("VIRTREV Files" ${VIRTREV_SOURCE})
ivw_group("VIRTREV Files" ${VIRTREV_INLINE})
ivw_group("VIRTREV Files" ${VIRTREV_HEADER})

ivw_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(glm)
if(BUILD_SHARED_LIBS)
    ivw_add_definition(GLM_SHARED_BUILD)
endif()

add_library(glm 
	${ROOT_SOURCE}    ${ROOT_INLINE}    ${ROOT_HEADER} 
	${CORE_SOURCE}    ${CORE_INLINE}    ${CORE_HEADER}
	${GTC_SOURCE}     ${GTC_INLINE}     ${GTC_HEADER} 
	${GTX_SOURCE}     ${GTX_INLINE}     ${GTX_HEADER} 
	${VIRTREV_SOURCE} ${VIRTREV_INLINE} ${VIRTREV_HEADER})
    
#--------------------------------------------------------------------
# Creates VS folder structure
ivw_vs_folder(glm ext)

#--------------------------------------------------------------------
# Make package (for other projects to find)
ivw_make_package(GLM glm)
