// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_npc.hpp"

#include "map/battle.hpp"
#include "map/map.hpp"
#include "map/status.hpp"
#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/mob.hpp"
#include <common/utils.hpp>
#include <common/random.hpp>
#include "map/unit.hpp"
#include <config/core.hpp>
#include "map/path.hpp"
#include "skill_impl.hpp"

SkillAcidBreath::SkillAcidBreath() : SkillImpl(NPC_ACIDBREATH) {
}

void SkillAcidBreath::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_POISON,70,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillAcidBreath::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillAcidBreath::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
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

void SkillAcidBreath::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate *= 2;
}

SkillAgilityUp::SkillAgilityUp() : SkillImpl(NPC_AGIUP) {
}

void SkillAgilityUp::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target,skill_get_sc(getSkillId()),100,50,100,skill_get_time(getSkillId(), skill_lv)));
}

SkillAntiMagic::SkillAntiMagic() : SkillImpl(NPC_ANTIMAGIC) {
}

void SkillAntiMagic::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,getSkillId(),skill_get_time(getSkillId(),skill_lv)));
}

SkillAttributeChange::SkillAttributeChange() : SkillImpl(NPC_ATTRICHANGE) {
}

void SkillAttributeChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
		skill_get_time(getSkillId(), skill_lv)));
}

SkillBleeding::SkillBleeding() : WeaponSkillImpl(NPC_BLEEDING) {
}

void SkillBleeding::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_BLEEDING,(20*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillBleeding::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillBleeding2::SkillBleeding2() : WeaponSkillImpl(NPC_BLEEDING2) {
}

void SkillBleeding2::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_BLEEDING,(50+10*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillBleeding2::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillBlindAttack::SkillBlindAttack() : WeaponSkillImpl(NPC_BLINDATTACK) {
}

void SkillBlindAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_BLIND,(20*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillBlindAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillBreakArmor::SkillBreakArmor() : WeaponSkillImpl(NPC_ARMORBRAKE) {
}

void SkillBreakArmor::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	skill_break_equip(src,target, EQP_ARMOR, 150*skill_lv, BCT_ENEMY);
}

SkillBreakHelm::SkillBreakHelm() : WeaponSkillImpl(NPC_HELMBRAKE) {
}

void SkillBreakHelm::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	skill_break_equip(src,target, EQP_HELM, 150*skill_lv, BCT_ENEMY);
}

SkillBreakShield::SkillBreakShield() : WeaponSkillImpl(NPC_SHIELDBRAKE) {
}

void SkillBreakShield::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	skill_break_equip(src,target, EQP_SHIELD, 150*skill_lv, BCT_ENEMY);
}

SkillCaneOfEvilEye::SkillCaneOfEvilEye() : SkillImpl(NPC_CANE_OF_EVIL_EYE) {
}

void SkillCaneOfEvilEye::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;
	if(skill_unitsetting(src,getSkillId(),skill_lv,x,y,0))
		clif_skill_poseffect( *src, getSkillId(), skill_lv, x, y, tick );
}

SkillChangeLocation::SkillChangeLocation() : SkillImpl(NPC_MOVE_COORDINATE) {
}

void SkillChangeLocation::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int16 px = target->x, py = target->y;
	if (!skill_check_unit_movepos(0, target, src->x, src->y, 1, 1)) {
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	clif_blown(target);

	// If caster is not a boss, switch coordinates with the target
	if (status_get_class_(src) != CLASS_BOSS) {
		if (!skill_check_unit_movepos(0, src, px, py, 1, 1)) {
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		clif_blown(src);
	}
}

SkillComet2::SkillComet2() : SkillImpl(NPC_COMET) {
}

void SkillComet2::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start4(src,target,SC_BURNING,100,skill_lv,1000,src->id,0,skill_get_time2(getSkillId(),skill_lv));
}

void SkillComet2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	int32 i = (sc ? distance_xy(target->x, target->y, sc->comet_x, sc->comet_y) : 8) / 2;
	i = cap_value(i, 1, 4);
	base_skillratio = 2500 + ((skill_lv - i + 1) * 500);
}

void SkillComet2::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillCriticalWounds::SkillCriticalWounds() : WeaponSkillImpl(NPC_CRITICALWOUND) {
}

void SkillCriticalWounds::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_CRITICALWOUND,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

SkillCrossOfDarkness::SkillCrossOfDarkness() : WeaponSkillImpl(NPC_DARKCROSS) {
}

void SkillCrossOfDarkness::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_BLIND,3*skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillCrossOfDarkness::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 35 * skill_lv;
}

SkillCurseAttack::SkillCurseAttack() : WeaponSkillImpl(NPC_CURSEATTACK) {
}

void SkillCurseAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_CURSE,(20*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillCurseAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillDancingBlade::SkillDancingBlade() : SkillImpl(NPC_DANCINGBLADE) {
}

void SkillDancingBlade::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_addtimerskill(src, tick + status_get_amotion(src), target->id, 0, 0, NPC_DANCINGBLADE_ATK, skill_lv, 0, 0);
}

SkillDarkBlessing::SkillDarkBlessing() : SkillImpl(NPC_DARKBLESSING) {
}

void SkillDarkBlessing::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target,skill_get_sc(getSkillId()),(50+skill_lv*5),skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv)));
}

SkillDarkBreath::SkillDarkBreath() : SkillImpl(NPC_DARKBREATH) {
}

void SkillDarkBreath::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_emotion( *src, ET_ANGER );
	if (rnd() % 2 == 0)
		return; // 50% chance
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillDarknessBreath::SkillDarknessBreath() : SkillImpl(NPC_DARKNESSBREATH) {
}

void SkillDarknessBreath::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillDarknessBreath::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
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

void SkillDarknessBreath::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate *= 2;
}

SkillDarknessJupitel::SkillDarknessJupitel() : SkillImpl(NPC_DARKTHUNDER) {
}

void SkillDarknessJupitel::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillDarkPiercing::SkillDarkPiercing() : SkillImpl(NPC_DARKPIERCING) {
}

void SkillDarkPiercing::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
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

SkillDeadlyCurse::SkillDeadlyCurse() : SkillImpl(NPC_DEADLYCURSE) {
}

void SkillDeadlyCurse::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_start(src, target, skill_get_sc(getSkillId()), 10000, skill_lv, 0, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NOAVOID|SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillDeadlyCurse2::SkillDeadlyCurse2() : SkillImpl(NPC_DEADLYCURSE2) {
}

void SkillDeadlyCurse2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillDeathSummon::SkillDeathSummon() : SkillImpl(NPC_DEATHSUMMON) {
}

void SkillDeathSummon::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	if(md && md->skill_idx >= 0)
		mob_summonslave(md,md->db->skill[md->skill_idx]->val,skill_lv,getSkillId());
}

SkillDecreaseAllStats::SkillDecreaseAllStats() : SkillImpl(NPC_ALL_STAT_DOWN) {
}

void SkillDecreaseAllStats::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_start(src, target, skill_get_sc(getSkillId()), 10000, skill_lv, 0, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NOAVOID|SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
}

SkillDemonShockAttack::SkillDemonShockAttack() : SkillImpl(NPC_MAGICALATTACK) {
}

void SkillDemonShockAttack::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	sc_start(src,src,SC_MAGICALATTACK,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

SkillDragonFear::SkillDragonFear() : SkillImpl(NPC_DRAGONFEAR) {
}

void SkillDragonFear::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = 0;

	if (flag&1) {
		const enum sc_type sc[] = { SC_STUN, SC_SILENCE, SC_CONFUSION, SC_BLEEDING };
		int32 j;
		j = i = rnd()%ARRAYLENGTH(sc);
		while ( !sc_start2(src,target,sc[i],100,skill_lv,src->id,skill_get_time2(getSkillId(),i+1)) ) {
			i++;
			if ( i == ARRAYLENGTH(sc) )
				i = 0;
			if (i == j)
				break;
		}
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillEarthAttributeAttack::SkillEarthAttributeAttack() : WeaponSkillImpl(NPC_GROUNDATTACK) {
}

void SkillEarthAttributeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillEarthAttributeAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillEarthAttributeChange::SkillEarthAttributeChange() : SkillImpl(NPC_CHANGEGROUND) {
}

void SkillEarthAttributeChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
		skill_get_time(getSkillId(), skill_lv)));
}

SkillEarthquake::SkillEarthquake() : SkillImpl(NPC_EARTHQUAKE) {
}

void SkillEarthquake::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), -1, DMG_SINGLE );
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillEarthquake::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	element = ELE_NEUTRAL;
}

SkillEmotion::SkillEmotion() : SkillImpl(NPC_EMOTION) {
}

