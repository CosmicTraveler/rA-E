// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_merchant.hpp"

#include "map/clif.hpp"
#include "map/mob.hpp"
#include "map/status.hpp"
#include <config/core.hpp>
#include "map/pc.hpp"
#include "map/party.hpp"
#include "map/itemdb.hpp"
#include "map/script.hpp"
#include "map/skill.hpp"
#include "map/unit.hpp"
#include "map/homunculus.hpp"
#include "map/battle.hpp"
#include "map/map.hpp"
#include <common/timer.hpp>
#include "map/path.hpp"
#include "map/intif.hpp"
#include "map/npc.hpp"
#include "skill_impl.hpp"

SkillAbrBattleWarrior::SkillAbrBattleWarrior() : SkillImpl(MT_SUMMON_ABR_BATTLE_WARIOR) {
}

void SkillAbrBattleWarrior::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", MOBID_ABR_BATTLE_WARIOR, "", SZ_SMALL, AI_ABR);

	if (md) {
		md->master_id = src->id;
		md->special_state.ai = AI_ABR;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}

SkillAbrDualCannon::SkillAbrDualCannon() : SkillImpl(MT_SUMMON_ABR_DUAL_CANNON) {
}

void SkillAbrDualCannon::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", MOBID_ABR_DUAL_CANNON, "", SZ_SMALL, AI_ABR);

	if (md) {
		md->master_id = src->id;
		md->special_state.ai = AI_ABR;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}

SkillAbrInfinity::SkillAbrInfinity() : SkillImpl(MT_SUMMON_ABR_INFINITY) {
}

void SkillAbrInfinity::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", MOBID_ABR_INFINITY, "", SZ_SMALL, AI_ABR);

	if (md) {
		md->master_id = src->id;
		md->special_state.ai = AI_ABR;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}

SkillAbrMotherNet::SkillAbrMotherNet() : SkillImpl(MT_SUMMON_ABR_MOTHER_NET) {
}

void SkillAbrMotherNet::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", MOBID_ABR_MOTHER_NET, "", SZ_SMALL, AI_ABR);

	if (md) {
		md->master_id = src->id;
		md->special_state.ai = AI_ABR;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}

SkillAcidDemonstration::SkillAcidDemonstration() : WeaponSkillImpl(CR_ACIDDEMONSTRATION) {
}

void SkillAcidDemonstration::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	base_skillratio += -100 + 200 * skill_lv + sstatus->int_ + tstatus->vit; // !TODO: Confirm status bonus
	if (target->type == BL_PC)
		base_skillratio /= 2;
#endif
}

void SkillAcidDemonstration::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
#else
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
#endif
}

void SkillAcidDemonstration::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	skill_break_equip(src,target, EQP_WEAPON|EQP_ARMOR, 100*skill_lv, BCT_ENEMY);
}

// BO_ACIDIFIED_ZONE_FIRE
SkillAcidifiedZoneFire::SkillAcidifiedZoneFire() : SkillImplRecursiveDamageSplash(BO_ACIDIFIED_ZONE_FIRE) {
}

void SkillAcidifiedZoneFire::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	// All BO_ACIDIFIED_ZONE_* deal the same damage? [Rytech]
	skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		skillratio += skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			skillratio += skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

void SkillAcidifiedZoneFire::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillAcidifiedZoneFire::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (target->type == BL_PC)// Place single cell AoE if hitting a player.
		skill_castend_pos2(src, target->x, target->y, getSkillId(), skill_lv, tick, 0);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}


// BO_ACIDIFIED_ZONE_FIRE_ATK
SkillActifiedZoneFireAttack::SkillActifiedZoneFireAttack() : WeaponSkillImpl(BO_ACIDIFIED_ZONE_FIRE_ATK) {
}

void SkillActifiedZoneFireAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	// All BO_ACIDIFIED_ZONE_* deal the same damage? [Rytech]
	skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		skillratio += skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			skillratio += skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

// BO_ACIDIFIED_ZONE_GROUND
SkillAcidifiedZoneGround::SkillAcidifiedZoneGround() : SkillImplRecursiveDamageSplash(BO_ACIDIFIED_ZONE_GROUND) {
}

void SkillAcidifiedZoneGround::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	// All BO_ACIDIFIED_ZONE_* deal the same damage? [Rytech]
	skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		skillratio += skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			skillratio += skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

void SkillAcidifiedZoneGround::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillAcidifiedZoneGround::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (target->type == BL_PC)// Place single cell AoE if hitting a player.
		skill_castend_pos2(src, target->x, target->y, getSkillId(), skill_lv, tick, 0);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}


// BO_ACIDIFIED_ZONE_GROUND_ATK
SkillActifiedZoneGroundAttack::SkillActifiedZoneGroundAttack() : WeaponSkillImpl(BO_ACIDIFIED_ZONE_GROUND_ATK) {
}

void SkillActifiedZoneGroundAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	// All BO_ACIDIFIED_ZONE_* deal the same damage? [Rytech]
	skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		skillratio += skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			skillratio += skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

// BO_ACIDIFIED_ZONE_WATER
SkillAcidifiedZoneWater::SkillAcidifiedZoneWater() : SkillImplRecursiveDamageSplash(BO_ACIDIFIED_ZONE_WATER) {
}

void SkillAcidifiedZoneWater::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	// All BO_ACIDIFIED_ZONE_* deal the same damage? [Rytech]
	skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		skillratio += skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			skillratio += skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

void SkillAcidifiedZoneWater::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillAcidifiedZoneWater::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (target->type == BL_PC)// Place single cell AoE if hitting a player.
		skill_castend_pos2(src, target->x, target->y, getSkillId(), skill_lv, tick, 0);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}


// BO_ACIDIFIED_ZONE_WATER_ATK
SkillActifiedZoneWaterAttack::SkillActifiedZoneWaterAttack() : WeaponSkillImpl(BO_ACIDIFIED_ZONE_WATER_ATK) {
}

void SkillActifiedZoneWaterAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	// All BO_ACIDIFIED_ZONE_* deal the same damage? [Rytech]
	skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		skillratio += skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			skillratio += skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

// BO_ACIDIFIED_ZONE_WIND
SkillAcidifiedZoneWind::SkillAcidifiedZoneWind() : SkillImplRecursiveDamageSplash(BO_ACIDIFIED_ZONE_WIND) {
}

void SkillAcidifiedZoneWind::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	// All BO_ACIDIFIED_ZONE_* deal the same damage? [Rytech]
	skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		skillratio += skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			skillratio += skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

void SkillAcidifiedZoneWind::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillAcidifiedZoneWind::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (target->type == BL_PC)// Place single cell AoE if hitting a player.
		skill_castend_pos2(src, target->x, target->y, getSkillId(), skill_lv, tick, 0);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}


// BO_ACIDIFIED_ZONE_WIND_ATK
SkillActifiedZoneWindAttack::SkillActifiedZoneWindAttack() : WeaponSkillImpl(BO_ACIDIFIED_ZONE_WIND_ATK) {
}

void SkillActifiedZoneWindAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	// All BO_ACIDIFIED_ZONE_* deal the same damage? [Rytech]
	skillratio += -100 + 400 * skill_lv + 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_RESEARCHREPORT ) ){
		skillratio += skillratio * 50 / 100;

		if (tstatus->race == RC_FORMLESS || tstatus->race == RC_PLANT)
			skillratio += skillratio * 50 / 100;
	}

	RE_LVL_DMOD(100);
}

SkillAcidTerror::SkillAcidTerror() : WeaponSkillImpl(AM_ACIDTERROR) {
}

void SkillAcidTerror::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += -100 + 200 * skill_lv;
	if (sd && pc_checkskill(sd, AM_LEARNINGPOTION))
		base_skillratio += 100; // !TODO: What's this bonus increase?
#else
	base_skillratio += -50 + 50 * skill_lv;
#endif
}

void SkillAcidTerror::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start2(src,target,SC_BLEEDING,(skill_lv*3),skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
#ifdef RENEWAL
	if (skill_break_equip(src,target, EQP_ARMOR, (1000 * skill_lv + 500) - 1000, BCT_ENEMY))
#else
	if (skill_break_equip(src,target, EQP_ARMOR, 100*skill_get_time(getSkillId(),skill_lv), BCT_ENEMY))
#endif
		clif_emotion( *target, ET_HUK );
}

SkillAdrenalineRush::SkillAdrenalineRush() : SkillImpl(BS_ADRENALINE) {
}

void SkillAdrenalineRush::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		int32 weapontype = skill_get_weapontype(getSkillId());
		if (!weapontype || !dstsd || pc_check_weapontype(dstsd, weapontype)) {
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv,
				sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, (src == target) ? 1 : 0, skill_get_time(getSkillId(), skill_lv)));
		}
	} else if (sd) {
		party_foreachsamemap(skill_area_sub,
			sd,skill_get_splash(getSkillId(), skill_lv),
			src,getSkillId(),skill_lv,tick, flag|BCT_PARTY|1,
			skill_castend_nodamage_id);
	}
}

SkillAdvancedAdrenalineRush::SkillAdvancedAdrenalineRush() : SkillImpl(BS_ADRENALINE2) {
}

void SkillAdvancedAdrenalineRush::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		int32 weapontype = skill_get_weapontype(getSkillId());
		if (!weapontype || !dstsd || pc_check_weapontype(dstsd, weapontype)) {
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv,
				sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, (src == target) ? 1 : 0, skill_get_time(getSkillId(), skill_lv)));
		}
	} else if (sd) {
		party_foreachsamemap(skill_area_sub,
			sd,skill_get_splash(getSkillId(), skill_lv),
			src,getSkillId(),skill_lv,tick, flag|BCT_PARTY|1,
			skill_castend_nodamage_id);
	}
}

SkillAdvanceProtection::SkillAdvanceProtection() : StatusSkillImpl(BO_ADVANCE_PROTECTION) {
}

void SkillAdvanceProtection::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (dstsd == nullptr || pc_checkequip(dstsd, EQP_SHADOW_GEAR) < 0) {
		if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
			clif_skill_fail(*sd, getSkillId());

		}

		// Don't consume item requirements
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillAidBerserkPotion::SkillAidBerserkPotion() : SkillImpl(AM_BERSERKPITCHER) {
}

