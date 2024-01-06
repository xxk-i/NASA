// dllmain.cpp : Defines the entry point for the DLL application.
#include <iostream>
#include <fstream>
#include "minhook/include/MinHook.h"
#include "niera/NierA.h"

//Stinky globals :)
uintptr_t modBase = NULL;
FILE* stream;

__int64 hkSetAccessory(__int64 a1, const char* meshGroup, int accessoryFileID, __int64 pUnk)
{
	printf("Mesh Group: %s\t FileID: %#010x\n", meshGroup, accessoryFileID);
	return NieR::SetMeshToGroup(a1, meshGroup, accessoryFileID, pUnk);
}


//Used a lot in the executable
__int64* lambda_meme(__int64* a1, int* a2)
{
	*(__int64*)a1 = *a2;
	return a1;
}

//Unused since the outfit ID returned by the original function
//may be used somewhere else in the executable which results in failed outfit validation checks.
__int64 hkGetOutfitIDFromItemID(__int64 item_base, int item_ID)
{
	//Original function does some goofy shit IDK my way or the highway bitch
	switch (item_ID)
	{
	case NieR::ItemId::ITEM_2P_BODY:    //2P
		return 5;
	case NieR::ItemId::ITEM_2B_KIMONO:  //2B Kimono
		return 6;
	case NieR::ItemId::ITEM_9P_BODY:    //9P
		return 3;
	case NieR::ItemId::ITEM_9S_KIMONO:  //9S Kimono
		return 4;
	case NieR::ItemId::ITEM_P2_BODY:    //P2
		return 7;
	case NieR::ItemId::ITEM_A2_KIMONO:  //A2 Kimono
		return 8;
	}

	return NieR::GetOutfitIDFromItemID(item_base, item_ID);
}

/*
Function optimized and decompiled odd, but basically only reaches the end if
a strcmp doesnt match (strcmp returns 0) and returns if the outfit is active

This is my refactored version
*/
__int64 hkUpdateEquippedActive(__int64 a1, __int64 item_id, int currentPlayer)
{
	//std::cout << "ItemID: " << item_id << " ResolvedName: " << NieR::ResolveNameFromItemID(modBase + 0x133b510, item_id) << "\n";

	const char* resolvedName = "";
	if (!NieR::ValidateNonCharacterSpecificEquippable(modBase + 0x133b510, item_id) ||
		currentPlayer > NieR::PlayerId::PID_A2 ||
		item_id == -1)
	{
		//std::cout << "Failed equippable outfit validation, valid=" << NieR::ValidateNonCharacterSpecificEquippable(modBase + 0x133b510, item_id)
		//		  << ", itemId=" << item_id
		//		  << ", currentPlayer=" << currentPlayer << "\n";
		return 0;
	}

	//2B
	if (currentPlayer == NieR::PlayerId::PID_2B)
	{
		resolvedName = NieR::ResolveNameFromItemID(modBase + 0x133b510, item_id);
		if (strcmp(resolvedName, "item_uq_changeArmour1") &&
			strcmp(resolvedName, "item_uq_changeArmour2") &&
			strcmp(resolvedName, "item_uq_dlcCloth1") &&
			strcmp(resolvedName, "item_uq_dlcOutfit1") &&
			strcmp(resolvedName, "item_uq_dlcOutfit2"))
		{
			return 0;
		}
	}

	//9S
	else if (currentPlayer == NieR::PlayerId::PID_9S)
	{
		resolvedName = NieR::ResolveNameFromItemID(modBase + 0x133b510, item_id);
		if (strcmp(resolvedName, "item_uq_dlcCloth2") &&
			strcmp(resolvedName, "item_uq_dlcOutfit3") &&
			strcmp(resolvedName, "item_uq_dlcOutfit4"))
		{
			return 0;
		}
	}

	//A2
	else if (currentPlayer == NieR::PlayerId::PID_A2)
	{
		resolvedName = NieR::ResolveNameFromItemID(modBase + 0x133b510, item_id);
		if (strcmp(resolvedName, "item_uq_dlcCloth3") &&
			strcmp(resolvedName, "item_uq_dlcOutfit5") &&
			strcmp(resolvedName, "item_uq_dlcOutfit6"))
		{
			return 0;
		}
	}

	if (NieR::sub_7c4b50() != 0)
	{
		//If this returns true, the outfit shows as ACTIVE

		/// @note NOTE: Since GetOutfitIDFromItemID() returns unexpected outfit IDs for the Switch DLC outfits
		/// we need to manually return the expected outfit IDs based on item ID for the new Switch DLC outfits
		/// and call the original function for every other outfit. We avoid hooking GetOutfitIDFromItemID()
		/// because there may be some additional logic that is performed in the original function that could
		/// cause the new Switch DLC outfits to fail the outfit validation at the top of this function.
		int outfitId = (item_id == NieR::ItemId::ITEM_2P_BODY)   ? NieR::OutfitId::OUTFIT_2B_2P_BODY :
					   (item_id == NieR::ItemId::ITEM_2B_KIMONO) ? NieR::OutfitId::OUTFIT_2B_KIMONO :
					   (item_id == NieR::ItemId::ITEM_9P_BODY)   ? NieR::OutfitId::OUTFIT_9S_9P_BODY :
					   (item_id == NieR::ItemId::ITEM_9S_KIMONO) ? NieR::OutfitId::OUTFIT_9S_KIMONO :
					   (item_id == NieR::ItemId::ITEM_P2_BODY)   ? NieR::OutfitId::OUTFIT_A2_P2_BODY :
					   (item_id == NieR::ItemId::ITEM_A2_KIMONO) ? NieR::OutfitId::OUTFIT_A2_KIMONO :
					   NieR::GetOutfitIDFromItemID(modBase + 0x133b510, item_id);
		int* activePlayerOutfits = (int*)(modBase + 0x1494354);

		//std::cout << "Outfit_ID: " << NieR::GetOutfitIDFromItemID(modBase + 0x133b510, item_id) << "\n";
		//std::cout << "OutFitEquipped: " << ((NieR::PlayerModelInfo*)NieR::GetPlayerFromPlayerNum(currentPlayer))->outfitEquipped << "\n";
		//std::cout << "ActiveOutfitId: " << activePlayerOutfits[currentPlayer] << "\n";

		return (outfitId == activePlayerOutfits[currentPlayer]);
	}

	return 0;
}