void SkillEmotion::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;
	mob_data* md = BL_CAST(BL_MOB, src);

	//val[0] is the emotion to use.
	//NPC_EMOTION can change a mob's mode 'permanently' [Skotlex]
	//val[1] 'sets' the mode
	//val[2] adds to the current mode
	//val[3] removes from the current mode
	//val[4] if set, asks to delete the previous mode change.
	if(md && md->skill_idx >= 0 && tsc)
	{
		clif_emotion( *target, static_cast<emotion_type>( md->db->skill[md->skill_idx]->val[0] ) );
		if(md->db->skill[md->skill_idx]->val[4] && tsce)
			status_change_end(target, type);

		//If mode gets set by NPC_EMOTION then the target should be reset [Playtester]
		if (!battle_config.npc_emotion_behavior
			&& md->state.skillstate != MSS_IDLE && md->state.skillstate != MSS_WALK
			&& md->db->skill[md->skill_idx]->val[1])
			mob_unlocktarget(md, tick);

		if(md->db->skill[md->skill_idx]->val[1] || md->db->skill[md->skill_idx]->val[2])
			sc_start4(src,src, type, 100, skill_lv,
				md->db->skill[md->skill_idx]->val[1],
				md->db->skill[md->skill_idx]->val[2],
				md->db->skill[md->skill_idx]->val[3],
				skill_get_time(getSkillId(), skill_lv));

		//Reset aggressive state depending on resulting mode
		if (!battle_config.npc_emotion_behavior)
			md->state.aggressive = status_has_mode(&md->status,MD_ANGRY)?1:0;
	}
}

SkillEmotionOn::SkillEmotionOn() : SkillImpl(NPC_EMOTION_ON) {
}

void SkillEmotionOn::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;
	mob_data* md = BL_CAST(BL_MOB, src);

	//val[0] is the emotion to use.
	//NPC_EMOTION_ON can change a mob's mode 'permanently' [Skotlex]
	//val[1] 'sets' the mode
	//val[2] adds to the current mode
	//val[3] removes from the current mode
	//val[4] if set, asks to delete the previous mode change.
	if(md && md->skill_idx >= 0 && tsc)
	{
		clif_emotion( *target, static_cast<emotion_type>( md->db->skill[md->skill_idx]->val[0] ) );
		if(md->db->skill[md->skill_idx]->val[4] && tsce)
			status_change_end(target, type);

		if(md->db->skill[md->skill_idx]->val[1] || md->db->skill[md->skill_idx]->val[2])
			sc_start4(src,src, type, 100, skill_lv,
				md->db->skill[md->skill_idx]->val[1],
				md->db->skill[md->skill_idx]->val[2],
				md->db->skill[md->skill_idx]->val[3],
				skill_get_time(getSkillId(), skill_lv));

		//Reset aggressive state depending on resulting mode
		if (!battle_config.npc_emotion_behavior)
			md->state.aggressive = status_has_mode(&md->status,MD_ANGRY)?1:0;
	}
}

SkillEnergyDrain::SkillEnergyDrain() : SkillImpl(NPC_ENERGYDRAIN) {
}

void SkillEnergyDrain::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * skill_lv;
}

void SkillEnergyDrain::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 heal = (int32)skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
	if (heal > 0){
		clif_skill_nodamage(nullptr, *src, AL_HEAL, heal);
		status_heal(src, heal, 0, 0);
	}
}

SkillEvilLand::SkillEvilLand() : SkillImpl(NPC_EVILLAND) {
}

void SkillEvilLand::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_BLIND,5*skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillEvilLand::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillExpulsion::SkillExpulsion() : SkillImpl(NPC_EXPULSION) {
}

void SkillExpulsion::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	unit_warp(target,-1,-1,-1,CLR_TELEPORT);
}

SkillFireAttributeAttack::SkillFireAttributeAttack() : WeaponSkillImpl(NPC_FIREATTACK) {
}

void SkillFireAttributeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillFireAttributeAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillFireAttributeChange::SkillFireAttributeChange() : SkillImpl(NPC_CHANGEFIRE) {
}

void SkillFireAttributeChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
		skill_get_time(getSkillId(), skill_lv)));
}

SkillFireBreath::SkillFireBreath() : SkillImpl(NPC_FIREBREATH) {
}

void SkillFireBreath::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillFireBreath::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
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

void SkillFireBreath::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate *= 2;
}

SkillFireStorm::SkillFireStorm() : SkillImpl(NPC_FIRESTORM) {
}

void SkillFireStorm::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_BURNT,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

void SkillFireStorm::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 200;
}

void SkillFireStorm::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillFireStorm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 sflag = flag;

	if( skill_lv > 1 )
		sflag |= 4;
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	map_foreachinshootrange(skill_area_sub,src,skill_get_splash(getSkillId(),skill_lv),splash_target(src),src,
		getSkillId(),skill_lv,tick,sflag|BCT_ENEMY|SD_ANIMATION|1,skill_castend_damage_id);
}

SkillFlameCross::SkillFlameCross() : SkillImpl(NPC_FLAMECROSS) {
}

void SkillFlameCross::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillFollowerSummons::SkillFollowerSummons() : SkillImpl(NPC_SUMMONSLAVE) {
}

void SkillFollowerSummons::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	if(md && md->skill_idx >= 0)
		mob_summonslave(md,md->db->skill[md->skill_idx]->val,skill_lv,getSkillId());
}

SkillFullHeal::SkillFullHeal() : SkillImpl(NPC_ALLHEAL) {
}

void SkillFullHeal::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( status_isimmune(target) )
		return;

	mob_data* dstmd = BL_CAST(BL_MOB, target);
	int32 heal = status_percent_heal(target, 100, 0);

	clif_skill_nodamage(nullptr, *target, AL_HEAL, heal);
	if( dstmd )
	{ // Reset Damage Logs
		dstmd->dmglog.clear();
	}
}

SkillGhostAttributeAttack::SkillGhostAttributeAttack() : WeaponSkillImpl(NPC_TELEKINESISATTACK) {
}

void SkillGhostAttributeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillGhostAttributeAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillGhostAttributeChange::SkillGhostAttributeChange() : SkillImpl(NPC_CHANGETELEKINESIS) {
}

void SkillGhostAttributeChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
		skill_get_time(getSkillId(), skill_lv)));
}

SkillGrandCrossOfDarkness::SkillGrandCrossOfDarkness() : SkillImpl(NPC_GRANDDARKNESS) {
}

void SkillGrandCrossOfDarkness::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_BLIND, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillGrandCrossOfDarkness::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillGroundDrive::SkillGroundDrive() : SkillImpl(NPC_GROUNDDRIVE) {
}

void SkillGroundDrive::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 25;
#endif
}

void SkillGroundDrive::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillHallucination::SkillHallucination() : SkillImpl(NPC_HALLUCINATION) {
}

void SkillHallucination::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
		sc_start(src, target, skill_get_sc(getSkillId()), skill_lv*20, skill_lv, skill_get_time2(getSkillId(), skill_lv)));
}

SkillHellBurning::SkillHellBurning() : SkillImpl(NPC_HELLBURNING) {
}

void SkillHellBurning::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 900;
}

void SkillHellBurning::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillHellDignity::SkillHellDignity() : SkillImpl(NPC_WIDEHELLDIGNITY) {
}

void SkillHellDignity::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillHellPower::SkillHellPower() : WeaponSkillImpl(NPC_HELLPOWER) {
}

void SkillHellPower::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
		sc_start(src, target, skill_get_sc(getSkillId()), skill_lv*20, skill_lv, skill_get_time2(getSkillId(), skill_lv)));
}

SkillHellsJudgement::SkillHellsJudgement() : SkillImplRecursiveDamageSplash(NPC_HELLJUDGEMENT) {
}

void SkillHellsJudgement::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_CURSE,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillHellsJudgement::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillHellsJudgement::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

SkillHellsJudgement2::SkillHellsJudgement2() : SkillImplRecursiveDamageSplash(NPC_HELLJUDGEMENT2) {
}

