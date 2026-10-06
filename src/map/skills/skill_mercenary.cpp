// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_mercenary.hpp"

#include <config/core.hpp>
#include "map/map.hpp"
#include "map/clif.hpp"
#include "map/status.hpp"
#include "map/pc.hpp"
#include <common/random.hpp>
#include "map/battle.hpp"
#include "map/unit.hpp"
#include "map/mercenary.hpp"
#include "map/party.hpp"
#include "map/path.hpp"
#include "map/mob.hpp"
#include "skill_impl.hpp"

SkillMercenaryArrowRepel::SkillMercenaryArrowRepel() : WeaponSkillImpl(MA_CHARGEARROW) {
}

void SkillMercenaryArrowRepel::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 50;
}

SkillMercenaryArrowShower::SkillMercenaryArrowShower() : SkillImplRecursiveDamageSplash(MA_SHOWER) {
}

void SkillMercenaryArrowShower::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 50 + 10 * skill_lv;
#else
	base_skillratio += -25 + 5 * skill_lv;
#endif
}

SkillMercenaryBash::SkillMercenaryBash() : WeaponSkillImpl(MS_BASH) {
}

void SkillMercenaryBash::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	// It is proven that bonus is applied on final hitrate, not hit.
	// Base 100% + 30% per level
	base_skillratio += 30 * skill_lv;
}

void SkillMercenaryBash::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	// +5% hit per level
	hit_rate += hit_rate * 5 * skill_lv / 100;
}

SkillMercenaryBenediction::SkillMercenaryBenediction() : SkillImpl(MER_BENEDICTION) {
}

void SkillMercenaryBenediction::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_CURSE);
	status_change_end(target, SC_BLIND);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillMercenaryBlessing::SkillMercenaryBlessing() : SkillImpl(MER_BLESSING) {
}

void SkillMercenaryBlessing::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (dstsd != nullptr && tsc && tsc->getSCE(SC_CHANGEUNDEAD)) {
		if (tstatus->hp > 1)
			skill_attack(BF_MISC,src,src,target,getSkillId(),skill_lv,tick,flag);
		return;
	}
	sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillMercenaryBowlingBash::SkillMercenaryBowlingBash() : WeaponSkillImpl(MS_BOWLINGBASH) {
}

void SkillMercenaryBowlingBash::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.blewcount = 0;
}

void SkillMercenaryBowlingBash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 40 * skill_lv;
}