//TODO: DO THIS FOR ACCESSORIES
bool hkValidateAccessory(__int64 item_base, __int64 item_id, int currentPlayer)
{
	return true;
}

__int64 hkValidateDLCArmor(__int64 item_base, __int64 item_id, int currentPlayer)
{
	const char* resolved_name = "";
	const char* expected_name = "";

	if (item_id == -1 ||
		currentPlayer <= NieR::PlayerId::PID_INVALID ||
		currentPlayer > NieR::PlayerId::PID_A2)
	{
		return 0i64;
	}

	//2B
	if (currentPlayer == NieR::PlayerId::PID_2B)
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
		if (!strcmp(resolved_name, "item_uq_dlcCloth1"))
			return 1i64;
	}

	//9S
	else if (currentPlayer == NieR::PlayerId::PID_9S)
	{
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit3"))	//9P
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit4"))	//Kimono
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcCloth2"))
			return 1i64;
	}

	//A2
	else if (currentPlayer == NieR::PlayerId::PID_A2)
	{
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit5"))	//P2
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcOutfit6"))	//Kimono
			return 1i64;
		resolved_name = (const char*)NieR::ResolveNameFromItemID(item_base, item_id);
		if (!strcmp(resolved_name, "item_uq_dlcCloth3"))
			return 1i64;
	}

	return 0;
}

//If this returns 1, the "Use" button will instead show "Equip"
//NOTE: The outfit will still have to pass the "ValidateOutfit" to be clickable in the menu!
__int64 hkValidateNonSpecificCharacterEquippable(__int64 item_base, int item_id)
{
	if (item_id != -1)
	{
		const char* szName = NieR::ResolveNameFromItemID(item_base, item_id);

		// Base game outfits
		if (!strcmp(szName, "item_uq_changeArmour1"))
			return true;

		if (!strcmp(szName, "item_uq_changeArmour2"))
			return true;

		// NieR Replicant DLC outfits
		if (!strcmp(szName, "item_uq_dlcCloth1"))
			return true;

		if (!strcmp(szName, "item_uq_dlcCloth2"))
			return true;

		if (!strcmp(szName, "item_uq_dlcCloth3"))
			return true;

		// Switch DLC outfit additions
		if (!strcmp(szName, "item_uq_dlcOutfit1"))
			return true;

		if (!strcmp(szName, "item_uq_dlcOutfit2"))
			return true;

		if (!strcmp(szName, "item_uq_dlcOutfit3"))
			return true;

		if (!strcmp(szName, "item_uq_dlcOutfit4"))
			return true;

		if (!strcmp(szName, "item_uq_dlcOutfit5"))
			return true;

		if (!strcmp(szName, "item_uq_dlcOutfit6"))
			return true;
	}

	return false;
}

void SetMeshInvisible(NieR::PlayerModelInfo* pPlayerModelInfo, const char* mesh_name)
{
	if (pPlayerModelInfo->m_pModelData)
	{
		int part_index = NieR::SearchMeshGroupIndex(pPlayerModelInfo->m_pModelData, mesh_name);
		if (part_index >= 0 && part_index < pPlayerModelInfo->m_nParts)
		{
			NieR::CModelPart* pPart = &pPlayerModelInfo->m_pParts[part_index];
			if (pPart)
			{
				*(DWORD*)((byte*)pPart + 100) = 0;
			}
		}
	}
}

void SetMeshVisible(NieR::PlayerModelInfo* pPlayerModelInfo, const char* mesh_name)
{
	if (pPlayerModelInfo->m_pModelData)
	{
		int part_index = NieR::SearchMeshGroupIndex(pPlayerModelInfo->m_pModelData, mesh_name);
		if (part_index >= 0 && part_index < pPlayerModelInfo->m_nParts)
		{
			NieR::CModelPart* pPart = &pPlayerModelInfo->m_pParts[part_index];
			if (pPart)
			{
				*(DWORD*)((byte*)pPart + 100) = 1;
			}
		}
	}
}