void SkillHellsJudgement2::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	switch(rnd()%6) {
	case 0:
		sc_start(src,target,SC_SLEEP,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
		break;
	case 1:
		sc_start(src,target,SC_CONFUSION,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
		break;
	case 2:
		sc_start(src,target,SC_HALLUCINATION,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
		break;
	case 3:
		sc_start(src,target,SC_STUN,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
		break;
	case 4:
		sc_start(src,target,SC_FEAR,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
		break;
	default:
		sc_start(src,target,SC_CURSE,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
		break;
	}
}

void SkillHellsJudgement2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillHellsJudgement2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

SkillHolyAttributeAttack::SkillHolyAttributeAttack() : WeaponSkillImpl(NPC_HOLYATTACK) {
}

void SkillHolyAttributeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillHolyAttributeAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillHolyAttributeChange::SkillHolyAttributeChange() : SkillImpl(NPC_CHANGEHOLY) {
}

void SkillHolyAttributeChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
		skill_get_time(getSkillId(), skill_lv)));
}

SkillIceBreath::SkillIceBreath() : SkillImpl(NPC_ICEBREATH) {
}

void SkillIceBreath::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_FREEZE,70,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillIceBreath::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillIceBreath::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
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

void SkillIceBreath::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate *= 2;
}

SkillIceBreath2::SkillIceBreath2() : SkillImpl(NPC_ICEBREATH2) {
}

void SkillIceBreath2::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_FREEZE,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillIceBreath2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillIceBreath2::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
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

void SkillIceBreath2::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate *= 2;
}

SkillIceMine::SkillIceMine() : SkillImpl(NPC_ICEMINE) {
}

void SkillIceMine::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillIncreasedGravity::SkillIncreasedGravity() : SkillImpl(NPC_GRADUAL_GRAVITY) {
}

void SkillIncreasedGravity::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_start(src, target, skill_get_sc(getSkillId()), 10000, skill_lv, 0, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NOAVOID|SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillInvincibleOff::SkillInvincibleOff() : SkillImpl(NPC_INVINCIBLEOFF) {
}

void SkillInvincibleOff::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	status_change_end(target, SC_INVINCIBLE);
}

SkillInvisible::SkillInvisible() : SkillImpl(NPC_INVISIBLE) {
}

void SkillInvisible::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Have val4 passed as 6 is for "infinite cloak" (do not end on attack/skill use).
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start4(src,target,skill_get_sc(getSkillId()),100,skill_lv,0,0,6,skill_get_time(getSkillId(),skill_lv)));
}

SkillJackFrost2::SkillJackFrost2() : SkillImplRecursiveDamageSplash(NPC_JACKFROST) {
}

void SkillJackFrost2::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_FREEZE,200,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

void SkillJackFrost2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *tsc = status_get_sc(target);

	if (tsc && tsc->getSCE(SC_FREEZING)) {
		skillratio += 900 + 300 * skill_lv;
		RE_LVL_DMOD(100);
	} else {
		skillratio += 400 + 100 * skill_lv;
		RE_LVL_DMOD(150);
	}
}

void SkillJackFrost2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	map_foreachinrange(skill_area_sub,target,skill_get_splash(getSkillId(),skill_lv),BL_CHAR|BL_SKILL,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
}

SkillLeash::SkillLeash() : SkillImpl(NPC_LEASH) {
}

void SkillLeash::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if( !skill_check_unit_movepos( 0, target, src->x, src->y, 1, 1 ) ){
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	clif_blown( target );
}

SkillLexAeterna2::SkillLexAeterna2() : SkillImpl(NPC_LEX_AETERNA) {
}

void SkillLexAeterna2::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub, src->m, x-i, y-i, x+i, y+i, BL_CHAR, src, PR_LEXAETERNA, 1, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
}

SkillLick::SkillLick() : SkillImpl(NPC_LICK) {
}

void SkillLick::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_zap(target, 0, 100);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start(src,target,skill_get_sc(getSkillId()),(skill_lv*20),skill_lv,skill_get_time2(getSkillId(),skill_lv)));
}

SkillMetamorphosis::SkillMetamorphosis() : SkillImpl(NPC_METAMORPHOSIS) {
}

void SkillMetamorphosis::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	if(md && md->skill_idx >= 0) {
		int32 class_ = mob_random_class (md->db->skill[md->skill_idx]->val,0);
		if (skill_lv > 1) //Multiply the rest of mobs. [Skotlex]
			mob_summonslave(md,md->db->skill[md->skill_idx]->val,skill_lv-1,getSkillId());
		if (class_) mob_class_change(md, class_);
	}
}

SkillMilleniumShield2::SkillMilleniumShield2() : SkillImpl(NPC_MILLENNIUMSHIELD) {
}

void SkillMilleniumShield2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)))
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillMonsterSummons::SkillMonsterSummons() : SkillImpl(NPC_SUMMONMONSTER) {
}

void SkillMonsterSummons::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	if(md && md->skill_idx >= 0)
		mob_summonslave(md,md->db->skill[md->skill_idx]->val,skill_lv,getSkillId());
}

SkillMultiStageAttack::SkillMultiStageAttack() : WeaponSkillImpl(NPC_COMBOATTACK) {
}

void SkillMultiStageAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 25 * skill_lv;
}

SkillNpcArrowStorm::SkillNpcArrowStorm() : SkillImplRecursiveDamageSplash(NPC_ARROWSTORM) {
}

void SkillNpcArrowStorm::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	if (skill_lv > 4)
		base_skillratio += 1900;
	else
		base_skillratio += 900;
}

SkillNpcCloudKill::SkillNpcCloudKill() : SkillImpl(NPC_CLOUD_KILL) {
}

void SkillNpcCloudKill::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 50 * skill_lv;
}

void SkillNpcCloudKill::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= 4;
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillNpcColuceoHeal::SkillNpcColuceoHeal() : SkillImpl(NPC_CHEAL) {
}

void SkillNpcColuceoHeal::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 ) {
		status_data* tstatus = status_get_status_data(*target);
		status_change *tsc = status_get_sc(target);

		if( tstatus && !battle_check_undead(tstatus->race, tstatus->def_ele) && tsc != nullptr && !tsc->hasSCE(SC_BERSERK) ) {
			int32 i = skill_calc_heal(src, target, AL_HEAL, 10, true);
			if (status_isimmune(target))
				i = 0;
			clif_skill_nodamage(src, *target, getSkillId(), i);
			if( tsc && tsc->getSCE(SC_AKAITSUKI) && i )
				i = ~i + 1;
			status_heal(target, i, 0, 0);
		}
	}
	else {
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_MOB,
			src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillNpcCursedCircle::SkillNpcCursedCircle() : SkillImpl(NPC_SR_CURSEDCIRCLE) {
}

void SkillNpcCursedCircle::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 ) {
		if( status_get_class_(target) == CLASS_BOSS )
			return;
		if( sc_start2(src,target, skill_get_sc(getSkillId()), 50, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv))) {
			if( target->type == BL_MOB )
				mob_unlocktarget((TBL_MOB*)target,gettick());
			clif_bladestop( *src, target->id, true );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
	} else {
		map_session_data* sd = BL_CAST(BL_PC, src);
		int32 count = 0;

		clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
		count = map_forcountinrange(skill_area_sub, src, skill_get_splash(getSkillId(),skill_lv), (sd)?sd->spiritball_old:15, // Assume 15 spiritballs in non-characters
			BL_CHAR, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
		if( sd ) pc_delspiritball(sd, count, 0);
		clif_skill_nodamage(src, *src, getSkillId(), skill_lv,
			sc_start2(src,src, SC_CURSEDCIRCLE_ATKER, 50, skill_lv, count, skill_get_time(getSkillId(),skill_lv)));
	}
}

SkillNpcDragonBreath::SkillNpcDragonBreath() : WeaponSkillImpl(NPC_DRAGONBREATH) {
}

void SkillNpcDragonBreath::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	if (skill_lv > 5)
		sc_start4(src,target,SC_FREEZING,50,skill_lv,1000,src->id,0,skill_get_time(getSkillId(),skill_lv));
	else
		sc_start4(src,target,SC_BURNING,50,skill_lv,1000,src->id,0,skill_get_time(getSkillId(),skill_lv));
}

void SkillNpcDragonBreath::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	if (skill_lv > 5)
		base_skillratio += 500 + 500 * (skill_lv - 5);	// Level 6-10 is using water element, like RK_DRAGONBREATH_WATER
	else
		base_skillratio += 500 + 500 * skill_lv;	// Level 1-5 is using fire element, like RK_DRAGONBREATH
}

void SkillNpcDragonBreath::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if( tsc && tsc->getSCE(SC_HIDING) )
		clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
	else {
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	}
}

void SkillNpcDragonBreath::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Cast center might be relevant later (e.g. for knockback direction)
	skill_area_temp[4] = x;
	skill_area_temp[5] = y;
	int32 i = skill_get_splash(getSkillId(),skill_lv);
	map_foreachinarea(skill_area_sub,src->m,x-i,y-i,x+i,y+i,BL_CHAR|BL_SKILL,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
}

SkillNpcElectricWalk::SkillNpcElectricWalk() : SkillImpl(NPC_ELECTRICWALK) {
}