void SkillAidBerserkPotion::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_data* sstatus = status_get_status_data(*src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_data* tstatus = status_get_status_data(*target);
	status_change* tsc = status_get_sc(target);

	int32 j,hp = 0,sp = 0;
	if( dstmd && dstmd->mob_id == MOBID_EMPERIUM ) {
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	if( sd ) {
		int32 x,bonus=100;
		struct s_skill_condition require = skill_get_requirement(sd, getSkillId(), skill_lv);
		x = skill_lv%11 - 1;
		j = pc_search_inventory(sd, require.itemid[x]);
		if (j < 0 || require.itemid[x] <= 0) {
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
		if (sd->inventory_data[j] == nullptr || sd->inventory.u.items_inventory[j].amount < require.amount[x]) {
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
		if( dstsd && dstsd->status.base_level < (uint32)sd->inventory_data[j]->elv ) {
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
		potion_flag = 1;
		potion_hp = potion_sp = potion_per_hp = potion_per_sp = 0;
		potion_target = target->id;
		run_script(sd->inventory_data[j]->script,0,sd->id,0);
		potion_flag = potion_target = 0;
		if( sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_ALCHEMIST )
			bonus += sd->status.base_level;
		if( potion_per_hp > 0 || potion_per_sp > 0 ) {
			hp = tstatus->max_hp * potion_per_hp / 100;
			hp = hp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
			if( dstsd ) {
				sp = dstsd->status.max_sp * potion_per_sp / 100;
				sp = sp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
			}
		} else {
			if( potion_hp > 0 ) {
				hp = potion_hp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
				hp = hp * (100 + (tstatus->vit * 2)) / 100;
				if( dstsd )
					hp = hp * (100 + pc_checkskill(dstsd,SM_RECOVERY)*10) / 100;
			}
			if( potion_sp > 0 ) {
				sp = potion_sp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
				sp = sp * (100 + (tstatus->int_ * 2)) / 100;
				if( dstsd )
					sp = sp * (100 + pc_checkskill(dstsd,MG_SRECOVERY)*10) / 100;
			}
		}

		if ((bonus = pc_get_itemgroup_bonus_group(sd, IG_POTION, sd->itemgrouphealrate))) {
			hp += hp * bonus / 100;
		}

		if( ( bonus = pc_get_itemgroup_bonus_group( sd, IG_POTION, sd->itemgroupsphealrate ) ) ){
			sp += sp * bonus / 100;
		}

		if( (j = pc_skillheal_bonus(sd, getSkillId())) ) {
			hp += hp * j / 100;
			sp += sp * j / 100;
		}
	} else {
		//Maybe replace with potion_hp, but I'm unsure how that works [Playtester]
		switch (skill_lv) {
			case 1: hp = 45; break;
			case 2: hp = 105; break;
			case 3: hp = 175; break;
			default: hp = 325; break;
		}
		hp = (hp + rnd()%(skill_lv*20+1)) * (150 + skill_lv*10) / 100;
		hp = hp * (100 + (tstatus->vit * 2)) / 100;
		if( dstsd )
			hp = hp * (100 + pc_checkskill(dstsd,SM_RECOVERY)*10) / 100;
	}
	if( dstsd && (j = pc_skillheal2_bonus(dstsd, getSkillId())) ) {
		hp += hp * j / 100;
		sp += sp * j / 100;
	}
	// Final heal increased by HPlus.
	// Is this the right place for this??? [Rytech]
	// Can HPlus also affect SP recovery???
	if (sd && sstatus->hplus > 0) {
		hp += hp * sstatus->hplus / 100;
		sp += sp * sstatus->hplus / 100;
	}
	if (tsc != nullptr && !tsc->empty()) {
		uint8 penalty = 0;

		if (tsc->getSCE(SC_WATER_INSIGNIA) && tsc->getSCE(SC_WATER_INSIGNIA)->val1 == 2) {
			hp += hp / 10;
			sp += sp / 10;
		}
		if (tsc->getSCE(SC_CRITICALWOUND))
			penalty += tsc->getSCE(SC_CRITICALWOUND)->val2;
		if (tsc->getSCE(SC_DEATHHURT) && tsc->getSCE(SC_DEATHHURT)->val3)
			penalty += 20;
		if (tsc->getSCE(SC_NORECOVER_STATE))
			penalty = 100;
		if (penalty > 0) {
			hp -= hp * penalty / 100;
			sp -= sp * penalty / 100;
		}
	}

#ifdef RENEWAL
	if (target->type == BL_HOM)
		hp *= 3; // Heal effectiveness is 3x for Homunculus
#endif

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if( hp > 0 )
		clif_skill_nodamage(nullptr,*target,AL_HEAL,hp,1);
	if( sp > 0 )
		clif_skill_nodamage(nullptr,*target,MG_SRECOVERY,sp);
	if (tsc) {
#ifdef RENEWAL
		if (tsc->getSCE(SC_EXTREMITYFIST))
			sp = 0;
#endif
		if (tsc->getSCE(SC_NORECOVER_STATE)) {
			hp = 0;
			sp = 0;
		}
	}
	status_heal(target,hp,sp,0);
}

SkillAidCondensedPotion::SkillAidCondensedPotion() : SkillImpl(CR_SLIMPITCHER) {
}

void SkillAidCondensedPotion::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	map_session_data* dstsd = BL_CAST( BL_PC, target );
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);

	// Updated to block Slim Pitcher from working on barricades and guardian stones.
	if (dstmd && (dstmd->mob_id == MOBID_EMPERIUM || status_get_class_(target) == CLASS_BATTLEFIELD))
		return;
	if (potion_hp || potion_sp) {
		int32 hp = potion_hp, sp = potion_sp;
		hp = hp * (100 + (tstatus->vit * 2))/100;
		sp = sp * (100 + (tstatus->int_ * 2))/100;
		if (dstsd) {
			if (hp)
				hp = hp * (100 + pc_checkskill(dstsd,SM_RECOVERY)*10 + pc_skillheal2_bonus(dstsd, getSkillId()))/100;
			if (sp)
				sp = sp * (100 + pc_checkskill(dstsd,MG_SRECOVERY)*10 + pc_skillheal2_bonus(dstsd, getSkillId()))/100;
		}
		if (tsc != nullptr && !tsc->empty()) {
			uint8 penalty = 0;

			if (tsc->getSCE(SC_WATER_INSIGNIA) && tsc->getSCE(SC_WATER_INSIGNIA)->val1 == 2) {
				hp += hp / 10;
				sp += sp / 10;
			}
			if (tsc->getSCE(SC_CRITICALWOUND))
				penalty += tsc->getSCE(SC_CRITICALWOUND)->val2;
			if (tsc->getSCE(SC_DEATHHURT) && tsc->getSCE(SC_DEATHHURT)->val3 == 1)
				penalty += 20;
			if (tsc->getSCE(SC_NORECOVER_STATE))
				penalty = 100;
			if (penalty > 0) {
				hp -= hp * penalty / 100;
				sp -= sp * penalty / 100;
			}
		}
		if(hp > 0)
			clif_skill_nodamage(nullptr,*target,AL_HEAL,hp);
		if(sp > 0)
			clif_skill_nodamage(nullptr,*target,MG_SRECOVERY,sp);
		status_heal(target,hp,sp,0);
	}
}

void SkillAidCondensedPotion::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd) {
		int32 i_lv = 0, j = 0;
		struct s_skill_condition require = skill_get_requirement(sd, getSkillId(), skill_lv);
		i_lv = skill_lv%11 - 1;
		j = pc_search_inventory(sd, require.itemid[i_lv]);
		if (j < 0 || require.itemid[i_lv] <= 0 || sd->inventory_data[j] == nullptr || sd->inventory.u.items_inventory[j].amount < require.amount[i_lv])
		{
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
		potion_flag = 1;
		potion_hp = 0;
		potion_sp = 0;
		run_script(sd->inventory_data[j]->script,0,sd->id,0);
		potion_flag = 0;
		//Apply skill bonuses
		i_lv = pc_checkskill(sd,CR_SLIMPITCHER)*10
			+ pc_checkskill(sd,AM_POTIONPITCHER)*10
			+ pc_checkskill(sd,AM_LEARNINGPOTION)*5
			+ pc_skillheal_bonus(sd, getSkillId());

		potion_hp = potion_hp * (100+i_lv)/100;
		potion_sp = potion_sp * (100+i_lv)/100;

		// Final heal increased by HPlus.
		// Is this the right place for this??? [Rytech]
		// Can HPlus also affect SP recovery???
		status_data* sstatus = status_get_status_data(*src);

		if (sstatus && sstatus->hplus > 0) {
			potion_hp += potion_hp * sstatus->hplus / 100;
			potion_sp += potion_sp * sstatus->hplus / 100;
		}

		if(potion_hp > 0 || potion_sp > 0) {
			i_lv = skill_get_splash(getSkillId(), skill_lv);
			map_foreachinallarea(skill_area_sub,
				src->m,x-i_lv,y-i_lv,x+i_lv,y+i_lv,BL_CHAR,
				src,getSkillId(),skill_lv,tick,flag|BCT_PARTY|BCT_GUILD|1,
				skill_castend_nodamage_id);
		}
	} else {
		struct item_data *item = itemdb_search(skill_db.find(getSkillId())->require.itemid[skill_lv - 1]);
		int32 id = skill_get_max(CR_SLIMPITCHER) * 10;

		potion_flag = 1;
		potion_hp = 0;
		potion_sp = 0;
		run_script(item->script,0,src->id,0);
		potion_flag = 0;
		potion_hp = potion_hp * (100+id)/100;
		potion_sp = potion_sp * (100+id)/100;

		if(potion_hp > 0 || potion_sp > 0) {
			id = skill_get_splash(getSkillId(), skill_lv);
			map_foreachinallarea(skill_area_sub,
				src->m,x-id,y-id,x+id,y+id,BL_CHAR,
				src,getSkillId(),skill_lv,tick,flag|BCT_PARTY|BCT_GUILD|1,
					skill_castend_nodamage_id);
		}
	}
}

SkillAidPotion::SkillAidPotion() : SkillImpl(AM_POTIONPITCHER) {
}

void SkillAidPotion::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_data* sstatus = status_get_status_data(*src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_data* tstatus = status_get_status_data(*target);
	status_change* tsc = status_get_sc(target);

	int32 j,hp = 0,sp = 0;
	if( dstmd && dstmd->mob_id == MOBID_EMPERIUM ) {
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	if( sd ) {
		int32 x,bonus=100;
		struct s_skill_condition require = skill_get_requirement(sd, getSkillId(), skill_lv);
		x = skill_lv%11 - 1;
		j = pc_search_inventory(sd, require.itemid[x]);
		if (j < 0 || require.itemid[x] <= 0) {
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
		if (sd->inventory_data[j] == nullptr || sd->inventory.u.items_inventory[j].amount < require.amount[x]) {
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
		potion_flag = 1;
		potion_hp = potion_sp = potion_per_hp = potion_per_sp = 0;
		potion_target = target->id;
		run_script(sd->inventory_data[j]->script,0,sd->id,0);
		potion_flag = potion_target = 0;
		if( sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_ALCHEMIST )
			bonus += sd->status.base_level;
		if( potion_per_hp > 0 || potion_per_sp > 0 ) {
			hp = tstatus->max_hp * potion_per_hp / 100;
			hp = hp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
			if( dstsd ) {
				sp = dstsd->status.max_sp * potion_per_sp / 100;
				sp = sp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
			}
		} else {
			if( potion_hp > 0 ) {
				hp = potion_hp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
				hp = hp * (100 + (tstatus->vit * 2)) / 100;
				if( dstsd )
					hp = hp * (100 + pc_checkskill(dstsd,SM_RECOVERY)*10) / 100;
			}
			if( potion_sp > 0 ) {
				sp = potion_sp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
				sp = sp * (100 + (tstatus->int_ * 2)) / 100;
				if( dstsd )
					sp = sp * (100 + pc_checkskill(dstsd,MG_SRECOVERY)*10) / 100;
			}
		}

		if ((bonus = pc_get_itemgroup_bonus_group(sd, IG_POTION, sd->itemgrouphealrate))) {
			hp += hp * bonus / 100;
		}

		if( ( bonus = pc_get_itemgroup_bonus_group( sd, IG_POTION, sd->itemgroupsphealrate ) ) ){
			sp += sp * bonus / 100;
		}

		if( (j = pc_skillheal_bonus(sd, getSkillId())) ) {
			hp += hp * j / 100;
			sp += sp * j / 100;
		}
	} else {
		//Maybe replace with potion_hp, but I'm unsure how that works [Playtester]
		switch (skill_lv) {
			case 1: hp = 45; break;
			case 2: hp = 105; break;
			case 3: hp = 175; break;
			default: hp = 325; break;
		}
		hp = (hp + rnd()%(skill_lv*20+1)) * (150 + skill_lv*10) / 100;
		hp = hp * (100 + (tstatus->vit * 2)) / 100;
		if( dstsd )
			hp = hp * (100 + pc_checkskill(dstsd,SM_RECOVERY)*10) / 100;
	}
	if( dstsd && (j = pc_skillheal2_bonus(dstsd, getSkillId())) ) {
		hp += hp * j / 100;
		sp += sp * j / 100;
	}
	// Final heal increased by HPlus.
	// Is this the right place for this??? [Rytech]
	// Can HPlus also affect SP recovery???
	if (sd && sstatus->hplus > 0) {
		hp += hp * sstatus->hplus / 100;
		sp += sp * sstatus->hplus / 100;
	}
	if (tsc != nullptr && !tsc->empty()) {
		uint8 penalty = 0;

		if (tsc->getSCE(SC_WATER_INSIGNIA) && tsc->getSCE(SC_WATER_INSIGNIA)->val1 == 2) {
			hp += hp / 10;
			sp += sp / 10;
		}
		if (tsc->getSCE(SC_CRITICALWOUND))
			penalty += tsc->getSCE(SC_CRITICALWOUND)->val2;
		if (tsc->getSCE(SC_DEATHHURT) && tsc->getSCE(SC_DEATHHURT)->val3)
			penalty += 20;
		if (tsc->getSCE(SC_NORECOVER_STATE))
			penalty = 100;
		if (penalty > 0) {
			hp -= hp * penalty / 100;
			sp -= sp * penalty / 100;
		}
	}

#ifdef RENEWAL
	if (target->type == BL_HOM)
		hp *= 3; // Heal effectiveness is 3x for Homunculus
#endif

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if( hp > 0 || sp <= 0 )
		clif_skill_nodamage(nullptr,*target,AL_HEAL,hp,1);
	if( sp > 0 )
		clif_skill_nodamage(nullptr,*target,MG_SRECOVERY,sp);
	if (tsc) {
#ifdef RENEWAL
		if (tsc->getSCE(SC_EXTREMITYFIST))
			sp = 0;
#endif
		if (tsc->getSCE(SC_NORECOVER_STATE)) {
			hp = 0;
			sp = 0;
		}
	}
	status_heal(target,hp,sp,0);
}

SkillAlchemicalWeapon::SkillAlchemicalWeapon() : SkillImpl(AM_CP_WEAPON) {
}

void SkillAlchemicalWeapon::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if( sd && ( target->type != BL_PC || ( dstsd && pc_checkequip(dstsd,EQP_WEAPON) < 0 ) ) ){
		clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start(src,target,skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillAnalyze::SkillAnalyze() : SkillImpl(NC_ANALYZE) {
}

void SkillAnalyze::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	sc_start(src,target,skill_get_sc(getSkillId()), 30 + 12 * skill_lv,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

SkillArmCannon::SkillArmCannon() : SkillImplRecursiveDamageSplash(NC_ARMSCANNON) {
}

void SkillArmCannon::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_ABR_DUAL_CANNON))
		dmg.div_ = 2;
}

void SkillArmCannon::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += -100 + 400 + 350 * skill_lv;
	RE_LVL_DMOD(100);
}

int32 SkillArmCannon::getSplashTarget(block_list* src) const {
	return splash_target(src);
}

void SkillArmCannon::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	skill_area_temp[1] = 0;

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

void SkillArmCannon::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->state.arrow_atk > 0)
		element = sd->bonus.arrow_ele;
}

SkillAttackMachine::SkillAttackMachine() : SkillImplRecursiveDamageSplash(MT_A_MACHINE) {
}

void SkillAttackMachine::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 150 + 700 * skill_lv;
	skillratio += 5 * sstatus->pow;	// TODO : unknown pow ratio

	RE_LVL_DMOD(100);
}

