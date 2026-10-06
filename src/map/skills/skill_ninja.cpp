// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_ninja.hpp"

#include "map/clif.hpp"
#include "map/status.hpp"
#include <config/core.hpp>
#include "map/map.hpp"
#include "map/pc.hpp"
#include "map/unit.hpp"
#include "map/battle.hpp"
#include <common/random.hpp>
#include "map/mob.hpp"
#include "map/path.hpp"
#include "skill_impl.hpp"

Skill16thNight::Skill16thNight() : StatusSkillImpl(KO_IZAYOI) {
}

void Skill16thNight::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
}

SkillCastNinjaSpell::SkillCastNinjaSpell() : SkillImpl(KO_ZENKAI) {
}

void SkillCastNinjaSpell::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillColdBloodedCannon::SkillColdBloodedCannon() : SkillImpl(SS_REIKETSUHOU) {
}

void SkillColdBloodedCannon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 450 + 950 * skill_lv;
	skillratio += 40 * pc_checkskill( sd, SS_ANTENPOU ) * skill_lv;
	skillratio += 5 * sstatus->spl;

	if( sc != nullptr && sc->hasSCE( SC_WATER_CHARM_POWER ) ){
		skillratio += 7000;
	}

	RE_LVL_DMOD(100);
}

void SkillColdBloodedCannon::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillColdBloodedCannon::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	skill_mirage_cast(*src, nullptr, SS_ANTENPOU, skill_lv, 0, 0, tick, flag | BCT_WOS);
	if (map_getcell(src->m, x, y, CELL_CHKLANDPROTECTOR)) {
		if (sd != nullptr) {
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		}
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub, src->m, x - i, y - i, x + i, y + i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
}

SkillCrimsonFireFormation::SkillCrimsonFireFormation() : SkillImpl(NJ_KAENSIN) {
}

void SkillCrimsonFireFormation::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio -= 50;
	if(sd && sd->spiritcharm_type == CHARM_TYPE_FIRE && sd->spiritcharm > 0)
		base_skillratio += 20 * sd->spiritcharm;
}

void SkillCrimsonFireFormation::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillCrimsonFirePetal::SkillCrimsonFirePetal() : SkillImpl(NJ_KOUENKA) {
}

void SkillCrimsonFirePetal::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio -= 10;
	if(sd && sd->spiritcharm_type == CHARM_TYPE_FIRE && sd->spiritcharm > 0)
		base_skillratio += 10 * sd->spiritcharm;
}

void SkillCrimsonFirePetal::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillDarkDragonNightmare::SkillDarkDragonNightmare() : SkillImpl(SS_ANKOKURYUUAKUMU) {
}

void SkillDarkDragonNightmare::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change_end(target, SC_NIGHTMARE);
}

void SkillDarkDragonNightmare::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 17500 * skill_lv;
	skillratio += 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillDarkDragonNightmare::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if (flag & 1) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);

		if( tsc != nullptr && tsc->getSCE( SC_NIGHTMARE ) != nullptr ){
			skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag | SKILL_ALTDMG_FLAG);
		}
	}
}

void SkillDarkDragonNightmare::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 range = skill_get_splash( getSkillId(), skill_lv );

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	map_foreachinrange( skill_area_sub, target, range, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id );
}

void SkillDarkDragonNightmare::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	if (dmg.miscflag & SKILL_ALTDMG_FLAG)
		element = ELE_FIRE;
}

SkillDarkeningCannon::SkillDarkeningCannon() : SkillImpl(SS_ANTENPOU) {
}

void SkillDarkeningCannon::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillDarkeningCannon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 450 + 950 * skill_lv;
	skillratio += 5 * sstatus->spl;
	RE_LVL_DMOD(100);
	if (mflag & SKILL_ALTDMG_FLAG)
		skillratio = skillratio * 3 / 10;
}

void SkillDarkeningCannon::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillDarkeningCannon::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_mirage_cast(*src, nullptr,getSkillId(), skill_lv, 0, 0, tick, flag | BCT_WOS);
	int32 range = skill_get_splash( getSkillId(), skill_lv );

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	map_foreachinrange( skill_area_sub, target, range, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id );
}

SkillDistortedCrescent::SkillDistortedCrescent() : StatusSkillImpl(OB_ZANGETSU) {
}

void SkillDistortedCrescent::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
}

SkillEarthCharm::SkillEarthCharm() : SkillImpl(KO_DOHU_KOUKAI) {
}

void SkillEarthCharm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		int32 ele_type = skill_get_ele(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		pc_addspiritcharm(sd,skill_get_time(getSkillId(),skill_lv),MAX_SPIRITCHARM,ele_type);
	}
}

SkillEmptyShadow::SkillEmptyShadow() : StatusSkillImpl(KG_KYOMU) {
}

void SkillEmptyShadow::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
}

SkillFinalStrike::SkillFinalStrike() : WeaponSkillImpl(NJ_ISSEN) {
}

void SkillFinalStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	int16 x, y;
	int16 dir = map_calc_dir(src, target->x, target->y);

	int16 i = 2; // Move 2 cells (From target)

	if (dir > 0 && dir < 4)
		x = -i;
	else if (dir > 4)
		x = i;
	else
		x = 0;
	if (dir > 2 && dir < 6)
		y = -i;
	else if (dir == 7 || dir < 2)
		y = i;
	else
		y = 0;

