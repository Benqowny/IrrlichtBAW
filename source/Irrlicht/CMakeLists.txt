cmake_minimum_required(VERSION 3.1)
project(Irrlicht)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11, but is it really needed?

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: Debug or Release" FORCE)
endif()

option(FAST_MATH "Enable fast low-precision math" ON)

if(NOT NASTY_OPENSSL_WORKAROUND)
	find_package(OpenSSL REQUIRED)
endif()

set(IRRLICHT_SRCS_COMMON
# Mesh loaders
	C3DSMeshFileLoader.cpp
	CB3DMeshFileLoader.cpp
	CBAWMeshFileLoader.cpp
	CLMTSMeshFileLoader.cpp
	CLWOMeshFileLoader.cpp
	CMS3DMeshFileLoader.cpp
	CMY3DMeshFileLoader.cpp
	COBJMeshFileLoader.cpp
	COCTLoader.cpp
	COgreMeshFileLoader.cpp
	CPLYMeshFileLoader.cpp
	CSTLMeshFileLoader.cpp
	CXMeshFileLoader.cpp

# Mesh writers
	CBAWMeshWriter.cpp
	COBJMeshWriter.cpp
	CPLYMeshWriter.cpp
	CSTLMeshWriter.cpp

# Other mesh-related stuff
	CAnimatedMeshSceneNode.cpp
	CBAWFile.cpp
	CBlobsLoadingManager.cpp
	CForsythVertexCacheOptimizer.cpp
	CMeshCache.cpp
	CMeshManipulator.cpp
	CMeshSceneNode.cpp
	CMeshSceneNodeInstanced.cpp
	COverdrawMeshOptimizer.cpp
	CSkinnedMesh.cpp
	CSkinnedMeshSceneNode.cpp
	TypedBlob.cpp

# Scene objects
	CBillboardSceneNode.cpp
	CCameraSceneNode.cpp
	CCubeSceneNode.cpp
	CGeometryCreator.cpp
	CSceneManager.cpp
	CSkyBoxSceneNode.cpp
	CSkyDomeSceneNode.cpp
	CSphereSceneNode.cpp

# Animators
	CSceneNodeAnimatorCameraFPS.cpp
	CSceneNodeAnimatorCameraMaya.cpp
	CSceneNodeAnimatorDelete.cpp
	CSceneNodeAnimatorFlyCircle.cpp
	CSceneNodeAnimatorFlyStraight.cpp
	CSceneNodeAnimatorFollowSpline.cpp
	CSceneNodeAnimatorRotation.cpp
	CSceneNodeAnimatorTexture.cpp

# Driver-specific objects
	CNullDriver.cpp
	COpenGLDriver.cpp
	COpenGLExtensionHandler.cpp
	COpenGLFrameBuffer.cpp
	COpenGLSLMaterialRenderer.cpp
	COpenGLTexture.cpp
	STextureSamplingParams.cpp
	FW_Mutex.cpp

# Image processing
	CColorConverter.cpp
	CImage.cpp
	CImageLoaderBMP.cpp
	CImageLoaderDDS.cpp
	CImageLoaderJPG.cpp
	CImageLoaderPNG.cpp
	CImageLoaderRGB.cpp
	CImageLoaderTGA.cpp
	CImageWriterBMP.cpp
	CImageWriterJPG.cpp
	CImageWriterPNG.cpp
	CImageWriterTGA.cpp

# Video
	CFPSCounter.cpp
	CVideoModeList.cpp

# Software renderer
	CBurningShader_Raster_Reference.cpp
	CDepthBuffer.cpp
	CSoftwareDriver2.cpp
	CSoftwareTexture2.cpp
	CTRGouraud2.cpp
	CTRTextureGouraud2.cpp
	CTRTextureGouraudNoZ2.cpp
	CTRTextureGouraudAdd2.cpp
	CTRTextureGouraudAddNoZ2.cpp
	CTRTextureGouraudVertexAlpha2.cpp
	IBurningShader.cpp

# Input/output
	CFileList.cpp
	CFileSystem.cpp
	CLimitReadFile.cpp
	CMemoryFile.cpp
	CReadFile.cpp
	CWriteFile.cpp
	CMountPointReader.cpp
	CNPKReader.cpp
	CPakReader.cpp
	CTarReader.cpp
	CWADReader.cpp
	CZipReader.cpp

# Other
	coreutil.cpp
	CIrrDeviceSDL.cpp
	CIrrDeviceLinux.cpp
	CIrrDeviceConsole.cpp
	CIrrDeviceStub.cpp
	CIrrDeviceWin32.cpp
	CLogger.cpp
	COSOperator.cpp
	Irrlicht.cpp
	os.cpp
)