SkillAxeBoomerang::SkillAxeBoomerang() : WeaponSkillImpl(NC_AXEBOOMERANG) {
}

void SkillAxeBoomerang::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += 150 + 50 * skill_lv;
	if (sd) {
		int16 index = sd->equip_index[EQI_HAND_R];

		if (index >= 0 && sd->inventory_data[index] && sd->inventory_data[index]->type == IT_WEAPON)
			skillratio += sd->inventory_data[index]->weight / 10;// Weight is divided by 10 since 10 weight in coding make 1 whole actual weight. [Rytech]
	}
	RE_LVL_DMOD(100);
}

SkillAxeStomp::SkillAxeStomp() : SkillImplRecursiveDamageSplash(MT_AXE_STOMP) {
}

void SkillAxeStomp::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->status.weapon == W_2HAXE)
		dmg.div_ = 3;
}

void SkillAxeStomp::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillAxeStomp::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 450 + 1150 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

SkillAxeTornado::SkillAxeTornado() : SkillImplRecursiveDamageSplash(NC_AXETORNADO) {
}

void SkillAxeTornado::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 200 + 180 * skill_lv + sstatus->vit * 2;
	if (sc && sc->getSCE(SC_AXE_STOMP)) {
		skillratio += 380;
	}
	RE_LVL_DMOD(100);
}

void SkillAxeTornado::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);

	if (skill_area_temp[2] == 0) {
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	}
}

SkillBackSideSlide::SkillBackSideSlide() : SkillImpl(NC_B_SIDESLIDE) {
}

void SkillBackSideSlide::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	uint8 dir = unit_getdir(src);
	skill_blown(src,target,skill_get_blewcount(getSkillId(),skill_lv),dir,BLOWN_IGNORE_NO_KNOCKBACK);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillBiochemicalHelm::SkillBiochemicalHelm() : SkillImpl(AM_CP_HELM) {
}

void SkillBiochemicalHelm::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if( sd && ( target->type != BL_PC || ( dstsd && pc_checkequip(dstsd,EQP_HEAD_TOP) < 0 ) ) ){
		clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start(src,target,skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillBionicPharmacy::SkillBionicPharmacy() : SkillImpl(BO_BIONIC_PHARMACY) {
}

void SkillBionicPharmacy::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		sd->skill_id_old = getSkillId();
		sd->skill_lv_old = skill_lv;

		clif_cooking_list( *sd, 32, getSkillId(), 1, 8 );
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillBomb::SkillBomb() : SkillImpl(AM_DEMONSTRATION) {
}

void SkillBomb::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag|=1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillBomb::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 20 * skill_lv;
}

void SkillBomb::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
#ifdef RENEWAL
	skill_break_equip(src,target, EQP_WEAPON, 300 * skill_lv, BCT_ENEMY);
#else
	skill_break_equip(src,target, EQP_WEAPON, 100*skill_lv, BCT_ENEMY);
#endif
}

SkillBoostKnuckle::SkillBoostKnuckle() : WeaponSkillImpl(NC_BOOSTKNUCKLE) {
}

void SkillBoostKnuckle::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_ABR_DUAL_CANNON))
		dmg.div_ = 2;
}

void SkillBoostKnuckle::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 260 * skill_lv + sstatus->dex; // !TODO: What's the DEX bonus?
	RE_LVL_DMOD(100);
}

SkillCallHomunculus::SkillCallHomunculus() : SkillImpl(AM_CALLHOMUN) {
}

void SkillCallHomunculus::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && !hom_call(sd))
		clif_skill_fail( *sd, getSkillId() );
#ifdef RENEWAL
	else if (sd && hom_is_active(sd->hd))
		skill_area_temp[0] = 1; // Already passed pre-cast checks
#endif
}

SkillCartCannon::SkillCartCannon() : SkillImplRecursiveDamageSplash(GN_CARTCANNON) {
}

void SkillCartCannon::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_BIONIC_WOODENWARRIOR))
		dmg.div_ = 2;
}

void SkillCartCannon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + (250 + 20 * pc_checkskill(sd, GN_REMODELING_CART)) * skill_lv + 2 * sstatus->int_ / (6 - pc_checkskill(sd, GN_REMODELING_CART));
	RE_LVL_DMOD(100);
}

void SkillCartCannon::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && pc_checkskill(sd, GN_REMODELING_CART))
		hit_rate += pc_checkskill(sd, GN_REMODELING_CART) * 4;
}

void SkillCartCannon::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

void SkillCartCannon::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->state.arrow_atk > 0)
		element = sd->bonus.arrow_ele;
}

SkillCartRevolution::SkillCartRevolution() : SkillImplRecursiveDamageSplash(MC_CARTREVOLUTION) {
}

void SkillCartRevolution::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data *sd = BL_CAST(BL_PC, src);
	base_skillratio += 50;
	if (sd && sd->cart_weight)
		base_skillratio += 100 * sd->cart_weight / sd->cart_weight_max; // +1% every 1% weight
	else if (!sd)
		base_skillratio += 100; // Max damage for non players.
}

void SkillCartRevolution::modifyHitRate(int16 &hit_rate, const block_list *src, const block_list *target, uint16 skill_lv) const {
	const map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd && pc_checkskill(sd, GN_REMODELING_CART))
		hit_rate += pc_checkskill(sd, GN_REMODELING_CART) * 4;
}

void SkillCartRevolution::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= SD_PREAMBLE; // a fake packet will be sent for the first target to be hit

	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillCartTermination::SkillCartTermination() : WeaponSkillImpl(WS_CARTTERMINATION) {
}

void SkillCartTermination::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST( BL_PC, src );

	int32 i = 10 * (16 - skill_lv);
	if (i < 1) i = 1;
	//Preserve damage ratio when max cart weight is changed.
	if (sd && sd->cart_weight)
		base_skillratio += sd->cart_weight / i * 80000 / battle_config.max_cart_weight - 100;
	else if (!sd)
		base_skillratio += 80000 / i - 100;
}

void SkillCartTermination::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_STUN,5*skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

SkillCartTornado::SkillCartTornado() : SkillImplRecursiveDamageSplash(GN_CART_TORNADO) {
}

void SkillCartTornado::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	// ATK [( Skill Level x 200 ) + ( Cart Weight / ( 150 - Caster Base STR ))] + ( Cart Remodeling Skill Level x 50 )] %
	base_skillratio += -100 + 200 * skill_lv;
	if(sd && sd->cart_weight)
		base_skillratio += sd->cart_weight / 10 / (150 - min(sd->status.str,120)) + pc_checkskill(sd,GN_REMODELING_CART) * 50;
	if (sc && sc->getSCE(SC_BIONIC_WOODENWARRIOR))
		base_skillratio *= 2;
}

void SkillCartTornado::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

void SkillCartTornado::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && pc_checkskill(sd, GN_REMODELING_CART))
		hit_rate += pc_checkskill(sd, GN_REMODELING_CART) * 4;
}

SkillChangeCart::SkillChangeCart() : SkillImpl(MC_CHANGECART) {
}

void SkillChangeCart::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillChangeMaterial::SkillChangeMaterial() : SkillImpl(GN_CHANGEMATERIAL) {
}

void SkillChangeMaterial::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		clif_skill_itemlistwindow(sd,getSkillId(),skill_lv);
	}
}

SkillColdSlower::SkillColdSlower() : SkillImplRecursiveDamageSplash(NC_COLDSLOWER) {
}