void SkillNpcElectricWalk::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 100 * skill_lv;
}

void SkillNpcElectricWalk::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());

	if( sc && sc->getSCE(type) )
		status_change_end(src,type);
	sc_start2(src, src, type, 100, getSkillId(), skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillNpcFatalMenace::SkillNpcFatalMenace() : WeaponSkillImpl(NPC_FATALMENACE) {
}

void SkillNpcFatalMenace::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// todo should it teleport the target ?
	if( flag&1 )
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	else {
		int16 x, y;
		map_search_freecell(src, 0, &x, &y, -1, -1, 0);
		// Destination area
		skill_area_temp[4] = x;
		skill_area_temp[5] = y;
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), splash_target(src), src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_damage_id);
		skill_addtimerskill(src,tick + 800,src->id,x,y,getSkillId(),skill_lv,0,flag); // To teleport Self
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	}
}

SkillNpcFireWalk::SkillNpcFireWalk() : SkillImpl(NPC_FIREWALK) {
}

void SkillNpcFireWalk::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 100 * skill_lv;
}

void SkillNpcFireWalk::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());

	if( sc && sc->getSCE(type) )
		status_change_end(src,type);
	sc_start2(src, src, type, 100, getSkillId(), skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillNpcHowlingOfMandragora::SkillNpcHowlingOfMandragora() : SkillImpl(NPC_MANDRAGORA) {
}

void SkillNpcHowlingOfMandragora::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	if( flag&1 ) {
		int32 rate;
		rate = (20 * skill_lv) - (tstatus->vit + tstatus->luk) / 5;

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

SkillNpcIgnitionBreak::SkillNpcIgnitionBreak() : SkillImplRecursiveDamageSplash(NPC_IGNITIONBREAK) {
}

void SkillNpcIgnitionBreak::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	// 3x3 cell Damage   = 1000  1500  2000  2500  3000 %
	// 7x7 cell Damage   = 750   1250  1750  2250  2750 %
	// 11x11 cell Damage = 500   1000  1500  2000  2500 %
	int32 i = distance_bl(src,target);
	if (i < 2)
		base_skillratio += -100 + 500 * (skill_lv + 1);
	else if (i < 4)
		base_skillratio += -100 + 250 + 500 * skill_lv;
	else
		base_skillratio += -100 + 500 * skill_lv;
}

void SkillNpcIgnitionBreak::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = 0;
#if PACKETVER >= 20180207
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
#else
	clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
#endif
	map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|SD_SPLASH|1, skill_castend_damage_id);
}

SkillNpcMagmaEruption::SkillNpcMagmaEruption() : WeaponSkillImpl(NPC_MAGMA_ERUPTION) {
}

void SkillNpcMagmaEruption::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Stun effect from 'slam'
	sc_start(src, target, SC_STUN, 90, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillNpcMagmaEruption::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// 1st, AoE 'slam' damage
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinarea(skill_area_sub, src->m, x-i, y-i, x+i, y+i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|SD_ANIMATION|1, skill_castend_damage_id);
	// 2nd, AoE 'eruption' unit
	skill_addtimerskill(src,tick + status_get_amotion(src) * 2,0,x,y,getSkillId(),skill_lv,0,flag);
}

SkillNpcPhantomThrust::SkillNpcPhantomThrust() : WeaponSkillImpl(NPC_PHANTOMTHRUST) {
}

// SkillNpcPhantomThrust::calculateSkillRatio : ATK = 100% for all level

void SkillNpcPhantomThrust::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	unit_setdir(src,map_calc_dir(src, target->x, target->y));
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);

	skill_blown(src,target,distance_bl(src,target)-1,unit_getdir(src),BLOWN_NONE);
	if( battle_check_target(src,target,BCT_ENEMY) > 0 )
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillNpcPoisonBuster::SkillNpcPoisonBuster() : SkillImpl(NPC_POISON_BUSTER) {
}

void SkillNpcPoisonBuster::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 1500 * skill_lv;
}

void SkillNpcPoisonBuster::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( tsc && tsc->getSCE(SC_POISON) ) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
		status_change_end(target, SC_POISON);
	}
	else if( sd )
		clif_skill_fail( *sd, getSkillId() );
}

SkillNpcPsychicWave::SkillNpcPsychicWave() : SkillImpl(NPC_PSYCHIC_WAVE) {
}

void SkillNpcPsychicWave::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 500 * skill_lv;
}

void SkillNpcPsychicWave::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillNpcPsychicWave::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if( sc != nullptr && !sc->empty() ) {
		static const std::vector<sc_type> types = {
			SC_HEATER_OPTION,
			SC_COOLER_OPTION,
			SC_BLAST_OPTION,
			SC_CURSED_SOIL_OPTION,
			SC_FLAMETECHNIC_OPTION,
			SC_COLD_FORCE_OPTION,
			SC_GRACE_BREEZE_OPTION,
			SC_EARTH_CARE_OPTION,
			SC_DEEP_POISONING_OPTION
		};
		for( sc_type type : types ){
			if( sc->hasSCE( type ) ){
				element = sc->getSCE( type )->val3;
				break;
			}
		}
	}
}

SkillNpcRayOfGenesis::SkillNpcRayOfGenesis() : SkillImplRecursiveDamageSplash(NPC_RAYOFGENESIS) {
}

void SkillNpcRayOfGenesis::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	if (skill_lv < 8)
		sc_start(src,target, SC_BLIND, 50, skill_lv, skill_get_time(getSkillId(),skill_lv));
	else
		sc_start(src,target, SC_BLIND, 100, skill_lv, skill_get_time(getSkillId(),skill_lv));
}

void SkillNpcRayOfGenesis::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 200 * skill_lv;
}

void SkillNpcRayOfGenesis::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillNpcRun::SkillNpcRun() : SkillImpl(NPC_RUN) {
}

void SkillNpcRun::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	if (md) {
		block_list* tbl = map_id2bl(md->target_id);

		if (tbl) {
			md->state.can_escape = 1;
			mob_unlocktarget(md, tick);
			// Official distance is 7, if level > 1, distance = level
			t_tick time = unit_escape(src, tbl, skill_lv > 1 ? skill_lv : 7, 3);

			if (time) {
				// Need to set state here as it's not set otherwise
				mob_setstate(*md, MSS_WALK);
				// Set AI to inactive for the duration of this movement
				md->next_thinktime = tick + time;
			}
		}
	}
}

SkillNpcSuicide::SkillNpcSuicide() : SkillImpl(NPC_SUICIDE) {
}

void SkillNpcSuicide::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	status_kill(src); //When suiciding, neither exp nor drops is given.
}

SkillNpcVenomImpress::SkillNpcVenomImpress() : WeaponSkillImpl(NPC_VENOMIMPRESS) {
}

void SkillNpcVenomImpress::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_VENOMIMPRESS, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillPetrifyAttack::SkillPetrifyAttack() : WeaponSkillImpl(NPC_PETRIFYATTACK) {
}

void SkillPetrifyAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start2(src,target,SC_STONEWAIT,(20*skill_lv),skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv),skill_get_time(getSkillId(), skill_lv));
}

void SkillPetrifyAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillPiercingAttack::SkillPiercingAttack() : WeaponSkillImpl(NPC_PIERCINGATT) {
}

void SkillPiercingAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -25; //75% base damage
}

SkillPoisonAttack::SkillPoisonAttack() : WeaponSkillImpl(NPC_POISON) {
}

void SkillPoisonAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_POISON,(20*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillPoisonAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillPoisonAttributeAttack::SkillPoisonAttributeAttack() : WeaponSkillImpl(NPC_POISONATTACK) {
}

void SkillPoisonAttributeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillPoisonAttributeAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillPoisonAttributeChange::SkillPoisonAttributeChange() : SkillImpl(NPC_CHANGEPOISON) {
}

void SkillPoisonAttributeChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
		skill_get_time(getSkillId(), skill_lv)));
}

SkillPowerUp::SkillPowerUp() : SkillImpl(NPC_POWERUP) {
}

void SkillPowerUp::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target,skill_get_sc(getSkillId()),100,200,100,skill_get_time(getSkillId(), skill_lv)));
}

SkillPropertyImmune::SkillPropertyImmune() : SkillImpl(NPC_IMMUNE_PROPERTY) {
}

