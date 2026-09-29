// Gondos Daniel all rights reserved.


#include "FrameworkClasses/GD_PlayerStateBase.h"

#include "Characters/CharacterComponents/GD_InventoryComponent.h"


AGD_PlayerStateBase::AGD_PlayerStateBase()
{
	Inventory = CreateDefaultSubobject<UGD_InventoryComponent>(TEXT("Inventory"));
}