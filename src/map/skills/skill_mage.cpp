// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_mage.hpp"

#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include <config/core.hpp>
#include "map/map.hpp"
#include "map/mob.hpp"
#include "map/pet.hpp"
#include "map/unit.hpp"
#include "map/elemental.hpp"
#include "map/party.hpp"
#include "map/battle.hpp"
#include "map/log.hpp"
#include "map/path.hpp"
#include "skill_impl.hpp"

SkillActivityBurn::SkillActivityBurn() : SkillImpl(EM_ACTIVITY_BURN) {
}

void SkillActivityBurn::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (target->type == BL_PC && rnd() % 100 < 20 + 10 * skill_lv) {
		uint8 ap_burn[5] = { 20, 30, 50, 60, 70 };

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		status_zap(target, 0, 0, ap_burn[skill_lv - 1]);
	} else if (sd)
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
}

// AG_ALL_BLOOM
SkillAllBloom::SkillAllBloom() : SkillImpl(AG_ALL_BLOOM) {
}

void SkillAllBloom::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	sc_start(src, target, type, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillAllBloom::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);

	int32 area = skill_get_splash(getSkillId(), skill_lv);
	int32 unit_time = skill_get_time(getSkillId(), skill_lv);
	int32 unit_interval = skill_get_unit_interval(getSkillId());
	uint16 tmpx = 0, tmpy = 0, climax_lv = 0;
	int32 i = 0;

	// Grab Climax's effect level if active.
	if (sc && sc->getSCE(SC_CLIMAX))
		climax_lv = sc->getSCE(SC_CLIMAX)->val1;

	if (climax_lv == 1) { // Rose buds spawn at double the speed.
		unit_time /= 2;
		unit_interval /= 2;
	}

	// Displays the flower garden.
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);

	if (climax_lv == 4) { // Deals no damage and instead inflicts a status on the enemys in range.
		i = skill_get_splash(getSkillId(), skill_lv);
		map_foreachinallarea(skill_area_sub, src->m, x - i, y - i, x + i, y + i, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	} else for (i = 1; i <= unit_time / unit_interval; i++) { // Spawn the rose buds on random spots at separate intervals
		tmpx = x - area + rnd() % (area * 2 + 1);
		tmpy = y - area + rnd() % (area * 2 + 1);
		skill_unitsetting(src, AG_ALL_BLOOM_ATK, skill_lv, tmpx, tmpy, flag + i * unit_interval);

		if (getSkillId() == AG_ALL_BLOOM && climax_lv == 2) { // Spwan a 2nd rose bud along with the 1st one.
			tmpx = x - area + rnd() % (area * 2 + 1);
			tmpy = y - area + rnd() % (area * 2 + 1);
			skill_unitsetting(src, AG_ALL_BLOOM_ATK, skill_lv, tmpx, tmpy, flag + i * unit_interval);
		}
	}

	// One final attack the size of the flower garden is dealt after
	// all rose buds explode if Climax level 5 is active.
	if (climax_lv == 5)
		skill_unitsetting(src, AG_ALL_BLOOM_ATK2, skill_lv, x, y, flag + i * unit_interval);
}


// AG_ALL_BLOOM_ATK
SkillAllBloomAttack::SkillAllBloomAttack() : SkillImpl(AG_ALL_BLOOM_ATK) {
}

void SkillAllBloomAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 200 + 1200 * skill_lv + 5 * sstatus->spl;
	// (climax buff applied with pc_skillatk_bonus)
	RE_LVL_DMOD(100);
}


// AG_ALL_BLOOM_ATK2
SkillAllBloomAttack2::SkillAllBloomAttack2() : SkillImpl(AG_ALL_BLOOM_ATK2) {
}

void SkillAllBloomAttack2::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 85000;
	// Skill not affected by Baselevel and SPL
}

SkillArrullo::SkillArrullo() : SkillImpl(SO_ARRULLO) {
}

void SkillArrullo::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	int32 rate = (15 + 5 * skill_lv) + status_get_int(src) / 5 + (sd ? sd->status.job_level / 5 : 0) - status_get_int(target) / 6 - status_get_luk(target) / 10;

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, target, skill_get_sc(getSkillId()), rate, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillArrullo::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(),skill_lv);
	map_foreachinallarea(skill_area_sub,src->m,x-i,y-i,x+i,y+i,BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
}

// AG_ASTRAL_STRIKE
SkillAstralStrike::SkillAstralStrike() : SkillImpl(AG_ASTRAL_STRIKE) {
}

void SkillAstralStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 700 + 2600 * skill_lv;
	skillratio += 10 * sstatus->spl;

	if (tstatus->race == RC_UNDEAD || tstatus->race == RC_DRAGON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillAstralStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillAstralStrike::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub, src->m, x-i, y-i, x+i, y+i, BL_CHAR, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_damage_id);
	flag |= 1;
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}


// AG_ASTRAL_STRIKE_ATK
SkillAstralStrikeAttack::SkillAstralStrikeAttack() : SkillImpl(AG_ASTRAL_STRIKE_ATK) {
}

void SkillAstralStrikeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 650 * skill_lv + 10 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillAstralStrikeAttack::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillBeastlyHypnosis::SkillBeastlyHypnosis() : SkillImpl(SA_TAMINGMONSTER) {
}

void SkillBeastlyHypnosis::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if (sd != nullptr && dstmd != nullptr) {
		pet_catch_process_start( *sd, 0, PET_CATCH_UNIVERSAL_ALL );
	}
}

SkillBlindingMist::SkillBlindingMist() : SkillImpl(PF_FOGWALL) {
}

void SkillBlindingMist::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change* tsc = status_get_sc(target);

	if (src != target && (tsc == nullptr || !tsc->hasSCE(SC_DELUGE))) {
		sc_start(src, target, SC_BLIND, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	}
}

void SkillBlindingMist::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= 1;	// Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

SkillCastCancel::SkillCastCancel() : SkillImpl(SA_CASTCANCEL) {
}

void SkillCastCancel::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	unit_skillcastcancel(src,1);
	if(sd) {
		int32 sp = skill_get_sp(sd->skill_id_old,sd->skill_lv_old);
		sp = sp * (90 - (skill_lv-1)*20) / 100;
		if(sp < 0) sp = 0;
		status_zap(src, 0, sp);
	}
}

// WL_CHAINLIGHTNING
SkillChainLightning::SkillChainLightning() : SkillImpl(WL_CHAINLIGHTNING) {
}

void SkillChainLightning::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_addtimerskill(src, tick + status_get_amotion(src), target->id, 0, 0, WL_CHAINLIGHTNING_ATK, skill_lv, 0, 0);
}


// WL_CHAINLIGHTNING_ATK
SkillChainLightningAttack::SkillChainLightningAttack() : SkillImpl(WL_CHAINLIGHTNING_ATK) {
}

void SkillChainLightningAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += 400 + 100 * skill_lv;
	RE_LVL_DMOD(100);
	if (mflag > 0)
		skillratio += 100 * mflag;
}

SkillClassChange::SkillClassChange() : SkillImpl(SA_CLASSCHANGE) {
}

void SkillClassChange::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );
	int32 i = 0;

	if (dstmd)
	{
		int32 class_;

		if ( sd && status_has_mode(&dstmd->status,MD_STATUSIMMUNE) ) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		class_ = mob_get_random_id(MOBG_CLASSCHANGE, RMF_DB_RATE, 0);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		mob_class_change(dstmd,class_);
		if( tsc && status_has_mode(&dstmd->status,MD_STATUSIMMUNE) ) {
			const enum sc_type scs[] = { SC_QUAGMIRE, SC_PROVOKE, SC_ROKISWEIL, SC_GRAVITATION, SC_SUITON, SC_STRIPWEAPON, SC_STRIPSHIELD, SC_STRIPARMOR, SC_STRIPHELM, SC_BLADESTOP };
			for (i = SC_COMMON_MIN; i <= SC_COMMON_MAX; i++)
				if (tsc->getSCE(i)) status_change_end(target, (sc_type)i);
			for (i = 0; i < ARRAYLENGTH(scs); i++)
				if (tsc->getSCE(scs[i])) status_change_end(target, scs[i]);
		}
	}
}

SkillCloudKill::SkillCloudKill() : SkillImpl(SO_CLOUD_KILL) {
}

void SkillCloudKill::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillCloudKill::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 40 * skill_lv;
	skillratio += sstatus->int_ * 3;
	RE_LVL_DMOD(100);
	if (sc) {
		if (sc->getSCE(SC_CURSED_SOIL_OPTION))
			skillratio += (sd ? sd->status.job_level : 0);

		if (sc->getSCE(SC_DEEP_POISONING_OPTION))
			skillratio += skillratio * 1500 / 100;
	}
}

void SkillCloudKill::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= 4;
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillColdBolt::SkillColdBolt() : SkillImpl(MG_COLDBOLT) {
}

void SkillColdBolt::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	if (sc) {
		if (sc->getSCE(SC_COLD_FORCE_OPTION))
			base_skillratio *= 5;

		if (sc->getSCE(SC_SPELLFIST) && mflag & BF_SHORT) {
			base_skillratio += (sc->getSCE(SC_SPELLFIST)->val3 * 100) + (sc->getSCE(SC_SPELLFIST)->val1 * 50 - 50) - 100;
			// val3 = used bolt level, val1 = used spellfist level. [Rytech]
		}
	}
}

void SkillColdBolt::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillColdBolt::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr) {
		if (sc->hasSCE(SC_SPELLFIST) && (dmg.miscflag & BF_SHORT)) {
			dmg.div_ = 1; // ad mods, to make it work similar to regular hits [Xazax]
			dmg.flag = BF_WEAPON | BF_SHORT;
			dmg.type = DMG_NORMAL;
		}
	}
}

SkillComa::SkillComa() : SkillImpl(SA_COMA) {
}

void SkillComa::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start(src,target,type,100,skill_lv,skill_get_time2(getSkillId(),skill_lv)));
}

SkillComet::SkillComet() : SkillImpl(WL_COMET) {
}

void SkillComet::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_MAGIC_POISON, 100, skill_lv, 20000);
}

void SkillComet::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 2500 + 700 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillComet::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillConflagration::SkillConflagration() : SkillImpl(EM_CONFLAGRATION) {
}

void SkillConflagration::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_HANDICAPSTATE_CONFLAGRATION, 3, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillConflagration::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 700 + 1100 * skill_lv;
	skillratio += 5 * sstatus->spl;

	if( sc != nullptr && sc->getSCE( SC_SUMMON_ELEMENTAL_ARDOR ) ){
		skillratio += 200 * skill_lv;
		skillratio += 2 * sstatus->spl;
	}

	RE_LVL_DMOD(100);
}

void SkillConflagration::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillCreateElementalConverter::SkillCreateElementalConverter() : SkillImpl(SA_CREATECON) {
}

void SkillCreateElementalConverter::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd != nullptr ){
		clif_elementalconverter_list( *sd );
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

// AG_CRIMSON_ARROW
SkillCrimsonArrow::SkillCrimsonArrow() : SkillImpl(AG_CRIMSON_ARROW) {
}

void SkillCrimsonArrow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 400 * skill_lv + 3 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillCrimsonArrow::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = target->id;
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
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
	skill_castend_damage_id(src, target, AG_CRIMSON_ARROW_ATK, skill_lv, tick, flag|SD_LEVEL|SD_ANIMATION);
}


// AG_CRIMSON_ARROW_ATK
SkillCrimsonArrowAttack::SkillCrimsonArrowAttack() : SkillImplRecursiveDamageSplash(AG_CRIMSON_ARROW_ATK) {
}

void SkillCrimsonArrowAttack::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_CLIMAX)) {
		dmg.div_ = 2;
	}
}

void SkillCrimsonArrowAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 750 * skill_lv + 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

SkillCrimsonRock::SkillCrimsonRock() : SkillImplRecursiveDamageSplash(WL_CRIMSONROCK) {
}

void SkillCrimsonRock::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 700 + 600 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillCrimsonRock::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	skill_area_temp[4] = target->x;
	skill_area_temp[5] = target->y;

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

// AG_CRYSTAL_IMPACT
SkillCrystalImpact::SkillCrystalImpact() : SkillImplRecursiveDamageSplash(AG_CRYSTAL_IMPACT) {
}

void SkillCrystalImpact::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_CLIMAX) && sc->getSCE(SC_CLIMAX)->val1 == 2)
		dmg.div_ = 2;
}

void SkillCrystalImpact::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 400 + 1400 * skill_lv;
	skillratio += 10 * sstatus->spl;

	// (climax buff applied with pc_skillatk_bonus)
	RE_LVL_DMOD(100);
}

void SkillCrystalImpact::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());

	if (flag&1) { // Buff from Crystal Impact with level 1 Climax.
		sc_start(src, target, type, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	} else {
		uint16 climax_lv = 0, splash_size = skill_get_splash(getSkillId(), skill_lv);

		if (sc && sc->getSCE(SC_CLIMAX))
			climax_lv = sc->getSCE(SC_CLIMAX)->val1;

		if (climax_lv == 5) { // Adjusts splash AoE size depending on skill.
			splash_size = 7; // 15x15
		}

		skill_area_temp[1] = 0;
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		if (climax_lv == 1) // Buffs the caster and allies instead of doing damage AoE.
			map_foreachinrange(skill_area_sub, target, splash_size, BL_CHAR, src, getSkillId(), skill_lv, tick, flag|BCT_ALLY|SD_SPLASH|1, skill_castend_nodamage_id);
		else
			map_foreachinrange(skill_area_sub, target, splash_size, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
}

void SkillCrystalImpact::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Targets hit are dealt aftershock damage.
	skill_castend_damage_id(src, target, AG_CRYSTAL_IMPACT_ATK, skill_lv, tick, SD_LEVEL);
}


// AG_CRYSTAL_IMPACT_ATK
SkillCrystalImpactAttack::SkillCrystalImpactAttack() : SkillImplRecursiveDamageSplash(AG_CRYSTAL_IMPACT_ATK) {
}

void SkillCrystalImpactAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 400 + 1400 * skill_lv;
	skillratio += 10 * sstatus->spl;

	// (climax buff applied with pc_skillatk_bonus)
	RE_LVL_DMOD(100);
}

int16 SkillCrystalImpactAttack::getSearchSize(block_list* src, uint16 skill_lv) const {
	status_change *sc = status_get_sc(src);

	if (sc != nullptr && sc->hasSCE(SC_CLIMAX) && sc->getSCE(SC_CLIMAX)->val1 == 5)
		return 2;// Gives the aftershock hit a 5x5 splash AoE.

	return SkillImplRecursiveDamageSplash::getSearchSize(src, skill_lv);
}

int16 SkillCrystalImpactAttack::getSplashSearchSize(block_list* src, uint16 skill_lv) const {
	status_change *sc = status_get_sc(src);

	if (sc != nullptr && sc->hasSCE(SC_CLIMAX) && sc->getSCE(SC_CLIMAX)->val1 == 5)
		return 2;// Gives the aftershock hit a 5x5 splash AoE.

	return SkillImplRecursiveDamageSplash::getSplashSearchSize(src, skill_lv);
}

SkillDeadlyProjection::SkillDeadlyProjection() : SkillImpl(AG_DEADLY_PROJECTION) {
}

