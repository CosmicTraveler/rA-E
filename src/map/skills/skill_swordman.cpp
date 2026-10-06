// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_swordman.hpp"

#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include <config/core.hpp>
#include <common/db.hpp>
#include "map/battle.hpp"
#include "map/unit.hpp"
#include "map/map.hpp"
#include "map/path.hpp"
#include "map/party.hpp"
#include "map/skill.hpp"
#include <common/random.hpp>
#include "map/mob.hpp"
#include <common/nullpo.hpp>
#include "skill_impl.hpp"

SkillAbundance::SkillAbundance() : SkillImpl(RK_ABUNDANCE) {
}

void SkillAbundance::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
		if (pc_checkskill(sd, RK_RUNEMASTERY) >= 6) {
			if (sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)))
				clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		} else
			clif_skill_fail( *sd, getSkillId() );
 	}
}

SkillAutoBerserk::SkillAutoBerserk() : SkillImpl(SM_AUTOBERSERK)
{
}

void SkillAutoBerserk::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(bl);
	status_change_entry *tsce = (tsc) ? tsc->getSCE(type) : nullptr;

	int32 i;
	if (tsce)
		i = status_change_end(bl, type);
	else
		i = sc_start(src, bl, type, 100, skill_lv, 60000);
	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv, i);
}

SkillBanding::SkillBanding() : SkillImpl(LG_BANDING) {
}

void SkillBanding::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	std::shared_ptr<s_skill_unit_group> sg;
	status_change* sc = status_get_sc(src);

	if( sc && sc->getSCE(SC_BANDING) )
		status_change_end(src,SC_BANDING);
	else if( (sg = skill_unitsetting(src,getSkillId(),skill_lv,src->x,src->y,0)) != nullptr )
		sc_start4(src,src,SC_BANDING,100,skill_lv,0,0,sg->group_id,skill_get_time(getSkillId(),skill_lv));
	clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
}

SkillBanishingPoint::SkillBanishingPoint() : WeaponSkillImpl(LG_BANISHINGPOINT) {
}

void SkillBanishingPoint::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + (100 * skill_lv);

	if (sd != nullptr) {
		skillratio += pc_checkskill(sd, SM_BASH) * 70;
	}

	if (sc != nullptr && sc->getSCE(SC_SPEAR_SCAR)) {
		skillratio += 800;
	}

	RE_LVL_DMOD(100);
}

void SkillBanishingPoint::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += 5 * skill_lv;
}

SkillBash::SkillBash() : WeaponSkillImpl(SM_BASH) {
}

void SkillBash::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	// Base 100% + 30% per level
	base_skillratio += 30 * skill_lv;
}

void SkillBash::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	// It is proven that bonus is applied on final hitrate, not hit.
	// +5% hit per level
	hit_rate += hit_rate * 5 * skill_lv / 100;
}

void SkillBash::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd != nullptr && skill_lv > 5 && pc_checkskill(sd, SM_FATALBLOW) > 0) {
		// BaseChance gets multiplied with BaseLevel/50.0; 500/50 simplifies to 10 [Playtester]
		int32 stun_chance = (skill_lv - 5) * sd->status.base_level * 10;
		status_change_start(src, target, SC_STUN, stun_chance, skill_lv, 0, 0, 0, skill_get_time2(getSkillId(), skill_lv), SCSTART_NONE);
	}
}

SkillBattleChant::SkillBattleChant() : SkillImpl(PA_GOSPEL) {
}

void SkillBattleChant::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change* sc = status_get_sc(src);
	status_change_entry *sce = (sc && type != SC_NONE)?sc->getSCE(type):nullptr;

	if (sce && sce->val4 == BCT_SELF)
	{
		status_change_end(src, SC_GOSPEL);
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	else
	{
		std::shared_ptr<s_skill_unit_group> sg = skill_unitsetting(src,getSkillId(),skill_lv,src->x,src->y,0);
		if (!sg) return;
		if (sce)
			status_change_end(src, type); //Was under someone else's Gospel. [Skotlex]
		sc_start4(src,src,type,100,skill_lv,0,sg->group_id,BCT_SELF,skill_get_time(getSkillId(),skill_lv));
		clif_skill_poseffect( *src, getSkillId(), skill_lv, 0, 0, tick ); // PA_GOSPEL music packet
	}
}

SkillBowlingBash::SkillBowlingBash() : SkillImpl(KN_BOWLINGBASH) {
}

void SkillBowlingBash::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->status.weapon == W_2HSWORD) {
		if (dmg.miscflag >= 4)
			dmg.div_ = 4;
		else if (dmg.miscflag >= 2)
			dmg.div_ = 3;
	}
#else
	dmg.blewcount = 0;
#endif
}

void SkillBowlingBash::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 40 * skill_lv;
}

void SkillBowlingBash::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	if (flag & 1) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, (skill_area_temp[0]) > 0 ? SD_ANIMATION | skill_area_temp[0] : skill_area_temp[0]);
	} else {
		skill_area_temp[0] = map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
#else
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
			if(target->x >= min_x && target->x <= max_x && target->y >= min_y && target->y <= max_y)
				skill_attack(BF_WEAPON,src,src,target,getSkillId(),skill_lv,tick,(flag&0xFFF)>0?SD_ANIMATION|count:count);
			break;
		}
	}
	// Original hit or chain hit depending on flag
	skill_attack(BF_WEAPON,src,src,target,getSkillId(),skill_lv,tick,(flag&0xFFF)>0?SD_ANIMATION:0);
#endif
}

SkillBrandishSpear::SkillBrandishSpear() : SkillImpl(KN_BRANDISHSPEAR) {
}

void SkillBrandishSpear::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
		skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), 0, splash_target(src),
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 0,
		skill_castend_damage_id);
#else
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
#endif
}

void SkillBrandishSpear::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
#else
	//Coded apart for it needs the flag passed to the damage calculation.
	if (skill_area_temp[1] != target->id)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag|SD_ANIMATION);
	else
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
#endif
}

void SkillBrandishSpear::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 400 + 100 * skill_lv + sstatus->str * 3;
#else
	int32 ratio = 100 + 20 * skill_lv;

	base_skillratio += -100 + ratio;
	if (skill_lv > 3 && wd->miscflag == 0)
		base_skillratio += ratio / 2;
	if (skill_lv > 6 && wd->miscflag == 0)
		base_skillratio += ratio / 4;
	if (skill_lv > 9 && wd->miscflag == 0)
		base_skillratio += ratio / 8;
	if (skill_lv > 6 && wd->miscflag == 1)
		base_skillratio += ratio / 2;
	if (skill_lv > 9 && wd->miscflag == 1)
		base_skillratio += ratio / 4;
	if (skill_lv > 9 && wd->miscflag == 2)
		base_skillratio += ratio / 2;
#endif
}

SkillCannonSpear::SkillCannonSpear() : SkillImplRecursiveDamageSplash(LG_CANNONSPEAR) {
}

void SkillCannonSpear::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);

	if (skill_area_temp[2] == 0) {
		clif_skill_damage(*src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE);
	}
}

void SkillCannonSpear::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + skill_lv * (120 + sstatus->str);

	if (sc != nullptr && sc->getSCE(SC_SPEAR_SCAR)) {
		skillratio += 400;
	}

	RE_LVL_DMOD(100);
}

SkillChargeAttack::SkillChargeAttack() : SkillImpl(KN_CHARGEATK) {
}

void SkillChargeAttack::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	bool path = path_search_long(nullptr, src->m, src->x, src->y, target->x, target->y,CELL_CHKWALL);
#ifdef RENEWAL
	int32 dist = skill_get_blewcount(getSkillId(), skill_lv);
#else
	// Charge attack in pre-renewal calculates the distance mathetically
	int32 dist = static_cast<int32>(distance_math_bl(src, target));