//Yeah it's accessory time
//I don't want to make a proper decomp of this, so we are only handling our cases
__int64 hkUpdateAccessoryOnUnpause(NieR::PlayerModelInfo* pPlayerModelInfo)
{
	__int64 result = 0;
	__int64 accessory = 0;
	int resolvedAccessoryID = 0;

	__int64 v2 = NieR::sub_745c50((int*)&pPlayerModelInfo->unsigned_int170C0);
	if (v2)
		NieR::sub_7463c0(v2);

	pPlayerModelInfo->unsigned_int170C0 = 0;
	result = pPlayerModelInfo->accessoryEquipped - 1;

	switch (pPlayerModelInfo->accessoryEquipped)
	{
	case 0x1:
		resolvedAccessoryID = 0x4E001;
		result = NieR::SetMeshToGroup((__int64)modBase + 0x1029558, "Layout", 0x4E006, 0i64);	//last arg is some pointer
		if (result != 0)
		{
			//LAMBDA CALL
			*(int*)((char*)pPlayerModelInfo + 0x170c0) = *NieR::sub_745c10(result);
			result = NieR::sub_45a8c0(result);
			accessory = result;
			if (result != 0)
			{
				__int64 rcx_6 = *(__int64*)((char*)pPlayerModelInfo + 0x610);
				if (rcx_6 != 0)
				{
					*(int*)(accessory + 0x830) = *NieR::sub_745c10(rcx_6);
					int rax_7 = *(int*)(accessory + 0x5b8) - 0x4E003;
					LONG base = 0x30D;
					if (rax_7 > 9 || !_bittest(&base, rax_7))
					{
						int rsi_2 = 0;
						int arg_8 = *(int*)(accessory + 0x830);
						__int64* rax_9 = (__int64*)NieR::sub_3876a0((__int64*)&arg_8);
						__int64 rbp_1 = 0;
						if (rax_9 != 0)
						{
							int rax_10 = rax_9[0xB7];
							if (rax_10 == 0x10200 || rax_10 == 0x10203)
							{
								rsi_2 = 1;
								rbp_1 = 1;
							}
						}
						NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)(accessory + 0x390), 0);
						if (rsi_2 < *(int*)(accessory + 0x3a8))
						{
							__int64 rax_11 = (rbp_1 * 0x70);
							void* rax_12 = (void*)(rax_11 + *(__int64*)(accessory + 0x3a0));
							if (rax_11 != (-*(__int64*)(accessory + 0x3a0)))
							{
								*(int*)((char*)rax_12 + 0x64) = 1;
							}
						}
					}
				}
			}
		}

		//This sets the single mesh group to visible inside of the accessory mesh
		if (accessory != 0)
			*(int*)(accessory + 0x834) = 1;

		return pPlayerModelInfo->accessoryEquipped - 8;
	}
	return NieR::OriginalUpdateAccessoryOnUnpause(pPlayerModelInfo);
}

