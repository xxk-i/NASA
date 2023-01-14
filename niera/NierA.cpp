#include "NierA.h"

// NOTE: (Martin) Normalized all function typedef names 
namespace NieR
{
	//Known-function pointers
	FnManageMeshVisibilities ManageMeshVisibilities = NULL;
	FnSearchMeshGroupIndex SearchMeshGroupIndex = NULL;
	FnSetDrawBasePlayerMeshes SetDrawBasePlayerMeshes = NULL;
	FnUpdateAccessoryOnUnpause UpdateAccessoryOnUnpause = NULL;
	FnUpdateEquippedActive UpdateEquippedActive = NULL;
	FnAnotherAccessoryThing AnotherAccessoryThing = NULL;
	FnResolveNameFromItemID ResolveNameFromItemID = NULL;
	FnGetAccessoryIDFromItemID GetAccessoryIDFromItemID = NULL;
	FnGetOutfitIDFromItemID GetOutfitIDFromItemID = NULL;
	FnValidateAccessory ValidateAccessory = NULL;
	FnValidateDLCArmor ValidateDLCArmor = NULL;
	FnValidatePodSkins ValidatePodSkins = NULL;
	FnValidateNonCharacterSpecificEquippable ValidateNonCharacterSpecificEquippable = NULL;
	FnSetOutfitFromPause SetOutfitFromPause = NULL;
	FnSetEquippedFromPause SetEquippedFromPause = NULL;
	FnLambda Lambda = NULL;

	//Unknown-function pointers
	// X - RVA from imagebase
	// sub_X
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
	FnSetMeshToGroup SetMeshToGroup = NULL;
	_sub_745c10 sub_745c10 = NULL;
	_sub_45a8c0 sub_45a8c0 = NULL;
	_sub_3876a0 sub_3876a0 = NULL;
	FnGetPlayerFromPlayerNum GetPlayerFromPlayerNum = NULL;
	_sub_7c4b50 sub_7c4b50 = NULL;
	_sub_7c4b90 sub_7c4b90 = NULL;
	_sub_7c9cb0 sub_7c9cb0 = NULL;

	//Saved original functions from hooks
	FnManageMeshVisibilities ManageMeshVisiblities = NULL;
	FnGetOutfitIDFromItemID GetOutfitIDFromItemID = NULL;
	FnSetEquippedFromPause SetEquippedFromPause = NULL;
	FnUpdateAccessoryOnUnpause UpdateAccessoryOnUnpause = NULL;
	FnUpdateEquippedActive UpdateEquippedActive = NULL;
	FnValidateDLCArmor ValidateDLCArmor = NULL;
	FnValidateNonCharacterSpecificEquippable ValidateNonSpecificCharacterEquippable = NULL;
	FnSetMeshToGroup SetMeshToGroup = NULL;
}
