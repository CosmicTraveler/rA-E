// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_summoner.hpp"

#include "map/clif.hpp"
#include "map/status.hpp"
#include "map/party.hpp"
#include "map/pc.hpp"
#include <config/core.hpp>
#include "map/map.hpp"
#include "map/unit.hpp"
#include "map/battle.hpp"
#include "map/mob.hpp"
#include "skill_impl.hpp"

SkillBite::SkillBite() : WeaponSkillImpl(SU_BITE) {
}

void SkillBite::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100;
}

SkillBlessingofMysticalCreatures::SkillBlessingofMysticalCreatures() : SkillImpl(SH_BLESSING_OF_MYSTICAL_CREATURES) {
}

void SkillBlessingofMysticalCreatures::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_heal(target, 0, 0, 200-status_get_ap(target), 0);
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
}

SkillBunchofShrimp::SkillBunchofShrimp() : SkillImpl(SU_BUNCHOFSHRIMP) {
}

void SkillBunchofShrimp::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || flag&1) {
		int32 duration = skill_get_time(getSkillId(), skill_lv);

		if (pc_checkskill(sd, SU_SPIRITOFSEA))
			duration += skill_get_time2(SU_BUNCHOFSHRIMP, skill_lv);
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, duration));
	} else if (sd) {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

// SU_CN_METEOR
SkillCatnipMeteor::SkillCatnipMeteor() : SkillImpl(SU_CN_METEOR) {
}

void SkillCatnipMeteor::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 200 + 100 * skill_lv;
	if (status_get_lv(src) > 99) {
		skillratio += sstatus->int_ * 5;
	}
	RE_LVL_DMOD(100);
}

void SkillCatnipMeteor::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	e_skill skill_id = getSkillId();

	if (sd) {
		// FIX ME: missing check of required item
		if (pc_search_inventory(sd, skill_db.find(SU_CN_METEOR)->require.itemid[0]) >= 0)
			skill_id = SU_CN_METEOR2;
		if (pc_checkskill(sd, SU_SPIRITOFLAND))
			sc_start(src, src, SC_DORAM_SVSP, 100, 100, skill_get_time(SU_SPIRITOFLAND, 1));
	}

	int32 area = skill_get_splash(skill_id, skill_lv);
	int16 tmpx = 0, tmpy = 0;

	for (int32 i = 1; i <= skill_get_time(skill_id, skill_lv) / skill_get_unit_interval(skill_id); i++) {
		// Creates a random Cell in the Splash Area
		tmpx = x - area + rnd() % (area * 2 + 1);
		tmpy = y - area + rnd() % (area * 2 + 1);
		skill_unitsetting(src, skill_id, skill_lv, tmpx, tmpy, flag + i * skill_get_unit_interval(skill_id));
	}
}

void SkillCatnipMeteor::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if( sc != nullptr && !sc->empty() ){
		if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_1 ) ){
			element = ELE_WATER;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_2 ) ){
			element = ELE_WIND;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_3 ) ){
			element = ELE_EARTH;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_4 ) ){
			element = ELE_FIRE;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_5 ) ){
			element = ELE_DARK;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_6 ) ){
			element = ELE_HOLY;
		}
	}
}


// SU_CN_METEOR2
SkillCatnipMeteor2::SkillCatnipMeteor2() : SkillImpl(SU_CN_METEOR2) {
}

void SkillCatnipMeteor2::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_CURSE, 20, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillCatnipMeteor2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 200 + 100 * skill_lv;
	if (status_get_lv(src) > 99) {
		skillratio += sstatus->int_ * 5;
	}
	RE_LVL_DMOD(100);
}

void SkillCatnipMeteor2::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if( sc != nullptr && !sc->empty() ){
		if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_1 ) ){
			element = ELE_WATER;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_2 ) ){
			element = ELE_WIND;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_3 ) ){
			element = ELE_EARTH;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_4 ) ){
			element = ELE_FIRE;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_5 ) ){
			element = ELE_DARK;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_6 ) ){
			element = ELE_HOLY;
		}
	}
}

SkillCatnipPowdering::SkillCatnipPowdering() : SkillImpl(SU_CN_POWDERING) {
}