void SkillDeadlyProjection::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 2800 * skill_lv + 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillDeadlyProjection::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_start(src, target, SC_DEADLY_DEFEASANCE, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillDeluge::SkillDeluge() : SkillImpl(SA_DELUGE) {
}

void SkillDeluge::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Does not consumes if the skill is already active. [Skotlex]
	std::shared_ptr<s_skill_unit_group> sg2;
	if ((sg2= skill_locate_element_field(src)) != nullptr && ( sg2->skill_id == SA_VOLCANO || sg2->skill_id == SA_DELUGE || sg2->skill_id == SA_VIOLENTGALE ))
	{
		if (sg2->limit - DIFF_TICK(gettick(), sg2->tick) > 0)
		{
			skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
			flag |= SKILL_NOCONSUME_REQ; // not to consume items
			return;
		}
		else
			sg2->limit = 0; //Disable it.
	}
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

// AG_DESTRUCTIVE_HURRICANE
SkillDestructiveHurricane::SkillDestructiveHurricane() : SkillImplRecursiveDamageSplash(AG_DESTRUCTIVE_HURRICANE) {
}

void SkillDestructiveHurricane::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_CLIMAX) && sc->getSCE(SC_CLIMAX)->val1 == 2)
		dmg.blewcount = 2;
}

void SkillDestructiveHurricane::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 600 + 3250 * skill_lv;
	skillratio += 10 * sstatus->spl;

	// (climax buff applied with pc_skillatk_bonus)
	RE_LVL_DMOD(100);
}

void SkillDestructiveHurricane::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());

	if (flag&1) { // Buff from Crystal Impact with level 1 Climax.
		sc_start(src, target, type, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	} else {
		uint16 climax_lv = 0, splash_size = skill_get_splash(getSkillId(), skill_lv);

		if (sc && sc->getSCE(SC_CLIMAX))
			climax_lv = sc->getSCE(SC_CLIMAX)->val1;

		if (climax_lv == 5) { // Adjusts splash AoE size depending on skill.
			splash_size = 9; // 19x19
		}

		skill_area_temp[1] = 0;
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		if (climax_lv == 4) // Buff for caster instead of damage AoE.
			sc_start(src, target, type, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
		else {
			if (climax_lv == 1) // Display extra animation for the additional hit cast.
				clif_skill_nodamage(src, *target, AG_DESTRUCTIVE_HURRICANE_CLIMAX, skill_lv);

			map_foreachinrange(skill_area_sub, target, splash_size, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		}
	}
}

void SkillDestructiveHurricane::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change *sc = status_get_sc(src);

	// Targets hit are dealt a additional hit through Climax.
	if (sc && sc->getSCE(SC_CLIMAX) && sc->getSCE(SC_CLIMAX)->val1 == 1)
		skill_castend_damage_id(src, target, AG_DESTRUCTIVE_HURRICANE_CLIMAX, skill_lv, tick, SD_LEVEL|SD_ANIMATION);
}


// AG_DESTRUCTIVE_HURRICANE_CLIMAX
SkillDestructiveHurricaneClimax::SkillDestructiveHurricaneClimax() : SkillImpl(AG_DESTRUCTIVE_HURRICANE_CLIMAX) {
}

void SkillDestructiveHurricaneClimax::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 12500;
	// Skill not affected by Baselevel and SPL
}

void SkillDestructiveHurricaneClimax::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillDiamondDust::SkillDiamondDust() : SkillImpl(SO_DIAMONDDUST) {
}

void SkillDiamondDust::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change* sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 rate = 5 + 5 * skill_lv;

	if( sc && sc->getSCE(SC_COOLER_OPTION) )
		rate += (sd ? sd->status.job_level / 5 : 0);

	sc_start(src, target, SC_CRYSTALIZE, rate, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillDiamondDust::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 2 * sstatus->int_ + 300 * pc_checkskill(sd, SA_FROSTWEAPON) + sstatus->int_ * skill_lv;
	RE_LVL_DMOD(100);
	if( sc && sc->getSCE(SC_COOLER_OPTION) )
		skillratio += (sd ? sd->status.job_level * 5 : 0);
}

void SkillDiamondDust::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillDiamondStorm::SkillDiamondStorm() : SkillImpl(EM_DIAMOND_STORM) {
}

void SkillDiamondStorm::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_HANDICAPSTATE_FROSTBITE, 5, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillDiamondStorm::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	if (const status_change *sc = status_get_sc(src); sc != nullptr && sc->hasSCE( SC_SUMMON_ELEMENTAL_DILUVIO )) {
		skillratio += -100 + 8100 + 2700 * skill_lv;
		skillratio += 10 * sstatus->spl;
	}
	else {
		skillratio += -100 + 600 + 3000 * skill_lv;
		skillratio += 7 * sstatus->spl;
	}

	RE_LVL_DMOD(100);
}

void SkillDiamondStorm::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillDispell::SkillDispell() : SkillImpl(SA_DISPELL) {
}

void SkillDispell::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );
	int32 i = 0;

	if (flag&1 || (i = skill_get_splash(getSkillId(), skill_lv)) < 1) {
		if (sd && dstsd && !map_flag_vs(sd->m) && (!sd->duel_group || sd->duel_group != dstsd->duel_group) && (!sd->status.party_id || sd->status.party_id != dstsd->status.party_id))
			return; // Outside PvP it should only affect party members and no skill fail message
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		if((dstsd && (dstsd->class_&MAPID_SECONDMASK) == MAPID_SOUL_LINKER)
			|| (tsc && tsc->getSCE(SC_SPIRIT) && tsc->getSCE(SC_SPIRIT)->val2 == SL_ROGUE) //Rogue's spirit defends against dispel.
			|| rnd()%100 >= 50+10*skill_lv)
		{
			if (sd)
				clif_skill_fail( *sd, getSkillId() );
			return;
		}
		if(status_isimmune(target))
			return;

		//Remove bonus_script by Dispell
		if (dstsd)
			pc_bonus_script_clear(dstsd,BSF_REM_ON_DISPELL);
		// Monsters will unlock their target instead
		else if (dstmd)
			mob_unlocktarget(dstmd, tick);

		if(tsc == nullptr || tsc->empty())
			return;

		//Statuses that can't be Dispelled
		for (const auto &it : status_db) {
			sc_type status = static_cast<sc_type>(it.first);

			if (!tsc->getSCE(status))
				continue;

			if (it.second->flag[SCF_NODISPELL])
				continue;
			switch (status) {
				// bugreport:4888 these songs may only be dispelled if you're not in their song area anymore
				case SC_WHISTLE:		case SC_ASSNCROS:		case SC_POEMBRAGI:
				case SC_APPLEIDUN:		case SC_HUMMING:		case SC_DONTFORGETME:
				case SC_FORTUNE:		case SC_SERVICE4U:
					if (!battle_config.dispel_song || tsc->getSCE(status)->val4 == 0)
						continue; //If in song area don't end it, even if config enatargeted
					break;
				case SC_ASSUMPTIO:
					if( target->type == BL_MOB )
						continue;
					break;
			}
			if (status == SC_BERSERK || status == SC_SATURDAYNIGHTFEVER)
				tsc->getSCE(status)->val2 = 0; //Mark a dispelled berserk to avoid setting hp to 100 by setting hp penalty to 0.
			status_change_end(target, status);
		}
		return;
	}

	//Affect all targets on splash area.
	map_foreachinallrange(skill_area_sub, target, i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag|1,
		skill_castend_damage_id);
}

SkillDrainLife::SkillDrainLife() : SkillImpl(WL_DRAINLIFE) {
}

void SkillDrainLife::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 200 * skill_lv + sstatus->int_;
	RE_LVL_DMOD(100);
}

void SkillDrainLife::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 heal = (int32)skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	int32 rate = 70 + 5 * skill_lv;

	heal = heal * (5 + 5 * skill_lv) / 100;

	if( target->type == BL_SKILL )
		heal = 0; // Don't absorb heal from Ice Walls or other skill units.

	if( heal && rnd()%100 < rate )
	{
		status_heal(src, heal, 0, 0);
		clif_skill_nodamage(nullptr, *src, AL_HEAL, heal);
	}
}

SkillEarthGrave::SkillEarthGrave() : SkillImpl(SO_EARTHGRAVE) {
}

void SkillEarthGrave::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start2(src, target, SC_BLEEDING, 5 * skill_lv, skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv)); // Need official rate. [LimitLine]
}

void SkillEarthGrave::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 2 * sstatus->int_ + 300 * pc_checkskill(sd, SA_SEISMICWEAPON) + sstatus->int_ * skill_lv;
	RE_LVL_DMOD(100);
	if( sc && sc->getSCE(SC_CURSED_SOIL_OPTION) )
		skillratio += (sd ? sd->status.job_level * 5 : 0);
}

void SkillEarthGrave::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillEarthInsignia::SkillEarthInsignia() : SkillImpl(SO_EARTH_INSIGNIA) {
}

void SkillEarthInsignia::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillEarthSpike::SkillEarthSpike() : SkillImpl(WZ_EARTHSPIKE) {
}

void SkillEarthSpike::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillEarthSpike::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_change* sc = status_get_sc(src);

	base_skillratio += 100;
	if (sc && sc->getSCE(SC_EARTH_CARE_OPTION))
		base_skillratio += base_skillratio * 800 / 100;
#endif
}

SkillEarthStrain::SkillEarthStrain() : SkillImpl(WL_EARTHSTRAIN) {
}

void SkillEarthStrain::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 1000 + 600 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillEarthStrain::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 w, wave = skill_lv + 4, dir = map_calc_dir(src,x,y);
	int32 sx = x = src->x, sy = y = src->y; // Store first caster's location to avoid glitch on unit setting

	for( w = 1; w <= wave; w++ )
	{
		switch( dir ){
			case 0: case 1: case 7: sy = y + w; break;
			case 3: case 4: case 5: sy = y - w; break;
			case 2: sx = x - w; break;
			case 6: sx = x + w; break;
		}
		skill_addtimerskill(src,gettick() + (140 * w),0,sx,sy,getSkillId(),skill_lv,dir,flag&2);
	}
}

SkillElectricWalk::SkillElectricWalk() : SkillImpl(SO_ELECTRICWALK) {
}

void SkillElectricWalk::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());

	if (sc && sc->getSCE(type))
		status_change_end(src, type);

	sc_start2(src, src, type, 100, getSkillId(), skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillElectricWalk::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 60 * skill_lv;
	RE_LVL_DMOD(100);
	if( sc && sc->getSCE(SC_BLAST_OPTION) )
		skillratio += (sd ? sd->status.job_level / 2 : 0);
}

SkillElementalAction::SkillElementalAction() : SkillImpl(SO_EL_ACTION) {
}

void SkillElementalAction::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int32 duration = 3000;
		if( !sd->ed )
			return;
		switch(sd->ed->db->class_) {
			case ELEMENTALID_AGNI_M: case ELEMENTALID_AQUA_M:
			case ELEMENTALID_VENTUS_M: case ELEMENTALID_TERA_M:
				duration = 6000;
				break;
			case ELEMENTALID_AGNI_L: case ELEMENTALID_AQUA_L:
			case ELEMENTALID_VENTUS_L: case ELEMENTALID_TERA_L:
				duration = 9000;
				break;
		}
		sd->skill_id_old = getSkillId();
		elemental_action(sd->ed, target, tick);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		skill_blockpc_start(*sd, getSkillId(), duration);
	}
}

// EM_ELEMENTAL_BUSTER
SkillElementalBuster::SkillElementalBuster() : SkillImpl(EM_ELEMENTAL_BUSTER) {
}

void SkillElementalBuster::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr)
		return;

	if (!sd->ed || !(sd->ed->elemental.class_ >= ELEMENTALID_DILUVIO && sd->ed->elemental.class_ <= ELEMENTALID_SERPENS)) {
		clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	uint16 buster_element;

	switch (sd->ed->elemental.class_) {
		case ELEMENTALID_ARDOR:
			buster_element = EM_ELEMENTAL_BUSTER_FIRE;
			break;
		case ELEMENTALID_DILUVIO:
			buster_element = EM_ELEMENTAL_BUSTER_WATER;
			break;
		case ELEMENTALID_PROCELLA:
			buster_element = EM_ELEMENTAL_BUSTER_WIND;
			break;
		case ELEMENTALID_TERREMOTUS:
			buster_element = EM_ELEMENTAL_BUSTER_GROUND;
			break;
		case ELEMENTALID_SERPENS:
			buster_element = EM_ELEMENTAL_BUSTER_POISON;
			break;
	}

	skill_area_temp[1] = 0;
	clif_skill_nodamage(src, *target, buster_element, skill_lv);// Animation for the triggered blaster element.
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);// Triggered after blaster animation to make correct skill name scream appear.
	map_foreachinrange(skill_area_sub, target, 6, BL_CHAR | BL_SKILL, src, buster_element, skill_lv, tick, flag | BCT_ENEMY | SD_LEVEL | SD_SPLASH | 1, skill_castend_damage_id);
}


// EM_ELEMENTAL_BUSTER_FIRE
SkillElementalBusterFire::SkillElementalBusterFire() : SkillImplRecursiveDamageSplash(EM_ELEMENTAL_BUSTER_FIRE) {
}

void SkillElementalBusterFire::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 550 + 2650 * skill_lv;
	skillratio += 10 * sstatus->spl;
	if (tstatus->race == RC_FORMLESS || tstatus->race == RC_DRAGON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}


// EM_ELEMENTAL_BUSTER_GROUND
SkillElementalBusterGround::SkillElementalBusterGround() : SkillImplRecursiveDamageSplash(EM_ELEMENTAL_BUSTER_GROUND) {
}

void SkillElementalBusterGround::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 550 + 2650 * skill_lv;
	skillratio += 10 * sstatus->spl;
	if (tstatus->race == RC_FORMLESS || tstatus->race == RC_DRAGON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}


// EM_ELEMENTAL_BUSTER_POISON
SkillElementalBusterPoison::SkillElementalBusterPoison() : SkillImplRecursiveDamageSplash(EM_ELEMENTAL_BUSTER_POISON) {
}

void SkillElementalBusterPoison::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 550 + 2650 * skill_lv;
	skillratio += 10 * sstatus->spl;
	if (tstatus->race == RC_FORMLESS || tstatus->race == RC_DRAGON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}


// EM_ELEMENTAL_BUSTER_WATER
SkillElementalBusterWater::SkillElementalBusterWater() : SkillImplRecursiveDamageSplash(EM_ELEMENTAL_BUSTER_WATER) {
}

void SkillElementalBusterWater::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 550 + 2650 * skill_lv;
	skillratio += 10 * sstatus->spl;
	if (tstatus->race == RC_FORMLESS || tstatus->race == RC_DRAGON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}


// EM_ELEMENTAL_BUSTER_WIND
SkillElementalBusterWind::SkillElementalBusterWind() : SkillImplRecursiveDamageSplash(EM_ELEMENTAL_BUSTER_WIND) {
}

