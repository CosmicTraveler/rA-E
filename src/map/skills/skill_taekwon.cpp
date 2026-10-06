// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_taekwon.hpp"

#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include "map/unit.hpp"
#include <config/core.hpp>
#include "map/battle.hpp"
#include "map/mob.hpp"
#include "map/party.hpp"
#include <common/ers.hpp>
#include "map/path.hpp"
#include "map/map.hpp"
#include <common/random.hpp>
#include "skill_impl.hpp"

SkillAllInTheSky::SkillAllInTheSky() : SkillImpl(SKE_ALL_IN_THE_SKY) {
}

void SkillAllInTheSky::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (target->type == BL_PC)
		status_zap(target, 0, 0, status_get_ap(target));
	if( unit_movepos( src, target->x, target->y, 2, true ) ){
		clif_snap(src, src->x, src->y);
	}
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillAllInTheSky::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 250 + 1200 * skill_lv;
	base_skillratio += 5 * sstatus->pow;
}

void SkillAllInTheSky::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	switch (status_get_race(&target)) {
		case RC_DEMIHUMAN:
		case RC_DEMON:
			dmg.div_ = 3;
			break;
	}
}

SkillBookofCreatingStar::SkillBookofCreatingStar() : SkillImpl(SJ_BOOKOFCREATINGSTAR) {
}

void SkillBookofCreatingStar::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillCircleOfDirectionsAndElementals::SkillCircleOfDirectionsAndElementals() : SkillImplRecursiveDamageSplash(SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS) {
}

void SkillCircleOfDirectionsAndElementals::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 500 + 2000 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_TALISMAN_MASTERY) * 15 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_SOUL_MASTERY) * 15 * skill_lv;
	skillratio += 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillCircleOfDirectionsAndElementals::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_area_temp[0] = map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
	sc_start(src,src,skill_get_sc(getSkillId()),100,skill_lv,skill_get_time(getSkillId(),skill_lv));

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillCounter::SkillCounter() : WeaponSkillImpl(TK_COUNTER) {
}

void SkillCounter::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 90 + 30 * skill_lv;
}

SkillCurseExplosion::SkillCurseExplosion() : SkillImplRecursiveDamageSplash(SP_CURSEEXPLOSION) {
}

void SkillCurseExplosion::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *tsc = status_get_sc(target);

	if (tsc && tsc->getSCE(SC_SOULCURSE))
		skillratio += -100 + 1200 + 300 * skill_lv;
	else
		skillratio += -100 + 400 + 100 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillDawnBreak::SkillDawnBreak() : SkillImplRecursiveDamageSplash(SKE_DAWN_BREAK) {
}

void SkillDawnBreak::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillDawnBreak::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 750 + 850 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;

	if (sc != nullptr && (sc->getSCE(SC_DAWN_MOON) != nullptr || sc->getSCE(SC_SKY_ENCHANT) != nullptr)) {
		skillratio += 200 + 200 * skill_lv;
	}

	RE_LVL_DMOD(100);
}

SkillDocumentofSunMoonAndStar::SkillDocumentofSunMoonAndStar() : SkillImpl(SJ_DOCUMENT) {
}

void SkillDocumentofSunMoonAndStar::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		switch (skill_lv) {
			case 1:
				pc_resetfeel(sd);
				break;
			case 2:
				pc_resethate(sd);
				break;
			case 3:
				pc_resetfeel(sd);
				pc_resethate(sd);
				break;
		}
	}
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillDownKick::SkillDownKick() : WeaponSkillImpl(TK_DOWNKICK) {
}

void SkillDownKick::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 60 + 20 * skill_lv;
}

void SkillDownKick::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_STUN, 3333, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

SkillEsha::SkillEsha() : SkillImplRecursiveDamageSplash(SP_SHA) {
}

void SkillEsha::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_SP_SHA, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillEsha::applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const {
	sc_start(src, src, SC_USE_SKILL_SP_SHA, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillEsha::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 5 * skill_lv;
}

int64 SkillEsha::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	// If a enemy player is standing next to a mob when splash Es- skill is casted, the player won't get hurt.
	if (!battle_config.allow_es_magic_pc && target->type != BL_MOB)
		return 0;

	return SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag);
}

void SkillEsha::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd && !battle_config.allow_es_magic_pc && target->type != BL_MOB) {
		status_change_start(src, target, SC_STUN, 10000, skill_lv, 0, 0, 0, 500, 10);
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		return;
	}

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillEska::SkillEska() : StatusSkillImpl(SL_SKA) {
}

void SkillEska::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc && type != SC_NONE)?tsc->getSCE(type):nullptr;
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (tsce) {
		if(sd)
			clif_skill_fail( *sd, getSkillId() );
		status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,10000,SCSTART_NORATEDEF);
		status_change_end(target, SC_SWOO);
		return;
	}
	if (sd && !battle_config.allow_es_magic_pc && target->type != BL_MOB) {
		clif_skill_fail( *sd, getSkillId() );
		status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
		return;
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillEske::SkillEske() : StatusSkillImpl(SL_SKE) {
}

void SkillEske::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd && !battle_config.allow_es_magic_pc && target->type != BL_MOB) {
		clif_skill_fail( *sd, getSkillId() );
		status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
		return;
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);

	sc_start(src,src,SC_SMA,100,skill_lv,skill_get_time(SL_SMA,skill_lv));
}

SkillEsma::SkillEsma() : SkillImpl(SL_SMA) {
}

void SkillEsma::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	// Base damage is 40% + lv%
	base_skillratio += -60 + status_get_lv(src);
}

void SkillEsma::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	status_change_end(src, SC_SMA);
	if (sd && !battle_config.allow_es_magic_pc && target->type != BL_MOB) {
		status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
		clif_skill_fail( *sd, getSkillId() );
		return;
	}
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillEspa::SkillEspa() : SkillImpl(SP_SPA) {
}

void SkillEspa::applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const {
	sc_start(src, src, SC_USE_SKILL_SP_SPA, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillEspa::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += 400 + 250 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillEspa::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd && !battle_config.allow_es_magic_pc && target->type != BL_MOB) {
		status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
		clif_skill_fail( *sd, getSkillId() );
		return;
	}
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillEstin::SkillEstin() : SkillImpl(SL_STIN) {
}

void SkillEstin::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_data* tstatus = status_get_status_data(*target);

	// Target size must be small (0) for full damage
	base_skillratio += (tstatus->size != SZ_SMALL ? -99 : 10 * skill_lv);
}

void SkillEstin::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd && !battle_config.allow_es_magic_pc && target->type != BL_MOB) {
		status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
		clif_skill_fail( *sd, getSkillId() );
		return;
	}
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillEstun::SkillEstun() : SkillImpl(SL_STUN) {
}

void SkillEstun::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 5 * skill_lv;
}

void SkillEstun::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd && !battle_config.allow_es_magic_pc && target->type != BL_MOB) {
		status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
		clif_skill_fail( *sd, getSkillId() );
		return;
	}
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillEstun::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data* tstatus = status_get_status_data(*target);

	if (tstatus->size==SZ_MEDIUM) //Only stuns mid-sized mobs.
		sc_start(src,target,SC_STUN,(30+10*skill_lv),skill_lv,skill_get_time(getSkillId(),skill_lv));
}

SkillEswhoo::SkillEswhoo() : SkillImplRecursiveDamageSplash(SP_SWHOO) {
}

void SkillEswhoo::applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const {
	sc_start(src, src, SC_USE_SKILL_SP_SHA, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillEswhoo::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += 1000 + 200 * skill_lv;
	RE_LVL_DMOD(100);
}

int64 SkillEswhoo::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	// If a enemy player is standing next to a mob when splash Es- skill is casted, the player won't get hurt.
	if (!battle_config.allow_es_magic_pc && target->type != BL_MOB)
		return 0;

	return SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag);
}

void SkillEswhoo::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd && !battle_config.allow_es_magic_pc && target->type != BL_MOB) {
		status_change_start(src, target, SC_STUN, 10000, skill_lv, 0, 0, 0, 500, 10);
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		return;
	}
	status_change_end(src, SC_USE_SKILL_SP_SPA);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillEswoo::SkillEswoo() : StatusSkillImpl(SL_SWOO) {
}