void SkillPropertyImmune::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	switch (skill_lv) {
		case 1: type = SC_IMMUNE_PROPERTY_NOTHING; break;
		case 2: type = SC_IMMUNE_PROPERTY_WATER; break;
		case 3: type = SC_IMMUNE_PROPERTY_GROUND; break;
		case 4: type = SC_IMMUNE_PROPERTY_FIRE; break;
		case 5: type = SC_IMMUNE_PROPERTY_WIND; break;
		case 6: type = SC_IMMUNE_PROPERTY_DARKNESS; break;
		case 7: type = SC_IMMUNE_PROPERTY_SAINT; break;
		case 8: type = SC_IMMUNE_PROPERTY_POISON; break;
		case 9: type = SC_IMMUNE_PROPERTY_TELEKINESIS; break;
		case 10: type = SC_IMMUNE_PROPERTY_UNDEAD; break;
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
}

SkillProvocation::SkillProvocation() : SkillImpl(NPC_PROVOCATION) {
}

void SkillProvocation::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if (md) mob_unlocktarget(md, tick);
}

SkillPulseStrike::SkillPulseStrike() : SkillImplRecursiveDamageSplash(NPC_PULSESTRIKE) {
}

void SkillPulseStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillPulseStrike::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

SkillPulseStrike2::SkillPulseStrike2() : SkillImplRecursiveDamageSplash(NPC_PULSESTRIKE2) {
}

void SkillPulseStrike2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100;
}

void SkillPulseStrike2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	for (int32 i = 0; i < 3; i++)
		skill_addtimerskill(src, tick + (t_tick)skill_get_time(getSkillId(), skill_lv) * i, target->id, 0, 0, getSkillId(), skill_lv, skill_get_type(getSkillId()), flag);
}

SkillRainOfMeteor::SkillRainOfMeteor() : SkillImpl(NPC_RAINOFMETEOR) {
}

void SkillRainOfMeteor::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 350;	// unknown ratio
}

void SkillRainOfMeteor::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 area = skill_get_splash(getSkillId(), skill_lv);
	int16 tmpx = 0;
	int16 tmpy = 0;

	for (int32 i = 1; i <= (skill_get_time(getSkillId(), skill_lv)/skill_get_unit_interval(getSkillId())); i++) {
		// Casts a double meteor in the first interval.
		if (i == 1) {
			// The first meteor is at the center
			skill_unitsetting(src, getSkillId(), skill_lv, x, y, flag+skill_get_unit_interval(getSkillId()));

			// The second meteor is near the first
			tmpx = x - 1 + rnd()%3;
			tmpy = y - 1 + rnd()%3;
			skill_unitsetting(src, getSkillId(), skill_lv, tmpx, tmpy, flag+skill_get_unit_interval(getSkillId()));
		}
		else {	// Casts 1 meteor per interval in the splash area
			tmpx = x - area + rnd()%(area * 2 + 1);
			tmpy = y - area + rnd()%(area * 2 + 1);
			skill_unitsetting(src, getSkillId(), skill_lv, tmpx, tmpy, flag+i*skill_get_unit_interval(getSkillId()));
		}
	}
}

SkillRandomAttack::SkillRandomAttack() : WeaponSkillImpl(NPC_RANDOMATTACK) {
}

void SkillRandomAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

SkillRandomMove::SkillRandomMove() : SkillImpl(NPC_RANDOMMOVE) {
}

void SkillRandomMove::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	if (md != nullptr) {
		// This skill creates fake casting state where a monster moves while showing a cast bar
		int32 tricktime = MOB_SKILL_INTERVAL * 3;
		md->trickcasting = tick + tricktime;
		clif_skillcasting(*src, src, 0, 0, getSkillId(), skill_lv, ELE_FIRE, tricktime + MOB_SKILL_INTERVAL / 2);
		// Monster cannot be stopped while moving
		md->state.can_escape = 1;
		// Move up to 8 cells
		unit_escape(md, target, 8, 3);
	}
}

SkillRebirth::SkillRebirth() : SkillImpl(NPC_REBIRTH) {
}

void SkillRebirth::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	if( md && md->state.rebirth )
		return; // only works once
	sc_start(src,target,skill_get_sc(getSkillId()),100,skill_lv,INFINITE_TICK);
}

SkillRecallSlaves::SkillRecallSlaves() : SkillImpl(NPC_CALLSLAVE) {
}

void SkillRecallSlaves::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_warpslave(src,MOB_SLAVEDISTANCE);
}

SkillRevenge::SkillRevenge() : SkillImpl(NPC_REVENGE) {
}

void SkillRevenge::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);
	status_data* sstatus = status_get_status_data(*src);

	// not really needed... but adding here anyway ^^
	if (md && md->master_id > 0) {
		block_list *mbl, *tbl;
		if ((mbl = map_id2bl(md->master_id)) == nullptr ||
			(tbl = battle_gettargeted(mbl)) == nullptr)
			return;
		md->state.provoke_flag = tbl->id;
		mob_target(md, tbl, sstatus->rhw.range);
	}
}

// NPC_REVERBERATION
SkillReverberation2::SkillReverberation2() : SkillImpl(NPC_REVERBERATION) {
}

void SkillReverberation2::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}


// NPC_REVERBERATION_ATK
SkillReverberationAttack::SkillReverberationAttack() : SkillImplRecursiveDamageSplash(NPC_REVERBERATION_ATK) {
}

void SkillReverberationAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 400 + 200 * skill_lv;
}

int32 SkillReverberationAttack::getSplashTarget(block_list* src) const {
	return splash_target(src);
}

void SkillReverberationAttack::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	skill_area_temp[1] = 0;

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillShadowAttributeAttack::SkillShadowAttributeAttack() : WeaponSkillImpl(NPC_DARKNESSATTACK) {
}

void SkillShadowAttributeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillShadowAttributeAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillShadowAttributeChange::SkillShadowAttributeChange() : SkillImpl(NPC_CHANGEDARKNESS) {
}

void SkillShadowAttributeChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
		skill_get_time(getSkillId(), skill_lv)));
}

SkillSiegeMode::SkillSiegeMode() : SkillImpl(NPC_SIEGEMODE) {
}

void SkillSiegeMode::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Not implemented/used: Gives EFST_SIEGEMODE which reduces speed to 1000.
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillSilenceAttack::SkillSilenceAttack() : WeaponSkillImpl(NPC_SILENCEATTACK) {
}

void SkillSilenceAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_SILENCE,(20*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillSilenceAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillSleepAttack::SkillSleepAttack() : WeaponSkillImpl(NPC_SLEEPATTACK) {
}

void SkillSleepAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_SLEEP,(20*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillSleepAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillSlowCast::SkillSlowCast() : SkillImpl(NPC_SLOWCAST) {
}

void SkillSlowCast::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillSmoking::SkillSmoking() : SkillImpl(NPC_SMOKING) {
}

void SkillSmoking::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillSoulStrikeOfDarkness::SkillSoulStrikeOfDarkness() : SkillImpl(NPC_DARKSTRIKE) {
}

void SkillSoulStrikeOfDarkness::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillSpeedUp::SkillSpeedUp() : SkillImpl(NPC_SPEEDUP) {
}

void SkillSpeedUp::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	if (md != nullptr) {
		// Officially, trickcasting continues as long as there are more than 700ms left
		int32 trickstop = (MOB_SKILL_INTERVAL * 7) / 10;
		if (DIFF_TICK(md->trickcasting, tick) >= trickstop) {
			// This skill directly modifies a monster's base speed value
			md->base_status->speed = std::max(md->base_status->speed - 250, MIN_WALK_SPEED);
			// Need to recalc speed based on new base value
			status_calc_bl(md, { SCB_SPEED });
			// We use skills only on each full cell, to fix the inaccuracy we do this on last move interval
			if (DIFF_TICK(md->trickcasting, tick) < trickstop + MOB_SKILL_INTERVAL)
				md->last_skillcheck = tick + 100;
		}
		else {
			// Synchronize skill usage
			md->last_skillcheck = md->trickcasting;
			// Causes monster to stop and get ready for next alchemist skill
			md->trickcasting = 0;
			md->state.can_escape = 0;
		}
	}
}

SkillSpiritDestruction::SkillSpiritDestruction() : WeaponSkillImpl(NPC_MENTALBREAKER) {
}

void SkillSpiritDestruction::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	//SP Damage 12%/16%/25%/50%/100% of MaxSP
	int32 rate;
	switch (skill_lv) {
		case 1:
			rate = 12;
			break;
		case 2:
			rate = 16;
			break;
		case 3:
			rate = 25;
			break;
		case 4:
			rate = 50;
			break;
		case 5:
			rate = 100;
			break;
		default:
			// For easy customization
			rate = skill_lv;
			break;
	}
	status_percent_damage(src, target, 0, -rate, false);
}

void SkillSpiritDestruction::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillSplashAttack::SkillSplashAttack() : SkillImplRecursiveDamageSplash(NPC_SPLASHATTACK) {
}