void SkillElementalBusterWind::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 550 + 2650 * skill_lv;
	skillratio += 10 * sstatus->spl;
	if (tstatus->race == RC_FORMLESS || tstatus->race == RC_DRAGON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillElementalChangeEarth::SkillElementalChangeEarth() : SkillImpl(SA_ELEMENTGROUND) {
}

void SkillElementalChangeEarth::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	mob_data* dstmd = BL_CAST( BL_MOB, target );

	if (sd && (!dstmd || status_has_mode(tstatus,MD_STATUSIMMUNE))) // Only works on monsters (Except status immune monsters).
		return;
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, type, 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
			skill_get_time(getSkillId(), skill_lv)));
}

SkillElementalChangeFire::SkillElementalChangeFire() : SkillImpl(SA_ELEMENTFIRE) {
}

void SkillElementalChangeFire::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	mob_data* dstmd = BL_CAST( BL_MOB, target );

	if (sd && (!dstmd || status_has_mode(tstatus,MD_STATUSIMMUNE))) // Only works on monsters (Except status immune monsters).
		return;
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, type, 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
			skill_get_time(getSkillId(), skill_lv)));
}

SkillElementalChangeWater::SkillElementalChangeWater() : SkillImpl(SA_ELEMENTWATER) {
}

void SkillElementalChangeWater::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	mob_data* dstmd = BL_CAST( BL_MOB, target );

	if (sd && (!dstmd || status_has_mode(tstatus,MD_STATUSIMMUNE))) // Only works on monsters (Except status immune monsters).
		return;
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, type, 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
			skill_get_time(getSkillId(), skill_lv)));
}

SkillElementalChangeWind::SkillElementalChangeWind() : SkillImpl(SA_ELEMENTWIND) {
}

void SkillElementalChangeWind::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	mob_data* dstmd = BL_CAST( BL_MOB, target );

	if (sd && (!dstmd || status_has_mode(tstatus,MD_STATUSIMMUNE))) // Only works on monsters (Except status immune monsters).
		return;
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target, type, 100, skill_lv, skill_get_ele(getSkillId(),skill_lv),
			skill_get_time(getSkillId(), skill_lv)));
}

SkillElementalShield::SkillElementalShield() : SkillImpl(SO_ELEMENTAL_SHIELD) {
}

void SkillElementalShield::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (!sd || sd->status.party_id == 0 || flag&1) {
		if (sd && sd->status.party_id == 0) {
			clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
			if (sd->ed && skill_get_state(getSkillId()) == ST_ELEMENTALSPIRIT2)
				elemental_delete(sd->ed);
		}
		skill_unitsetting(target, MG_SAFETYWALL, skill_lv + 5, target->x, target->y, 0);
		skill_unitsetting(target, AL_PNEUMA, 1, target->x, target->y, 0);
	}
	else {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		if (sd->ed && skill_get_state(getSkillId()) == ST_ELEMENTALSPIRIT2)
			elemental_delete(sd->ed);
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(),skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillElementalVeil::SkillElementalVeil() : SkillImpl(EM_ELEMENTAL_VEIL) {
}

void SkillElementalVeil::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if (sd == nullptr)
		return;

	if (sd->ed && sd->ed->elemental.class_ >= ELEMENTALID_DILUVIO && sd->ed->elemental.class_ <= ELEMENTALID_SERPENS)
		sc_start(src, sd->ed, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	else
		clif_skill_fail( *sd, getSkillId() );
}

SkillEndowBlaze::SkillEndowBlaze() : SkillImpl(SA_FLAMELAUNCHER) {
}

void SkillEndowBlaze::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );

	if (dstsd && dstsd->status.weapon == W_FIST) {
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,false);
		return;
	}
#ifdef RENEWAL
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
#else
	// 100% success rate at lv4 & 5, but lasts longer at lv5
	if(!clif_skill_nodamage(src,*target,getSkillId(),skill_lv, sc_start(src,target,type,(60+skill_lv*10),skill_lv, skill_get_time(getSkillId(),skill_lv)))) {
		if (dstsd){
			int16 index = dstsd->equip_index[EQI_HAND_R];
			if (index != -1 && dstsd->inventory_data[index] && dstsd->inventory_data[index]->type == IT_WEAPON)
				pc_unequipitem(dstsd, index, 3); //Must unequip the weapon instead of breaking it [Daegaladh]
		}
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
	}
#endif
}

SkillEndowQuake::SkillEndowQuake() : SkillImpl(SA_SEISMICWEAPON) {
}

void SkillEndowQuake::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );

	if (dstsd && dstsd->status.weapon == W_FIST) {
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,false);
		return;
	}
#ifdef RENEWAL
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
#else
	// 100% success rate at lv4 & 5, but lasts longer at lv5
	if(!clif_skill_nodamage(src,*target,getSkillId(),skill_lv, sc_start(src,target,type,(60+skill_lv*10),skill_lv, skill_get_time(getSkillId(),skill_lv)))) {
		if (dstsd){
			int16 index = dstsd->equip_index[EQI_HAND_R];
			if (index != -1 && dstsd->inventory_data[index] && dstsd->inventory_data[index]->type == IT_WEAPON)
				pc_unequipitem(dstsd, index, 3); //Must unequip the weapon instead of breaking it [Daegaladh]
		}
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
	}
#endif
}

SkillEndowTornado::SkillEndowTornado() : SkillImpl(SA_LIGHTNINGLOADER) {
}

void SkillEndowTornado::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );

	if (dstsd && dstsd->status.weapon == W_FIST) {
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,false);
		return;
	}
#ifdef RENEWAL
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
#else
	// 100% success rate at lv4 & 5, but lasts longer at lv5
	if(!clif_skill_nodamage(src,*target,getSkillId(),skill_lv, sc_start(src,target,type,(60+skill_lv*10),skill_lv, skill_get_time(getSkillId(),skill_lv)))) {
		if (dstsd){
			int16 index = dstsd->equip_index[EQI_HAND_R];
			if (index != -1 && dstsd->inventory_data[index] && dstsd->inventory_data[index]->type == IT_WEAPON)
				pc_unequipitem(dstsd, index, 3); //Must unequip the weapon instead of breaking it [Daegaladh]
		}
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
	}
#endif
}

SkillEndowTsunami::SkillEndowTsunami() : SkillImpl(SA_FROSTWEAPON) {
}

void SkillEndowTsunami::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );

	if (dstsd && dstsd->status.weapon == W_FIST) {
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,false);
		return;
	}
#ifdef RENEWAL
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
#else
	// 100% success rate at lv4 & 5, but lasts longer at lv5
	if(!clif_skill_nodamage(src,*target,getSkillId(),skill_lv, sc_start(src,target,type,(60+skill_lv*10),skill_lv, skill_get_time(getSkillId(),skill_lv)))) {
		if (dstsd){
			int16 index = dstsd->equip_index[EQI_HAND_R];
			if (index != -1 && dstsd->inventory_data[index] && dstsd->inventory_data[index]->type == IT_WEAPON)
				pc_unequipitem(dstsd, index, 3); //Must unequip the weapon instead of breaking it [Daegaladh]
		}
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
	}
#endif
}

SkillEnergyCoat::SkillEnergyCoat() : StatusSkillImpl(MG_ENERGYCOAT) {
}

SkillEnergyConversion::SkillEnergyConversion() : SkillImpl(AG_ENERGY_CONVERSION) {
}

void SkillEnergyConversion::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (status_get_sp(src) == status_get_max_sp(src)) {
		if( sd != nullptr ){
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		}
		return;
	}
	
	// Apply the SP gain to the caster
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	status_heal(target, 0, (skill_lv * (skill_lv + 1) / 2) * 80, 1);
}

SkillFiberLock::SkillFiberLock() : SkillImpl(PF_SPIDERWEB) {
}

void SkillFiberLock::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= 1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

SkillFireBall::SkillFireBall() : SkillImplRecursiveDamageSplash(MG_FIREBALL) {
}

void SkillFireBall::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 40 + 20 * skill_lv;
#else
	base_skillratio += -30 + 10 * skill_lv;
#endif
	if (wd->miscflag == 2) //Enemies at the edge of the area will take 75% of the damage
		base_skillratio = base_skillratio * 3 / 4;
}

void SkillFireBall::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);
}

int64 SkillFireBall::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	// For players, the distance between original target and splash target determines the damage
	if( map_session_data* sd = BL_CAST( BL_PC, src ); sd != nullptr ){
		if (block_list* orig_bl = map_id2bl(skill_area_temp[1]); orig_bl != nullptr)
			flag |= distance_bl(orig_bl, target);
	}

	// Call default implementation
	return SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag);
}

SkillFireBolt::SkillFireBolt() : SkillImpl(MG_FIREBOLT) {
}

void SkillFireBolt::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	if (sc) {
		if (sc->getSCE(SC_FLAMETECHNIC_OPTION))
			base_skillratio *= 5;

		if (sc->getSCE(SC_SPELLFIST) && mflag & BF_SHORT) {
			base_skillratio += (sc->getSCE(SC_SPELLFIST)->val3 * 100) + (sc->getSCE(SC_SPELLFIST)->val1 * 50 - 50) - 100;
			// val3 = used bolt level, val1 = used spellfist level. [Rytech]
		}
	}
}

void SkillFireBolt::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillFireBolt::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr) {
		if (sc->hasSCE(SC_SPELLFIST) && (dmg.miscflag & BF_SHORT)) {
			dmg.div_ = 1; // ad mods, to make it work similar to regular hits [Xazax]
			dmg.flag = BF_WEAPON | BF_SHORT;
			dmg.type = DMG_NORMAL;
		}
	}
}

SkillFireInsignia::SkillFireInsignia() : SkillImpl(SO_FIRE_INSIGNIA) {
}

void SkillFireInsignia::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillFirePillar::SkillFirePillar() : SkillImpl(WZ_FIREPILLAR) {
}

void SkillFirePillar::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillFirePillar::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += -60 + 20 * skill_lv; //20% MATK each hit
}

void SkillFirePillar::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	unit_set_walkdelay(target, tick, skill_get_time2(getSkillId(), skill_lv), 1);
}

void SkillFirePillar::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && dmg.div_ > 0)
		dmg.div_ *= -1; // For players, damage is divided by number of hits
}

SkillFireWalk::SkillFireWalk() : SkillImpl(SO_FIREWALK) {
}

void SkillFireWalk::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());

	if (sc && sc->getSCE(type))
		status_change_end(src, type);

	sc_start2(src, src, type, 100, getSkillId(), skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillFireWalk::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 60 * skill_lv;
	RE_LVL_DMOD(100);
	if( sc && sc->getSCE(SC_HEATER_OPTION) )
		skillratio += (sd ? sd->status.job_level / 2 : 0);
}

SkillFireWall::SkillFireWall() : SkillImpl(MG_FIREWALL) {
}

void SkillFireWall::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillFireWall::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio -= 50;
}

void SkillFireWall::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_data* tstatus = status_get_status_data(target);

	if (tstatus->def_ele == ELE_FIRE || battle_check_undead(tstatus->race, tstatus->def_ele)) {
		dmg.blewcount = 0; // No knockback
	}
}

SkillFloralFlareRoad::SkillFloralFlareRoad() : SkillImpl(AG_FLORAL_FLARE_ROAD) {
}

void SkillFloralFlareRoad::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 50 + 740 * skill_lv + 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillFloralFlareRoad::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillFourSpiritAnalysis::SkillFourSpiritAnalysis() : SkillImpl(SO_EL_ANALYSIS) {
}

void SkillFourSpiritAnalysis::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		clif_skill_itemlistwindow(sd,getSkillId(),skill_lv);
	}
}

SkillFrostDiver::SkillFrostDiver() : SkillImpl(MG_FROSTDIVER) {
}

void SkillFrostDiver::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 10 * skill_lv;
}

void SkillFrostDiver::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillFrostDiver::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	if (!sc_start(src, target, SC_FREEZE, min(skill_lv * 3 + 35, skill_lv + 60), skill_lv, skill_get_time2(getSkillId(), skill_lv)) && sd)
		clif_skill_fail(*sd, getSkillId());
}

SkillFrostNova::SkillFrostNova() : SkillImpl(WZ_FROSTNOVA) {
}

void SkillFrostNova::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_area_temp[1] = 0;
	map_foreachinshootrange(skill_attack_area, src,
		skill_get_splash(getSkillId(), skill_lv), splash_target(src),
		BF_MAGIC, src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
}

void SkillFrostNova::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	// In renewal the damage formula is identical to MG_FROSTDIVER
	base_skillratio += 10 * skill_lv;
#else
	base_skillratio += -100 + (100 + skill_lv * 10) * 2 / 3;
#endif
}

void SkillFrostNova::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	sc_start(src,target,SC_FREEZE,(sd!=nullptr)?skill_lv*5+33:skill_lv*3+35,skill_lv,skill_get_time2(getSkillId(), skill_lv));
}

SkillFrostyMisty::SkillFrostyMisty() : SkillImpl(WL_FROSTMISTY) {
}

void SkillFrostyMisty::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 200 + 100 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillFrostyMisty::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Causes Freezing status through walls.
	sc_start(src, target, SC_FREEZING, 25 + 5 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
	sc_start(src, target, SC_MISTY_FROST, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	// Doesn't deal damage through non-shootable walls.
	if( !battle_config.skill_wall_check || (battle_config.skill_wall_check && path_search(nullptr,src->m,src->x,src->y,target->x,target->y,1,CELL_CHKWALL)) )
		skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag|SD_ANIMATION);
}

void SkillFrostyMisty::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = 0;

	// Cast center might be relevant later (e.g. for knockback direction)
	skill_area_temp[4] = x;
	skill_area_temp[5] = y;
	i = skill_get_splash(getSkillId(),skill_lv);
	map_foreachinarea(skill_area_sub,src->m,x-i,y-i,x+i,y+i,BL_CHAR|BL_SKILL,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
}

SkillFrozenSlash::SkillFrozenSlash() : SkillImplRecursiveDamageSplash(AG_FROZEN_SLASH) {
}

void SkillFrozenSlash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 450 + 950 * skill_lv + 5 * sstatus->spl;

	if( sc != nullptr && sc->getSCE( SC_CLIMAX ) ){
		skillratio += 150 + 350 * skill_lv;
	}

	RE_LVL_DMOD(100);
}

void SkillFrozenSlash::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillGanbantein::SkillGanbantein() : SkillImpl(HW_GANBANTEIN) {
}

void SkillGanbantein::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (rnd()%100 < 80) {
		int32 dummy = 1;
		clif_skill_poseffect( *src, getSkillId(), skill_lv, x, y, tick );
		bool i = skill_get_splash(getSkillId(), skill_lv);
		map_foreachinallarea(skill_cell_overlap, src->m, x-i, y-i, x+i, y+i, BL_SKILL, getSkillId(), &dummy, src);
	} else {
		if (sd) clif_skill_fail( *sd, getSkillId() );
	
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
}

SkillGoldDigger::SkillGoldDigger() : SkillImpl(SA_FORTUNE) {
}

void SkillGoldDigger::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if(sd) pc_getzeny(sd,status_get_lv(target)*100,LOG_TYPE_STEAL);
}

SkillGravitationField::SkillGravitationField() : SkillImpl(HW_GRAVITATION) {
}

void SkillGravitationField::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	skillratio += -100 + 100 * skill_lv;
	RE_LVL_DMOD(100);