//Workaround ACTIVE not working by checking equipped outfit
__int64 hkSetEquippedFromPause(__int64 a1, int item_id)
{
	if (!a1)
	{
		return 0;
	}

	//2P's Body Replica (White)
	if (item_id == NieR::ItemId::ITEM_2P_BODY)
	{
		int playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;

		//Workaround
		if (v9->outfitEquipped == 4)
		{
			NieR::SetOutfitFromPause(v9, 0, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}

		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 4, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//Yorha Uniform 1 (Kimono)
	if (item_id == NieR::ItemId::ITEM_2B_KIMONO)
	{
		int playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;

		//Workaround
		if (v9->outfitEquipped == 5)
		{
			NieR::SetOutfitFromPause(v9, 0, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}

		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 5, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//9P (White 9S)
	if (item_id == NieR::ItemId::ITEM_9P_BODY)
	{
		int playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;

		//Workaround
		if (v9->outfitEquipped == 2)
		{
			NieR::SetOutfitFromPause(v9, 0, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}

		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 2, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//9S Kimono
	if (item_id == NieR::ItemId::ITEM_9S_KIMONO)
	{
		int playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;

		//Workaround
		if (v9->outfitEquipped == 3)
		{
			NieR::SetOutfitFromPause(v9, 0, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}

		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 3, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//P2's Body Replica
	if (item_id == NieR::ItemId::ITEM_P2_BODY)
	{
		int playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;

		//Workaround
		if (v9->outfitEquipped == 3)
		{
			NieR::SetOutfitFromPause(v9, 0, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}

		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 3, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	//YoRHA Uniform Prototype (NS_A2)
	if (item_id == NieR::ItemId::ITEM_A2_KIMONO)
	{
		int playernum = *(DWORD*)(modBase + 0x125025c);
		__int64 v7 = NieR::GetPlayerFromPlayerNum(NieR::sub_745c50(&playernum));
		NieR::PlayerModelInfo* v9 = (NieR::PlayerModelInfo*)v7;

		//Workaround
		if (v9->outfitEquipped == 2)
		{
			NieR::SetOutfitFromPause(v9, 0, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}

		if (v7)
		{
			NieR::SetOutfitFromPause(v9, 2, 1);
			*(int*)(modBase + 0x13fd4ac) = 1;
			return 1;
		}
	}

	return NieR::OriginalSetEquippedFromPause(a1, item_id);
}

//Hook
__int64 __fastcall HkManageMeshVisibilites(NieR::PlayerModelInfo* pPlayerModelInfo)
{
	const char* v15 = "";

	//2B
	if (pPlayerModelInfo->currentPlayer == 0x10000)
	{
		NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)&pPlayerModelInfo->gap0[0x390], 1);
		__int64 thing = (__int64)pPlayerModelInfo;
		if (pPlayerModelInfo->outfitEquipped == 1) //kaine outfit
		{
			SetMeshInvisible(pPlayerModelInfo, "Armor_Body");
			SetMeshInvisible(pPlayerModelInfo, "Armor_Head");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyemask");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Feather");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyelash");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "Broken");
			SetMeshInvisible(pPlayerModelInfo, "Body");
			SetMeshInvisible(pPlayerModelInfo, "Feather");
			SetMeshInvisible(pPlayerModelInfo, "Skirt");
		}

		else if (pPlayerModelInfo->outfitEquipped == 2) //full armor
		{
			NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)&pPlayerModelInfo->gap0[0x390], 0);
			SetMeshVisible(pPlayerModelInfo, "Armor_Body");
			SetMeshVisible(pPlayerModelInfo, "Armor_Head");
		}

		else if (pPlayerModelInfo->outfitEquipped == 3)	//just armor body
		{
			SetMeshInvisible(pPlayerModelInfo, "Armor_Head");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Broken");
			SetMeshInvisible(pPlayerModelInfo, "Body");
			SetMeshInvisible(pPlayerModelInfo, "Feather");
			SetMeshInvisible(pPlayerModelInfo, "Skirt");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyemask");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Feather");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyelash");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Skirt");
			v15 = "Broken";
		}

		else if (pPlayerModelInfo->outfitEquipped == 4)	//2P
		{
			// We could do this nicely and do activations instead of deactivations but it doesn't play as nice with the other code
			SetMeshInvisible(pPlayerModelInfo, "Armor_Body");
			SetMeshInvisible(pPlayerModelInfo, "Armor_Head");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			SetMeshInvisible(pPlayerModelInfo, "Broken");
			SetMeshInvisible(pPlayerModelInfo, "Body");
			SetMeshInvisible(pPlayerModelInfo, "Feather");
			SetMeshInvisible(pPlayerModelInfo, "Skirt");
		}

		else if (pPlayerModelInfo->outfitEquipped == 5) //Kimono
		{
			SetMeshInvisible(pPlayerModelInfo, "Armor_Body");
			SetMeshInvisible(pPlayerModelInfo, "Armor_Head");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyemask");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Feather");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyelash");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "Broken");
			SetMeshInvisible(pPlayerModelInfo, "Body");
			SetMeshInvisible(pPlayerModelInfo, "Feather");
			SetMeshInvisible(pPlayerModelInfo, "Skirt");
		}

		else  // normal
		{
			SetMeshInvisible(pPlayerModelInfo, "Armor_Body");
			SetMeshInvisible(pPlayerModelInfo, "Armor_Head");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Chest");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyemask");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Feather");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyelash");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Broken");
			SetMeshInvisible(pPlayerModelInfo, "NS_2P_Skirt");
		}


		if (pPlayerModelInfo->dwordisDestructed == 0)
		{
			//There are usually outfit checks here but it really doesn't matter, we arent saving frames reducing these calls
			if (pPlayerModelInfo->isBuddy == 0)
			{
				SetMeshInvisible(pPlayerModelInfo, "Broken");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Broken");
			}
			if (pPlayerModelInfo->isBuddy != 0)
			{
				SetMeshInvisible(pPlayerModelInfo, "Feather");
				SetMeshInvisible(pPlayerModelInfo, "Skirt");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Skirt");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Feather");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Skirt");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			}

			if (pPlayerModelInfo->dwordShowPants == 0)
			{
				SetMeshInvisible(pPlayerModelInfo, "Broken");
				SetMeshInvisible(pPlayerModelInfo, "Skirt");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Broken");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Skirt");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Skirt");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
			}

			if ((pPlayerModelInfo->dword16CEC) != 0)
			{
				if ((pPlayerModelInfo->accessoryEquipped) != 0xe && (pPlayerModelInfo->accessoryEquipped) != 0x3)
				{
					SetMeshInvisible(pPlayerModelInfo, "Eyelash");
					SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyelash");
				}

				//This doesn't seem to exist (anymore)(?)
				//set_mesh_invisible(pPlayerModelInfo, "Eyelash_serious");
			}

			if (((pPlayerModelInfo->dword16CEC != 0) && (pPlayerModelInfo->accessoryEquipped == 0xe) || (pPlayerModelInfo->accessoryEquipped == 0x3)))
			{
				SetMeshInvisible(pPlayerModelInfo, "Eyemask");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyemask");
			}

			if ((pPlayerModelInfo->dwordisFacialNormal) == 1)
			{
				SetMeshInvisible(pPlayerModelInfo, "facial_normal");
				SetMeshInvisible(pPlayerModelInfo, "Eyelash");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Normal");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Normal");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyelash");
			}
			else
			{
				SetMeshInvisible(pPlayerModelInfo, "facial_serious");
				SetMeshInvisible(pPlayerModelInfo, "Eyelash_serious");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_serious");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Serious");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyelash");
			}
		}
		else
		{
			NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)(&pPlayerModelInfo->gap0[0x390]), 1);
			if ((pPlayerModelInfo->dword178E0) == 0)
			{
				SetMeshInvisible(pPlayerModelInfo, "Armor_Head");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Skirt");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Broken");
				SetMeshInvisible(pPlayerModelInfo, "Body");
				SetMeshInvisible(pPlayerModelInfo, "Feather");
				SetMeshInvisible(pPlayerModelInfo, "Skirt");
				SetMeshInvisible(pPlayerModelInfo, "Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Skirt");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Chest");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Normal");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Serious");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Serious");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyemask");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Normal");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Feather");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Eyelash");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Body");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_2P_Skirt");
			}
			else
			{
				NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)(&pPlayerModelInfo->gap0[0x390]), 1);
				SetMeshVisible(pPlayerModelInfo, "Armor_Body");
				SetMeshVisible(pPlayerModelInfo, "Armor_Head");
			}

		}
		//TODO remove when mask accessory support is added
		SetMeshInvisible(pPlayerModelInfo, "Hair_Mask");
	}


	//A2
	if (pPlayerModelInfo->currentPlayer != 0x10000 ||
		(pPlayerModelInfo->currentPlayer == 0x10000 && pPlayerModelInfo->dwordisDestructed == 0))
	{
		const char* extra = "";
		if ((pPlayerModelInfo->currentPlayer) - 0x10100 <= 1)
		{
			NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)(&pPlayerModelInfo->gap0[0x390]), 1);
			// Destroyer Outfit
			if ((pPlayerModelInfo->outfitEquipped) == 1)
			{
				SetMeshInvisible(pPlayerModelInfo, "Body");
				SetMeshInvisible(pPlayerModelInfo, "Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Body");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Normal");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Serious");
			}

			// YoRHA Uniform Prototype
			else if (pPlayerModelInfo->outfitEquipped == 2)
			{
				SetMeshInvisible(pPlayerModelInfo, "Body");
				SetMeshInvisible(pPlayerModelInfo, "Cloth");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Body");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Normal");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Serious");
			}

			// P2's Body Replica
			else if (pPlayerModelInfo->outfitEquipped == 3)
			{
				SetMeshInvisible(pPlayerModelInfo, "Body");
				SetMeshInvisible(pPlayerModelInfo, "Cloth");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Serious");
			}

			// Normal
			else
			{
				SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
				SetMeshInvisible(pPlayerModelInfo, "DLC_Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Body");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Cloth");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Normal");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Serious");
			}

			if ((NieR::sub_745c50((int*)(&pPlayerModelInfo->unsigned_int17084)) != 0 && ((pPlayerModelInfo->accessoryEquipped) != 0xf)))
			{
				SetMeshInvisible(pPlayerModelInfo, "Hair");
			}

			//Mask
			if ((NieR::sub_745c50((int*)(&pPlayerModelInfo->unsigned_int17084)) != 0 && ((pPlayerModelInfo->accessoryEquipped) != 0x10))) //temporary value
			{
				SetMeshInvisible(pPlayerModelInfo, "Hair_Mask");
			}

			int var_30 = 0;
			int var_38 = 0;

			__int64* rax_101;
			rax_101 = (__int64*)NieR::sub_45a850(NieR::sub_745c50(NieR::EntityHandleCopy(&var_38, NieR::EntityHandleCopy(&var_30, (int*)((char*)pPlayerModelInfo + 0x17084)))));

			if (rax_101 != 0)
			{
				*(int*)((char*)rax_101 + 0x834) = *(int*)((char*)pPlayerModelInfo + 0x170bc) != 0xf;
			}


			if ((pPlayerModelInfo->outfitEquipped) != 1)
			{
				if ((pPlayerModelInfo->dwordShowPants) == 0)
				{
					SetMeshInvisible(pPlayerModelInfo, "Cloth");
					SetMeshInvisible(pPlayerModelInfo, "DLC_Cloth");
					SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Cloth");
					SetMeshInvisible(pPlayerModelInfo, "NS_P2_Cloth");
				}

				if (pPlayerModelInfo->outfitEquipped == 2)
				{
					SetMeshVisible(pPlayerModelInfo, "NS_KIMONO_Broken");
				}
			}

			if ((pPlayerModelInfo->dwordisFacialNormal) == 1)
			{
				SetMeshInvisible(pPlayerModelInfo, "facial_normal");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Normal");
			}

			else
			{
				SetMeshInvisible(pPlayerModelInfo, "facial_serious");
				SetMeshInvisible(pPlayerModelInfo, "NS_P2_Serious");
			}
		}
	}

	//9S
	int temp_106 = (pPlayerModelInfo->currentPlayer);
	const char* extra = "";
	if (temp_106 == 0x10200 || temp_106 == 0x10203)
	{
		NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)(&pPlayerModelInfo->gap0[0x390]), 1);

		//Normal
		if (pPlayerModelInfo->outfitEquipped == 0)
		{
			SetMeshInvisible(pPlayerModelInfo, "DLC_Pants");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Leg");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Eyemask");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Leg");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Pants");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Leg");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Pants");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken_Pants");
		}

		//Young Man Outfit
		if (pPlayerModelInfo->outfitEquipped == 1)
		{
			SetMeshInvisible(pPlayerModelInfo, "Pants");
			SetMeshInvisible(pPlayerModelInfo, "Body");
			SetMeshInvisible(pPlayerModelInfo, "Leg");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Eyemask");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Leg");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Pants");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Leg");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Pants");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken_Pants");
		}

		//9P (White 9S)
		if (pPlayerModelInfo->outfitEquipped == 2)
		{
			SetMeshInvisible(pPlayerModelInfo, "Pants");
			SetMeshInvisible(pPlayerModelInfo, "Body");
			SetMeshInvisible(pPlayerModelInfo, "Leg");
			SetMeshInvisible(pPlayerModelInfo, "Eyemask");	//this one got custom eyewear ray-ban
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Pants");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Leg");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Leg");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Pants");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Broken_Pants");
		}

		//Kimono
		else if (pPlayerModelInfo->outfitEquipped == 3)
		{
			SetMeshInvisible(pPlayerModelInfo, "Pants");
			SetMeshInvisible(pPlayerModelInfo, "Body");
			SetMeshInvisible(pPlayerModelInfo, "Leg");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Pants");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Body");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Leg");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Eyemask");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Body");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Leg");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Pants");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Serious");
		}

		int temp = pPlayerModelInfo->dword177D8 - 1;
		if (pPlayerModelInfo->dword177D8 == 1)
		{
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0201");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0202");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0206");
			SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			SetMeshInvisible(pPlayerModelInfo, "Pants");
			SetMeshInvisible(pPlayerModelInfo, "Leg");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Pants");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Leg");
		}

		else
		{
			if (temp == 1)
			{
				SetMeshInvisible(pPlayerModelInfo, "mesh_es0200");
				SetMeshInvisible(pPlayerModelInfo, "mesh_es0202");
				SetMeshInvisible(pPlayerModelInfo, "mesh_es0206");
				SetMeshInvisible(pPlayerModelInfo, "mesh_pl0200");
				SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0200");
				SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0206");
				SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0202");
				SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_pl0200");

				//idk this is in the decomp but set_mesh_invisible doesnt do anything if pWMB is 0 so
				//if (pPlayerModelInfo->pWMB == 0)
				//{
				//	set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_es0206");
				//	set_mesh_invisible(pPlayerModelInfo, "DLC_mesh_pl0200");
				//}
			}
			else
			{
				SetMeshInvisible(pPlayerModelInfo, "mesh_es0200");
				SetMeshInvisible(pPlayerModelInfo, "mesh_es0201");
				SetMeshInvisible(pPlayerModelInfo, "mesh_es0206");
				SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0200");
				SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0202");
			}
			if (temp == 3)
			{
				SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0206");
				SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_pl0200");
			}
			if (temp != 1)
			{
				if (temp != 2 && temp != 3)
				{
					SetMeshInvisible(pPlayerModelInfo, "mesh_es0200");
					SetMeshInvisible(pPlayerModelInfo, "mesh_es0201");
					SetMeshInvisible(pPlayerModelInfo, "mesh_es0202");
					SetMeshInvisible(pPlayerModelInfo, "mesh_es0206");
					SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0200");
					SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0201");
					SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0202");
					SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0206");
					SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
					SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
					SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
					SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0206");
					SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
					SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
					SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
					SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0206");
				}
				if (temp == 1)
				{
					SetMeshInvisible(pPlayerModelInfo, "mesh_es0200");
					SetMeshInvisible(pPlayerModelInfo, "mesh_es0201");
					SetMeshInvisible(pPlayerModelInfo, "mesh_es0202");
					SetMeshInvisible(pPlayerModelInfo, "mesh_pl0200");
					SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0200");
					SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0201");
					SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_es0202");
					SetMeshInvisible(pPlayerModelInfo, "DLC_mesh_pl0200");
					SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0200");
					SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0201");
					SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_es0202");
					SetMeshInvisible(pPlayerModelInfo, "NS_9P_Mesh_pl0200");
					SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0200");
					SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0201");
					SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_es0202");
					SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Mesh_pl0200");
				}
			}
		}
		if (pPlayerModelInfo->isBuddy)
		{
			if (pPlayerModelInfo->dword16CE0 != 2)
			{
				SetMeshInvisible(pPlayerModelInfo, "Pants");
			}
			SetMeshInvisible(pPlayerModelInfo, "DLC_Pants");
		}

		//dress module lol
		if (!pPlayerModelInfo->dwordShowPants)
		{
			SetMeshInvisible(pPlayerModelInfo, "Pants");
			SetMeshInvisible(pPlayerModelInfo, "DLC_Pants");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Pants");
			SetMeshInvisible(pPlayerModelInfo, "NS_KIMONO_Pants");
		}

		char buffer[256];
		__int64 var_18_1 = 0;
		sprintf_s(buffer, 0x10, "PL/9S_NoPatch");

		//count size of string inside buffer

		__int64 count = strlen(buffer);

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
				SetMeshInvisible(pPlayerModelInfo, "Eyelash");
				if (pPlayerModelInfo != 0)
				{
					temp_20 = "Eyelash_serious";
					SetMeshInvisible(pPlayerModelInfo, temp_20);
				}
			}
		}

		//TODO bullshit if
		if ((((pPlayerModelInfo->dword16CEC) == 0 || ((pPlayerModelInfo->dword16CEC) != 0 && pPlayerModelInfo->accessoryEquipped == 0xe)) || (((pPlayerModelInfo->dword16CEC) != 0 && pPlayerModelInfo->accessoryEquipped != 0xe) && temp_221 != 0)))
		{
			SetMeshInvisible(pPlayerModelInfo, "Eyemask");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Eyemask");
		}

		//Camouflage Goggles
		if (pPlayerModelInfo->accessoryEquipped == 3)
		{
			SetMeshInvisible(pPlayerModelInfo, "Eyemask");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Eyemask");
		}

		const char* temp_21 = "";
		if (pPlayerModelInfo->dwordisFacialNormal == 1)
		{
			SetMeshInvisible(pPlayerModelInfo, "facial_normal");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Normal");

			if (pPlayerModelInfo->m_pModelData)
			{
				temp_21 = "Eyelash";
			}
		}
		else
		{
			SetMeshInvisible(pPlayerModelInfo, "facial_serious");
			SetMeshInvisible(pPlayerModelInfo, "NS_9P_Serious");

			if (pPlayerModelInfo->m_pModelData)
			{
				temp_21 = "Eyelash_serious";
			}
		}

		if ((pPlayerModelInfo->dwordisFacialNormal == 1 && pPlayerModelInfo->m_pModelData) || (pPlayerModelInfo->dwordisFacialNormal != 1 && pPlayerModelInfo->m_pModelData))
		{
			SetMeshInvisible(pPlayerModelInfo, temp_21);
		}

		if (((pPlayerModelInfo->currentPlayer - 0x10500) & 0xfffffeff) == 0)
		{
			NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)&pPlayerModelInfo->gap0[0x390], 1);
			if (pPlayerModelInfo->dwordisFacialNormal == 1 && pPlayerModelInfo->m_pModelData)
			{
				SetMeshInvisible(pPlayerModelInfo, "facial_normal");
			}
			if (pPlayerModelInfo->dwordisFacialNormal != 1 && pPlayerModelInfo->m_pModelData)
			{
				SetMeshInvisible(pPlayerModelInfo, "facial_serious");
			}

			SetMeshInvisible(pPlayerModelInfo, "Clothing_Sit");
		}
	}

	//the return code is weird just using this as default maybe itll end the world idk
	__int64 result = 0x10000;
	if (((*(int8_t*)(modBase + 0x10297F0)) & 0x20) == 0 || ((*(int8_t*)(modBase + 0x10297F0)) & 20) != 0 && (pPlayerModelInfo->dword10628 == 0))
	{
		if (pPlayerModelInfo->unsigned_int17084 != 0)
		{
			__int64* tmp1 = 0;
			__int64* tmp2 = 0;
			__int64* temp_247 = (__int64*)NieR::sub_45a850(
				NieR::sub_745c50(
					(int*)lambda_meme((__int64*)&tmp2, (int*)lambda_meme((__int64*)&tmp1, (int*)((__int64)pPlayerModelInfo + 0x17084)))));
			if (temp_247 != 0)
			{
				NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)&temp_247[0x72], 1);
			}

			tmp1 = 0;
			tmp2 = 0;
			__int64* temp_251 = (__int64*)NieR::sub_45a850(
				NieR::sub_745c50(
					(int*)lambda_meme((__int64*)&tmp2, (int*)lambda_meme((__int64*)&tmp1, (int*)((__int64)pPlayerModelInfo + 0x170c0)))));
			if (temp_251 != 0)
			{
				*(int*)(temp_251 + 0x834) = 1;
			}
		}
	}

	if (((*(int8_t*)(modBase + 0x10297F0)) & 0x20) != 0 && (pPlayerModelInfo->dword10628 != 0))
	{
		NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)&pPlayerModelInfo->gap0[0x390], 0);
		__int64* temp_241 = (__int64*)NieR::sub_491170((int*)&pPlayerModelInfo->unsigned_int17084);
		if (temp_241 != 0)
		{
			NieR::SetDrawBasePlayerMeshes((NieR::CModelWork*)&temp_241[0x72], 0);
		}
		__int64 temp_243 = NieR::sub_491400((int*)&pPlayerModelInfo->unsigned_int170C0);
		if (temp_243 != 0)
		{
			*((int*)(temp_243 + 0x834)) = 0;
			result = *(DWORD*)(result + 2100);
		}
	}
	pPlayerModelInfo->dword10628 = 0;
	return 0x10000;
}