void SkillEswoo::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc && type != SC_NONE)?tsc->getSCE(type):nullptr;
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (tsce) {
		if(sd)
			clif_skill_fail( *sd, getSkillId() );
		status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,10000,SCSTART_NORATEDEF);
		status_change_end(target, SC_SWOO);
		return;
	}
	if (sd && !battle_config.allow_es_magic_pc && target->type != BL_MOB) {
		clif_skill_fail( *sd, getSkillId() );
		status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NOTICKDEF|SCSTART_NORATEDEF);
		return;
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillExorcismOfMaliciousSoul::SkillExorcismOfMaliciousSoul() : SkillImplRecursiveDamageSplash(SOA_EXORCISM_OF_MALICIOUS_SOUL) {
}

void SkillExorcismOfMaliciousSoul::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const status_change *tsc = status_get_sc(target);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 150 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_SOUL_MASTERY) * 2;
	skillratio += 1 * sstatus->spl;

	if ((tsc != nullptr && tsc->getSCE(SC_SOULCURSE) != nullptr) || (sc != nullptr && sc->getSCE(SC_TOTEM_OF_TUTELARY) != nullptr))
		skillratio += 100 * skill_lv;

	if (sd != nullptr)
		skillratio *= sd->soulball_old;
	RE_LVL_DMOD(100);
}

void SkillExorcismOfMaliciousSoul::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd != nullptr ){
		// Remove old souls if any exist.
		sd->soulball_old = sd->soulball;
		pc_delsoulball( *sd, sd->soulball, 0 );
	}

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillFairysSoul::SkillFairysSoul() : SkillImpl(SP_SOULFAIRY) {
}

void SkillFairysSoul::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( sc_start( src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}else{
		map_session_data* sd = BL_CAST( BL_PC, src );

		if( sd ){
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		}
	}
}

SkillFalconsSoul::SkillFalconsSoul() : SkillImpl(SP_SOULFALCON) {
}

void SkillFalconsSoul::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( sc_start( src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}else{
		map_session_data* sd = BL_CAST( BL_PC, src );

		if( sd ){
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		}
	}
}

// SJ_FALLINGSTAR
SkillFallingStar::SkillFallingStar() : StatusSkillImpl(SJ_FALLINGSTAR) {
}


// SJ_FALLINGSTAR_ATK
SkillFallingStarAttack::SkillFallingStarAttack() : SkillImplRecursiveDamageSplash(SJ_FALLINGSTAR_ATK) {
}

void SkillFallingStarAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	skillratio += 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFSTAR))
		skillratio += skillratio * sc->getSCE(SC_LIGHTOFSTAR)->val2 / 100;
}

int64 SkillFallingStarAttack::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const{
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST(BL_PC, src);

	// TODO: refactor logic
	if (sd) { // If a player used the skill it will search for targets marked by that player. 
		if (tsc && tsc->getSCE(SC_FLASHKICK) && tsc->getSCE(SC_FLASHKICK)->val4 == 1) { // Mark placed by a player.
			int8 i = 0;

			ARR_FIND(0, MAX_STELLAR_MARKS, i, sd->stellar_mark[i] == target->id);
			if (i < MAX_STELLAR_MARKS) {
				int64 dmg = SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag);

				skill_castend_damage_id(src, target, SJ_FALLINGSTAR_ATK2, skill_lv, tick, 0);

				return dmg;
			}
		}
	} else if ( tsc && tsc->getSCE(SC_FLASHKICK) && tsc->getSCE(SC_FLASHKICK)->val4 == 2 ) { // Mark placed by a monster.
		// If a monster used the skill it will search for targets marked by any monster since they can't track their own targets.
		int64 dmg = SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag);

		skill_castend_damage_id(src, target, SJ_FALLINGSTAR_ATK2, skill_lv, tick, 0);

		return dmg;
	}

	return 0;
}

void SkillFallingStarAttack::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}


// SJ_FALLINGSTAR_ATK2
SkillFallingStarAttack2::SkillFallingStarAttack2() : SkillImplRecursiveDamageSplash(SJ_FALLINGSTAR_ATK2) {
}

void SkillFallingStarAttack2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	skillratio += 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFSTAR))
		skillratio += skillratio * sc->getSCE(SC_LIGHTOFSTAR)->val2 / 100;
}

SkillFeelingtheSunMoonandStars::SkillFeelingtheSunMoonandStars() : SkillImpl(SG_FEEL) {
}

void SkillFeelingtheSunMoonandStars::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	//AuronX reported you CAN memorize the same map as all three. [Skotlex]
	if (sd) {
		if(!sd->feel_map[skill_lv-1].index)
			clif_feel_req(sd->fd,sd, skill_lv);
		else
			clif_feel_info(sd, skill_lv-1, 1);
	}
}

SkillFlashKick::SkillFlashKick() : SkillImpl(SJ_FLASHKICK) {
}

void SkillFlashKick::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);
	mob_data* tmd = BL_CAST(BL_MOB, target);
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* tsd = BL_CAST(BL_PC, target);

	// Only players and monsters can be tagged....I think??? [Rytech]
	// Lets only allow players and monsters to use this skill for safety reasons.
	if ((!tsd && !tmd) || !sd && !md) {
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		return;
	}

	// Check if the target is already tagged by another source.
	if ((tsd && tsd->sc.getSCE(SC_FLASHKICK) && tsd->sc.getSCE(SC_FLASHKICK)->val1 != src->id) || (tmd && tmd->sc.getSCE(SC_FLASHKICK) && tmd->sc.getSCE(SC_FLASHKICK)->val1 != src->id)) { // Same as the above check, but for monsters.
		// Can't tag a player that was already tagged from another source.
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	if (sd) { // Tagging the target.
		int32 i;

		ARR_FIND(0, MAX_STELLAR_MARKS, i, sd->stellar_mark[i] == target->id);
		if (i == MAX_STELLAR_MARKS) {
			ARR_FIND(0, MAX_STELLAR_MARKS, i, sd->stellar_mark[i] == 0);
			if (i == MAX_STELLAR_MARKS) { // Max number of targets tagged. Fail the skill.
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
				flag |= SKILL_NOCONSUME_REQ;
				return;
			}
		}

		// Tag the target only if damage was done. If it deals no damage, it counts as a miss and won't tag.
		// Note: Not sure if it works like this in official but you can't mark on something you can't
		// hit, right? For now well just use this logic until we can get a confirm on if it does this or not. [Rytech]
		if (skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag) > 0) { // Add the ID of the tagged target to the player's tag list and start the status on the target.
			sd->stellar_mark[i] = target->id;

			// Val4 flags if the status was applied by a player or a monster.
			// This will be important for other skills that work together with this one.
			// 1 = Player, 2 = Monster.
			// Note: Because the attacker's ID and the slot number is handled here, we have to
			// apply the status here. We can't pass this data to skill_additional_effect.
			sc_start4(src, target, SC_FLASHKICK, 100, src->id, i, skill_lv, 1, skill_get_time(getSkillId(), skill_lv));
		}
	} else if (md) { // Monsters can't track with this skill. Just give the status.
		if (skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag) > 0)
			sc_start4(src, target, SC_FLASHKICK, 100, 0, 0, skill_lv, 2, skill_get_time(getSkillId(), skill_lv));
	}
}

SkillFullMoonKick::SkillFullMoonKick() : SkillImplRecursiveDamageSplash(SJ_FULLMOONKICK) {
}

void SkillFullMoonKick::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_BLIND, 15 + 5 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillFullMoonKick::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	skillratio += 1000 + 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFMOON))
		skillratio += skillratio * sc->getSCE(SC_LIGHTOFMOON)->val2 / 100;
}

void SkillFullMoonKick::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillGolemsSoul::SkillGolemsSoul() : SkillImpl(SP_SOULGOLEM) {
}

void SkillGolemsSoul::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( sc_start( src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}else{
		map_session_data* sd = BL_CAST( BL_PC, src );

		if( sd ){
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		}
	}
}

SkillGravityControl::SkillGravityControl() : SkillImpl(SJ_GRAVITYCONTROL) {
}