#endif
	uint8 dir = map_calc_dir(target, src->x, src->y);

	// teleport to target (if not on WoE grounds)
	if (skill_check_unit_movepos(5, src, target->x + dirx[dir], target->y + diry[dir], 0, true))
		clif_blown(src);

	// cause damage and knockback if the path to target was a straight one
	if (path) {
		if(skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, dist)) {
#ifdef RENEWAL
			if (map_getmapdata(src->m)->getMapFlag(MF_PVP))
				dist += 2; // Knockback is 4 on PvP maps
#endif
			skill_blown(src, target, dist, dir, BLOWN_NONE);
		}
	}
}

void SkillChargeAttack::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 600;
#else
	// +100% every 3 cells of distance but hard-limited to 500%
	int32 k = (wd->miscflag - 1) / 3;
	if (k < 0)
		k = 0;
	else if (k > 4)
		k = 4;
	base_skillratio += 100 * k;
#endif
}

SkillCounterAttack::SkillCounterAttack() : SkillImpl(KN_AUTOCOUNTER) {
}

void SkillCounterAttack::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.flag = (dmg.flag&~BF_SKILLMASK)|BF_NORMAL;
}

void SkillCounterAttack::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	skill_addtimerskill(src, tick + 100, target->id, 0, 0, getSkillId(), skill_lv, BF_WEAPON, flag);
}

void SkillCounterAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillCrossRain::SkillCrossRain() : SkillImpl(IG_CROSS_RAIN) {
}

void SkillCrossRain::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillCrossRain::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	if( sc && sc->getSCE( SC_HOLY_S ) ){
		skillratio += -100 + ( 650 + 15 * pc_checkskill( sd, IG_SPEAR_SWORD_M ) ) * skill_lv;
	}else{
		skillratio += -100 + ( 450 + 10 * pc_checkskill( sd, IG_SPEAR_SWORD_M ) ) * skill_lv;
	}
	skillratio += 7 * sstatus->spl;
	RE_LVL_DMOD(100);
}

SkillCrushStrike::SkillCrushStrike() : SkillImpl(RK_CRUSHSTRIKE) {
}

void SkillCrushStrike::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
		if (pc_checkskill(sd, RK_RUNEMASTERY) >= 7) {
			if (sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)))
				clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		} else
			clif_skill_fail( *sd, getSkillId() );
	}
}

// RK_DRAGONBREATH
SkillDragonBreath::SkillDragonBreath() : SkillImplRecursiveDamageSplash(RK_DRAGONBREATH) {
}

void SkillDragonBreath::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start4(src,target,SC_BURNING,15,skill_lv,1000,src->id,0,skill_get_time(getSkillId(),skill_lv));
}

void SkillDragonBreath::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if( tsc && tsc->getSCE(SC_HIDING) )
		clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
	else {
		skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
	}
}

void SkillDragonBreath::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr) {
		if (sc->hasSCE(SC_LUXANIMA)) // Lux Anima has priority over Giant Growth
			element = ELE_DARK;
		else if (sc->hasSCE(SC_GIANTGROWTH))
			element = ELE_HOLY;
	}
}


// RK_DRAGONBREATH_WATER
SkillDragonBreathWater::SkillDragonBreathWater() : SkillImplRecursiveDamageSplash(RK_DRAGONBREATH_WATER) {
}

void SkillDragonBreathWater::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_FREEZING,15,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

void SkillDragonBreathWater::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if( tsc && tsc->getSCE(SC_HIDING) )
		clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
	else {
		skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
	}
}

void SkillDragonBreathWater::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr) {
		if (sc->hasSCE(SC_LUXANIMA)) // Lux Anima has priority over Fighting Spirit
			element = ELE_NEUTRAL;
		else if (sc->hasSCE(SC_FIGHTINGSPIRIT))
			element = ELE_GHOST;
	}
}

SkillDragonHowling::SkillDragonHowling() : SkillImpl(RK_DRAGONHOWLING) {
}

void SkillDragonHowling::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	if (flag & 1) {
		sc_start(src, target, type, 50 + 6 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		skill_area_temp[2] = 0;
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR,
			src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_PREAMBLE | 1, skill_castend_nodamage_id);
	}
}

SkillDragonicAura::SkillDragonicAura() : WeaponSkillImpl(DK_DRAGONIC_AURA) {
}

void SkillDragonicAura::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(),skill_lv));
}

void SkillDragonicAura::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += 3650 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_ANGEL)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillDragonicBreath::SkillDragonicBreath() : SkillImplRecursiveDamageSplash(DK_DRAGONIC_BREATH) {
}

void SkillDragonicBreath::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 250 + 400 * skill_lv;
	skillratio += 7 * sstatus->pow;

	if (sc && sc->getSCE(SC_DRAGONIC_AURA)) {
		skillratio += 3 * sstatus->pow;
		skillratio += (skill_lv * (sstatus->max_hp * 25 / 100) * 7) / 100;
		skillratio += (skill_lv * sstatus->max_sp * 7) / 100;
	} else {
		skillratio += (skill_lv * (sstatus->max_hp * 25 / 100) * 5) / 100;
		skillratio += (skill_lv * sstatus->max_sp * 5) / 100;
	}

	RE_LVL_DMOD(100);
}

void SkillDragonicBreath::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillDragonicPierce::SkillDragonicPierce() : WeaponSkillImpl(DK_DRAGONIC_PIERCE) {
}

void SkillDragonicPierce::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillDragonicPierce::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 900 + 730 * skill_lv;
	skillratio += 7 * sstatus->pow;	// !TODO: unknown ratio

	if (sc != nullptr && sc->hasSCE(SC_DRAGONIC_AURA))
		skillratio += 200 + 50 * skill_lv;

	RE_LVL_DMOD(100);
}

SkillEarthDrive::SkillEarthDrive() : SkillImplRecursiveDamageSplash(LG_EARTHDRIVE) {
}

void SkillEarthDrive::castendNoDamageId(block_list* src, block_list* bl, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 dummy = 1;

	clif_skill_damage( *src, *bl,tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	int32 i = skill_get_splash(getSkillId(),skill_lv);
	map_foreachinallarea(skill_cell_overlap, src->m, src->x-i, src->y-i, src->x+i, src->y+i, BL_SKILL, getSkillId(), &dummy, src);
	map_foreachinrange(skill_area_sub, bl,i,BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
}

void SkillEarthDrive::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 380 * skill_lv + sstatus->str + sstatus->vit; // !TODO: What's the STR/VIT bonus?

	if( sc != nullptr && sc->getSCE( SC_SHIELD_POWER ) ){
		skillratio += skill_lv * 37 * pc_checkskill( sd, IG_SHIELD_MASTERY );
	}

	RE_LVL_DMOD(100);
}

SkillEnchantBlade::SkillEnchantBlade() : SkillImpl(RK_ENCHANTBLADE) {
}

void SkillEnchantBlade::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	const status_data* sstatus = status_get_status_data(*src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
		sc_start2(src, target, type, 100, skill_lv, ((100 + 20 * skill_lv) * status_get_lv(src)) / 100 + sstatus->int_, skill_get_time(getSkillId(), skill_lv)));
}

SkillFightingSpirit::SkillFightingSpirit() : SkillImpl(RK_FIGHTINGSPIRIT) {
}

void SkillFightingSpirit::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	uint8 runemastery_skill_lv = (sd ? pc_checkskill(sd, RK_RUNEMASTERY) : skill_get_max(RK_RUNEMASTERY));

	// val1: ATKBonus: ? // !TODO: Confirm new ATK formula
	// val2: ASPD boost: [RK_RUNEMASTERYlevel * 4 / 10] * 10 ==> RK_RUNEMASTERYlevel * 4
	sc_start2(src,target,skill_get_sc(getSkillId()),100,70 + 7 * runemastery_skill_lv,4 * runemastery_skill_lv,skill_get_time(getSkillId(),skill_lv));
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillForceOfVanguard::SkillForceOfVanguard() : SkillImpl(LG_FORCEOFVANGUARD) {
}

void SkillForceOfVanguard::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const status_change* tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	const status_change_entry* tsce = (tsc && type != SC_NONE) ? tsc->getSCE(type) : nullptr;
	map_session_data* sd = BL_CAST(BL_PC, src);

	int32 result;
	if (tsce != nullptr) {
		result = status_change_end(target, type);
		if (result) {
			clif_skill_nodamage(src, *target, getSkillId(), skill_lv, result);
		} else if (sd != nullptr) {
			clif_skill_fail(*sd, getSkillId());
		}
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	result = sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	if (result) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, result);
	} else if (sd != nullptr) {
		clif_skill_fail(*sd, getSkillId(), USESKILL_FAIL_LEVEL);
	}
}

