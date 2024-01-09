#ifndef _NASA_DLL_MAIN_H_
#define _NASA_DLL_MAIN_H_

#include "niera/NierA.h"

// Hooks
__int64 hkSetAccessory(__int64 a1, const char* meshGroup, int accessoryFileID, __int64 pUnk); // Not currently hooked

__int64 hkGetOutfitIDFromItemID(__int64 item_base, int item_ID); // Not currently hooked

__int64 hkUpdateEquippedActive(__int64 a1, __int64 item_id, int currentPlayer);

bool hkValidateAccessory(__int64 item_base, __int64 item_id, int currentPlayer); // Not yet implemented

__int64 hkValidateDLCArmor(__int64 item_base, __int64 item_id, int currentPlayer);

__int64 hkValidateNonSpecificCharacterEquippable(__int64 item_base, int item_id);

__int64 hkUpdateAccessoryOnUnpause(NieR::PlayerModelInfo* pPlayerModelInfo); // Not currently hooked

__int64 hkSetEquippedFromPause(__int64 a1, int item_id);

__int64 __fastcall HkManageMeshVisibilites(NieR::PlayerModelInfo* pPlayerModelInfo);

// Mesh helpers
void SetMeshInvisible(NieR::PlayerModelInfo* pPlayerModelInfo, const char* mesh_name);

void SetMeshVisible(NieR::PlayerModelInfo* pPlayerModelInfo, const char* mesh_name);

// Other helpers
__int64* lambda_meme(__int64* a1, int* a2);

void InitializeFunctionPointers();

void ConsoleSetup();

#endif