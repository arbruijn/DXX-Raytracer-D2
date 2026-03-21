cmake_minimum_required( VERSION 3.21 )

set( SDL_mixer ${CMAKE_CURRENT_SOURCE_DIR}/SDL )

file(
	GLOB SDL_MIXER_SOURCES
	${SDL_mixer}/[delmw]*.c
)

set( LIBSDL_MIXER_SOURCES ${SDL_MIXER_SOURCES} )

add_compile_definitions(PRIVATE WAV_MUSIC OGG_MUSIC MP3_MUSIC FLAC_MUSIC)

add_library( SDL_mixer_master ${LIBSDL_MIXER_SOURCES} )
target_include_directories( SDL_mixer_master PRIVATE
 "../sdl-master"
 "../sdl-master/sdl"
 "../sdl-master/sdl/include")
target_include_directories( SDL_mixer_master PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR} )

find_package(PkgConfig) 
if(PkgConfig_FOUND)
pkg_check_modules(OGG REQUIRED IMPORTED_TARGET ogg)
#pkg_check_modules(VORBIS REQUIRED IMPORTED_TARGET vorbis)
pkg_check_modules(VORBISFILE REQUIRED IMPORTED_TARGET vorbisfile)
pkg_check_modules(FLAC REQUIRED IMPORTED_TARGET flac)
pkg_check_modules(LIBMPG123 REQUIRED IMPORTED_TARGET libmpg123)
target_link_libraries(SDL_mixer_master PUBLIC
	PkgConfig::OGG
	#PkgConfig::VORBIS
	PkgConfig::VORBISFILE
	PkgConfig::FLAC
	PkgConfig::LIBMPG123)
else()
find_package(Ogg REQUIRED)
find_package(vorbis REQUIRED)
find_package(flac REQUIRED)
find_package(mpg123 REQUIRED)
target_link_libraries(SDL_mixer_master PUBLIC
 Ogg::ogg
 Vorbis::vorbisfile
 FLAC::FLAC
 MPG123::libmpg123)
endif()
