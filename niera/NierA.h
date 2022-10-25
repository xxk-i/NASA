#pragma once
#include <Windows.h>

namespace NieR
{
	struct PlayerModelInfo {
		BYTE gap0[0x398];
		void* pWMB;
		void* mesh_groups;
		DWORD MaxMeshGroupIndex;
		BYTE gap3AC[524];
		DWORD currentPlayer;
		BYTE gap5BC[65644];
		DWORD dword10628;
		BYTE gap1062C[26292];
		DWORD dword16CE0;
		DWORD dwordShowPants;
		BYTE gap16CE8[4];
		DWORD dword16CEC;
		BYTE gap16CF0[4];
		DWORD isBuddy;
		BYTE gap16CF8[908];
		const unsigned int unsigned_int17084;
		BYTE gap17088[48];
		DWORD dword170B8;
		DWORD accessoryEquipped;
		unsigned int unsigned_int170C0;
		DWORD outfitEquipped;
		BYTE gap170C8[1808];
		DWORD dword177D8;
		BYTE gap177DC[256];
		DWORD dword178DC;
		DWORD dword178E0;
	};

	//Known functions
	typedef __int64(__fastcall* _ManageMeshVisibilities)(__int64 pPlayerModelInfo);

	typedef int(__fastcall* _SearchMeshGroupIndex)(void* pWMB, const char* mesh_name);

	typedef __int64(__fastcall* _SetDrawBasePlayerMeshes)(void* a1, int a2);

	typedef __int64(__fastcall* _UpdateAccessoryOnUnpause)(PlayerModelInfo* pPlayerModelInfo);

	typedef bool(__fastcall* _UpdateEquippedActive)(__int64, __int64, int);

	typedef void(__fastcall* _AnotherAccessoryThing)(PlayerModelInfo*, int);

	typedef char* (__fastcall* _ResolveNameFromItemID)(__int64, __int64);

	typedef int(__fastcall* _GetAccessoryIDFromItemID)(__int64, int);

	typedef int(__fastcall* _GetOutfitIDFromItemID)(__int64, int);

	typedef bool(__fastcall* _ValidateAccessory)(__int64, __int64, int);

	typedef bool(__fastcall* _ValidateDLCArmor)(__int64, __int64, int);

	typedef bool(__fastcall* _ValidatePodSkins)(__int64, int);

	typedef bool(__fastcall* _ValidateNonCharacterSpecificEquippable)(__int64, int);

	typedef void(__fastcall* _SetOutfitFromPause)(PlayerModelInfo*, int, int);

	typedef __int64(__fastcall* _SetEquippedFromPause)(__int64, int);


	//Unknown functions
	typedef __int64(__fastcall* _sub_745c50)(DWORD*);

	typedef __int64(__fastcall* _sub_45a850)(__int64);

	typedef __int64(__fastcall* _sub_28ed30)(void*, int);

	typedef __int64(__fastcall* _sub_150d80)(void*, void*, void*, int);

	typedef bool(__fastcall* _sub_495970)(void*, unsigned int);

	typedef void(__fastcall* _sub_150b70)(void*);

	typedef __int64(__fastcall* _sub_491170)(int*);

	typedef __int64(__fastcall* _sub_491400)(int*);

	typedef __int64(__fastcall* _sub_7463c0)(__int64);

	typedef void(__fastcall* _sub_3e6b70)(__int64);

	typedef __int64(__fastcall* _sub_7459b0)(__int64, const char*, int, __int64);

	typedef __int64(__fastcall* _sub_45a8c0)(__int64);

	typedef __int64(__fastcall* _sub_3876a0)(__int64*);

	typedef bool(__fastcall* _sub_7e6e60)(__int64, int, __int64);

	typedef __int64(__fastcall* _GetPlayerFromPlayerNum)(__int64);

	typedef __int64(__fastcall* _sub_7c4b50)();

	typedef int(__fastcall* _sub_7c4b90)();

	typedef __int64(__fastcall* _sub_7c9cb0)(__int64, int, int);

	typedef void(__fastcall* _sub_52e9e0)(__int64, int);

	//Known-function pointers
	extern _ManageMeshVisibilities ManageMeshVisibilities;
	extern _SearchMeshGroupIndex SearchMeshGroupIndex;
	extern _SetDrawBasePlayerMeshes setDrawBasePlayerMeshes;
	extern _UpdateAccessoryOnUnpause updateAccessoryOnUnpause;
	extern _UpdateEquippedActive UpdateEquippedActive;
	extern _AnotherAccessoryThing AnotherAccessoryThing;
	extern _ResolveNameFromItemID ResolveNameFromItemID;
	extern _GetAccessoryIDFromItemID GetAccessoryIDFromItemID;
	extern _GetOutfitIDFromItemID GetOutfitIDFromItemID;
	extern _ValidateAccessory ValidateAccessory;
	extern _ValidateDLCArmor ValidateDLCArmor;
	extern _ValidatePodSkins ValidatePodSkins;
	extern _ValidateNonCharacterSpecificEquippable ValidateNonCharacterSpecificEquippable;
	extern _SetOutfitFromPause SetOutfitFromPause;
	extern _SetEquippedFromPause SetEquippedFromPause;

	//Unknown-function pointers
	extern _sub_52e9e0 sub_52e9e0;
	extern _sub_7e6e60 sub_7e6e60;
	extern _sub_745c50 sub_745c50;
	extern _sub_45a850 sub_45a850;
	extern _sub_28ed30 sub_28ed30;
	extern _sub_150d80 sub_150d80;
	extern _sub_495970 sub_495970;
	extern _sub_150b70 sub_150b70;
	extern _sub_491170 sub_491170;
	extern _sub_491400 sub_491400;
	extern _sub_7463c0 sub_7463c0;
	extern _sub_3e6b70 sub_3e6b70;
	extern _sub_7459b0 sub_7459b0;
	extern _sub_45a8c0 sub_45a8c0;
	extern _sub_3876a0 sub_3876a0;
	extern _GetPlayerFromPlayerNum GetPlayerFromPlayerNum;
	extern _sub_7c4b50 sub_7c4b50;
	extern _sub_7c4b90 sub_7c4b90;
	extern _sub_7c9cb0 sub_7c9cb0;

	//Saved original functions from hooks
	extern _ManageMeshVisibilities fpManageMeshVisiblities;
	extern _GetOutfitIDFromItemID fpGetOutfitIDFromItemID;
	extern _SetEquippedFromPause fpSetEquippedFromPause;
	extern _UpdateAccessoryOnUnpause fpUpdateAccessoryOnUnpause;
	extern _UpdateEquippedActive fpUpdateEquippedActive;
	extern _ValidateDLCArmor fpValidateDLCArmor;
	extern _ValidateNonCharacterSpecificEquippable fpValidateNonSpecificCharacterEquippable;

	extern __int64* lambda_meme(__int64* a1, __int64* a2);
}