void SkillCatnipPowdering::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && pc_checkskill(sd, SU_SPIRITOFLAND)) {
		sc_start(src, src, SC_DORAM_FLEE2, 100, sd->status.base_level * 10 / 12, skill_get_time(SU_SPIRITOFLAND, 1));
	}
	flag |= 1;
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

SkillChattering::SkillChattering() : SkillImpl(SU_CHATTERING) {
}

void SkillChattering::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(target,*target,getSkillId(),skill_lv,
		sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
}

SkillChulhoBattering::SkillChulhoBattering() : SkillImplRecursiveDamageSplash(SH_CHUL_HO_BATTERING) {
}

void SkillChulhoBattering::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 550 + 250 * skill_lv;
	skillratio += 70 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillChulhoBattering::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillChulhoSonicClaw::SkillChulhoSonicClaw() : WeaponSkillImpl(SH_CHUL_HO_SONIC_CLAW) {
}

void SkillChulhoSonicClaw::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 1450 + 2650 * skill_lv;
	skillratio += 50 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->pow;

	if( pc_checkskill( sd, SH_COMMUNE_WITH_CHUL_HO ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) ){
		skillratio += -50 + 350 * skill_lv;
		skillratio += 50 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	}
	RE_LVL_DMOD(100);
}

void SkillChulhoSonicClaw::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillColorsofHyunrok::SkillColorsofHyunrok() : SkillImpl(SH_COLORS_OF_HYUN_ROK) {
}

void SkillColorsofHyunrok::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (skill_lv == 7) {
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_1);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_2);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_3);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_4);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_5);
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_6);
		// The skill also ends the buff that increases Catnip Meteor damage
		status_change_end(src, SC_COLORS_OF_HYUN_ROK_BUFF);

		clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
	}
	else {
		map_session_data* sd = BL_CAST(BL_PC, src);
		status_change *sc = status_get_sc(src);
		sc_type type = skill_get_sc(getSkillId());

		// Buff to increase Catnip Meteor damage
		if( pc_checkskill( sd, SH_COMMUNE_WITH_HYUN_ROK ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) )
			sc_start(src, target, SC_COLORS_OF_HYUN_ROK_BUFF, 100, 1, skill_get_time(getSkillId(), skill_lv));

		// Endows elemental property to Catnip Meteor, Hyunrok Breeze and Hyunrok Cannon skills
		switch (skill_lv) {
			case 1:
				type = SC_COLORS_OF_HYUN_ROK_1;
				break;
			case 2:
				type = SC_COLORS_OF_HYUN_ROK_2;
				break;
			case 3:
				type = SC_COLORS_OF_HYUN_ROK_3;
				break;
			case 4:
				type = SC_COLORS_OF_HYUN_ROK_4;
				break;
			case 5:
				type = SC_COLORS_OF_HYUN_ROK_5;
				break;
			case 6:
				type = SC_COLORS_OF_HYUN_ROK_6;
				break;
		}
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(),skill_lv));
		clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
	}
}

SkillGrooming::SkillGrooming() : SkillImpl(SU_GROOMING) {
}

void SkillGrooming::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(target,*target,getSkillId(),skill_lv,
		sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
}

SkillHiss::SkillHiss() : SkillImpl(SU_HISS) {
}

void SkillHiss::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || flag&1) {
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	} else if (sd) {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillHogogongStrike::SkillHogogongStrike() : SkillImpl(SH_HOGOGONG_STRIKE) {
}

void SkillHogogongStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 180 + 200 * skill_lv;
	skillratio += 10 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->pow;

	if( pc_checkskill( sd, SH_COMMUNE_WITH_CHUL_HO ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) ){
		skillratio += 70 + 150 * skill_lv;
		skillratio += 10 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	}
	RE_LVL_DMOD(100);
}

void SkillHogogongStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if( flag&1 && tsc != nullptr && tsc->getSCE( SC_HOGOGONG ) != nullptr ){
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	}
}

void SkillHogogongStrike::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( pc_checkskill( sd, SH_COMMUNE_WITH_CHUL_HO ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) )
		status_heal(src, 0, 0, 1, 0);
	skill_area_temp[0] = 0;
	skill_area_temp[1] = target->id;
	skill_area_temp[2] = 0;
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
}