void SkillSplashAttack::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= SD_PREAMBLE; // a fake packet will be sent for the first target to be hit

	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillStoneSkin::SkillStoneSkin() : SkillImpl(NPC_STONESKIN) {
}

void SkillStoneSkin::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,getSkillId(),skill_get_time(getSkillId(),skill_lv)));
}

SkillStop::SkillStop() : SkillImpl(NPC_STOP) {
}

void SkillStop::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	if( clif_skill_nodamage(src,*target,getSkillId(),skill_lv, sc_start2(src,target,type,100,skill_lv,src->id,skill_get_time(getSkillId(),skill_lv)) ) )
		sc_start2(src,src,type,100,skill_lv,target->id,skill_get_time(getSkillId(),skill_lv));
}

SkillStormGust2::SkillStormGust2() : SkillImpl(NPC_STORMGUST2) {
}

void SkillStormGust2::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	if (skill_lv == 1)
		sc_start(src,target,SC_FREEZE,10,skill_lv,skill_get_time2(getSkillId(),skill_lv));
	else if (skill_lv == 2)
		sc_start(src,target,SC_FREEZE,7,skill_lv,skill_get_time2(getSkillId(),skill_lv));
	else
		sc_start(src,target,SC_FREEZE,3,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillStormGust2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 200 * skill_lv;
}

void SkillStormGust2::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillStunAttack::SkillStunAttack() : WeaponSkillImpl(NPC_STUNATTACK) {
}

void SkillStunAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_STUN,(20*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillStunAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillSuckingBlood::SkillSuckingBlood() : SkillImpl(NPC_BLOODDRAIN) {
}

void SkillSuckingBlood::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillSuckingBlood::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 heal = (int32)skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
	if (heal > 0){
		clif_skill_nodamage(nullptr, *src, AL_HEAL, heal);
		status_heal(src, heal, 0, 0);
	}
}

SkillSuicideBombing::SkillSuicideBombing() : SkillImpl(NPC_SELFDESTRUCTION) {
}

void SkillSuicideBombing::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if( tsc && tsc->getSCE(SC_HIDING) )
		return;
	if (src != target)
		skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillSuicideBombing::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);
	status_data* sstatus = status_get_status_data(*src);
	int32 i = 0;

	//Self Destruction hits everyone in range (allies+enemies)
	//Except for Summoned Marine spheres on non-versus maps, where it's just enemies and your own slaves.
	if ((md == nullptr || md->special_state.ai == AI_SPHERE) && !map_flag_vs(src->m)) {
		// Enable Marine Spheres to damage own Homunculus and summons outside PVP
		if (battle_config.alchemist_summon_setting&8)
			i = BCT_ENEMY|BCT_SLAVE;
		else
			i = BCT_ENEMY;
	} else {
		i = BCT_ALL;
	}
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
	map_delblock(src); //Required to prevent chain-self-destructions hitting back.
	map_foreachinshootrange(skill_area_sub, target,
		skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL,
		src, getSkillId(), skill_lv, tick, flag|i,
		skill_castend_damage_id);
	if(map_addblock(src)) {
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	// Won't display the damage, but drop items and give exp
	status_zap(src, sstatus->hp, 0, 0);
}

SkillTalk::SkillTalk() : SkillImpl(NPC_TALK) {
}

void SkillTalk::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillThunderBreath::SkillThunderBreath() : SkillImpl(NPC_THUNDERBREATH) {
}

void SkillThunderBreath::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillThunderBreath::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
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

void SkillThunderBreath::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate *= 2;
}

SkillTransformation::SkillTransformation() : SkillImpl(NPC_TRANSFORMATION) {
}

void SkillTransformation::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	if(md && md->skill_idx >= 0) {
		int32 class_ = mob_random_class (md->db->skill[md->skill_idx]->val,0);
		if (skill_lv > 1) //Multiply the rest of mobs. [Skotlex]
			mob_summonslave(md,md->db->skill[md->skill_idx]->val,skill_lv-1,getSkillId());
		if (class_) mob_class_change(md, class_);
	}
}

SkillUndeadAttributeChange::SkillUndeadAttributeChange() : WeaponSkillImpl(NPC_CHANGEUNDEAD) {
}

void SkillUndeadAttributeChange::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_CHANGEUNDEAD, (10 * skill_lv), skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillUndeadAttributeChange::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillUndeadElementAttack::SkillUndeadElementAttack() : WeaponSkillImpl(NPC_UNDEADATTACK) {
}

void SkillUndeadElementAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillUndeadElementAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillVampireGift::SkillVampireGift() : SkillImplRecursiveDamageSplash(NPC_VAMPIRE_GIFT) {
}

void SkillVampireGift::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += ((skill_lv - 1) % 5 + 1) * 100;
}

void SkillVampireGift::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

int64 SkillVampireGift::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	int32 heal = static_cast<int32>(SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag));

	if (heal > 0) {
		clif_skill_nodamage(nullptr, *src, AL_HEAL, heal);
		status_heal(src, heal, 0, 0);
	}

	return heal;
}

SkillVenomFog::SkillVenomFog() : SkillImpl(NPC_VENOMFOG) {
}

void SkillVenomFog::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 600 + 100 * skill_lv;
}

void SkillVenomFog::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillWaterAttributeAttack::SkillWaterAttributeAttack() : WeaponSkillImpl(NPC_WATERATTACK) {
}

void SkillWaterAttributeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillWaterAttributeAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillWaterAttributeChange::SkillWaterAttributeChange() : SkillImpl(NPC_CHANGEWATER) {
}

void SkillWaterAttributeChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
		skill_get_time(getSkillId(), skill_lv)));
}

SkillWideBleeding::SkillWideBleeding() : SkillImpl(NPC_WIDEBLEEDING) {
}

void SkillWideBleeding::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideBleeding2::SkillWideBleeding2() : SkillImpl(NPC_WIDEBLEEDING2) {
}

void SkillWideBleeding2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideConfusion::SkillWideConfusion() : SkillImpl(NPC_WIDECONFUSE) {
}

void SkillWideConfusion::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideConfusion2::SkillWideConfusion2() : SkillImpl(NPC_WIDECONFUSE2) {
}

void SkillWideConfusion2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideCriticalWounds::SkillWideCriticalWounds() : SkillImplRecursiveDamageSplash(NPC_WIDECRITICALWOUND) {
}

void SkillWideCriticalWounds::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_CRITICALWOUND,100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillWideCriticalWounds::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

SkillWideCurse::SkillWideCurse() : SkillImpl(NPC_WIDECURSE) {
}

void SkillWideCurse::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideCurse2::SkillWideCurse2() : SkillImpl(NPC_WIDECURSE2) {
}

void SkillWideCurse2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideFreeze::SkillWideFreeze() : SkillImpl(NPC_WIDEFREEZE) {
}

void SkillWideFreeze::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideFreeze2::SkillWideFreeze2() : SkillImpl(NPC_WIDEFREEZE2) {
}

void SkillWideFreeze2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideLeash::SkillWideLeash() : SkillImpl(NPC_WIDELEASH) {
}

void SkillWideLeash::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag & 1 ){
		if( !skill_check_unit_movepos( 0, target, src->x, src->y, 1, 1 ) ){
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		clif_blown( target );
	}else{
		skill_area_temp[2] = 0; // For SD_PREAMBLE
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinallrange( skill_area_sub, target, skill_get_splash( getSkillId(), skill_lv ), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_PREAMBLE | 1, skill_castend_nodamage_id );
	}
}

SkillWidePetrify::SkillWidePetrify() : SkillImpl(NPC_WIDESTONE) {
}

void SkillWidePetrify::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv),skill_get_time(getSkillId(), skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWidePetrify2::SkillWidePetrify2() : SkillImpl(NPC_WIDESTONE2) {
}

void SkillWidePetrify2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv),skill_get_time(getSkillId(), skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideSight::SkillWideSight() : SkillImpl(NPC_WIDESIGHT) {
}

void SkillWideSight::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,getSkillId(),skill_get_time(getSkillId(),skill_lv)));
}

SkillWideSilence::SkillWideSilence() : SkillImpl(NPC_WIDESILENCE) {
}

void SkillWideSilence::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideSilence2::SkillWideSilence2() : SkillImpl(NPC_WIDESILENCE2) {
}

void SkillWideSilence2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideSleep::SkillWideSleep() : SkillImpl(NPC_WIDESLEEP) {
}

void SkillWideSleep::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideSleep2::SkillWideSleep2() : SkillImpl(NPC_WIDESLEEP2) {
}

