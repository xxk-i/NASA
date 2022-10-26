// dllmain.cpp : Defines the entry point for the DLL application.
#include "minhook/include/MinHook.h"
#include "niera/NierA.h"
#include <iostream>

//Stinky globals :)
uintptr_t modBase = NULL;
FILE* stream;

//TODO maybe look at this? try to avoid resolving name?
__int64 hkGetOutfitIDFromItemID(__int64 item_base, int item_ID)
{
	//Original function does some goofy shit IDK my way or the highway bitch
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

	return NieR::fpGetOutfitIDFromItemID(item_base, item_ID);
}

/*
Function optimized and decompiled odd, but basically only reaches the end if
a strcmp doesnt match (strcmp returns 0) and returns if the outfit is active

This is my refactored version
*/
bool hkUpdateEquippedActive(__int64 a1, __int64 item_id, int currentPlayer)
{
	//std::cout << "ItemID: " << item_id << " ResolvedName: " << NieR::ResolveNameFromItemID(modBase + 0x133b510, item_id) << std::endl;

	const char* resolvedName = "";
	// Remember we hook this validate function, so this calls the original, which actually jumps to our hook
	if (!NieR::ValidateNonCharacterSpecificEquippable(modBase + 0x133b510, item_id) || currentPlayer > 2 || item_id == -1)
	{
		return 0;
	}
	
	//2B
	if (currentPlayer == 0)
	{
		resolvedName = NieR::ResolveNameFromItemID(modBase + 0x133b510, item_id);
		if (strcmp(resolvedName, "item_uq_changeArmour1") &&
			strcmp(resolvedName, "item_uq_changeArmour2") &&
			strcmp(resolvedName, "item_uq_dlcCloth1") &&
			strcmp(resolvedName, "item_uq_dlcOutfit1") &&
			strcmp(resolvedName, "item_uq_dlcOutfit2"))
		{
			return false;
		}
	}

	//9S
	else if (currentPlayer == 1)
	{
		resolvedName = NieR::ResolveNameFromItemID(modBase + 0x133b510, item_id);
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
		resolvedName = NieR::ResolveNameFromItemID(modBase + 0x133b510, item_id);
		if (strcmp(resolvedName, "item_uq_dlcCloth3") &&
			strcmp(resolvedName, "item_uq_dlcOutfit5") &&
			strcmp(resolvedName, "item_uq_dlcOutfit6"))
		{
			return false;
		}
	}

	if (NieR::sub_7c4b50() != 0)
	{
		//Same reminder, we are hooking GetOutfitIDFromItemID
		//If this returns true, the outfit shows as ACTIVE
		if (NieR::GetPlayerFromPlayerNum(currentPlayer))
		{
			std::cout << "Outfit_ID: " << NieR::GetOutfitIDFromItemID(modBase + 0x133b510, item_id) << " OutFitEquipped: " << ((NieR::PlayerModelInfo*)NieR::GetPlayerFromPlayerNum(currentPlayer))->outfitEquipped << std::endl;
			return NieR::GetOutfitIDFromItemID(modBase + 0x133b510, item_id) == ((NieR::PlayerModelInfo*)NieR::GetPlayerFromPlayerNum(currentPlayer))->outfitEquipped;
		}
		//return NieR::GetOutfitIDFromItemID(modBase + 0x133b510, item_id) == *(int*)((modBase + 0x1494354) + (sizeof(int) * currentPlayer));
	}

	return false;
}

//TODO: DO THIS FOR ACCESSORIES
bool hkValidateAccessory(__int64 item_base, __int64 item_id, int currentPlayer)
{
	return true;
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
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_changeArmour1"))
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_changeArmour2"))
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit1"))
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit2"))
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		expected_name = "item_uq_dlcCloth1";
	}

	//9S
	else if (currentPlayer == 1)
	{
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit3"))	//9P
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit4"))	//Kimono
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		expected_name = "item_uq_dlcCloth2";
	}

	//A2
	else if (currentPlayer == 2)
	{
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit5"))	//P2
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit6"))	//Kimono
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
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
		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_changeArmour1"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_changeArmour2"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_changeArmour2"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_dlcCloth1"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_dlcCloth2"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_dlcCloth3"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit1"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit2"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit3"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit4"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit5"))
			return 1;

		if (!strcmp(NieR::ResolveNameFromItemID(item_base, item_id), "item_uq_dlcOutfit6"))
			return 1;
	}

	return 0;
}