SkillHowlingofChulho::SkillHowlingofChulho() : SkillImpl(SH_HOWLING_OF_CHUL_HO) {
}

void SkillHowlingofChulho::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillHowlingofChulho::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 600 + 1050 * skill_lv;
	skillratio += 50 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->pow;

	if( pc_checkskill( sd, SH_COMMUNE_WITH_CHUL_HO ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) ){
		skillratio += 100 + 100 * skill_lv;
		skillratio += 50 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	}
	RE_LVL_DMOD(100);
}

void SkillHowlingofChulho::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillHowlingofChulho::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);

	int32 range = skill_get_splash(getSkillId(), skill_lv);

	if( pc_checkskill( sd, SH_COMMUNE_WITH_CHUL_HO ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) ){
		range += 1;
	}

	skill_area_temp[0] = 0;
	skill_area_temp[1] = target->id;
	skill_area_temp[2] = 0;
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	map_foreachinrange(skill_area_sub, target, range, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
}

SkillHyunrokBreeze::SkillHyunrokBreeze() : SkillImpl(SH_HYUN_ROKS_BREEZE) {
}

void SkillHyunrokBreeze::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 650 + 750 * skill_lv;
	skillratio += 20 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->spl;

	if( pc_checkskill( sd, SH_COMMUNE_WITH_HYUN_ROK ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) ){
		skillratio += 100 + 200 * skill_lv;
		skillratio += 20 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	}
	RE_LVL_DMOD(100);
}

void SkillHyunrokBreeze::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillHyunrokBreeze::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if( sc != nullptr && !sc->empty() ){
		if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_1 ) ){
			element = ELE_WATER;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_2 ) ){
			element = ELE_WIND;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_3 ) ){
			element = ELE_EARTH;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_4 ) ){
			element = ELE_FIRE;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_5 ) ){
			element = ELE_DARK;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_6 ) ){
			element = ELE_HOLY;
		}
	}
}

SkillHyunrokCannon::SkillHyunrokCannon() : SkillImpl(SH_HYUN_ROK_CANNON) {
}

void SkillHyunrokCannon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 1450 + 2250 * skill_lv;
	skillratio += 50 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->spl;

	if( pc_checkskill( sd, SH_COMMUNE_WITH_HYUN_ROK ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) ){
		skillratio += 450 * skill_lv;
		skillratio += 25 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	}
	RE_LVL_DMOD(100);
}

void SkillHyunrokCannon::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillHyunrokCannon::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if( sc != nullptr && !sc->empty() ){
		if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_1 ) ){
			element = ELE_WATER;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_2 ) ){
			element = ELE_WIND;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_3 ) ){
			element = ELE_EARTH;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_4 ) ){
			element = ELE_FIRE;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_5 ) ){
			element = ELE_DARK;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_6 ) ){
			element = ELE_HOLY;
		}
	}
}

SkillHyunrokSpiritPower::SkillHyunrokSpiritPower() : SkillImplRecursiveDamageSplash(SH_HYUN_ROK_SPIRIT_POWER) {
}

void SkillHyunrokSpiritPower::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 350 + 200 * skill_lv;
	skillratio += 30 * pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY);
	skillratio += 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillHyunrokSpiritPower::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

void SkillHyunrokSpiritPower::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if( sc != nullptr && !sc->empty() ){
		if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_1 ) ){
			element = ELE_WATER;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_2 ) ){
			element = ELE_WIND;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_3 ) ){
			element = ELE_EARTH;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_4 ) ){
			element = ELE_FIRE;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_5 ) ){
			element = ELE_DARK;
		}else if( sc->hasSCE( SC_COLORS_OF_HYUN_ROK_6 ) ){
			element = ELE_HOLY;
		}
	}
}

SkillKisulRampage::SkillKisulRampage() : SkillImpl(SH_KI_SUL_RAMPAGE) {
}