//Woeful_Wolf's Limit Break
//https://docs.google.com/spreadsheets/d/1rGSfrN9eiRGONrDsQKA0A6bI5_EVO-NmpLg8WuwNEyg/edit#gid=715728971
/*
void LimitBreak()
{
	//Lazy instruction byteswap, this should be replaced with hooks (it its own plugin even)
	//This is protected executable memory, so we have to VirtualProtect everything

	DWORD oldProtect = NULL; //One var, reduce my typing :)

	//FILE ROOT
	VirtualProtect((LPVOID)(modBase + 0x86A9E5), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86A9E5) = 0x29E20000;
	VirtualProtect((LPVOID)(modBase + 0x86A9E5), sizeof(__int64), oldProtect, &oldProtect);

	//TEXTURE ROOT
	VirtualProtect((LPVOID)(modBase + 0x86AA0B), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86AA0B) = 0xF74C0000;
	VirtualProtect((LPVOID)(modBase + 0x86AA0B), sizeof(__int64), oldProtect, &oldProtect);

	//PL FILE
	VirtualProtect((LPVOID)(modBase + 0x86AC05), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86AC05) = 0x6E40000;
	VirtualProtect((LPVOID)(modBase + 0x86AC05), sizeof(__int64), oldProtect, &oldProtect);

	//EM+BG FILE
	VirtualProtect((LPVOID)(modBase + 0x86AC53), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86AC53) = 0x8400000;
	VirtualProtect((LPVOID)(modBase + 0x86AC53), sizeof(__int64), oldProtect, &oldProtect);

	//UI FILE
	VirtualProtect((LPVOID)(modBase + 0x86AD16), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86AD16) = 0xF00000;
	VirtualProtect((LPVOID)(modBase + 0x86AD16), sizeof(__int64), oldProtect, &oldProtect);

	//UIFont FILE
	VirtualProtect((LPVOID)(modBase + 0x86AD3D), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86AD3D) = 0xA00000;
	VirtualProtect((LPVOID)(modBase + 0x86AD3D), sizeof(__int64), oldProtect, &oldProtect);

	//PL VRAM
	VirtualProtect((LPVOID)(modBase + 0x86AE9C), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86AE9C) = 0x2A800000;
	VirtualProtect((LPVOID)(modBase + 0x86AE9C), sizeof(__int64), oldProtect, &oldProtect);

	//EM+BG VRAM
	VirtualProtect((LPVOID)(modBase + 0x86AEEA), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86AEEA) = 0x47880000;
	VirtualProtect((LPVOID)(modBase + 0x86AEEA), sizeof(__int64), oldProtect, &oldProtect);

	//UI VRAM
	VirtualProtect((LPVOID)(modBase + 0x86AFD4), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86AFD4) = 0xF000000;
	VirtualProtect((LPVOID)(modBase + 0x86AFD4), sizeof(__int64), oldProtect, &oldProtect);

	//UIFont VRAM
	VirtualProtect((LPVOID)(modBase + 0x86AFFB), sizeof(__int64), PAGE_EXECUTE_READWRITE, &oldProtect);
	*(__int64*)(modBase + 0x86AFFB) = 0x15000000;
	VirtualProtect((LPVOID)(modBase + 0x86AFFB), sizeof(__int64), oldProtect, &oldProtect);
}
*/

