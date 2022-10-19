// dllmain.cpp : Defines the entry point for the DLL application.
#include "minhook/include/MinHook.h"
#include <iostream>

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
    DWORD dword16CE4;
    BYTE gap16CE8[4];
    DWORD dword16CEC;
    BYTE gap16CF0[4];
    DWORD dword16CF4;
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

//Funciton definition
typedef __int64(__fastcall * _ManageMeshVisibilities)(__int64 pPlayerModelInfo);

typedef int(__fastcall * _SearchMeshGroupIndex)(void* pWMB, const char* mesh_name);

typedef __int64(__fastcall* _SetDrawBasePlayerMeshes)(void* a1, int a2);

typedef __int64(__fastcall* _UpdateAccessoryOnUnpause)(PlayerModelInfo* pPlayerModelInfo);

typedef bool(__fastcall* _UpdateEquippedActive)(__int64, __int64, int);

typedef void(__fastcall* _AnotherAccessoryThing)(PlayerModelInfo*, int);

typedef char*(__fastcall* _ResolveNameFromItemID)(__int64, __int64);

typedef int(__fastcall* _GetAccessoryIDFromItemID)(__int64, int);

typedef int(__fastcall* _GetOutfitIDFromItemID)(__int64, int);

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

typedef __int64(__fastcall* _sub_3084c0)(__int64);

typedef __int64(__fastcall* _sub_7c4b50)();

typedef int(__fastcall* _sub_7c4b90)();

typedef __int64(__fastcall* _sub_7c9cb0)(__int64, int, int);

typedef bool(__fastcall* _ValidateAccessory)(__int64, __int64, int);

typedef bool(__fastcall* _ValidateDLCArmor)(__int64, __int64, int);

typedef bool(__fastcall* _ValidatePodSkins)(__int64, int);

typedef bool(__fastcall* _ValidateNonCharacterSpecificEquippable)(__int64, int);

typedef void(__fastcall* _SetOutfitFromPause)(PlayerModelInfo*, int, int);

typedef __int64(__fastcall* _SetEquippedFromPause)(__int64, int);

typedef void(__fastcall* _sub_52e9e0)(__int64, int);

//Pointer to original
_ManageMeshVisibilities fpManageMeshVisiblities = NULL;

_SearchMeshGroupIndex SearchMeshGroupIndex = NULL;

_SetDrawBasePlayerMeshes setDrawBasePlayerMeshes = NULL;

_UpdateAccessoryOnUnpause updateAccessoryOnUnpause = NULL;

_UpdateEquippedActive UpdateEquippedActive = NULL;

_AnotherAccessoryThing AnotherAccessoryThing = NULL;

_ResolveNameFromItemID ResolveNameFromItemID = NULL;

_GetAccessoryIDFromItemID GetAccessoryIDFromItemID = NULL;

_GetOutfitIDFromItemID GetOutfitIDFromItemID = NULL;

_GetOutfitIDFromItemID fpGetOutfitIDFromItemID = NULL;

_ValidateAccessory ValidateAccessory = NULL;

_ValidateDLCArmor ValidateDLCArmor = NULL;

_ValidatePodSkins ValidatePodSkins = NULL;

_ValidateNonCharacterSpecificEquippable ValidateNonCharacterSpecificEquippable = NULL;

_SetOutfitFromPause SetOutfitFromPause = NULL;

_SetEquippedFromPause SetEquippedFromPause = NULL;

_SetEquippedFromPause fpSetEquippedFromPause = NULL;

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

_sub_7459b0 sub_7459b0 = NULL;

_sub_45a8c0 sub_45a8c0 = NULL;

_sub_3876a0 sub_3876a0 = NULL;

_sub_3084c0 sub_3084c0 = NULL;

_sub_7c4b50 sub_7c4b50 = NULL;

_sub_7c4b90 sub_7c4b90 = NULL;

_sub_7c9cb0 sub_7c9cb0 = NULL;

uintptr_t modBase = NULL;

FILE* stream;

__int64* lambda_meme(__int64* a1, __int64* a2)
{
	*(__int64*)a1 = *a2;
	return a1;
}

//TODO maybe look at this? try to avoid resolving name?
__int64 hkGetOutfitIDFromItemID(__int64 item_base, int item_ID)
{
	//Original function does some goofy shit IDK my way or the highway bitch
	/*
	switch (item_ID)
	{
	case 81:	//2P
		return 5;
	case 82:	//2B Kimono
		return 6;
	case 85:	//P2
		return 7;
	case 86:	//A2 Kimono
		return 8;
	}
	*/

	return fpGetOutfitIDFromItemID(item_base, item_ID);
}

