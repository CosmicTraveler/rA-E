// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_novice.hpp"

#include <config/core.hpp>
#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include "map/map.hpp"
#include "map/party.hpp"
#include "map/mob.hpp"
#include "skill_impl.hpp"

SkillDoubleBowlingBash::SkillDoubleBowlingBash() : SkillImpl(HN_DOUBLEBOWLINGBASH) {
}

void SkillDoubleBowlingBash::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	if (dmg.miscflag > 1) {
		dmg.div_ += min(4, dmg.miscflag);
	}
}

void SkillDoubleBowlingBash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 250 + 400 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillDoubleBowlingBash::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, skill_area_temp[0] & 0xFFF);
	} else {
		int32 splash = skill_get_splash(getSkillId(), skill_lv);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		skill_area_temp[0] = map_foreachinallrange(skill_area_sub, target, splash, BL_CHAR, src, getSkillId(), skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
		map_foreachinrange(skill_area_sub, target, splash, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		sc_start(src, src, SC_HNNOWEAPON, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	}
}

SkillFirstAid::SkillFirstAid() : SkillImpl(NV_FIRSTAID) {
}

void SkillFirstAid::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), 5);
	status_heal(target, 5, 0, 0);
}

SkillGroundGravitation::SkillGroundGravitation() : SkillImpl(HN_GROUND_GRAVITATION) {
}

void SkillGroundGravitation::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, 0, skill_get_time2(getSkillId(), skill_lv));
}

void SkillGroundGravitation::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	if (mflag & SKILL_ALTDMG_FLAG) {
		// Initial damage
		skillratio += -100 + 3000 + 1500 * skill_lv;
		skillratio += pc_checkskill(sd, HN_SELFSTUDY_SOCERY) * 4 * skill_lv;
		skillratio += 5 * sstatus->spl;
	} else {
		// Gravitational field damage
		skillratio += -100 + 800 + 700 * skill_lv;
		skillratio += pc_checkskill(sd, HN_SELFSTUDY_SOCERY) * 2 * skill_lv;
		skillratio += 2 * sstatus->spl;
	}
	RE_LVL_DMOD(100);
	// After RE_LVL_DMOD calculation, HN_SELFSTUDY_SOCERY amplifies the skill ratio of HN_GROUND_GRAVITATION (gravity field damage) by (skill level)%
	if (!(mflag & SKILL_ALTDMG_FLAG))
		skillratio += skillratio * pc_checkskill(sd, HN_SELFSTUDY_SOCERY) / 100;
	// SC_RULEBREAK increases the skill ratio after HN_SELFSTUDY_SOCERY
	if (sc && sc->getSCE(SC_RULEBREAK))
		skillratio += skillratio * 50 / 100;
}

void SkillGroundGravitation::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillGroundGravitation::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( map_getcell(src->m, x, y, CELL_CHKLANDPROTECTOR) ) {
		if( sd != nullptr ){
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		}

		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	int32 splash = skill_get_splash(getSkillId(), skill_lv);

	map_foreachinarea(skill_area_sub, src->m, x - splash, y - splash, x + splash, y + splash, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | SKILL_ALTDMG_FLAG | 1, skill_castend_damage_id);
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, flag);

	for (int32 i = 1; i <= (skill_get_time(getSkillId(), skill_lv) / skill_get_unit_interval(getSkillId())); i++) {
		skill_addtimerskill(src, tick + (t_tick)i*skill_get_unit_interval(getSkillId()), 0, x, y, getSkillId(), skill_lv, 0, flag);
	}
}

void SkillGroundGravitation::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	if (dmg.miscflag & SKILL_ALTDMG_FLAG) {
		// Initial damage
		dmg.div_ = -2;
	}
}

SkillHellsDrive::SkillHellsDrive() : SkillImpl(HN_HELLS_DRIVE) {
}

void SkillHellsDrive::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 1700 + 900 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_SOCERY) * 4 * skill_lv;
	skillratio += 3 * sstatus->spl;
	RE_LVL_DMOD(100);
	// After RE_LVL_DMOD calculation, HN_SELFSTUDY_SOCERY amplifies the skill ratio of HN_HELLS_DRIVE by (skill level)%
	skillratio += skillratio * pc_checkskill(sd, HN_SELFSTUDY_SOCERY) / 100;
	// SC_RULEBREAK increases the skill ratio after HN_SELFSTUDY_SOCERY
	if (sc && sc->getSCE(SC_RULEBREAK))
		skillratio += skillratio * 70 / 100;
}

void SkillHellsDrive::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillHellsDrive::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
}

SkillHelpAngel::SkillHelpAngel() : StatusSkillImpl(NV_HELPANGEL) {
}

void SkillHelpAngel::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) )
		StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillJackFrostNova::SkillJackFrostNova() : SkillImpl(HN_JACK_FROST_NOVA) {
}

void SkillJackFrostNova::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, 0, skill_get_time2(getSkillId(), skill_lv));
}