void SkillWideSleep2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideSoulDrain::SkillWideSoulDrain() : SkillImpl(NPC_WIDESOULDRAIN) {
}

void SkillWideSoulDrain::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1)
		status_percent_damage(src,target,0,((skill_lv-1)%5+1)*20,false);
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideStun::SkillWideStun() : SkillImpl(NPC_WIDESTUN) {
}

void SkillWideStun::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideStun2::SkillWideStun2() : SkillImpl(NPC_WIDESTUN2) {
}

void SkillWideStun2::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWideSuck::SkillWideSuck() : SkillImpl(NPC_WIDESUCK) {
}

void SkillWideSuck::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;	// Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillWideWeb::SkillWideWeb() : SkillImpl(NPC_WIDEWEB) {
}

void SkillWideWeb::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1){
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,src->id,skill_get_time2(getSkillId(),skill_lv));
	}
	else {
		skill_area_temp[2] = 0; //For SD_PREAMBLE
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_PREAMBLE|1,
			skill_castend_nodamage_id);
	}
}

SkillWindAttributeAttack::SkillWindAttributeAttack() : WeaponSkillImpl(NPC_WINDATTACK) {
}

void SkillWindAttributeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillWindAttributeAttack::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 20 / 100;
}

SkillWindAttributeChange::SkillWindAttributeChange() : SkillImpl(NPC_CHANGEWIND) {
}

void SkillWindAttributeChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
		skill_get_time(getSkillId(), skill_lv)));
}