bool thinghkUpdateEquippedActive(__int64 a1, __int64 item_id, int currentPlayer)
{
	const char* resolvedName = "";
	// Remember we hook this validate function, so this calls the original, which actually jumps to our hook
	if (!ValidateNonCharacterSpecificEquippable(modBase + 0x133b510, item_id) || currentPlayer > 2 || item_id == -1)
	{
		return 0;
	}

	if (currentPlayer)
	{
		const char* v7 = ResolveNameFromItemID(modBase + 0x133b510, item_id);
		const char* v8;
		if (currentPlayer == 1)
		{
			v8 = "item_uq_dlcCloth2";
		}

		else 
		{
			v8 = "item_uq_dlcCloth3";
		}

		if (strcmp(v7, v8))
		{
			return 0;
		}

		if (sub_7c4b50() != 0)
		{
			//Same reminder, we are hooking GetOutfitIDFromItemID
			//If this returns true, the outfit shows as ACTIVE
			return GetOutfitIDFromItemID(modBase + 0x133b510, item_id) == *(int*)((modBase + 0x1494354) + (sizeof(int) * currentPlayer));
		}

		return 0;
	}

	const char* v5 = ResolveNameFromItemID(modBase + 0x133b510, item_id);
	if (strcmp(v5, "item_uq_changeArmour1"))
	{
		const char* v6 = ResolveNameFromItemID(modBase + 0x133b510, item_id);
		if (strcmp(v5, "item_uq_changeArmour1"))
		{
			const char* v7 = ResolveNameFromItemID(modBase + 0x133b510, item_id);
			if (strcmp(v7, "item_uq_dlcCloth1"))
			{
				return 0;
			}
			if (sub_7c4b50())
			{
				return GetOutfitIDFromItemID(modBase + 0x133b510, item_id) == *(int*)((modBase + 0x1494354) + (sizeof(int) * currentPlayer));
			}
		}
	}

	if (sub_7c4b50())
	{
		return GetOutfitIDFromItemID(modBase + 0x133b510, item_id) == *(int*)((modBase + 0x1494354) + (sizeof(int) * currentPlayer));
	}

	return 0;
}

/*
Function optimized and decompiled odd, but basically only reaches the end if
a strcmp doesnt match (strcmp returns 0) and returns if the outfit is active

This is my refactored version
*/
bool hkUpdateEquippedActive(__int64 a1, __int64 item_id, int currentPlayer)
{
	std::cout << "ItemID: " << item_id << " ResolvedName: " << ResolveNameFromItemID(modBase + 0x133b510, item_id) << std::endl;

	const char* resolvedName = "";
	// Remember we hook this validate function, so this calls the original, which actually jumps to our hook
	if (!ValidateNonCharacterSpecificEquippable(modBase + 0x133b510, item_id) || currentPlayer > 2 || item_id == -1)
	{
		return 0;
	}
	
	//2B
	if (currentPlayer == 0)
	{
		resolvedName = ResolveNameFromItemID(modBase + 0x133b510, item_id);
		if (strcmp(resolvedName, "item_uq_changeArmour1") &&
			strcmp(resolvedName, "item_uq_changeArmour2") &&
			strcmp(resolvedName, "item_uq_dlcCloth1") &&
			strcmp(resolvedName, "item_uq_dlcOutfit1") &&
			strcmp(resolvedName, "item_uq_dlcOutfit2"))
		{
			return 0;
		}

		return false;
	}

	//9S
	else if (currentPlayer == 1)
	{
		resolvedName = ResolveNameFromItemID(modBase + 0x133b510, item_id);
		if (strcmp(resolvedName, "item_uq_dlcCloth2") &&
			strcmp(resolvedName, "item_uq_dlcOutfit3") &&
			strcmp(resolvedName, "item_uq_dlcOutfit4"))
		{
			return false;
		}
	}

	//A2
	else if (currentPlayer == 2)
	{
		resolvedName = ResolveNameFromItemID(modBase + 0x133b510, item_id);
		if (strcmp(resolvedName, "item_uq_dlcCloth3") &&
			strcmp(resolvedName, "item_uq_dlcOutfit5") &&
			strcmp(resolvedName, "item_uq_dlcOutfit6"))
		{
			return false;
		}
	}

	if (sub_7c4b50() != 0)
	{
		//Same reminder, we are hooking GetOutfitIDFromItemID
		//If this returns true, the outfit shows as ACTIVE
		return GetOutfitIDFromItemID(modBase + 0x133b510, item_id) == *(int*)((modBase + 0x1494354) + (sizeof(int) * currentPlayer));
	}

	return false;
}

//TODO: DO THIS FOR ACCESSORIES
bool hkValidateAccessory(__int64 item_base, __int64 item_id, int currentPlayer)
{
	return true;
}

bool oldhkValidateDLCArmor(__int64 item_base, __int64 item_id, int currentPlayer)
{
	const char* resolved_name = "";
	const char* expected_name = "";
	if (item_id == -1 || currentPlayer == -1)
	{
		return 0;
	}

	// if currentPlayer > 1 (A2 or 9S)
	if (currentPlayer)
	{
		// 9S
		if (currentPlayer == 1)
		{	
			// 9P (White 9S)
			resolved_name = ResolveNameFromItemID(item_base, item_id);
			if (!strcmp(resolved_name, "item_uq_dlcOutfit3"))
			{
				return 1;
			}

			// Kimono (9S)
			resolved_name = ResolveNameFromItemID(item_base, item_id);
			if (!strcmp(resolved_name, "item_uq_dlcOutfit4"))
			{
				return 1;
			}

			// data100 DLC outfit
			resolved_name = ResolveNameFromItemID(item_base, item_id);
			if (!strcmp(resolved_name, "item_uq_dlcCloth2"))
			{
				return 1;
			}
		}

		// A2
		else
		{
			// Unknown current player value
			if (currentPlayer != 2)
			{
				return 0;
			}
			// P2 (White A2)
			resolved_name = ResolveNameFromItemID(item_base, item_id);
			if (!strcmp(resolved_name, "item_uq_dlcOutfit5"))
			{
				return 1;
			}

			// Kimono (A2)
			resolved_name = ResolveNameFromItemID(item_base, item_id);
			if (!strcmp(resolved_name, "item_uq_dlcOutfit6"))
			{
				return 1;
			}

			// data100 DLC Outfit
			resolved_name = ResolveNameFromItemID(item_base, item_id);
			if (!strcmp(resolved_name, "item_uq_dlcCloth3"))
			{
				return 1;
			}
		}
	}

	// if currentPlayer == 0 (2B)
	else
	{
		// Full Armor
		resolved_name = ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_changeArmour1"))
		{
			return 1;
		}

		// Armor (so no head)
		resolved_name = ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_changeArmour2"))
		{
			return 1;
		}

		// 2P (White 2B)
		resolved_name = ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit1"))
		{
			return 1;
		}

		// Kimono (2B)
		resolved_name = ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit2"))
		{
			return 1;
		}

		// data100 DLC outfit
		resolved_name = ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcCloth1"))
		{
			return 1;
		}
	}

	return 0;
}