void SkillKisulRampage::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&2 ){
		if( src == target ){
			return;
		}

		int64 ap = 2;

		if( flag&4 ){
			ap += 4;
		}

		status_heal( target, 0, 0, ap, 0 );
	}else if( flag&1 ){
		map_session_data* sd = BL_CAST(BL_PC, src);
		status_change *sc = status_get_sc(src);
		int32 range = skill_get_splash( getSkillId(), skill_lv );

		if( pc_checkskill( sd, SH_COMMUNE_WITH_KI_SUL ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) ){
			range += 2;
			// Set a flag for AP increase
			flag |= 4;
		}

		clif_skill_nodamage( src, *target, getSkillId(), 0 );
		map_foreachinrange( skill_area_sub, target, range, BL_CHAR, target, getSkillId(), skill_lv, tick, flag|BCT_PARTY|2, skill_castend_nodamage_id );
	}else{
		// No party check required
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
}

SkillKisulWaterSpraying::SkillKisulWaterSpraying() : SkillImpl(SH_KI_SUL_WATER_SPRAYING) {
}

void SkillKisulWaterSpraying::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		// TODO: verify on official server, if this should be moved into skill_calc_heal
		int32 heal = 500 * skill_lv + status_get_int(src) * 5;
		heal += pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY) * 100;

		if( pc_checkskill( sd, SH_COMMUNE_WITH_KI_SUL ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) ){
			heal += 250 * skill_lv;
			heal += pc_checkskill(sd, SH_MYSTICAL_CREATURE_MASTERY) * 50;
		}
		heal = heal * (100 + status_get_crt(src)) * status_get_lv(src) / 10000;
		status_heal(target, heal, 0, 0, 0);
		clif_skill_nodamage(nullptr, *target, AL_HEAL, heal);
	}
	else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		int32 range = skill_get_splash(getSkillId(), skill_lv);
		if( pc_checkskill( sd, SH_COMMUNE_WITH_KI_SUL ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) )
			range += 2;
		party_foreachsamemap(skill_area_sub, sd, range, src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillLope::SkillLope() : SkillImpl(SU_LOPE) {
}

void SkillLope::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Fails on noteleport maps, except for GvG and BG maps
	if (map_getmapflag(src->m, MF_NOTELEPORT) && !(map_getmapflag(src->m, MF_BATTLEGROUND) || map_flag_gvg2(src->m))) {
		x = src->x;
		y = src->y;
	}

	clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
	if (!map_count_oncell(src->m, x, y, BL_PC|BL_NPC|BL_MOB, 0) && map_getcell(src->m, x, y, CELL_CHKREACH) && unit_movepos(src, x, y, 1, 0))
		clif_blown(src);
}

// SU_LUNATICCARROTBEAT
SkillLunaticCarrotBeat::SkillLunaticCarrotBeat() : SkillImplRecursiveDamageSplash(SU_LUNATICCARROTBEAT) {
}

void SkillLunaticCarrotBeat::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += 100 + 100 * skill_lv;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		skillratio += skillratio * status_get_hp(src) / status_get_max_hp(src);
	if (status_get_lv(src) > 99)
		skillratio += sstatus->str;
	RE_LVL_DMOD(100);
}

void SkillLunaticCarrotBeat::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (!(flag & 1)) {
		map_session_data* sd = BL_CAST(BL_PC, src);

		// FIX ME: missing check of required item
		if (sd && pc_search_inventory(sd, skill_db.find(getSkillId())->require.itemid[0]) >= 0) {
			SkillLunaticCarrotBeat2 lunatic2;
			lunatic2.castendDamageId(src, target, skill_lv, tick, flag);
		}
		else {
			SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);
		}
	}
}


// SU_LUNATICCARROTBEAT2
SkillLunaticCarrotBeat2::SkillLunaticCarrotBeat2() : SkillImplRecursiveDamageSplash(SU_LUNATICCARROTBEAT2) {
}

void SkillLunaticCarrotBeat2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += 100 + 100 * skill_lv;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		skillratio += skillratio * status_get_hp(src) / status_get_max_hp(src);
	if (status_get_lv(src) > 99)
		skillratio += sstatus->str;
	RE_LVL_DMOD(100);
}

void SkillLunaticCarrotBeat2::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_STUN, 20, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

