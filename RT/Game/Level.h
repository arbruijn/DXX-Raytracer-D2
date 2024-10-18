#ifndef _RT_LEVEL_H
#define _RT_LEVEL_H

#include "segment.h"

#include "ApiTypes.h"

typedef struct RT_Light RT_Light;
void RT_FindAndSubmitNearbyLights(RT_Vec3 player_pos);

bool RT_LoadLevel();
void RT_RenderLevel(RT_Vec3 player_pos);
bool RT_UnloadLevel();

RT_ResourceHandle RT_UploadLevelGeometry(void);

void RT_BlowUpLight(side* sd);

#endif //_RT_LEVEL_H