SkillGiantGrowth::SkillGiantGrowth() : SkillImpl(RK_GIANTGROWTH) {
}

void SkillGiantGrowth::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
		if (pc_checkskill(sd, RK_RUNEMASTERY) >= 1) {
			if (sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)))
				clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		} else
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillGloriaDomini::SkillGloriaDomini() : SkillImpl(PA_PRESSURE) {
}

void SkillGloriaDomini::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	skillratio += -100 + 500 + 150 * skill_lv;
	RE_LVL_DMOD(100);
#endif
}

void SkillGloriaDomini::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
#else
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
#endif
}

void SkillGloriaDomini::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
#ifndef RENEWAL
	status_percent_damage(src, target, 0, 15+5*skill_lv, false);
	//Pressure can trigger physical autospells
	attack_type |= BF_NORMAL;
	attack_type |= BF_WEAPON;
#endif
}

SkillGrandCross::SkillGrandCross() : SkillImpl(CR_GRANDCROSS) {
}

void SkillGrandCross::applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const {
	if (src == target) {
		// Grand Cross on self specifically only triggers "When hit by physical attack" autospells and ignores everything else
		attack_type |= BF_WEAPON;
		attack_type &= ~BF_MAGIC;
	}
}

void SkillGrandCross::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag|=1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillGrandCross::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	status_data* tstatus = status_get_status_data(*target);

	//Chance to cause blind status vs demon and undead element, but not against players
	if(!dstsd && (battle_check_undead(tstatus->race,tstatus->def_ele) || tstatus->race == RC_DEMON))
		sc_start(src,target,SC_BLIND,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

SkillGrandJudgement::SkillGrandJudgement() : SkillImplRecursiveDamageSplash(IG_GRAND_JUDGEMENT) {
}

void SkillGrandJudgement::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	int32 i;

	skillratio += -100 + 250 + 1500 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_PLANT || tstatus->race == RC_INSECT)
		skillratio += 100 + 150 * skill_lv;
	RE_LVL_DMOD(100);
	if ((i = pc_checkskill_imperial_guard(sd, 3)) > 0)
		skillratio += skillratio * i / 100;
}

void SkillGrandJudgement::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillGuardianShield::SkillGuardianShield() : SkillImpl(IG_GUARDIAN_SHIELD) {
}

void SkillGuardianShield::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) )
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillHackAndSlasher::SkillHackAndSlasher() : SkillImplRecursiveDamageSplash(DK_HACKANDSLASHER) {
}

void SkillHackAndSlasher::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 500 + 1000 * skill_lv;
	skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillHackAndSlasher::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillHackAndSlasherAttack::SkillHackAndSlasherAttack() : SkillImpl(DK_HACKANDSLASHER_ATK) {
}

void SkillHackAndSlasherAttack::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 500 + 1000 * skill_lv;
	skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
}

SkillHesperusLit::SkillHesperusLit() : WeaponSkillImpl(LG_HESPERUSLIT) {
}

void SkillHesperusLit::applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr) {
		return;
	}

	status_change_entry* sce = sd->sc.getSCE(SC_FORCEOFVANGUARD);

	if (sce == nullptr) {
		return;
	}

	for (int32 i = 0; i < sce->val3; i++) {
		pc_addspiritball(sd, skill_get_time(LG_FORCEOFVANGUARD, 1), sce->val3);
	}
}

void SkillHesperusLit::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	if (sc && sc->getSCE(SC_INSPIRATION))
		skillratio += -100 + 450 * skill_lv;
	else
		skillratio += -100 + 300 * skill_lv;
	skillratio += sstatus->vit / 6; // !TODO: What's the VIT bonus?
	RE_LVL_DMOD(100);
}

void SkillHesperusLit::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_change* sc = status_get_sc(src);

	if( pc_checkskill(sd,LG_PINPOINTATTACK) > 0 && sc && sc->getSCE(SC_BANDING) && sc->getSCE(SC_BANDING)->val2 > 5 )
		skill_castend_damage_id(src,target,LG_PINPOINTATTACK, rnd_value<uint16>(1, pc_checkskill(sd,LG_PINPOINTATTACK)),tick,0);
}

void SkillHesperusLit::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_BANDING) && sc->getSCE(SC_BANDING)->val2 > 4)
		element = ELE_HOLY;
}

SkillHolyCross::SkillHolyCross() : WeaponSkillImpl(CR_HOLYCROSS) {
}

void SkillHolyCross::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd && sd->status.weapon == W_2HSPEAR)
		base_skillratio += 70 * skill_lv;
	else
#endif
		base_skillratio += 35 * skill_lv;
}

void SkillHolyCross::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_BLIND,3*skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

SkillHundredSpear::SkillHundredSpear() : SkillImplRecursiveDamageSplash(RK_HUNDREDSPEAR) {
}

void SkillHundredSpear::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 600 + 200 * skill_lv;
	if (sd)
		skillratio += 50 * pc_checkskill(sd,LK_SPIRALPIERCE);
	if (sc) {
		if( sc->getSCE( SC_DRAGONIC_AURA ) ){
			skillratio += sc->getSCE( SC_DRAGONIC_AURA )->val1 * 160;
		}

		if (sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
			skillratio *= 2;
	}
	RE_LVL_DMOD(100);
}

SkillIgnitionBreak::SkillIgnitionBreak() : SkillImplRecursiveDamageSplash(RK_IGNITIONBREAK) {
}

void SkillIgnitionBreak::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 450 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillIgnitionBreak::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = 0;

#if PACKETVER >= 20180207
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
#else
	clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
#endif
	map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|SD_SPLASH|1, skill_castend_damage_id);
}

SkillImperialCross::SkillImperialCross() : WeaponSkillImpl(IG_IMPERIAL_CROSS) {
}

void SkillImperialCross::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillImperialCross::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 1650 + 1350 * skill_lv;
	skillratio += pc_checkskill(sd, IG_SPEAR_SWORD_M) * 25;
	skillratio += 5 * sstatus->pow;	// !TODO: check POW ratio

	if (sc != nullptr && sc->getSCE(SC_SPEAR_SCAR))
		skillratio += 100 + 300 * skill_lv;

	RE_LVL_DMOD(100);
}

SkillImperialPressure::SkillImperialPressure() : SkillImplRecursiveDamageSplash(IG_IMPERIAL_PRESSURE) {
}

void SkillImperialPressure::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 3750 + 2650 * skill_lv;
	skillratio += 7 * sstatus->spl;
	skillratio += 50 * pc_checkskill(sd, IG_SPEAR_SWORD_M);
	RE_LVL_DMOD(100);
}

void SkillImperialPressure::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

void SkillImperialPressure::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_GUARD_STANCE))
		element = ELE_HOLY;
}

SkillJudgementCross::SkillJudgementCross() : SkillImpl(IG_JUDGEMENT_CROSS) {
}

void SkillJudgementCross::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillJudgementCross::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	int32 i;

	skillratio += -100 + 1950 * skill_lv + 10 * sstatus->spl;
	if (tstatus->race == RC_PLANT || tstatus->race == RC_INSECT)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
	if ((i = pc_checkskill_imperial_guard(sd, 3)) > 0)
		skillratio += skillratio * i / 100;
}

SkillKingsGrace::SkillKingsGrace() : SkillImpl(LG_KINGS_GRACE) {
}

void SkillKingsGrace::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillLuxAnima::SkillLuxAnima() : SkillImpl(RK_LUXANIMA) {
}

void SkillLuxAnima::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	status_change_clear_buffs(target, SCCB_LUXANIMA); // For bonus_script
	sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillMadnessCrusher::SkillMadnessCrusher() : SkillImplRecursiveDamageSplash(DK_MADNESS_CRUSHER) {
}