#endif
}

void SkillGravitationField::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#else
	std::shared_ptr<s_skill_unit_group> sg;
	sc_type type = skill_get_sc(getSkillId());

	if ((sg = skill_unitsetting(src,getSkillId(),skill_lv,x,y,0)))
		sc_start4(src,src,type,100,skill_lv,0,BCT_SELF,sg->group_id,skill_get_time(getSkillId(),skill_lv));
	flag|=1;
#endif
}

void SkillGravitationField::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
#ifndef RENEWAL
	// Gravitation can trigger physical autospells
	attack_type |= BF_NORMAL;
	attack_type |= BF_WEAPON;
#endif
}

SkillGravity::SkillGravity() : SkillImpl(SA_GRAVITY) {
}

void SkillGravity::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Skill does nothing. It is only triggered randomly by Hocus Pocus
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillGrimReaper::SkillGrimReaper() : SkillImpl(SA_DEATH) {
}

void SkillGrimReaper::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if ( sd && dstmd && status_has_mode(&dstmd->status,MD_STATUSIMMUNE) ) {
		clif_skill_fail( *sd, getSkillId() );
		return;
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	status_kill(target);
}

SkillHeavensDrive::SkillHeavensDrive() : SkillImpl(WZ_HEAVENDRIVE) {
}

void SkillHeavensDrive::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag|=1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillHeavensDrive::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 25;
#endif
}

void SkillHeavensDrive::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change_end(target, SC_SV_ROOTTWIST);
}

// TODO: refactor to SkillImplRecursiveDamageSplash
SkillHellInferno::SkillHellInferno() : SkillImpl(WL_HELLINFERNO) {
}

void SkillHellInferno::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	if (dmg.miscflag & 2) { // ELE_DARK
		dmg.div_ = -3;
	}
}

void SkillHellInferno::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 400 * skill_lv;
	if (mflag & 2) // ELE_DARK
		skillratio += 200 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillHellInferno::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1) {
		skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
		skill_addtimerskill(src, tick + 300, target->id, 0, 0, getSkillId(), skill_lv, BF_MAGIC, flag | 2);
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
}

void SkillHellInferno::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	if (dmg.miscflag & 2) {
		element = ELE_DARK;
	}
}

SkillHindsight::SkillHindsight() : SkillImpl(SA_AUTOSPELL) {
}

void SkillHindsight::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if (sd) {
		sd->state.workinprogress = WIP_DISABLE_ALL;
		clif_autospell( *sd, skill_lv );
	} else {
		int32 maxlv=1,spellid=0;
		static const int32 spellarray[3] = { MG_COLDBOLT,MG_FIREBOLT,MG_LIGHTNINGBOLT };

		if(skill_lv >= 10) {
			spellid = MG_FROSTDIVER;
//			if (tsc && tsc->getSCE(SC_SPIRIT) && tsc->getSCE(SC_SPIRIT)->val2 == SA_SAGE)
//				maxlv = 10;
//			else
				maxlv = skill_lv - 9;
		}
		else if(skill_lv >=8) {
			spellid = MG_FIREBALL;
			maxlv = skill_lv - 7;
		}
		else if(skill_lv >=5) {
			spellid = MG_SOULSTRIKE;
			maxlv = skill_lv - 4;
		}
		else if(skill_lv >=2) {
			int32 i_rnd = rnd()%3;
			spellid = spellarray[i_rnd];
			maxlv = skill_lv - 1;
		}
		else if(skill_lv > 0) {
			spellid = MG_NAPALMBEAT;
			maxlv = 3;
		}

		if(spellid > 0)
			sc_start4(src,src,SC_AUTOSPELL,100,skill_lv,spellid,maxlv,0,
				skill_get_time(SA_AUTOSPELL,skill_lv));
	}
}

SkillHocusPocus::SkillHocusPocus() : SkillImpl(SA_ABRACADABRA) {
}

void SkillHocusPocus::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (abra_db.empty()) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		return;
	}
	else {
		int32 abra_skill_id = 0, abra_skill_lv;
		size_t checked = 0, checked_max = abra_db.size() * 3;

		do {
			auto abra_spell = abra_db.random();

			abra_skill_id = abra_spell->skill_id;
			abra_skill_lv = min(skill_lv, skill_get_max(abra_skill_id));

			if( rnd() % 10000 < abra_spell->per[max(skill_lv - 1, 0)] ){
				break;
			}
		} while (checked++ < checked_max);

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

		if( sd )
		{// player-casted
			sd->state.abra_flag = 1;
			sd->skillitem = abra_skill_id;
			sd->skillitemlv = abra_skill_lv;
			sd->skillitem_keep_requirement = false;
			clif_item_skill(sd, abra_skill_id, abra_skill_lv);
		}
		else
		{// mob-casted
			struct unit_data *ud = unit_bl2ud(src);
			int32 inf = skill_get_inf(abra_skill_id);
			if (!ud) return;
			if (inf&INF_SELF_SKILL || inf&INF_SUPPORT_SKILL) {
				if (src->type == BL_PET)
					target = (block_list*)((TBL_PET*)src)->master;
				if (!target) target = src;
				unit_skilluse_id(src, target->id, abra_skill_id, abra_skill_lv);
			} else {	//Assume offensive skills
				int32 target_id = 0;
				if (ud->target)
					target_id = ud->target;
				else switch (src->type) {
					case BL_MOB: target_id = ((TBL_MOB*)src)->target_id; break;
					case BL_PET: target_id = ((TBL_PET*)src)->target_id; break;
				}
				if (!target_id)
					return;
				if (skill_get_casttype(abra_skill_id) == CAST_GROUND) {
					target = map_id2bl(target_id);
					if (!target) target = src;
					unit_skilluse_pos(src, target->x, target->y, abra_skill_id, abra_skill_lv);
				} else
					unit_skilluse_id(src, target_id, abra_skill_id, abra_skill_lv);
			}
		}
	}
}

SkillIceWall::SkillIceWall() : SkillImpl(WZ_ICEWALL) {
}

void SkillIceWall::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;
	if(skill_unitsetting(src,getSkillId(),skill_lv,x,y,0))
		clif_skill_poseffect( *src, getSkillId(), skill_lv, x, y, tick );
}

SkillIncreasingActivity::SkillIncreasingActivity() : SkillImpl(EM_INCREASING_ACTIVITY) {
}

void SkillIncreasingActivity::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (target->type == BL_PC) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		status_heal(target, 0, 0, 10 * skill_lv, 0);
	} else if (sd)
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
}

SkillIndulge::SkillIndulge() : SkillImpl(PF_HPCONVERSION) {
}

void SkillIndulge::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const status_data* sstatus = status_get_status_data(*src);
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 hp = sstatus->max_hp / 10;
	int32 sp = hp * skill_lv;

	if (!status_charge(src, hp, 0)) {
		if (sd != nullptr) {
			clif_skill_fail(*sd, getSkillId());
		}
		return;
	}

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	status_heal(target, 0, sp, 2);
}

SkillJackFrost::SkillJackFrost() : SkillImplRecursiveDamageSplash(WL_JACKFROST) {
}

void SkillJackFrost::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *tsc = status_get_sc(target);

	if (tsc && tsc->getSCE(SC_MISTY_FROST))
		skillratio += -100 + 1200 + 600 * skill_lv;
	else
		skillratio += -100 + 1000 + 300 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillJupitelThunder::SkillJupitelThunder() : SkillImpl(WZ_JUPITEL) {
}

void SkillJupitelThunder::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Jupitel Thunder is delayed by 150ms, you can cast another spell before the knockback
	skill_addtimerskill(src, tick + TIMERSKILL_INTERVAL, target->id, 0, 0, getSkillId(), skill_lv, 1, flag);
}

SkillLeveling::SkillLeveling() : SkillImpl(SA_LEVELUP) {
}

void SkillLeveling::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if (sd && pc_nextbaseexp(sd))
		pc_gainexp(sd, nullptr, pc_nextbaseexp(sd) * 10 / 100, 0, 0);
}

SkillLightningBolt::SkillLightningBolt() : SkillImpl(MG_LIGHTNINGBOLT) {
}

void SkillLightningBolt::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	if (sc) {
		if (sc->getSCE(SC_GRACE_BREEZE_OPTION))
			base_skillratio *= 5;

		if (sc->getSCE(SC_SPELLFIST) && mflag & BF_SHORT) {
			base_skillratio += (sc->getSCE(SC_SPELLFIST)->val3 * 100) + (sc->getSCE(SC_SPELLFIST)->val1 * 50 - 50) - 100;
			// val3 = used bolt level, val1 = used spellfist level. [Rytech]
		}
	}
}

void SkillLightningBolt::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillLightningBolt::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr) {
		if (sc->hasSCE(SC_SPELLFIST) && (dmg.miscflag & BF_SHORT)) {
			dmg.div_ = 1; // ad mods, to make it work similar to regular hits [Xazax]
			dmg.flag = BF_WEAPON | BF_SHORT;
			dmg.type = DMG_NORMAL;
		}
	}
}

SkillLightningLand::SkillLightningLand() : SkillImpl(EM_LIGHTNING_LAND) {
}

void SkillLightningLand::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_HANDICAPSTATE_LIGHTNINGSTRIKE, 3, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillLightningLand::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 700 + 1100 * skill_lv;
	skillratio += 5 * sstatus->spl;

	if( sc != nullptr && sc->getSCE( SC_SUMMON_ELEMENTAL_PROCELLA ) ){
		skillratio += 200 * skill_lv;
		skillratio += 2 * sstatus->spl;
	}

	RE_LVL_DMOD(100);
}

void SkillLightningLand::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillLordOfVermilion::SkillLordOfVermilion() : SkillImpl(WZ_VERMILION) {
}

void SkillLordOfVermilion::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src, getSkillId(),skill_lv,x,y,0);
}

void SkillLordOfVermilion::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd)
		base_skillratio += 300 + skill_lv * 100;
	else
		base_skillratio += 20 * skill_lv - 20; //Monsters use old formula
#else
	base_skillratio += 20 * skill_lv - 20;
#endif
}

void SkillLordOfVermilion::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
#ifdef RENEWAL
	sc_start(src,target,SC_BLIND,10 + 5 * skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv));
#else
	sc_start(src,target,SC_BLIND,min(4*skill_lv,40),skill_lv,skill_get_time2(getSkillId(),skill_lv));
#endif
}

SkillMagicRod::SkillMagicRod() : SkillImpl(SA_MAGICROD) {
}

void SkillMagicRod::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

#ifdef RENEWAL
	clif_skill_nodamage(src,*src,SA_MAGICROD,skill_lv);
#endif
	sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

SkillMagneticEarth::SkillMagneticEarth() : SkillImpl(SA_LANDPROTECTOR) {
}

void SkillMagneticEarth::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillMeteorStorm::SkillMeteorStorm() : SkillImpl(WZ_METEOR) {
}

void SkillMeteorStorm::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 area = skill_get_splash(getSkillId(), skill_lv);
	int16 tmpx = 0, tmpy = 0;

	for (int32 i = 1; i <= skill_get_time(getSkillId(), skill_lv) / skill_get_unit_interval(getSkillId()); i++) {
		// Creates a random Cell in the Splash Area
		tmpx = x - area + rnd() % (area * 2 + 1);
		tmpy = y - area + rnd() % (area * 2 + 1);
		skill_unitsetting(src, getSkillId(), skill_lv, tmpx, tmpy, flag + i * skill_get_unit_interval(getSkillId()));
	}
}

void SkillMeteorStorm::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 25;
#endif
}

void SkillMeteorStorm::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_STUN,3*skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

SkillMindBreaker::SkillMindBreaker() : SkillImpl(PF_MINDBREAKER) {
}

void SkillMindBreaker::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const status_data* tstatus = status_get_status_data(*target);
	status_change* tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;
	map_session_data* sd = BL_CAST(BL_PC, src);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if (status_has_mode(tstatus, MD_STATUSIMMUNE) || battle_check_undead(tstatus->race, tstatus->def_ele)) {
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	if (tsce != nullptr)
	{	//HelloKitty2 (?) explained that this silently fails when target is
		//already inflicted. [Skotlex]
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	//Has a 55% + skill_lv*5% success chance.
	if (!clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
			sc_start(src, target, type, 55 + 5 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv)))) {
		if (sd != nullptr) {
			clif_skill_fail(*sd, getSkillId());
		}
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	unit_skillcastcancel(target, 0);

	if (dstmd != nullptr) {
		mob_target(dstmd, src, skill_get_range2(src, getSkillId(), skill_lv, true));
	}
}

SkillMonocell::SkillMonocell() : SkillImpl(SA_MONOCELL) {
}

void SkillMonocell::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );
	int32 i = 0;

	if (dstmd)
	{
		int32 class_;

		if ( sd && status_has_mode(&dstmd->status,MD_STATUSIMMUNE) ) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		class_ = MOBID_PORING;
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		mob_class_change(dstmd,class_);
		if( tsc && status_has_mode(&dstmd->status,MD_STATUSIMMUNE) ) {
			const enum sc_type scs[] = { SC_QUAGMIRE, SC_PROVOKE, SC_ROKISWEIL, SC_GRAVITATION, SC_SUITON, SC_STRIPWEAPON, SC_STRIPSHIELD, SC_STRIPARMOR, SC_STRIPHELM, SC_BLADESTOP };
			for (i = SC_COMMON_MIN; i <= SC_COMMON_MAX; i++)
				if (tsc->getSCE(i)) status_change_end(target, (sc_type)i);
			for (i = 0; i < ARRAYLENGTH(scs); i++)
				if (tsc->getSCE(scs[i])) status_change_end(target, scs[i]);
		}
	}
}

SkillMonsterChant::SkillMonsterChant() : SkillImpl(SA_SUMMONMONSTER) {
}

void SkillMonsterChant::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if (sd)
		mob_once_spawn(sd, src->m, src->x, src->y,"--ja--", -1, 1, "", SZ_SMALL, AI_NONE);
}

SkillMysteryIllusion::SkillMysteryIllusion() : SkillImpl(AG_MYSTERY_ILLUSION) {
}

void SkillMysteryIllusion::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 950 * skill_lv + 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillMysteryIllusion::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillNapalmBeat::SkillNapalmBeat() : SkillImplRecursiveDamageSplash(MG_NAPALMBEAT) {
}

void SkillNapalmBeat::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -30 + 10 * skill_lv;
}

void SkillNapalmBeat::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillNapalmVulcan::SkillNapalmVulcan() : SkillImplRecursiveDamageSplash(HW_NAPALMVULCAN) {
}

void SkillNapalmVulcan::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	skillratio += -100 + 70 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += 25;
#endif
}

void SkillNapalmVulcan::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_CURSE,5*skill_lv,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

SkillPoisonBuster::SkillPoisonBuster() : SkillImplRecursiveDamageSplash(SO_POISON_BUSTER) {
}

void SkillPoisonBuster::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const status_change *tsc = status_get_sc(target);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 1000 + 300 * skill_lv;
	skillratio += sstatus->int_;
	if( tsc && tsc->getSCE(SC_CLOUD_POISON) )
		skillratio += 200 * skill_lv;
	RE_LVL_DMOD(100);
	if( sc && sc->getSCE(SC_CURSED_SOIL_OPTION) )
		skillratio += (sd ? sd->status.job_level * 5 : 0);
}

