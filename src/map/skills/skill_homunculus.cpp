// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_homunculus.hpp"

#include "map/status.hpp"
#include "map/clif.hpp"
#include <array>
#include <common/random.hpp>
#include "map/battle.hpp"
#include "map/homunculus.hpp"
#include "map/map.hpp"
#include "map/pc.hpp"
#include "map/unit.hpp"
#include <config/core.hpp>
#include "map/mob.hpp"
#include "skill_impl.hpp"

SkillAbsoluteZephyr::SkillAbsoluteZephyr() : SkillImplRecursiveDamageSplash(MH_ABSOLUTE_ZEPHYR) {
}

void SkillAbsoluteZephyr::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 1000 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->int_; // !TODO: Confirm Base Level and INT bonus
}

SkillAvoid::SkillAvoid() : SkillImpl(HLIF_AVOID) {
}

void SkillAvoid::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	// Master
	sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	// Homunculus
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv, sc_start(src, src, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillBenedictionOfChaos::SkillBenedictionOfChaos() : SkillImpl(HVAN_CHAOTIC) {
}

void SkillBenedictionOfChaos::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Chance per skill level
	static const std::array<uint8, 5> chance_homunculus = {
		20,
		50,
		25,
		50,
		34
	};
	static const std::array<uint8, 5> chance_master = {
		static_cast<uint8>(chance_homunculus[0] + 30),
		static_cast<uint8>(chance_homunculus[1] + 10),
		static_cast<uint8>(chance_homunculus[2] + 50),
		static_cast<uint8>(chance_homunculus[3] + 4),
		static_cast<uint8>(chance_homunculus[4] + 33)
	};

	uint8 chance = rnd_value(1, 100);

	// Homunculus
	if (chance <= chance_homunculus[skill_lv - 1]) {
		target = src;
	// Master
	} else if (chance <= chance_master[skill_lv - 1]) {
		target = battle_get_master(src);
	// Enemy (A random enemy targeting the master)
	} else {
		target = battle_gettargeted(battle_get_master(src));
	}

	// If there's no enemy the chance reverts to the homunculus
	if (target == nullptr) {
		target = src;
	}

	int32 heal = skill_calc_heal(src, target, getSkillId(), rnd_value<uint16>(1, skill_lv), true);

	// Official servers send the Heal skill packet with the healed amount, and then the skill packet with 1 as healed amount
	clif_skill_nodamage(src, *target, AL_HEAL, heal);
	clif_skill_nodamage(src, *target, getSkillId(), 1);
	status_heal(target, heal, 0, 0);
}

SkillBioExplosion::SkillBioExplosion() : SkillImpl(HVAN_EXPLOSION) {
}

void SkillBioExplosion::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	homun_data* hd = BL_CAST(BL_HOM, src);

	if (hd != nullptr) {
		clif_skill_nodamage(src, *src, getSkillId(), skill_lv, 1);
		map_foreachinshootrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR | BL_SKILL, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY, skill_castend_damage_id);

		hd->homunculus.intimacy = hom_intimacy_grade2intimacy(HOMGRADE_HATE_WITH_PASSION);
		clif_send_homdata(*hd, SP_INTIMATE);

		// There's a delay between the explosion and the homunculus death
		skill_addtimerskill(src, tick + skill_get_time(getSkillId(), skill_lv), src->id, 0, 0, getSkillId(), skill_lv, 0, flag);
	}
}

void SkillBioExplosion::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (src != target) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	}
}

SkillBlastForge::SkillBlastForge() : SkillImpl(MH_BLAST_FORGE) {
}

void SkillBlastForge::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;
	// Ammo should be deleted right away.
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillBlastForge::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 70 * skill_lv * status_get_lv(src) / 100 + sstatus->str;
}

SkillBlazingAndFurious::SkillBlazingAndFurious() : SkillImplRecursiveDamageSplash(MH_BLAZING_AND_FURIOUS) {
}

void SkillBlazingAndFurious::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const homun_data *hd = BL_CAST(BL_HOM, &src);

	if (hd != nullptr) {
		dmg.div_ = hd->homunculus.spiritball;
	}
}