void set_mesh_invisible(NieR::PlayerModelInfo* pPlayerModelInfo, const char* mesh_name)
{
    if (pPlayerModelInfo->pWMB)
    {
        __int64 mesh_group_index = NieR::SearchMeshGroupIndex(pPlayerModelInfo->pWMB, mesh_name);
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

void set_mesh_visible(NieR::PlayerModelInfo* pPlayerModelInfo, const char* mesh_name)
{
    if (pPlayerModelInfo->pWMB)
    {
        __int64 mesh_group_index = NieR::SearchMeshGroupIndex(pPlayerModelInfo->pWMB, mesh_name);
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
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;
		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 4, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//Yorha Uniform 1 (Kimono)
	if (item_id == 82)
	{
		DWORD playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;
		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 5, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//9P (White 9S)
	if (item_id == 83)
	{
		DWORD playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;
		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 2, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//9S Kimono
	if (item_id == 84)
	{
		DWORD playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;
		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 3, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//P2's Body Replica
	if (item_id == 85)
	{
		DWORD playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;
		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 3, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//YoRHA Uniform Prototype (NS_A2)
	if (item_id == 86)
	{
		DWORD playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;
		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 2, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	else
	{
		return NieR::fpSetEquippedFromPause(a1, item_id);
	}
}

//Hook
__int64 __fastcall HkManageMeshVisibilites(NieR::PlayerModelInfo* pPlayerModelInfo)
{
	const char* v15 = "";

	//2B
	if (pPlayerModelInfo->currentPlayer == 0x10000)
	{
		NieR::setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 1);
		__int64 thing = (__int64)pPlayerModelInfo;
		if (pPlayerModelInfo->outfitEquipped == 1) //kaine outfit
		{
			set_mesh_invisible(pPlayerModelInfo, "Armor_Body");
			set_mesh_invisible(pPlayerModelInfo, "Armor_Head");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Feather");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash");
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
			NieR::setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 0);
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
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Feather");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Skirt");
			v15 = "Broken";
		}

		else if (pPlayerModelInfo->outfitEquipped == 4)	//2P
		{
			// We could do this nicely and do activations instead of deactivations but it doesn't play as nice with the other code
			set_mesh_invisible(pPlayerModelInfo, "Armor_Body");
			set_mesh_invisible(pPlayerModelInfo, "Armor_Head");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			set_mesh_invisible(pPlayerModelInfo, "Broken");
			set_mesh_invisible(pPlayerModelInfo, "Body");
			set_mesh_invisible(pPlayerModelInfo, "Feather");
			set_mesh_invisible(pPlayerModelInfo, "Skirt");
		}

		else if (pPlayerModelInfo->outfitEquipped == 5) //Kimono
		{
			set_mesh_invisible(pPlayerModelInfo, "Armor_Body");
			set_mesh_invisible(pPlayerModelInfo, "Armor_Head");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Feather");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "Broken");
			set_mesh_invisible(pPlayerModelInfo, "Body");
			set_mesh_invisible(pPlayerModelInfo, "Feather");
			set_mesh_invisible(pPlayerModelInfo, "Skirt");
		}

		else  // normal
		{
			set_mesh_invisible(pPlayerModelInfo, "Armor_Body");
			set_mesh_invisible(pPlayerModelInfo, "Armor_Head");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Feather");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Broken");
			set_mesh_invisible(pPlayerModelInfo, "NS_2P_Skirt");
		}


		if (pPlayerModelInfo->dwordisDestructed == 0)
		{
			//There are usually outfit checks here but it really doesn't matter, we arent saving frames reducing these calls
			if (pPlayerModelInfo->isBuddy == 0)
			{
				set_mesh_invisible(pPlayerModelInfo, "Broken");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Broken");
			}
			if (pPlayerModelInfo->isBuddy != 0)
			{
				set_mesh_invisible(pPlayerModelInfo, "Feather");
				set_mesh_invisible(pPlayerModelInfo, "Skirt");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Skirt");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Feather");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Skirt");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			}

			if (pPlayerModelInfo->dwordShowPants == 0)
			{
				set_mesh_invisible(pPlayerModelInfo, "Broken");
				set_mesh_invisible(pPlayerModelInfo, "Skirt");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Broken");
				set_mesh_invisible(pPlayerModelInfo, "DLC_Skirt");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Skirt");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			}

			if ((pPlayerModelInfo->dword16CEC) != 0)
			{
				if ((pPlayerModelInfo->accessoryEquipped) != 0xe && (pPlayerModelInfo->accessoryEquipped) != 0x3)
				{
					set_mesh_invisible(pPlayerModelInfo, "Eyelash");
					set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash");
				}

				//This doesn't seem to exist (anymore)(?)
				//set_mesh_invisible(pPlayerModelInfo, "Eyelash_serious");
			}

			if (((pPlayerModelInfo->dword16CEC) != 0 || ((pPlayerModelInfo->dword16CEC) != 0 && (pPlayerModelInfo->accessoryEquipped) == 0xe)))
			{
				set_mesh_visible(pPlayerModelInfo, "Eyemask");
				set_mesh_visible(pPlayerModelInfo, "NS_2P_Eyemask");
			}
			if ((pPlayerModelInfo->accessoryEquipped) != 3)
			{
				set_mesh_visible(pPlayerModelInfo, "Eyemask");
				set_mesh_visible(pPlayerModelInfo, "NS_2P_Eyemask");
			}

			if ((pPlayerModelInfo->dword170B8) == 1)
			{
				set_mesh_invisible(pPlayerModelInfo, "facial_normal");
				set_mesh_invisible(pPlayerModelInfo, "Eyelash");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Normal");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Normal");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash");
			}
			else
			{
				set_mesh_invisible(pPlayerModelInfo, "facial_serious");
				set_mesh_invisible(pPlayerModelInfo, "Eyelash_serious");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_serious");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Serious");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash");
			}
		}
		else
		{
			NieR::setDrawBasePlayerMeshes((void*)(&pPlayerModelInfo->gap0[0x390]), 1);
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
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Chest");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Normal");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Serious");
				set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Serious");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyemask");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Normal");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Feather");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Eyelash");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Body");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Broken");
				set_mesh_invisible(pPlayerModelInfo, "NS_2P_Skirt");
			}
			else
			{
				NieR::setDrawBasePlayerMeshes((void*)(&pPlayerModelInfo->gap0[0x390]), 1);
				set_mesh_visible(pPlayerModelInfo, "Armor_Body");
				set_mesh_visible(pPlayerModelInfo, "Armor_Head");
			}
		}
	}


	if (((pPlayerModelInfo->currentPlayer) != 0x10000 || ((pPlayerModelInfo->currentPlayer) == 0x10000 && (pPlayerModelInfo->dwordisDestructed) == 0)))
	{
		const char* extra = "";
		if ((pPlayerModelInfo->currentPlayer) - 0x10100 <= 1)
		{
			NieR::setDrawBasePlayerMeshes((void*)(&pPlayerModelInfo->gap0[0x390]), 1);
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
				if ((NieR::sub_745c50((DWORD*)(pPlayerModelInfo->unsigned_int17084)) != 0 && ((pPlayerModelInfo->accessoryEquipped) != 0xf)))
				{
					set_mesh_invisible(pPlayerModelInfo, "Hair");
				}
				DWORD tmp = *(DWORD*)pPlayerModelInfo->unsigned_int17084;
				DWORD tmp2 = *(DWORD*)tmp;

				DWORD* temp_101 = (DWORD*)NieR::sub_45a850(NieR::sub_745c50((DWORD*)tmp2));
				if (temp_101 != 0)
				{
					*(DWORD*)(DWORD)(temp_101 + 0x843) = (pPlayerModelInfo->accessoryEquipped) != 0xF;
				}
			}

			__int64 temp_144;
			if ((pPlayerModelInfo->outfitEquipped) != 1)
			{
				if ((pPlayerModelInfo->dwordShowPants) == 0)
				{
					set_mesh_invisible(pPlayerModelInfo, "Cloth");
				}
			}

			else if ((pPlayerModelInfo->dwordShowPants) == 0)
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
		NieR::setDrawBasePlayerMeshes((void*)(&pPlayerModelInfo->gap0[0x390]), 1);

		//Normal
		if (pPlayerModelInfo->outfitEquipped == 0)
		{
			set_mesh_invisible(pPlayerModelInfo, "DLC_Pants");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Leg");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Leg");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Pants");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Leg");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Pants");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken_Pants");
		}

		//Young Man Outfit
		if (pPlayerModelInfo->outfitEquipped == 1)
		{
			set_mesh_invisible(pPlayerModelInfo, "Pants");
			set_mesh_invisible(pPlayerModelInfo, "Body");
			set_mesh_invisible(pPlayerModelInfo, "Leg");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Leg");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Pants");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Leg");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Pants");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken_Pants");
		}

		//9P (White 9S)
		if (pPlayerModelInfo->outfitEquipped == 2)
		{
			set_mesh_invisible(pPlayerModelInfo, "Pants");
			set_mesh_invisible(pPlayerModelInfo, "Body");
			set_mesh_invisible(pPlayerModelInfo, "Leg");
			set_mesh_invisible(pPlayerModelInfo, "Eyemask");	//this one got custom eyewear ray-ban
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Pants");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Leg");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Leg");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Pants");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Broken_Pants");
		}

		//Kimono
		else if (pPlayerModelInfo->outfitEquipped == 3)
		{
			set_mesh_invisible(pPlayerModelInfo, "Pants");
			set_mesh_invisible(pPlayerModelInfo, "Body");
			set_mesh_invisible(pPlayerModelInfo, "Leg");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Pants");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Body");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Leg");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0206");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_pl0200");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Body");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Leg");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Pants");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Serious");
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
				set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
				set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0202");
				set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");

				//idk this is in the decomp but set_mesh_invisible doesnt do anything if pWMB is 0 so
				//if (pPlayerModelInfo->pWMB == 0)
				//{
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
					set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
				//}
			}
			else
			{
				set_mesh_invisible(pPlayerModelInfo, "mesh_es0200");
				set_mesh_invisible(pPlayerModelInfo, "mesh_es0201");
				set_mesh_invisible(pPlayerModelInfo, "mesh_es0206");
				set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0200");
				set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0202");
			}
			if (temp == 3)
			{
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
					set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
					set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
					set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
					set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0206");
					set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
					set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
					set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
					set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0206");
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
					set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
					set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
					set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
					set_mesh_invisible(pPlayerModelInfo, "NS_9P_Mesh_pl0200");
					set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
					set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
					set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
					set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Mesh_pl0200");
				}
			}
		}
		if (pPlayerModelInfo->isBuddy)
		{
			if (pPlayerModelInfo->dword16CE0 != 2)
			{
				set_mesh_invisible(pPlayerModelInfo, "Pants");
			}
			set_mesh_invisible(pPlayerModelInfo, "DLC_Pants");
		}

		//dress module lol
		if (!pPlayerModelInfo->dwordShowPants)
		{
			set_mesh_invisible(pPlayerModelInfo, "Pants");
			set_mesh_invisible(pPlayerModelInfo, "DLC_Pants");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Pants");
			set_mesh_invisible(pPlayerModelInfo, "NS_KIMONO_Pants");
		}

		char* buffer = new char[256];
		__int64 var_18_1 = 0;
		sprintf_s(buffer, 0x10, "PL/9S_NoPatch");

		//count size of string inside buffer
		
		__int64 count = -1;
		while (buffer[count])
		{
			++count;
		}

		//mostly 1:1 to binaryninja
		__int64 temp_220 = NieR::sub_150d80((void*)(modBase + 0xF8C220), (void*)NieR::sub_28ed30(buffer, count), buffer, count);
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
		char temp_221 = NieR::sub_495970((void*)(modBase + 0xFC2370), temp_19);
		NieR::sub_150b70(&temp_38);
		

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

		//TODO bullshit if
		if ((((pPlayerModelInfo->dword16CEC) == 0 || ((pPlayerModelInfo->dword16CEC) != 0 && pPlayerModelInfo->accessoryEquipped == 0xe)) || (((pPlayerModelInfo->dword16CEC) != 0 && pPlayerModelInfo->accessoryEquipped != 0xe) && temp_221 != 0)))
		{
			set_mesh_invisible(pPlayerModelInfo, "Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Eyemask");
		}

		//Camouflage Goggles
		if (pPlayerModelInfo->accessoryEquipped == 3)
		{
			set_mesh_invisible(pPlayerModelInfo, "Eyemask");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Eyemask");
		}

		const char* temp_21 = "";
		if (pPlayerModelInfo->dword170B8 == 1)
		{
			set_mesh_invisible(pPlayerModelInfo, "facial_normal");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Normal");

			if (pPlayerModelInfo->pWMB)
			{
				temp_21 = "Eyelash";
			}
		}
		else
		{
			set_mesh_invisible(pPlayerModelInfo, "facial_serious");
			set_mesh_invisible(pPlayerModelInfo, "NS_9P_Serious");

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
			NieR::setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 1);
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
	if (((*(DWORD*)(modBase + 0x10297F0)) & 0x20) == 0 || ((*(DWORD*)(modBase + 0x10297F0)) & 20) != 0 && (pPlayerModelInfo->dword10628 == 0))
	{
		if (pPlayerModelInfo->unsigned_int17084 != 0)
		{
			DWORD tmp = *(DWORD*)pPlayerModelInfo->unsigned_int17084;
			DWORD tmp2 = *(DWORD*)tmp;
			DWORD* temp_247 = (DWORD*)NieR::sub_45a850(NieR::sub_745c50((DWORD*)tmp2));
			if (temp_247 != 0)
			{
				NieR::setDrawBasePlayerMeshes(temp_247 + 912, 1);
			}
			
			tmp = *(DWORD*)pPlayerModelInfo->unsigned_int170C0;
			tmp2 = *(DWORD*)tmp;
			DWORD* tmp_251 = (DWORD*)NieR::sub_45a850(NieR::sub_745c50((DWORD*)tmp2));
			if (tmp_251 != 0)
			{
				*(tmp_251 + 0x834) = 1;
			}
		}
	}

	if (((*(__int8*)(modBase + 0x10297F0)) & 0x20) != 0 && (pPlayerModelInfo->dword10628 != 0))
	{
		NieR::setDrawBasePlayerMeshes((void*)&pPlayerModelInfo->gap0[0x390], 0);
		__int64 temp_241 = NieR::sub_491170((int*)pPlayerModelInfo->unsigned_int17084);
		if (temp_241 != 0)
		{
			NieR::setDrawBasePlayerMeshes((void*)(temp_241 + 0x390), 1);
		}
		__int64 temp_243 = NieR::sub_491400((int*)pPlayerModelInfo->unsigned_int170C0);
		if (temp_243 != 0)
		{
			*((int*)(temp_243 + 0x834)) = 0;
			result = *(DWORD*)(result + 2100);
		}
	}
	pPlayerModelInfo->dword10628 = 0;
	return result;
}