void SkillGravityControl::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* sstatus = status_get_status_data(*src);
	status_data* tstatus = status_get_status_data(*target);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	int32 fall_damage = sstatus->batk + sstatus->rhw.atk - tstatus->def2;

	if (target->type == BL_PC)
		fall_damage += dstsd->weight / 10 - tstatus->def;
	else // Monster's don't have weight. Put something in its place.
		fall_damage += 50 * status_get_lv(src) - tstatus->def;

	fall_damage = max(1, fall_damage);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, fall_damage, skill_get_time(getSkillId(), skill_lv)));
}

SkillHatredoftheSunMoonandStars::SkillHatredoftheSunMoonandStars() : SkillImpl(SG_HATE) {
}

void SkillHatredoftheSunMoonandStars::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd) {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		if (!pc_set_hate_mob(sd, skill_lv-1, target))
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillHighJump::SkillHighJump() : SkillImpl(TK_HIGHJUMP) {
}

void SkillHighJump::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	int32 x, y, dir = unit_getdir(src);
	map_data *mapdata = map_getmapdata(src->m);

	// Fails on noteleport maps, except for GvG and BG maps [Skotlex]
	if (mapdata->getMapFlag(MF_NOTELEPORT) && !(mapdata->getMapFlag(MF_BATTLEGROUND) || mapdata_flag_gvg(mapdata))) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		return;
	} else if (dir % 2) {
		// Diagonal
		x = src->x + dirx[dir] * (skill_lv * 4) / 3;
		y = src->y + diry[dir] * (skill_lv * 4) / 3;
	} else {
		x = src->x + dirx[dir] * skill_lv * 2;
		y = src->y + diry[dir] * skill_lv * 2;
	}

	int32 x1 = x + dirx[dir];
	int32 y1 = y + diry[dir];

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (!map_count_oncell(src->m, x, y, BL_PC | BL_NPC | BL_MOB, 0) && map_getcell(src->m, x, y, CELL_CHKREACH) &&
	    !map_count_oncell(src->m, x1, y1, BL_PC | BL_NPC | BL_MOB, 0) && map_getcell(src->m, x1, y1, CELL_CHKREACH) &&
	    unit_movepos(src, x, y, 1, 0))
		clif_blown(src);
}

SkillJumpKick::SkillJumpKick() : SkillImpl(TK_JUMPKICK) {
}

void SkillJumpKick::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);
	// Different damage formulas depending on damage trigger
	if (sc && sc->getSCE(SC_COMBO) && sc->getSCE(SC_COMBO)->val1 == getSkillId())
		base_skillratio += -100 + 4 * status_get_lv(src); // Tumble formula [4%*baselevel]
	else if (wd->miscflag) {
		base_skillratio += -100 + 4 * status_get_lv(src); // Running formula [4%*baselevel]
		if (sc && sc->getSCE(SC_SPURT)) // Spurt formula [8%*baselevel]
			base_skillratio *= 2;
	} else
		base_skillratio += -70 + 10 * skill_lv;
}

void SkillJumpKick::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change *tsc = status_get_sc(target);
	map_session_data *dstsd = BL_CAST(BL_PC, target);

	// debuff the following statuses
	if (dstsd && dstsd->class_ != MAPID_SOUL_LINKER && tsc != nullptr && !tsc->getSCE(SC_PRESERVE)) {
		status_change_end(target, SC_SPIRIT);
		status_change_end(target, SC_ADRENALINE2);
		status_change_end(target, SC_KAITE);
		status_change_end(target, SC_KAAHI);
		status_change_end(target, SC_ONEHAND);
		status_change_end(target, SC_ASPDPOTION2);
		// New soul links confirmed to not dispell with this skill
		// but thats likely a bug since soul links can't stack and
		// soul cutter skill works on them. So ill add this here for now. [Rytech]
		status_change_end(target, SC_SOULGOLEM);
		status_change_end(target, SC_SOULSHADOW);
		status_change_end(target, SC_SOULFALCON);
		status_change_end(target, SC_SOULFAIRY);
	}
}

void SkillJumpKick::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);

	/* Check if the target is an enemy; if not, skill should fail so the character doesn't unit_movepos (exploitable) */
	if (battle_check_target(src, target, BCT_ENEMY) > 0) {
		if (unit_movepos(src, target->x, target->y, 2, 1)) {
			skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
			clif_blown(src);
		}
	} else if (sd) {
		clif_skill_fail(*sd, getSkillId(), USESKILL_FAIL);
	}
}

SkillKaahi::SkillKaahi() : StatusSkillImpl(SL_KAAHI) {
}

void SkillKaahi::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data *dstsd = BL_CAST( BL_PC, target );

	if (sd) {
		if (!dstsd || !(
			(sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_SOULLINKER) ||
			(dstsd->class_&MAPID_SECONDMASK) == MAPID_SOUL_LINKER ||
			dstsd->status.char_id == sd->status.char_id ||
			dstsd->status.char_id == sd->status.partner_id ||
			dstsd->status.char_id == sd->status.child
		)) {
			status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NORATEDEF);
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillKaite::SkillKaite() : StatusSkillImpl(SL_KAITE) {
}

void SkillKaite::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data *dstsd = BL_CAST( BL_PC, target );

	if (sd) {
		if (!dstsd || !(
			(sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_SOULLINKER) ||
			(dstsd->class_&MAPID_SECONDMASK) == MAPID_SOUL_LINKER ||
			dstsd->status.char_id == sd->status.char_id ||
			dstsd->status.char_id == sd->status.partner_id ||
			dstsd->status.char_id == sd->status.child
		)) {
			status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NORATEDEF);
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillKaizel::SkillKaizel() : StatusSkillImpl(SL_KAIZEL) {
}

void SkillKaizel::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data *dstsd = BL_CAST( BL_PC, target );

	if (sd) {
		if (!dstsd || !(
			(sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_SOULLINKER) ||
			(dstsd->class_&MAPID_SECONDMASK) == MAPID_SOUL_LINKER ||
			dstsd->status.char_id == sd->status.char_id ||
			dstsd->status.char_id == sd->status.partner_id ||
			dstsd->status.char_id == sd->status.child
		)) {
			status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NORATEDEF);
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillKaupe::SkillKaupe() : StatusSkillImpl(SL_KAUPE) {
}

void SkillKaupe::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data *dstsd = BL_CAST( BL_PC, target );

	if (sd) {
		if (!dstsd || !(
			(sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_SOULLINKER) ||
			(dstsd->class_&MAPID_SECONDMASK) == MAPID_SOUL_LINKER ||
			dstsd->status.char_id == sd->status.char_id ||
			dstsd->status.char_id == sd->status.partner_id ||
			dstsd->status.char_id == sd->status.child
		)) {
			status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NORATEDEF);
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillKaute::SkillKaute() : SkillImpl(SP_KAUTE) {
}

void SkillKaute::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* sstatus = status_get_status_data(*src);
	status_data* tstatus = status_get_status_data(*target);
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );

	if (sd) {
		if (!dstsd || !(
			(sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_SOULLINKER) ||
			(dstsd->class_&MAPID_SECONDMASK) == MAPID_SOUL_LINKER ||
			dstsd->status.char_id == sd->status.char_id ||
			dstsd->status.char_id == sd->status.partner_id ||
			dstsd->status.char_id == sd->status.child ||
			(dstsd->sc.getSCE(SC_SOULUNITY))
		)) {
			status_change_start(src,src,SC_STUN,10000,skill_lv,0,0,0,500,SCSTART_NORATEDEF);
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
	}
	if (!status_charge(src, sstatus->max_hp * (10 + 2 * skill_lv) / 100, 0)) {
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		return;
	}
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	status_heal(target, 0, tstatus->max_sp * (10 + 2 * skill_lv) / 100, 2);
}

SkillMidnightKick::SkillMidnightKick() : SkillImplRecursiveDamageSplash(SKE_MIDNIGHT_KICK) {
}

void SkillMidnightKick::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillMidnightKick::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 850 + 1700 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;

	if (sc != nullptr && (sc->getSCE(SC_MIDNIGHT_MOON) != nullptr || sc->getSCE(SC_SKY_ENCHANT) != nullptr)) {
		skillratio += 950 + 300 * skill_lv;
	}

	RE_LVL_DMOD(100);
}

SkillMission::SkillMission() : SkillImpl(TK_MISSION) {
}

void SkillMission::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd) {
		if (sd->mission_mobid && (sd->mission_count || rnd() % 100)) {
			// Cannot change target when already have one
			clif_mission_info(sd, sd->mission_mobid, sd->mission_count);
			clif_skill_fail(*sd, getSkillId());
			return;
		}

		int32 id = mob_get_random_id(MOBG_TAEKWON_MISSION, RMF_NONE, 0);

		if (!id) {
			clif_skill_fail(*sd, getSkillId());
			return;
		}
		sd->mission_mobid = id;
		sd->mission_count = 0;
		pc_setglobalreg(sd, add_str(TKMISSIONID_VAR), id);
		clif_mission_info(sd, id, 0);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillNewMoonKick::SkillNewMoonKick() : SkillImplRecursiveDamageSplash(SJ_NEWMOONKICK) {
}

void SkillNewMoonKick::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 600 + 100 * skill_lv;
}

void SkillNewMoonKick::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if (tsce) {
		status_change_end(target, type);
		return;
	} else
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillNoonBlast::SkillNoonBlast() : SkillImplRecursiveDamageSplash(SKE_NOON_BLAST) {
}

void SkillNoonBlast::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1750 + 1550 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillNoonBlast::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillNovaExplosion::SkillNovaExplosion() : SkillImpl(SJ_NOVAEXPLOSING) {
}

void SkillNovaExplosion::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);

	skill_attack(BF_MISC, src, src, target, getSkillId(), skill_lv, tick, flag);

	// We can end Dimension here since the cooldown code is processed before this point.
	if (sc && sc->getSCE(SC_DIMENSION))
		status_change_end(src, SC_DIMENSION);
	else // Dimension not active? Activate the 2 second skill block penalty.
		sc_start(src, sd, SC_NOVAEXPLOSING, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillProminenceKick::SkillProminenceKick() : SkillImplRecursiveDamageSplash(SJ_PROMINENCEKICK) {
}

void SkillProminenceKick::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 50 + 50 * skill_lv;
}