std::unique_ptr<const SkillImpl> SkillFactoryNpc::create(const e_skill skill_id) const {
	switch( skill_id ){
		case NPC_ACIDBREATH:
			return std::make_unique<SkillAcidBreath>();
		case NPC_AGIUP:
			return std::make_unique<SkillAgilityUp>();
		case NPC_ALLHEAL:
			return std::make_unique<SkillFullHeal>();
		case NPC_ALL_STAT_DOWN:
			return std::make_unique<SkillDecreaseAllStats>();
		case NPC_ANTIMAGIC:
			return std::make_unique<SkillAntiMagic>();
		case NPC_ARMORBRAKE:
			return std::make_unique<SkillBreakArmor>();
		case NPC_ARROWSTORM:
			return std::make_unique<SkillNpcArrowStorm>();
		case NPC_ATTRICHANGE:
			return std::make_unique<SkillAttributeChange>();
		case NPC_BARRIER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_BLEEDING:
			return std::make_unique<SkillBleeding>();
		case NPC_BLEEDING2:
			return std::make_unique<SkillBleeding2>();
		case NPC_BLINDATTACK:
			return std::make_unique<SkillBlindAttack>();
		case NPC_BLOODDRAIN:
			return std::make_unique<SkillSuckingBlood>();
		case NPC_CALLSLAVE:
			return std::make_unique<SkillRecallSlaves>();
		case NPC_CANE_OF_EVIL_EYE:
			return std::make_unique<SkillCaneOfEvilEye>();
		case NPC_CHANGEDARKNESS:
			return std::make_unique<SkillShadowAttributeChange>();
		case NPC_CHANGEFIRE:
			return std::make_unique<SkillFireAttributeChange>();
		case NPC_CHANGEGROUND:
			return std::make_unique<SkillEarthAttributeChange>();
		case NPC_CHANGEHOLY:
			return std::make_unique<SkillHolyAttributeChange>();
		case NPC_CHANGEPOISON:
			return std::make_unique<SkillPoisonAttributeChange>();
		case NPC_CHANGETELEKINESIS:
			return std::make_unique<SkillGhostAttributeChange>();
		case NPC_CHANGEUNDEAD:
			return std::make_unique<SkillUndeadAttributeChange>();
		case NPC_CHANGEWATER:
			return std::make_unique<SkillWaterAttributeChange>();
		case NPC_CHANGEWIND:
			return std::make_unique<SkillWindAttributeChange>();
		case NPC_CHEAL:
			return std::make_unique<SkillNpcColuceoHeal>();
		case NPC_CLOUD_KILL:
			return std::make_unique<SkillNpcCloudKill>();
		case NPC_COMBOATTACK:
			return std::make_unique<SkillMultiStageAttack>();
		case NPC_COMET:
			return std::make_unique<SkillComet2>();
		case NPC_CRITICALSLASH:
			return std::make_unique<WeaponSkillImpl>(skill_id);
		case NPC_CRITICALWOUND:
			return std::make_unique<SkillCriticalWounds>();
		case NPC_CURSEATTACK:
			return std::make_unique<SkillCurseAttack>();
		case NPC_DAMAGE_HEAL:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_DANCINGBLADE:
			return std::make_unique<SkillDancingBlade>();
		case NPC_DARKBLESSING:
			return std::make_unique<SkillDarkBlessing>();
		case NPC_DARKBREATH:
			return std::make_unique<SkillDarkBreath>();
		case NPC_DARKCROSS:
			return std::make_unique<SkillCrossOfDarkness>();
		case NPC_DARKNESSATTACK:
			return std::make_unique<SkillShadowAttributeAttack>();
		case NPC_DARKNESSBREATH:
			return std::make_unique<SkillDarknessBreath>();
		case NPC_DARKPIERCING:
			return std::make_unique<SkillDarkPiercing>();
		case NPC_DARKSTRIKE:
			return std::make_unique<SkillSoulStrikeOfDarkness>();
		case NPC_DARKTHUNDER:
			return std::make_unique<SkillDarknessJupitel>();
		case NPC_DEADLYCURSE:
			return std::make_unique<SkillDeadlyCurse>();
		case NPC_DEADLYCURSE2:
			return std::make_unique<SkillDeadlyCurse2>();
		case NPC_DEATHSUMMON:
			return std::make_unique<SkillDeathSummon>();
		case NPC_DEFENDER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_DRAGONBREATH:
			return std::make_unique<SkillNpcDragonBreath>();
		case NPC_DRAGONFEAR:
			return std::make_unique<SkillDragonFear>();
		case NPC_EARTHQUAKE:
			return std::make_unique<SkillEarthquake>();
		case NPC_ELECTRICWALK:
			return std::make_unique<SkillNpcElectricWalk>();
		case NPC_EMOTION:
			return std::make_unique<SkillEmotion>();
		case NPC_EMOTION_ON:
			return std::make_unique<SkillEmotionOn>();
		case NPC_ENERGYDRAIN:
			return std::make_unique<SkillEnergyDrain>();
		case NPC_EVILLAND:
			return std::make_unique<SkillEvilLand>();
		case NPC_EXPULSION:
			return std::make_unique<SkillExpulsion>();
		case NPC_FATALMENACE:
			return std::make_unique<SkillNpcFatalMenace>();
		case NPC_FIREATTACK:
			return std::make_unique<SkillFireAttributeAttack>();
		case NPC_FIREBREATH:
			return std::make_unique<SkillFireBreath>();
		case NPC_FIRESTORM:
			return std::make_unique<SkillFireStorm>();
		case NPC_FIREWALK:
			return std::make_unique<SkillNpcFireWalk>();
		case NPC_FLAMECROSS:
			return std::make_unique<SkillFlameCross>();
		case NPC_GRADUAL_GRAVITY:
			return std::make_unique<SkillIncreasedGravity>();
		case NPC_GRANDDARKNESS:
			return std::make_unique<SkillGrandCrossOfDarkness>();
		case NPC_GROUNDATTACK:
			return std::make_unique<SkillEarthAttributeAttack>();
		case NPC_GROUNDDRIVE:
			return std::make_unique<SkillGroundDrive>();
		case NPC_GUIDEDATTACK:
			return std::make_unique<WeaponSkillImpl>(skill_id);
		case NPC_HALLUCINATION:
			return std::make_unique<SkillHallucination>();
		case NPC_HALLUCINATIONWALK:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_HELLBURNING:
			return std::make_unique<SkillHellBurning>();
		case NPC_HELLJUDGEMENT:
			return std::make_unique<SkillHellsJudgement>();
		case NPC_HELLJUDGEMENT2:
			return std::make_unique<SkillHellsJudgement2>();
		case NPC_HELLPOWER:
			return std::make_unique<SkillHellPower>();
		case NPC_HELMBRAKE:
			return std::make_unique<SkillBreakHelm>();
		case NPC_HOLYATTACK:
			return std::make_unique<SkillHolyAttributeAttack>();
		case NPC_ICEBREATH:
			return std::make_unique<SkillIceBreath>();
		case NPC_ICEBREATH2:
			return std::make_unique<SkillIceBreath2>();
		case NPC_ICEMINE:
			return std::make_unique<SkillIceMine>();
		case NPC_IGNITIONBREAK:
			return std::make_unique<SkillNpcIgnitionBreak>();
		case NPC_IMMUNE_PROPERTY:
			return std::make_unique<SkillPropertyImmune>();
		case NPC_INVINCIBLE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_INVINCIBLEOFF:
			return std::make_unique<SkillInvincibleOff>();
		case NPC_INVISIBLE:
			return std::make_unique<SkillInvisible>();
		case NPC_JACKFROST:
			return std::make_unique<SkillJackFrost2>();
		case NPC_KEEPING:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_KILLING_AURA:
			return std::make_unique<SkillImplRecursiveDamageSplash>(skill_id);
		case NPC_LEASH:
			return std::make_unique<SkillLeash>();
		case NPC_LEX_AETERNA:
			return std::make_unique<SkillLexAeterna2>();
		case NPC_LICK:
			return std::make_unique<SkillLick>();
		case NPC_MAGICALATTACK:
			return std::make_unique<SkillDemonShockAttack>();
		case NPC_MAGICMIRROR:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_MAGMA_ERUPTION:
			return std::make_unique<SkillNpcMagmaEruption>();
		case NPC_MANDRAGORA:
			return std::make_unique<SkillNpcHowlingOfMandragora>();
		case NPC_MAXPAIN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_MAXPAIN_ATK:
			return std::make_unique<SkillImplRecursiveDamageSplash>(skill_id);
		case NPC_MENTALBREAKER:
			return std::make_unique<SkillSpiritDestruction>();
		case NPC_METAMORPHOSIS:
			return std::make_unique<SkillMetamorphosis>();
		case NPC_MILLENNIUMSHIELD:
			return std::make_unique<SkillMilleniumShield2>();
		case NPC_MOVE_COORDINATE:
			return std::make_unique<SkillChangeLocation>();
		case NPC_PETRIFYATTACK:
			return std::make_unique<SkillPetrifyAttack>();
		case NPC_PHANTOMTHRUST:
			return std::make_unique<SkillNpcPhantomThrust>();
		case NPC_PIERCINGATT:
			return std::make_unique<SkillPiercingAttack>();
		case NPC_POISON:
			return std::make_unique<SkillPoisonAttack>();
		case NPC_POISONATTACK:
			return std::make_unique<SkillPoisonAttributeAttack>();
		case NPC_POISON_BUSTER:
			return std::make_unique<SkillNpcPoisonBuster>();
		case NPC_POWERUP:
			return std::make_unique<SkillPowerUp>();
		case NPC_PROVOCATION:
			return std::make_unique<SkillProvocation>();
		case NPC_PSYCHIC_WAVE:
			return std::make_unique<SkillNpcPsychicWave>();
		case NPC_PULSESTRIKE:
			return std::make_unique<SkillPulseStrike>();
		case NPC_PULSESTRIKE2:
			return std::make_unique<SkillPulseStrike2>();
		case NPC_RAINOFMETEOR:
			return std::make_unique<SkillRainOfMeteor>();
		case NPC_RANDOMATTACK:
			return std::make_unique<SkillRandomAttack>();
		case NPC_RANDOMMOVE:
			return std::make_unique<SkillRandomMove>();
		case NPC_RANGEATTACK:
			return std::make_unique<WeaponSkillImpl>(skill_id);
		case NPC_RAYOFGENESIS:
			return std::make_unique<SkillNpcRayOfGenesis>();
		case NPC_REBIRTH:
			return std::make_unique<SkillRebirth>();
		case NPC_RELIEVE_OFF:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_RELIEVE_ON:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_REVENGE:
			return std::make_unique<SkillRevenge>();
		case NPC_REVERBERATION:
			return std::make_unique<SkillReverberation2>();
		case NPC_REVERBERATION_ATK:
			return std::make_unique<SkillReverberationAttack>();
		case NPC_RUN:
			return std::make_unique<SkillNpcRun>();
		case NPC_SELFDESTRUCTION:
			return std::make_unique<SkillSuicideBombing>();
		case NPC_SHIELDBRAKE:
			return std::make_unique<SkillBreakShield>();
		case NPC_SIEGEMODE:
			return std::make_unique<SkillSiegeMode>();
		case NPC_SILENCEATTACK:
			return std::make_unique<SkillSilenceAttack>();
		case NPC_SLEEPATTACK:
			return std::make_unique<SkillSleepAttack>();
		case NPC_SLOWCAST:
			return std::make_unique<SkillSlowCast>();
		case NPC_SMOKING:
			return std::make_unique<SkillSmoking>();
		case NPC_SPEEDUP:
			return std::make_unique<SkillSpeedUp>();
		case NPC_SPLASHATTACK:
			return std::make_unique<SkillSplashAttack>();
		case NPC_SR_CURSEDCIRCLE:
			return std::make_unique<SkillNpcCursedCircle>();
		case NPC_STONESKIN:
			return std::make_unique<SkillStoneSkin>();
		case NPC_STOP:
			return std::make_unique<SkillStop>();
		case NPC_STORMGUST2:
			return std::make_unique<SkillStormGust2>();
		case NPC_STUNATTACK:
			return std::make_unique<SkillStunAttack>();
		case NPC_SUICIDE:
			return std::make_unique<SkillNpcSuicide>();
		case NPC_SUMMONMONSTER:
			return std::make_unique<SkillMonsterSummons>();
		case NPC_SUMMONSLAVE:
			return std::make_unique<SkillFollowerSummons>();
		case NPC_TALK:
			return std::make_unique<SkillTalk>();
		case NPC_TELEKINESISATTACK:
			return std::make_unique<SkillGhostAttributeAttack>();
		case NPC_THUNDERBREATH:
			return std::make_unique<SkillThunderBreath>();
		case NPC_TRANSFORMATION:
			return std::make_unique<SkillTransformation>();
		case NPC_UNDEADATTACK:
			return std::make_unique<SkillUndeadElementAttack>();
		case NPC_VAMPIRE_GIFT:
			return std::make_unique<SkillVampireGift>();
		case NPC_VENOMFOG:
			return std::make_unique<SkillVenomFog>();
		case NPC_VENOMIMPRESS:
			return std::make_unique<SkillNpcVenomImpress>();
		case NPC_WATERATTACK:
			return std::make_unique<SkillWaterAttributeAttack>();
		case NPC_WEAPONBRAKER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NPC_WIDEBLEEDING:
			return std::make_unique<SkillWideBleeding>();
		case NPC_WIDEBLEEDING2:
			return std::make_unique<SkillWideBleeding2>();
		case NPC_WIDECONFUSE:
			return std::make_unique<SkillWideConfusion>();
		case NPC_WIDECONFUSE2:
			return std::make_unique<SkillWideConfusion2>();
		case NPC_WIDECRITICALWOUND:
			return std::make_unique<SkillWideCriticalWounds>();
		case NPC_WIDECURSE:
			return std::make_unique<SkillWideCurse>();
		case NPC_WIDECURSE2:
			return std::make_unique<SkillWideCurse2>();
		case NPC_WIDEFREEZE:
			return std::make_unique<SkillWideFreeze>();
		case NPC_WIDEFREEZE2:
			return std::make_unique<SkillWideFreeze2>();
		case NPC_WIDEHELLDIGNITY:
			return std::make_unique<SkillHellDignity>();
		case NPC_WIDELEASH:
			return std::make_unique<SkillWideLeash>();
		case NPC_WIDESIGHT:
			return std::make_unique<SkillWideSight>();
		case NPC_WIDESILENCE:
			return std::make_unique<SkillWideSilence>();
		case NPC_WIDESILENCE2:
			return std::make_unique<SkillWideSilence2>();
		case NPC_WIDESLEEP:
			return std::make_unique<SkillWideSleep>();
		case NPC_WIDESLEEP2:
			return std::make_unique<SkillWideSleep2>();
		case NPC_WIDESOULDRAIN:
			return std::make_unique<SkillWideSoulDrain>();
		case NPC_WIDESTONE:
			return std::make_unique<SkillWidePetrify>();
		case NPC_WIDESTONE2:
			return std::make_unique<SkillWidePetrify2>();
		case NPC_WIDESTUN:
			return std::make_unique<SkillWideStun>();
		case NPC_WIDESTUN2:
			return std::make_unique<SkillWideStun2>();
		case NPC_WIDESUCK:
			return std::make_unique<SkillWideSuck>();
		case NPC_WIDEWEB:
			return std::make_unique<SkillWideWeb>();
		case NPC_WINDATTACK:
			return std::make_unique<SkillWindAttributeAttack>();

		default:
			return nullptr;
	}
}

#endif