void SkillColdSlower::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change *tsc = status_get_sc(target);

	// Status chances are applied officially through a check
	// The skill first trys to give the frozen status to targets that are hit
	sc_start(src, target, SC_FREEZE, 10 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
	if (tsc && !tsc->getSCE(SC_FREEZE)) // If it fails to give the frozen status, it will attempt to give the freezing status
		sc_start(src, target, SC_FREEZING, 20 + skill_lv * 10, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillColdSlower::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += 200 + 300 * skill_lv;
	RE_LVL_DMOD(150);
}

void SkillColdSlower::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Cast center might be relevant later (e.g. for knockback direction)
	skill_area_temp[4] = x;
	skill_area_temp[5] = y;
	int32 i = skill_get_splash(getSkillId(),skill_lv);
	map_foreachinarea(skill_area_sub,src->m,x-i,y-i,x+i,y+i,BL_CHAR|BL_SKILL,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
}

SkillCrazyUproar::SkillCrazyUproar() : StatusSkillImpl(MC_LOUD) {
}

#ifdef RENEWAL
void SkillCrazyUproar::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	sc_type type = skill_get_sc(getSkillId());

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
	} else if (sd) {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
}
#endif

// GN_CRAZYWEED
SkillCrazyWeed::SkillCrazyWeed() : SkillImpl(GN_CRAZYWEED) {
}

void SkillCrazyWeed::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 area = skill_get_splash(GN_CRAZYWEED_ATK, skill_lv);
	for( int32 i = 0; i < 3 + (skill_lv/2); i++ ) {
		int32 x1 = x - area + rnd()%(area * 2 + 1);
		int32 y1 = y - area + rnd()%(area * 2 + 1);
		skill_addtimerskill(src,tick+i*150,0,x1,y1,GN_CRAZYWEED_ATK,skill_lv,-1,0);
	}
}


// GN_CRAZYWEED_ATK
SkillCrazyWeedAttack::SkillCrazyWeedAttack() : SkillImpl(GN_CRAZYWEED_ATK) {
}

void SkillCrazyWeedAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 700 + 100 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillCreateBomb::SkillCreateBomb() : SkillImpl(GN_MAKEBOMB) {
}

void SkillCreateBomb::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int32 qty = 1;
		sd->skill_id_old = getSkillId();
		sd->skill_lv_old = skill_lv;
		if( skill_lv > 1 )
			qty = 10;
		clif_cooking_list( *sd, 28, getSkillId(), qty, 5 );
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillCreeper::SkillCreeper() : SkillImpl(BO_CREEPER) {
}

void SkillCreeper::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", MOBID_BIONIC_CREEPER, "", SZ_SMALL, AI_BIONIC);

	if (md) {
		md->master_id = src->id;
		md->special_state.ai = AI_BIONIC;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}

SkillDecorateCart::SkillDecorateCart() : SkillImpl(MC_CARTDECORATE) {
}

void SkillDecorateCart::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (sd) {
		clif_SelectCart(sd);
	}
}

SkillDemonicFire::SkillDemonicFire() : SkillImplRecursiveDamageSplash(GN_DEMONIC_FIRE) {
}

void SkillDemonicFire::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if (skill_lv > 20)	// Fire expansion Lv.2
		skillratio += 10 + 20 * (skill_lv - 20) + status_get_int(src) * 10;
	else if (skill_lv > 10) { // Fire expansion Lv.1
		skillratio += 10 + 20 * (skill_lv - 10) + status_get_int(src) + ((sd) ? sd->status.job_level : 50);
		RE_LVL_DMOD(100);
	} else
		skillratio += 10 + 20 * skill_lv;
}

void SkillDemonicFire::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Ammo should be deleted right away.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillDustExplosion::SkillDustExplosion() : SkillImplRecursiveDamageSplash(BO_DUST_EXPLOSION) {
}

void SkillDustExplosion::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 500 + 620 * skill_lv;
	skillratio += 5 * sstatus->pow;	// !TODO: check POW ratio
	if (sc != nullptr && sc->hasSCE(SC_RESEARCHREPORT))
		skillratio += 50 + 210 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillDustExplosion::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillEmergencyCool::SkillEmergencyCool() : SkillImpl(NC_EMERGENCYCOOL) {
}

void SkillEmergencyCool::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (sd == nullptr) {
		return;
	}

	struct s_skill_condition req = skill_get_requirement(sd, getSkillId(), skill_lv);
	int16 limit[] = { -45, -75, -105 };
	int32 i = 0;

	for (const auto& reqItem : req.eqItem) {
		if (pc_search_inventory(sd, reqItem) != -1) {
			break;
		}
		i++;
	}

	pc_overheat(*sd, limit[(i > 2) ? 2 : i]);
}

SkillEnergyCannonade::SkillEnergyCannonade() : SkillImplRecursiveDamageSplash(MT_ENERGY_CANNONADE) {
}

void SkillEnergyCannonade::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 250 + 750 * skill_lv;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	RE_LVL_DMOD(100);
}

void SkillEnergyCannonade::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillExplosivePowder::SkillExplosivePowder() : SkillImplRecursiveDamageSplash(BO_EXPLOSIVE_POWDER) {
}

void SkillExplosivePowder::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_RESEARCHREPORT))
		dmg.div_ = 5;
}

void SkillExplosivePowder::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 500 + 650 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (sc && sc->getSCE(SC_RESEARCHREPORT))
		skillratio += 100 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillExplosivePowder::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillFawMagicDecoy::SkillFawMagicDecoy() : SkillImpl(NC_MAGICDECOY) {
}

void SkillFawMagicDecoy::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		clif_magicdecoy_list(*sd, skill_lv, x, y);
	}
}

SkillFawRemoval::SkillFawRemoval() : SkillImpl(NC_DISJOINT) {
}

void SkillFawRemoval::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (target->type != BL_MOB) {
		return;
	}

	mob_data* md = map_id2md(target->id);
	if (md && md->mob_id >= MOBID_SILVERSNIPER && md->mob_id <= MOBID_MAGICDECOY_WIND) {
		status_kill(target);
	}
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillFawSilverSniper::SkillFawSilverSniper() : SkillImpl(NC_SILVERSNIPER) {
}

void SkillFawSilverSniper::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = mob_once_spawn_sub(src, src->m, x, y, status_get_name(*src), MOBID_SILVERSNIPER, "", SZ_SMALL, AI_NONE);
	if (md) {
		md->master_id = src->id;
		md->special_state.ai = AI_FAW;
		if (md->deletetimer != INVALID_TIMER) {
			delete_timer(md->deletetimer, mob_timer_delete);
		}
		md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}

// GN_FIRE_EXPANSION
SkillFireExpansion::SkillFireExpansion() : SkillImpl(GN_FIRE_EXPANSION) {
}

void SkillFireExpansion::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	struct unit_data* ud = unit_bl2ud(src);

	if (!ud)
		return;

	auto predicate = [x, y](std::shared_ptr<s_skill_unit_group> sg) { auto* su = sg->unit; return sg->skill_id == GN_DEMONIC_FIRE && distance_xy(x, y, su->x, su->y) < 4; };
	auto it = std::find_if(ud->skillunits.begin(), ud->skillunits.end(), predicate);
	if (it != ud->skillunits.end()) {
		auto* unit_group = it->get();
		skill_unit* su = unit_group->unit;

		switch (skill_lv) {
		case 1: {
			// TODO:
			int32 duration = (int32)(unit_group->limit - DIFF_TICK(tick, unit_group->tick));

			skill_delunit(su);
			skill_unitsetting(src, GN_DEMONIC_FIRE, 1, x, y, duration);
			flag |= 1;
		}
				break;
		case 2:
			map_foreachinallarea(skill_area_sub, src->m, su->x - 2, su->y - 2, su->x + 2, su->y + 2, BL_CHAR, src, GN_DEMONIC_FIRE, skill_lv + 20, tick, flag | BCT_ENEMY | SD_LEVEL | 1, skill_castend_damage_id);
			if (su != nullptr)
				skill_delunit(su);
			break;
		case 3:
			skill_delunit(su);
			skill_unitsetting(src, GN_FIRE_EXPANSION_SMOKE_POWDER, 1, x, y, 0);
			flag |= 1;
			break;
		case 4:
			skill_delunit(su);
			skill_unitsetting(src, GN_FIRE_EXPANSION_TEAR_GAS, 1, x, y, 0);
			flag |= 1;
			break;
		case 5: {
			uint16 acid_lv = 5; // Cast at Acid Demonstration at level 5 unless the user has a higher level learned.

			if (sd && pc_checkskill(sd, CR_ACIDDEMONSTRATION) > 5)
				acid_lv = pc_checkskill(sd, CR_ACIDDEMONSTRATION);
			map_foreachinallarea(skill_area_sub, src->m, su->x - 2, su->y - 2, su->x + 2, su->y + 2, BL_CHAR, src, GN_FIRE_EXPANSION_ACID, acid_lv, tick, flag | BCT_ENEMY | SD_LEVEL | 1, skill_castend_damage_id);
			if (su != nullptr)
				skill_delunit(su);
		}
			break;
		}
	}
}


// GN_FIRE_EXPANSION_ACID
SkillFireExpansionAcid::SkillFireExpansionAcid() : SkillImplRecursiveDamageSplash(GN_FIRE_EXPANSION_ACID) {
}

void SkillFireExpansionAcid::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	skill_break_equip(src,target, EQP_WEAPON|EQP_ARMOR, 100*skill_lv, BCT_ENEMY);
}

SkillFlameLauncher::SkillFlameLauncher() : SkillImpl(NC_FLAMELAUNCHER) {
}

void SkillFlameLauncher::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_BURNING, 20 + 10 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillFlameLauncher::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += 200 + 300 * skill_lv;
	RE_LVL_DMOD(150);
}

void SkillFlameLauncher::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = target->id;
	if (battle_config.skill_eightpath_algorithm) {
		//Use official AoE algorithm
		map_foreachindir(skill_attack_area, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), 0, splash_target(src),
			skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
	} else {
		map_foreachinpath(skill_attack_area, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), splash_target(src),
			skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
	}
}

SkillFrontSideSlide::SkillFrontSideSlide() : SkillImpl(NC_F_SIDESLIDE) {
}

void SkillFrontSideSlide::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	uint8 dir = (unit_getdir(src) + 4) % 8;
	skill_blown(src, target, skill_get_blewcount(getSkillId(), skill_lv), dir, BLOWN_IGNORE_NO_KNOCKBACK);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillFullProtection::SkillFullProtection() : SkillImpl(CR_FULLPROTECTION) {
}

void SkillFullProtection::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );

	uint32 equip[] = {EQP_WEAPON, EQP_SHIELD, EQP_ARMOR, EQP_HEAD_TOP};
	int32 i_eqp, s = 0, skilltime = skill_get_time(getSkillId(),skill_lv);

	for (i_eqp = 0; i_eqp < 4; i_eqp++) {
		if( target->type != BL_PC || ( dstsd && pc_checkequip(dstsd,equip[i_eqp]) < 0 ) )
			continue;
		sc_start(src,target,(sc_type)(SC_CP_WEAPON + i_eqp),100,skill_lv,skilltime);
		s++;
	}
	if( sd && !s ){
		clif_skill_fail( *sd, getSkillId() );
		// Don't consume item requirements
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillGreed::SkillGreed() : SkillImpl(BS_GREED) {
}

void SkillGreed::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd){
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_greed,target,
			skill_get_splash(getSkillId(), skill_lv),BL_ITEM,target);
	}
}