int64 SkillProminenceKick::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	int64 dmg = SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag);

	// Trigger the 2nd hit. (100% fire damage.)
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag|8|SD_ANIMATION);

	return dmg;
}

void SkillProminenceKick::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	element = ELE_FIRE;
}

SkillRisingMoon::SkillRisingMoon() : SkillImplRecursiveDamageSplash(SKE_RISING_MOON) {
}

void SkillRisingMoon::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* sc = status_get_sc(src);

	if( sc == nullptr || ( sc->getSCE( SC_RISING_MOON ) == nullptr && sc->getSCE( SC_MIDNIGHT_MOON ) == nullptr && sc->getSCE( SC_DAWN_MOON ) == nullptr ) ){
		sc_start(src, src, SC_RISING_MOON, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}else if( sc->getSCE( SC_MIDNIGHT_MOON ) == nullptr && sc->getSCE( SC_DAWN_MOON ) == nullptr ){
		sc_start(src, src, SC_MIDNIGHT_MOON, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}else if( sc->getSCE( SC_DAWN_MOON ) == nullptr ){
		sc_start(src, src, SC_DAWN_MOON, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}else if( sc->getSCE( SC_RISING_SUN ) != nullptr ){
		status_change_end(target, SC_DAWN_MOON);
	}

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillRisingMoon::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 700 + 450 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

SkillRisingSun::SkillRisingSun() : SkillImpl(SKE_RISING_SUN) {
}

void SkillRisingSun::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* sc = status_get_sc(src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);

	if ( sc == nullptr || ( sc->getSCE( SC_RISING_SUN ) == nullptr && sc->getSCE( SC_NOON_SUN ) == nullptr && sc->getSCE( SC_SUNSET_SUN ) == nullptr ) ){
		sc_start(src, src, SC_RISING_SUN, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}else if( sc->getSCE( SC_NOON_SUN ) == nullptr && sc->getSCE( SC_SUNSET_SUN ) == nullptr ){
		sc_start(src, src, SC_NOON_SUN, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}else if( sc->getSCE( SC_SUNSET_SUN ) == nullptr ){
		sc_start(src, src, SC_SUNSET_SUN, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
}

void SkillRisingSun::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 500 + 600 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

SkillRun::SkillRun() : SkillImpl(TK_RUN) {
}

void SkillRun::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc && type != SC_NONE) ? tsc->getSCE(type) : nullptr;
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (tsce) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, status_change_end(target, type));
		return;
	}

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start4(src, target, type, 100, skill_lv, unit_getdir(target), 0, 0, 0));
	if (sd) // If the client receives a skill-use packet inmediately before a walkok packet, it will discard the walk packet! [Skotlex]
		clif_walkok(*sd); // So aegis has to resend the walk ok.
}

SkillSevenWind::SkillSevenWind() : SkillImpl(TK_SEVENWIND) {
}

void SkillSevenWind::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	sc_type type = SC_NONE;

	switch (skill_get_ele(getSkillId(), skill_lv)) {
		case ELE_EARTH:
			type = SC_EARTHWEAPON;
			break;
		case ELE_WIND:
			type = SC_WINDWEAPON;
			break;
		case ELE_WATER:
			type = SC_WATERWEAPON;
			break;
		case ELE_FIRE:
			type = SC_FIREWEAPON;
			break;
		case ELE_GHOST:
			type = SC_GHOSTWEAPON;
			break;
		case ELE_DARK:
			type = SC_SHADOWWEAPON;
			break;
		case ELE_HOLY:
			type = SC_ASPERSIO;
			break;
	}

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	sc_start(src, target, SC_SEVENWIND, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillShadowsSoul::SkillShadowsSoul() : SkillImpl(SP_SOULSHADOW) {
}

void SkillShadowsSoul::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( sc_start( src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}else{
		map_session_data* sd = BL_CAST( BL_PC, src );

		if( sd ){
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		}
	}
}

SkillSkyMoon::SkillSkyMoon() : SkillImplRecursiveDamageSplash(SKE_SKY_MOON) {
}

void SkillSkyMoon::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1250 + 500 * skill_lv;
	skillratio += skill_lv * 9 * pc_checkskill(sd, SKE_SKY_MASTERY);
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillSkyMoon::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillSkySun::SkillSkySun() : SkillImplRecursiveDamageSplash(SKE_SKY_SUN) {
}

void SkillSkySun::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillSkySun::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 250 + 1650 * skill_lv;
	skillratio += skill_lv * 7 * pc_checkskill(sd, SKE_SKY_MASTERY);
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillSkySun::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillSolarBurst::SkillSolarBurst() : SkillImplRecursiveDamageSplash(SJ_SOLARBURST) {
}

void SkillSolarBurst::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	skillratio += 900 + 220 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_LIGHTOFSUN))
		skillratio += skillratio * sc->getSCE(SC_LIGHTOFSUN)->val2 / 100;
}

void SkillSolarBurst::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillSoulCollect::SkillSoulCollect() : SkillImpl(SP_SOULCOLLECT) {
}

void SkillSoulCollect::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;

	if( tsce )
	{
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,status_change_end(target, type));
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start2(src, target, type, 100, skill_lv, pc_checkskill(sd, SP_SOULENERGY), skill_get_time(getSkillId(), skill_lv)));
}

SkillSoulCurse::SkillSoulCurse() : SkillImpl(SP_SOULCURSE) {
}

void SkillSoulCurse::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1)
		sc_start(src, target, skill_get_sc(getSkillId()), 30 + 10 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
	else {
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillSoulDivision::SkillSoulDivision() : StatusSkillImpl(SP_SOULDIVISION) {
}

void SkillSoulDivision::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (target->type != BL_PC) {
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		return;
	}

	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
}

SkillSoulExplosion::SkillSoulExplosion() : SkillImpl(SP_SOULEXPLOSION) {
}

void SkillSoulExplosion::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Remove soul link when hit.
	status_change_end(target, SC_SPIRIT);
	status_change_end(target, SC_SOULGOLEM);
	status_change_end(target, SC_SOULSHADOW);
	status_change_end(target, SC_SOULFALCON);
	status_change_end(target, SC_SOULFAIRY);
}