#ifdef RENEWAL
	// Doesn't have slide effect in GVG
	if (skill_check_unit_movepos(5, src, target->x + x, target->y + y, 1, 1)) {
		clif_blown(src);
		clif_spiritball(src);
	}
	skill_attack(BF_MISC, src, src, target, getSkillId(), skill_lv, tick, flag);
	status_set_hp(src, umax(status_get_max_hp(src) / 100, 1), 0);
	status_change_end(src, SC_NEN);
	status_change_end(src, SC_HIDING);
#else
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);

	status_set_hp(src, 1, 0);
	status_change_end(src, SC_NEN);
	status_change_end(src, SC_HIDING);

	// Doesn't have slide effect in GVG
	if (skill_check_unit_movepos(5, src, target->x + x, target->y + y, 1, 1)) {
		clif_blown(src);
		clif_spiritball(src);
	}
#endif
}

SkillFireCharm::SkillFireCharm() : SkillImpl(KO_KAHU_ENTEN) {
}

void SkillFireCharm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		int32 ele_type = skill_get_ele(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		pc_addspiritcharm(sd,skill_get_time(getSkillId(),skill_lv),MAX_SPIRITCHARM,ele_type);
	}
}

SkillFourColorsCharm::SkillFourColorsCharm() : StatusSkillImpl(SS_FOUR_CHARM) {
}

void SkillFourColorsCharm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd != nullptr) {
		sc_type type = skill_get_sc(getSkillId());

		switch (sd->spiritcharm_type) {
			case CHARM_TYPE_FIRE:  type = SC_FIRE_CHARM_POWER;    break;
			case CHARM_TYPE_WATER: type = SC_WATER_CHARM_POWER;   break;
			case CHARM_TYPE_LAND:  type = SC_GROUND_CHARM_POWER;  break;
			case CHARM_TYPE_WIND:  type = SC_WIND_CHARM_POWER;    break;
			default:  type = SC_NONE;    break;
		}
		if (type != SC_NONE) {
			clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
				sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
		}
	}
}

SkillGoldenDragonCannon::SkillGoldenDragonCannon() : SkillImplRecursiveDamageSplash(SS_KINRYUUHOU) {
}

void SkillGoldenDragonCannon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 800 + 1500 * skill_lv;
	skillratio += 15 * pc_checkskill( sd, SS_ANTENPOU ) * skill_lv;
	skillratio += 5 * sstatus->spl;

	if( sc != nullptr && sc->hasSCE( SC_GROUND_CHARM_POWER ) ){
		skillratio += 5500;
	}

	RE_LVL_DMOD(100);
}

void SkillGoldenDragonCannon::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	skill_mirage_cast(*src, nullptr, SS_ANTENPOU, skill_lv, 0, 0, tick, flag | BCT_WOS);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillHiddenWater::SkillHiddenWater() : SkillImpl(NJ_SUITON) {
}

void SkillHiddenWater::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillHuumaShurikenConstruct::SkillHuumaShurikenConstruct() : WeaponSkillImpl(SS_FUUMAKOUCHIKU) {
}

void SkillHuumaShurikenConstruct::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 900 + 1750 * skill_lv;
	if( wd->miscflag&SKILL_ALTDMG_FLAG ){
		skillratio += 200;
	}
	skillratio += pc_checkskill( sd, SS_FUUMASHOUAKU ) * 100 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillHuumaShurikenConstruct::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = 0;
	if (battle_config.skill_eightpath_algorithm) {
		//Use official AoE algorithm
		map_foreachindir(skill_attack_area, src->m, src->x, src->y, x, y,
			skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), 0, BL_CHAR | BL_SKILL,
			skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
	}
	else {
		map_foreachinpath(skill_attack_area, src->m, src->x, src->y, x, y,
			skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), BL_CHAR | BL_SKILL,
			skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
	}
}

void SkillHuumaShurikenConstruct::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if( sd != nullptr ){
		element = sd->bonus.arrow_ele;
	}
}

SkillHuumaShurikenGrasp::SkillHuumaShurikenGrasp() : SkillImpl(SS_FUUMASHOUAKU) {
}

void SkillHuumaShurikenGrasp::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillHuumaShurikenGrasp::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 850 + 350 * skill_lv;
	skillratio += pc_checkskill( sd, SS_FUUMAKOUCHIKU ) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillHuumaShurikenGrasp::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Ammo should be deleted right away.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillHuumaShurikenGrasp::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if( sd != nullptr ){
		element = sd->bonus.arrow_ele;
	}
}

SkillIceCharm::SkillIceCharm() : SkillImpl(KO_HYOUHU_HUBUKI) {
}

void SkillIceCharm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		int32 ele_type = skill_get_ele(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		pc_addspiritcharm(sd,skill_get_time(getSkillId(),skill_lv),MAX_SPIRITCHARM,ele_type);
	}
}

SkillIceMeteor::SkillIceMeteor() : SkillImpl(NJ_HYOUSYOURAKU) {
}

void SkillIceMeteor::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_FREEZE,(10+10*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillIceMeteor::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += 50 * skill_lv;
	if(sd && sd->spiritcharm_type == CHARM_TYPE_WATER && sd->spiritcharm > 0)
		base_skillratio += 100 * sd->spiritcharm;
}

void SkillIceMeteor::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillIllusionBewitch::SkillIllusionBewitch() : SkillImpl(KO_GENWAKU) {
}