void SkillMadnessCrusher::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1750 + 4350 * skill_lv;
	skillratio += 10 * sstatus->pow;

	if (sd != nullptr) {
		int16 index = sd->equip_index[EQI_HAND_R];

		if (index >= 0 && sd->inventory_data[index] != nullptr) {
			skillratio += sd->inventory_data[index]->weight / 10 * sd->inventory_data[index]->weapon_level;
		}
	}
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		skillratio *= 2;
}

SkillMagnumBreak::SkillMagnumBreak() : SkillImpl(SM_MAGNUM)
{
}

void SkillMagnumBreak::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const
{
	if (wd->miscflag == 1)
	 	// Inner 3x3 circle takes 100%+20%*level damage [Playtester]
		base_skillratio += 20 * skill_lv;
	else
		// Outer 5x5 circle takes 100%+10%*level damage [Playtester]
		base_skillratio += 10 * skill_lv;
}

void SkillMagnumBreak::modifyHitRate(int16 &hit_rate, const block_list *src, const block_list *target, uint16 skill_lv) const
{
	hit_rate += hit_rate * 10 * skill_lv / 100;
}

void SkillMagnumBreak::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const
{
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (flag & 1)
	{
		// For players, damage depends on distance, so add it to flag if it is > 1
		// Cannot hit hidden targets
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag | SD_ANIMATION | (sd?distance_bl(src, target):0));
	}
}

void SkillMagnumBreak::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	// Passive Magnum, should had been casted on yourself.
	skill_area_temp[1] = 0;
	e_skill skillId = getSkillId();
	map_foreachinshootrange(skill_area_sub, src, skill_get_splash(skillId, skill_lv), BL_SKILL | BL_CHAR,
							src, skillId, skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	clif_skill_nodamage(src, *src, skillId, skill_lv);
	// Initiate 20% of your damage becomes fire element.
#ifdef RENEWAL
	sc_start4(src, src, SC_SUB_WEAPONPROPERTY, 100, ELE_FIRE, 20, skillId, 0, skill_get_time2(skillId, skill_lv));
#else
	sc_start4(src, src, SC_WATK_ELEMENT, 100, ELE_FIRE, 20, 0, 0, skill_get_time2(skillId, skill_lv));
#endif
}

SkillMartyrsReckoning::SkillMartyrsReckoning() : WeaponSkillImpl(PA_SACRIFICE) {
}

void SkillMartyrsReckoning::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -10 + 10 * skill_lv;
}

void SkillMartyrsReckoning::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
}

SkillMilleniumShield::SkillMilleniumShield() : SkillImpl(RK_MILLENNIUMSHIELD) {
}

void SkillMilleniumShield::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
		if (pc_checkskill(sd, RK_RUNEMASTERY) >= 9) {
			if (sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)))
				clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		} else
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillMoonSlasher::SkillMoonSlasher() : SkillImplRecursiveDamageSplash(LG_MOONSLASHER) {
}

void SkillMoonSlasher::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

void SkillMoonSlasher::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 120 * skill_lv + ((sd) ? pc_checkskill(sd, LG_OVERBRAND) * 80 : 0);
	RE_LVL_DMOD(100);
}

void SkillMoonSlasher::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, src, SC_OVERBRANDREADY, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillMoonSlasher::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillOverBrand::SkillOverBrand() : SkillImplRecursiveDamageSplash(LG_OVERBRAND) {
}

void SkillOverBrand::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillOverBrand::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);

	if(sc && sc->getSCE(SC_OVERBRANDREADY))
		skillratio += -100 + 500 * skill_lv;
	else
		skillratio += -100 + 350 * skill_lv;
	skillratio += ((sd) ? pc_checkskill(sd, CR_SPEARQUICKEN) * 50 : 0);
	RE_LVL_DMOD(100);
}

SkillOverSlash::SkillOverSlash() : SkillImplRecursiveDamageSplash(IG_OVERSLASH) {
}

void SkillOverSlash::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	if (dmg.miscflag >= 4) {
		dmg.div_ = 7;
	} else if (dmg.miscflag >= 2) {
		dmg.div_ = 5;
	}
}

void SkillOverSlash::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);
	int32 i;

	skillratio += -100 + 260 * skill_lv;
	skillratio += pc_checkskill(sd, IG_SPEAR_SWORD_M) * 60 * skill_lv;
	skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
	if ((i = pc_checkskill_imperial_guard(sd, 3)) > 0)
		skillratio += skillratio * i / 100;
}

void SkillOverSlash::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_area_temp[0] = map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, BCT_ENEMY, skill_area_sub_count);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillPhantomThrust::SkillPhantomThrust() : WeaponSkillImpl(RK_PHANTOMTHRUST) {
}

void SkillPhantomThrust::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	// ATK = [{(Skill Level x 50) + (Spear Master Level x 10)} x Caster's Base Level / 150] %
	skillratio += -100 + 50 * skill_lv + 10 * (sd ? pc_checkskill(sd,KN_SPEARMASTERY) : 5);
	RE_LVL_DMOD(150); // Base level bonus.
}

void SkillPhantomThrust::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	unit_setdir(src,map_calc_dir(src, target->x, target->y));
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);

	skill_blown(src,target,distance_bl(src,target)-1,unit_getdir(src),BLOWN_NONE);
	if( battle_check_target(src,target,BCT_ENEMY) > 0 )
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillPierce::SkillPierce() : WeaponSkillImpl(KN_PIERCE) {
}

void SkillPierce::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_data* tstatus = status_get_status_data(target);

	dmg.div_= (dmg.div_> 0 ? tstatus->size+1 : -(tstatus->size+1));
}

void SkillPierce::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);

	base_skillratio += 10 * skill_lv;

	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		base_skillratio *= 2;
}

void SkillPierce::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 5 * skill_lv / 100;
}

SkillPiety::SkillPiety() : SkillImpl(LG_PIETY) {
}

void SkillPiety::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1) {
		sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		skill_area_temp[2] = 0;
		map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_PC,
			src, getSkillId(), skill_lv, tick, flag | SD_PREAMBLE | BCT_PARTY | BCT_SELF | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillPinpointAttack::SkillPinpointAttack() : WeaponSkillImpl(LG_PINPOINTATTACK) {
}

void SkillPinpointAttack::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (skill_check_unit_movepos(5, src, target->x, target->y, 1, 1)) {
		clif_blown(src);
	}

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillPinpointAttack::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += -100 + 100 * skill_lv + 5 * status_get_agi(src);
	RE_LVL_DMOD(120);
}

void SkillPinpointAttack::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 rate = 30 + 5 * ((sd) ? pc_checkskill(sd, getSkillId()) : skill_lv) + (status_get_agi(src) + status_get_lv(src)) / 10;

	switch (skill_lv) {
		case 1:
			sc_start2(src, target, SC_BLEEDING, rate, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv));
			break;
		case 2:
			skill_break_equip(src, target, EQP_HELM, rate * 100, BCT_ENEMY);
			break;
		case 3:
			skill_break_equip(src, target, EQP_SHIELD, rate * 100, BCT_ENEMY);
			break;
		case 4:
			skill_break_equip(src, target, EQP_ARMOR, rate * 100, BCT_ENEMY);
			break;
		case 5:
			skill_break_equip(src, target, EQP_WEAPON, rate * 100, BCT_ENEMY);
			break;
	}
}

SkillProvoke::SkillProvoke() : SkillImpl(SM_PROVOKE)
{
}

void SkillProvoke::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	sc_type type = skill_get_sc(getSkillId());
	status_data *tstatus = status_get_status_data(*bl);
	map_session_data *sd = BL_CAST(BL_PC, src);
	mob_data *dstmd = BL_CAST(BL_MOB, bl);

	if (status_has_mode(tstatus, MD_STATUSIMMUNE) || battle_check_undead(tstatus->race, tstatus->def_ele))
	{
		return;
	}
	// Official chance is 70% + 3%*skill_lv + srcBaseLevel% - tarBaseLevel%
	int32 success = sc_start(src, bl, type, 70 + 3 * skill_lv + status_get_lv(src) - status_get_lv(bl), skill_lv, skill_get_time(getSkillId(), skill_lv));
	if (!success)
	{
		if (sd)
			clif_skill_fail(*sd, getSkillId());
		return;
	}
	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv, success != 0);
	unit_skillcastcancel(bl, 2);

	if (dstmd)
	{
		dstmd->state.provoke_flag = src->id;
		mob_target(dstmd, src, skill_get_range2(src, getSkillId(), skill_lv, true));
	}
	// Provoke can cause Coma even though it's a nodamage skill
	if (sd && battle_check_coma(*sd, *bl, BF_MISC))
		status_change_start(src, bl, SC_COMA, 10000, skill_lv, 0, src->id, 0, 0, SCSTART_NONE);
}