void SkillSoulExplosion::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (!(tsc && (tsc->getSCE(SC_SPIRIT) || tsc->getSCE(SC_SOULGOLEM) || tsc->getSCE(SC_SOULSHADOW) || tsc->getSCE(SC_SOULFALCON) || tsc->getSCE(SC_SOULFAIRY))) || tstatus->hp < 10 * tstatus->max_hp / 100) { // Requires target to have a soul link and more then 10% of MaxHP.
		// With this skill requiring a soul link, and the target to have more then 10% if MaxHP, I wonder
		// if the cooldown still happens after it fails. Need a confirm. [Rytech] 
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		return;
	}

	skill_attack(BF_MISC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillSoulGathering::SkillSoulGathering() : SkillImpl(SOA_SOUL_GATHERING) {
}

void SkillSoulGathering::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if( sd != nullptr ){
		int32 limit = 5 + pc_checkskill(sd, SP_SOULENERGY) * 3;

		for (int32 i = 0; i < limit; i++)
			pc_addsoulball(*sd,limit);
	}
}

SkillSoulOfHeavenAndEarth::SkillSoulOfHeavenAndEarth() : SkillImpl(SOA_SOUL_OF_HEAVEN_AND_EARTH) {
}

void SkillSoulOfHeavenAndEarth::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {

		// Animations don't play when outside visitargete range
		if (check_distance_bl(src, target, AREA_SIZE))
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv);

		status_percent_heal(target, 0, 100);

		if( src != target && sc != nullptr && sc->getSCE(SC_TOTEM_OF_TUTELARY) != nullptr ){
			status_heal(target, 0, 0, 3 * skill_lv, 0);
		}

		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
}

SkillSoulRevolution::SkillSoulRevolution() : SkillImpl(SP_SOULREVOLVE) {
}

void SkillSoulRevolution::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (!(tsc && (tsc->getSCE(SC_SPIRIT) || tsc->getSCE(SC_SOULGOLEM) || tsc->getSCE(SC_SOULSHADOW) || tsc->getSCE(SC_SOULFALCON) || tsc->getSCE(SC_SOULFAIRY)))) {
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		return;
	}
	status_heal(target, 0, 50*skill_lv, 2);
	status_change_end(target, SC_SPIRIT);
	status_change_end(target, SC_SOULGOLEM);
	status_change_end(target, SC_SOULSHADOW);
	status_change_end(target, SC_SOULFALCON);
	status_change_end(target, SC_SOULFAIRY);
}

SkillSoulUnity::SkillSoulUnity() : SkillImpl(SP_SOULUNITY) {
}

void SkillSoulUnity::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );
	int32 i = 0;

	int8 count = min(5 + skill_lv, MAX_UNITED_SOULS);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		if (!dstsd || !sd) { // Only put player's souls in unity.
			if (sd)
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
			return;
		}

		if (dstsd->sc.getSCE(type) && dstsd->sc.getSCE(type)->val2 != src->id) { // Fail if a player is in unity with another source.
			if (sd)
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		if (sd) { // Unite player's soul with caster's soul.
			i = 0;

			ARR_FIND(0, count, i, sd->united_soul[i] == target->id);
			if (i == count) {
				ARR_FIND(0, count, i, sd->united_soul[i] == 0);
				if(i == count) { // No more free slots? Fail the skill.
					clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
					flag |= SKILL_NOCONSUME_REQ;
					return;
				}
			}

			sd->united_soul[i] = target->id;
		}

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start4(src, target, type, 100, skill_lv, src->id, i, 0, skill_get_time(getSkillId(), skill_lv)));
	} else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillSpiritofRebirth::SkillSpiritofRebirth() : SkillImpl(SL_HIGH) {
}

void SkillSpiritofRebirth::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheAlchemist::SkillSpiritoftheAlchemist() : SkillImpl(SL_ALCHEMIST) {
}

void SkillSpiritoftheAlchemist::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheArtist::SkillSpiritoftheArtist() : SkillImpl(SL_BARDDANCER) {
}

void SkillSpiritoftheArtist::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheAssasin::SkillSpiritoftheAssasin() : SkillImpl(SL_ASSASIN) {
}

void SkillSpiritoftheAssasin::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheBlacksmith::SkillSpiritoftheBlacksmith() : SkillImpl(SL_BLACKSMITH) {
}

void SkillSpiritoftheBlacksmith::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheCrusader::SkillSpiritoftheCrusader() : SkillImpl(SL_CRUSADER) {
}

void SkillSpiritoftheCrusader::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheHunter::SkillSpiritoftheHunter() : SkillImpl(SL_HUNTER) {
}

void SkillSpiritoftheHunter::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheKnight::SkillSpiritoftheKnight() : SkillImpl(SL_KNIGHT) {
}

void SkillSpiritoftheKnight::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheMonk::SkillSpiritoftheMonk() : SkillImpl(SL_MONK) {
}

void SkillSpiritoftheMonk::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritofthePriest::SkillSpiritofthePriest() : SkillImpl(SL_PRIEST) {
}

void SkillSpiritofthePriest::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheRogue::SkillSpiritoftheRogue() : SkillImpl(SL_ROGUE) {
}

void SkillSpiritoftheRogue::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheSage::SkillSpiritoftheSage() : SkillImpl(SL_SAGE) {
}

void SkillSpiritoftheSage::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheSoulLinker::SkillSpiritoftheSoulLinker() : SkillImpl(SL_SOULLINKER) {
}

void SkillSpiritoftheSoulLinker::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheStarGladiator::SkillSpiritoftheStarGladiator() : SkillImpl(SL_STAR) {
}

void SkillSpiritoftheStarGladiator::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheSupernovice::SkillSpiritoftheSupernovice() : SkillImpl(SL_SUPERNOVICE) {
}

void SkillSpiritoftheSupernovice::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data *dstsd = BL_CAST( BL_PC, target );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		// 1% chance to erase death count on successful cast
		if( dstsd && dstsd->die_counter && rnd_chance( 1, 100 )  ){
			pc_setparam( dstsd, SP_PCDIECOUNTER, 0 );
			clif_specialeffect( target, EF_ANGEL2, AREA );
			status_calc_pc( dstsd, SCO_NONE );
		}

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillSpiritoftheWizard::SkillSpiritoftheWizard() : SkillImpl(SL_WIZARD) {
}

void SkillSpiritoftheWizard::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start2( src, target, type, 100, skill_lv, getSkillId(), skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		sc_start( src, src, SC_SMA, 100, skill_lv, skill_get_time( SL_SMA, skill_lv ) );
	}else{
		if( sd ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillStarBurst::SkillStarBurst() : SkillImpl(SKE_STAR_BURST) {
}

void SkillStarBurst::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	} else {
		unit_data* ud = unit_bl2ud( src );

		if( ud != nullptr ){
			for( const std::shared_ptr<s_skill_unit_group>& sug : ud->skillunits ){
				if( sug->skill_id != SKE_TWINKLING_GALAXY ){
					continue;
				}

				skill_unit* su = sug->unit;

				// Check if it is too far away
				if( distance_xy( target->x, target->y, su->x, su->y ) > skill_get_unit_range( sug->skill_id, sug->skill_lv ) ){
					continue;
				}

				std::shared_ptr<s_skill_unit_group> sg = su->group;

				for( int32 i = 0; i < MAX_SKILLTIMERSKILL; i++ ){
					if( ud->skilltimerskill[i] == nullptr ){
						continue;
					}

					if( ud->skilltimerskill[i]->skill_id != sug->skill_id ){
						continue;
					}

					delete_timer(ud->skilltimerskill[i]->timer, skill_timerskill);
					ers_free(skill_timer_ers, ud->skilltimerskill[i]);
					ud->skilltimerskill[i] = nullptr;
				}

				skill_delunitgroup(sg);
				sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv));

				skill_castend_pos2(src, target->x, target->y, getSkillId(), skill_lv, tick, 0);
				return;
			}
		}

		if( sd != nullptr ){
			clif_skill_fail(*sd, getSkillId(), USESKILL_FAIL_LEVEL);
		}

		flag |= SKILL_NOCONSUME_REQ;
	}
}

void SkillStarBurst::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= 1;
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillStarBurst::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 500 + 400 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

SkillStarCannon::SkillStarCannon() : SkillImpl(SKE_STAR_CANNON) {
}