void SkillIllusionBewitch::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_data* tstatus = status_get_status_data(*target);
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if ((dstsd || dstmd) && !status_has_mode(tstatus,MD_IGNOREMELEE|MD_IGNOREMAGIC|MD_IGNORERANGED|MD_IGNOREMISC) && battle_check_target(src,target,BCT_ENEMY) > 0) {
		int32 x = src->x, y = src->y;

		if (sd && rnd()%100 > ((45+5*skill_lv) - status_get_int(target)/10)) { //[(Base chance of success) - (Intelligence Objectives / 10)]%.
			clif_skill_fail( *sd, getSkillId() );
			return;
		}

		// Confusion is still inflicted (but rate isn't reduced), no matter map type.
		status_change_start(src, src, SC_CONFUSION, 2500, skill_lv, 0, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NORATEDEF);
		status_change_start(src, target, SC_CONFUSION, 7500, skill_lv, 0, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NORATEDEF);

		if (skill_check_unit_movepos(5,src,target->x,target->y,0,0)) {
			clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
			clif_blown(src);
			if (!unit_blown_immune(target, 0x1)) {
				unit_movepos(target,x,y,0,0);
				if (target->type == BL_PC && pc_issit((TBL_PC*)target))
					clif_sitting(*target); //Avoid sitting sync problem
				clif_blown(target);
				map_foreachinallrange(unit_changetarget, src, AREA_SIZE, BL_CHAR, src, target);
			}
		}
	}
}

SkillIllusionDeath::SkillIllusionDeath() : SkillImpl(KO_JYUSATSU) {
}

void SkillIllusionDeath::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if( dstsd && tsc && !tsc->getSCE(type) &&
		rnd()%100 < ((45+5*skill_lv) + skill_lv*5 - status_get_int(target)/2) ){//[(Base chance of success) + (Skill Level x 5) - (int32 / 2)]%.
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
			status_change_start(src,target,type,10000,skill_lv,0,0,0,skill_get_time(getSkillId(),skill_lv),SCSTART_NOAVOID|SCSTART_NOTICKDEF));
		status_percent_damage(src, target, tstatus->hp * skill_lv * 5, 0, false); // Does not kill the target.
		if( status_get_lv(target) <= status_get_lv(src) )
			status_change_start(src,target,SC_COMA,10,skill_lv,0,src->id,0,0,SCSTART_NONE);
	}else if( sd )
		clif_skill_fail( *sd, getSkillId() );
}

SkillIllusionShadow::SkillIllusionShadow() : SkillImpl(KO_ZANZOU) {
}

void SkillIllusionShadow::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd){
		mob_data *md2 = mob_once_spawn_sub(src, src->m, src->x, src->y, status_get_name(*src), MOBID_ZANZOU, "", SZ_SMALL, AI_NONE);
		if( md2 )
		{
			md2->master_id = src->id;
			md2->special_state.ai = AI_ZANZOU;
			if( md2->deletetimer != INVALID_TIMER )
				delete_timer(md2->deletetimer, mob_timer_delete);
			md2->deletetimer = add_timer (gettick() + skill_get_time(getSkillId(), skill_lv), mob_timer_delete, md2->id, 0);
			mob_spawn( md2 );
			map_foreachinallrange(unit_changetarget, src, AREA_SIZE, BL_MOB, src, md2);
			clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
			skill_blown(src,target,skill_get_blewcount(getSkillId(),skill_lv),unit_getdir(target),BLOWN_NONE);
		}
	}
}

SkillIllusionShock::SkillIllusionShock() : StatusSkillImpl(KO_KYOUGAKU) {
}

void SkillIllusionShock::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if( dstsd && tsc && !tsc->getSCE(type) && rnd()%100 < tstatus->int_/2 ){
		StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
	}else if( sd )
		clif_skill_fail( *sd, getSkillId() );
}

SkillImprovisedDefense::SkillImprovisedDefense() : SkillImpl(NJ_TATAMIGAESHI) {
}

void SkillImprovisedDefense::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 10 * skill_lv;
#ifdef RENEWAL
	base_skillratio *= 2;
#endif
}

void SkillImprovisedDefense::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (skill_unitsetting(src,getSkillId(),skill_lv,src->x,src->y,0))
		sc_start(src,src,skill_get_sc(getSkillId()),100,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

SkillInfiltrate::SkillInfiltrate() : SkillImpl(SS_SHIMIRU) {
}

void SkillInfiltrate::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 700 * skill_lv;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

void SkillInfiltrate::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	struct unit_data *ud = unit_bl2ud(src);

	if (!check_distance_bl(src, target, 0)) {
		uint8 dir = map_calc_dir(src, target->x, target->y);
		int16 x, y;

		if (dir > DIR_NORTH && dir < DIR_SOUTH)
			x = -1;
		else if (dir > DIR_SOUTH)
			x = 1;
		else
			x = 0;

		if (dir > DIR_WEST && dir < DIR_EAST)
			y = -1;
		else if (dir == DIR_NORTHEAST || dir < DIR_WEST)
			y = 1;
		else
			y = 0;

		if (battle_check_target(src, target, BCT_ENEMY) > 0 && unit_movepos(src, target->x + x, target->y + y, 2, true)) {// Display movement + animation.
			dir = dir < 4 ? dir+4 : dir-4; // change direction [Celest]
			unit_setdir(target,dir);
			clif_blown(src);
		} else {
			if (sd != nullptr) {
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_TARGET_SHADOW_SPACE );
			}
			return;
		}
	}

	if (ud == nullptr)
		return;

	for (const std::shared_ptr<s_skill_unit_group>& sug : ud->skillunits) {
		skill_unit* su = sug->unit;
		std::shared_ptr<s_skill_unit_group> sg = su->group;
		int16 dx = src->x - su->x;
		int16 dy = src->y - su->y;

		for( size_t count = 0; count < 1000; count++ ){
			if (map_foreachincell(skill_shimiru_check_cell, src->m, su->x + dx, su->y + dy, BL_CHAR|BL_SKILL) == 0)
				break;
			dx += rnd() % 3 - 1;
			dy += rnd() % 3 - 1;
		}

		if (sug->skill_id == SS_SHINKIROU)
			skill_unit_move_unit_group(sg, src->m, dx,dy);
	}

	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);

	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