void SkillMercenaryBowlingBash::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 sflag;

	int32 min_x,max_x,min_y,max_y,i,c,dir,tx,ty;
	// Chain effect and check range gets reduction by recursive depth, as this can reach 0, we don't use blowcount
	c = (skill_lv-(flag&0xFFF)+1)/2;
	// Determine the Bowling Bash area depending on configuration
	if (battle_config.bowling_bash_area == 0) {
		// Gutter line system
		min_x = ((src->x)-c) - ((src->x)-c)%40;
		if(min_x < 0) min_x = 0;
		max_x = min_x + 39;
		min_y = ((src->y)-c) - ((src->y)-c)%40;
		if(min_y < 0) min_y = 0;
		max_y = min_y + 39;
	} else if (battle_config.bowling_bash_area == 1) {
		// Gutter line system without demi gutter bug
		min_x = src->x - (src->x)%40;
		max_x = min_x + 39;
		min_y = src->y - (src->y)%40;
		max_y = min_y + 39;
	} else {
		// Area around caster
		min_x = src->x - battle_config.bowling_bash_area;
		max_x = src->x + battle_config.bowling_bash_area;
		min_y = src->y - battle_config.bowling_bash_area;
		max_y = src->y + battle_config.bowling_bash_area;
	}
	// Initialization, break checks, direction
	if((flag&0xFFF) > 0) {
		// Ignore monsters outside area
		if(target->x < min_x || target->x > max_x || target->y < min_y || target->y > max_y)
			return;
		// Ignore monsters already in list
		if(idb_exists(bowling_db, target->id))
			return;
		// Random direction
		dir = rnd()%8;
	} else {
		// Create an empty list of already hit targets
		db_clear(bowling_db);
		// Direction is walkpath
		dir = (unit_getdir(src)+4)%8;
	}
	// Add current target to the list of already hit targets
	idb_put(bowling_db, target->id, target);
	// Keep moving target in direction square by square
	tx = target->x;
	ty = target->y;
	for(i=0;i<c;i++) {
		// Target coordinates (get changed even if knockback fails)
		tx -= dirx[dir];
		ty -= diry[dir];
		// If target cell is a wall then break
		if(map_getcell(target->m,tx,ty,CELL_CHKWALL))
			break;
		skill_blown(src,target,1,dir,BLOWN_NONE);

		int32 count;

		// Splash around target cell, but only cells inside area; we first have to check the area is not negative
		if((max(min_x,tx-1) <= min(max_x,tx+1)) &&
			(max(min_y,ty-1) <= min(max_y,ty+1)) &&
			(count = map_foreachinallarea(skill_area_sub, target->m, max(min_x,tx-1), max(min_y,ty-1), min(max_x,tx+1), min(max_y,ty+1), splash_target(src), src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY, skill_area_sub_count))) {
			// Recursive call
			map_foreachinallarea(skill_area_sub, target->m, max(min_x,tx-1), max(min_y,ty-1), min(max_x,tx+1), min(max_y,ty+1), splash_target(src), src, getSkillId(), skill_lv, tick, (flag|BCT_ENEMY)+1, skill_castend_damage_id);
			// Self-collision
			if(target->x >= min_x && target->x <= max_x && target->y >= min_y && target->y <= max_y) {
				sflag = (flag&0xFFF) > 0 ? SD_ANIMATION|count : count;

				WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, sflag);
			}
			break;
		}
	}
	// Original hit or chain hit depending on flag
	sflag = (flag&0xFFF) > 0 ? SD_ANIMATION : 0;

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, sflag);
}

SkillMercenaryBrandishSpear::SkillMercenaryBrandishSpear() : SkillImpl(ML_BRANDISH) {
}

void SkillMercenaryBrandishSpear::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	int32 ratio = 100 + 20 * skill_lv;

	base_skillratio += -100 + ratio;
	if(skill_lv > 3 && wd->miscflag == 0)
		base_skillratio += ratio / 2;
	if(skill_lv > 6 && wd->miscflag == 0)
		base_skillratio += ratio / 4;
	if(skill_lv > 9 && wd->miscflag == 0)
		base_skillratio += ratio / 8;
	if(skill_lv > 6 && wd->miscflag == 1)
		base_skillratio += ratio / 2;
	if(skill_lv > 9 && wd->miscflag == 1)
		base_skillratio += ratio / 4;
	if(skill_lv > 9 && wd->miscflag == 2)
		base_skillratio += ratio / 2;
}

void SkillMercenaryBrandishSpear::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Coded apart for it needs the flag passed to the damage calculation.
	if (skill_area_temp[1] != target->id)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag|SD_ANIMATION);
	else
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillMercenaryBrandishSpear::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	skill_area_temp[1] = target->id;

	if(skill_lv >= 10)
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), 1, skill_get_maxcount(getSkillId(), skill_lv)-1, splash_target(src),
			src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | (sd?3:0),
			skill_castend_damage_id);
	if(skill_lv >= 7)
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), 1, skill_get_maxcount(getSkillId(), skill_lv)-2, splash_target(src),
			src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | (sd?2:0),
			skill_castend_damage_id);
	if(skill_lv >= 4)
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), 1, skill_get_maxcount(getSkillId(), skill_lv)-3, splash_target(src),
			src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | (sd?1:0),
			skill_castend_damage_id);
	map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
		skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv)-3, 0, splash_target(src),
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 0,
		skill_castend_damage_id);
}

SkillMercenaryCompress::SkillMercenaryCompress() : SkillImpl(MER_COMPRESS) {
}

void SkillMercenaryCompress::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_BLEEDING);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillMercenaryCrash::SkillMercenaryCrash() : WeaponSkillImpl(MER_CRASH) {
}