SkillHammerFall::SkillHammerFall() : SkillImpl(BS_HAMMERFALL) {
}

void SkillHammerFall::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_addtimerskill(src, tick+1000, target->id, 0, 0, getSkillId(), skill_lv, min(20+10*skill_lv, 50+5*skill_lv), flag);
}

void SkillHammerFall::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub,
		src->m, x-i, y-i, x+i, y+i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|2,
		skill_castend_nodamage_id);
}

// GN_HELLS_PLANT
SkillHellsPlant::SkillHellsPlant() : StatusSkillImpl(GN_HELLS_PLANT) {
}

// GN_HELLS_PLANT_ATK
SkillHellsPlantAttack::SkillHellsPlantAttack() : SkillImplRecursiveDamageSplash(GN_HELLS_PLANT_ATK) {
}

void SkillHellsPlantAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target, SC_STUN,  20 + 10 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
	sc_start2(src,target, SC_BLEEDING, 5 + 5 * skill_lv, skill_lv, src->id,skill_get_time(getSkillId(), skill_lv));
}

void SkillHellsPlantAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 100 * skill_lv + sstatus->int_ * (sd ? pc_checkskill(sd, AM_CANNIBALIZE) : 5); // !TODO: Confirm INT and Cannibalize bonus
	RE_LVL_DMOD(100);
}

SkillHellTree::SkillHellTree() : SkillImpl(BO_HELLTREE) {
}

void SkillHellTree::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", MOBID_BIONIC_HELLTREE, "", SZ_SMALL, AI_BIONIC);

	if (md) {
		md->master_id = src->id;
		md->special_state.ai = AI_BIONIC;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}

SkillHomunculusResurrection::SkillHomunculusResurrection() : SkillImpl(AM_RESURRECTHOMUN) {
}

void SkillHomunculusResurrection::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd)
	{
		if (!hom_ressurect(sd, 20*skill_lv, x, y))
		{
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
	}
}

SkillHowlingOfMandragora::SkillHowlingOfMandragora() : SkillImpl(GN_MANDRAGORA) {
}

void SkillHowlingOfMandragora::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 ) {
		sc_type type = skill_get_sc(getSkillId());
		status_change *tsc = status_get_sc(target);
		status_data* tstatus = status_get_status_data(*target);

		int32 rate = 25 + (10 * skill_lv) - (tstatus->vit + tstatus->luk) / 5;

		if (rate < 10)
			rate = 10;
		if (target->type == BL_MOB || (tsc && tsc->getSCE(type)))
			return; // Don't activate if target is a monster or zap SP if target already has Mandragora active.
		if (rnd()%100 < rate) {
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			status_zap(target,0,status_get_max_sp(target) * (25 + 5 * skill_lv) / 100);
		}
	} else {
		map_foreachinallrange(skill_area_sub,target,skill_get_splash(getSkillId(),skill_lv),BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_nodamage_id);
		clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
	}
}

SkillIllusionDoping::SkillIllusionDoping() : SkillImplRecursiveDamageSplash(GN_ILLUSIONDOPING) {
}

void SkillIllusionDoping::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	if( sc_start(src,target,SC_ILLUSIONDOPING,100 - skill_lv * 10,skill_lv,skill_get_time(getSkillId(),skill_lv)) )
		sc_start(src,target,SC_HALLUCINATION,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

SkillInfraredScan::SkillInfraredScan() : SkillImpl(NC_INFRAREDSCAN) {
}

void SkillInfraredScan::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* tsc = status_get_sc(target);

	if (flag & 1) {
		status_change_end(target, SC_HIDING);
		status_change_end(target, SC_CLOAKING);
		status_change_end(target, SC_CLOAKINGEXCEED);
		status_change_end(target, SC_CAMOUFLAGE);
		status_change_end(target, SC_NEWMOON);
		if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) {// [100 - (Skill Level x 10)] %
			status_change_end(target, SC__SHADOWFORM);
		}
		sc_start(src, target, SC_INFRAREDSCAN, 10000, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE);
		map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), splash_target(src), src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
}

void SkillInfraredScan::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

SkillItemAppraisal::SkillItemAppraisal() : SkillImpl(MC_IDENTIFY) {
}

void SkillItemAppraisal::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd) {
		clif_item_identify_list(sd);
		if (sd->menuskill_id != getSkillId()) {
			// failed, dont consume anything
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
	}
}

// NC_MAGMA_ERUPTION
SkillMagmaEruption::SkillMagmaEruption() : WeaponSkillImpl(NC_MAGMA_ERUPTION) {
}

void SkillMagmaEruption::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Stun effect from 'slam'
	sc_start(src, target, SC_STUN, 90, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillMagmaEruption::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	// 'Slam' damage
	base_skillratio += 350 + 50 * skill_lv;
}

void SkillMagmaEruption::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// 1st, AoE 'slam' damage
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinarea(skill_area_sub, src->m, x-i, y-i, x+i, y+i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|SD_ANIMATION|1, skill_castend_damage_id);
	// 2nd, AoE 'eruption' unit
	skill_addtimerskill(src,tick + status_get_amotion(src) * 2,0,x,y,getSkillId(),skill_lv,0,flag);
}


// NC_MAGMA_ERUPTION_DOTDAMAGE
SkillMagmaEruptionDotDamage::SkillMagmaEruptionDotDamage() : SkillImpl(NC_MAGMA_ERUPTION_DOTDAMAGE) {
}

void SkillMagmaEruptionDotDamage::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Burning effect from 'eruption'
	sc_start4(src, target, SC_BURNING, 10 * skill_lv, skill_lv, 1000, src->id, 0, skill_get_time2(getSkillId(), skill_lv));
}

SkillMagneticField::SkillMagneticField() : SkillImpl(NC_MAGNETICFIELD) {
}

void SkillMagneticField::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1) {
		sc_start2(src, target, SC_MAGNETICFIELD, 100, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv));
	} else {
		if (map_flag_vs(src->m)) // Doesn't affect the caster in non-PVP maps [exneval]
			sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv));
		map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), splash_target(src), src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_nodamage_id);
		clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	}
}

SkillMammonite::SkillMammonite() : WeaponSkillImpl(MC_MAMMONITE) {
}

void SkillMammonite::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 50 * skill_lv;
}

SkillManufactureMachine::SkillManufactureMachine() : SkillImpl(MT_M_MACHINE) {
}

void SkillManufactureMachine::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		sd->skill_id_old = getSkillId();
		sd->skill_lv_old = skill_lv;

		clif_cooking_list( *sd, 31, getSkillId(), 1, 7 );
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillMayhemicThorns::SkillMayhemicThorns() : SkillImplRecursiveDamageSplash(BO_MAYHEMIC_THORNS) {
}

void SkillMayhemicThorns::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_RESEARCHREPORT))
		dmg.div_ = 4;
}

void SkillMayhemicThorns::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 200 + 340 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (sc != nullptr && sc->hasSCE(SC_RESEARCHREPORT))
		skillratio += 200;
	RE_LVL_DMOD(100);
}

void SkillMayhemicThorns::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillMightySmash::SkillMightySmash() : SkillImplRecursiveDamageSplash(MT_MIGHTY_SMASH) {
}

void SkillMightySmash::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_AXE_STOMP))
		dmg.div_ = 7;
}

void SkillMightySmash::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillMightySmash::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 80 + 240 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (sc && sc->getSCE(SC_AXE_STOMP)) {
		skillratio += 20;
		skillratio += 5 * sstatus->pow;
	}
	RE_LVL_DMOD(100);
}

SkillMixCooking::SkillMixCooking() : SkillImpl(GN_MIX_COOKING) {
}

void SkillMixCooking::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int32 qty = 1;
		sd->skill_id_old = getSkillId();
		sd->skill_lv_old = skill_lv;
		if( skill_lv > 1 )
			qty = 10;
		clif_cooking_list( *sd, 27, getSkillId(), qty, 6 );
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillMysteryPowder::SkillMysteryPowder() : SkillImplRecursiveDamageSplash(BO_MYSTERY_POWDER) {
}

void SkillMysteryPowder::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1500 + 4000 * skill_lv;
	skillratio += 5 * sstatus->pow;	// !TODO: check POW ratio
	RE_LVL_DMOD(100);
}

void SkillMysteryPowder::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillNeutralBarrier::SkillNeutralBarrier() : SkillImpl(NC_NEUTRALBARRIER) {
}

void SkillNeutralBarrier::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	std::shared_ptr<s_skill_unit_group> sg;

	if (sc != nullptr && sc->getSCE(SC_NEUTRALBARRIER_MASTER)) {
		skill_clear_unitgroup(src);
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	skill_clear_unitgroup(src); // To remove previous skills - cannot used combined
	if( (sg = skill_unitsetting(src,getSkillId(),skill_lv,src->x,src->y,0)) != nullptr ) {
		sc_start2(src,src,SC_NEUTRALBARRIER_MASTER,100,skill_lv,sg->group_id,skill_get_time(getSkillId(),skill_lv));
	}
}

SkillPileBunker::SkillPileBunker() : WeaponSkillImpl(NC_PILEBUNKER) {
}

void SkillPileBunker::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	if( rnd()%100 < 25 + 15*skill_lv ) {
		status_change_end(target, SC_KYRIE);
		status_change_end(target, SC_ASSUMPTIO);
		status_change_end(target, SC_STEELBODY);
		status_change_end(target, SC_GT_CHANGE);
		status_change_end(target, SC_GT_REVITALIZE);
		status_change_end(target, SC_AUTOGUARD);
		status_change_end(target, SC_REFLECTDAMAGE);
		status_change_end(target, SC_DEFENDER);
		status_change_end(target, SC_PRESTIGE);
		status_change_end(target, SC_BANDING);
		status_change_end(target, SC_MILLENNIUMSHIELD);
	}
}

void SkillPileBunker::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += 200 + 100 * skill_lv + status_get_str(src);
	RE_LVL_DMOD(100);
}

SkillPlantCultivation::SkillPlantCultivation() : SkillImpl(CR_CULTIVATION) {
}

void SkillPlantCultivation::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd) {
		if( map_count_oncell(src->m,x,y,BL_CHAR,0) > 0 )
		{
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
		clif_skill_poseffect( *src, getSkillId(), skill_lv, x, y, tick );
		if (rnd()%100 < 50) {
			clif_skill_fail( *sd, getSkillId() );
		} else {
			TBL_MOB* md = nullptr;
			int32 t, mob_id;

			if (skill_lv == 1)
				mob_id = MOBID_BLACK_MUSHROOM + rnd() % 2;
			else {
				int32 rand_val = rnd() % 100;

				if (rand_val < 30)
					mob_id = MOBID_GREEN_PLANT;
				else if (rand_val < 55)
					mob_id = MOBID_RED_PLANT;
				else if (rand_val < 80)
					mob_id = MOBID_YELLOW_PLANT;
				else if (rand_val < 90)
					mob_id = MOBID_WHITE_PLANT;
				else if (rand_val < 98)
					mob_id = MOBID_BLUE_PLANT;
				else
					mob_id = MOBID_SHINING_PLANT;
			}

			md = mob_once_spawn_sub(src, src->m, x, y, "--ja--", mob_id, "", SZ_SMALL, AI_NONE);
			if (!md)
				return;
			if ((t = skill_get_time(getSkillId(), skill_lv)) > 0)
			{
				if( md->deletetimer != INVALID_TIMER )
					delete_timer(md->deletetimer, mob_timer_delete);
				md->deletetimer = add_timer (tick + t, mob_timer_delete, md->id, 0);
			}
			mob_spawn(md);
		}
	}
}