SkillKamaitachi::SkillKamaitachi() : SkillImpl(NJ_KAMAITACHI) {
}

void SkillKamaitachi::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += 100 * skill_lv;
	if(sd && sd->spiritcharm_type == CHARM_TYPE_WIND && sd->spiritcharm > 0)
		base_skillratio += 100 * sd->spiritcharm;
}

void SkillKamaitachi::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
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

void SkillKamaitachi::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillKoCrossSlash::SkillKoCrossSlash() : WeaponSkillImpl(KO_JYUMONJIKIRI) {
}

void SkillKoCrossSlash::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *tsc = status_get_sc(&target);

	if (tsc != nullptr && tsc->hasSCE(SC_JYUMONJIKIRI))
		dmg.div_ *= -1; // TODO: needs more info
}

void SkillKoCrossSlash::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_JYUMONJIKIRI,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

void SkillKoCrossSlash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);
	const status_change *tsc = status_get_sc(target);

	skillratio += -100 + 200 * skill_lv;
	RE_LVL_DMOD(120);
	if(tsc && tsc->getSCE(SC_JYUMONJIKIRI))
		skillratio += skill_lv * status_get_lv(src);
	if (sc && sc->getSCE(SC_KAGEMUSYA))
		skillratio += skillratio * sc->getSCE(SC_KAGEMUSYA)->val2 / 100;
}

void SkillKoCrossSlash::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int16 x, y;
	int16 dir = map_calc_dir(src,target->x,target->y);

	if (dir > 0 && dir < 4)
		x = 2;
	else if (dir > 4)
		x = -2;
	else
		x = 0;
	if (dir > 2 && dir < 6)
		y = 2;
	else if (dir == 7 || dir < 2)
		y = -2;
	else
		y = 0;
	if (unit_movepos(src,target->x + x,target->y + y,1,1)) {
		clif_blown(src);
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	}
}

SkillKunaiDistortion::SkillKunaiDistortion() : SkillImpl(SS_KUNAIWAIKYOKU) {
}

void SkillKunaiDistortion::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillKunaiDistortion::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 300 + 600 * skill_lv;
	skillratio += pc_checkskill( sd, SS_KUNAIKUSSETSU ) * 10 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
		skillratio = skillratio * 3 / 10;
}

void SkillKunaiDistortion::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillKunaiDistortion::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_mirage_cast(*src, nullptr, getSkillId(), skill_lv, x, y, tick, flag | BCT_WOS);
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub, src->m, x - i, y - i, x + i, y + i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, UNIT_NOCONSUME_AMMO);
}

SkillKunaiExplosion::SkillKunaiExplosion() : SkillImplRecursiveDamageSplash(KO_BAKURETSU) {
}

void SkillKunaiExplosion::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + (sd ? pc_checkskill(sd,NJ_TOBIDOUGU) : 1) * (50 + sstatus->dex / 4) * skill_lv * 4 / 10;
	RE_LVL_DMOD(120);
	skillratio += 10 * (sd ? sd->status.job_level : 1);
	if (sc && sc->getSCE(SC_KAGEMUSYA))
		skillratio += skillratio * sc->getSCE(SC_KAGEMUSYA)->val2 / 100;
}

SkillKunaiNightmare::SkillKunaiNightmare() : SkillImpl(SS_HITOUAKUMU) {
}

void SkillKunaiNightmare::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change_end(target, SC_NIGHTMARE);
}

void SkillKunaiNightmare::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *tsc = status_get_sc(target);

	skillratio += -100 + 22500;
	skillratio += 5 * sstatus->pow;

	if( tsc != nullptr && tsc->getSCE( SC_NIGHTMARE ) != nullptr ){
		skillratio += skillratio / 2;
	}

	RE_LVL_DMOD(100);
}

void SkillKunaiNightmare::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillKunaiNightmare::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 range = skill_get_splash( getSkillId(), skill_lv );

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	map_foreachinrange( skill_area_sub, target, range, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id );
}

SkillKunaiRefraction::SkillKunaiRefraction() : SkillImpl(SS_KUNAIKUSSETSU) {
}

void SkillKunaiRefraction::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 300 + 450 * skill_lv;
	skillratio += pc_checkskill( sd, SS_KUNAIKAITEN ) * 10 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillKunaiRefraction::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_foreachinallrange(skill_detonator, src, skill_get_splash(getSkillId(), skill_lv), BL_SKILL, src, skill_lv);
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
}

SkillKunaiRotation::SkillKunaiRotation() : SkillImpl(SS_KUNAIKAITEN) {
}

void SkillKunaiRotation::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 1000 + 1350 * skill_lv;
	skillratio += pc_checkskill( sd, SS_KUNAIWAIKYOKU ) * 100 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillKunaiRotation::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, UNIT_NOCONSUME_AMMO);
	skill_unitsetting(src, SS_KUNAIWAIKYOKU, skill_lv, x, y, UNIT_NOCONSUME_AMMO);
}

SkillKunaiSplash::SkillKunaiSplash() : SkillImplRecursiveDamageSplash(KO_HAPPOKUNAI) {
}

void SkillKunaiSplash::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);

	if (skill_area_temp[2] == 0) {
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	}
}

SkillLightningStrikeOfDestruction::SkillLightningStrikeOfDestruction() : SkillImpl(NJ_RAIGEKISAI) {
}