void SkillMercenaryCrash::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_STUN,(6*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillMercenaryCrash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 10 * skill_lv;
}

SkillMercenaryDecreaseAgi::SkillMercenaryDecreaseAgi() : SkillImpl(MER_DECAGI) {
}

void SkillMercenaryDecreaseAgi::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* sstatus = status_get_status_data(*src);
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
		sc_start(src,target, type, (50 + skill_lv * 3 + (status_get_lv(src) + sstatus->int_)/5), skill_lv, skill_get_time(getSkillId(),skill_lv)));
}

SkillMercenaryDoubleStrafe::SkillMercenaryDoubleStrafe() : WeaponSkillImpl(MA_DOUBLE) {
}

void SkillMercenaryDoubleStrafe::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 10 * (skill_lv - 1);
}

SkillMercenaryFocusedArrowStrike::SkillMercenaryFocusedArrowStrike() : SkillImpl(MA_SHARPSHOOTING) {
}

void SkillMercenaryFocusedArrowStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	skillratio += -100 + 300 + 300 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += 100 + 50 * skill_lv;
#endif
}

void SkillMercenaryFocusedArrowStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = target->id;
	if (battle_config.skill_eightpath_algorithm) {
		//Use official AoE algorithm
		if (!(map_foreachindir(skill_attack_area, src->m, src->x, src->y, target->x, target->y,
		   skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), 0, splash_target(src),
		   skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY))) {

			//These skills hit at least the target if the AoE doesn't hit
			skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
		}
	} else {
		map_foreachinpath(skill_attack_area, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), splash_target(src),
			skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
	}
}

SkillMercenaryFreezingTrap::SkillMercenaryFreezingTrap() : SkillImpl(MA_FREEZINGTRAP) {
}

void SkillMercenaryFreezingTrap::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data* sstatus = status_get_status_data(*src);

	sc_start(src, target, SC_FREEZE, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv), sstatus->amotion + 100);
}

void SkillMercenaryFreezingTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillMercenaryIncreaseAgility::SkillMercenaryIncreaseAgility() : SkillImpl(MER_INCAGI) {
}

void SkillMercenaryIncreaseAgility::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (dstsd != nullptr && tsc && tsc->getSCE(SC_CHANGEUNDEAD)) {
		if (tstatus->hp > 1)
			skill_attack(BF_MISC,src,src,target,getSkillId(),skill_lv,tick,flag);
		return;
	}
	sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillMercenaryKyrieEleison::SkillMercenaryKyrieEleison() : SkillImpl(MER_KYRIE) {
}

void SkillMercenaryKyrieEleison::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(target,*target,getSkillId(),skill_lv,
	sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
}

SkillMercenaryLandMine::SkillMercenaryLandMine() : SkillImpl(MA_LANDMINE) {
}

void SkillMercenaryLandMine::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_STUN, 10, skill_lv, skill_get_time2(getSkillId(), skill_lv), 1000);
}

void SkillMercenaryLandMine::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillMercenaryLexDivina::SkillMercenaryLexDivina() : SkillImpl(MER_LEXDIVINA) {
}

void SkillMercenaryLexDivina::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;

	if (tsce)
		status_change_end(target, type);
	else
		skill_addtimerskill(src, tick+1000, target->id, 0, 0, getSkillId(), skill_lv, 100, flag);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillMercenaryMagnificat::SkillMercenaryMagnificat() : SkillImpl(MER_MAGNIFICAT) {
}

void SkillMercenaryMagnificat::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	s_mercenary_data* mer = BL_CAST(BL_MER, src);
	sc_type type = skill_get_sc(getSkillId());

	if( mer != nullptr )
	{
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
		if( mer->master && mer->master->status.party_id != 0 && !(flag&1) )
			party_foreachsamemap(skill_area_sub, mer->master, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
		else if( mer->master && !(flag&1) )
			clif_skill_nodamage(src, *mer->master, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	}
}

SkillMercenaryMagnumBreak::SkillMercenaryMagnumBreak() : SkillImpl(MS_MAGNUM) {
}

void SkillMercenaryMagnumBreak::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	if(wd->miscflag == 1)
		base_skillratio += 20 * skill_lv; //Inner 3x3 circle takes 100%+20%*level damage [Playtester]
	else
		base_skillratio += 10 * skill_lv; //Outer 5x5 circle takes 100%+10%*level damage [Playtester]
}