SkillPsychicStream::SkillPsychicStream() : SkillImplRecursiveDamageSplash(EM_PSYCHIC_STREAM) {
}

void SkillPsychicStream::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1750 + 3850 * skill_lv;
	skillratio += 12 * sstatus->spl;

	RE_LVL_DMOD(100);
}

void SkillPsychicStream::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	uint8 dir = DIR_NORTHEAST;

	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);	// dir based on target as we move player based on target location

	if (skill_check_unit_movepos(0, src, target->x + dirx[dir], target->y + diry[dir], 1, 1)) {
		clif_blown(src);
		skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
	else {
		if (sd != nullptr)
			clif_skill_fail(*sd, getSkillId(), USESKILL_FAIL);

		// TODO: Should we return here?
	}

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillPsychicWave::SkillPsychicWave() : SkillImpl(SO_PSYCHIC_WAVE) {
}

void SkillPsychicWave::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && (sd->weapontype1 == W_STAFF || sd->weapontype1 == W_2HSTAFF || sd->weapontype1 == W_BOOK))
		dmg.div_ = 2;
}

void SkillPsychicWave::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 70 * skill_lv + 3 * sstatus->int_;
	RE_LVL_DMOD(100);
	if (sc && (sc->getSCE(SC_HEATER_OPTION) || sc->getSCE(SC_COOLER_OPTION) || sc->getSCE(SC_BLAST_OPTION) || sc->getSCE(SC_CURSED_SOIL_OPTION)))
		skillratio += 20;
}

void SkillPsychicWave::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillPsychicWave::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
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

SkillQuagmire::SkillQuagmire() : SkillImpl(WZ_QUAGMIRE) {
}

void SkillQuagmire::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag|=1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillQuestioning::SkillQuestioning() : SkillImpl(SA_QUESTION) {
}

void SkillQuestioning::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Skill does nothing. It is only triggered randomly by Hocus Pocus
	clif_emotion( *src, ET_QUESTION );
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillRainOfCrystal::SkillRainOfCrystal() : SkillImpl(AG_RAIN_OF_CRYSTAL) {
}

void SkillRainOfCrystal::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 180 + 760 * skill_lv + 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillRainOfCrystal::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillReadingSpellbook::SkillReadingSpellbook() : SkillImpl(WL_READING_SB_READING) {
}

void SkillReadingSpellbook::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		if (pc_checkskill(sd, WL_READING_SB) == 0 || skill_lv < 1 || skill_lv > 10) {
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_SPELLBOOK_READING );
			return;
		}

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		skill_spellbook(*sd, ITEMID_WL_MB_SG + skill_lv - 1);
	}
}

SkillRejuvenation::SkillRejuvenation() : SkillImpl(SA_FULLRECOVERY) {
}

void SkillRejuvenation::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if (status_isimmune(target))
		return;
	status_percent_heal(target, 100, 100);
}

SkillRelease::SkillRelease() : SkillImpl(WL_RELEASE) {
}

void SkillRelease::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sc == nullptr)
		return;
	if (sd) {
		int32 i;

#ifndef RENEWAL
		skill_toggle_magicpower(src, getSkillId()); // No hit will be amplified
#endif
		if (skill_lv == 1) { // SpellBook
			if (sc->getSCE(SC_FREEZE_SP) == nullptr)
				return;

			bool found_spell = false;

			for (i = SC_MAXSPELLBOOK; i >= SC_SPELLBOOK1; i--) { // List all available spell to be released
				if (sc->getSCE(i) != nullptr) {
					found_spell = true;
					break;
				}
			}

			if (!found_spell)
				return;

			// Now extract the data from the preserved spell
			uint16 pres_skill_id = sc->getSCE(i)->val1;
			uint16 pres_skill_lv = sc->getSCE(i)->val2;
			uint16 point = sc->getSCE(i)->val3;

			status_change_end(src, static_cast<sc_type>(i));

			if( sc->getSCE(SC_FREEZE_SP)->val2 > point )
				sc->getSCE(SC_FREEZE_SP)->val2 -= point;
			else // Last spell to be released
				status_change_end(src, SC_FREEZE_SP);

			if( !skill_check_condition_castbegin(*sd, pres_skill_id, pres_skill_lv) )
				return;

			// Get the requirement for the preserved skill
			skill_consume_requirement(sd, pres_skill_id, pres_skill_lv, 1);

			switch( skill_get_casttype(pres_skill_id) )
			{
				case CAST_GROUND:
					skill_castend_pos2(src, target->x, target->y, pres_skill_id, pres_skill_lv, tick, 0);
					break;
				case CAST_NODAMAGE:
					skill_castend_nodamage_id(src, target, pres_skill_id, pres_skill_lv, tick, 0);
					break;
				case CAST_DAMAGE:
					skill_castend_damage_id(src, target, pres_skill_id, pres_skill_lv, tick, 0);
					break;
			}

			sd->ud.canact_tick = i64max(tick + skill_delayfix(src, pres_skill_id, pres_skill_lv), sd->ud.canact_tick);
			clif_status_change(src, EFST_POSTDELAY, 1, skill_delayfix(src, pres_skill_id, pres_skill_lv), 0, 0, 0);

			int32 cooldown = pc_get_skillcooldown(sd,pres_skill_id, pres_skill_lv);

			if( cooldown > 0 )
				skill_blockpc_start(*sd, pres_skill_id, cooldown);
		} else { // Summoned Balls
			for (i = SC_SPHERE_5; i >= SC_SPHERE_1; i--) {
				if (sc->getSCE(static_cast<sc_type>(i)) == nullptr)
					continue;

				int32 skele = WL_RELEASE - 5 + sc->getSCE(static_cast<sc_type>(i))->val1 - WLS_FIRE; // Convert Ball Element into Skill ATK for balls

				// WL_SUMMON_ATK_FIRE, WL_SUMMON_ATK_WIND, WL_SUMMON_ATK_WATER, WL_SUMMON_ATK_GROUND
				skill_addtimerskill(src, tick + (t_tick)status_get_adelay(src) * abs(i - SC_SPHERE_1), target->id, 0, 0, skele, sc->getSCE(static_cast<sc_type>(i))->val2, BF_MAGIC, flag | SD_LEVEL);
				status_change_end(src, static_cast<sc_type>(i)); // Eliminate ball
			}
			clif_skill_nodamage(src, *target, getSkillId(), 0);
		}
	}
}

SkillRockDown::SkillRockDown() : SkillImplRecursiveDamageSplash(AG_ROCK_DOWN) {
}

void SkillRockDown::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 1550 * skill_lv + 5 * sstatus->spl;

	if( sc != nullptr && sc->getSCE( SC_CLIMAX ) ){
		skillratio += 300 * skill_lv;
	}

	RE_LVL_DMOD(100);
}

void SkillRockDown::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillSafetyWall::SkillSafetyWall() : SkillImpl(MG_SAFETYWALL) {
}

void SkillSafetyWall::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 dummy = 1;

	if (map_foreachincell(skill_cell_overlap, src->m, x, y, BL_SKILL, getSkillId(), &dummy, src)) {
		skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
		// Don't consume gems if cast on Land Protector
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillSense::SkillSense() : SkillImpl(WZ_ESTIMATION) {
}

void SkillSense::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if( sd == nullptr )
		return;
	if( dstsd )
	{ // Fail on Players
		clif_skill_fail( *sd, getSkillId() );
		return;
	}

	if (dstmd != nullptr)
		clif_skill_estimation( *sd, *dstmd );

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillSiennaExecrate::SkillSiennaExecrate() : SkillImpl(WL_SIENNAEXECRATE) {
}

void SkillSiennaExecrate::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_change *tsc = status_get_sc(target);

	if( status_isimmune(target) || !tsc )
		return;

	if( flag&1 ) {
		if( target->id == skill_area_temp[1] )
			return; // Already work on this target

		status_change_start(src,target,type,10000,skill_lv,src->id,0,0,skill_get_time2(getSkillId(),skill_lv), SCSTART_NOTICKDEF, skill_get_time(getSkillId(), skill_lv));
	} else {
		int32 rate = 45 + 5 * skill_lv + ( sd? sd->status.job_level : 50 ) / 4;
		// IroWiki says Rate should be reduced by target stats, but currently unknown
		if( rnd()%100 < rate ) { // Success on First Target
			if( status_change_start(src,target,type,10000,skill_lv,src->id,0,0,skill_get_time2(getSkillId(),skill_lv), SCSTART_NOTICKDEF, skill_get_time(getSkillId(), skill_lv)) ) {
				clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
				skill_area_temp[1] = target->id;
				map_foreachinallrange(skill_area_sub,target,skill_get_splash(getSkillId(),skill_lv),BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_nodamage_id);
			}
			// Doesn't send failure packet if it fails on defense.
		}
		else if( sd ) // Failure on Rate
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillSight::SkillSight() : SkillImpl(MG_SIGHT) {
}

void SkillSight::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
	                    sc_start2(src, target, type, 100, skill_lv, getSkillId(), skill_get_time(getSkillId(), skill_lv)));
}

SkillSightBlaster::SkillSightBlaster() : SkillImpl(WZ_SIGHTBLASTER) {
}

void SkillSightBlaster::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start2(src,target,skill_get_sc(getSkillId()),100,skill_lv,getSkillId(),skill_get_time(getSkillId(),skill_lv)));
}

void SkillSightBlaster::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillSightBlaster::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 500;
#endif
}

SkillSightRasher::SkillSightRasher() : SkillImpl(WZ_SIGHTRASHER) {
}

void SkillSightRasher::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Passive side of the attack.
	status_change_end(src, SC_SIGHT);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	map_foreachinshootrange(skill_area_sub,src,
		skill_get_splash(getSkillId(), skill_lv),BL_CHAR|BL_SKILL,
		src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|SD_ANIMATION|1,
		skill_castend_damage_id);
}

void SkillSightRasher::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillSightRasher::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 20 * skill_lv;
}

SkillSoulExhale::SkillSoulExhale() : SkillImpl(PF_SOULCHANGE) {
}

void SkillSoulExhale::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	status_change* tsc = status_get_sc(target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	map_session_data* sd = BL_CAST(BL_PC, src);
	uint32 sp1 = 0, sp2 = 0;

	if (dstmd != nullptr) {
		if (dstmd->state.soul_change_flag) {
			if (sd != nullptr) {
				clif_skill_fail(*sd, getSkillId());
			}
			return;
		}

		dstmd->state.soul_change_flag = 1;
		sp2 = sstatus->max_sp * 3 / 100;
		status_heal(src, 0, sp2, 2);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		return;
	}

	sp1 = sstatus->sp;
	sp2 = tstatus->sp;
#ifdef RENEWAL
	sp1 /= 2;
	sp2 /= 2;
	if (tsc != nullptr && tsc->hasSCE(SC_EXTREMITYFIST)) {
		sp1 = tstatus->sp;
	}
#endif
	if (tsc != nullptr && tsc->hasSCE(SC_NORECOVER_STATE)) {
		sp1 = tstatus->sp;
	}

	status_set_sp(src, sp2, 3);
	status_set_sp(target, sp1, 3);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillSoulExpansion::SkillSoulExpansion() : SkillImplRecursiveDamageSplash(WL_SOULEXPANSION) {
}

void SkillSoulExpansion::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1000 + skill_lv * 200;
	skillratio += sstatus->int_;
	RE_LVL_DMOD(100);
}

SkillSoulSiphon::SkillSoulSiphon() : SkillImpl(PF_SOULBURN) {
}

void SkillSoulSiphon::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (rnd() % 100 < (skill_lv < 5 ? 30 + skill_lv * 10 : 70)) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		if (skill_lv == 5) {
			skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
		}
		status_percent_damage(src, target, 0, 100, false);
	} else {
		clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
		if (skill_lv == 5) {
			skill_attack(BF_MAGIC, src, src, src, getSkillId(), skill_lv, tick, flag);
		}
		status_percent_damage(src, src, 0, 100, false);
	}
}

SkillSoulStrike::SkillSoulStrike() : SkillImpl(MG_SOULSTRIKE) {
}

void SkillSoulStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_data *tstatus = status_get_status_data(*target);

	if (battle_check_undead(tstatus->race, tstatus->def_ele))
		base_skillratio += 5 * skill_lv;
}

void SkillSoulStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillSoulVulcanStrike::SkillSoulVulcanStrike() : SkillImplRecursiveDamageSplash(AG_SOUL_VC_STRIKE) {
}

void SkillSoulVulcanStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 50 + 330 * skill_lv;
	skillratio += 3 * sstatus->spl;

	RE_LVL_DMOD(100);
}

SkillSpellBreaker::SkillSpellBreaker() : SkillImpl(SA_SPELLBREAKER) {
}

void SkillSpellBreaker::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );

	int32 sp;
	if (dstsd && tsc && tsc->getSCE(SC_MAGICROD)) {
		// If target enemy player has Magic Rod, then 20% of your SP is transferred to that player
		sp = status_percent_damage(target, src, 0, -20, false);
		status_heal(target, 0, sp, 2);
	}
	else {
		struct unit_data* ud = unit_bl2ud(target);
		if (!ud || ud->skilltimer == INVALID_TIMER)
			return; //Nothing to cancel.
		int32 hp = 0;
		if (status_has_mode(tstatus, MD_STATUSIMMUNE)) { //Only 10% success chance against status immune. [Skotlex]
			if (rnd_chance(90, 100))
			{
				if (sd) clif_skill_fail( *sd, getSkillId() );
				return;
			}
		}
#ifdef RENEWAL
		else // HP damage does not work on bosses in renewal
#endif
			if (skill_lv >= 5 && (!dstsd || map_flag_vs(target->m))) //HP damage only on pvp-maps when against players.
				hp = tstatus->max_hp / 50; //Siphon 2% HP at level 5

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		unit_skillcastcancel(target, 0);
		sp = skill_get_sp(ud->skill_id, ud->skill_lv);
		status_zap(target, 0, sp);
		// Recover some of the SP used
		status_heal(src, 0, sp * (25 * (skill_lv - 1)) / 100, 2);

		// If damage would be lethal, it does not deal damage
		if (hp && hp < tstatus->hp) {
			clif_damage(*src, *target, tick, 0, 0, hp, 0, DMG_NORMAL, 0, false);
			status_zap(target, hp, 0);
			// Recover 50% of damage dealt
			status_heal(src, hp / 2, 0, 2);
		}
	}
}

SkillSpellFist::SkillSpellFist() : SkillImpl(SO_SPELLFIST) {
}

void SkillSpellFist::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	unit_skillcastcancel(src, 1);

	if (sd) {
		if (sd->skill_id_old != 0 && sd->skill_lv_old != 0) {
			sc_start4(src, src, skill_get_sc(getSkillId()), 100, skill_lv, sd->skill_id_old, sd->skill_lv_old, 0, skill_get_time(getSkillId(), skill_lv));
		}
		sd->skill_id_old = sd->skill_lv_old = 0;
	}
}

SkillSpiritControl::SkillSpiritControl() : SkillImpl(SO_EL_CONTROL) {
}