void SkillLightningStrikeOfDestruction::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

#ifdef RENEWAL
	base_skillratio += 100 * skill_lv;
#else
	base_skillratio += 60 + 40 * skill_lv;
#endif
	if(sd && sd->spiritcharm_type == CHARM_TYPE_WIND && sd->spiritcharm > 0)
		base_skillratio += 20 * sd->spiritcharm;
}

void SkillLightningStrikeOfDestruction::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillMakibishi::SkillMakibishi() : SkillImpl(KO_MAKIBISHI) {
}

void SkillMakibishi::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target, SC_STUN, 10 * skill_lv, skill_lv, skill_get_time2(getSkillId(),skill_lv));
}

void SkillMakibishi::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 20 * skill_lv;
}

void SkillMakibishi::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	for( int32 i = 0; i < (skill_lv+2); i++ ) {
		x = src->x - 1 + rnd()%3;
		y = src->y - 1 + rnd()%3;
		skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
	}
}

SkillMeltAway::SkillMeltAway() : SkillImpl(SS_TOKEDASU) {
}

void SkillMeltAway::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 700 * skill_lv;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

void SkillMeltAway::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillMeltAway::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub, src->m, x - i, y - i, x + i, y + i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	unit_setdir(src, map_calc_dir_xy(src->x, src->y, x, y, unit_getdir(src)));
	skill_blown(src, src, skill_get_blewcount(getSkillId(), skill_lv), unit_getdir(src), (enum e_skill_blown)(BLOWN_IGNORE_NO_KNOCKBACK | BLOWN_DONT_SEND_PACKET));
	clif_blown(src);
}

SkillMirage::SkillMirage() : SkillImpl(SS_SHINKIROU) {
}

void SkillMirage::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= 1;
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

SkillMirrorImage::SkillMirrorImage() : StatusSkillImpl(NJ_BUNSINJYUTSU) {
}

void SkillMirrorImage::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// TODO: refactor into status.yml
	status_change_end(target, SC_BUNSINJYUTSU); // on official recasting cancels existing mirror image [helvetica]
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
	status_change_end(target, SC_NEN);
}

SkillMoonlightFantasy::SkillMoonlightFantasy() : StatusSkillImpl(OB_OBOROGENSOU) {
}

void SkillMoonlightFantasy::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	// This skill does not work on monsters. And it does not work on status immune monsters.
	if( sd && ( target->type == BL_MOB || status_bl_has_mode(target,MD_STATUSIMMUNE) ) ){ 
		clif_skill_fail( *sd, getSkillId() );
		return;
	}
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
}

SkillNightmareErasion::SkillNightmareErasion() : SkillImpl(SS_AKUMUKESU) {
}

void SkillNightmareErasion::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1) {
		status_change_end(target, SC_NIGHTMARE);
	} else {
		int32 range = skill_get_splash( getSkillId(), skill_lv );

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		map_foreachinrange( skill_area_sub, target, range, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_nodamage_id );
	}
}

SkillOminousMoonlight::SkillOminousMoonlight() : StatusSkillImpl(OB_AKAITSUKI) {
}

void SkillOminousMoonlight::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd && status_bl_has_mode(target,MD_STATUSIMMUNE) ){ // Does not work on status immune monsters.
		clif_skill_fail( *sd, getSkillId() );
		return;
	}
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
}

SkillRagingFireDragon::SkillRagingFireDragon() : SkillImpl(NJ_BAKUENRYU) {
}

void SkillRagingFireDragon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += 50 + 150 * skill_lv;
	if(sd && sd->spiritcharm_type == CHARM_TYPE_FIRE && sd->spiritcharm > 0)
		base_skillratio += 100 * sd->spiritcharm;
}

void SkillRagingFireDragon::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Place units around target
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_unitsetting(src, getSkillId(), skill_lv, target->x, target->y, 0);
}

void SkillRagingFireDragon::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillRapidThrow::SkillRapidThrow() : SkillImplRecursiveDamageSplash(KO_MUCHANAGE) {
}

void SkillRapidThrow::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* sstatus = status_get_status_data(*src);
	int32 i = skill_get_splash(getSkillId(),skill_lv);
	int32 rate = (100 - (1000 / (sstatus->dex + sstatus->luk) * 5)) * (skill_lv / 2 + 5) / 10;
	if( rate < 0 )
		rate = 0;
	skill_area_temp[0] = map_foreachinarea(skill_area_sub,src->m,x-i,y-i,x+i,y+i,BL_CHAR,src,getSkillId(),skill_lv,tick,BCT_ENEMY,skill_area_sub_count);
	if( rnd()%100 < rate )
		map_foreachinarea(skill_area_sub,src->m,x-i,y-i,x+i,y+i,BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
}

SkillRedFlameCannon::SkillRedFlameCannon() : SkillImplRecursiveDamageSplash(SS_SEKIENHOU) {
}

void SkillRedFlameCannon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 600 + 1100 * skill_lv;
	skillratio += 70 * pc_checkskill( sd, SS_ANTENPOU ) * skill_lv;
	skillratio += 5 * sstatus->spl;

	if( sc != nullptr && sc->hasSCE( SC_FIRE_CHARM_POWER ) ){
		skillratio += 8500;
	}

	RE_LVL_DMOD(100);
}

void SkillRedFlameCannon::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	skill_mirage_cast(*src, nullptr, SS_ANTENPOU, skill_lv, 0, 0, tick, flag | BCT_WOS);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillReleaseNinjaSpell::SkillReleaseNinjaSpell() : SkillImpl(KO_KAIHOU) {
}