void SkillMercenaryMagnumBreak::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( flag&1 ) {
		// For players, damage depends on distance, so add it to flag if it is > 1
		// Cannot hit hidden targets
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag|SD_ANIMATION|(sd?distance_bl(src, target):0));
	}
}

void SkillMercenaryMagnumBreak::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = 0;
	map_foreachinshootrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_SKILL|BL_CHAR,
		src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|1, skill_castend_damage_id);
	clif_skill_nodamage(src, *src,getSkillId(),skill_lv);
	// Initiate 20% of your damage becomes fire element.
#ifdef RENEWAL
	sc_start4(src,src,SC_SUB_WEAPONPROPERTY,100,ELE_FIRE,20,getSkillId(),0,skill_get_time2(getSkillId(), skill_lv));
#else
	sc_start4(src,src,SC_WATK_ELEMENT,100,ELE_FIRE,20,0,0,skill_get_time2(getSkillId(), skill_lv));
#endif
}

void SkillMercenaryMagnumBreak::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 10 * skill_lv / 100;
}

SkillMercenaryMentalCure::SkillMercenaryMentalCure() : SkillImpl(MER_MENTALCURE) {
}

void SkillMercenaryMentalCure::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_CONFUSION);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillMercenaryMindBlaster::SkillMercenaryMindBlaster() : SkillImpl(MER_INVINCIBLEOFF2) {
}

void SkillMercenaryMindBlaster::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	status_change_end(target, SC_INVINCIBLE);
}

SkillMercenaryPierce::SkillMercenaryPierce() : WeaponSkillImpl(ML_PIERCE) {
}

void SkillMercenaryPierce::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_data* tstatus = status_get_status_data(target);

	dmg.div_= (dmg.div_> 0 ? tstatus->size+1 : -(tstatus->size+1));
}

void SkillMercenaryPierce::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 10 * skill_lv;
}

void SkillMercenaryPierce::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 5 * skill_lv / 100;
}

SkillMercenaryProvoke::SkillMercenaryProvoke() : SkillImpl(MER_PROVOKE) {
}

void SkillMercenaryProvoke::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_data* tstatus = status_get_status_data(*target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 i = 0;

	if( status_has_mode(tstatus,MD_STATUSIMMUNE) || battle_check_undead(tstatus->race,tstatus->def_ele) ) {
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	// Official chance is 70% + 3%*skill_lv + srcBaseLevel% - tarBaseLevel%
	if(!(i = sc_start(src, target, type, 70 + 3 * skill_lv + status_get_lv(src) - status_get_lv(target), skill_lv, skill_get_time(getSkillId(), skill_lv))))
	{
		if( sd )
			clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, i != 0);
	unit_skillcastcancel(target, 2);

	if( dstmd )
	{
		dstmd->state.provoke_flag = src->id;
		mob_target(dstmd, src, skill_get_range2(src, getSkillId(), skill_lv, true));
	}
	// Provoke can cause Coma even though it's a nodamage skill
	if (sd && battle_check_coma(*sd, *target, BF_MISC))
		status_change_start(src, target, SC_COMA, 10000, skill_lv, 0, src->id, 0, 0, SCSTART_NONE);
}

SkillMercenaryRecuperate::SkillMercenaryRecuperate() : SkillImpl(MER_RECUPERATE) {
}

void SkillMercenaryRecuperate::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_POISON);
	status_change_end(target, SC_DPOISON);
	status_change_end(target, SC_SILENCE);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillMercenaryRegain::SkillMercenaryRegain() : SkillImpl(MER_REGAIN) {
}

void SkillMercenaryRegain::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_SLEEP);
	status_change_end(target, SC_STUN);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillMercenaryRemoveTrap::SkillMercenaryRemoveTrap() : SkillImpl(MA_REMOVETRAP) {
}