void SkillSpiritControl::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int32 mode;

		if( !sd->ed )
			return;

		if( skill_lv == 4 ) {// At level 4 delete elementals.
			elemental_delete(sd->ed);
			return;
		}
		switch( skill_lv ) {// Select mode bassed on skill level used.
			case 1: mode = EL_MODE_PASSIVE; break; // Standard mode.
			case 2: mode = EL_MODE_ASSIST; break;
			case 3: mode = EL_MODE_AGGRESSIVE; break;
		}
		if( !elemental_change_mode(sd->ed,mode) ) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillSpiritRecovery::SkillSpiritRecovery() : SkillImpl(SO_EL_CURE) {
}

void SkillSpiritRecovery::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		s_elemental_data *ed = sd->ed;

		if( !ed )
			return;

		int32 s_hp = sd->battle_status.hp * 10 / 100;
		int32 s_sp = sd->battle_status.sp * 10 / 100;

		if( !status_charge(sd,s_hp,s_sp) ) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}

		status_heal(ed,s_hp,s_sp,3);
		clif_skill_nodamage(src,*ed,getSkillId(),skill_lv);
	}
}

SkillStasis::SkillStasis() : SkillImpl(WL_STASIS) {
}

void SkillStasis::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	if (flag&1)
		sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
	else {
		struct map_data *mapdata = map_getmapdata(src->m);

		map_foreachinallrange(skill_area_sub,src,skill_get_splash(getSkillId(), skill_lv),BL_CHAR,src,getSkillId(),skill_lv,tick,(mapdata_flag_vs(mapdata)?BCT_ALL:BCT_ENEMY|BCT_SELF)|flag|1,skill_castend_nodamage_id);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillStoneCurse::SkillStoneCurse() : SkillImpl(MG_STONECURSE) {
}

void SkillStoneCurse::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	status_data *tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(&*target);
	sc_type type = skill_get_sc(getSkillId());

	if (status_has_mode(tstatus, MD_STATUSIMMUNE)) {
		if (sd)
			clif_skill_fail(*sd, getSkillId());
		return;
	}

	if (status_isimmune(target) || !tsc)
		return;

	int32 brate = 0;

	if (sd && sd->sc.getSCE(SC_PETROLOGY_OPTION))
		brate = sd->sc.getSCE(SC_PETROLOGY_OPTION)->val3;

	// Except for players, the skill animation shows even if the status change doesn't start
	// Players get a skill has failed message instead
	if (sc_start2(src, target, type, (skill_lv * 4 + 20) + brate, skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv), skill_get_time(getSkillId(), skill_lv)) || sd == nullptr)
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	else {
		clif_skill_fail( *sd, getSkillId() );
		// Level 6-10 doesn't consume a red gem if it fails [celest]
		if (skill_lv > 5)
		{ // not to consume items
			flag |= SKILL_NOCONSUME_REQ;
		}
	}
}

SkillStormCannon::SkillStormCannon() : SkillImpl(AG_STORM_CANNON) {
}

void SkillStormCannon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 1550 * skill_lv + 5 * sstatus->spl;

	if( sc != nullptr && sc->getSCE( SC_CLIMAX ) ){
		skillratio += 300 * skill_lv;
	}

	RE_LVL_DMOD(100);
}

void SkillStormCannon::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = target->id;
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
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

SkillStormGust::SkillStormGust() : SkillImpl(WZ_STORMGUST) {
}

void SkillStormGust::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillStormGust::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio -= 30; // Offset only once
	base_skillratio += 50 * skill_lv;
#else
	base_skillratio += 40 * skill_lv;
#endif
}

void SkillStormGust::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Storm Gust counter was dropped in renewal
#ifdef RENEWAL
	sc_start(src,target,SC_FREEZE,65-(5*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
#else
	status_change* tsc = status_get_sc( target );

	if (tsc != nullptr) {
		//On third hit, there is a 150% to freeze the target
		if(tsc->sg_counter >= 3 &&
			sc_start(src,target,SC_FREEZE,150,skill_lv,skill_get_time2(getSkillId(),skill_lv)))
			tsc->sg_counter = 0;
		// Being it only resets on success it'd keep stacking and eventually overflowing on mvps, so we reset at a high value
		else if( tsc->sg_counter > 250 )
			tsc->sg_counter = 0;
	}
#endif
}

SkillStrantumTremor::SkillStrantumTremor() : SkillImpl(AG_STRANTUM_TREMOR) {
}

void SkillStrantumTremor::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 100 + 730 * skill_lv + 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillStrantumTremor::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillStriking::SkillStriking() : SkillImpl(SO_STRIKING) {
}

void SkillStriking::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (battle_check_target(src, target, BCT_SELF|BCT_PARTY) > 0) {
		int32 bonus = 0;

		if (dstsd) {
			int16 index = dstsd->equip_index[EQI_HAND_R];

			if (index >= 0 && dstsd->inventory_data[index] && dstsd->inventory_data[index]->type == IT_WEAPON)
				bonus = (20 * skill_lv) * dstsd->inventory_data[index]->weapon_level;
		}

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start2(src,target, type, 100, skill_lv, bonus, skill_get_time(getSkillId(), skill_lv)));
	} else if (sd)
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_TOTARGET );
}

SkillSuicide::SkillSuicide() : SkillImpl(SA_INSTANTDEATH) {
}

void SkillSuicide::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	status_kill(src);
}

SkillSummonEarthSpiritTera::SkillSummonEarthSpiritTera() : SkillImpl(SO_SUMMON_TERA) {
}

void SkillSummonEarthSpiritTera::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int32 elemental_class = skill_get_elemental_type(getSkillId(),skill_lv);

		// Remove previous elemental first.
		if( sd->ed )
			elemental_delete(sd->ed);

		// Summoning the new one.
		if( !elemental_create(sd,elemental_class,skill_get_time(getSkillId(),skill_lv)) ) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillSummonElementalArdor::SkillSummonElementalArdor() : SkillImpl(EM_SUMMON_ELEMENTAL_ARDOR) {
}

void SkillSummonElementalArdor::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if (sd == nullptr)
		return;

	if (sd->ed && sd->ed->elemental.class_ == ELEMENTALID_AGNI_L) {
		// Remove the old elemental before summoning the super one.
		elemental_delete(sd->ed);

		if (!elemental_create(sd, ELEMENTALID_ARDOR, skill_get_time(getSkillId(), skill_lv))) {
			clif_skill_fail( *sd, getSkillId() );
		} else // Elemental summoned. Buff the player with the bonus.
			sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		clif_skill_fail( *sd, getSkillId() );
	}
}

SkillSummonElementalDiluvio::SkillSummonElementalDiluvio() : SkillImpl(EM_SUMMON_ELEMENTAL_DILUVIO) {
}

void SkillSummonElementalDiluvio::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if (sd == nullptr)
		return;

	if (sd->ed && sd->ed->elemental.class_ == ELEMENTALID_AQUA_L) {
		// Remove the old elemental before summoning the super one.
		elemental_delete(sd->ed);

		if (!elemental_create(sd, ELEMENTALID_DILUVIO, skill_get_time(getSkillId(), skill_lv))) {
			clif_skill_fail( *sd, getSkillId() );
		} else // Elemental summoned. Buff the player with the bonus.
			sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		clif_skill_fail( *sd, getSkillId() );
	}
}

SkillSummonElementalProcella::SkillSummonElementalProcella() : SkillImpl(EM_SUMMON_ELEMENTAL_PROCELLA) {
}

void SkillSummonElementalProcella::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if (sd == nullptr)
		return;

	if (sd->ed && sd->ed->elemental.class_ == ELEMENTALID_VENTUS_L) {
		// Remove the old elemental before summoning the super one.
		elemental_delete(sd->ed);

		if (!elemental_create(sd, ELEMENTALID_PROCELLA, skill_get_time(getSkillId(), skill_lv))) {
			clif_skill_fail( *sd, getSkillId() );
		} else // Elemental summoned. Buff the player with the bonus.
			sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		clif_skill_fail( *sd, getSkillId() );
	}
}

SkillSummonElementalSerpens::SkillSummonElementalSerpens() : SkillImpl(EM_SUMMON_ELEMENTAL_SERPENS) {
}

void SkillSummonElementalSerpens::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if (sd == nullptr)
		return;

	if (sd->ed && (sd->ed->elemental.class_ == ELEMENTALID_AGNI_L || sd->ed->elemental.class_ == ELEMENTALID_AQUA_L ||
				sd->ed->elemental.class_ == ELEMENTALID_VENTUS_L || sd->ed->elemental.class_ == ELEMENTALID_TERA_L)) {
		// Remove the old elemental before summoning the super one.
		elemental_delete(sd->ed);

		if (!elemental_create(sd, ELEMENTALID_SERPENS, skill_get_time(getSkillId(), skill_lv))) {
			clif_skill_fail( *sd, getSkillId() );
		} else // Elemental summoned. Buff the player with the bonus.
			sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		clif_skill_fail( *sd, getSkillId() );
	}
}

SkillSummonElementalTerremotus::SkillSummonElementalTerremotus() : SkillImpl(EM_SUMMON_ELEMENTAL_TERREMOTUS) {
}

void SkillSummonElementalTerremotus::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if (sd == nullptr)
		return;

	if (sd->ed && sd->ed->elemental.class_ == ELEMENTALID_TERA_L) {
		// Remove the old elemental before summoning the super one.
		elemental_delete(sd->ed);

		if (!elemental_create(sd, ELEMENTALID_TERREMOTUS, skill_get_time(getSkillId(), skill_lv))) {
			clif_skill_fail( *sd, getSkillId() );
		} else // Elemental summoned. Buff the player with the bonus.
			sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		clif_skill_fail( *sd, getSkillId() );
	}
}

// WL_SUMMONFB
SkillSummonFireBall::SkillSummonFireBall() : SkillImpl(WL_SUMMONFB) {
}

void SkillSummonFireBall::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 i = 0;

	if (sc == nullptr)
		return;

	// Set val2. The SC element for this ball
	e_wl_spheres element = WLS_FIRE;

	if (skill_lv == 1) {
		sc_type sphere = SC_NONE;

		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++) {
			if (sc->getSCE(i) == nullptr) {
				sphere = static_cast<sc_type>(i); // Take the free SC
				break;
			}
		}

		if (sphere == SC_NONE) {
			if (sd) // No free slots to put SC
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_SUMMON );
			return;
		}

		sc_start2(src, src, sphere, 100, element, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++) {
			status_change_end(src, static_cast<sc_type>(i)); // Removes previous type
			sc_start2(src, src, static_cast<sc_type>(i), 100, element, skill_lv, skill_get_time(getSkillId(), skill_lv));
		}
	}

	clif_skill_nodamage(src, *target, getSkillId(), 0, false);
}


// WL_SUMMON_ATK_FIRE
SkillSummonAttackFire::SkillSummonAttackFire() : SkillImpl(WL_SUMMON_ATK_FIRE) {
}

void SkillSummonAttackFire::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += 200;
	RE_LVL_DMOD(100);
}

SkillSummonFireSpiritAgni::SkillSummonFireSpiritAgni() : SkillImpl(SO_SUMMON_AGNI) {
}

void SkillSummonFireSpiritAgni::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int32 elemental_class = skill_get_elemental_type(getSkillId(),skill_lv);

		// Remove previous elemental first.
		if( sd->ed )
			elemental_delete(sd->ed);

		// Summoning the new one.
		if( !elemental_create(sd,elemental_class,skill_get_time(getSkillId(),skill_lv)) ) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

// WL_SUMMONBL
SkillSummonLightningBall::SkillSummonLightningBall() : SkillImpl(WL_SUMMONBL) {
}

void SkillSummonLightningBall::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 i = 0;

	if (sc == nullptr)
		return;

	// Set val2. The SC element for this ball
	e_wl_spheres element = WLS_WIND;

	if (skill_lv == 1) {
		sc_type sphere = SC_NONE;

		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++) {
			if (sc->getSCE(i) == nullptr) {
				sphere = static_cast<sc_type>(i); // Take the free SC
				break;
			}
		}

		if (sphere == SC_NONE) {
			if (sd) // No free slots to put SC
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_SUMMON );
			return;
		}

		sc_start2(src, src, sphere, 100, element, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++) {
			status_change_end(src, static_cast<sc_type>(i)); // Removes previous type
			sc_start2(src, src, static_cast<sc_type>(i), 100, element, skill_lv, skill_get_time(getSkillId(), skill_lv));
		}
	}

	clif_skill_nodamage(src, *target, getSkillId(), 0, false);
}


// WL_SUMMON_ATK_WIND
SkillSummonAttackWind::SkillSummonAttackWind() : SkillImpl(WL_SUMMON_ATK_WIND) {
}

void SkillSummonAttackWind::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += 200;
	RE_LVL_DMOD(100);
}

// WL_SUMMONSTONE
SkillSummonStone::SkillSummonStone() : SkillImpl(WL_SUMMONSTONE) {
}

void SkillSummonStone::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 i = 0;

	if (sc == nullptr)
		return;

	// Set val2. The SC element for this ball
	e_wl_spheres element = WLS_STONE;

	if (skill_lv == 1) {
		sc_type sphere = SC_NONE;

		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++) {
			if (sc->getSCE(i) == nullptr) {
				sphere = static_cast<sc_type>(i); // Take the free SC
				break;
			}
		}

		if (sphere == SC_NONE) {
			if (sd) // No free slots to put SC
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_SUMMON );
			return;
		}

		sc_start2(src, src, sphere, 100, element, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++) {
			status_change_end(src, static_cast<sc_type>(i)); // Removes previous type
			sc_start2(src, src, static_cast<sc_type>(i), 100, element, skill_lv, skill_get_time(getSkillId(), skill_lv));
		}
	}

	clif_skill_nodamage(src, *target, getSkillId(), 0, false);
}


// WL_SUMMON_ATK_GROUND
SkillSummonAttackEarth::SkillSummonAttackEarth() : SkillImpl(WL_SUMMON_ATK_GROUND) {
}

void SkillSummonAttackEarth::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += 200;
	RE_LVL_DMOD(100);
}

// WL_SUMMONWB
SkillSummonWaterBall::SkillSummonWaterBall() : SkillImpl(WL_SUMMONWB) {
}

void SkillSummonWaterBall::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 i = 0;

	if (sc == nullptr)
		return;

	// Set val2. The SC element for this ball
	e_wl_spheres element = WLS_WATER;

	if (skill_lv == 1) {
		sc_type sphere = SC_NONE;

		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++) {
			if (sc->getSCE(i) == nullptr) {
				sphere = static_cast<sc_type>(i); // Take the free SC
				break;
			}
		}

		if (sphere == SC_NONE) {
			if (sd) // No free slots to put SC
				clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_SUMMON );
			return;
		}

		sc_start2(src, src, sphere, 100, element, skill_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		for (i = SC_SPHERE_1; i <= SC_SPHERE_5; i++) {
			status_change_end(src, static_cast<sc_type>(i)); // Removes previous type
			sc_start2(src, src, static_cast<sc_type>(i), 100, element, skill_lv, skill_get_time(getSkillId(), skill_lv));
		}
	}

	clif_skill_nodamage(src, *target, getSkillId(), 0, false);
}