SkillRadiantSpear::SkillRadiantSpear() : SkillImplRecursiveDamageSplash(IG_RADIANT_SPEAR) {
}

void SkillRadiantSpear::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 3500 + 1150 * skill_lv;
	skillratio += pc_checkskill(sd, IG_SPEAR_SWORD_M) * 50;
	skillratio += 5 * sstatus->pow;	// !TODO: check POW ratio

	if (sc != nullptr && sc->getSCE(SC_SPEAR_SCAR))
		skillratio += 250 * skill_lv;

	RE_LVL_DMOD(100);
}

void SkillRadiantSpear::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillRageBurst::SkillRageBurst() : WeaponSkillImpl(LG_RAGEBURST) {
}

void SkillRageBurst::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && sd->spiritball_old) {
		skillratio += -100 + 200 * sd->spiritball_old + (status_get_max_hp(src) - status_get_hp(src)) / 100;
	} else {
		skillratio += 2900 + (status_get_max_hp(src) - status_get_hp(src));
	}

	RE_LVL_DMOD(100);
}

SkillRayOfGenesis::SkillRayOfGenesis() : SkillImplRecursiveDamageSplash(LG_RAYOFGENESIS) {
}

void SkillRayOfGenesis::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillRayOfGenesis::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 350 * skill_lv;
	skillratio += sstatus->int_ * 3;
	RE_LVL_DMOD(100);
}

void SkillRayOfGenesis::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data* tstatus = status_get_status_data(*target);

	// 50% chance to cause Blind on Undead and Demon monsters.
	if ( battle_check_undead(tstatus->race, tstatus->def_ele) || tstatus->race == RC_DEMON )
		sc_start(src,target, SC_BLIND, 50, skill_lv, skill_get_time(getSkillId(),skill_lv));
}

void SkillRayOfGenesis::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_INSPIRATION))
		element = ELE_NEUTRAL;
}

SkillRefresh::SkillRefresh() : SkillImpl(RK_REFRESH) {
}

void SkillRefresh::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
		if (pc_checkskill(sd, RK_RUNEMASTERY) >= 8) {
			if (sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)))
				clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		} else
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillRelax::SkillRelax() : SkillImpl(LK_TENSIONRELAX) {
}

void SkillRelax::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start4(src,target,type,100,skill_lv,0,0,skill_get_time2(getSkillId(),skill_lv),
			skill_get_time(getSkillId(),skill_lv)));
}

SkillResistantSouls::SkillResistantSouls() : SkillImpl(CR_PROVIDENCE) {
}

void SkillResistantSouls::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if(sd && dstsd){ //Check they are not another crusader [Skotlex]
		if ((dstsd->class_&MAPID_SECONDMASK) == MAPID_CRUSADER) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start(src,target,skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillSacrifice::SkillSacrifice() : SkillImpl(CR_DEVOTION) {
}

void SkillSacrifice::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (!sd) {
		return;
	}

	sc_type type = skill_get_sc(getSkillId());

	int32 count, lv;
	if( !dstsd )
	{ // Only players can be devoted
		clif_skill_fail( *sd, getSkillId() );
		return;
	}

	if( (lv = status_get_lv(src) - dstsd->status.base_level) < 0 )
		lv = -lv;
	if( lv > battle_config.devotion_level_difference || // Level difference requeriments
		(dstsd->sc.getSCE(type) && dstsd->sc.getSCE(type)->val1 != src->id) || // Cannot Devote a player devoted from another source
		(dstsd->class_&MAPID_SECONDMASK) == MAPID_CRUSADER || // Crusader Cannot be devoted
		(dstsd->sc.getSCE(SC_HELLPOWER))) // Players affected by SC_HELLPOWER cannot be devoted.
	{
		clif_skill_fail( *sd, getSkillId() );
		return;
	}

	int32 i = 0;
	count = min(skill_lv,MAX_DEVOTION);

	ARR_FIND(0, count, i, sd->devotion[i] == target->id );
	if( i == count )
	{
		ARR_FIND(0, count, i, sd->devotion[i] == 0 );
		if( i == count )
		{ // No free slots, skill Fail
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
	}

	sd->devotion[i] = target->id;

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
		sc_start4(src, target, type, 10000, src->id, i, skill_get_range2(src, getSkillId(), skill_lv, true), 0, skill_get_time2(getSkillId(), skill_lv)));
	clif_devotion(src, nullptr);
}

SkillProvokeSelf::SkillProvokeSelf() : SkillImpl(SM_SELFPROVOKE)
{
}

void SkillProvokeSelf::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	sc_type type = skill_get_sc(getSkillId());
	status_data *tstatus = status_get_status_data(*bl);
	map_session_data *sd = BL_CAST(BL_PC, src);
	mob_data *dstmd = BL_CAST(BL_MOB, bl);

	if (status_has_mode(tstatus, MD_STATUSIMMUNE) || battle_check_undead(tstatus->race, tstatus->def_ele))
	{
		return;
	}

	int32 success = sc_start(src, bl, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	if (!success)
	{
		if (sd)
			clif_skill_fail(*sd, getSkillId());
		return;
	}
	clif_skill_nodamage(src, *bl, SM_PROVOKE, skill_lv, success != 0);
	unit_skillcastcancel(bl, 2);

	if (dstmd)
	{
		dstmd->state.provoke_flag = src->id;
		mob_target(dstmd, src, skill_get_range2(src, getSkillId(), skill_lv, true));
	}
	// Provoke can cause Coma even though it's a nodamage skill
	if (sd && battle_check_coma(*sd, *bl, BF_MISC))
		status_change_start(src, bl, SC_COMA, 10000, skill_lv, 0, src->id, 0, 0, SCSTART_NONE);
}

SkillServantWeapon::SkillServantWeapon() : SkillImpl(DK_SERVANTWEAPON) {
}

void SkillServantWeapon::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv)));
}

SkillServantWeaponAttack::SkillServantWeaponAttack() : SkillImplRecursiveDamageSplash(DK_SERVANTWEAPON_ATK) {
}

void SkillServantWeaponAttack::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 600 + 850 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

SkillServantWeaponDemolition::SkillServantWeaponDemolition() : SkillImplRecursiveDamageSplash(DK_SERVANT_W_DEMOL) {
}

void SkillServantWeaponDemolition::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && (sd->servantball + sd->servantball_old) < dmg.div_)
		dmg.div_ = sd->servantball + sd->servantball_old;
}

void SkillServantWeaponDemolition::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillServantWeaponDemolition::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 500 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

int64 SkillServantWeaponDemolition::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	status_change* tsc = status_get_sc(target);

	// Servant Weapon - Demol only hits if the target is marked with a sign by the attacking caster.
	if (!(tsc && tsc->getSCE(SC_SERVANT_SIGN) && tsc->getSCE(SC_SERVANT_SIGN)->val1 == src->id))
		return 0;

	return SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag);
}

SkillServantWeaponPhantom::SkillServantWeaponPhantom() : SkillImplRecursiveDamageSplash(DK_SERVANT_W_PHANTOM) {
}

void SkillServantWeaponPhantom::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && (sd->servantball + sd->servantball_old) < dmg.div_)
		dmg.div_ = sd->servantball + sd->servantball_old;
}