void SkillStarCannon::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillStarCannon::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	unit_data* ud = unit_bl2ud( src );

	if( ud == nullptr ){
		return;
	}

	for( const std::shared_ptr<s_skill_unit_group>& sug : ud->skillunits ){
		if( sug->skill_id != SKE_TWINKLING_GALAXY ){
			continue;
		}

		skill_unit* su = sug->unit;

		if( distance_xy( x, y, su->x, su->y ) > skill_get_unit_range( sug->skill_id, sug->skill_lv ) ){
			continue;
		}

		std::shared_ptr<s_skill_unit_group> sg = su->group;

		for( int32 i = 0; i< MAX_SKILLTIMERSKILL; i++ ){
			if( ud->skilltimerskill[i] == nullptr ){
				continue;
			}

			if( ud->skilltimerskill[i]->skill_id != SKE_TWINKLING_GALAXY ){
				continue;
			}

			delete_timer(ud->skilltimerskill[i]->timer, skill_timerskill);
			ers_free(skill_timer_ers, ud->skilltimerskill[i]);
			ud->skilltimerskill[i] = nullptr;
		}

		skill_delunitgroup(sg);

		for (int32 i = 0; i < skill_get_time(getSkillId(), skill_lv) / skill_get_unit_interval(getSkillId()); i++)
			skill_addtimerskill(src, tick + (t_tick)i*skill_get_unit_interval(getSkillId()), 0, x, y, getSkillId(), skill_lv, 0, flag);
		flag |= 1;
		skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
	}
}

void SkillStarCannon::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 150 + 650 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;

	RE_LVL_DMOD(100);
}

SkillStarEmperorAdvent::SkillStarEmperorAdvent() : SkillImplRecursiveDamageSplash(SJ_STAREMPEROR) {
}

void SkillStarEmperorAdvent::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_SILENCE, 50 + 10 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillStarEmperorAdvent::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 700 + 200 * skill_lv;
}

void SkillStarEmperorAdvent::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sc && sc->getSCE(SC_DIMENSION)) {
		if (sd) {
			// Remove old shields if any exist.
			pc_delspiritball(sd, sd->spiritball, 0);
			sc_start2(src, target, SC_DIMENSION1, 100, skill_lv, status_get_max_sp(src), skill_get_time2(SJ_BOOKOFDIMENSION, 1));
			sc_start2(src, target, SC_DIMENSION2, 100, skill_lv, status_get_max_sp(src), skill_get_time2(SJ_BOOKOFDIMENSION, 1));
		}
		status_change_end(src, SC_DIMENSION);
	}

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillStarLightKick::SkillStarLightKick() : SkillImplRecursiveDamageSplash(SKE_STAR_LIGHT_KICK) {
}

void SkillStarLightKick::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 400 + 200 * skill_lv;
	skillratio += skill_lv * 5 * pc_checkskill(sd, SKE_SKY_MASTERY);
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillStarLightKick::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	uint8 dir = DIR_NORTHEAST;
	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);	// dir based on target as we move player based on target location
	if (skill_check_unit_movepos(0, src, target->x + dirx[dir], target->y + diry[dir], 1, 1)) {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,1);
		clif_blown(src);
		skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
	} else {
		if (sd != nullptr)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );

		// TODO: Should we return here?
	}

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillStormKick::SkillStormKick() : SkillImpl(TK_STORMKICK) {
}

void SkillStormKick::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 60 + 20 * skill_lv;
}

void SkillStormKick::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_area_temp[1] = 0;
	map_foreachinshootrange(skill_attack_area, src,
	                        skill_get_splash(getSkillId(), skill_lv), BL_CHAR | BL_SKILL,
	                        BF_WEAPON, src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
}

SkillSunsetBlast::SkillSunsetBlast() : SkillImplRecursiveDamageSplash(SKE_SUNSET_BLAST) {
}

void SkillSunsetBlast::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1200 + 500 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 5 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillSunsetBlast::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillTalismanOfBlackTortoise::SkillTalismanOfBlackTortoise() : SkillImpl(SOA_TALISMAN_OF_BLACK_TORTOISE) {
}

void SkillTalismanOfBlackTortoise::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 2150 + 1600 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_TALISMAN_MASTERY) * 15 * skill_lv;
	skillratio += 5 * sstatus->spl;
	if (sc != nullptr && sc->getSCE(SC_T_FIFTH_GOD) != nullptr)
		skillratio += 150 + 500 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillTalismanOfBlackTortoise::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);

	if (sc != nullptr && sc->getSCE(SC_T_THIRD_GOD) != nullptr){
		sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	}
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillTalismanOfBlueDragon::SkillTalismanOfBlueDragon() : SkillImpl(SOA_TALISMAN_OF_BLUE_DRAGON) {
}

void SkillTalismanOfBlueDragon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 1250 + 2650 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_TALISMAN_MASTERY) * 15 * skill_lv;
	skillratio += 5 * sstatus->spl;
	if (sc != nullptr && sc->getSCE(SC_T_FIFTH_GOD) != nullptr)
		skillratio += 850 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillTalismanOfBlueDragon::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
	sc_start(src,src,skill_get_sc(getSkillId()), 100, 1, skill_get_time(getSkillId(), skill_lv));
}

SkillTalismanOfFiveElements::SkillTalismanOfFiveElements() : SkillImpl(SOA_TALISMAN_OF_FIVE_ELEMENTS) {
}

void SkillTalismanOfFiveElements::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );
	sc_type type = skill_get_sc(getSkillId());

	if( dstsd != nullptr ){
		int16 index = dstsd->equip_index[EQI_HAND_R];

		if (index >= 0 && dstsd->inventory_data[index] != nullptr && dstsd->inventory_data[index]->type == IT_WEAPON) {
			clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
			return;
		}
	}

	if( sd != nullptr ){
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_NEED_WEAPON );
	}
}

SkillTalismanOfFourBearingGod::SkillTalismanOfFourBearingGod() : SkillImplRecursiveDamageSplash(SOA_TALISMAN_OF_FOUR_BEARING_GOD) {
}

void SkillTalismanOfFourBearingGod::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr){
		if (sc->hasSCE(SC_T_FIRST_GOD))
			dmg.div_ = 2;
		else if (sc->hasSCE(SC_T_SECOND_GOD))
			dmg.div_ = 3;
		else if (sc->hasSCE(SC_T_THIRD_GOD))
			dmg.div_ = 4;
		else if (sc->hasSCE(SC_T_FOURTH_GOD))
			dmg.div_ = 5;
		else if (sc->hasSCE(SC_T_FIFTH_GOD))
			dmg.div_ = 7;
	}
}

void SkillTalismanOfFourBearingGod::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 50 + 250 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_TALISMAN_MASTERY) * 15 * skill_lv;
	skillratio += 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillTalismanOfFourBearingGod::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillTalismanOfMagician::SkillTalismanOfMagician() : SkillImpl(SOA_TALISMAN_OF_MAGICIAN) {
}

void SkillTalismanOfMagician::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );
	sc_type type = skill_get_sc(getSkillId());

	if( dstsd != nullptr ){
		int16 index = dstsd->equip_index[EQI_HAND_R];

		if (index >= 0 && dstsd->inventory_data[index] != nullptr && dstsd->inventory_data[index]->type == IT_WEAPON) {
			clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
			return;
		}
	}

	if( sd != nullptr ){
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_NEED_WEAPON );
	}
}

SkillTalismanOfProtection::SkillTalismanOfProtection() : SkillImpl(SOA_TALISMAN_OF_PROTECTION) {
}

void SkillTalismanOfProtection::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start2(src, target, type, 100, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv)));
}

SkillTalismanOfRedPhoenix::SkillTalismanOfRedPhoenix() : SkillImplRecursiveDamageSplash(SOA_TALISMAN_OF_RED_PHOENIX) {
}

void SkillTalismanOfRedPhoenix::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 1650 + 2150 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_TALISMAN_MASTERY) * 15 * skill_lv;
	skillratio += 5 * sstatus->spl;
	if (sc != nullptr && sc->getSCE(SC_T_FIFTH_GOD) != nullptr)
		skillratio += 600 + 500 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillTalismanOfRedPhoenix::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	status_change *sc = status_get_sc(src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_area_temp[0] = map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
	if (sc != nullptr && sc->getSCE(SC_T_SECOND_GOD) != nullptr){
		sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillTalismanOfSoulStealing::SkillTalismanOfSoulStealing() : SkillImpl(SOA_TALISMAN_OF_SOUL_STEALING) {
}

void SkillTalismanOfSoulStealing::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 500 + 1250 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_TALISMAN_MASTERY) * 7 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_SOUL_MASTERY) * 7 * skill_lv;
	skillratio += 3 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillTalismanOfSoulStealing::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	if( target->type != BL_SKILL ){
		int32 sp = (100 + status_get_lv(src) / 50) * skill_lv;

		status_heal(src, 0, sp, 0, 0);
		clif_skill_nodamage( src, *src, getSkillId(), sp );
	}
}