bool hkValidateDLCArmor(__int64 item_base, __int64 item_id, int currentPlayer)
{
	const char* resolved_name = "";
	const char* expected_name = "";

	if (item_id == -1 || currentPlayer <= -1 || currentPlayer > 2)
		return 0i64;

	//2B
	if (currentPlayer == 0)
	{
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_changeArmour1"))
			return 1i64;
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_changeArmour2"))
			return 1i64;
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit1"))
			return 1i64;
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit2"))
			return 1i64;
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		expected_name = "item_uq_dlcCloth1";
	}

	//9S
	else if (currentPlayer == 1)
	{
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit3"))	//9P
			return 1i64;
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit4"))	//Kimono
			return 1i64;
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		expected_name = "item_uq_dlcCloth2";
	}

	//A2
	else if (currentPlayer == 2)
	{
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit5"))	//P2
			return 1i64;
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit6"))	//Kimono
			return 1i64;
		resolved_name = (const char*)ResolveNameFromItemID(item_base, item_id);
		expected_name = "item_uq_dlcCloth3";
	}

	return !strcmp(resolved_name, expected_name);
}

//If this returns 1, the "Use" button will instead show "Equip"
//NOTE: The outfit will still have to pass the "ValidateOutfit" to be clickable in the menu!
bool hkValidateNonSpecificCharacterEquippable(__int64 item_base, __int64 item_id)
{
	if (item_id != -1)
	{
		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_changeArmour1"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_changeArmour2"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_changeArmour2"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_dlcCloth1"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_dlcCloth2"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_dlcCloth3"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit1"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit2"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit3"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit4"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit5"))
			return 1;

		if (!strcmp(ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit6"))
			return 1;
	}

	return 0;
}

void set_mesh_invisible(PlayerModelInfo* pPlayerModelInfo, const char* mesh_name)
{
    if (pPlayerModelInfo->pWMB)
    {
        __int64 mesh_group_index = SearchMeshGroupIndex(pPlayerModelInfo->pWMB, mesh_name);
        if (mesh_group_index >= 0 && (int)mesh_group_index < pPlayerModelInfo->MaxMeshGroupIndex)
        {
            __int64 mesh_group = (__int64)pPlayerModelInfo->mesh_groups + 0x70 * (int)mesh_group_index;
			if (mesh_group)
			{
				*(DWORD*)(mesh_group + 100) = 0;
			}
        }
    }
}

void set_mesh_visible(PlayerModelInfo* pPlayerModelInfo, const char* mesh_name)
{
    if (pPlayerModelInfo->pWMB)
    {
        __int64 mesh_group_index = SearchMeshGroupIndex(pPlayerModelInfo->pWMB, mesh_name);
        if (mesh_group_index >= 0 && (int)mesh_group_index < pPlayerModelInfo->MaxMeshGroupIndex)
        {
            __int64 mesh_group = (__int64)pPlayerModelInfo->mesh_groups + 0x70 * (int)mesh_group_index;
			if (mesh_group)
			{
				*(DWORD*)(mesh_group + 100) = 1;
			}
        }
    }
}

__int64 hkSetEquippedFromPause(__int64 a1, int item_id)
{
	if (!a1)
	{
		return 0;
	}

	//2P's Body Replica (White)
	if (item_id == 81)
	{
		DWORD playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = sub_3084c0(sub_745c50(&playernum));
		PlayerModelInfo* v9 = (PlayerModelInfo*)v7;
		if (v7)
		{
			SetOutfitFromPause(v9, 5, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//Yorha Uniform 1 (Kimono)
	if (item_id == 82)
	{
		DWORD playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = sub_3084c0(sub_745c50(&playernum));
		PlayerModelInfo* v9 = (PlayerModelInfo*)v7;
		if (v7)
		{
			SetOutfitFromPause(v9, 4, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//P2's Body Replica
	if (item_id == 85)
	{
		DWORD playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = sub_3084c0(sub_745c50(&playernum));
		PlayerModelInfo* v9 = (PlayerModelInfo*)v7;
		if (v7)
		{
			SetOutfitFromPause(v9, 3, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//YoRHA Uniform Prototype (NS_A2)
	if (item_id == 86)
	{
		DWORD playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = sub_3084c0(sub_745c50(&playernum));
		PlayerModelInfo* v9 = (PlayerModelInfo*)v7;
		if (v7)
		{
			SetOutfitFromPause(v9, 2, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	else
	{
		return fpSetEquippedFromPause(a1, item_id);
	}
}

//Hook
__int64 __fastcall HkManageMeshVisibilites(PlayerModelInfo* pPlayerModelInfo)
{
	const char* v15 = "";

	//2B
	if (pPlayerModelInfo->currentPlayer == 0x10000)
	{
		setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 1);
		__int64 thing = (__int64)pPlayerModelInfo;
		if (pPlayerModelInfo->outfitEquipped == 1) //kaine outfit
		{
			set_mesh_invisible(pPlayerModelInfo, "Armor_Body");
			set_mesh_invisible(pPlayerModelInfo, "Armor_Head");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Feather");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash_serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "Broken");
			set_mesh_invisible(pPlayerModelInfo, "Body");
			set_mesh_invisible(pPlayerModelInfo, "Feather");
			set_mesh_invisible(pPlayerModelInfo, "Skirt");
		}

		else if (pPlayerModelInfo->outfitEquipped == 2) //full armor
		{
			setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 0);
			set_mesh_visible(pPlayerModelInfo, "Armor_Body");
			set_mesh_visible(pPlayerModelInfo, "Armor_Head");
		}

		else if (pPlayerModelInfo->outfitEquipped == 3)	//just armor body
		{
			set_mesh_invisible(pPlayerModelInfo, "Armor_Head");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Broken");
			set_mesh_invisible(pPlayerModelInfo, "Body");
			set_mesh_invisible(pPlayerModelInfo, "Feather");
			set_mesh_invisible(pPlayerModelInfo, "Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Feather");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash_serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Skirt");
			v15 = "Broken";
		}

		else if (pPlayerModelInfo->outfitEquipped == 4)	//Kimono
		{
			setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 0);
			set_mesh_visible(pPlayerModelInfo, "Hair");
			set_mesh_visible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_visible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_visible(pPlayerModelInfo, "NS_KIMONO_Chest");
			set_mesh_visible(pPlayerModelInfo, "NS_KIMONO_Normal");
			set_mesh_visible(pPlayerModelInfo, "NS_KIMONO_Skirt");
		}

		else if (pPlayerModelInfo->outfitEquipped == 5) //2P
		{
			setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 0);
			set_mesh_visible(pPlayerModelInfo, "Hair");
			set_mesh_visible(pPlayerModelInfo, "NS_2P_Eyemask");
			set_mesh_visible(pPlayerModelInfo, "NS_2P_Normal");
			set_mesh_visible(pPlayerModelInfo, "NS_2P_Feather");
			set_mesh_visible(pPlayerModelInfo, "NS_2P_Body");
			set_mesh_visible(pPlayerModelInfo, "NS_2P_Skirt");
		}

		else  // normal
		{
			set_mesh_invisible(pPlayerModelInfo, "Armor_Body");
			set_mesh_invisible(pPlayerModelInfo, "Armor_Head");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Feather");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash_serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Skirt");
			v15 = "DLC_Broken";
		}

		set_mesh_invisible(pPlayerModelInfo, v15);

		const char* leftover = "";
		if (pPlayerModelInfo->dword178DC == 0)
		{
			if ((pPlayerModelInfo->outfitEquipped) != 1)
			{
				const char* extra = "";
				if (pPlayerModelInfo->dword16CF4 == 0)
				{
					extra = "Broken";
				}
				if (pPlayerModelInfo->dword16CF4 != 0)
				{
					set_mesh_invisible(pPlayerModelInfo, "Feather");
					if (pPlayerModelInfo->pWMB != 0)
					{
						extra = "Skirt";
					}
				}
				if ((pPlayerModelInfo->dword16CF4 == 0) || ((pPlayerModelInfo->dword16CF4 != 0)))
				{
					set_mesh_invisible(pPlayerModelInfo, extra);
				}

				if (pPlayerModelInfo->dword16CE4 == 0)
				{
					set_mesh_invisible(pPlayerModelInfo, "Broken");
					if (pPlayerModelInfo->pWMB != 0)
					{
						leftover = "Skirt";
					}
				}
			}
			else
			{
				const char* extra = "";
				if ((pPlayerModelInfo->dword16CF4) == 0 && pPlayerModelInfo->pWMB != 0)
				{
					extra = "DLC_Broken";
				}
				if ((pPlayerModelInfo->dword16CF4) != 0 && pPlayerModelInfo->pWMB != 0)
				{
					extra = "DLC_Skirt";
				}
				if (((pPlayerModelInfo->dword16CF4) == 0 && pPlayerModelInfo->pWMB != 0) || ((pPlayerModelInfo->dword16CF4) != 0 && (pPlayerModelInfo->pWMB != 0)))
				{
					set_mesh_invisible(pPlayerModelInfo, extra);
				}

				if (pPlayerModelInfo->dword16CE4 == 0)
				{
					set_mesh_invisible(pPlayerModelInfo, "DLC_Broken");
					if (pPlayerModelInfo->pWMB)
					{
						leftover = "DLC_Skirt";
					}
				}
			}

			if ((((pPlayerModelInfo->outfitEquipped) != 1 && (pPlayerModelInfo->dword16CE4) == 0) && pPlayerModelInfo->pWMB != 0) || (((pPlayerModelInfo->outfitEquipped) == 1 && (pPlayerModelInfo->dword16CE4) == 0) && pPlayerModelInfo->pWMB != 0))
			{
				set_mesh_invisible(pPlayerModelInfo, leftover);
			}

			const char* extra_6 = "";
			if ((pPlayerModelInfo->dword16CEC) != 0)
			{
				if ((pPlayerModelInfo->accessoryEquipped) != 0xe && (pPlayerModelInfo->accessoryEquipped) != 0x3)
				{
					set_mesh_invisible(pPlayerModelInfo, "Eyelash");
				}

				set_mesh_invisible(pPlayerModelInfo, "Eyelash_serious");
			}

			if (((pPlayerModelInfo->dword16CEC) != 0 || ((pPlayerModelInfo->dword16CEC) != 0 && (pPlayerModelInfo->accessoryEquipped) == 0xe)))
			{
				set_mesh_visible(pPlayerModelInfo, "Eyemask");
			}
			if ((pPlayerModelInfo->accessoryEquipped) != 3)
			{
				set_mesh_visible(pPlayerModelInfo, "Eyemask");
			}

			const char* extra2 = "";
			if ((pPlayerModelInfo->dword170B8) == 1)
			{
				set_mesh_invisible(pPlayerModelInfo, "facial_normal");
				set_mesh_invisible(pPlayerModelInfo, "Eyelash");
			}
			else
			{
				set_mesh_invisible(pPlayerModelInfo, "facial_serious");
				set_mesh_invisible(pPlayerModelInfo, "Eyelash_serious");
			}
		}
		else
		{
			setDrawBasePlayerMeshes((void*)(&pPlayerModelInfo->gap0[0x390]), 1);
			if ((pPlayerModelInfo->dword178E0) == 0)
			{
				set_mesh_invisible(pPlayerModelInfo, "Armor_Head");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Skirt");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Broken");
				set_mesh_invisible(pPlayerModelInfo, "Body");
				set_mesh_invisible(pPlayerModelInfo, "Feather");
				set_mesh_invisible(pPlayerModelInfo, "Skirt");
				set_mesh_invisible(pPlayerModelInfo, "Broken");
			}
			else
			{
				setDrawBasePlayerMeshes((void*)(&pPlayerModelInfo->gap0[0x390]), 1);
				set_mesh_visible(pPlayerModelInfo, "Armor_Body");
				set_mesh_visible(pPlayerModelInfo, "Armor_Head");
			}
		}
	}


	if (((pPlayerModelInfo->currentPlayer) != 0x10000 || ((pPlayerModelInfo->currentPlayer) == 0x10000 && (pPlayerModelInfo->dword178DC) == 0)))
	{
		const char* extra = "";
		if ((pPlayerModelInfo->currentPlayer) - 0x10100 <= 1)
		{
			setDrawBasePlayerMeshes((void*)(&pPlayerModelInfo->gap0[0x390]), 1);
			// Destroyer Outfit
			if ((pPlayerModelInfo->outfitEquipped) == 1)
			{
				set_mesh_invisible(pPlayerModelInfo, "Body");
				set_mesh_invisible(pPlayerModelInfo, "Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Body");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Normal");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Serious");
			}

			// YoRHA Uniform Prototype
			else if (pPlayerModelInfo->outfitEquipped == 2)
			{
				set_mesh_invisible(pPlayerModelInfo, "Body");
				set_mesh_invisible(pPlayerModelInfo, "Cloth");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Body");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Normal");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Serious");
			}

			// P2's Body Replica
			else if (pPlayerModelInfo->outfitEquipped == 3)
			{
				set_mesh_invisible(pPlayerModelInfo, "Body");
				set_mesh_invisible(pPlayerModelInfo, "Cloth");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Serious");
			}

			// Normal
			else
			{
				set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Body");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Cloth");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Normal");
				set_mesh_invisible(pPlayerModelInfo, "NS_P2_Serious");
			}

			if (pPlayerModelInfo->unsigned_int17084 != 0)
			{
				if ((sub_745c50((DWORD*)(pPlayerModelInfo->unsigned_int17084)) != 0 && ((pPlayerModelInfo->accessoryEquipped) != 0xf)))
				{
					set_mesh_invisible(pPlayerModelInfo, "Hair");
				}
				DWORD tmp = *(DWORD*)pPlayerModelInfo->unsigned_int17084;
				DWORD tmp2 = *(DWORD*)tmp;

				DWORD* temp_101 = (DWORD*)sub_45a850(sub_745c50((DWORD*)tmp2));
				if (temp_101 != 0)
				{
					*(DWORD*)(DWORD)(temp_101 + 0x843) = (pPlayerModelInfo->accessoryEquipped) != 0xF;
				}
			}

			__int64 temp_144;
			if ((pPlayerModelInfo->outfitEquipped) != 1)
			{
				if ((pPlayerModelInfo->dword16CE4) == 0)
				{
					set_mesh_invisible(pPlayerModelInfo, "Cloth");
				}
			}

			else if ((pPlayerModelInfo->dword16CE4) == 0)
			{
				set_mesh_invisible(pPlayerModelInfo, "DLC_Cloth");
			}

			__int64 temp_147 = (__int64)pPlayerModelInfo->pWMB;
			const char* temp_12 = "";
			if ((pPlayerModelInfo->dword170B8) == 1 && temp_147 != 0)
			{
				temp_12 = "facial_normal";
			}
			if ((pPlayerModelInfo->dword170B8) != 1 && temp_147 != 0)
			{
				temp_12 = "facial_serious";
			}
			if ((((pPlayerModelInfo->dword170B8) == 1 && temp_147 != 0) || ((pPlayerModelInfo->dword170B8) != 1 && temp_147 != 0)))
			{
				set_mesh_invisible(pPlayerModelInfo, temp_12);
			}
		}
	}

	int temp_106 = (pPlayerModelInfo->currentPlayer);
	const char* extra = "";
	if (temp_106 == 0x10200 || temp_106 == 0x10203)
	{
		setDrawBasePlayerMeshes((void*)(&pPlayerModelInfo->gap0[0x390]), 1);
		if (pPlayerModelInfo->outfitEquipped == 1) //young man outfit
		{
			set_mesh_invisible(pPlayerModelInfo, "Pants");
			set_mesh_invisible(pPlayerModelInfo, "Body");
			set_mesh_invisible(pPlayerModelInfo, "Leg");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "mesh_pl0200");
		}
		else
		{
			set_mesh_invisible(pPlayerModelInfo, "DLC_Pants");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Leg");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
		}

		int temp = pPlayerModelInfo->dword177D8 - 1;
		if (pPlayerModelInfo->dword177D8 == 1)
		{
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "Pants");
			set_mesh_invisible(pPlayerModelInfo, "Leg");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Pants");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Leg");
		}

		else
		{
			if (temp == 1)
			{
				set_mesh_invisible(pPlayerModelInfo, "mesh_es0200");
				set_mesh_invisible(pPlayerModelInfo, "mesh_es0202");
				set_mesh_invisible(pPlayerModelInfo, "mesh_es0206");
				set_mesh_invisible(pPlayerModelInfo, "mesh_pl0200");
				set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0200");

				if (pPlayerModelInfo->pWMB != 0)
				{
					extra = "DLC_mesh_es0202";
				}
				else
				{
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
				}
			}
			else
			{
				//normal 9s probably
				set_mesh_invisible(pPlayerModelInfo, "mesh_es0200");	//odd one thing
				set_mesh_invisible(pPlayerModelInfo, "mesh_es0201");
				set_mesh_invisible(pPlayerModelInfo, "mesh_es0206");
				set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0200");
				
				if (pPlayerModelInfo->pWMB != 0)
				{
					extra = "DLC_mesh_es0202";
				}
			}
			if ((temp == 1 && pPlayerModelInfo->pWMB != 0) || ((temp != 1 && temp != 2) && temp == 3))
			{
				SearchMeshGroupIndex(pPlayerModelInfo, extra);
				set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
				set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			}
			if (temp != 1)
			{
				if (temp != 2 && temp != 3)
				{
					set_mesh_invisible(pPlayerModelInfo, "mesh_es0200");
					set_mesh_invisible(pPlayerModelInfo, "mesh_es0201");
					set_mesh_invisible(pPlayerModelInfo, "mesh_es0202");
					set_mesh_invisible(pPlayerModelInfo, "mesh_es0206");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0200");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0201");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0202");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
				}
				if (temp == 1)
				{
					set_mesh_invisible(pPlayerModelInfo, "mesh_es0200");
					set_mesh_invisible(pPlayerModelInfo, "mesh_es0201");
					set_mesh_invisible(pPlayerModelInfo, "mesh_es0202");
					set_mesh_invisible(pPlayerModelInfo, "mesh_pl0200");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0200");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0201");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0202");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
				}
			}
		}
		if (pPlayerModelInfo->dword16CF4)
		{
			if (pPlayerModelInfo->dword16CE0 != 2)
			{
				set_mesh_invisible(pPlayerModelInfo, "Pants");
			}
			set_mesh_invisible(pPlayerModelInfo, "DLC_Pants");
		}

		//dress module lol
		if (!pPlayerModelInfo->dword16CE4)
		{
			set_mesh_invisible(pPlayerModelInfo, "Pants");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Pants");
		}

		char buffer[256];
		__int64 var_18_1 = 0;
		sprintf_s(buffer, 0x10, "PL/9S_NoPatch");

		//count size of string inside buffer
		
		int count = -1;
		while (buffer[count])
		{
			++count;
		}

		//mostly 1:1 to binaryninja
		__int64 temp_220 = sub_150d80((void*)(modBase + 0xF8C220), (void*)sub_28ed30(&buffer, count), &buffer, count);
		__int64 temp_38 = temp_220;
		int temp_19;

		if (temp_220 == 0)
		{
			temp_19 = 0;
		}
		else
		{
			temp_19 = *(unsigned int*)(temp_220 + 32);
		}
		char temp_221 = sub_495970((void*)(modBase + 0xFC2370), temp_19);
		sub_150b70(&temp_38);
		

		const char* temp_20 = "";
		if (pPlayerModelInfo->dword16CEC != 0)
		{
			DWORD temp_324 = pPlayerModelInfo->accessoryEquipped;
			if ((temp_324 != 0xe && temp_221 == 0) && temp_324 != 3)
			{
				set_mesh_invisible(pPlayerModelInfo, "Eyelash");
				if (pPlayerModelInfo != 0)
				{
					temp_20 = "Eyelash_serious";
					set_mesh_invisible(pPlayerModelInfo, temp_20);
				}
			}
		}

		if ((((pPlayerModelInfo->dword16CEC) == 0 || ((pPlayerModelInfo->dword16CEC) != 0 && pPlayerModelInfo->accessoryEquipped == 0xe)) || (((pPlayerModelInfo->dword16CEC) != 0 && pPlayerModelInfo->accessoryEquipped != 0xe) && temp_221 != 0)))
		{
			set_mesh_invisible(pPlayerModelInfo, "Eyemask");
		}

		if (pPlayerModelInfo->accessoryEquipped == 3)
		{
			set_mesh_invisible(pPlayerModelInfo, "Eyemask");
		}

		const char* temp_21 = "";
		if (pPlayerModelInfo->dword170B8 == 1)
		{
			set_mesh_invisible(pPlayerModelInfo, "facial_normal");

			if (pPlayerModelInfo->pWMB)
			{
				temp_21 = "Eyelash";
			}
		}
		else
		{
			set_mesh_invisible(pPlayerModelInfo, "facial_serious");

			if (pPlayerModelInfo->pWMB)
			{
				temp_21 = "Eyelash_serious";
			}
		}

		if ((pPlayerModelInfo->dword170B8 == 1 && pPlayerModelInfo->pWMB != 0) || (pPlayerModelInfo->dword170B8 != 1 && pPlayerModelInfo->pWMB != 0))
		{
			set_mesh_invisible(pPlayerModelInfo, temp_21);
		}

		if (((pPlayerModelInfo->currentPlayer - 0x10500) & 0xfffffeff) == 0)
		{
			setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 1);
			const char* temp_22 = "";
			if (pPlayerModelInfo->dword170B8 == 1 && pPlayerModelInfo->pWMB != 0)
			{
				temp_22 = "facial_normal";
			}
			if (pPlayerModelInfo->dword170B8 != 1 && pPlayerModelInfo->pWMB != 0)
			{
				temp_22 = "facial_serious";
			}
			if ((pPlayerModelInfo->dword170B8 != 1 && pPlayerModelInfo->pWMB != 0) || (pPlayerModelInfo->dword170B8 != 1 && pPlayerModelInfo->pWMB != 0))
			{
				set_mesh_invisible(pPlayerModelInfo, temp_22);
			}
			
			set_mesh_invisible(pPlayerModelInfo, "Clothing_Sit");
		}
	}
	
	//the return code is weird just using this as default maybe itll end the world idk
	__int64 result = 0x10000;
	if (((*(__int8*)modBase + 0x10297F0) & 0x20) == 0 || ((*(__int8*)modBase + 0x10297F0) & 20) != 0 && (pPlayerModelInfo->dword10628 == 0))
	{
		if (pPlayerModelInfo->unsigned_int17084 != 0)
		{
			DWORD tmp = *(DWORD*)pPlayerModelInfo->unsigned_int17084;
			DWORD tmp2 = *(DWORD*)tmp;
			DWORD* temp_247 = (DWORD*)sub_45a850(sub_745c50((DWORD*)tmp2));
			if (temp_247 != 0)
			{
				setDrawBasePlayerMeshes(temp_247 + 912, 1);
			}
			
			tmp = *(DWORD*)pPlayerModelInfo->unsigned_int170C0;
			tmp2 = *(DWORD*)tmp;
			DWORD* tmp_251 = (DWORD*)sub_45a850(sub_745c50((DWORD*)tmp2));
			if (tmp_251 != 0)
			{
				*(tmp_251 + 0x834) = 1;
			}
		}
	}

	if (((*(__int8*)modBase + 0x10297F0) & 0x20) != 0 && (pPlayerModelInfo->dword10628 != 0))
	{
		setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 0);
		__int64 temp_241 = sub_491170((int*)pPlayerModelInfo->unsigned_int17084);
		if (temp_241 != 0)
		{
			setDrawBasePlayerMeshes((void*)(temp_241 + 0x390), 1);
		}
		__int64 temp_243 = sub_491400((int*)pPlayerModelInfo->unsigned_int170C0);
		if (temp_243 != 0)
		{
			*((int*)(temp_243 + 0x834)) = 0;
			result = *(DWORD*)(result + 2100);
		}
	}
	pPlayerModelInfo->dword10628 = 0;
	return result;
}