void SkillBlazingAndFurious::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	/* Check if the target is an enemy; if not, skill should fail so the character doesn't unit_movepos (exploitable) */
	if( battle_check_target(src, target, BCT_ENEMY) > 0 ) {
		if( unit_movepos(src, target->x, target->y, 2, 1) ) {
			skill_attack(BF_WEAPON,src,src,target,getSkillId(),skill_lv,tick,flag);
			clif_blown(src);
		}
	}else if( sd ){
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
	}
}

void SkillBlazingAndFurious::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 80 * skill_lv * status_get_lv(src) / 100 + sstatus->str;
}

SkillCaprice::SkillCaprice() : SkillImpl(HVAN_CAPRICE) {
}

void SkillCaprice::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	static const std::array<e_skill, 4> subskills = { MG_COLDBOLT, MG_FIREBOLT, MG_LIGHTNINGBOLT, WZ_EARTHSPIKE };
	e_skill subskill_id = subskills.at(rnd() % subskills.size());
	skill_attack(skill_get_type(subskill_id), src, src, target, subskill_id, skill_lv, tick, flag);
}

SkillCastling::SkillCastling() : SkillImpl(HAMI_CASTLE) {
}

void SkillCastling::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (src != target && rnd_chance(20 * skill_lv, 100)) {
		// Get one of the monsters targeting the player and set the homunculus as its new target
		if (block_list* tbl = battle_gettargeted(target); tbl != nullptr && tbl->type == BL_MOB) {
			if (unit_data* ud = unit_bl2ud(tbl); ud != nullptr) {
				unit_changetarget_sub(*ud, *src);
			}
		}

		int16 x = src->x, y = src->y;
		// Move homunculus
		if (unit_movepos(src, target->x, target->y, 0, false)) {
			clif_blown(src);
			// Move player
			if (unit_movepos(target, x, y, 0, false)) {
				clif_blown(target);
			}
			// Show the animation on the homunculus only
			clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
		}
	} else if (homun_data* hd = BL_CAST(BL_HOM, src); hd != nullptr && hd->master != nullptr) {
		clif_skill_fail(*hd->master, getSkillId());
	} else if (map_session_data* sd = BL_CAST(BL_PC, target); sd != nullptr) {
		clif_skill_fail(*sd, getSkillId());
	}
}

SkillChange::SkillChange() : StatusSkillImpl(HLIF_CHANGE) {
}

void SkillChange::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	status_percent_heal(target, 100, 100);
#endif

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillContinualBreakCombo::SkillContinualBreakCombo() : SkillImpl(MH_CBC) {
}

void SkillContinualBreakCombo::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 duration = max(skill_lv, (status_get_str(src) / 7 - status_get_str(target) / 10)) * 1000; //Yommy formula

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start4(src, target, SC_CBC, 100, skill_lv, src->id, 0, 0, duration));
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillDefense::SkillDefense() : SkillImpl(HAMI_DEFENCE) {
}

void SkillDefense::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	// Master
	sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	// Homunculus
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv, sc_start(src, src, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillEraserCutter::SkillEraserCutter() : SkillImpl(MH_ERASER_CUTTER) {
}

void SkillEraserCutter::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillEraserCutter::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->int_; // !TODO: Confirm Base Level and INT bonus
}

SkillEternalQuickCombo::SkillEternalQuickCombo() : SkillImpl(MH_EQC) {
}

void SkillEternalQuickCombo::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 duration = max(skill_lv, (status_get_str(src) / 7 - status_get_str(target) / 10)) * 1000; //Yommy formula

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start4(src, target, SC_EQC, 100, skill_lv, src->id, 0, 0, duration));
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillEternalQuickCombo::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	homun_data* hd = BL_CAST(BL_HOM, src);

	if (hd) {
		sc_start2(src, target, SC_STUN, 100, skill_lv, target->id, 1000 * hd->homunculus.level / 50 + 500 * skill_lv);
		status_change_end(target, SC_TINDER_BREAKER2);
	}
}

SkillGlanzenSpies::SkillGlanzenSpies() : SkillImpl(MH_GLANZEN_SPIES) {
}

void SkillGlanzenSpies::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillGlanzenSpies::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 300 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->vit; // !TODO: Confirm VIT bonus
}

SkillGoldeneTone::SkillGoldeneTone() : SkillImpl(MH_GOLDENE_TONE) {
}