void SkillMercenaryRemoveTrap::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_unit* su = BL_CAST(BL_SKILL, target);
	std::shared_ptr<s_skill_unit_group> sg;
	std::shared_ptr<s_skill_db> skill_group;

	// Mercenaries can remove any trap
	if( su && (sg = su->group) && ( skill_group = skill_db.find(sg->skill_id) ) && skill_group->inf2[INF2_ISTRAP] )
	{
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		skill_delunit(su);
	}
}

SkillMercenarySacrifice::SkillMercenarySacrifice() : SkillImpl(ML_DEVOTION) {
}

void SkillMercenarySacrifice::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	s_mercenary_data* mer = BL_CAST(BL_MER, src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	int32 i = 0;

	int32 lv;
	if( !dstsd || !mer )
	{ // Only players can be devoted
		return;
	}

	if( (lv = status_get_lv(src) - dstsd->status.base_level) < 0 )
		lv = -lv;
	if( lv > battle_config.devotion_level_difference || // Level difference requeriments
		(dstsd->sc.getSCE(type) && dstsd->sc.getSCE(type)->val1 != src->id) || // Cannot Devote a player devoted from another source
		mer != dstsd->md || // Mercenary only can devote owner
		(dstsd->class_&MAPID_SECONDMASK) == MAPID_CRUSADER || // Crusader Cannot be devoted
		(dstsd->sc.getSCE(SC_HELLPOWER))) // Players affected by SC_HELLPOWER cannot be devoted.
	{
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	i = 0;
	mer->devotion_flag = 1; // Mercenary Devoting Owner

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
		sc_start4(src, target, type, 10000, src->id, i, skill_get_range2(src, getSkillId(), skill_lv, true), 0, skill_get_time2(getSkillId(), skill_lv)));
	clif_devotion(src, nullptr);
}

SkillMercenarySandman::SkillMercenarySandman() : SkillImpl(MA_SANDMAN) {
}

void SkillMercenarySandman::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_SLEEP, (10 * skill_lv + 40), skill_lv, skill_get_time2(getSkillId(), skill_lv), 1000);
}

void SkillMercenarySandman::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillMercenaryScapegoat::SkillMercenaryScapegoat() : SkillImpl(MER_SCAPEGOAT) {
}

void SkillMercenaryScapegoat::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	s_mercenary_data* mer = BL_CAST(BL_MER, src);

	if( mer && mer->master )
	{
		status_heal(mer->master, mer->battle_status.hp, 0, 2);
		status_damage(src, src, mer->battle_status.max_hp, 0, 0, 1, getSkillId());
	}
}

SkillMercenarySense::SkillMercenarySense() : SkillImpl(MER_ESTIMATION) {
}

void SkillMercenarySense::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	s_mercenary_data* mer = BL_CAST(BL_MER, src);
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if( !mer )
		return;
	sd = mer->master;
	if( sd == nullptr )
		return;
	if( dstsd )
	{ // Fail on Players
		clif_skill_fail( *sd, getSkillId() );
		return;
	}

	if (dstmd != nullptr)
		clif_skill_estimation( *sd, *dstmd );
	sd = nullptr;
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillMercenaryShieldReflect::SkillMercenaryShieldReflect() : StatusSkillImpl(MS_REFLECTSHIELD) {
}

void SkillMercenaryShieldReflect::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (tsc && tsc->getSCE(SC_DARKCROW)) { // SC_DARKCROW prevents using reflecting skills
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		return;
	}
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillMercenarySight::SkillMercenarySight() : SkillImpl(MER_SIGHT) {
}

void SkillMercenarySight::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target,type,100,skill_lv,getSkillId(),skill_get_time(getSkillId(),skill_lv)));
}

SkillMercenarySkidTrap::SkillMercenarySkidTrap() : SkillImpl(MA_SKIDTRAP) {
}

void SkillMercenarySkidTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillMercenarySpiralPierce::SkillMercenarySpiralPierce() : WeaponSkillImpl(ML_SPIRALPIERCE) {
}