SkillPowerfulSwing::SkillPowerfulSwing() : SkillImplRecursiveDamageSplash(MT_POWERFUL_SWING) {
}

void SkillPowerfulSwing::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 300 + 880 * skill_lv;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	if (sc && sc->getSCE(SC_AXE_STOMP))
		skillratio += 100 + 100 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillPowerfulSwing::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillPowerSwing::SkillPowerSwing() : WeaponSkillImpl(NC_POWERSWING) {
}

void SkillPowerSwing::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_ABR_BATTLE_WARIOR))
		dmg.div_ = -2;
}

void SkillPowerSwing::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target, SC_STUN, 10, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillPowerSwing::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	// According to current sources, only the str + dex gets modified by level [Akinari]
	skillratio += -100 + ((sstatus->str + sstatus->dex)/ 2) + 300 + 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_ABR_BATTLE_WARIOR)) {
		skillratio *= 2;
	}
}

SkillPowerThrust::SkillPowerThrust() : SkillImpl(BS_OVERTHRUST) {
}

void SkillPowerThrust::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		int32 weapontype = skill_get_weapontype(getSkillId());
		if (!weapontype || !dstsd || pc_check_weapontype(dstsd, weapontype)) {
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv,
				sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, (src == target) ? 1 : 0, skill_get_time(getSkillId(), skill_lv)));
		}
	} else if (sd) {
		party_foreachsamemap(skill_area_sub,
			sd,skill_get_splash(getSkillId(), skill_lv),
			src,getSkillId(),skill_lv,tick, flag|BCT_PARTY|1,
			skill_castend_nodamage_id);
	}
}

SkillPreparePotion::SkillPreparePotion() : SkillImpl(AM_PHARMACY) {
}

void SkillPreparePotion::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd) {
		clif_skill_produce_mix_list( *sd, getSkillId(), 22);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillRepair::SkillRepair() : SkillImpl(NC_REPAIR) {
}

void SkillRepair::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (sd == nullptr) {
		return;
	}

	if (!dstsd || !pc_ismadogear(dstsd)) {
		clif_skill_fail(*sd, getSkillId(), USESKILL_FAIL_TOTARGET);
		return;
	}

	int32 hp = 0;
	switch (skill_lv) {
		case 1: hp = 4; break;
		case 2: hp = 7; break;
		case 3: hp = 13; break;
		case 4: hp = 17; break;
		case 5:
		default: hp = 23; break;
	}

	int32 heal = dstsd->status.max_hp * hp / 100;
	status_heal(target, heal, 0, 2);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, heal != 0);
}

SkillRushQuake::SkillRushQuake() : SkillImplRecursiveDamageSplash(MT_RUSH_QUAKE) {
}

void SkillRushQuake::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 3600 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_FORMLESS || tstatus->race == RC_INSECT)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillRushQuake::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start( src, target, SC_RUSH_QUAKE1, 100, skill_lv, skill_get_time( getSkillId(), skill_lv ) );
}

void SkillRushQuake::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	// Jump to the target before attacking.
	if( skill_check_unit_movepos( 5, src, target->x, target->y, 0, 1 ) ){
		skill_blown( src, src, 1, direction_opposite( static_cast<enum directions>( map_calc_dir( target, src->x, src->y ) ) ), BLOWN_NONE);
	}
	clif_skill_nodamage( src, *target, getSkillId(), skill_lv); // Trigger animation
	clif_blown( src );

	// TODO: does this buff start before or after dealing damage? [Muh]
	sc_start( src, src, SC_RUSH_QUAKE2, 100, skill_lv, skill_get_time2( getSkillId(), skill_lv ) );

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillRushStrike::SkillRushStrike() : SkillImplRecursiveDamageSplash(MT_RUSH_STRIKE) {
}

void SkillRushStrike::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 650 + 3750 * skill_lv;
	skillratio += 5 * sstatus->pow; // !TODO: check POW ratio
	RE_LVL_DMOD(100);
}

void SkillRushStrike::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	// Jump to the target before attacking.
	if( skill_check_unit_movepos( 5, src, target->x, target->y, 0, 1 ) ){
		skill_blown( src, src, 1, direction_opposite( static_cast<enum directions>( map_calc_dir( target, src->x, src->y ) ) ), BLOWN_NONE);
	}
	clif_skill_nodamage( src, *target, getSkillId(), skill_lv); // Trigger animation
	clif_blown( src );

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillSelfDestruction::SkillSelfDestruction() : SkillImplRecursiveDamageSplash(NC_SELFDESTRUCTION) {
}

void SkillSelfDestruction::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr) {
		return;
	}

	if (pc_ismadogear(sd)) {
		pc_setmadogear(sd, false);
	}

	skill_area_temp[1] = 0;
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR | BL_SKILL, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	status_set_sp(src, 0, 0);
	skill_clear_unitgroup(src);
}

SkillVending::SkillVending() : SkillImpl(MC_VENDING) {
}

void SkillVending::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	if (sd) {
		// Prevent vending of GMs with unnecessary Level to trade/drop. [Skotlex]
		if (!pc_can_give_items(sd))
			clif_skill_fail(*sd, MC_VENDING);
		else {
			int32 i = 0;
			sd->state.prevend = 1;
			sd->state.workinprogress = WIP_DISABLE_ALL;
			sd->vend_skill_lv = skill_lv;
			ARR_FIND(0, MAX_CART, i, sd->cart.u.items_cart[i].nameid && sd->cart.u.items_cart[i].id == 0);
			if (i < MAX_CART) {
				// Save the cart before opening the vending UI
				sd->state.pending_vending_ui = true;
				intif_storage_save(sd, &sd->cart);
			} else {
				// Instantly open the vending UI
				sd->state.pending_vending_ui = false;
				clif_openvendingreq(*sd, 2 + skill_lv);
			}
		}
	}
}

// GN_SLINGITEM
SkillSlingItem::SkillSlingItem() : SkillImpl(GN_SLINGITEM) {
}

void SkillSlingItem::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	int32 i = 0;

	if( sd ) {
		i = sd->equip_index[EQI_AMMO];
		if( i < 0 )
			return; // No ammo.
		t_itemid ammo_id = sd->inventory_data[i]->nameid;
		if( ammo_id == 0 )
			return;
		sd->itemid = ammo_id;
		if( itemdb_group.item_exists(IG_BOMB, ammo_id) ) {
			if(battle_check_target(src,target,BCT_ENEMY) > 0) {// Only attack if the target is an enemy.
				if( ammo_id == ITEMID_PINEAPPLE_BOMB )
					map_foreachincell(skill_area_sub,target->m,target->x,target->y,BL_CHAR,src,GN_SLINGITEM_RANGEMELEEATK,skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
				else
					skill_attack(BF_WEAPON,src,src,target,GN_SLINGITEM_RANGEMELEEATK,skill_lv,tick,flag);
			} else //Otherwise, it fails, shows animation and removes items.
				clif_skill_fail( *sd, GN_SLINGITEM_RANGEMELEEATK, USESKILL_FAIL );
		} else if (itemdb_group.item_exists(IG_THROWABLE, ammo_id)) {
			switch (ammo_id) {
				case ITEMID_HP_INC_POTS_TO_THROW: // MaxHP +(500 + Thrower BaseLv * 10 / 3) and heals 1% MaxHP
					sc_start2(src, target, SC_PROMOTE_HEALTH_RESERCH, 100, 2, 1, 500000);
					status_percent_heal(target, 1, 0);
					break;
				case ITEMID_HP_INC_POTM_TO_THROW: // MaxHP +(1500 + Thrower BaseLv * 10 / 3) and heals 2% MaxHP
					sc_start2(src, target, SC_PROMOTE_HEALTH_RESERCH, 100, 2, 2, 500000);
					status_percent_heal(target, 2, 0);
					break;
				case ITEMID_HP_INC_POTL_TO_THROW: // MaxHP +(2500 + Thrower BaseLv * 10 / 3) and heals 5% MaxHP
					sc_start2(src, target, SC_PROMOTE_HEALTH_RESERCH, 100, 2, 3, 500000);
					status_percent_heal(target, 5, 0);
					break;
				case ITEMID_SP_INC_POTS_TO_THROW: // MaxSP +(Thrower BaseLv / 10 - 5)% and recovers 2% MaxSP
					sc_start2(src, target, SC_ENERGY_DRINK_RESERCH, 100, 2, 1, 500000);
					status_percent_heal(target, 0, 2);
					break;
				case ITEMID_SP_INC_POTM_TO_THROW: // MaxSP +(Thrower BaseLv / 10)% and recovers 4% MaxSP
					sc_start2(src, target, SC_ENERGY_DRINK_RESERCH, 100, 2, 2, 500000);
					status_percent_heal(target, 0, 4);
					break;
				case ITEMID_SP_INC_POTL_TO_THROW: // MaxSP +(Thrower BaseLv / 10 + 5)% and recovers 8% MaxSP
					sc_start2(src, target, SC_ENERGY_DRINK_RESERCH, 100, 2, 3, 500000);
					status_percent_heal(target, 0, 8);
					break;
				default:
					if (dstsd)
						run_script(sd->inventory_data[i]->script, 0, dstsd->id, fake_nd->id);
					break;
			}
		}
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);// This packet is received twice actually, I think it is to show the animation.
}


// GN_SLINGITEM_RANGEMELEEATK
SkillSlingItemAttack::SkillSlingItemAttack() : WeaponSkillImpl(GN_SLINGITEM_RANGEMELEEATK) {
}

void SkillSlingItemAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data* sstatus = status_get_status_data(*src);
	status_data* tstatus = status_get_status_data(*target);
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		switch( sd->itemid ) {	// Starting SCs here instead of do it in skill_additional_effect to simplify the code.
			case ITEMID_COCONUT_BOMB:
				sc_start(src,target, SC_STUN, 5 + sd->status.job_level / 2, skill_lv, 1000 * sd->status.job_level / 3);
				sc_start2(src,target, SC_BLEEDING, 3 + sd->status.job_level / 2, skill_lv, src->id, 1000 * status_get_lv(src) / 4 + sd->status.job_level / 3);
				break;
			case ITEMID_MELON_BOMB:
				sc_start4(src, target, SC_MELON_BOMB, 100, skill_lv, 20 + sd->status.job_level, 10 + sd->status.job_level / 2, 0, 1000 * status_get_lv(src) / 4);
				break;
			case ITEMID_BANANA_BOMB:
				{
					uint16 duration = (battle_config.banana_bomb_duration ? battle_config.banana_bomb_duration : 1000 * sd->status.job_level / 4);

					sc_start(src,target, SC_BANANA_BOMB_SITDOWN, status_get_lv(src) + sd->status.job_level + sstatus->dex / 6 - status_get_lv(target) - tstatus->agi / 4 - tstatus->luk / 5, skill_lv, duration);
					sc_start(src,target, SC_BANANA_BOMB, 100, skill_lv, 30000);
					break;
				}
		}
		sd->itemid = 0;
	}
}

void SkillSlingItemAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		switch( sd->itemid ) {
			case ITEMID_APPLE_BOMB:
				skillratio += 200 + status_get_str(src) + status_get_dex(src);
				break;
			case ITEMID_COCONUT_BOMB:
			case ITEMID_PINEAPPLE_BOMB:
				skillratio += 700 + status_get_str(src) + status_get_dex(src);
				break;
			case ITEMID_MELON_BOMB:
				skillratio += 400 + status_get_str(src) + status_get_dex(src);
				break;
			case ITEMID_BANANA_BOMB:
				skillratio += 777 + status_get_str(src) + status_get_dex(src);
				break;
			case ITEMID_BLACK_LUMP:
				skillratio += -100 + (status_get_str(src) + status_get_agi(src) + status_get_dex(src)) / 3;
				break;
			case ITEMID_BLACK_HARD_LUMP:
				skillratio += -100 + (status_get_str(src) + status_get_agi(src) + status_get_dex(src)) / 2;
				break;
			case ITEMID_VERY_HARD_LUMP:
				skillratio += -100 + status_get_str(src) + status_get_agi(src) + status_get_dex(src);
				break;
		}
		RE_LVL_DMOD(100);
	}
}

SkillSparkBlaster::SkillSparkBlaster() : SkillImplRecursiveDamageSplash(MT_SPARK_BLASTER) {
}

void SkillSparkBlaster::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 600 + 1400 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillSparkBlaster::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillSpecialPharmacy::SkillSpecialPharmacy() : SkillImpl(GN_S_PHARMACY) {
}

void SkillSpecialPharmacy::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int32 qty = 1;
		sd->skill_id_old = getSkillId();
		sd->skill_lv_old = skill_lv;
		clif_cooking_list( *sd, 29, getSkillId(), qty, 6 );
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillSporeExplosion::SkillSporeExplosion() : SkillImplRecursiveDamageSplash(GN_SPORE_EXPLOSION) {
}

void SkillSporeExplosion::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_SPORE_EXPLOSION, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillSporeExplosion::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 400 + 200 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_BIONIC_WOODEN_FAIRY))
		skillratio *= 2;
}

SkillStealthField::SkillStealthField() : SkillImpl(NC_STEALTHFIELD) {
}

void SkillStealthField::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* sc = status_get_sc(src);

	if (sc != nullptr && sc->getSCE(SC_STEALTHFIELD_MASTER)) {
		skill_clear_unitgroup(src);
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	skill_clear_unitgroup(src);
	std::shared_ptr<s_skill_unit_group> sg = skill_unitsetting(src, getSkillId(), skill_lv, src->x, src->y, 0);
	if (sg != nullptr) {
		sc_start2(src, src, SC_STEALTHFIELD_MASTER, 100, skill_lv, sg->group_id, skill_get_time(getSkillId(), skill_lv));
	}
}

SkillSummonFlora::SkillSummonFlora() : SkillImpl(AM_CANNIBALIZE) {
}

void SkillSummonFlora::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 summons[5] = { MOBID_G_MANDRAGORA, MOBID_G_HYDRA, MOBID_G_FLORA, MOBID_G_PARASITE, MOBID_G_GEOGRAPHER };
	int32 class_ = summons[skill_lv-1];
	enum mob_ai ai = AI_FLORA;
	mob_data *md;

	// Correct info, don't change any of this! [celest]
	md = mob_once_spawn_sub(src, src->m, x, y, status_get_name(*src), class_, "", SZ_SMALL, ai);
	if (md) {
		md->master_id = src->id;
		md->special_state.ai = ai;
		if( md->deletetimer != INVALID_TIMER )
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer (gettick() + skill_get_time(getSkillId(),skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn (md); //Now it is ready for spawning.
	}
}

SkillSummonMarineSphere::SkillSummonMarineSphere() : SkillImpl(AM_SPHEREMINE) {
}

void SkillSummonMarineSphere::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 class_ = MOBID_MARINE_SPHERE;
	enum mob_ai ai = AI_SPHERE;
	mob_data *md;

	// Correct info, don't change any of this! [celest]
	md = mob_once_spawn_sub(src, src->m, x, y, status_get_name(*src), class_, "", SZ_SMALL, ai);
	if (md) {
		md->master_id = src->id;
		md->special_state.ai = ai;
		if( md->deletetimer != INVALID_TIMER )
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer (gettick() + skill_get_time(getSkillId(),skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn (md); //Now it is ready for spawning.
	}
}

SkillSynthesizedShield::SkillSynthesizedShield() : SkillImpl(AM_CP_SHIELD) {
}

void SkillSynthesizedShield::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if( sd && ( target->type != BL_PC || ( dstsd && pc_checkequip(dstsd,EQP_SHIELD) < 0 ) ) ){
		clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start(src,target,skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillSyntheticArmor::SkillSyntheticArmor() : SkillImpl(AM_CP_ARMOR) {
}

void SkillSyntheticArmor::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if( sd && ( target->type != BL_PC || ( dstsd && pc_checkequip(dstsd,EQP_ARMOR) < 0 ) ) ){
		clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start(src,target,skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillTheWholeProtection::SkillTheWholeProtection() : SkillImpl(BO_THE_WHOLE_PROTECTION) {
}

void SkillTheWholeProtection::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		uint32 equip[] = { EQP_WEAPON, EQP_SHIELD, EQP_ARMOR, EQP_HEAD_TOP };

		for (uint8 i_eqp = 0; i_eqp < 4; i_eqp++) {
			if (target->type != BL_PC || (dstsd && pc_checkequip(dstsd, equip[i_eqp]) < 0))
				continue;
			sc_start(src, target, (sc_type)(SC_CP_WEAPON + i_eqp), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
		}
	} else if (sd) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
}

SkillThornTrap::SkillThornTrap() : SkillImpl(GN_THORNS_TRAP) {
}

void SkillThornTrap::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillThornTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillTripleLaser::SkillTripleLaser() : WeaponSkillImpl(MT_TRIPLE_LASER) {
}

void SkillTripleLaser::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 650 + 1150 * skill_lv;
	skillratio += 12 * sstatus->pow;
	RE_LVL_DMOD(100);
}

SkillTwilightAlchemy1::SkillTwilightAlchemy1() : SkillImpl(AM_TWILIGHT1) {
}

void SkillTwilightAlchemy1::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		//Prepare 200 White Potions.
		if (!skill_produce_mix(sd, getSkillId(), ITEMID_WHITE_POTION, 0, 0, 0, 200, -1))
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillTwilightAlchemy2::SkillTwilightAlchemy2() : SkillImpl(AM_TWILIGHT2) {
}

void SkillTwilightAlchemy2::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		//Prepare 200 Slim White Potions.
		if (!skill_produce_mix(sd, getSkillId(), ITEMID_WHITE_SLIM_POTION, 0, 0, 0, 200, -1))
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillTwilightAlchemy3::SkillTwilightAlchemy3() : SkillImpl(AM_TWILIGHT3) {
}

void SkillTwilightAlchemy3::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		int32 ebottle = pc_search_inventory(sd,ITEMID_EMPTY_BOTTLE);
		int16 alcohol_idx = -1, acid_idx = -1, fire_idx = -1;
		if( ebottle >= 0 )
			ebottle = sd->inventory.u.items_inventory[ebottle].amount;
		//check if you can produce all three, if not, then fail:
		if (!(alcohol_idx = skill_can_produce_mix(sd,ITEMID_ALCOHOL,-1, 100)) //100 Alcohol
			|| !(acid_idx = skill_can_produce_mix(sd,ITEMID_ACID_BOTTLE,-1, 50)) //50 Acid Bottle
			|| !(fire_idx = skill_can_produce_mix(sd,ITEMID_FIRE_BOTTLE,-1, 50)) //50 Flame Bottle
			|| ebottle < 200 //200 empty bottle are required at total.
		) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		skill_produce_mix(sd, getSkillId(), ITEMID_ALCOHOL, 0, 0, 0, 100, alcohol_idx-1);
		skill_produce_mix(sd, getSkillId(), ITEMID_ACID_BOTTLE, 0, 0, 0, 50, acid_idx-1);
		skill_produce_mix(sd, getSkillId(), ITEMID_FIRE_BOTTLE, 0, 0, 0, 50, fire_idx-1);
	}
}

SkillUpgradeWeapon::SkillUpgradeWeapon() : SkillImpl(WS_WEAPONREFINE) {
}

void SkillUpgradeWeapon::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd != nullptr ){
		clif_item_refine_list( *sd );
	}
}

SkillVaporize::SkillVaporize() : SkillImpl(AM_REST) {
}

void SkillVaporize::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		if (hom_vaporize(sd,HOM_ST_REST))
			clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		else
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillVulcanArm::SkillVulcanArm() : SkillImplRecursiveDamageSplash(NC_VULCANARM) {
}

void SkillVulcanArm::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_ABR_DUAL_CANNON))
		dmg.div_ = 2;
}

void SkillVulcanArm::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 230 * skill_lv + sstatus->dex; // !TODO: What's the DEX bonus?
	RE_LVL_DMOD(100);
}

SkillWallOfThorns::SkillWallOfThorns() : SkillImpl(GN_WALLOFTHORN) {
}

void SkillWallOfThorns::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 10 * skill_lv;
}

void SkillWallOfThorns::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Ammo should be deleted right away.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillWeaponPerfection::SkillWeaponPerfection() : SkillImpl(BS_WEAPONPERFECT) {
}

void SkillWeaponPerfection::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		int32 weapontype = skill_get_weapontype(getSkillId());
		if (!weapontype || !dstsd || pc_check_weapontype(dstsd, weapontype)) {
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv,
				sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, (src == target) ? 1 : 0, skill_get_time(getSkillId(), skill_lv)));
		}
	} else if (sd) {
		party_foreachsamemap(skill_area_sub,
			sd,skill_get_splash(getSkillId(), skill_lv),
			src,getSkillId(),skill_lv,tick, flag|BCT_PARTY|1,
			skill_castend_nodamage_id);
	}
}

SkillWeaponRepair::SkillWeaponRepair() : SkillImpl(BS_REPAIRWEAPON) {
}

void SkillWeaponRepair::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if(sd && dstsd)
		clif_item_repair_list( *sd, *dstsd, skill_lv );
}

SkillWoodenFairy::SkillWoodenFairy() : SkillImpl(BO_WOODEN_FAIRY) {
}

void SkillWoodenFairy::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", MOBID_BIONIC_WOODEN_FAIRY, "", SZ_SMALL, AI_BIONIC);

	if (md) {
		md->master_id = src->id;
		md->special_state.ai = AI_BIONIC;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}

SkillWoodenWarrior::SkillWoodenWarrior() : SkillImpl(BO_WOODENWARRIOR) {
}

void SkillWoodenWarrior::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	mob_data *md = mob_once_spawn_sub(src, src->m, src->x, src->y, "--ja--", MOBID_BIONIC_WOODENWARRIOR, "", SZ_SMALL, AI_BIONIC);

	if (md) {
		md->master_id = src->id;
		md->special_state.ai = AI_BIONIC;

		if (md->deletetimer != INVALID_TIMER)
			delete_timer(md->deletetimer, mob_timer_delete);
		md->deletetimer = add_timer(gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md->id, 0);
		mob_spawn(md);
	}
}

