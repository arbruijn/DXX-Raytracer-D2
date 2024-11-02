cmake_minimum_required( VERSION 3.21 )

set( SDL_mixer ${CMAKE_CURRENT_SOURCE_DIR}/SDL )

file(
	GLOB SDL_MIXER_SOURCES
	${SDL_mixer}/[delmw]*.c
)

set( LIBSDL_MIXER_SOURCES ${SDL_MIXER_SOURCES} )

find_package(Ogg REQUIRED)
find_package(vorbis REQUIRED)
find_package(flac REQUIRED)
find_package(mpg123 REQUIRED)
add_compile_definitions(PRIVATE WAV_MUSIC OGG_MUSIC MP3_MUSIC FLAC_MUSIC)

add_library( SDL_mixer_master ${LIBSDL_MIXER_SOURCES} )
target_include_directories( SDL_mixer_master PRIVATE
 "../sdl-master"
 "../sdl-master/sdl"
 "../sdl-master/sdl/include")
target_include_directories( SDL_mixer_master PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(SDL_mixer_master PUBLIC
 Ogg::ogg
 Vorbis::vorbisfile
 FLAC::FLAC
 MPG123::libmpg123)