void SkillServantWeaponPhantom::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 200 + 300 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillServantWeaponPhantom::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_HANDICAPSTATE_DEEPBLIND, 30 + 10 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillServantWeaponPhantom::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	status_change* tsc = status_get_sc(target);

	// Jump to the target before attacking.
	if (skill_check_unit_movepos(5, src, target->x, target->y, 0, 1))
		skill_blown(src, src, 1, (map_calc_dir(target, src->x, src->y) + 4) % 8, BLOWN_NONE);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);// Trigger animation on servants.
	clif_blown(src);

	// Deal no damage if no Servant Sign on Enemy
	if (tsc == nullptr || !tsc->hasSCE(SC_SERVANT_SIGN) || tsc->getSCE(SC_SERVANT_SIGN)->val1 != src->id)
		return;

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillServantWeaponSign::SkillServantWeaponSign() : SkillImpl(DK_SERVANT_W_SIGN) {
}

void SkillServantWeaponSign::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	mob_data* md = BL_CAST(BL_MOB, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_change* tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	// Max allowed targets to be marked.
	// Only players and monsters can be marked....I think??? [Rytech]
	// Lets only allow players and monsters to use this skill for safety reasons.
	if ((!dstsd && !dstmd) || !sd && !md) {
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		return;
	}

	// Check if the target is already marked by another source.
	if (tsc && tsc->getSCE(type) && tsc->getSCE(type)->val1 != src->id) {
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

		
	// Mark the target.
	if( sd ){
		int8 i;
		int8 count = MAX_SERVANT_SIGN;

		ARR_FIND(0, count, i, sd->servant_sign[i] == target->id);
		if (i == count) {
			ARR_FIND(0, count, i, sd->servant_sign[i] == 0);
			if (i == count) { // Max number of targets marked. Fail the skill.
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
				flag |= SKILL_NOCONSUME_REQ;
				return;
			}

			// Add the ID of the marked target to the player's sign list.
			sd->servant_sign[i] = target->id;
		}

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		sc_start4(src, target, type, 100, src->id, i, skill_lv, 0, skill_get_time(getSkillId(), skill_lv));
	} else if (md) // Monster's cant track with this skill. Just give the status.
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start4(src, target, type, 100, 0, 0, skill_lv, 0, skill_get_time(getSkillId(), skill_lv)));
}

SkillShieldBoomerang::SkillShieldBoomerang() : WeaponSkillImpl(CR_SHIELDBOOMERANG) {
}

void SkillShieldBoomerang::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += -100 + skill_lv * 80;
#else
	base_skillratio += 30 * skill_lv;
#endif
}

void SkillShieldBoomerang::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
#ifdef RENEWAL
	// flag 1 means the element should be calculated for damage only
	if (flag & 1)
		element = ELE_NEUTRAL;
#endif
}

SkillShieldChain::SkillShieldChain() : WeaponSkillImpl(PA_SHIELDCHAIN) {
}

void SkillShieldChain::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

#ifdef RENEWAL
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio = -100 + 300 + 200 * skill_lv;

	if( sd != nullptr ){
		int16 index = sd->equip_index[EQI_HAND_L];

		// Damage affected by the shield's weight and refine.
		if( index >= 0 && sd->inventory_data[index] != nullptr && sd->inventory_data[index]->type == IT_ARMOR ){
			skillratio += sd->inventory_data[index]->weight / 10 + 4 * sd->inventory.u.items_inventory[index].refine;
		}

		// Damage affected by shield mastery
		if( sc != nullptr && sc->getSCE( SC_SHIELD_POWER ) ){
			skillratio += skill_lv * 14 * pc_checkskill( sd, IG_SHIELD_MASTERY );
		}
	}

	RE_LVL_DMOD(100);
#else
	skillratio += 30 * skill_lv;
#endif
	if (sc && sc->getSCE(SC_SHIELD_POWER))// Whats the official increase? [Rytech]
		skillratio += skillratio * 50 / 100;
}

SkillShieldPress::SkillShieldPress() : WeaponSkillImpl(LG_SHIELDPRESS) {
}

void SkillShieldPress::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 200 * skill_lv;
	if (sd != nullptr) {
		// Shield Press only considers base STR without job bonus
		skillratio += sd->status.str;

		if (sc != nullptr && sc->getSCE(SC_SHIELD_POWER)) {
			skillratio += skill_lv * 15 * pc_checkskill(sd, IG_SHIELD_MASTERY);
		}

		int16 index = sd->equip_index[EQI_HAND_L];
		if (index >= 0 && sd->inventory_data[index] && sd->inventory_data[index]->type == IT_ARMOR) {
			skillratio += sd->inventory_data[index]->weight / 10;
		}
	}
	RE_LVL_DMOD(100);
}

SkillShieldReflect::SkillShieldReflect() : StatusSkillImpl(CR_REFLECTSHIELD) {
}

void SkillShieldReflect::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_change* tsc = status_get_sc(target);

	if (tsc && tsc->getSCE(SC_DARKCROW)) { // SC_DARKCROW prevents using reflecting skills
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		return;
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillShieldShooting::SkillShieldShooting() : SkillImplRecursiveDamageSplash(IG_SHIELD_SHOOTING) {
}

void SkillShieldShooting::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1000 + 3500 * skill_lv;
	skillratio += 10 * sstatus->pow;
	skillratio += skill_lv * 150 * pc_checkskill(sd, IG_SHIELD_MASTERY);
	if (sd) { // Damage affected by the shield's weight and refine. Need official formula. [Rytech]
		int16 index = sd->equip_index[EQI_HAND_L];

		if (index >= 0 && sd->inventory_data[index] && sd->inventory_data[index]->type == IT_ARMOR) {
			skillratio += (sd->inventory_data[index]->weight * 7 / 6) / 10;
			skillratio += sd->inventory.u.items_inventory[index].refine * 100;
		}
	}
	RE_LVL_DMOD(100);
}

void SkillShieldShooting::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillShieldSpell::SkillShieldSpell() : SkillImpl(LG_SHIELDSPELL) {
}

void SkillShieldSpell::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type;

	if (skill_lv == 1) {
		type = SC_SHIELDSPELL_HP;
	} else if (skill_lv == 2) {
		type = SC_SHIELDSPELL_SP;
	} else {
		type = SC_SHIELDSPELL_ATK;
	}

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillSmite::SkillSmite() : WeaponSkillImpl(CR_SHIELDCHARGE) {
}

void SkillSmite::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 20 * skill_lv;
}

void SkillSmite::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_STUN,(15+skill_lv*5),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

SkillSonicWave::SkillSonicWave() : WeaponSkillImpl(RK_SONICWAVE) {
}

void SkillSonicWave::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += -100 + 1050 + 150 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillSonicWave::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 3 * skill_lv / 100; // !TODO: Confirm the hitrate bonus
}

SkillSpearBoomerang::SkillSpearBoomerang() : WeaponSkillImpl(KN_SPEARBOOMERANG) {
}

void SkillSpearBoomerang::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 50 * skill_lv;
}

SkillSpearStab::SkillSpearStab() : SkillImpl(KN_SPEARSTAB) {
}

void SkillSpearStab::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.blewcount = 0;
}

void SkillSpearStab::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if(flag&1) {
		if (target->id==skill_area_temp[1])
			return;
		if (skill_attack(BF_WEAPON,src,src,target,getSkillId(), skill_lv, tick, SD_ANIMATION))
			skill_blown(src,target,skill_area_temp[2],-1,BLOWN_NONE);
	} else {
		int32 x=target->x,y=target->y,i,dir;
		dir = map_calc_dir(target,src->x,src->y);
		skill_area_temp[1] = target->id;
		skill_area_temp[2] = skill_get_blewcount(getSkillId(),skill_lv);
		// all the enemies between the caster and the target are hit, as well as the target
		if (skill_attack(BF_WEAPON,src,src,target, getSkillId(),skill_lv,tick,0))
			skill_blown(src,target,skill_area_temp[2],-1,BLOWN_NONE);
		for (i=0;i<4;i++) {
			map_foreachincell(skill_area_sub,target->m,x,y,BL_CHAR,
				src, getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
			x += dirx[dir];
			y += diry[dir];
		}
	}
}

void SkillSpearStab::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 20 * skill_lv;
}

