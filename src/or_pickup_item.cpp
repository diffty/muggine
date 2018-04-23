//
//  or_pickup_item.cpp
//  K-Mion
//
//  Created by DiFFtY on 22/04/2018.
//  Copyright © 2018 RIX. All rights reserved.
//

#include "or_pickup_item.hpp"
#include "or_game_mode.hpp"


// ORPickupItem
ORPickupItem::ORPickupItem(Image* pItemImg) : Sprite(pItemImg, {0, 0}) {}
void ORPickupItem::update() {}


// ORPickupItemHouse
ORPickupItemHouse::ORPickupItemHouse() : ORPickupItem(RscManager::get()->getImgRsc(11)) {}
void ORPickupItemHouse::onPickup() { ORGameMode::get()->onHouseItemPicked(this); }


// ORPickupItemIndustry
ORPickupItemIndustry::ORPickupItemIndustry() : ORPickupItem(RscManager::get()->getImgRsc(12)) {}
void ORPickupItemIndustry::onPickup() { ORGameMode::get()->onIndustryItemPicked(this); }


// ORPickupItemEcology
ORPickupItemEcology::ORPickupItemEcology() : ORPickupItem(RscManager::get()->getImgRsc(10)) {}
void ORPickupItemEcology::onPickup() { ORGameMode::get()->onEcologyItemPicked(this); }


// ORPickupItemTomato
ORPickupItemTomato::ORPickupItemTomato() : ORPickupItem(RscManager::get()->getImgRsc(18)) {}
void ORPickupItemTomato::onPickup() { ORGameMode::get()->onTomatoItemPicked(this); }