// WL_SUMMON_ATK_WATER
SkillSummonAttackWater::SkillSummonAttackWater() : SkillImpl(WL_SUMMON_ATK_WATER) {
}

void SkillSummonAttackWater::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += 200;
	RE_LVL_DMOD(100);
}

SkillSummonWaterSpiritAqua::SkillSummonWaterSpiritAqua() : SkillImpl(SO_SUMMON_AQUA) {
}

void SkillSummonWaterSpiritAqua::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int32 elemental_class = skill_get_elemental_type(getSkillId(),skill_lv);

		// Remove previous elemental first.
		if( sd->ed )
			elemental_delete(sd->ed);

		// Summoning the new one.
		if( !elemental_create(sd,elemental_class,skill_get_time(getSkillId(),skill_lv)) ) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillSummonWindSpiritVentus::SkillSummonWindSpiritVentus() : SkillImpl(SO_SUMMON_VENTUS) {
}

void SkillSummonWindSpiritVentus::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int32 elemental_class = skill_get_elemental_type(getSkillId(),skill_lv);

		// Remove previous elemental first.
		if( sd->ed )
			elemental_delete(sd->ed);

		// Summoning the new one.
		if( !elemental_create(sd,elemental_class,skill_get_time(getSkillId(),skill_lv)) ) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillTerraDrive::SkillTerraDrive() : SkillImpl(EM_TERRA_DRIVE) {
}

void SkillTerraDrive::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_HANDICAPSTATE_CRYSTALLIZATION, 5, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillTerraDrive::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	if (const status_change *sc = status_get_sc(src); sc != nullptr && sc->hasSCE(SC_SUMMON_ELEMENTAL_TERREMOTUS)) {
		skillratio += -100 + 8100 + 2700 * skill_lv;
		skillratio += 10 * sstatus->spl;
	}
	else {
		skillratio += -100 + 600 + 3000 * skill_lv;
		skillratio += 7 * sstatus->spl;
	}

	RE_LVL_DMOD(100);
}

void SkillTerraDrive::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

// WL_TETRAVORTEX
SkillTetraVortex::SkillTetraVortex() : SkillImpl(WL_TETRAVORTEX) {
}

void SkillTetraVortex::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr) { // Monster usage
		uint8 i = 0;
		const static std::vector<std::vector<uint16>> tetra_skills = { { WL_TETRAVORTEX_FIRE, 1 },
																	   { WL_TETRAVORTEX_WIND, 4 },
																	   { WL_TETRAVORTEX_WATER, 2 },
																	   { WL_TETRAVORTEX_GROUND, 8 } };

		for (const auto &skill : tetra_skills) {
			if (skill_lv > 5) {
				skill_area_temp[0] = i;
				skill_area_temp[1] = skill[1];
				map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, skill[0], skill_lv, tick, flag | BCT_ENEMY, skill_castend_damage_id);
			} else
				skill_addtimerskill(src, tick + i * 200, target->id, skill[1], 0, skill[0], skill_lv, i, flag);
			i++;
		}
	} else if (sc) { // No SC? No spheres
		int32 i, k = 0;

		if (sc->getSCE(SC_SPHERE_5)) // If 5 spheres, remove last one (based on reverse order) and only do 4 actions (Official behavior)
			status_change_end(src, SC_SPHERE_1);

		for (i = SC_SPHERE_5; i >= SC_SPHERE_1; i--) { // Loop should always be 4 for regular players, but unconditional_skill could be less
			if (sc->getSCE(static_cast<sc_type>(i)) == nullptr)
				continue;

			uint16 subskill = 0;

			switch (sc->getSCE(static_cast<sc_type>(i))->val1) {
				case WLS_FIRE:
					subskill = WL_TETRAVORTEX_FIRE;
					k |= 1;
					break;
				case WLS_WIND:
					subskill = WL_TETRAVORTEX_WIND;
					k |= 4;
					break;
				case WLS_WATER:
					subskill = WL_TETRAVORTEX_WATER;
					k |= 2;
					break;
				case WLS_STONE:
					subskill = WL_TETRAVORTEX_GROUND;
					k |= 8;
					break;
			}

			if (skill_lv > 5) {
				skill_area_temp[0] = abs(i - SC_SPHERE_5);
				skill_area_temp[1] = k;
				map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, subskill, skill_lv, tick, flag | BCT_ENEMY, skill_castend_damage_id);
			} else
				skill_addtimerskill(src, tick + abs(i - SC_SPHERE_5) * 200, target->id, k, 0, subskill, skill_lv, abs(i - SC_SPHERE_5), flag);
			status_change_end(src, static_cast<sc_type>(i));
		}
	}
}


// WL_TETRAVORTEX_GROUND
SkillTetraVortexEarth::SkillTetraVortexEarth() : SkillImpl(WL_TETRAVORTEX_GROUND) {
}

void SkillTetraVortexEarth::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 800 + 400 * skill_lv;
}

void SkillTetraVortexEarth::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_addtimerskill(src, tick + skill_area_temp[0] * 200, target->id, skill_area_temp[1], 0, getSkillId(), skill_lv, 0, flag);
}


// WL_TETRAVORTEX_FIRE
SkillTetraVortexFire::SkillTetraVortexFire() : SkillImpl(WL_TETRAVORTEX_FIRE) {
}

void SkillTetraVortexFire::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 800 + 400 * skill_lv;
}

void SkillTetraVortexFire::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_addtimerskill(src, tick + skill_area_temp[0] * 200, target->id, skill_area_temp[1], 0, getSkillId(), skill_lv, 0, flag);
}


// WL_TETRAVORTEX_WATER
SkillTetraVortexWater::SkillTetraVortexWater() : SkillImpl(WL_TETRAVORTEX_WATER) {
}

void SkillTetraVortexWater::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 800 + 400 * skill_lv;
}

void SkillTetraVortexWater::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_addtimerskill(src, tick + skill_area_temp[0] * 200, target->id, skill_area_temp[1], 0, getSkillId(), skill_lv, 0, flag);
}


// WL_TETRAVORTEX_WIND
SkillTetraVortexWind::SkillTetraVortexWind() : SkillImpl(WL_TETRAVORTEX_WIND) {
}

void SkillTetraVortexWind::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 800 + 400 * skill_lv;
}

void SkillTetraVortexWind::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_addtimerskill(src, tick + skill_area_temp[0] * 200, target->id, skill_area_temp[1], 0, getSkillId(), skill_lv, 0, flag);
}

SkillThunderStorm::SkillThunderStorm() : SkillImpl(MG_THUNDERSTORM) {
}

void SkillThunderStorm::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillThunderStorm::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	// in Renewal Thunder Storm boost is 100% (in pre-re, 80%)
#ifndef RENEWAL
	base_skillratio -= 20;
#endif
}

SkillTornadoStorm::SkillTornadoStorm() : SkillImpl(AG_TORNADO_STORM) {
}

void SkillTornadoStorm::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 100 + 760 * skill_lv + 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillTornadoStorm::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillVacuumExtreme::SkillVacuumExtreme() : SkillImpl(SO_VACUUM_EXTREME) {
}

void SkillVacuumExtreme::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillVaretyrSpear::SkillVaretyrSpear() : SkillImplRecursiveDamageSplash(SO_VARETYR_SPEAR) {
}

void SkillVaretyrSpear::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target, SC_STUN, 5 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillVaretyrSpear::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + (2 * sstatus->int_ + 150 * (pc_checkskill(sd, SO_STRIKING) + pc_checkskill(sd, SA_LIGHTNINGLOADER)) + sstatus->int_ * skill_lv / 2) / 3;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_BLAST_OPTION))
		skillratio += (sd ? sd->status.job_level * 5 : 0);
}

SkillVenomSwamp::SkillVenomSwamp() : SkillImpl(EM_VENOM_SWAMP) {
}

void SkillVenomSwamp::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_HANDICAPSTATE_DEADLYPOISON, 3, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillVenomSwamp::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 700 + 1100 * skill_lv;
	skillratio += 5 * sstatus->spl;

	if( sc && sc->getSCE( SC_SUMMON_ELEMENTAL_SERPENS ) ){
		skillratio += 200 * skill_lv;
		skillratio += 2 * sstatus->spl;
	}

	RE_LVL_DMOD(100);
}

void SkillVenomSwamp::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

// AG_VIOLENT_QUAKE
SkillViolentQuake::SkillViolentQuake() : SkillImpl(AG_VIOLENT_QUAKE) {
}

void SkillViolentQuake::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	sc_start(src, target, type, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillViolentQuake::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);

	int32 area = skill_get_splash(getSkillId(), skill_lv);
	int32 unit_time = skill_get_time(getSkillId(), skill_lv);
	int32 unit_interval = skill_get_unit_interval(getSkillId());
	uint16 tmpx = 0, tmpy = 0, climax_lv = 0;
	int32 i = 0;

	// Grab Climax's effect level if active.
	if (sc && sc->getSCE(SC_CLIMAX))
		climax_lv = sc->getSCE(SC_CLIMAX)->val1;

	// Fixes rising rocks spawn area to 7x7.
	if (climax_lv == 5)
		area = 3;

	// Displays the earthquake.
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);

	if (climax_lv == 4) { // Deals no damage and instead inflicts a status on the enemys in range.
		i = skill_get_splash(getSkillId(), skill_lv);
		map_foreachinallarea(skill_area_sub, src->m, x - i, y - i, x + i, y + i, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	} else for (i = 1; i <= unit_time / unit_interval; i++) { // Spawn the rising rocks on random spots at separate intervals
		tmpx = x - area + rnd() % (area * 2 + 1);
		tmpy = y - area + rnd() % (area * 2 + 1);
		skill_unitsetting(src, AG_VIOLENT_QUAKE_ATK, skill_lv, tmpx, tmpy, flag + i * unit_interval);

		if (climax_lv == 1) { // Spwan a 2nd rising rock along with the 1st one.
			tmpx = x - area + rnd() % (area * 2 + 1);
			tmpy = y - area + rnd() % (area * 2 + 1);
			skill_unitsetting(src, AG_VIOLENT_QUAKE_ATK, skill_lv, tmpx, tmpy, flag + i * unit_interval);
		}
	}
}


// AG_VIOLENT_QUAKE_ATK
SkillViolentQuakeAttack::SkillViolentQuakeAttack() : SkillImpl(AG_VIOLENT_QUAKE_ATK) {
}

void SkillViolentQuakeAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 200 + 1200 * skill_lv + 5 * sstatus->spl;
	// (climax buff applied with pc_skillatk_bonus)
	RE_LVL_DMOD(100);
}

SkillVolcano::SkillVolcano() : SkillImpl(SA_VOLCANO) {
}

void SkillVolcano::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Does not consumes if the skill is already active. [Skotlex]
	std::shared_ptr<s_skill_unit_group> sg2;
	if ((sg2= skill_locate_element_field(src)) != nullptr && ( sg2->skill_id == SA_VOLCANO || sg2->skill_id == SA_DELUGE || sg2->skill_id == SA_VIOLENTGALE ))
	{
		if (sg2->limit - DIFF_TICK(gettick(), sg2->tick) > 0)
		{
			skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
			flag |= SKILL_NOCONSUME_REQ; // not to consume items
			return;
		}
		else
			sg2->limit = 0; //Disable it.
	}
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillWarmer::SkillWarmer() : SkillImpl(SO_WARMER) {
}

void SkillWarmer::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= 8;
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillWaterBall::SkillWaterBall() : SkillImpl(WZ_WATERBALL) {
}

void SkillWaterBall::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Deploy waterball cells, these are used and turned into waterballs via the timerskill
	skill_unitsetting(src, getSkillId(), skill_lv, src->x, src->y, 0);
	skill_addtimerskill(src, tick, target->id, src->x, src->y, getSkillId(), skill_lv, 0, flag);
}

void SkillWaterBall::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 30 * skill_lv;
}

SkillWaterInsignia::SkillWaterInsignia() : SkillImpl(SO_WATER_INSIGNIA) {
}

void SkillWaterInsignia::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillWhirlwind::SkillWhirlwind() : SkillImpl(SA_VIOLENTGALE) {
}

void SkillWhirlwind::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Does not consumes if the skill is already active. [Skotlex]
	std::shared_ptr<s_skill_unit_group> sg2;
	if ((sg2= skill_locate_element_field(src)) != nullptr && ( sg2->skill_id == SA_VOLCANO || sg2->skill_id == SA_DELUGE || sg2->skill_id == SA_VIOLENTGALE ))
	{
		if (sg2->limit - DIFF_TICK(gettick(), sg2->tick) > 0)
		{
			skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
			flag |= SKILL_NOCONSUME_REQ; // not to consume items
			return;
		}
		else
			sg2->limit = 0; //Disable it.
	}
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillWhiteImprison::SkillWhiteImprison() : SkillImpl(WL_WHITEIMPRISON) {
}

void SkillWhiteImprison::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 i = 0;

	if( (src == target || battle_check_target(src, target, BCT_ENEMY)>0) && status_get_class_(target) != CLASS_BOSS && !status_isimmune(target) ) // Should not work with Bosses.
	{
		int32 rate = ( sd? sd->status.job_level : 50 ) / 4;

		if( src == target ) rate = 100; // Success Chance: On self, 100%
		else if(target->type == BL_PC) rate += 20 + 10 * skill_lv; // On Players, (20 + 10 * Skill Level) %
		else rate += 40 + 10 * skill_lv; // On Monsters, (40 + 10 * Skill Level) %

		if( sd )
			skill_blockpc_start(*sd,getSkillId(),4000);

		if( !(tsc && tsc->getSCE(type)) ){
			i = sc_start2(src,target,type,rate,skill_lv,src->id,(src == target)?5000:(target->type == BL_PC)?skill_get_time(getSkillId(),skill_lv):skill_get_time2(getSkillId(), skill_lv));
			clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i);
			if( sd && !i )
				clif_skill_fail( *sd, getSkillId() );
		}
	}else
	if( sd )
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_TOTARGET );
}

SkillWindInsignia::SkillWindInsignia() : SkillImpl(SO_WIND_INSIGNIA) {
}

void SkillWindInsignia::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