void SkillGoldeneTone::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const sc_type type = skill_get_sc(getSkillId());
	block_list* master_bl = battle_get_master(src);

	if (master_bl != nullptr) {
		clif_skill_nodamage(src, *master_bl, getSkillId(), skill_lv);
		sc_start(src, master_bl, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
}

SkillGraniticArmor::SkillGraniticArmor() : SkillImpl(MH_GRANITIC_ARMOR) {
}

void SkillGraniticArmor::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const sc_type type = skill_get_sc(getSkillId());
	homun_data* hd = BL_CAST(BL_HOM, src);

	if (hd) {
		block_list* s_bl = battle_get_master(src);
		if (s_bl) {
			sc_start2(src, s_bl, type, 100, skill_lv, hd->homunculus.level, skill_get_time(getSkillId(), skill_lv)); //start on master
		}
		sc_start2(src, target, type, 100, skill_lv, hd->homunculus.level, skill_get_time(getSkillId(), skill_lv));
	}
}

SkillHealingTouch::SkillHealingTouch() : SkillImpl(HLIF_HEAL) {
}

void SkillHealingTouch::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_change* tsc = status_get_sc(target);
	status_data* sstatus = status_get_status_data(*src);

	int32 heal = skill_calc_heal(src, target, getSkillId(), skill_lv, true);

	if (status_isimmune(target) || (dstmd && (status_get_class(target) == MOBID_EMPERIUM || status_get_class_(target) == CLASS_BATTLEFIELD))) {
		heal = 0;
	}

	if (tsc != nullptr && !tsc->empty()) {
		if (tsc->getSCE(SC_KAITE) && !status_has_mode(sstatus, MD_STATUSIMMUNE)) { // Bounce back heal
			if (--tsc->getSCE(SC_KAITE)->val2 <= 0) {
				status_change_end(target, SC_KAITE);
			}
			if (src == target) {
				heal = 0; // When you try to heal yourself under Kaite, the heal is voided.
			} else {
				target = src;
				dstsd = sd;
			}
		} else if (tsc->getSCE(SC_BERSERK) || tsc->getSCE(SC_SATURDAYNIGHTFEVER)) {
			heal = 0; // Needed so that it actually displays 0 when healing.
		}
	}

	status_change_end(target, SC_BITESCAR);
	clif_skill_nodamage(src, *target, getSkillId(), heal);
	t_exp heal_get_jobexp = status_heal(target, heal, 0, 0);

	if (sd && dstsd && heal > 0 && sd != dstsd && battle_config.heal_exp > 0) {
		heal_get_jobexp = heal_get_jobexp * battle_config.heal_exp / 100;
		if (heal_get_jobexp <= 0) {
			heal_get_jobexp = 1;
		}
		pc_gainexp(sd, target, 0, heal_get_jobexp, 0);
	}
}

SkillHeiligePferd::SkillHeiligePferd() : SkillImplRecursiveDamageSplash(MH_HEILIGE_PFERD) {
}

void SkillHeiligePferd::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillHeiligePferd::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 1200 + 350 * skill_lv * status_get_lv(src) / 100 + sstatus->vit; // !TODO: Confirm VIT bonus
}

SkillHolyPole::SkillHolyPole() : SkillImplRecursiveDamageSplash(MH_HEILIGE_STANGE) {
}

void SkillHolyPole::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 1500 + 250 * skill_lv * status_get_lv(src) / 150 + sstatus->vit; // !TODO: Confirm VIT bonus
}

SkillLavaSlide::SkillLavaSlide() : SkillImpl(MH_LAVA_SLIDE) {
}

void SkillLavaSlide::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;
	// Ammo should be deleted right away.
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillLavaSlide::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 50 * skill_lv;
}

SkillLightOfRegene::SkillLightOfRegene() : SkillImpl(MH_LIGHT_OF_REGENE) {
}

void SkillLightOfRegene::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const sc_type type = skill_get_sc(getSkillId());
	homun_data* hd = BL_CAST(BL_HOM, src);

	if (hd == nullptr) {
		return;
	}

	block_list* s_bl = battle_get_master(src);
	if (s_bl != nullptr) {
		sc_start(src, s_bl, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
	sc_start2(src, src, type, 100, skill_lv, hd->homunculus.level, skill_get_time(getSkillId(), skill_lv));
}

SkillMagmaFlow::SkillMagmaFlow() : SkillImplRecursiveDamageSplash(MH_MAGMA_FLOW) {
}

void SkillMagmaFlow::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const sc_type type = skill_get_sc(getSkillId());

	sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillMagmaFlow::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if ((flag & 1) && ((rnd() % 100) > (3 * skill_lv))) {
		return; // chance to not trigger atk
	}

	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillMagmaFlow::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += -100 + (100 * skill_lv + 3 * status_get_lv(src)) * status_get_lv(src) / 120;
}