SkillSpiralPierce::SkillSpiralPierce() : WeaponSkillImpl(LK_SPIRALPIERCE) {
}

void SkillSpiralPierce::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd == nullptr)
		dmg.flag = (dmg.flag&~(BF_RANGEMASK|BF_WEAPONMASK))|BF_LONG|BF_MISC;
}

void SkillSpiralPierce::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_change *sc = status_get_sc(src);

	skillratio += 50 + 50 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		skillratio *= 2;
#endif
}

void SkillSpiralPierce::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *dstsd = BL_CAST(BL_PC, target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if( dstsd || ( dstmd && !status_bl_has_mode(target,MD_STATUSIMMUNE) ) ) //Does not work on status immune
		sc_start(src,target,SC_ANKLE,100,0,skill_get_time2(getSkillId(),skill_lv));
}

void SkillSpiralPierce::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	if (src.type != BL_PC)
		element = ELE_NEUTRAL; // forced neutral for monsters
}

SkillStoneHardSkin::SkillStoneHardSkin() : SkillImpl(RK_STONEHARDSKIN) {
}

void SkillStoneHardSkin::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
		if (pc_checkskill(sd, RK_RUNEMASTERY) >= 4) {
			if (sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)))
				clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
			else
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_HP_INSUFFICIENT );
		} else
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillStormBlast::SkillStormBlast() : SkillImplRecursiveDamageSplash(RK_STORMBLAST) {
}

void SkillStormBlast::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + (((sd) ? pc_checkskill(sd,RK_RUNEMASTERY) : 0) + sstatus->str / 6) * 100; // ATK = [{Rune Mastery Skill Level + (Caster's STR / 6)} x 100] %
	RE_LVL_DMOD(100);
}

void SkillStormBlast::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillStormSlash::SkillStormSlash() : WeaponSkillImpl(DK_STORMSLASH) {
}

void SkillStormSlash::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillStormSlash::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 300 + 750 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_GIANTGROWTH) && rnd_chance(60, 100))
		skillratio *= 2;
}

SkillTrample::SkillTrample() : SkillImpl(LG_TRAMPLE) {
}

/**
 * For Royal Guard's LG_TRAMPLE
 */
static int32 skill_destroy_trap(block_list *bl, va_list ap)
{
	skill_unit *su = (skill_unit *)bl;

	nullpo_ret(su);

	std::shared_ptr<s_skill_unit_group> sg;
	t_tick tick = va_arg(ap, t_tick);

	if (su->alive && (sg = su->group) && skill_get_inf2(sg->skill_id, INF2_ISTRAP)) {
		switch( sg->unit_id ) {
			case UNT_CLAYMORETRAP:
			case UNT_FIRINGTRAP:
			case UNT_ICEBOUNDTRAP:
				map_foreachinrange(skill_trap_splash,su, skill_get_splash(sg->skill_id, sg->skill_lv), sg->bl_flag|BL_SKILL|~BCT_SELF, su,tick);
				break;
			case UNT_LANDMINE:
			case UNT_BLASTMINE:
			case UNT_SHOCKWAVE:
			case UNT_SANDMAN:
			case UNT_FLASHER:
			case UNT_FREEZINGTRAP:
			case UNT_CLUSTERBOMB:
				if (battle_config.skill_wall_check && !skill_get_nk(sg->skill_id, NK_NODAMAGE))
					map_foreachinshootrange(skill_trap_splash,su, skill_get_splash(sg->skill_id, sg->skill_lv), sg->bl_flag, su,tick);
				else
					map_foreachinallrange(skill_trap_splash,su, skill_get_splash(sg->skill_id, sg->skill_lv), sg->bl_flag, su,tick);
				break;
		}
		// Traps aren't recovered.
		skill_delunit(su);
	}

	return 0;
}

void SkillTrample::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE);

	if (rnd() % 100 < (25 + 25 * skill_lv)) {
		map_foreachinallrange(skill_destroy_trap, target, skill_get_splash(getSkillId(), skill_lv), BL_SKILL, tick);
	}

	status_change_end(target, SC_SV_ROOTTWIST);
}

SkillTraumaticBlow::SkillTraumaticBlow() : WeaponSkillImpl(LK_HEADCRUSH) {
}

void SkillTraumaticBlow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 40 * skill_lv;
}

void SkillTraumaticBlow::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (status_get_class_(target) == CLASS_BOSS) {
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		return;
	}

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillTraumaticBlow::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data* tstatus = status_get_status_data(*target);

	 // Headcrush has chance of causing Bleeding status, except on demon and undead element
	if (!(battle_check_undead(tstatus->race, tstatus->def_ele) || tstatus->race == RC_DEMON))
		sc_start2(src,target, SC_BLEEDING,50, skill_lv, src->id, skill_get_time2(getSkillId(),skill_lv));
}

SkillUltimateSacrifice::SkillUltimateSacrifice() : SkillImpl(IG_ULTIMATE_SACRIFICE) {
}

void SkillUltimateSacrifice::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	// Is the animation on this skill correct? Check if its on caster only or all affected. [Rytech]
	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) )
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	else if (sd)
	{
		status_set_hp(src, 1, 0);
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillVitalityActivation::SkillVitalityActivation() : SkillImpl(RK_VITALITYACTIVATION) {
}

void SkillVitalityActivation::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
		if (pc_checkskill(sd, RK_RUNEMASTERY) >= 2) {
			if (sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)))
				clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		} else
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillVitalStrike::SkillVitalStrike() : SkillImpl(LK_JOINTBEAT) {
}

void SkillVitalStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_change *tsc = status_get_sc(target);

	base_skillratio += 10 * skill_lv - 50;

	// The 2x damage is only for the BREAK_NECK ailment.
	if (wd->miscflag & BREAK_NECK || (tsc && tsc->getSCE(SC_JOINTBEAT) && tsc->getSCE(SC_JOINTBEAT)->val2 & BREAK_NECK))
		base_skillratio *= 2;
}

void SkillVitalStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);

	flag = 1 << rnd() % 6;
	if (flag != BREAK_NECK && tsc && tsc->getSCE(SC_JOINTBEAT) && tsc->getSCE(SC_JOINTBEAT)->val2 & BREAK_NECK)
		flag = BREAK_NECK; // Target should always receive double damage if neck is already broken
	if (skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag))
		status_change_start(src, target, SC_JOINTBEAT, (50 * (skill_lv + 1) - (270 * tstatus->str) / 100) * 10, skill_lv, flag & BREAK_FLAGS, src->id, 0, skill_get_time2(getSkillId(), skill_lv), SCSTART_NONE);
}

SkillWindCutter::SkillWindCutter() : SkillImplRecursiveDamageSplash(RK_WINDCUTTER) {
}

void SkillWindCutter::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr) {
		if (sd->status.weapon == W_1HSPEAR || sd->status.weapon == W_2HSPEAR)
			dmg.flag |= BF_LONG;

		if (sd->weapontype1 == W_2HSWORD)
			dmg.div_ = 2;
	}
}

void SkillWindCutter::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		if (sd->weapontype1 == W_2HSWORD)
			skillratio += -100 + 250 * skill_lv;
		else if (sd->weapontype1 == W_1HSPEAR || sd->weapontype1 == W_2HSPEAR)
			skillratio += -100 + 400 * skill_lv;
		else
			skillratio += -100 + 300 * skill_lv;
	} else
		skillratio += -100 + 300 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillWindCutter::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);

	if (skill_area_temp[2] == 0) {
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	}
}