void SkillReleaseNinjaSpell::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd && sd->spiritcharm_type != CHARM_TYPE_NONE && sd->spiritcharm > 0) {
		skillratio += -100 + 200 * sd->spiritcharm;
		RE_LVL_DMOD(100);
		pc_delspiritcharm(const_cast<map_session_data*>(sd), sd->spiritcharm, sd->spiritcharm_type);
	}
}

void SkillReleaseNinjaSpell::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillReleaseNinjaSpell::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->spiritcharm_type != CHARM_TYPE_NONE && sd->spiritcharm > 0)
		element = sd->spiritcharm_type;
}

SkillShadowDance::SkillShadowDance() : SkillImpl(SS_KAGENOMAI) {
}

void SkillShadowDance::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 750 + 900 * skill_lv;
	skillratio += pc_checkskill( sd, SS_KAGEGARI ) * 70 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
		skillratio = skillratio * 3 / 10;
}

void SkillShadowDance::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillShadowDance::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_mirage_cast(*src, nullptr,getSkillId(), skill_lv, 0, 0, tick, flag | BCT_WOS);
	int32 range = skill_get_splash( getSkillId(), skill_lv );

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	map_foreachinrange( skill_area_sub, target, range, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id );
}

SkillShadowFlash::SkillShadowFlash() : SkillImplRecursiveDamageSplash(SS_KAGEGISSEN) {
}

void SkillShadowFlash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 1500 + 950 * skill_lv;
	skillratio += pc_checkskill( sd, SS_KAGENOMAI ) * 150 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
	if (wd->miscflag & SKILL_ALTDMG_FLAG)
		skillratio = skillratio * 3 / 10;
}

SkillShadowHiding::SkillShadowHiding() : SkillImpl(KO_YAMIKUMO) {
}

void SkillShadowHiding::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;

	if (tsce)
	{
		clif_skill_nodamage(src,*target,getSkillId(),-1,status_change_end(target, type)); //Hide skill-scream animation.
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	clif_skill_nodamage(src,*target,getSkillId(),-1,sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
}

SkillShadowHunting::SkillShadowHunting() : SkillImpl(SS_KAGEGARI) {
}

void SkillShadowHunting::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillShadowHunting::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 600 + 900 * skill_lv;
	skillratio += pc_checkskill( sd, SS_KAGEGISSEN ) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillShadowHunting::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillShadowHunting::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub, src->m, x - i, y - i, x + i, y + i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
}

SkillShadowLeap::SkillShadowLeap() : SkillImpl(NJ_SHADOWJUMP) {
}

void SkillShadowLeap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( map_getcell(src->m,x,y,CELL_CHKREACH) && skill_check_unit_movepos(5, src, x, y, 1, 0) ) //You don't move on GVG grounds.
		clif_blown(src);
	status_change_end(src, SC_HIDING);
}

SkillShadowNightmare::SkillShadowNightmare() : SkillImpl(SS_KAGEAKUMU) {
}

void SkillShadowNightmare::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change_end(target, SC_NIGHTMARE);
}

void SkillShadowNightmare::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *tsc = status_get_sc(target);

	skillratio += -100 + 22500;
	skillratio += 5 * sstatus->pow;

	if( tsc != nullptr && tsc->getSCE( SC_NIGHTMARE ) != nullptr ){
		skillratio += skillratio / 2;
	}

	RE_LVL_DMOD(100);
}

void SkillShadowNightmare::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillShadowNightmare::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 range = skill_get_splash( getSkillId(), skill_lv );

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	map_foreachinrange( skill_area_sub, target, range, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id );
}

SkillShadowSlash::SkillShadowSlash() : WeaponSkillImpl(NJ_KIRIKAGE) {
}

void SkillShadowSlash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += -50 + 150 * skill_lv;
#else
	base_skillratio += 100 * (skill_lv - 1);
#endif
}

void SkillShadowSlash::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( !map_flag_gvg2(src->m) && !map_getmapflag(src->m, MF_BATTLEGROUND) )
	{	//You don't move on GVG grounds.
		int16 x, y;
		map_search_freecell(target, 0, &x, &y, 1, 1, 0);
		if (unit_movepos(src, x, y, 0, 0)) {
			clif_blown(src);
		}
	}
	status_change_end(src, SC_HIDING);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillShadowTrampling::SkillShadowTrampling() : SkillImpl(KG_KAGEHUMI) {
}

void SkillShadowTrampling::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	if( flag&1 ){
		if (target->type != BL_PC)
			return;
		if (tsc && (tsc->option & (OPTION_CLOAK | OPTION_HIDE) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC__SHADOWFORM) || tsc->getSCE(SC_MARIONETTE) || tsc->getSCE(SC_HARMONIZE))) {
				status_change_end(target, SC_HIDING);
				status_change_end(target, SC_CLOAKING);
				status_change_end(target, SC_CLOAKINGEXCEED);
				status_change_end(target, SC_CAMOUFLAGE);
				status_change_end(target, SC_NEWMOON);
				if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
					status_change_end(target, SC__SHADOWFORM);
				status_change_end(target, SC_MARIONETTE);
				status_change_end(target, SC_HARMONIZE);
				sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
		}
	}else{
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillShadowWarrior::SkillShadowWarrior() : StatusSkillImpl(KG_KAGEMUSYA) {
}

void SkillShadowWarrior::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
}

SkillSoulCutter::SkillSoulCutter() : WeaponSkillImpl(KO_SETSUDAN) {
}