SkillMarineFestivalofKisul::SkillMarineFestivalofKisul() : SkillImpl(SH_MARINE_FESTIVAL_OF_KI_SUL) {
}

void SkillMarineFestivalofKisul::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		int32 time = skill_get_time(getSkillId(), skill_lv);
		if( pc_checkskill( sd, SH_COMMUNE_WITH_KI_SUL ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) )
			time *= 2;
		sc_start(src, target, type, 100, skill_lv, time);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
	else {
		int32 range = skill_get_splash(getSkillId(), skill_lv);
		if( pc_checkskill( sd, SH_COMMUNE_WITH_KI_SUL ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) )
			range += 2;
		party_foreachsamemap(skill_area_sub, sd, range, src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillMeowMeow::SkillMeowMeow() : SkillImpl(SU_MEOWMEOW) {
}

void SkillMeowMeow::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || flag&1) {
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	} else if (sd) {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillNyangGrass::SkillNyangGrass() : SkillImpl(SU_NYANGGRASS) {
}

void SkillNyangGrass::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && pc_checkskill(sd, SU_SPIRITOFLAND)) {
		sc_start(src, src, SC_DORAM_MATK, 100, sd->status.base_level, skill_get_time(SU_SPIRITOFLAND, 1));
	}
	flag |= 1;
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

// SU_PICKYPECK
SkillPickyPeck::SkillPickyPeck() : WeaponSkillImpl(SU_PICKYPECK) {
}

void SkillPickyPeck::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += 100 + 100 * skill_lv;
	if (status_get_hp(target) < (status_get_max_hp(target) / 2))
		base_skillratio *= 2;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		base_skillratio += base_skillratio * status_get_hp(src) / status_get_max_hp(src);
}

void SkillPickyPeck::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}


// SU_PICKYPECK_DOUBLE_ATK
// FIX ME: this skill is never triggered
SkillPickyPeckDoubleAttack::SkillPickyPeckDoubleAttack() : SkillImpl(SU_PICKYPECK_DOUBLE_ATK) {
}

void SkillPickyPeckDoubleAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += 100 + 100 * skill_lv;
	if (status_get_hp(target) < (status_get_max_hp(target) / 2))
		base_skillratio *= 2;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		base_skillratio += base_skillratio * status_get_hp(src) / status_get_max_hp(src);
}

SkillPowerofFlock::SkillPowerofFlock() : SkillImpl(SU_POWEROFFLOCK) {
}

void SkillPowerofFlock::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1) {
		sc_start(src, target, SC_FEAR, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
		sc_start(src, target, SC_FREEZE, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv)); //! TODO: What's the duration?
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		if (battle_config.skill_wall_check)
			map_foreachinshootrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
		else
			map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
	}
}

SkillPurring::SkillPurring() : SkillImpl(SU_PURRING) {
}

void SkillPurring::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || flag&1) {
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	} else if (sd) {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillSandyFestivalofKisul::SkillSandyFestivalofKisul() : SkillImpl(SH_SANDY_FESTIVAL_OF_KI_SUL) {
}

void SkillSandyFestivalofKisul::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		int32 time = skill_get_time(getSkillId(), skill_lv);
		if( pc_checkskill( sd, SH_COMMUNE_WITH_KI_SUL ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) )
			time *= 2;
		sc_start(src, target, type, 100, skill_lv, time);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
	else {
		int32 range = skill_get_splash(getSkillId(), skill_lv);
		if( pc_checkskill( sd, SH_COMMUNE_WITH_KI_SUL ) > 0 || ( sc != nullptr && sc->getSCE( SC_TEMPORARY_COMMUNION ) != nullptr ) )
			range += 2;
		party_foreachsamemap(skill_area_sub, sd, range, src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillScarofTarou::SkillScarofTarou() : WeaponSkillImpl(SU_SCAROFTAROU) {
}

void SkillScarofTarou::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_STUN, 10, skill_lv, skill_get_time2(getSkillId(), skill_lv)); //! TODO: What's the chance/time?
}

void SkillScarofTarou::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += -100 + 100 * skill_lv;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		base_skillratio += base_skillratio * status_get_hp(src) / status_get_max_hp(src);
}