SkillMidnightFrenzy::SkillMidnightFrenzy() : SkillImpl(MH_MIDNIGHT_FRENZY) {
}

void SkillMidnightFrenzy::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillMidnightFrenzy::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 450 * skill_lv * status_get_lv(src) / 150 + sstatus->str; // !TODO: Confirm STR bonus
}

SkillMoonlight::SkillMoonlight() : WeaponSkillImpl(HFLI_MOON) {
}

void SkillMoonlight::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 10 + 110 * skill_lv;
}

SkillNeedleOfParalyze::SkillNeedleOfParalyze() : SkillImpl(MH_NEEDLE_OF_PARALYZE) {
}

void SkillNeedleOfParalyze::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillNeedleOfParalyze::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_data *sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // !TODO: Confirm Base Level and DEX bonus
}

void SkillNeedleOfParalyze::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_PARALYSIS, 30 + 5 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillNeedleStinger::SkillNeedleStinger() : SkillImpl(MH_NEEDLE_STINGER) {
}

void SkillNeedleStinger::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillNeedleStinger::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 200 + 500 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // !TODO: Confirm Base Level and DEX bonus
}

SkillOveredBoost::SkillOveredBoost() : SkillImpl(MH_OVERED_BOOST) {
}

void SkillOveredBoost::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const sc_type type = skill_get_sc(getSkillId());
	homun_data* hd = BL_CAST(BL_HOM, src);

	if (hd != nullptr && battle_get_master(src) != nullptr) {
		sc_start(src, battle_get_master(src), type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
}

SkillPainKiller::SkillPainKiller() : SkillImpl(MH_PAIN_KILLER) {
}

void SkillPainKiller::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const sc_type type = skill_get_sc(getSkillId());

	target = battle_get_master(src);
	if (target != nullptr) {
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
}

SkillPoisonMist::SkillPoisonMist() : SkillImpl(MH_POISON_MIST) {
}

void SkillPoisonMist::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;
	// Ammo should be deleted right away.
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillPoisonMist::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 200 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // ! TODO: Confirm DEX bonus
}

SkillPyroclastic::SkillPyroclastic() : SkillImpl(MH_PYROCLASTIC) {
}

void SkillPyroclastic::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const sc_type type = skill_get_sc(getSkillId());
	homun_data* hd = BL_CAST(BL_HOM, src);

	if (hd) {
		block_list* s_bl = battle_get_master(src);
		if (s_bl) {
			sc_start2(src, s_bl, type, 100, skill_lv, hd->homunculus.level, skill_get_time(getSkillId(), skill_lv)); //start on master
		}
		sc_start2(src, target, type, 100, skill_lv, hd->homunculus.level, skill_get_time(getSkillId(), skill_lv));
	}
}

SkillSBR44::SkillSBR44() : WeaponSkillImpl(HFLI_SBR44) {
}

void SkillSBR44::applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const {
	if (src->type == BL_HOM) {
		homun_data& hd = reinterpret_cast<homun_data&>(*src);

		hd.homunculus.intimacy = hom_intimacy_grade2intimacy(HOMGRADE_HATE_WITH_PASSION);

		clif_send_homdata(hd, SP_INTIMATE);
	}
}

void SkillSBR44::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

SkillSilentBreeze::SkillSilentBreeze() : SkillImpl(MH_SILENT_BREEZE) {
}

void SkillSilentBreeze::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	homun_data* hd = BL_CAST(BL_HOM, src);
	status_change* tsc = status_get_sc(target);
	int32 i = 0;
	int32 heal = 5 * status_get_lv(hd) +
#ifdef RENEWAL
		status_base_matk_min(target, &hd->battle_status, status_get_lv(hd));
#else
		status_base_matk_min(&hd->battle_status);