void SkillSoulCutter::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Remove soul link when hit.
	status_change_end(target, SC_SPIRIT);
	status_change_end(target, SC_SOULGOLEM);
	status_change_end(target, SC_SOULSHADOW);
	status_change_end(target, SC_SOULFALCON);
	status_change_end(target, SC_SOULFAIRY);
}

void SkillSoulCutter::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *tsc = status_get_sc(target);

	skillratio += 100 * (skill_lv - 1);
	RE_LVL_DMOD(100);
	if (tsc) {
		const status_change_entry *sce;

		if ((sce = tsc->getSCE(SC_SPIRIT)) || (sce = tsc->getSCE(SC_SOULGOLEM)) || (sce = tsc->getSCE(SC_SOULSHADOW)) || (sce = tsc->getSCE(SC_SOULFALCON)) || (sce = tsc->getSCE(SC_SOULFAIRY))) // Bonus damage added when target is soul linked.
			skillratio += 200 * sce->val1;
	}
}

SkillSpearOfIce::SkillSpearOfIce() : SkillImpl(NJ_HYOUSENSOU) {
}

void SkillSpearOfIce::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST( BL_PC, src );

#ifdef RENEWAL
	const status_change *sc = status_get_sc(src);

	base_skillratio -= 30;
	if (sc && sc->getSCE(SC_SUITON))
		base_skillratio += 2 * skill_lv;
#endif
	if(sd && sd->spiritcharm_type == CHARM_TYPE_WATER && sd->spiritcharm > 0)
		base_skillratio += 20 * sd->spiritcharm;
}

void SkillSpearOfIce::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillSwirlingPetal::SkillSwirlingPetal() : SkillImplRecursiveDamageSplash(KO_HUUMARANKA) {
}

void SkillSwirlingPetal::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 150 * skill_lv + sstatus->str + (sd ? pc_checkskill(sd,NJ_HUUMA) * 100 : 0);
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_KAGEMUSYA))
		skillratio += skillratio * sc->getSCE(SC_KAGEMUSYA)->val2 / 100;
}

SkillThrowHuumaShuriken::SkillThrowHuumaShuriken() : SkillImplRecursiveDamageSplash(NJ_HUUMA) {
}

void SkillThrowHuumaShuriken::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += -150 + 250 * skill_lv;
#else
	base_skillratio += 50 + 150 * skill_lv;
#endif
}

void SkillThrowHuumaShuriken::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
#ifdef RENEWAL
	clif_skill_damage( *src, *target,tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
#endif
	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillThrowKunai::SkillThrowKunai() : WeaponSkillImpl(NJ_KUNAI) {
}

void SkillThrowKunai::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += -100 + 100 * skill_lv;
#endif
}

SkillThrowShuriken::SkillThrowShuriken() : WeaponSkillImpl(NJ_SYURIKEN) {
}

void SkillThrowShuriken::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 5 * skill_lv;
#endif
}

SkillThrowZeny::SkillThrowZeny() : SkillImpl(NJ_ZENYNAGE) {
}

void SkillThrowZeny::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillThunderingCannon::SkillThunderingCannon() : SkillImplRecursiveDamageSplash(SS_RAIDENPOU) {
}

void SkillThunderingCannon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 600 + 1100 * skill_lv;
	skillratio += 70 * pc_checkskill( sd, SS_ANTENPOU ) * skill_lv;
	skillratio += 5 * sstatus->spl;

	if( sc != nullptr && sc->hasSCE( SC_WIND_CHARM_POWER ) ){
		skillratio += 8500;
	}

	RE_LVL_DMOD(100);
}

void SkillThunderingCannon::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	skill_mirage_cast(*src, nullptr, SS_ANTENPOU, skill_lv, 0, 0, tick, flag | BCT_WOS);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillVanishingSlash::SkillVanishingSlash() : WeaponSkillImpl(NJ_KASUMIKIRI) {
}

void SkillVanishingSlash::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillVanishingSlash::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 20 * skill_lv;
#else
	base_skillratio += 10 * skill_lv;
#endif
}

SkillWindBlade::SkillWindBlade() : SkillImpl(NJ_HUUJIN) {
}

void SkillWindBlade::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

#ifdef RENEWAL
	base_skillratio += 50;
#endif
	if(sd && sd->spiritcharm_type == CHARM_TYPE_WIND && sd->spiritcharm > 0)
		base_skillratio += 10 * sd->spiritcharm;
}

void SkillWindBlade::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillWindCharm::SkillWindCharm() : SkillImpl(KO_KAZEHU_SEIRAN) {
}

void SkillWindCharm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		int32 ele_type = skill_get_ele(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		pc_addspiritcharm(sd,skill_get_time(getSkillId(),skill_lv),MAX_SPIRITCHARM,ele_type);
	}
}