void SkillMercenarySpiralPierce::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	skillratio += 50 + 50 * skill_lv;
	RE_LVL_DMOD(100);
#endif
}

SkillMercenaryTender::SkillMercenaryTender() : SkillImpl(MER_TENDER) {
}

void SkillMercenaryTender::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_FREEZE);
	status_change_end(target, SC_STONE);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

std::unique_ptr<const SkillImpl> SkillFactoryMercenary::create(const e_skill skill_id) const {
	switch( skill_id ){
		case MA_CHARGEARROW:
			return std::make_unique<SkillMercenaryArrowRepel>();
		case MA_DOUBLE:
			return std::make_unique<SkillMercenaryDoubleStrafe>();
		case MA_FREEZINGTRAP:
			return std::make_unique<SkillMercenaryFreezingTrap>();
		case MA_LANDMINE:
			return std::make_unique<SkillMercenaryLandMine>();
		case MA_REMOVETRAP:
			return std::make_unique<SkillMercenaryRemoveTrap>();
		case MA_SANDMAN:
			return std::make_unique<SkillMercenarySandman>();
		case MA_SHARPSHOOTING:
			return std::make_unique<SkillMercenaryFocusedArrowStrike>();
		case MA_SHOWER:
			return std::make_unique<SkillMercenaryArrowShower>();
		case MA_SKIDTRAP:
			return std::make_unique<SkillMercenarySkidTrap>();
		case MER_AUTOBERSERK:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case MER_BENEDICTION:
			return std::make_unique<SkillMercenaryBenediction>();
		case MER_BLESSING:
			return std::make_unique<SkillMercenaryBlessing>();
		case MER_COMPRESS:
			return std::make_unique<SkillMercenaryCompress>();
		case MER_CRASH:
			return std::make_unique<SkillMercenaryCrash>();
		case MER_DECAGI:
			return std::make_unique<SkillMercenaryDecreaseAgi>();
		case MER_ESTIMATION:
			return std::make_unique<SkillMercenarySense>();
		case MER_INCAGI:
			return std::make_unique<SkillMercenaryIncreaseAgility>();
		case MER_INVINCIBLEOFF2:
			return std::make_unique<SkillMercenaryMindBlaster>();
		case MER_KYRIE:
			return std::make_unique<SkillMercenaryKyrieEleison>();
		case MER_LEXDIVINA:
			return std::make_unique<SkillMercenaryLexDivina>();
		case MER_MAGNIFICAT:
			return std::make_unique<SkillMercenaryMagnificat>();
		case MER_MENTALCURE:
			return std::make_unique<SkillMercenaryMentalCure>();
		case MER_PROVOKE:
			return std::make_unique<SkillMercenaryProvoke>();
		case MER_QUICKEN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case MER_RECUPERATE:
			return std::make_unique<SkillMercenaryRecuperate>();
		case MER_REGAIN:
			return std::make_unique<SkillMercenaryRegain>();
		case MER_SCAPEGOAT:
			return std::make_unique<SkillMercenaryScapegoat>();
		case MER_SIGHT:
			return std::make_unique<SkillMercenarySight>();
		case MER_TENDER:
			return std::make_unique<SkillMercenaryTender>();
		case ML_AUTOGUARD:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case ML_BRANDISH:
			return std::make_unique<SkillMercenaryBrandishSpear>();
		case ML_DEFENDER:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case ML_DEVOTION:
			return std::make_unique<SkillMercenarySacrifice>();
		case ML_PIERCE:
			return std::make_unique<SkillMercenaryPierce>();
		case ML_SPIRALPIERCE:
			return std::make_unique<SkillMercenarySpiralPierce>();
		case MS_BASH:
			return std::make_unique<SkillMercenaryBash>();
		case MS_BERSERK:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case MS_BOWLINGBASH:
			return std::make_unique<SkillMercenaryBowlingBash>();
		case MS_MAGNUM:
			return std::make_unique<SkillMercenaryMagnumBreak>();
		case MS_PARRYING:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case MS_REFLECTSHIELD:
			return std::make_unique<SkillMercenaryShieldReflect>();

		default:
			return nullptr;
	}
}

#endif