std::unique_ptr<const SkillImpl> SkillFactoryMerchant::create(const e_skill skill_id) const {
	switch (skill_id) {
		case AM_ACIDTERROR:
			return std::make_unique<SkillAcidTerror>();
		case AM_BERSERKPITCHER:
			return std::make_unique<SkillAidBerserkPotion>();
		case AM_CALLHOMUN:
			return std::make_unique<SkillCallHomunculus>();
		case AM_CANNIBALIZE:
			return std::make_unique<SkillSummonFlora>();
		case AM_CP_ARMOR:
			return std::make_unique<SkillSyntheticArmor>();
		case AM_CP_HELM:
			return std::make_unique<SkillBiochemicalHelm>();
		case AM_CP_SHIELD:
			return std::make_unique<SkillSynthesizedShield>();
		case AM_CP_WEAPON:
			return std::make_unique<SkillAlchemicalWeapon>();
		case AM_DEMONSTRATION:
			return std::make_unique<SkillBomb>();
		case AM_RESURRECTHOMUN:
			return std::make_unique<SkillHomunculusResurrection>();
		case AM_PHARMACY:
			return std::make_unique<SkillPreparePotion>();
		case AM_POTIONPITCHER:
			return std::make_unique<SkillAidPotion>();
		case AM_REST:
			return std::make_unique<SkillVaporize>();
		case AM_SPHEREMINE:
			return std::make_unique<SkillSummonMarineSphere>();
		case AM_TWILIGHT1:
			return std::make_unique<SkillTwilightAlchemy1>();
		case AM_TWILIGHT2:
			return std::make_unique<SkillTwilightAlchemy2>();
		case AM_TWILIGHT3:
			return std::make_unique<SkillTwilightAlchemy3>();
		case BO_ACIDIFIED_ZONE_FIRE:
			return std::make_unique<SkillAcidifiedZoneFire>();
		case BO_ACIDIFIED_ZONE_FIRE_ATK:
			return std::make_unique<SkillActifiedZoneFireAttack>();
		case BO_ACIDIFIED_ZONE_GROUND:
			return std::make_unique<SkillAcidifiedZoneGround>();
		case BO_ACIDIFIED_ZONE_GROUND_ATK:
			return std::make_unique<SkillActifiedZoneGroundAttack>();
		case BO_ACIDIFIED_ZONE_WATER:
			return std::make_unique<SkillAcidifiedZoneWater>();
		case BO_ACIDIFIED_ZONE_WATER_ATK:
			return std::make_unique<SkillActifiedZoneWaterAttack>();
		case BO_ACIDIFIED_ZONE_WIND:
			return std::make_unique<SkillAcidifiedZoneWind>();
		case BO_ACIDIFIED_ZONE_WIND_ATK:
			return std::make_unique<SkillActifiedZoneWindAttack>();
		case BO_ADVANCE_PROTECTION:
			return std::make_unique<SkillAdvanceProtection>();
		case BO_BIONIC_PHARMACY:
			return std::make_unique<SkillBionicPharmacy>();
		case BO_CREEPER:
			return std::make_unique<SkillCreeper>();
		case BO_DUST_EXPLOSION:
			return std::make_unique<SkillDustExplosion>();
		case BO_EXPLOSIVE_POWDER:
			return std::make_unique<SkillExplosivePowder>();
		case BO_HELLTREE:
			return std::make_unique<SkillHellTree>();
		case BO_MAYHEMIC_THORNS:
			return std::make_unique<SkillMayhemicThorns>();
		case BO_MYSTERY_POWDER:
			return std::make_unique<SkillMysteryPowder>();
		case BO_RESEARCHREPORT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case BO_THE_WHOLE_PROTECTION:
			return std::make_unique<SkillTheWholeProtection>();
		case BO_WOODENWARRIOR:
			return std::make_unique<SkillWoodenWarrior>();
		case BO_WOODEN_ATTACK:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case BO_WOODEN_FAIRY:
			return std::make_unique<SkillWoodenFairy>();
		case BO_WOODEN_THROWROCK:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case BS_ADRENALINE:
			return std::make_unique<SkillAdrenalineRush>();
		case BS_ADRENALINE2:
			return std::make_unique<SkillAdvancedAdrenalineRush>();
		case BS_GREED:
			return std::make_unique<SkillGreed>();
		case BS_HAMMERFALL:
			return std::make_unique<SkillHammerFall>();
		case BS_MAXIMIZE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case BS_OVERTHRUST:
			return std::make_unique<SkillPowerThrust>();
		case BS_REPAIRWEAPON:
			return std::make_unique<SkillWeaponRepair>();
		case BS_WEAPONPERFECT:
			return std::make_unique<SkillWeaponPerfection>();
		case CR_ACIDDEMONSTRATION:
			return std::make_unique<SkillAcidDemonstration>();
		case CR_CULTIVATION:
			return std::make_unique<SkillPlantCultivation>();
		case CR_FULLPROTECTION:
			return std::make_unique<SkillFullProtection>();
		case CR_SLIMPITCHER:
			return std::make_unique<SkillAidCondensedPotion>();
		case GN_BLOOD_SUCKER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case GN_CARTBOOST:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case GN_CARTCANNON:
			return std::make_unique<SkillCartCannon>();
		case GN_CART_TORNADO:
			return std::make_unique<SkillCartTornado>();
		case GN_CHANGEMATERIAL:
			return std::make_unique<SkillChangeMaterial>();
		case GN_CRAZYWEED:
			return std::make_unique<SkillCrazyWeed>();
		case GN_CRAZYWEED_ATK:
			return std::make_unique<SkillCrazyWeedAttack>();
		case GN_DEMONIC_FIRE:
			return std::make_unique<SkillDemonicFire>();
		case GN_FIRE_EXPANSION:
			return std::make_unique<SkillFireExpansion>();
		case GN_FIRE_EXPANSION_ACID:
			return std::make_unique<SkillFireExpansionAcid>();
		case GN_HELLS_PLANT:
			return std::make_unique<SkillHellsPlant>();
		case GN_HELLS_PLANT_ATK:
			return std::make_unique<SkillHellsPlantAttack>();
		case GN_ILLUSIONDOPING:
			return std::make_unique<SkillIllusionDoping>();
		case GN_MAKEBOMB:
			return std::make_unique<SkillCreateBomb>();
		case GN_MANDRAGORA:
			return std::make_unique<SkillHowlingOfMandragora>();
		case GN_MIX_COOKING:
			return std::make_unique<SkillMixCooking>();
		case GN_SLINGITEM:
			return std::make_unique<SkillSlingItem>();
		case GN_SLINGITEM_RANGEMELEEATK:
			return std::make_unique<SkillSlingItemAttack>();
		case GN_SPORE_EXPLOSION:
			return std::make_unique<SkillSporeExplosion>();
		case GN_S_PHARMACY:
			return std::make_unique<SkillSpecialPharmacy>();
		case GN_THORNS_TRAP:
			return std::make_unique<SkillThornTrap>();
		case GN_WALLOFTHORN:
			return std::make_unique<SkillWallOfThorns>();
		case MC_CARTDECORATE:
			return std::make_unique<SkillDecorateCart>();
		case MC_CARTREVOLUTION:
			return std::make_unique<SkillCartRevolution>();
		case MC_CHANGECART:
			return std::make_unique<SkillChangeCart>();
		case MC_IDENTIFY:
			return std::make_unique<SkillItemAppraisal>();
		case MC_LOUD:
			return std::make_unique<SkillCrazyUproar>();
		case MC_MAMMONITE:
			return std::make_unique<SkillMammonite>();
		case MC_VENDING:
			return std::make_unique<SkillVending>();
		case MT_AXE_STOMP:
			return std::make_unique<SkillAxeStomp>();
		case MT_A_MACHINE:
			return std::make_unique<SkillAttackMachine>();
		case MT_D_MACHINE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case MT_ENERGY_CANNONADE:
			return std::make_unique<SkillEnergyCannonade>();
		case MT_MIGHTY_SMASH:
			return std::make_unique<SkillMightySmash>();
		case MT_M_MACHINE:
			return std::make_unique<SkillManufactureMachine>();
		case MT_POWERFUL_SWING:
			return std::make_unique<SkillPowerfulSwing>();
		case MT_RUSH_QUAKE:
			return std::make_unique<SkillRushQuake>();
		case MT_RUSH_STRIKE:
			return std::make_unique<SkillRushStrike>();
		case MT_SPARK_BLASTER:
			return std::make_unique<SkillSparkBlaster>();
		case MT_SUMMON_ABR_BATTLE_WARIOR:
			return std::make_unique<SkillAbrBattleWarrior>();
		case MT_SUMMON_ABR_DUAL_CANNON:
			return std::make_unique<SkillAbrDualCannon>();
		case MT_SUMMON_ABR_INFINITY:
			return std::make_unique<SkillAbrInfinity>();
		case MT_SUMMON_ABR_MOTHER_NET:
			return std::make_unique<SkillAbrMotherNet>();
		case MT_TRIPLE_LASER:
			return std::make_unique<SkillTripleLaser>();
		case NC_ACCELERATION:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NC_ANALYZE:
			return std::make_unique<SkillAnalyze>();
		case NC_ARMSCANNON:
			return std::make_unique<SkillArmCannon>();
		case NC_AXEBOOMERANG:
			return std::make_unique<SkillAxeBoomerang>();
		case NC_AXETORNADO:
			return std::make_unique<SkillAxeTornado>();
		case NC_B_SIDESLIDE:
			return std::make_unique<SkillBackSideSlide>();
		case NC_BOOSTKNUCKLE:
			return std::make_unique<SkillBoostKnuckle>();
		case NC_COLDSLOWER:
			return std::make_unique<SkillColdSlower>();
		case NC_DISJOINT:
			return std::make_unique<SkillFawRemoval>();
		case NC_EMERGENCYCOOL:
			return std::make_unique<SkillEmergencyCool>();
		case NC_FLAMELAUNCHER:
			return std::make_unique<SkillFlameLauncher>();
		case NC_F_SIDESLIDE:
			return std::make_unique<SkillFrontSideSlide>();
		case NC_HOVERING:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NC_INFRAREDSCAN:
			return std::make_unique<SkillInfraredScan>();
		case NC_MAGICDECOY:
			return std::make_unique<SkillFawMagicDecoy>();
		case NC_MAGMA_ERUPTION:
			return std::make_unique<SkillMagmaEruption>();
		case NC_MAGMA_ERUPTION_DOTDAMAGE:
			return std::make_unique<SkillMagmaEruptionDotDamage>();
		case NC_MAGNETICFIELD:
			return std::make_unique<SkillMagneticField>();
		case NC_NEUTRALBARRIER:
			return std::make_unique<SkillNeutralBarrier>();
		case NC_PILEBUNKER:
			return std::make_unique<SkillPileBunker>();
		case NC_POWERSWING:
			return std::make_unique<SkillPowerSwing>();
		case NC_REPAIR:
			return std::make_unique<SkillRepair>();
		case NC_SELFDESTRUCTION:
			return std::make_unique<SkillSelfDestruction>();
		case NC_SHAPESHIFT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NC_SILVERSNIPER:
			return std::make_unique<SkillFawSilverSniper>();
		case NC_STEALTHFIELD:
			return std::make_unique<SkillStealthField>();
		case NC_VULCANARM:
			return std::make_unique<SkillVulcanArm>();
		case WS_CARTBOOST:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case WS_CARTTERMINATION:
			return std::make_unique<SkillCartTermination>();
		case WS_MELTDOWN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case WS_OVERTHRUSTMAX:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case WS_WEAPONREFINE:
			return std::make_unique<SkillUpgradeWeapon>();

		default:
			return nullptr;
	}
}

#endif
