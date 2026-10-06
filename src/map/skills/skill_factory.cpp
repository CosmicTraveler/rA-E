// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "skill_factory.hpp"

#include <memory>
#include <vector>

// map-server-generator does not need concrete skill implementations
// This will save compile time
#ifndef MAP_GENERATOR

// Include job factory headers for the create() dispatcher
#include "skill_acolyte.hpp"
#include "skill_archer.hpp"
#include "skill_custom.hpp"
#include "skill_elemental.hpp"
#include "skill_gunslinger.hpp"
#include "skill_homunculus.hpp"
#include "skill_mage.hpp"
#include "skill_mercenary.hpp"
#include "skill_merchant.hpp"
#include "skill_npc.hpp"
#include "skill_ninja.hpp"
#include "skill_novice.hpp"
#include "skill_other.hpp"
#include "skill_summoner.hpp"
#include "skill_swordman.hpp"
#include "skill_taekwon.hpp"
#include "skill_thief.hpp"

std::unique_ptr<const SkillImpl> SkillFactoryImpl::create(const e_skill skill_id) const {
	static const std::vector<std::shared_ptr<SkillFactory>> factories = {
		// Custom Skills (Always first to allow overwriting skills)
		std::make_shared<SkillFactoryCustom>(),
		// Normal Skills
		std::make_shared<SkillFactoryAcolyte>(),
		std::make_shared<SkillFactoryArcher>(),
		std::make_shared<SkillFactoryElemental>(),
		std::make_shared<SkillFactoryGunslinger>(),
		std::make_shared<SkillFactoryHomunculus>(),
		std::make_shared<SkillFactoryMage>(),
		std::make_shared<SkillFactoryMercenary>(),
		std::make_shared<SkillFactoryMerchant>(),
		std::make_shared<SkillFactoryNinja>(),
		std::make_shared<SkillFactoryNpc>(),
		std::make_shared<SkillFactoryNovice>(),
		std::make_shared<SkillFactoryOther>(),
		std::make_shared<SkillFactorySummoner>(),
		std::make_shared<SkillFactorySwordman>(),
		std::make_shared<SkillFactoryTaekwon>(),
		std::make_shared<SkillFactoryThief>(),
	};

	for (const std::shared_ptr<SkillFactory>& factory : factories) {
		if (std::unique_ptr<const SkillImpl> impl = factory->create(skill_id); impl != nullptr) {
			return impl;
		}
	}

	return nullptr;
}

#else

std::unique_ptr<const SkillImpl> SkillFactoryImpl::create(const e_skill skill_id) const {
	return nullptr;
}

#endif