void SkillJackFrostNova::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	if (mflag & SKILL_ALTDMG_FLAG) {
		// Initial damage
		skillratio += -100 + 200 * skill_lv;
		skillratio += 2 * sstatus->spl;
	} else {
		// Explosion damage
		skillratio += -100 + 400 + 500 * skill_lv;
		skillratio += 4 * sstatus->spl;
	}
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_SOCERY) * 3 * skill_lv;
	RE_LVL_DMOD(100);
	// After RE_LVL_DMOD calculation, HN_SELFSTUDY_SOCERY amplifies the skill ratio of HN_JACK_FROST_NOVA (explosion damage) by (skill level)%
	if (!(mflag & SKILL_ALTDMG_FLAG))
		skillratio += skillratio * pc_checkskill(sd, HN_SELFSTUDY_SOCERY) / 100;
	// SC_RULEBREAK increases the skill ratio after HN_SELFSTUDY_SOCERY
	if (sc && sc->getSCE(SC_RULEBREAK))
		skillratio += skillratio * 70 / 100;
}

void SkillJackFrostNova::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillJackFrostNova::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( map_getcell(src->m, x, y, CELL_CHKLANDPROTECTOR) ) {
		if( sd != nullptr ){
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		}

		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	int32 splash = skill_get_splash(getSkillId(), skill_lv);

	map_foreachinarea(skill_area_sub, src->m, x - splash, y - splash, x + splash, y + splash, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | SKILL_ALTDMG_FLAG | 1, skill_castend_damage_id);
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, flag);

	for (int32 i = 1; i <= (skill_get_time(getSkillId(), skill_lv) / skill_get_unit_interval(getSkillId())); i++) {
		skill_addtimerskill(src, tick + (t_tick)i*skill_get_unit_interval(getSkillId()), 0, x, y, getSkillId(), skill_lv, 0, flag);
	}
}

void SkillJackFrostNova::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	if (dmg.miscflag & SKILL_ALTDMG_FLAG) {
		// Initial damage
		dmg.div_ = 1;
	}
}

SkillJupitelThunderstorm::SkillJupitelThunderstorm() : SkillImplRecursiveDamageSplash(HN_JUPITEL_THUNDER_STORM) {
}

void SkillJupitelThunderstorm::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 1800 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_SOCERY) * 3 * skill_lv;
	skillratio += 3 * sstatus->spl;
	RE_LVL_DMOD(100);
	// After RE_LVL_DMOD calculation, HN_SELFSTUDY_SOCERY amplifies the skill ratio of HN_JUPITEL_THUNDER_STORM by (skill level)%
	skillratio += skillratio * pc_checkskill(sd, HN_SELFSTUDY_SOCERY) / 100;
	// SC_RULEBREAK increases the skill ratio after HN_SELFSTUDY_SOCERY
	if (sc && sc->getSCE(SC_RULEBREAK))
		skillratio += skillratio * 70 / 100;
}

void SkillJupitelThunderstorm::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillMegaSonicBlow::SkillMegaSonicBlow() : WeaponSkillImpl(HN_MEGA_SONIC_BLOW) {
}

void SkillMegaSonicBlow::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_STUN, (2 * skill_lv + 10), skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillMegaSonicBlow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 900 + 750 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (status_get_hp(target) < status_get_max_hp(target) / 2)
		skillratio *= 2;
	RE_LVL_DMOD(100);
}

void SkillMegaSonicBlow::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillMeteorStormBuster::SkillMeteorStormBuster() : SkillImpl(HN_METEOR_STORM_BUSTER) {
}

void SkillMeteorStormBuster::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_STUN,3*skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillMeteorStormBuster::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	if (mflag & SKILL_ALTDMG_FLAG) {
		// Fall damage
		skillratio += -100 + 300 + 320 * skill_lv;
	} else {
		// Explosion damage
		skillratio += -100 + 450 + 160 * skill_lv;
	}
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_SOCERY) * 5 * skill_lv;
	skillratio += 3 * sstatus->spl;
	RE_LVL_DMOD(100);
	// After RE_LVL_DMOD calculation, HN_SELFSTUDY_SOCERY amplifies the skill ratio of HN_METEOR_STORM_BUSTER (fall damage) by (skill level)%
	if (mflag & SKILL_ALTDMG_FLAG)
		skillratio += skillratio * pc_checkskill(sd, HN_SELFSTUDY_SOCERY) / 100;
	// SC_RULEBREAK increases the skill ratio after HN_SELFSTUDY_SOCERY
	if (sc && sc->getSCE(SC_RULEBREAK))
		skillratio += skillratio * 50 / 100;
}