#endif
	//Silences the homunculus and target
	status_change_start(src, src, SC_SILENCE, 10000, skill_lv, 0, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NONE);
	status_change_start(src, target, SC_SILENCE, 10000, skill_lv, 0, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NONE);

	//Recover the target's HP
	status_heal(target, heal, 0, 3);

	//Removes these SC from target
	if (tsc) {
		const enum sc_type scs[] = {
			SC_MANDRAGORA, SC_HARMONIZE, SC_DEEPSLEEP, SC_VOICEOFSIREN, SC_SLEEP, SC_CONFUSION, SC_HALLUCINATION
		};
		for (i = 0; i < ARRAYLENGTH(scs); i++) {
			if (tsc->getSCE(scs[i])) {
				status_change_end(target, scs[i]);
			}
		}
	}
}

SkillSilverVeinRush::SkillSilverVeinRush() : SkillImpl(MH_SILVERVEIN_RUSH) {
}

void SkillSilverVeinRush::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillSilverVeinRush::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 250 * skill_lv * status_get_lv(src) / 100 + sstatus->str; // !TODO: Confirm STR bonus
}

SkillSonicClaw::SkillSonicClaw() : SkillImpl(MH_SONIC_CRAW) {
}

void SkillSonicClaw::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const homun_data *hd = BL_CAST(BL_HOM, &src);

	if (hd != nullptr) {
		dmg.div_ = hd->homunculus.spiritball;
	}
}

void SkillSonicClaw::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillSonicClaw::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 60 * skill_lv * status_get_lv(src) / 150;
}

SkillSteelHorn::SkillSteelHorn() : SkillImpl(MH_STAHL_HORN) {
}

void SkillSteelHorn::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillSteelHorn::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 1000 + 300 * skill_lv * status_get_lv(src) / 150 + sstatus->vit; // !TODO: Confirm VIT bonus
}

void SkillSteelHorn::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_STUN, 20 + 2 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillSteelHorn::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_GOLDENE_FERSE))
		element = ELE_HOLY;
}

SkillStoneWall::SkillStoneWall() : SkillImpl(MH_STEINWAND) {
}

void SkillStoneWall::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;
	// Ammo should be deleted right away.
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

SkillStyleChange::SkillStyleChange() : SkillImpl(MH_STYLE_CHANGE) {
}

void SkillStyleChange::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	homun_data* hd = BL_CAST(BL_HOM, src);

	if (hd) {
		struct status_change_entry* sce;
		if ((sce = hd->sc.getSCE(SC_STYLE_CHANGE)) != nullptr) { //in preparation for other bl usage
			if (sce->val1 == MH_MD_FIGHTING) {
				sce->val1 = MH_MD_GRAPPLING;
			} else {
				sce->val1 = MH_MD_FIGHTING;
			}
			//if(hd->master && hd->sc.getSCE(SC_STYLE_CHANGE)) { // Aegis does not show any message when switching fighting style
			//	char output[128];
			//	safesnprintf(output,sizeof(output),msg_txt(sd,378),(sce->val1==MH_MD_FIGHTING?"fighthing":"grappling"));
			//	clif_messagecolor(hd->master, color_table[COLOR_RED], output, false, SELF);
			//}
		} else {
			sc_start(hd, hd, SC_STYLE_CHANGE, 100, MH_MD_FIGHTING, INFINITE_TICK);
		}
	}
}

static int32 summon_legion_count_sub(block_list* bl, va_list ap);

SkillSummonLegion::SkillSummonLegion() : SkillImpl(MH_SUMMON_LEGION) {
}

void SkillSummonLegion::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	int32 summons[5] = { MOBID_S_HORNET, MOBID_S_GIANT_HORNET, MOBID_S_GIANT_HORNET, MOBID_S_LUCIOLA_VESPA, MOBID_S_LUCIOLA_VESPA };
	int32 qty[5] = { 3, 3, 4, 4, 5 };
	int32 count = 0;
	int32 maxcount = qty[skill_lv - 1];

	map_foreachinmap(summon_legion_count_sub, src->m, BL_MOB, src->id, summons[skill_lv - 1], &count);
	if (count >= maxcount) {
		flag |= SKILL_NOCONSUME_REQ;
		return; //max qty already spawned
	}

	for (int32 i_slave = 0; i_slave < qty[skill_lv - 1]; i_slave++) { //easy way
		mob_data *sum_md = mob_once_spawn_sub(src, src->m, src->x, src->y, status_get_name(*src), summons[skill_lv - 1], "", SZ_SMALL, AI_ATTACK);
		if (sum_md) {
			sum_md->master_id = src->id;
			sum_md->special_state.ai = AI_LEGION;
			if (sum_md->deletetimer != INVALID_TIMER) {
				delete_timer(sum_md->deletetimer, mob_timer_delete);
			}
			sum_md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, sum_md->id, 0);
			mob_spawn(sum_md); //Now it is ready for spawning.
			sc_start4(sum_md, sum_md, SC_MODECHANGE, 100, 1, 0, MD_CANATTACK | MD_AGGRESSIVE, 0, 60000);
		}
	}
}