void InitializeFunctionPointers()
{
    NieR::ManageMeshVisibilities = (NieR::_ManageMeshVisibilities)(modBase + 0x51B940);
    NieR::SearchMeshGroupIndex = (NieR::_SearchMeshGroupIndex)(modBase + 0x1910B0);
	NieR::updateAccessoryOnUnpause = (NieR::_UpdateAccessoryOnUnpause)(modBase + 0x482360);
	NieR::UpdateEquippedActive = (NieR::_UpdateEquippedActive)(modBase + 0x7f50f0);
	NieR::ValidateAccessory = (NieR::_ValidateAccessory)(modBase + 0x7f4830);
	NieR::ValidateDLCArmor = (NieR::_ValidateDLCArmor)(modBase + 0x7f4770);
	NieR::ValidatePodSkins = (NieR::_ValidatePodSkins)(modBase + 0x7f6d90);
	NieR::ValidateNonCharacterSpecificEquippable = (NieR::_ValidateNonCharacterSpecificEquippable)(modBase + 0x7f4020);
	NieR::AnotherAccessoryThing = (NieR::_AnotherAccessoryThing)(modBase + 0x4eca60);
	NieR::SetOutfitFromPause = (NieR::_SetOutfitFromPause)(modBase + 0x475eb0);
	NieR::SetEquippedFromPause = (NieR::_SetEquippedFromPause)(modBase + 0x843ee0);
	NieR::ResolveNameFromItemID = (NieR::_ResolveNameFromItemID)(modBase + 0x7e35c0);
	NieR::GetAccessoryIDFromItemID = (NieR::_GetAccessoryIDFromItemID)(modBase + 0x7d4310);
	NieR::GetOutfitIDFromItemID = (NieR::_GetOutfitIDFromItemID)(modBase + 0x7d42a0);
	NieR::sub_52e9e0 = (NieR::_sub_52e9e0)(modBase + 0x52e9e0);
	NieR::sub_745c50 = (NieR::_sub_745c50)(modBase + 0x745C50);
	NieR::sub_45a850 = (NieR::_sub_45a850)(modBase + 0x45A850);
	NieR::sub_28ed30 = (NieR::_sub_28ed30)(modBase + 0x28ED30);
	NieR::sub_150d80 = (NieR::_sub_150d80)(modBase + 0x150D80);
	NieR::sub_495970 = (NieR::_sub_495970)(modBase + 0x495970);
	NieR::sub_150b70 = (NieR::_sub_150b70)(modBase + 0x150B70);
	NieR::sub_491170 = (NieR::_sub_491170)(modBase + 0x491170);
	NieR::sub_491400 = (NieR::_sub_491400)(modBase + 0x491400);
	NieR::sub_7463c0 = (NieR::_sub_7463c0)(modBase + 0x7463C0);
	NieR::sub_3e6b70 = (NieR::_sub_3e6b70)(modBase + 0x3e6B70);
	NieR::sub_45a8c0 = (NieR::_sub_45a8c0)(modBase + 0x45a8c0);
	NieR::sub_3876a0 = (NieR::_sub_3876a0)(modBase + 0x3876a0);
	NieR::sub_7459b0 = (NieR::_sub_7459b0)(modBase + 0x7459b0);
	NieR::sub_7e6e60 = (NieR::_sub_7e6e60)(modBase + 0x7e6e60);
	NieR::GetPlayerFromPlayerNum = (NieR::_GetPlayerFromPlayerNum)(modBase + 0x3084c0);
	NieR::sub_7c4b50 = (NieR::_sub_7c4b50)(modBase + 0x7c4b50);
	NieR::sub_7c4b90 = (NieR::_sub_7c4b90)(modBase + 0x7c4b90);
	NieR::sub_7c9cb0 = (NieR::_sub_7c9cb0)(modBase + 0x7c9cb0);
	NieR::setDrawBasePlayerMeshes = (NieR::_SetDrawBasePlayerMeshes)(modBase + 0x197C70);
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
	//Thanks wolf!
	ConsoleSetup();

    modBase = (uintptr_t)GetModuleHandle(NULL);
	InitializeFunctionPointers();

    if (MH_Initialize() != MH_OK)
    {
        return 1;
    }

	//if (MH_CreateHook(NieR::UpdateEquippedActive, &hkUpdateEquippedActive, reinterpret_cast<LPVOID*>(&NieR::fpUpdateEquippedActive)) != MH_OK) return 1;
	//if (MH_EnableHook(NieR::UpdateEquippedActive) != MH_OK) return 1;

    if (MH_CreateHook(NieR::ManageMeshVisibilities, &HkManageMeshVisibilites, reinterpret_cast<LPVOID*>(&NieR::fpManageMeshVisiblities)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::ManageMeshVisibilities) != MH_OK) return 1;

	if (MH_CreateHook(NieR::ValidateDLCArmor, &hkValidateDLCArmor, reinterpret_cast<LPVOID*>(&NieR::fpValidateDLCArmor)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::ValidateDLCArmor) != MH_OK) return 1;

	if (MH_CreateHook(NieR::ValidateNonCharacterSpecificEquippable, &hkValidateNonSpecificCharacterEquippable, reinterpret_cast<LPVOID*>(&NieR::fpValidateNonSpecificCharacterEquippable)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::ValidateNonCharacterSpecificEquippable) != MH_OK) return 1;

	if (MH_CreateHook(NieR::GetOutfitIDFromItemID, &hkGetOutfitIDFromItemID, reinterpret_cast<LPVOID*>(&NieR::fpGetOutfitIDFromItemID)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::GetOutfitIDFromItemID) != MH_OK) return 1;

	if (MH_CreateHook(NieR::SetEquippedFromPause, &hkSetEquippedFromPause, reinterpret_cast<LPVOID*>(&NieR::fpSetEquippedFromPause)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::SetEquippedFromPause) != MH_OK) return 1;

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