void SkillMeteorStormBuster::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillMeteorStormBuster::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( map_getcell(src->m, x, y, CELL_CHKLANDPROTECTOR) ) {
		if( sd != nullptr ){
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		}

		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	int32 splash = skill_get_splash(getSkillId(), skill_lv);

	map_foreachinarea(skill_area_sub, src->m, x - splash, y - splash, x + splash, y + splash, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | SKILL_ALTDMG_FLAG | 1, skill_castend_damage_id);
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, skill_get_unit_interval(getSkillId()));

	for (int32 i = 1; i <= (skill_get_time(getSkillId(), skill_lv) / skill_get_time2(getSkillId(), skill_lv)); i++) {
		skill_addtimerskill(src, tick + (t_tick)i*skill_get_time2(getSkillId(), skill_lv), 0, x, y, getSkillId(), skill_lv, 0, flag);
	}
}

void SkillMeteorStormBuster::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	if (dmg.miscflag & SKILL_ALTDMG_FLAG) {
		// Fall damage
		dmg.div_ = -3;
	}
}

SkillNapalmVulcanStrike::SkillNapalmVulcanStrike() : SkillImpl(HN_NAPALM_VULCAN_STRIKE) {
}

void SkillNapalmVulcanStrike::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_CURSE,5*skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

void SkillNapalmVulcanStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 350 + 650 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_SOCERY) * 4 * skill_lv;
	skillratio += 3 * sstatus->spl;
	RE_LVL_DMOD(100);
	// After RE_LVL_DMOD calculation, HN_SELFSTUDY_SOCERY amplifies the skill ratio of HN_NAPALM_VULCAN_STRIKE by (2x skill level)%
	skillratio += skillratio * 2 * pc_checkskill(sd, HN_SELFSTUDY_SOCERY) / 100;
	// SC_RULEBREAK increases the skill ratio after HN_SELFSTUDY_SOCERY
	if (sc && sc->getSCE(SC_RULEBREAK))
		skillratio += skillratio * 40 / 100;
}

void SkillNapalmVulcanStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
}

SkillOvercomingCrisis::SkillOvercomingCrisis() : StatusSkillImpl(HN_OVERCOMING_CRISIS) {
}

void SkillOvercomingCrisis::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
	status_percent_heal(target, 100, 0);
}

SkillShieldChainRush::SkillShieldChainRush() : WeaponSkillImpl(HN_SHIELD_CHAIN_RUSH) {
}

void SkillShieldChainRush::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, 0, skill_get_time2(getSkillId(), skill_lv));
}

void SkillShieldChainRush::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 600 + 1300 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;

	RE_LVL_DMOD(100);
}

// TODO : refactor to SkillImplRecursiveDamageSplash
void SkillShieldChainRush::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1) {
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		sc_start(src, src, SC_HNNOWEAPON, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	}
}

SkillSpiralPierceMax::SkillSpiralPierceMax() : WeaponSkillImpl(HN_SPIRAL_PIERCE_MAX) {
}

void SkillSpiralPierceMax::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 1000 + 1500 * skill_lv;
	skillratio += pc_checkskill(sd, HN_SELFSTUDY_TATICS) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;
	switch (status_get_size(target)){
		case SZ_SMALL:
			skillratio = skillratio * 150 / 100;
			break;
		case SZ_MEDIUM:
			skillratio = skillratio * 130 / 100;
			break;
		case SZ_BIG:
			skillratio = skillratio * 120 / 100;
			break;
	}
	RE_LVL_DMOD(100);
}

void SkillSpiralPierceMax::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

std::unique_ptr<const SkillImpl> SkillFactoryNovice::create(const e_skill skill_id) const {
	switch( skill_id ){
		case HN_BREAKINGLIMIT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case HN_DOUBLEBOWLINGBASH:
			return std::make_unique<SkillDoubleBowlingBash>();
		case HN_GROUND_GRAVITATION:
			return std::make_unique<SkillGroundGravitation>();
		case HN_HELLS_DRIVE:
			return std::make_unique<SkillHellsDrive>();
		case HN_JACK_FROST_NOVA:
			return std::make_unique<SkillJackFrostNova>();
		case HN_JUPITEL_THUNDER_STORM:
			return std::make_unique<SkillJupitelThunderstorm>();
		case HN_MEGA_SONIC_BLOW:
			return std::make_unique<SkillMegaSonicBlow>();
		case HN_METEOR_STORM_BUSTER:
			return std::make_unique<SkillMeteorStormBuster>();
		case HN_NAPALM_VULCAN_STRIKE:
			return std::make_unique<SkillNapalmVulcanStrike>();
		case HN_OVERCOMING_CRISIS:
			return std::make_unique<SkillOvercomingCrisis>();
		case HN_RULEBREAK:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case HN_SHIELD_CHAIN_RUSH:
			return std::make_unique<SkillShieldChainRush>();
		case HN_SPIRAL_PIERCE_MAX:
			return std::make_unique<SkillSpiralPierceMax>();
		case NV_FIRSTAID:
			return std::make_unique<SkillFirstAid>();
		case NV_HELPANGEL:
			return std::make_unique<SkillHelpAngel>();
		case NV_TRICKDEAD:
			return std::make_unique<StatusSkillImpl>(skill_id, true);

		default:
			return nullptr;
	}
	return nullptr;
}

#endif