SkillTalismanOfWarrior::SkillTalismanOfWarrior() : SkillImpl(SOA_TALISMAN_OF_WARRIOR) {
}

void SkillTalismanOfWarrior::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );
	sc_type type = skill_get_sc(getSkillId());

	if( dstsd != nullptr ){
		int16 index = dstsd->equip_index[EQI_HAND_R];

		if (index >= 0 && dstsd->inventory_data[index] != nullptr && dstsd->inventory_data[index]->type == IT_WEAPON) {
			clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
			return;
		}
	}

	if( sd != nullptr ){
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_NEED_WEAPON );
	}
}

SkillTalismanOfWhiteTiger::SkillTalismanOfWhiteTiger() : SkillImplRecursiveDamageSplash(SOA_TALISMAN_OF_WHITE_TIGER) {
}

void SkillTalismanOfWhiteTiger::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 600 + 1200 * skill_lv;
	skillratio += pc_checkskill(sd, SOA_TALISMAN_MASTERY) * 15 * skill_lv;
	skillratio += 5 * sstatus->spl;
	if (sc != nullptr && sc->getSCE(SC_T_FIFTH_GOD) != nullptr)
		skillratio += 400 + 400 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillTalismanOfWhiteTiger::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);

	if (sc != nullptr && sc->getSCE(SC_T_FIRST_GOD) != nullptr) {
		sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillTotemOfTutelary::SkillTotemOfTutelary() : SkillImpl(SOA_TOTEM_OF_TUTELARY) {
}

void SkillTotemOfTutelary::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillTurnKick::SkillTurnKick() : SkillImpl(TK_TURNKICK) {
}

void SkillTurnKick::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.blewcount = 0;
}

void SkillTurnKick::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 90 + 30 * skill_lv;
}

void SkillTurnKick::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Note: attack_type is passed as BF_WEAPON for the actual target, BF_MISC for the splash-affected mobs.
	if (attack_type & BF_MISC) {
		sc_start(src, target, SC_STUN, 200, skill_lv, skill_get_time(getSkillId(), skill_lv));
		clif_specialeffect(target, EF_SPINEDBODY, AREA);
		sc_start(src, target, SC_NOACTION, 100, 1, skill_get_time2(getSkillId(), skill_lv));
	}
}

void SkillTurnKick::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	// Active part of the attack.
	// Note: skill_area_temp[1] is used in castendNoDamageId to avoid affecting the target.
	skill_area_temp[1] = target->id;

	if (skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag))
		map_foreachinallrange(skill_area_sub, target,
		                      skill_get_splash(getSkillId(), skill_lv), BL_MOB,
		                      src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1,
		                      skill_castend_nodamage_id);
}

void SkillTurnKick::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	// Passive part of the attack. Splash knock-back+stun.
	if (skill_area_temp[1] != target->id) {
		skill_blown(src, target, skill_get_blewcount(getSkillId(), skill_lv), -1, BLOWN_NONE);
		skill_additional_effect(src, target, getSkillId(), skill_lv, BF_MISC, ATK_DEF, tick); // Use Misc rather than weapon to signal passive pushback
	}
}

SkillTwinklingGalaxy::SkillTwinklingGalaxy() : SkillImpl(SKE_TWINKLING_GALAXY) {
}

void SkillTwinklingGalaxy::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillTwinklingGalaxy::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	for (int32 i = 0; i < skill_get_time(getSkillId(), skill_lv) / skill_get_unit_interval(getSkillId()); i++)
		skill_addtimerskill(src, tick + (t_tick)i*skill_get_unit_interval(getSkillId()), 0, x, y, getSkillId(), skill_lv, 0, flag);
	flag |= 1;
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillTwinklingGalaxy::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 250 + 600 * skill_lv;
	skillratio += pc_checkskill(sd, SKE_SKY_MASTERY) * 3 * skill_lv;
	skillratio += 5 * sstatus->pow;

	RE_LVL_DMOD(100);
}

SkillWarmthoftheMoon::SkillWarmthoftheMoon() : SkillImpl(SG_MOON_WARM) {
}

void SkillWarmthoftheMoon::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	// A random 0~3 knockback bonus is added to the base knockback
	dmg.blewcount += rnd_value(0, 3);
}

void SkillWarmthoftheMoon::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	std::shared_ptr<s_skill_unit_group> sg;

	skill_clear_unitgroup(src);
	if ((sg = skill_unitsetting(src,getSkillId(),skill_lv,src->x,src->y,0)))
		sc_start4(src,src,type,100,skill_lv,0,0,sg->group_id,skill_get_time(getSkillId(),skill_lv));
	flag|=1;
}

SkillWarmthoftheStars::SkillWarmthoftheStars() : SkillImpl(SG_STAR_WARM) {
}

void SkillWarmthoftheStars::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	// A random 0~3 knockback bonus is added to the base knockback
	dmg.blewcount += rnd_value(0, 3);
}

void SkillWarmthoftheStars::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	std::shared_ptr<s_skill_unit_group> sg;

	skill_clear_unitgroup(src);
	if ((sg = skill_unitsetting(src,getSkillId(),skill_lv,src->x,src->y,0)))
		sc_start4(src,src,type,100,skill_lv,0,0,sg->group_id,skill_get_time(getSkillId(),skill_lv));
	flag|=1;
}

SkillWarmthoftheSun::SkillWarmthoftheSun() : SkillImpl(SG_SUN_WARM) {
}

void SkillWarmthoftheSun::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	// A random 0~3 knockback bonus is added to the base knockback
	dmg.blewcount += rnd_value(0, 3);
}

void SkillWarmthoftheSun::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	std::shared_ptr<s_skill_unit_group> sg;

	skill_clear_unitgroup(src);
	if ((sg = skill_unitsetting(src,getSkillId(),skill_lv,src->x,src->y,0)))
		sc_start4(src,src,type,100,skill_lv,0,0,sg->group_id,skill_get_time(getSkillId(),skill_lv));
	flag|=1;
}