static int32 summon_legion_count_sub(block_list *bl, va_list ap) {
	mob_data *md = reinterpret_cast<mob_data *>(bl);
	int32 src_id = va_arg(ap, int32);
	int32 mob_class = va_arg(ap, int32);
	int32 *count = va_arg(ap, int32 *);

	if (md->master_id != src_id || md->special_state.ai != AI_LEGION) {
		return 0;
	}

	if (md->mob_id == mob_class) {
		(*count)++;
	}

	return 1;
}

SkillTempering::SkillTempering() : SkillImpl(MH_TEMPERING) {
}

void SkillTempering::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const sc_type type = skill_get_sc(getSkillId());
	block_list* master_bl = battle_get_master(src);

	if (master_bl != nullptr) {
		clif_skill_nodamage(src, *master_bl, getSkillId(), skill_lv);
		sc_start(src, master_bl, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
}

SkillTheOneFighterRises::SkillTheOneFighterRises() : SkillImplRecursiveDamageSplash(MH_THE_ONE_FIGHTER_RISES) {
}

void SkillTheOneFighterRises::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	homun_data* hd = BL_CAST(BL_HOM, src);

	if (hd != nullptr) {
		hom_addspiritball(hd, MAX_SPIRITBALL);
	}

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillTheOneFighterRises::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 580 * skill_lv * status_get_lv(src) / 100 + sstatus->str;
}

SkillTinderBreaker::SkillTinderBreaker() : SkillImpl(MH_TINDER_BREAKER) {
}

void SkillTinderBreaker::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 duration = max(skill_lv, (status_get_str(src) / 7 - status_get_str(target) / 10)) * 1000; //Yommy formula

	if (unit_movepos(src, target->x, target->y, 1, 1)) {
		clif_blown(src);
		clif_skill_poseffect(*src, getSkillId(), skill_lv, target->x, target->y, tick);
	}

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start4(src, target, SC_TINDER_BREAKER2, 100, skill_lv, src->id, 0, 0, duration));
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillToxinOfMandara::SkillToxinOfMandara() : SkillImplRecursiveDamageSplash(MH_TOXIN_OF_MANDARA) {
}

void SkillToxinOfMandara::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 400 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->dex; // !TODO: Confirm Base Level and DEX bonus
}

void SkillToxinOfMandara::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_TOXIN_OF_MANDARA, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillTwisterCutter::SkillTwisterCutter() : SkillImpl(MH_TWISTER_CUTTER) {
}

void SkillTwisterCutter::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillTwisterCutter::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 480 * skill_lv * status_get_lv(src) / 100 + sstatus->int_; // !TODO: Confirm Base Level and INT bonus
}

SkillVolcanicAsh::SkillVolcanicAsh() : SkillImpl(MH_VOLCANIC_ASH) {
}

void SkillVolcanicAsh::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;
	// Ammo should be deleted right away.
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

SkillXenoSlasher::SkillXenoSlasher() : SkillImplRecursiveDamageSplash(MH_XENO_SLASHER) {
}

void SkillXenoSlasher::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;
	// Ammo should be deleted right away.
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillXenoSlasher::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 450 * skill_lv * status_get_lv(src) / 100 + sstatus->int_; // !TODO: Confirm Base Level and INT bonus
}

void SkillXenoSlasher::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start4(src, target, SC_BLEEDING, skill_lv, skill_lv, src->id, 0, 0, skill_get_time2(getSkillId(), skill_lv));
}

void SkillXenoSlasher::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