set(IRRLICHT_SRCS_NORMAL
	CGPUTransientBuffer.cpp
	COpenCLHandler.cpp
	COpenGL2DTextureArray.cpp
	COpenGL2DTexture.cpp
	COpenGL3DTexture.cpp
	COpenGLCubemapTexture.cpp
	COpenGLMultisampleTextureArray.cpp
	COpenGLMultisampleTexture.cpp
	COpenGLQuery.cpp
	COpenGLTransformFeedback.cpp
	COpenGLVAOSpec.cpp
	CTRTextureGouraudAlpha.cpp
	CTRTextureGouraudAlphaNoZ.cpp
)

set(IRRLICHT_SRCS_SERVER
)

add_library(aesGladman OBJECT
	aesGladman/aes_ni.c
	aesGladman/aescrypt.c
	aesGladman/aeskey.c
	aesGladman/aestab.c
	aesGladman/fileenc.c
	aesGladman/hmac.c
	aesGladman/prng.c
	aesGladman/pwd2key.c
	aesGladman/sha1.c
)

add_library(bzip2 OBJECT
	bzip2/blocksort.c
	bzip2/bzlib.c
	bzip2/compress.c
	bzip2/crctable.c
	bzip2/decompress.c
	bzip2/huffman.c
	bzip2/randtable.c
)

add_library(convert_utf OBJECT
	convert_utf/ConvertUTF.c
)

add_library(jpeglib OBJECT
	jpeglib/jaricom.c
	jpeglib/jcapimin.c
	jpeglib/jcapistd.c
	jpeglib/jcarith.c
	jpeglib/jccoefct.c
	jpeglib/jccolor.c
	jpeglib/jcdctmgr.c
	jpeglib/jchuff.c
	jpeglib/jcinit.c
	jpeglib/jcmainct.c
	jpeglib/jcmarker.c
	jpeglib/jcmaster.c
	jpeglib/jcomapi.c
	jpeglib/jcparam.c
	jpeglib/jcprepct.c
	jpeglib/jcsample.c
	jpeglib/jctrans.c
	jpeglib/jdapimin.c
	jpeglib/jdapistd.c
	jpeglib/jdarith.c
	jpeglib/jdatadst.c
	jpeglib/jdatasrc.c
	jpeglib/jdcoefct.c
	jpeglib/jdcolor.c
	jpeglib/jddctmgr.c
	jpeglib/jdhuff.c
	jpeglib/jdinput.c
	jpeglib/jdmainct.c
	jpeglib/jdmarker.c
	jpeglib/jdmaster.c
	jpeglib/jdmerge.c
	jpeglib/jdpostct.c
	jpeglib/jdsample.c
	jpeglib/jdtrans.c
	jpeglib/jerror.c
	jpeglib/jfdctflt.c
	jpeglib/jfdctfst.c
	jpeglib/jfdctint.c
	jpeglib/jidctflt.c
	jpeglib/jidctfst.c
	jpeglib/jidctint.c
	jpeglib/jmemmgr.c
	jpeglib/jmemnobs.c
	jpeglib/jquant1.c
	jpeglib/jquant2.c
	jpeglib/jutils.c
)

add_library(libpng OBJECT
	libpng/png.c
	libpng/pngerror.c
	libpng/pngget.c
	libpng/pngmem.c
	libpng/pngpread.c
	libpng/pngread.c
	libpng/pngrio.c
	libpng/pngrtran.c
	libpng/pngrutil.c
	libpng/pngset.c
	libpng/pngtrans.c
	libpng/pngwio.c
	libpng/pngwrite.c
	libpng/pngwtran.c
	libpng/pngwutil.c
)

add_library(lz4 OBJECT
	lz4/lz4.c
	lz4/lz4frame.c
	lz4/lz4hc.c
	lz4/xxhash.c
)

add_library(lzma OBJECT
	lzma/Alloc.c
	lzma/LzFind.c
	lzma/LzmaDec.c
	lzma/LzmaEnc.c
)
target_compile_definitions(lzma PRIVATE _7ZIP_ST)

add_library(zlib OBJECT
	zlib/adler32.c
	zlib/crc32.c
	zlib/deflate.c
	zlib/infback.c
	zlib/inffast.c
	zlib/inflate.c
	zlib/inftrees.c
	zlib/trees.c
	zlib/uncompr.c
	zlib/zcompress.c
	zlib/zutil.c
)

add_definitions(
	-DPNG_THREAD_UNSAFE_OK
	-DPNG_NO_MMX_CODE
	-DPNG_NO_MNG_FEATURES
	-D_7ZIP_ST
)