std::unique_ptr<const SkillImpl> SkillFactoryTaekwon::create(const e_skill skill_id) const {
	switch (skill_id) {
		case SG_FEEL:
			return std::make_unique<SkillFeelingtheSunMoonandStars>();
		case SG_FUSION:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case SG_HATE:
			return std::make_unique<SkillHatredoftheSunMoonandStars>();
		case SG_MOON_COMFORT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SG_MOON_WARM:
			return std::make_unique<SkillWarmthoftheMoon>();
		case SG_STAR_COMFORT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SG_STAR_WARM:
			return std::make_unique<SkillWarmthoftheStars>();
		case SG_SUN_COMFORT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SG_SUN_WARM:
			return std::make_unique<SkillWarmthoftheSun>();
		case SJ_BOOKOFCREATINGSTAR:
			return std::make_unique<SkillBookofCreatingStar>();
		case SJ_BOOKOFDIMENSION:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SJ_DOCUMENT:
			return std::make_unique<SkillDocumentofSunMoonAndStar>();
		case SJ_FALLINGSTAR:
			return std::make_unique<SkillFallingStar>();
		case SJ_FALLINGSTAR_ATK:
			return std::make_unique<SkillFallingStarAttack>();
		case SJ_FALLINGSTAR_ATK2:
			return std::make_unique<SkillFallingStarAttack2>();
		case SJ_FLASHKICK:
			return std::make_unique<SkillFlashKick>();
		case SJ_FULLMOONKICK:
			return std::make_unique<SkillFullMoonKick>();
		case SJ_GRAVITYCONTROL:
			return std::make_unique<SkillGravityControl>();
		case SJ_LIGHTOFMOON:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SJ_LIGHTOFSTAR:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SJ_LIGHTOFSUN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SJ_LUNARSTANCE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case SJ_NEWMOONKICK:
			return std::make_unique<SkillNewMoonKick>();
		case SJ_NOVAEXPLOSING:
			return std::make_unique<SkillNovaExplosion>();
		case SJ_PROMINENCEKICK:
			return std::make_unique<SkillProminenceKick>();
		case SJ_SOLARBURST:
			return std::make_unique<SkillSolarBurst>();
		case SJ_STAREMPEROR:
			return std::make_unique<SkillStarEmperorAdvent>();
		case SJ_STARSTANCE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case SJ_SUNSTANCE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case SJ_UNIVERSESTANCE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case SKE_ALL_IN_THE_SKY:
			return std::make_unique<SkillAllInTheSky>();
		case SKE_DAWN_BREAK:
			return std::make_unique<SkillDawnBreak>();
		case SKE_ENCHANTING_SKY:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SKE_MIDNIGHT_KICK:
			return std::make_unique<SkillMidnightKick>();
		case SKE_NOON_BLAST:
			return std::make_unique<SkillNoonBlast>();
		case SKE_RISING_MOON:
			return std::make_unique<SkillRisingMoon>();
		case SKE_RISING_SUN:
			return std::make_unique<SkillRisingSun>();
		case SKE_SKY_MOON:
			return std::make_unique<SkillSkyMoon>();
		case SKE_SKY_SUN:
			return std::make_unique<SkillSkySun>();
		case SKE_STAR_BURST:
			return std::make_unique<SkillStarBurst>();
		case SKE_STAR_CANNON:
			return std::make_unique<SkillStarCannon>();
		case SKE_STAR_LIGHT_KICK:
			return std::make_unique<SkillStarLightKick>();
		case SKE_SUNSET_BLAST:
			return std::make_unique<SkillSunsetBlast>();
		case SKE_TWINKLING_GALAXY:
			return std::make_unique<SkillTwinklingGalaxy>();
		case SL_ALCHEMIST:
			return std::make_unique<SkillSpiritoftheAlchemist>();
		case SL_ASSASIN:
			return std::make_unique<SkillSpiritoftheAssasin>();
		case SL_BARDDANCER:
			return std::make_unique<SkillSpiritoftheArtist>();
		case SL_BLACKSMITH:
			return std::make_unique<SkillSpiritoftheBlacksmith>();
		case SL_CRUSADER:
			return std::make_unique<SkillSpiritoftheCrusader>();
		case SL_HIGH:
			return std::make_unique<SkillSpiritofRebirth>();
		case SL_HUNTER:
			return std::make_unique<SkillSpiritoftheHunter>();
		case SL_KAAHI:
			return std::make_unique<SkillKaahi>();
		case SL_KAITE:
			return std::make_unique<SkillKaite>();
		case SL_KAIZEL:
			return std::make_unique<SkillKaizel>();
		case SL_KAUPE:
			return std::make_unique<SkillKaupe>();
		case SL_KNIGHT:
			return std::make_unique<SkillSpiritoftheKnight>();
		case SL_MONK:
			return std::make_unique<SkillSpiritoftheMonk>();
		case SL_PRIEST:
			return std::make_unique<SkillSpiritofthePriest>();
		case SL_ROGUE:
			return std::make_unique<SkillSpiritoftheRogue>();
		case SL_SAGE:
			return std::make_unique<SkillSpiritoftheSage>();
		case SL_SKA:
			return std::make_unique<SkillEska>();
		case SL_SKE:
			return std::make_unique<SkillEske>();
		case SL_SMA:
			return std::make_unique<SkillEsma>();
		case SL_SOULLINKER:
			return std::make_unique<SkillSpiritoftheSoulLinker>();
		case SL_STAR:
			return std::make_unique<SkillSpiritoftheStarGladiator>();
		case SL_STIN:
			return std::make_unique<SkillEstin>();
		case SL_STUN:
			return std::make_unique<SkillEstun>();
		case SL_SUPERNOVICE:
			return std::make_unique<SkillSpiritoftheSupernovice>();
		case SL_SWOO:
			return std::make_unique<SkillEswoo>();
		case SL_WIZARD:
			return std::make_unique<SkillSpiritoftheWizard>();
		case SOA_CIRCLE_OF_DIRECTIONS_AND_ELEMENTALS:
			return std::make_unique<SkillCircleOfDirectionsAndElementals>();
		case SOA_EXORCISM_OF_MALICIOUS_SOUL:
			return std::make_unique<SkillExorcismOfMaliciousSoul>();
		case SOA_SOUL_GATHERING:
			return std::make_unique<SkillSoulGathering>();
		case SOA_SOUL_OF_HEAVEN_AND_EARTH:
			return std::make_unique<SkillSoulOfHeavenAndEarth>();
		case SOA_TALISMAN_OF_BLACK_TORTOISE:
			return std::make_unique<SkillTalismanOfBlackTortoise>();
		case SOA_TALISMAN_OF_BLUE_DRAGON:
			return std::make_unique<SkillTalismanOfBlueDragon>();
		case SOA_TALISMAN_OF_FIVE_ELEMENTS:
			return std::make_unique<SkillTalismanOfFiveElements>();
		case SOA_TALISMAN_OF_FOUR_BEARING_GOD:
			return std::make_unique<SkillTalismanOfFourBearingGod>();
		case SOA_TALISMAN_OF_MAGICIAN:
			return std::make_unique<SkillTalismanOfMagician>();
		case SOA_TALISMAN_OF_PROTECTION:
			return std::make_unique<SkillTalismanOfProtection>();
		case SOA_TALISMAN_OF_RED_PHOENIX:
			return std::make_unique<SkillTalismanOfRedPhoenix>();
		case SOA_TALISMAN_OF_SOUL_STEALING:
			return std::make_unique<SkillTalismanOfSoulStealing>();
		case SOA_TALISMAN_OF_WARRIOR:
			return std::make_unique<SkillTalismanOfWarrior>();
		case SOA_TALISMAN_OF_WHITE_TIGER:
			return std::make_unique<SkillTalismanOfWhiteTiger>();
		case SOA_TOTEM_OF_TUTELARY:
			return std::make_unique<SkillTotemOfTutelary>();
		case SP_CURSEEXPLOSION:
			return std::make_unique<SkillCurseExplosion>();
		case SP_KAUTE:
			return std::make_unique<SkillKaute>();
		case SP_SHA:
			return std::make_unique<SkillEsha>();
		case SP_SOULCOLLECT:
			return std::make_unique<SkillSoulCollect>();
		case SP_SOULCURSE:
			return std::make_unique<SkillSoulCurse>();
		case SP_SOULDIVISION:
			return std::make_unique<SkillSoulDivision>();
		case SP_SOULEXPLOSION:
			return std::make_unique<SkillSoulExplosion>();
		case SP_SOULFAIRY:
			return std::make_unique<SkillFairysSoul>();
		case SP_SOULFALCON:
			return std::make_unique<SkillFalconsSoul>();
		case SP_SOULGOLEM:
			return std::make_unique<SkillGolemsSoul>();
		case SP_SOULREAPER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SP_SOULREVOLVE:
			return std::make_unique<SkillSoulRevolution>();
		case SP_SOULSHADOW:
			return std::make_unique<SkillShadowsSoul>();
		case SP_SOULUNITY:
			return std::make_unique<SkillSoulUnity>();
		case SP_SPA:
			return std::make_unique<SkillEspa>();
		case SP_SWHOO:
			return std::make_unique<SkillEswhoo>();
		case TK_COUNTER:
			return std::make_unique<SkillCounter>();
		case TK_DODGE:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case TK_DOWNKICK:
			return std::make_unique<SkillDownKick>();
		case TK_HIGHJUMP:
			return std::make_unique<SkillHighJump>();
		case TK_JUMPKICK:
			return std::make_unique<SkillJumpKick>();
		case TK_MISSION:
			return std::make_unique<SkillMission>();
		case TK_READYCOUNTER:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case TK_READYDOWN:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case TK_READYSTORM:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case TK_READYTURN:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case TK_RUN:
			return std::make_unique<SkillRun>();
		case TK_SEVENWIND:
			return std::make_unique<SkillSevenWind>();
		case TK_STORMKICK:
			return std::make_unique<SkillStormKick>();
		case TK_TURNKICK:
			return std::make_unique<SkillTurnKick>();

		default:
			return nullptr;
	}
}

#endif