void ConsoleSetup() 
{
	AllocConsole();
	freopen_s(&stream, "log.txt", "w", stdout);
	freopen_s(&stream, "log.txt", "w", stderr);
	freopen_s(&stream, "log.txt", "w", stdin);
}

int Main(PVOID lpParameter)
{
    modBase = (uintptr_t)GetModuleHandle(NULL);
    _ManageMeshVisibilities ManageMeshVisibilities = (_ManageMeshVisibilities)(modBase + 0x51B940);
    SearchMeshGroupIndex = (_SearchMeshGroupIndex)(modBase + 0x1910B0);
	updateAccessoryOnUnpause = (_UpdateAccessoryOnUnpause)(modBase + 0x482360);
	UpdateEquippedActive = (_UpdateEquippedActive)(modBase + 0x7f50f0);
	ValidateAccessory = (_ValidateAccessory)(modBase + 0x7f4830);
	ValidateDLCArmor = (_ValidateDLCArmor)(modBase + 0x7f4770);
	ValidatePodSkins = (_ValidatePodSkins)(modBase + 0x7f6d90);
	ValidateNonCharacterSpecificEquippable = (_ValidateNonCharacterSpecificEquippable)(modBase + 0x7f4020);
	AnotherAccessoryThing = (_AnotherAccessoryThing)(modBase + 0x4eca60);
	SetOutfitFromPause = (_SetOutfitFromPause)(modBase + 0x475eb0);
	SetEquippedFromPause = (_SetEquippedFromPause)(modBase + 0x843ee0);
	ResolveNameFromItemID = (_ResolveNameFromItemID)(modBase + 0x7e35c0);
	GetAccessoryIDFromItemID = (_GetAccessoryIDFromItemID)(modBase + 0x7d4310);
	GetOutfitIDFromItemID = (_GetOutfitIDFromItemID)(modBase + 0x7d42a0);
	sub_52e9e0 = (_sub_52e9e0)(modBase + 0x52e9e0);
	sub_745c50 = (_sub_745c50)(modBase + 0x745C50);
	sub_45a850 = (_sub_45a850)(modBase + 0x45A850);
	sub_28ed30 = (_sub_28ed30)(modBase + 0x28ED30);
	sub_150d80 = (_sub_150d80)(modBase + 0x150D80);
	sub_495970 = (_sub_495970)(modBase + 0x495970);
	sub_150b70 = (_sub_150b70)(modBase + 0x150B70);
	sub_491170 = (_sub_491170)(modBase + 0x491170);
	sub_491400 = (_sub_491400)(modBase + 0x491400);
	sub_7463c0 = (_sub_7463c0)(modBase + 0x7463C0);
	sub_3e6b70 = (_sub_3e6b70)(modBase + 0x3e6B70);
	sub_45a8c0 = (_sub_45a8c0)(modBase + 0x45a8c0);
	sub_3876a0 = (_sub_3876a0)(modBase + 0x3876a0);
	sub_7459b0 = (_sub_7459b0)(modBase + 0x7459b0);
	sub_7e6e60 = (_sub_7e6e60)(modBase + 0x7e6e60);
	sub_3084c0 = (_sub_3084c0)(modBase + 0x3084c0);
	sub_7c4b50 = (_sub_7c4b50)(modBase + 0x7c4b50);
	sub_7c4b90 = (_sub_7c4b90)(modBase + 0x7c4b90);
	sub_7c9cb0 = (_sub_7c9cb0)(modBase + 0x7c9cb0);
	setDrawBasePlayerMeshes = (_SetDrawBasePlayerMeshes)(modBase + 0x197C70);

	_UpdateAccessoryOnUnpause fpUpdateAccessoryOnUnpause = NULL;
	_UpdateEquippedActive fpUpdateEquippedActive = NULL;
	_ValidateDLCArmor fpValidateDLCArmor = NULL;
	_ValidateNonCharacterSpecificEquippable fpValidateNonSpecificCharacterEquippable = NULL;
	
	//Thanks wolf!
	ConsoleSetup();

    if (MH_Initialize() != MH_OK)
    {
        return 1;
    }

	
    if (MH_CreateHook(ManageMeshVisibilities, &HkManageMeshVisibilites, reinterpret_cast<LPVOID*>(&fpManageMeshVisiblities)) != MH_OK)
    {
        return 1;
    }

	if (MH_CreateHook(ValidateDLCArmor, &hkValidateDLCArmor, reinterpret_cast<LPVOID*>(&fpValidateDLCArmor)) != MH_OK)
	{
		return 1;
	}

	if (MH_CreateHook(ValidateNonCharacterSpecificEquippable, &hkValidateNonSpecificCharacterEquippable, reinterpret_cast<LPVOID*>(&fpValidateNonSpecificCharacterEquippable)) != MH_OK)
	{
		return 1;
	}

	if (MH_CreateHook(SetEquippedFromPause, &hkSetEquippedFromPause, reinterpret_cast<LPVOID*>(&fpSetEquippedFromPause)) != MH_OK)
	{
		return 1;
	}

	if (MH_CreateHook(UpdateEquippedActive, &hkUpdateEquippedActive, reinterpret_cast<LPVOID*>(&fpUpdateEquippedActive)) != MH_OK)
	{
		return 1;
	}
	
	if (MH_CreateHook(GetOutfitIDFromItemID, &hkGetOutfitIDFromItemID, reinterpret_cast<LPVOID*>(&fpGetOutfitIDFromItemID)) != MH_OK)
	{
		return 1;
	}
	
    if (MH_EnableHook(ManageMeshVisibilities) != MH_OK)
    {
        return 1;
    }
	
	if (MH_EnableHook(ValidateDLCArmor) != MH_OK)
	{
		return 1;
	}

	if (MH_EnableHook(ValidateNonCharacterSpecificEquippable) != MH_OK)
	{
		return 1;
	}

	if (MH_EnableHook(SetEquippedFromPause) != MH_OK)
	{
		return 1;
	}

	if (MH_EnableHook(UpdateEquippedActive) != MH_OK)
	{
		return 1;
	}

	if (MH_EnableHook(GetOutfitIDFromItemID) != MH_OK)
	{
		return 1;
	}

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), (PVOID)hModule, NULL, NULL);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