void InitializeFunctionPointers()
{
	NieR::ManageMeshVisibilities = (NieR::FnManageMeshVisibilities)(modBase + 0x51B940);
	NieR::SearchMeshGroupIndex = (NieR::FnSearchMeshGroupIndex)(modBase + 0x1910B0);
	NieR::UpdateAccessoryOnUnpause = (NieR::FnUpdateAccessoryOnUnpause)(modBase + 0x482360);
	NieR::UpdateEquippedActive = (NieR::FnUpdateEquippedActive)(modBase + 0x7f50f0);
	NieR::ValidateAccessory = (NieR::FnValidateAccessory)(modBase + 0x7f4830);
	NieR::ValidateDLCArmor = (NieR::FnValidateDLCArmor)(modBase + 0x7f4770);
	NieR::ValidatePodSkins = (NieR::FnValidatePodSkins)(modBase + 0x7f6d90);
	NieR::ValidateNonCharacterSpecificEquippable = (NieR::FnValidateNonCharacterSpecificEquippable)(modBase + 0x7f4020);
	NieR::AnotherAccessoryThing = (NieR::FnAnotherAccessoryThing)(modBase + 0x4eca60);
	NieR::SetOutfitFromPause = (NieR::FnSetOutfitFromPause)(modBase + 0x475eb0);
	NieR::SetEquippedFromPause = (NieR::FnSetEquippedFromPause)(modBase + 0x843ee0);
	NieR::ResolveNameFromItemID = (NieR::FnResolveNameFromItemID)(modBase + 0x7e35c0);
	NieR::GetAccessoryIDFromItemID = (NieR::FnGetAccessoryIDFromItemID)(modBase + 0x7d4310);
	NieR::GetOutfitIDFromItemID = (NieR::FnGetOutfitIDFromItemID)(modBase + 0x7d42a0);
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
	NieR::SetMeshToGroup = (NieR::FnSetMeshToGroup)(modBase + 0x7459b0);
	NieR::sub_745c10 = (NieR::_sub_745c10)(modBase + 0x745c10);
	NieR::sub_7e6e60 = (NieR::_sub_7e6e60)(modBase + 0x7e6e60);
	NieR::GetPlayerFromPlayerNum = (NieR::FnGetPlayerFromPlayerNum)(modBase + 0x3084c0);
	NieR::sub_7c4b50 = (NieR::_sub_7c4b50)(modBase + 0x7c4b50);
	NieR::sub_7c4b90 = (NieR::_sub_7c4b90)(modBase + 0x7c4b90);
	NieR::sub_7c9cb0 = (NieR::_sub_7c9cb0)(modBase + 0x7c9cb0);
	NieR::SetDrawBasePlayerMeshes = (NieR::FnSetDrawBasePlayerMeshes)(modBase + 0x197C70);
	NieR::EntityHandleCopy = (NieR::FnEntityHandleCopy)(modBase + 0x744fa0);
}