void SkillScarofTarou::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_start(src, target, SC_BITESCAR, 10, skill_lv, skill_get_time(getSkillId(), skill_lv)); //! TODO: What's the activation chance for the Bite effect?

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillScratch::SkillScratch() : SkillImplRecursiveDamageSplash(SU_SCRATCH) {
}

void SkillScratch::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start2(src, target, SC_BLEEDING, skill_lv * 10 + 70, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv));
}

void SkillScratch::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -50 + 50 * skill_lv;
}

void SkillScratch::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillSilvervineRootTwist::SkillSilvervineRootTwist() : SkillImpl(SU_SV_ROOTTWIST) {
}

void SkillSilvervineRootTwist::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (sd && status_get_class_(target) == CLASS_BOSS) {
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_TOTARGET );
		return;
	}
	if (tsc != nullptr && tsc->hasSCE(type)) // Refresh the status only if it's already active.
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	else {
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
		if (sd && pc_checkskill(sd, SU_SPIRITOFLAND))
			sc_start(src, src, SC_DORAM_MATK, 100, sd->status.base_level, skill_get_time(SU_SPIRITOFLAND, 1));
		skill_addtimerskill(src, tick + 1000, target->id, 0, 0, SU_SV_ROOTTWIST_ATK, skill_lv, skill_get_type(SU_SV_ROOTTWIST_ATK), flag);
	}
}

SkillSilvervineStemSpear::SkillSilvervineStemSpear() : SkillImpl(SU_SV_STEMSPEAR) {
}

void SkillSilvervineStemSpear::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start2(src, target, SC_BLEEDING, 10, skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv));
}

void SkillSilvervineStemSpear::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 600;
}

void SkillSilvervineStemSpear::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && pc_checkskill(sd, SU_SPIRITOFLAND))
		sc_start(src, src, SC_DORAM_WALKSPEED, 100, 50, skill_get_time(SU_SPIRITOFLAND, 1));
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillSpiritofSavage::SkillSpiritofSavage() : SkillImpl(SU_SVG_SPIRIT) {
}

void SkillSpiritofSavage::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += 150 + 150 * skill_lv;
	if (sd && pc_checkskill(sd, SU_SPIRITOFLIFE))
		base_skillratio += base_skillratio * status_get_hp(src) / status_get_max_hp(src);
}

void SkillSpiritofSavage::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = target->id;
	map_foreachinpath(skill_attack_area, src->m, src->x, src->y, target->x, target->y,
		skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), splash_target(src),
		skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
}

SkillTastyShrimpParty::SkillTastyShrimpParty() : SkillImpl(SU_SHRIMPARTY) {
}

void SkillTastyShrimpParty::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 i = 0;

	if (sd == nullptr || sd->status.party_id == 0 || flag&1) {
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
		if (sd && (i = pc_checkskill(sd, SU_FRESHSHRIMP)) > 0) {
			clif_skill_nodamage(target, *target, SU_FRESHSHRIMP, i, 1);
			sc_start(src, target, SC_FRESHSHRIMP, 100, i, skill_get_time(SU_FRESHSHRIMP, i));
		}
	} else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillTunaBelly::SkillTunaBelly() : SkillImpl(SU_TUNABELLY) {
}

void SkillTunaBelly::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	uint32 heal = 0;

	if (dstmd && (dstmd->mob_id == MOBID_EMPERIUM || status_get_class_(target) == CLASS_BATTLEFIELD))
		heal = 0;
	else if (status_get_hp(target) != status_get_max_hp(target))
		heal = ((2 * skill_lv - 1) * 10) * status_get_max_hp(target) / 100;
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	status_heal(target, heal, 0, 0);
}

SkillTunaParty::SkillTunaParty() : SkillImpl(SU_TUNAPARTY) {
}