std::unique_ptr<const SkillImpl> SkillFactoryMage::create(const e_skill skill_id) const {
	switch (skill_id) {
		case AG_ALL_BLOOM:
			return std::make_unique<SkillAllBloom>();
		case AG_ALL_BLOOM_ATK:
			return std::make_unique<SkillAllBloomAttack>();
		case AG_ALL_BLOOM_ATK2:
			return std::make_unique<SkillAllBloomAttack2>();
		case AG_ASTRAL_STRIKE:
			return std::make_unique<SkillAstralStrike>();
		case AG_ASTRAL_STRIKE_ATK:
			return std::make_unique<SkillAstralStrikeAttack>();
		case AG_CLIMAX:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case AG_CRIMSON_ARROW:
			return std::make_unique<SkillCrimsonArrow>();
		case AG_CRIMSON_ARROW_ATK:
			return std::make_unique<SkillCrimsonArrowAttack>();
		case AG_CRYSTAL_IMPACT:
			return std::make_unique<SkillCrystalImpact>();
		case AG_CRYSTAL_IMPACT_ATK:
			return std::make_unique<SkillCrystalImpactAttack>();
		case AG_DEADLY_PROJECTION:
			return std::make_unique<SkillDeadlyProjection>();
		case AG_DESTRUCTIVE_HURRICANE:
			return std::make_unique<SkillDestructiveHurricane>();
		case AG_DESTRUCTIVE_HURRICANE_CLIMAX:
			return std::make_unique<SkillDestructiveHurricaneClimax>();
		case AG_ENERGY_CONVERSION:
			return std::make_unique<SkillEnergyConversion>();
		case AG_FLORAL_FLARE_ROAD:
			return std::make_unique<SkillFloralFlareRoad>();
		case AG_FROZEN_SLASH:
			return std::make_unique<SkillFrozenSlash>();
		case AG_MYSTERY_ILLUSION:
			return std::make_unique<SkillMysteryIllusion>();
		case AG_RAIN_OF_CRYSTAL:
			return std::make_unique<SkillRainOfCrystal>();
		case AG_ROCK_DOWN:
			return std::make_unique<SkillRockDown>();
		case AG_SOUL_VC_STRIKE:
			return std::make_unique<SkillSoulVulcanStrike>();
		case AG_STORM_CANNON:
			return std::make_unique<SkillStormCannon>();
		case AG_STRANTUM_TREMOR:
			return std::make_unique<SkillStrantumTremor>();
		case AG_TORNADO_STORM:
			return std::make_unique<SkillTornadoStorm>();
		case AG_VIOLENT_QUAKE:
			return std::make_unique<SkillViolentQuake>();
		case AG_VIOLENT_QUAKE_ATK:
			return std::make_unique<SkillViolentQuakeAttack>();
		case EM_ACTIVITY_BURN:
			return std::make_unique<SkillActivityBurn>();
		case EM_CONFLAGRATION:
			return std::make_unique<SkillConflagration>();
		case EM_DIAMOND_STORM:
			return std::make_unique<SkillDiamondStorm>();
		case EM_ELEMENTAL_BUSTER:
			return std::make_unique<SkillElementalBuster>();
		case EM_ELEMENTAL_BUSTER_FIRE:
			return std::make_unique<SkillElementalBusterFire>();
		case EM_ELEMENTAL_BUSTER_GROUND:
			return std::make_unique<SkillElementalBusterGround>();
		case EM_ELEMENTAL_BUSTER_POISON:
			return std::make_unique<SkillElementalBusterPoison>();
		case EM_ELEMENTAL_BUSTER_WATER:
			return std::make_unique<SkillElementalBusterWater>();
		case EM_ELEMENTAL_BUSTER_WIND:
			return std::make_unique<SkillElementalBusterWind>();
		case EM_ELEMENTAL_VEIL:
			return std::make_unique<SkillElementalVeil>();
		case EM_INCREASING_ACTIVITY:
			return std::make_unique<SkillIncreasingActivity>();
		case EM_LIGHTNING_LAND:
			return std::make_unique<SkillLightningLand>();
		case EM_PSYCHIC_STREAM:
			return std::make_unique<SkillPsychicStream>();
		case EM_SPELL_ENCHANTING:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case EM_SUMMON_ELEMENTAL_ARDOR:
			return std::make_unique<SkillSummonElementalArdor>();
		case EM_SUMMON_ELEMENTAL_DILUVIO:
			return std::make_unique<SkillSummonElementalDiluvio>();
		case EM_SUMMON_ELEMENTAL_PROCELLA:
			return std::make_unique<SkillSummonElementalProcella>();
		case EM_SUMMON_ELEMENTAL_SERPENS:
			return std::make_unique<SkillSummonElementalSerpens>();
		case EM_SUMMON_ELEMENTAL_TERREMOTUS:
			return std::make_unique<SkillSummonElementalTerremotus>();
		case EM_TERRA_DRIVE:
			return std::make_unique<SkillTerraDrive>();
		case EM_VENOM_SWAMP:
			return std::make_unique<SkillVenomSwamp>();
		case HW_GANBANTEIN:
			return std::make_unique<SkillGanbantein>();
		case HW_GRAVITATION:
			return std::make_unique<SkillGravitationField>();
		case HW_MAGICCRASHER:
			return std::make_unique<WeaponSkillImpl>(skill_id);
		case HW_MAGICPOWER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case HW_NAPALMVULCAN:
			return std::make_unique<SkillNapalmVulcan>();
		case MG_COLDBOLT:
			return std::make_unique<SkillColdBolt>();
		case MG_ENERGYCOAT:
			return std::make_unique<SkillEnergyCoat>();
		case MG_FIREBALL:
			return std::make_unique<SkillFireBall>();
		case MG_FIREBOLT:
			return std::make_unique<SkillFireBolt>();
		case MG_FIREWALL:
			return std::make_unique<SkillFireWall>();
		case MG_FROSTDIVER:
			return std::make_unique<SkillFrostDiver>();
		case MG_LIGHTNINGBOLT:
			return std::make_unique<SkillLightningBolt>();
		case MG_NAPALMBEAT:
			return std::make_unique<SkillNapalmBeat>();
		case MG_SAFETYWALL:
			return std::make_unique<SkillSafetyWall>();
		case MG_SIGHT:
			return std::make_unique<SkillSight>();
		case MG_SOULSTRIKE:
			return std::make_unique<SkillSoulStrike>();
		case MG_STONECURSE:
			return std::make_unique<SkillStoneCurse>();
		case MG_THUNDERSTORM:
			return std::make_unique<SkillThunderStorm>();
		case PF_DOUBLECASTING:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case PF_FOGWALL:
			return std::make_unique<SkillBlindingMist>();
		case PF_HPCONVERSION:
			return std::make_unique<SkillIndulge>();
		case PF_MEMORIZE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case PF_MINDBREAKER:
			return std::make_unique<SkillMindBreaker>();
		case PF_SOULBURN:
			return std::make_unique<SkillSoulSiphon>();
		case PF_SOULCHANGE:
			return std::make_unique<SkillSoulExhale>();
		case PF_SPIDERWEB:
			return std::make_unique<SkillFiberLock>();
		case SA_ABRACADABRA:
			return std::make_unique<SkillHocusPocus>();
		case SA_AUTOSPELL:
			return std::make_unique<SkillHindsight>();
		case SA_CASTCANCEL:
			return std::make_unique<SkillCastCancel>();
		case SA_CLASSCHANGE:
			return std::make_unique<SkillClassChange>();
		case SA_COMA:
			return std::make_unique<SkillComa>();
		case SA_CREATECON:
			return std::make_unique<SkillCreateElementalConverter>();
		case SA_DEATH:
			return std::make_unique<SkillGrimReaper>();
		case SA_DELUGE:
			return std::make_unique<SkillDeluge>();
		case SA_DISPELL:
			return std::make_unique<SkillDispell>();
		case SA_ELEMENTFIRE:
			return std::make_unique<SkillElementalChangeFire>();
		case SA_ELEMENTGROUND:
			return std::make_unique<SkillElementalChangeEarth>();
		case SA_ELEMENTWATER:
			return std::make_unique<SkillElementalChangeWater>();
		case SA_ELEMENTWIND:
			return std::make_unique<SkillElementalChangeWind>();
		case SA_FLAMELAUNCHER:
			return std::make_unique<SkillEndowBlaze>();
		case SA_FORTUNE:
			return std::make_unique<SkillGoldDigger>();
		case SA_FROSTWEAPON:
			return std::make_unique<SkillEndowTsunami>();
		case SA_FULLRECOVERY:
			return std::make_unique<SkillRejuvenation>();
		case SA_GRAVITY:
			return std::make_unique<SkillGravity>();
		case SA_INSTANTDEATH:
			return std::make_unique<SkillSuicide>();
		case SA_LANDPROTECTOR:
			return std::make_unique<SkillMagneticEarth>();
		case SA_LEVELUP:
			return std::make_unique<SkillLeveling>();
		case SA_LIGHTNINGLOADER:
			return std::make_unique<SkillEndowTornado>();
		case SA_MAGICROD:
			return std::make_unique<SkillMagicRod>();
		case SA_MONOCELL:
			return std::make_unique<SkillMonocell>();
		case SA_QUESTION:
			return std::make_unique<SkillQuestioning>();
		case SA_REVERSEORCISH:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SA_SEISMICWEAPON:
			return std::make_unique<SkillEndowQuake>();
		case SA_SPELLBREAKER:
			return std::make_unique<SkillSpellBreaker>();
		case SA_SUMMONMONSTER:
			return std::make_unique<SkillMonsterChant>();
		case SA_TAMINGMONSTER:
			return std::make_unique<SkillBeastlyHypnosis>();
		case SA_VIOLENTGALE:
			return std::make_unique<SkillWhirlwind>();
		case SA_VOLCANO:
			return std::make_unique<SkillVolcano>();
		case SO_ARRULLO:
			return std::make_unique<SkillArrullo>();
		case SO_CLOUD_KILL:
			return std::make_unique<SkillCloudKill>();
		case SO_DIAMONDDUST:
			return std::make_unique<SkillDiamondDust>();
		case SO_EARTHGRAVE:
			return std::make_unique<SkillEarthGrave>();
		case SO_EARTH_INSIGNIA:
			return std::make_unique<SkillEarthInsignia>();
		case SO_ELECTRICWALK:
			return std::make_unique<SkillElectricWalk>();
		case SO_ELEMENTAL_SHIELD:
			return std::make_unique<SkillElementalShield>();
		case SO_EL_ACTION:
			return std::make_unique<SkillElementalAction>();
		case SO_EL_ANALYSIS:
			return std::make_unique<SkillFourSpiritAnalysis>();
		case SO_EL_CONTROL:
			return std::make_unique<SkillSpiritControl>();
		case SO_EL_CURE:
			return std::make_unique<SkillSpiritRecovery>();
		case SO_FIREWALK:
			return std::make_unique<SkillFireWalk>();
		case SO_FIRE_INSIGNIA:
			return std::make_unique<SkillFireInsignia>();
		case SO_POISON_BUSTER:
			return std::make_unique<SkillPoisonBuster>();
		case SO_PSYCHIC_WAVE:
			return std::make_unique<SkillPsychicWave>();
		case SO_SPELLFIST:
			return std::make_unique<SkillSpellFist>();
		case SO_STRIKING:
			return std::make_unique<SkillStriking>();
		case SO_SUMMON_AGNI:
			return std::make_unique<SkillSummonFireSpiritAgni>();
		case SO_SUMMON_AQUA:
			return std::make_unique<SkillSummonWaterSpiritAqua>();
		case SO_SUMMON_TERA:
			return std::make_unique<SkillSummonEarthSpiritTera>();
		case SO_SUMMON_VENTUS:
			return std::make_unique<SkillSummonWindSpiritVentus>();
		case SO_VACUUM_EXTREME:
			return std::make_unique<SkillVacuumExtreme>();
		case SO_VARETYR_SPEAR:
			return std::make_unique<SkillVaretyrSpear>();
		case SO_WARMER:
			return std::make_unique<SkillWarmer>();
		case SO_WATER_INSIGNIA:
			return std::make_unique<SkillWaterInsignia>();
		case SO_WIND_INSIGNIA:
			return std::make_unique<SkillWindInsignia>();
		case WL_CHAINLIGHTNING:
			return std::make_unique<SkillChainLightning>();
		case WL_CHAINLIGHTNING_ATK:
			return std::make_unique<SkillChainLightningAttack>();
		case WL_COMET:
			return std::make_unique<SkillComet>();
		case WL_CRIMSONROCK:
			return std::make_unique<SkillCrimsonRock>();
		case WL_DRAINLIFE:
			return std::make_unique<SkillDrainLife>();
		case WL_EARTHSTRAIN:
			return std::make_unique<SkillEarthStrain>();
		case WL_FROSTMISTY:
			return std::make_unique<SkillFrostyMisty>();
		case WL_HELLINFERNO:
			return std::make_unique<SkillHellInferno>();
		case WL_JACKFROST:
			return std::make_unique<SkillJackFrost>();
		case WL_MARSHOFABYSS:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case WL_READING_SB_READING:
			return std::make_unique<SkillReadingSpellbook>();
		case WL_RECOGNIZEDSPELL:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case WL_RELEASE:
			return std::make_unique<SkillRelease>();
		case WL_SIENNAEXECRATE:
			return std::make_unique<SkillSiennaExecrate>();
		case WL_SOULEXPANSION:
			return std::make_unique<SkillSoulExpansion>();
		case WL_STASIS:
			return std::make_unique<SkillStasis>();
		case WL_SUMMONBL:
			return std::make_unique<SkillSummonLightningBall>();
		case WL_SUMMONFB:
			return std::make_unique<SkillSummonFireBall>();
		case WL_SUMMONSTONE:
			return std::make_unique<SkillSummonStone>();
		case WL_SUMMONWB:
			return std::make_unique<SkillSummonWaterBall>();
		case WL_SUMMON_ATK_FIRE:
			return std::make_unique<SkillSummonAttackFire>();
		case WL_SUMMON_ATK_GROUND:
			return std::make_unique<SkillSummonAttackEarth>();
		case WL_SUMMON_ATK_WATER:
			return std::make_unique<SkillSummonAttackWater>();
		case WL_SUMMON_ATK_WIND:
			return std::make_unique<SkillSummonAttackWind>();
		case WL_TELEKINESIS_INTENSE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case WL_TETRAVORTEX:
			return std::make_unique<SkillTetraVortex>();
		case WL_TETRAVORTEX_FIRE:
			return std::make_unique<SkillTetraVortexFire>();
		case WL_TETRAVORTEX_GROUND:
			return std::make_unique<SkillTetraVortexEarth>();
		case WL_TETRAVORTEX_WATER:
			return std::make_unique<SkillTetraVortexWater>();
		case WL_TETRAVORTEX_WIND:
			return std::make_unique<SkillTetraVortexWind>();
		case WL_WHITEIMPRISON:
			return std::make_unique<SkillWhiteImprison>();
		case WZ_EARTHSPIKE:
			return std::make_unique<SkillEarthSpike>();
		case WZ_ESTIMATION:
			return std::make_unique<SkillSense>();
		case WZ_FIREPILLAR:
			return std::make_unique<SkillFirePillar>();
		case WZ_FROSTNOVA:
			return std::make_unique<SkillFrostNova>();
		case WZ_HEAVENDRIVE:
			return std::make_unique<SkillHeavensDrive>();
		case WZ_ICEWALL:
			return std::make_unique<SkillIceWall>();
		case WZ_JUPITEL:
			return std::make_unique<SkillJupitelThunder>();
		case WZ_METEOR:
			return std::make_unique<SkillMeteorStorm>();
		case WZ_QUAGMIRE:
			return std::make_unique<SkillQuagmire>();
		case WZ_SIGHTBLASTER:
			return std::make_unique<SkillSightBlaster>();
		case WZ_SIGHTRASHER:
			return std::make_unique<SkillSightRasher>();
		case WZ_STORMGUST:
			return std::make_unique<SkillStormGust>();
		case WZ_VERMILION:
			return std::make_unique<SkillLordOfVermilion>();
		case WZ_WATERBALL:
			return std::make_unique<SkillWaterBall>();

		default:
			return nullptr;
	}
}

#endif