void ConsoleSetup()
{
	//AllocConsole();
	//SetConsoleTitle(TEXT("NASA Console"));
	

	freopen_s(&stream, "log.txt", "w", stdout);
	freopen_s(&stream, "log.txt", "w", stderr);
	freopen_s(&stream, "log.txt", "r", stdin);
}

int Main(PVOID lpParameter)
{
	//Thanks wolf!
#ifdef DEBUG
	ConsoleSetup();
#endif // DEBUG

	modBase = (uintptr_t)GetModuleHandle(NULL);
	InitializeFunctionPointers();
	//LimitBreak();

	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}

	/*
	if (MH_CreateHook(NieR::UpdateAccessoryOnUnpause, &hkUpdateAccessoryOnUnpause, reinterpret_cast<LPVOID*>(&NieR::fpUpdateAccessoryOnUnpause)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::UpdateAccessoryOnUnpause) != MH_OK) return 1;
	*/

	if (MH_CreateHook(NieR::UpdateEquippedActive, &hkUpdateEquippedActive, NULL) != MH_OK) return 1;
	if (MH_EnableHook(NieR::UpdateEquippedActive) != MH_OK) return 1;

	if (MH_CreateHook(NieR::ManageMeshVisibilities, &HkManageMeshVisibilites, reinterpret_cast<LPVOID*>(&NieR::OriginalManageMeshVisibilities)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::ManageMeshVisibilities) != MH_OK) return 1;

	if (MH_CreateHook(NieR::ValidateDLCArmor, &hkValidateDLCArmor, reinterpret_cast<LPVOID*>(&NieR::OriginalValidateDLCArmor)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::ValidateDLCArmor) != MH_OK) return 1;

	if (MH_CreateHook(NieR::ValidateNonCharacterSpecificEquippable, &hkValidateNonSpecificCharacterEquippable, reinterpret_cast<LPVOID*>(&NieR::OriginalValidateNonCharacterSpecificEquippable)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::ValidateNonCharacterSpecificEquippable) != MH_OK) return 1;

	/*
	if (MH_CreateHook(NieR::GetOutfitIDFromItemID, &hkGetOutfitIDFromItemID, reinterpret_cast<LPVOID*>(&NieR::fpGetOutfitIDFromItemID)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::GetOutfitIDFromItemID) != MH_OK) return 1;
	*/

	if (MH_CreateHook(NieR::SetEquippedFromPause, &hkSetEquippedFromPause, reinterpret_cast<LPVOID*>(&NieR::OriginalSetEquippedFromPause)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::SetEquippedFromPause) != MH_OK) return 1;

	/*
	if (MH_CreateHook(NieR::SetMeshToGroup, &hkSetAccessory, reinterpret_cast<LPVOID*>(&NieR::fpSetMeshToGroup)) != MH_OK) return 1;
	if (MH_EnableHook(NieR::SetMeshToGroup) != MH_OK) return 1;
	*/

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
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