void SkillTunaParty::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(target,*target,getSkillId(),skill_lv,
		sc_start(src,target,skill_get_sc(getSkillId()),100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
}

std::unique_ptr<const SkillImpl> SkillFactorySummoner::create(const e_skill skill_id) const {
	switch( skill_id ){
		case SH_BLESSING_OF_MYSTICAL_CREATURES:
			return std::make_unique<SkillBlessingofMysticalCreatures>();
		case SH_CHUL_HO_BATTERING:
			return std::make_unique<SkillChulhoBattering>();
		case SH_CHUL_HO_SONIC_CLAW:
			return std::make_unique<SkillChulhoSonicClaw>();
		case SH_COLORS_OF_HYUN_ROK:
			return std::make_unique<SkillColorsofHyunrok>();
		case SH_HOGOGONG_STRIKE:
			return std::make_unique<SkillHogogongStrike>();
		case SH_HOWLING_OF_CHUL_HO:
			return std::make_unique<SkillHowlingofChulho>();
		case SH_HYUN_ROKS_BREEZE:
			return std::make_unique<SkillHyunrokBreeze>();
		case SH_HYUN_ROK_CANNON:
			return std::make_unique<SkillHyunrokCannon>();
		case SH_HYUN_ROK_SPIRIT_POWER:
			return std::make_unique<SkillHyunrokSpiritPower>();
		case SH_KI_SUL_RAMPAGE:
			return std::make_unique<SkillKisulRampage>();
		case SH_KI_SUL_WATER_SPRAYING:
			return std::make_unique<SkillKisulWaterSpraying>();
		case SH_MARINE_FESTIVAL_OF_KI_SUL:
			return std::make_unique<SkillMarineFestivalofKisul>();
		case SH_SANDY_FESTIVAL_OF_KI_SUL:
			return std::make_unique<SkillSandyFestivalofKisul>();
		case SH_TEMPORARY_COMMUNION:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SU_ARCLOUSEDASH:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SU_BITE:
			return std::make_unique<SkillBite>();
		case SU_BUNCHOFSHRIMP:
			return std::make_unique<SkillBunchofShrimp>();
		case SU_CHATTERING:
			return std::make_unique<SkillChattering>();	// FIX ME: this skill seems to be StatusSkillImpl
		case SU_CN_METEOR:
			return std::make_unique<SkillCatnipMeteor>();
		case SU_CN_METEOR2:
			return std::make_unique<SkillCatnipMeteor2>();
		case SU_CN_POWDERING:
			return std::make_unique<SkillCatnipPowdering>();
		case SU_FRESHSHRIMP:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SU_GROOMING:
			return std::make_unique<SkillGrooming>();	// FIX ME: this skill seems to be StatusSkillImpl
		case SU_HIDE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case SU_HISS:
			return std::make_unique<SkillHiss>();
		case SU_LOPE:
			return std::make_unique<SkillLope>();
		case SU_LUNATICCARROTBEAT:
			return std::make_unique<SkillLunaticCarrotBeat>();
		case SU_LUNATICCARROTBEAT2:
			return std::make_unique<SkillLunaticCarrotBeat2>();
		case SU_MEOWMEOW:
			return std::make_unique<SkillMeowMeow>();
		case SU_NYANGGRASS:
			return std::make_unique<SkillNyangGrass>();
		case SU_PICKYPECK:
			return std::make_unique<SkillPickyPeck>();
		case SU_PICKYPECK_DOUBLE_ATK:
			return std::make_unique<SkillPickyPeckDoubleAttack>();
		case SU_POWEROFFLOCK:
			return std::make_unique<SkillPowerofFlock>();
		case SU_PURRING:
			return std::make_unique<SkillPurring>();
		case SU_SCAROFTAROU:
			return std::make_unique<SkillScarofTarou>();
		case SU_SCRATCH:
			return std::make_unique<SkillScratch>();
		case SU_SHRIMPARTY:
			return std::make_unique<SkillTastyShrimpParty>();
		case SU_STOOP:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SU_SVG_SPIRIT:
			return std::make_unique<SkillSpiritofSavage>();
		case SU_SV_ROOTTWIST:
			return std::make_unique<SkillSilvervineRootTwist>();
		case SU_SV_STEMSPEAR:
			return std::make_unique<SkillSilvervineStemSpear>();
		case SU_TUNABELLY:
			return std::make_unique<SkillTunaBelly>();
		case SU_TUNAPARTY:
			return std::make_unique<SkillTunaParty>();

		default:
			return nullptr;
	}
}

#endif
