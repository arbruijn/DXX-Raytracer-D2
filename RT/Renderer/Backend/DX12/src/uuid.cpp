#include <stdlib.h>
#include <windows.h>
#include <shobjidl.h>

//#include "dxcapi.h"

//#define UUIDOF(interface, spec) struct __declspec(uuid(spec)) interface;

#ifndef MSC_VER
#define UUID_DECL(type,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
    static const IID uuid__##type = {l,w1,w2, {b1,b2,b3,b4,b5,b6,b7,b8}}; \
    template<> const _GUID &__mingw_uuidof() { return uuid__##type; }
#endif

struct IDxcUtils;
struct IDxcCompiler;

__CRT_UUID_DECL(IDxcUtils, 0x4605C4CB, 0x2019, 0x492A, 0xAD, 0xA4, 0x65, 0xF2, 0x0B, 0xB7, 0xD6, 0x7F)
__CRT_UUID_DECL(IDxcCompiler, 0x8c210bf3, 0x011f, 0x4422, 0x8d, 0x70, 0x6f, 0x9a, 0xcb, 0x8d, 0xb6, 0x17)