std::unique_ptr<const SkillImpl> SkillFactoryHomunculus::create(const e_skill skill_id) const {
	switch (skill_id) {
		case HAMI_BLOODLUST:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case HAMI_CASTLE:
			return std::make_unique<SkillCastling>();
		case HAMI_DEFENCE:
			return std::make_unique<SkillDefense>();
		case HFLI_FLEET:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case HFLI_MOON:
			return std::make_unique<SkillMoonlight>();
		case HFLI_SBR44:
			return std::make_unique<SkillSBR44>();
		case HFLI_SPEED:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case HLIF_AVOID:
			return std::make_unique<SkillAvoid>();
		case HLIF_CHANGE:
			return std::make_unique<SkillChange>();
		case HLIF_HEAL:
			return std::make_unique<SkillHealingTouch>();
		case HVAN_CAPRICE:
			return std::make_unique<SkillCaprice>();
		case HVAN_CHAOTIC:
			return std::make_unique<SkillBenedictionOfChaos>();
		case HVAN_EXPLOSION:
			return std::make_unique<SkillBioExplosion>();
		case MH_ABSOLUTE_ZEPHYR:
			return std::make_unique<SkillAbsoluteZephyr>();
		case MH_ANGRIFFS_MODUS:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case MH_BLAST_FORGE:
			return std::make_unique<SkillBlastForge>();
		case MH_BLAZING_AND_FURIOUS:
			return std::make_unique<SkillBlazingAndFurious>();
		case MH_CBC:
			return std::make_unique<SkillContinualBreakCombo>();
		case MH_EQC:
			return std::make_unique<SkillEternalQuickCombo>();
		case MH_ERASER_CUTTER:
			return std::make_unique<SkillEraserCutter>();
		case MH_GLANZEN_SPIES:
			return std::make_unique<SkillGlanzenSpies>();
		case MH_GOLDENE_FERSE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case MH_GOLDENE_TONE:
			return std::make_unique<SkillGoldeneTone>();
		case MH_GRANITIC_ARMOR:
			return std::make_unique<SkillGraniticArmor>();
		case MH_HEILIGE_PFERD:
			return std::make_unique<SkillHeiligePferd>();
		case MH_HEILIGE_STANGE:
			return std::make_unique<SkillHolyPole>();
		case MH_LAVA_SLIDE:
			return std::make_unique<SkillLavaSlide>();
		case MH_LIGHT_OF_REGENE:
			return std::make_unique<SkillLightOfRegene>();
		case MH_MAGMA_FLOW:
			return std::make_unique<SkillMagmaFlow>();
		case MH_MIDNIGHT_FRENZY:
			return std::make_unique<SkillMidnightFrenzy>();
		case MH_NEEDLE_OF_PARALYZE:
			return std::make_unique<SkillNeedleOfParalyze>();
		case MH_NEEDLE_STINGER:
			return std::make_unique<SkillNeedleStinger>();
		case MH_OVERED_BOOST:
			return std::make_unique<SkillOveredBoost>();
		case MH_PAIN_KILLER:
			return std::make_unique<SkillPainKiller>();
		case MH_POISON_MIST:
			return std::make_unique<SkillPoisonMist>();
		case MH_PYROCLASTIC:
			return std::make_unique<SkillPyroclastic>();
		case MH_SILENT_BREEZE:
			return std::make_unique<SkillSilentBreeze>();
		case MH_SILVERVEIN_RUSH:
			return std::make_unique<SkillSilverVeinRush>();
		case MH_SONIC_CRAW:
			return std::make_unique<SkillSonicClaw>();
		case MH_STAHL_HORN:
			return std::make_unique<SkillSteelHorn>();
		case MH_STEINWAND:
			return std::make_unique<SkillStoneWall>();
		case MH_STYLE_CHANGE:
			return std::make_unique<SkillStyleChange>();
		case MH_SUMMON_LEGION:
			return std::make_unique<SkillSummonLegion>();
		case MH_TEMPERING:
			return std::make_unique<SkillTempering>();
		case MH_THE_ONE_FIGHTER_RISES:
			return std::make_unique<SkillTheOneFighterRises>();
		case MH_TINDER_BREAKER:
			return std::make_unique<SkillTinderBreaker>();
		case MH_TOXIN_OF_MANDARA:
			return std::make_unique<SkillToxinOfMandara>();
		case MH_TWISTER_CUTTER:
			return std::make_unique<SkillTwisterCutter>();
		case MH_VOLCANIC_ASH:
			return std::make_unique<SkillVolcanicAsh>();
		case MH_XENO_SLASHER:
			return std::make_unique<SkillXenoSlasher>();

		default:
			return nullptr;
	}
}

#endif