if(WIN32)
	add_definitions(
		-DWIN32
		-D__GNUWIN32__
		-D_CRT_SECURE_NO_DEPRECATE
		-DNOMINMAX
	)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	add_compile_options(
		-fuse-ld=gold
	)
	link_libraries(
		-fuse-ld=gold
	)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	add_compile_options( # TODO: check with make VERBOSE=1 that it gets added to all target objects C++ and C
		-std=c++11 #add it to all C++ target objects but not C
		-Wall
		-Wextra
		-Wno-unused-parameter
		-fno-exceptions
		-fstrict-aliasing
		-msse3
		-mfpmath=sse
	)
	if($<CONFIG:Release>) # TODO: check with make VERBOSE=1 that it gets added to all target objects C++ and C
		add_compile_options(
			-O3
			-fexpensive-optimizations
			-fno-omit-frame-pointer
			-fstack-protector-strong
		)
	endif()
	if($<CONFIG:Debug>) # TODO: check with make VERBOSE=1 that it gets added to all target objects C++ and C
		add_compile_options(
			-fno-omit-frame-pointer
			-fstack-protector-strong
		)
	endif()
	if(FAST_MATH) # TODO: check with make VERBOSE=1 that it gets added to all target objects C++ and C
		add_compile_options(
			-ffast-math
		)
	endif()
else()
	message(WARNING "UNTESTED COMPILER DETECTED, EXPECT WRONG OPTIMIZATION FLAGS! SUBMIT ISSUE ON GITHUB https://github.com/buildaworldnet/IrrlichtBAW/issues")
endif()


if(NOT NASTY_OPENSSL_WORKAROUND)
	link_libraries(
		OpenSSL::Crypto
		OpenSSL::SSL
	)
else()
	include_directories(
		../../../openssl/include/
	)
endif()

include_directories(
	convert_utf
	libpng
	utf8/source
	zlib
)

if($<CONFIG:Debug>)
	add_definitions(-D_DEBUG) # TODO: check with make VERBOSE=1 that it gets added to all target objects C++ and C
endif()

if($<BOOL:FAST_MATH>)
	add_definitions(-DIRRLICHT_FAST_MATH)
endif()

add_library(Irrlicht STATIC
	${IRRLICHT_SRCS_COMMON}
	${IRRLICHT_SRCS_NORMAL}
	$<TARGET_OBJECTS:aesGladman>
	$<TARGET_OBJECTS:bzip2>
	$<TARGET_OBJECTS:convert_utf>
	$<TARGET_OBJECTS:jpeglib>
	$<TARGET_OBJECTS:libpng>
	$<TARGET_OBJECTS:lz4>
	$<TARGET_OBJECTS:lzma>
	$<TARGET_OBJECTS:zlib>
)

target_include_directories(Irrlicht PUBLIC ../../include)

target_compile_definitions(Irrlicht PRIVATE
	_IRR_STATIC_LIB_
	_IRR_COMPILE_WITH_OPENGL_
)

add_library(IrrlichtServer STATIC
	${IRRLICHT_SRCS_COMMON}
	${IRRLICHT_SRCS_SERVER}
	$<TARGET_OBJECTS:aesGladman>
	$<TARGET_OBJECTS:bzip2>
	$<TARGET_OBJECTS:convert_utf>
	$<TARGET_OBJECTS:jpeglib>
	$<TARGET_OBJECTS:libpng>
	$<TARGET_OBJECTS:lz4>
	$<TARGET_OBJECTS:lzma>
	$<TARGET_OBJECTS:zlib>
)

target_include_directories(IrrlichtServer PUBLIC ../../include)

target_compile_definitions(IrrlichtServer PRIVATE
	_IRR_STATIC_LIB_
	BAW_SERVER
	NO_IRR_COMPILE_WITH_BURNINGSVIDEO_
	NO_IRR_COMPILE_WITH_OPENGL_
)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	if(MSVC)
		set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../../lib/Win64-visualStudio" CACHE PATH "default install path" FORCE )
	elseif(WIN32)
		set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../../lib/Win64-gcc" CACHE PATH "default install path" FORCE )
	else()
		if (APPLE)
			set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../../lib/MacOSX" CACHE PATH "default install path" FORCE )
		elseif(UNIX)
			set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../../lib/Linux" CACHE PATH "default install path" FORCE )
		else()
			message(WARNING "NO INSTALL TARGET COULD BE GENERATED")
		endif()
	endif()
endif()

install(TARGETS Irrlicht DESTINATION .)
install(TARGETS IrrlichtServer DESTINATION .)


if(NOT CONFIGURED_ONCE)
	# Add some optional flags to be used by default
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb3"
			CACHE STRING "Flags used by the C compiler during debug build." FORCE)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3"
			CACHE STRING "Flags used by the C++ compiler during debug build." FORCE)
	endif()

	# This is to add flags once only
	set(CONFIGURED_ONCE TRUE
		CACHE INTERNAL  "A flag showing that CMake has configured at least once.")
endif()