std::unique_ptr<const SkillImpl> SkillFactorySwordman::create(const e_skill skill_id) const {
	switch( skill_id ){
		case CR_AUTOGUARD:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case CR_DEFENDER:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case CR_DEVOTION:
			return std::make_unique<SkillSacrifice>();
		case CR_GRANDCROSS:
			return std::make_unique<SkillGrandCross>();
		case CR_HOLYCROSS:
			return std::make_unique<SkillHolyCross>();
		case CR_PROVIDENCE:
			return std::make_unique<SkillResistantSouls>();
		case CR_REFLECTSHIELD:
			return std::make_unique<SkillShieldReflect>();
		case CR_SHIELDBOOMERANG:
			return std::make_unique<SkillShieldBoomerang>();
		case CR_SHIELDCHARGE:
			return std::make_unique<SkillSmite>();
		case CR_SHRINK:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case CR_SPEARQUICKEN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case DK_CHARGINGPIERCE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case DK_DRAGONIC_AURA:
			return std::make_unique<SkillDragonicAura>();
		case DK_DRAGONIC_BREATH:
			return std::make_unique<SkillDragonicBreath>();
		case DK_DRAGONIC_PIERCE:
			return std::make_unique<SkillDragonicPierce>();
		case DK_HACKANDSLASHER:
			return std::make_unique<SkillHackAndSlasher>();
		case DK_HACKANDSLASHER_ATK:
			return std::make_unique<SkillHackAndSlasherAttack>();
		case DK_MADNESS_CRUSHER:
			return std::make_unique<SkillMadnessCrusher>();
		case DK_SERVANTWEAPON:
			return std::make_unique<SkillServantWeapon>();
		case DK_SERVANTWEAPON_ATK:
			return std::make_unique<SkillServantWeaponAttack>();
		case DK_SERVANT_W_DEMOL:
			return std::make_unique<SkillServantWeaponDemolition>();
		case DK_SERVANT_W_PHANTOM:
			return std::make_unique<SkillServantWeaponPhantom>();
		case DK_SERVANT_W_SIGN:
			return std::make_unique<SkillServantWeaponSign>();
		case DK_STORMSLASH:
			return std::make_unique<SkillStormSlash>();
		case DK_VIGOR:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case IG_ATTACK_STANCE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case IG_CROSS_RAIN:
			return std::make_unique<SkillCrossRain>();
		case IG_GRAND_JUDGEMENT:
			return std::make_unique<SkillGrandJudgement>();
		case IG_GUARD_STANCE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case IG_GUARDIAN_SHIELD:
			return std::make_unique<SkillGuardianShield>();
		case IG_HOLY_SHIELD:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case IG_IMPERIAL_CROSS:
			return std::make_unique<SkillImperialCross>();
		case IG_IMPERIAL_PRESSURE:
			return std::make_unique<SkillImperialPressure>();
		case IG_JUDGEMENT_CROSS:
			return std::make_unique<SkillJudgementCross>();
		case IG_OVERSLASH:
			return std::make_unique<SkillOverSlash>();
		case IG_RADIANT_SPEAR:
			return std::make_unique<SkillRadiantSpear>();
		case IG_REBOUND_SHIELD:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case IG_SHIELD_SHOOTING:
			return std::make_unique<SkillShieldShooting>();
		case IG_ULTIMATE_SACRIFICE:
			return std::make_unique<SkillUltimateSacrifice>();
		case KN_AUTOCOUNTER:
			return std::make_unique<SkillCounterAttack>();
		case KN_BOWLINGBASH:
			return std::make_unique<SkillBowlingBash>();
		case KN_BRANDISHSPEAR:
			return std::make_unique<SkillBrandishSpear>();
		case KN_CHARGEATK:
			return std::make_unique<SkillChargeAttack>();
		case KN_ONEHAND:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case KN_PIERCE:
			return std::make_unique<SkillPierce>();
		case KN_SPEARBOOMERANG:
			return std::make_unique<SkillSpearBoomerang>();
		case KN_SPEARSTAB:
			return std::make_unique<SkillSpearStab>();
		case KN_TWOHANDQUICKEN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case LG_BANDING:
			return std::make_unique<SkillBanding>();
		case LG_BANISHINGPOINT:
			return std::make_unique<SkillBanishingPoint>();
		case LG_CANNONSPEAR:
			return std::make_unique<SkillCannonSpear>();
		case LG_EARTHDRIVE:
			return std::make_unique<SkillEarthDrive>();
		case LG_EXEEDBREAK:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case LG_FORCEOFVANGUARD:
			return std::make_unique<SkillForceOfVanguard>();
		case LG_HESPERUSLIT:
			return std::make_unique<SkillHesperusLit>();
		case LG_INSPIRATION:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case LG_KINGS_GRACE:
			return std::make_unique<SkillKingsGrace>();
		case LG_MOONSLASHER:
			return std::make_unique<SkillMoonSlasher>();
		case LG_OVERBRAND:
			return std::make_unique<SkillOverBrand>();
		case LG_PIETY:
			return std::make_unique<SkillPiety>();
		case LG_PINPOINTATTACK:
			return std::make_unique<SkillPinpointAttack>();
		case LG_PRESTIGE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case LG_RAGEBURST:
			return std::make_unique<SkillRageBurst>();
		case LG_RAYOFGENESIS:
			return std::make_unique<SkillRayOfGenesis>();
		case LG_REFLECTDAMAGE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case LG_SHIELDPRESS:
			return std::make_unique<SkillShieldPress>();
		case LG_SHIELDSPELL:
			return std::make_unique<SkillShieldSpell>();
		case LG_TRAMPLE:
			return std::make_unique<SkillTrample>();
		case LK_AURABLADE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case LK_BERSERK:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case LK_CONCENTRATION:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case LK_HEADCRUSH:
			return std::make_unique<SkillTraumaticBlow>();
		case LK_JOINTBEAT:
			return std::make_unique<SkillVitalStrike>();
		case LK_PARRYING:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case LK_SPIRALPIERCE:
			return std::make_unique<SkillSpiralPierce>();
		case LK_TENSIONRELAX:
			return std::make_unique<SkillRelax>();
		case PA_GOSPEL:
			return std::make_unique<SkillBattleChant>();
		case PA_PRESSURE:
			return std::make_unique<SkillGloriaDomini>();
		case PA_SACRIFICE:
			return std::make_unique<SkillMartyrsReckoning>();
		case PA_SHIELDCHAIN:
			return std::make_unique<SkillShieldChain>();
		case RK_ABUNDANCE:
			return std::make_unique<SkillAbundance>();
		case RK_CRUSHSTRIKE:
			return std::make_unique<SkillCrushStrike>();
		case RK_DEATHBOUND:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case RK_DRAGONBREATH:
			return std::make_unique<SkillDragonBreath>();
		case RK_DRAGONBREATH_WATER:
			return std::make_unique<SkillDragonBreathWater>();
		case RK_DRAGONHOWLING:
			return std::make_unique<SkillDragonHowling>();
		case RK_ENCHANTBLADE:
			return std::make_unique<SkillEnchantBlade>();
		case RK_FIGHTINGSPIRIT:
			return std::make_unique<SkillFightingSpirit>();
		case RK_GIANTGROWTH:
			return std::make_unique<SkillGiantGrowth>();
		case RK_HUNDREDSPEAR:
			return std::make_unique<SkillHundredSpear>();
		case RK_IGNITIONBREAK:
			return std::make_unique<SkillIgnitionBreak>();
		case RK_LUXANIMA:
			return std::make_unique<SkillLuxAnima>();
		case RK_MILLENNIUMSHIELD:
			return std::make_unique<SkillMilleniumShield>();
		case RK_PHANTOMTHRUST:
			return std::make_unique<SkillPhantomThrust>();
		case RK_REFRESH:
			return std::make_unique<SkillRefresh>();
		case RK_SONICWAVE:
			return std::make_unique<SkillSonicWave>();
		case RK_STONEHARDSKIN:
			return std::make_unique<SkillStoneHardSkin>();
		case RK_STORMBLAST:
			return std::make_unique<SkillStormBlast>();
		case RK_VITALITYACTIVATION:
			return std::make_unique<SkillVitalityActivation>();
		case RK_WINDCUTTER:
			return std::make_unique<SkillWindCutter>();
		case SM_AUTOBERSERK:
			return std::make_unique<SkillAutoBerserk>();
		case SM_BASH:
			return std::make_unique<SkillBash>();
		case SM_ENDURE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SM_MAGNUM:
			return std::make_unique<SkillMagnumBreak>();
		case SM_PROVOKE:
			return std::make_unique<SkillProvoke>();
		case SM_SELFPROVOKE:
			return std::make_unique<SkillProvokeSelf>();

		default:
			return nullptr;
	}
}

#endif
