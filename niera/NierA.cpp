#include "NierA.h"

namespace NieR
{
	//Known-function pointers
	_ManageMeshVisibilities ManageMeshVisibilities = NULL;
	_SearchMeshGroupIndex SearchMeshGroupIndex = NULL;
	_SetDrawBasePlayerMeshes SetDrawBasePlayerMeshes = NULL;
	_UpdateAccessoryOnUnpause UpdateAccessoryOnUnpause = NULL;
	_UpdateEquippedActive UpdateEquippedActive = NULL;
	_AnotherAccessoryThing AnotherAccessoryThing = NULL;
	_ResolveNameFromItemID ResolveNameFromItemID = NULL;
	_GetAccessoryIDFromItemID GetAccessoryIDFromItemID = NULL;
	_GetOutfitIDFromItemID GetOutfitIDFromItemID = NULL;
	_ValidateAccessory ValidateAccessory = NULL;
	_ValidateDLCArmor ValidateDLCArmor = NULL;
	_ValidatePodSkins ValidatePodSkins = NULL;
	_ValidateNonCharacterSpecificEquippable ValidateNonCharacterSpecificEquippable = NULL;
	_SetOutfitFromPause SetOutfitFromPause = NULL;
	_SetEquippedFromPause SetEquippedFromPause = NULL;
	_lambda Lambda = NULL;

	//Unknown-function pointers
	_sub_52e9e0 sub_52e9e0 = NULL;
	_sub_7e6e60 sub_7e6e60 = NULL;
	_sub_745c50 sub_745c50 = NULL;
	_sub_45a850 sub_45a850 = NULL;
	_sub_28ed30 sub_28ed30 = NULL;
	_sub_150d80 sub_150d80 = NULL;
	_sub_495970 sub_495970 = NULL;
	_sub_150b70 sub_150b70 = NULL;
	_sub_491170 sub_491170 = NULL;
	_sub_491400 sub_491400 = NULL;
	_sub_7463c0 sub_7463c0 = NULL;
	_sub_3e6b70 sub_3e6b70 = NULL;
	_SetMeshToGroup SetMeshToGroup = NULL;
	_sub_745c10 sub_745c10 = NULL;
	_sub_45a8c0 sub_45a8c0 = NULL;
	_sub_3876a0 sub_3876a0 = NULL;
	_GetPlayerFromPlayerNum GetPlayerFromPlayerNum = NULL;
	_sub_7c4b50 sub_7c4b50 = NULL;
	_sub_7c4b90 sub_7c4b90 = NULL;
	_sub_7c9cb0 sub_7c9cb0 = NULL;

	//Saved original functions from hooks
	_ManageMeshVisibilities fpManageMeshVisiblities = NULL;
	_GetOutfitIDFromItemID fpGetOutfitIDFromItemID = NULL;
	_SetEquippedFromPause fpSetEquippedFromPause = NULL;
	_UpdateAccessoryOnUnpause fpUpdateAccessoryOnUnpause = NULL;
	_UpdateEquippedActive fpUpdateEquippedActive = NULL;
	_ValidateDLCArmor fpValidateDLCArmor = NULL;
	_ValidateNonCharacterSpecificEquippable fpValidateNonSpecificCharacterEquippable = NULL;
	_SetMeshToGroup fpSetMeshToGroup = NULL;
}