std::unique_ptr<const SkillImpl> SkillFactoryNinja::create(const e_skill skill_id) const {
	switch( skill_id ){
		case KG_KAGEHUMI:
			return std::make_unique<SkillShadowTrampling>();
		case KG_KAGEMUSYA:
			return std::make_unique<SkillShadowWarrior>();
		case KG_KYOMU:
			return std::make_unique<SkillEmptyShadow>();
		case KO_BAKURETSU:
			return std::make_unique<SkillKunaiExplosion>();
		case KO_DOHU_KOUKAI:
			return std::make_unique<SkillEarthCharm>();
		case KO_GENWAKU:
			return std::make_unique<SkillIllusionBewitch>();
		case KO_HAPPOKUNAI:
			return std::make_unique<SkillKunaiSplash>();
		case KO_HUUMARANKA:
			return std::make_unique<SkillSwirlingPetal>();
		case KO_HYOUHU_HUBUKI:
			return std::make_unique<SkillIceCharm>();
		case KO_IZAYOI:
			return std::make_unique<Skill16thNight>();
		case KO_JYUMONJIKIRI:
			return std::make_unique<SkillKoCrossSlash>();
		case KO_JYUSATSU:
			return std::make_unique<SkillIllusionDeath>();
		case KO_KAHU_ENTEN:
			return std::make_unique<SkillFireCharm>();
		case KO_KAIHOU:
			return std::make_unique<SkillReleaseNinjaSpell>();
		case KO_KAZEHU_SEIRAN:
			return std::make_unique<SkillWindCharm>();
		case KO_KYOUGAKU:
			return std::make_unique<SkillIllusionShock>();
		case KO_MAKIBISHI:
			return std::make_unique<SkillMakibishi>();
		case KO_MEIKYOUSISUI:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case KO_MUCHANAGE:
			return std::make_unique<SkillRapidThrow>();
		case KO_SETSUDAN:
			return std::make_unique<SkillSoulCutter>();
		case KO_YAMIKUMO:
			return std::make_unique<SkillShadowHiding>();
		case KO_ZANZOU:
			return std::make_unique<SkillIllusionShadow>();
		case KO_ZENKAI:
			return std::make_unique<SkillCastNinjaSpell>();
		case NJ_BAKUENRYU:
			return std::make_unique<SkillRagingFireDragon>();
		case NJ_BUNSINJYUTSU:
			return std::make_unique<SkillMirrorImage>();
		case NJ_HUUJIN:
			return std::make_unique<SkillWindBlade>();
		case NJ_HUUMA:
			return std::make_unique<SkillThrowHuumaShuriken>();
		case NJ_HYOUSENSOU:
			return std::make_unique<SkillSpearOfIce>();
		case NJ_HYOUSYOURAKU:
			return std::make_unique<SkillIceMeteor>();
		case NJ_ISSEN:
			return std::make_unique<SkillFinalStrike>();
		case NJ_KAENSIN:
			return std::make_unique<SkillCrimsonFireFormation>();
		case NJ_KAMAITACHI:
			return std::make_unique<SkillKamaitachi>();
		case NJ_KASUMIKIRI:
			return std::make_unique<SkillVanishingSlash>();
		case NJ_KIRIKAGE:
			return std::make_unique<SkillShadowSlash>();
		case NJ_KOUENKA:
			return std::make_unique<SkillCrimsonFirePetal>();
		case NJ_KUNAI:
			return std::make_unique<SkillThrowKunai>();
		case NJ_NEN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NJ_RAIGEKISAI:
			return std::make_unique<SkillLightningStrikeOfDestruction>();
		case NJ_SHADOWJUMP:
			return std::make_unique<SkillShadowLeap>();
		case NJ_SUITON:
			return std::make_unique<SkillHiddenWater>();
		case NJ_SYURIKEN:
			return std::make_unique<SkillThrowShuriken>();
		case NJ_TATAMIGAESHI:
			return std::make_unique<SkillImprovisedDefense>();
		case NJ_UTSUSEMI:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NJ_ZENYNAGE:
			return std::make_unique<SkillThrowZeny>();
		case OB_AKAITSUKI:
			return std::make_unique<SkillOminousMoonlight>();
		case OB_OBOROGENSOU:
			return std::make_unique<SkillMoonlightFantasy>();
		case OB_ZANGETSU:
			return std::make_unique<SkillDistortedCrescent>();
		case SS_AKUMUKESU:
			return std::make_unique<SkillNightmareErasion>();
		case SS_ANKOKURYUUAKUMU:
			return std::make_unique<SkillDarkDragonNightmare>();
		case SS_ANTENPOU:
			return std::make_unique<SkillDarkeningCannon>();
		case SS_FOUR_CHARM:
			return std::make_unique<SkillFourColorsCharm>();
		case SS_FUUMAKOUCHIKU:
			return std::make_unique<SkillHuumaShurikenConstruct>();
		case SS_FUUMASHOUAKU:
			return std::make_unique<SkillHuumaShurikenGrasp>();
		case SS_HITOUAKUMU:
			return std::make_unique<SkillKunaiNightmare>();
		case SS_KAGEAKUMU:
			return std::make_unique<SkillShadowNightmare>();
		case SS_KAGEGARI:
			return std::make_unique<SkillShadowHunting>();
		case SS_KAGEGISSEN:
			return std::make_unique<SkillShadowFlash>();
		case SS_KAGENOMAI:
			return std::make_unique<SkillShadowDance>();
		case SS_KINRYUUHOU:
			return std::make_unique<SkillGoldenDragonCannon>();
		case SS_KUNAIKAITEN:
			return std::make_unique<SkillKunaiRotation>();
		case SS_KUNAIKUSSETSU:
			return std::make_unique<SkillKunaiRefraction>();
		case SS_KUNAIWAIKYOKU:
			return std::make_unique<SkillKunaiDistortion>();
		case SS_RAIDENPOU:
			return std::make_unique<SkillThunderingCannon>();
		case SS_REIKETSUHOU:
			return std::make_unique<SkillColdBloodedCannon>();
		case SS_SEKIENHOU:
			return std::make_unique<SkillRedFlameCannon>();
		case SS_SHIMIRU:
			return std::make_unique<SkillInfiltrate>();
		case SS_SHINKIROU:
			return std::make_unique<SkillMirage>();
		case SS_TOKEDASU:
			return std::make_unique<SkillMeltAway>();

		default:
			return nullptr;
	}
}

#endif
