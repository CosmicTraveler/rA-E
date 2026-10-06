// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_archer.hpp"

#include <config/core.hpp>
#include "map/status.hpp"
#include "map/clif.hpp"
#include "map/map.hpp"
#include "map/pc.hpp"
#include "map/party.hpp"
#include "map/battle.hpp"
#include "map/mob.hpp"
#include <common/nullpo.hpp>
#include <common/random.hpp>
#include "map/log.hpp"
#include "map/unit.hpp"
#include "map/itemdb.hpp"
#include "map/path.hpp"
#include "skill_impl.hpp"

SkillAcousticRhythm::SkillAcousticRhythm() : SkillImpl(BD_SIEGFRIED) {
}

void SkillAcousticRhythm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillAcousticRhythm::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillAimedBolt::SkillAimedBolt() : WeaponSkillImpl(RA_AIMEDBOLT) {
}

void SkillAimedBolt::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	if (sc && sc->getSCE(SC_FEARBREEZE))
		skillratio += -100 + 800 + 35 * skill_lv;
	else
		skillratio += -100 + 500 + 20 * skill_lv;	
	RE_LVL_DMOD(100);
}

SkillAinRhapsody::SkillAinRhapsody() : SkillImpl(TR_AIN_RHAPSODY) {
}

void SkillAinRhapsody::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1)
		sc_start4(src, target, skill_get_sc(getSkillId()), 100, skill_lv, 0, flag, 0, skill_get_time(getSkillId(), skill_lv));
	else if (sd) {
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv);

		sd->skill_id_song = getSkillId();
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, getSkillId(), &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
}

SkillAmp::SkillAmp() : StatusSkillImpl(BD_ADAPTATION) {
}

void SkillAmp::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
#else
	status_change *tsc = status_get_sc(target);

	if(tsc && tsc->getSCE(SC_DANCING)){
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		status_change_end(target, SC_DANCING);
	}
#endif
}

SkillAnkleSnare::SkillAnkleSnare() : SkillImpl(HT_ANKLESNARE) {
}

void SkillAnkleSnare::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillArrowShower::SkillArrowShower() : SkillImplRecursiveDamageSplash(AC_SHOWER) {
}

void SkillArrowShower::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 50 + 10 * skill_lv;
#else
	base_skillratio += -25 + 5 * skill_lv;
#endif
}

void SkillArrowShower::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(src, SC_CAMOUFLAGE);

	SkillImplRecursiveDamageSplash::castendPos2(src, x, y, skill_lv, tick, flag);
}

SkillArrowStorm::SkillArrowStorm() : SkillImplRecursiveDamageSplash(RA_ARROWSTORM) {
}

void SkillArrowStorm::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	if (sc && sc->getSCE(SC_FEARBREEZE))
		skillratio += -100 + 200 + 250 * skill_lv;
	else
		skillratio += -100 + 200 + 180 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillArrowStorm::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);

	status_change_end(src, SC_CAMOUFLAGE);
}

SkillBattleTheme::SkillBattleTheme() : SkillImpl(BD_DRUMBATTLEFIELD) {
}

void SkillBattleTheme::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillBattleTheme::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillBeastStrafing::SkillBeastStrafing() : SkillImpl(HT_POWER) {
}

void SkillBeastStrafing::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	if( tstatus->race == RC_BRUTE || tstatus->race == RC_PLAYER_DORAM || tstatus->race == RC_INSECT )
		skill_attack(BF_WEAPON,src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillBeastStrafing::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -50 + 8 * sstatus->str;
}

SkillBlastMine::SkillBlastMine() : SkillImpl(HT_BLASTMINE) {
}

void SkillBlastMine::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillBlitzBeat::SkillBlitzBeat() : SkillImplRecursiveDamageSplash(HT_BLITZBEAT) {
}

SkillCamouflage::SkillCamouflage() : SkillImpl(RA_CAMOUFLAGE) {
}

void SkillCamouflage::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc && type != SC_NONE)?tsc->getSCE(type):nullptr;
	map_session_data* sd = BL_CAST( BL_PC, src );
	bool i = 0;

	if (tsce) {
		i = status_change_end(target, type);
		if( i )
			clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i);
		else if( sd )
			clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	i = sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
	if( i )
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i);
	else if( sd )
		clif_skill_fail( *sd, getSkillId(),  USESKILL_FAIL_LEVEL );
}

SkillChargeArrow::SkillChargeArrow() : WeaponSkillImpl(AC_CHARGEARROW)
{
}

void SkillChargeArrow::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const
{
	base_skillratio += 50;
}

SkillCircleOfNaturesSound::SkillCircleOfNaturesSound() : SkillImpl(WM_SIRCLEOFNATURE) {
}

void SkillCircleOfNaturesSound::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( flag&1 ) {	// These affect all party members near the caster.
		if( sc && sc->getSCE(type) ) {
			sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv));
		}
	} else if( sd ) {
		if( sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv)) )
			party_foreachsamemap(skill_area_sub,sd,skill_get_splash(getSkillId(),skill_lv),src,getSkillId(),skill_lv,tick,flag|BCT_PARTY|1,skill_castend_nodamage_id);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillClassicalPluck::SkillClassicalPluck() : SkillImpl(BD_ROKISWEIL) {
}

void SkillClassicalPluck::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillClassicalPluck::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillClaymoreTrap::SkillClaymoreTrap() : SkillImpl(HT_CLAYMORETRAP) {
}

void SkillClaymoreTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillClusterBomb::SkillClusterBomb() : SkillImpl(RA_CLUSTERBOMB) {
}

void SkillClusterBomb::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 + 100 * skill_lv;
}

void SkillClusterBomb::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillCobaltTrap::SkillCobaltTrap() : SkillImpl(RA_COBALTTRAP) {
}

void SkillCobaltTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillConcentration::SkillConcentration() : SkillImpl(AC_CONCENTRATION)
{
}

void SkillConcentration::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const
{
	sc_type type = skill_get_sc(getSkillId());

	int32 splash = skill_get_splash(getSkillId(), skill_lv);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	skill_reveal_trap_inarea(src, splash, src->x, src->y);
	map_foreachinallrange(status_change_timer_sub, src, splash, BL_CHAR, src, nullptr, type, tick);
}

SkillCresciveBolt::SkillCresciveBolt() : WeaponSkillImpl(WH_CRESCIVE_BOLT) {
}

void SkillCresciveBolt::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 500 + 1300 * skill_lv;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
	if (sc) {
		if (sc->getSCE(SC_CRESCIVEBOLT))
			skillratio += skillratio * (20 * sc->getSCE(SC_CRESCIVEBOLT)->val1) / 100;

		if (sc->getSCE(SC_CALAMITYGALE)) {
			skillratio += skillratio * 20 / 100;

			if (tstatus->race == RC_BRUTE || tstatus->race == RC_FISH)
				skillratio += skillratio * 50 / 100;
		}
	}
}

void SkillCresciveBolt::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	if( sc && sc->getSCE(SC_CRESCIVEBOLT) )
		sc_start(src, src, SC_CRESCIVEBOLT, 100, min( 3, 1 + sc->getSCE(SC_CRESCIVEBOLT)->val1 ), skill_get_time(getSkillId(), skill_lv));
	else
		sc_start(src, src, SC_CRESCIVEBOLT, 100, 1, skill_get_time(getSkillId(), skill_lv));
}

SkillDanceWithAWarg::SkillDanceWithAWarg() : SkillImpl(WM_DANCE_WITH_WUG) {
}

void SkillDanceWithAWarg::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( flag&1 ) {	// These affect all party members near the caster.
		if( sc && sc->getSCE(type) ) {
			sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv));
		}
	} else if( sd ) {
		if( sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv)) )
			party_foreachsamemap(skill_area_sub,sd,skill_get_splash(getSkillId(),skill_lv),src,getSkillId(),skill_lv,tick,flag|BCT_PARTY|1,skill_castend_nodamage_id);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillDazzler::SkillDazzler() : SkillImpl(DC_SCREAM) {
}

void SkillDazzler::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	int32 rate = 150 + 50 * skill_lv + 100; // Aegis accuracy (1000 = 100%). DC_SCREAM has a 10% higher base chance than BA_FROSTJOKER
	int32 duration = skill_get_time2(getSkillId(), skill_lv);
	if (battle_check_target(src, target, BCT_PARTY) > 0) {
		// TODO: check DC_SCREAM rate and duration.
		// DC_SCREAM and BA_FROSTJOKER initially shared the same code but the original comment only applies to BA_FROSTJOKER :
		// "On party members: Chance is divided by 4 and BA_FROSTJOKER duration is fixed to 15000ms"
		rate /= 4;
		duration = skill_get_time(getSkillId(), skill_lv);
	}
	status_change_start(src, target, skill_get_sc(getSkillId()), rate*10, skill_lv, 0, 0, 0, duration, SCSTART_NONE);
}

void SkillDazzler::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* md = BL_CAST(BL_MOB, src);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_addtimerskill(src,tick+3000,target->id,src->x,src->y,getSkillId(),skill_lv,0,flag);

	if (md) {
		// custom hack to make the mob display the skill, because these skills don't show the skill use text themselves
		//NOTE: mobs don't have the sprite animation that is used when performing this skill (will cause glitches)
		char temp[70];
		snprintf(temp, sizeof(temp), "%s : %s !!",md->name,skill_get_desc(getSkillId()));
		clif_disp_overhead(md,temp);
	}
}

SkillDeepBlindTrap::SkillDeepBlindTrap() : SkillImpl(WH_DEEPBLINDTRAP) {
}

void SkillDeepBlindTrap::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 850 * skill_lv + 5 * sstatus->con;
	RE_LVL_DMOD(100);
	skillratio += skillratio * (20 * (sd ? pc_checkskill(sd, WH_ADVANCED_TRAP) : 5)) / 100;
}

void SkillDeepBlindTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillDeepSleepLullaby::SkillDeepSleepLullaby() : SkillImpl(WM_LULLABY_DEEPSLEEP) {
}

void SkillDeepSleepLullaby::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag&1) {
		int32 rate = 4 * skill_lv + (sd ? pc_checkskill(sd, WM_LESSON) * 2 : 0) + status_get_lv(src) / 15 + (sd ? sd->status.job_level / 5 : 0);
		int32 duration = skill_get_time(getSkillId(), skill_lv) - (status_get_base_status(target)->int_ * 50 + status_get_lv(target) * 50); // Duration reduction for Deep Sleep Lullaby is doubled

		sc_start(src, target, type, rate, skill_lv, duration);
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
	}
}

SkillDetect::SkillDetect() : SkillImpl(HT_DETECTING) {
}

void SkillDetect::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea( status_change_timer_sub,
		src->m, x-i, y-i, x+i,y+i,BL_CHAR,
		src,nullptr,SC_SIGHT,tick);
	skill_reveal_trap_inarea(src, i, x, y);
}

SkillDetonator::SkillDetonator() : SkillImpl(RA_DETONATOR) {
}

void SkillDetonator::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_detonator, src->m, x-i, y-i, x+i, y+i, BL_SKILL, src);
	clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
}

static int32 skill_active_reverberation(block_list *bl, va_list ap);

SkillDominionImpulse::SkillDominionImpulse() : SkillImpl(WM_DOMINION_IMPULSE) {
}

void SkillDominionImpulse::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_active_reverberation, src->m, x-i, y-i, x+i,y+i,BL_SKILL);
}

static int32 skill_active_reverberation(block_list *bl, va_list ap) {
	skill_unit *su = (skill_unit*)bl;

	nullpo_ret(su);

	if (bl->type != BL_SKILL)
		return 0;

	std::shared_ptr<s_skill_unit_group> sg = su->group;

	if (su->alive && sg && sg->skill_id == NPC_REVERBERATION) {
		map_foreachinallrange(skill_trap_splash, bl, skill_get_splash(sg->skill_id, sg->skill_lv), sg->bl_flag, bl, gettick());
		su->limit = DIFF_TICK(gettick(), sg->tick);
		sg->unit_id = UNT_USED_TRAPS;
	}
	return 1;
}

SkillDoubleStrafe::SkillDoubleStrafe() : WeaponSkillImpl(AC_DOUBLE) {
}

void SkillDoubleStrafe::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 10 * (skill_lv - 1);
}

SkillDownTempo::SkillDownTempo() : SkillImpl(BD_ETERNALCHAOS) {
}

void SkillDownTempo::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillDownTempo::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillEchoSong::SkillEchoSong() : SkillImpl(MI_ECHOSONG) {
}

void SkillEchoSong::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	uint16 lesson_lv = (sd != nullptr) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON);

	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) ) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		sc_start2(src, target, type, 100, skill_lv, lesson_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
		sc_start2(src, target, type, 100, skill_lv, lesson_lv, skill_get_time(getSkillId(), skill_lv));
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillElectricShocker::SkillElectricShocker() : SkillImpl(RA_ELECTRICSHOCKER) {
}

void SkillElectricShocker::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillEncore::SkillEncore() : SkillImpl(BD_ENCORE) {
}

void SkillEncore::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if (sd != nullptr) {
		unit_skilluse_id(src,src->id,sd->skill_id_dance,sd->skill_lv_dance);
	}
}

SkillFalconAssault::SkillFalconAssault() : SkillImpl(SN_FALCONASSAULT) {
}

void SkillFalconAssault::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillFearBreeze::SkillFearBreeze() : StatusSkillImpl(RA_FEARBREEZE) {
}

void SkillFearBreeze::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillFiringTrap::SkillFiringTrap() : SkillImpl(RA_FIRINGTRAP) {
}

void SkillFiringTrap::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start4(src, target, SC_BURNING, 50 + skill_lv * 10, skill_lv, 1000, src->id, 0, skill_get_time2(getSkillId(), skill_lv));
}

void SkillFiringTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillFlameTrap::SkillFlameTrap() : SkillImpl(WH_FLAMETRAP) {
}

void SkillFlameTrap::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 850 * skill_lv + 5 * sstatus->con;
	RE_LVL_DMOD(100);
	skillratio += skillratio * (20 * (sd ? pc_checkskill(sd, WH_ADVANCED_TRAP) : 5)) / 100;
}

void SkillFlameTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillFlasher::SkillFlasher() : SkillImpl(HT_FLASHER) {
}

void SkillFlasher::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillFlasher::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_BLIND, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv), 1000);
}

SkillFocusBallet::SkillFocusBallet() : SkillImpl(DC_HUMMING) {
}

void SkillFocusBallet::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillFocusBallet::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	// Ammo should be deleted right away.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillFocusedArrowStrike::SkillFocusedArrowStrike() : SkillImplRecursiveDamageSplash(SN_SHARPSHOOTING) {
}

void SkillFocusedArrowStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	if (src->type == BL_MOB) { // TODO: Did these formulas change in the renewal balancing?
		if (wd->miscflag & 2) // Splash damage bonus
			skillratio += -100 + 140 * skill_lv;
		else
			skillratio += 100 + 50 * skill_lv;
		return;
	}
#ifdef RENEWAL
	skillratio += -100 + 300 + 300 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += 100 + 50 * skill_lv;
#endif
}

void SkillFocusedArrowStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);

	if( flag&1 ) {
		status_change_end(src, SC_CAMOUFLAGE);
	}
#else
	flag |= 2; // Flag for specific mob damage formula
	skill_area_temp[1] = target->id;
	if (battle_config.skill_eightpath_algorithm) {
		//Use official AoE algorithm
		if (!(map_foreachindir(skill_attack_area, src->m, src->x, src->y, target->x, target->y,
		   skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), 0, splash_target(src),
		   skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY))) {
			flag &= ~2; // Only targets in the splash area are affected

			//These skills hit at least the target if the AoE doesn't hit
			skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
		}
	} else {
		map_foreachinpath(skill_attack_area, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), splash_target(src),
			skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
	}
#endif
}

SkillFreezingTrap::SkillFreezingTrap() : SkillImpl(HT_FREEZINGTRAP) {
}

void SkillFreezingTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillFreezingTrap::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data* sstatus = status_get_status_data(*src);

	sc_start(src, target, SC_FREEZE, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv), sstatus->amotion + 100);
}

SkillFriggsSong::SkillFriggsSong() : SkillImpl(WM_FRIGG_SONG) {
}

void SkillFriggsSong::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) )
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillGaleStorm::SkillGaleStorm() : SkillImplRecursiveDamageSplash(WH_GALESTORM) {
}

void SkillGaleStorm::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 1350 * skill_lv;
	skillratio += 10 * sstatus->con;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CALAMITYGALE) && (tstatus->race == RC_BRUTE || tstatus->race == RC_FISH))
		skillratio += skillratio * 50 / 100;
}

void SkillGaleStorm::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	// Give AP if 3 or more targets are hit.
	if (sd && map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, BCT_ENEMY, skill_area_sub_count) >= 3)
		status_heal(src, 0, 0, 10, 0);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillGeffeniaNocturn::SkillGeffeniaNocturn() : SkillImpl(TR_GEF_NOCTURN) {
}

void SkillGeffeniaNocturn::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1)
		sc_start4(src, target, skill_get_sc(getSkillId()), 100, skill_lv, 0, flag, 0, skill_get_time(getSkillId(), skill_lv));
	else if (sd) {
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv);

		sd->skill_id_song = getSkillId();
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, getSkillId(), &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
}

SkillGloomyDay::SkillGloomyDay() : SkillImpl(WM_GLOOMYDAY) {
}

void SkillGloomyDay::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if( dstsd && ( pc_checkskill(dstsd,KN_BRANDISHSPEAR) || pc_checkskill(dstsd,LK_SPIRALPIERCE) ||
			pc_checkskill(dstsd,CR_SHIELDCHARGE) || pc_checkskill(dstsd,CR_SHIELDBOOMERANG) ||
			pc_checkskill(dstsd,PA_SHIELDCHAIN) || pc_checkskill(dstsd,LG_SHIELDPRESS) ) )
	{ // !TODO: Which skills aren't boosted anymore?
		sc_start(src,target,SC_GLOOMYDAY_SK,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		return;
	}

	sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

SkillGreatEcho::SkillGreatEcho() : WeaponSkillImpl(WM_GREAT_ECHO) {
}

void SkillGreatEcho::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 250 + 500 * skill_lv;
	if (sd) {
		skillratio += pc_checkskill(sd, WM_LESSON) * 50; // !TODO: Confirm bonus
		if (skill_check_pc_partner(const_cast<map_session_data*>(sd), getSkillId(), &skill_lv, AREA_SIZE, 0) > 0)
			skillratio *= 2;
	}
	RE_LVL_DMOD(100);
}

void SkillGreatEcho::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(),skill_lv);
	map_foreachinarea(skill_area_sub,src->m,x-i,y-i,x+i,y+i,BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
}

SkillGypsysKiss::SkillGypsysKiss() : SkillImpl(DC_SERVICEFORYOU) {
}

void SkillGypsysKiss::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillGypsysKiss::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	// Ammo should be deleted right away.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillHarmonicLick::SkillHarmonicLick() : SkillImpl(BD_RINGNIBELUNGEN) {
}

void SkillHarmonicLick::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillHarmonicLick::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillHarmonize::SkillHarmonize() : SkillImpl(MI_HARMONIZE) {
}

void SkillHarmonize::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	int32 duration = skill_get_time(getSkillId(), skill_lv);

	if( src != target ) {
		clif_skill_nodamage(src, *src, getSkillId(), skill_lv, sc_start(src, src, type, 100, skill_lv, duration));
	}

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, duration));
}

SkillHawkBoomerang::SkillHawkBoomerang() : WeaponSkillImpl(WH_HAWKBOOMERANG) {
}

void SkillHawkBoomerang::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 600 * skill_lv + 10 * sstatus->con;
	if (sd)
		skillratio += skillratio * pc_checkskill(sd, WH_NATUREFRIENDLY) / 10;
	if (tstatus->race == RC_BRUTE || tstatus->race == RC_FISH)
		skillratio += skillratio * 50 / 100;
	RE_LVL_DMOD(100);
}

void SkillHawkBoomerang::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillHawkMastery::SkillHawkMastery() : SkillImpl(WH_HAWK_M) {
}

void SkillHawkMastery::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd) {
		if (!pc_isfalcon(sd))
			pc_setoption(sd, sd->sc.option | OPTION_FALCON);
		else
			pc_setoption(sd, sd->sc.option&~OPTION_FALCON);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillHawkRush::SkillHawkRush() : WeaponSkillImpl(WH_HAWKRUSH) {
}

void SkillHawkRush::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 500 * skill_lv + 5 * sstatus->con;
	if (sd)
		skillratio += skillratio * pc_checkskill(sd, WH_NATUREFRIENDLY) / 10;
	RE_LVL_DMOD(100);
}

void SkillHawkRush::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillHipShaker::SkillHipShaker() : SkillImpl(DC_UGLYDANCE) {
}

void SkillHipShaker::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
#ifdef RENEWAL
	// !TODO: How does caster's DEX/AGI play a role?
	status_zap( target, 0, 2 * skill_lv + 10 );
#else
	map_session_data* sd = BL_CAST( BL_PC, src );

	int32 rate = 5 + 5 * skill_lv;
	rate += skill_lv * pc_checkskill(sd, DC_DANCINGLESSON);
	status_zap( target, 0, rate );
#endif
}

void SkillHipShaker::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillHipShaker::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	// Ammo should be deleted right away.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillIceboundTrap::SkillIceboundTrap() : SkillImpl(RA_ICEBOUNDTRAP) {
}

void SkillIceboundTrap::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_FREEZING, 50 + skill_lv * 10, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillIceboundTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillImpressiveRiff::SkillImpressiveRiff() : SkillImpl(BA_ASSASSINCROSS) {
}

void SkillImpressiveRiff::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillImpressiveRiff::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillImprovisedSong::SkillImprovisedSong() : SkillImpl(WM_RANDOMIZESPELL) {
}

void SkillImprovisedSong::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (rnd() % 100 < 30 + (10 * skill_lv)) {
		status_change_end(target, SC_SONGOFMANA);
		status_change_end(target, SC_DANCEWITHWUG);
		status_change_end(target, SC_LERADSDEW);
		status_change_end(target, SC_SATURDAYNIGHTFEVER);
		status_change_end(target, SC_BEYONDOFWARCRY);
		status_change_end(target, SC_MELODYOFSINK);
		status_change_end(target, SC_BEYONDOFWARCRY);
		status_change_end(target, SC_UNLIMITEDHUMMINGVOICE);

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillJawaiiSerenade::SkillJawaiiSerenade() : SkillImpl(TR_JAWAII_SERENADE) {
}

void SkillJawaiiSerenade::castendNoDamageId(block_list* src, block_list* bl, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
		sc_start4(src, bl, skill_get_sc(getSkillId()), 100, skill_lv, 0, flag, 0, skill_get_time(getSkillId(), skill_lv));
	else if (sd) {
		clif_skill_nodamage(bl, *bl, getSkillId(), skill_lv);

		sd->skill_id_song = getSkillId();
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, getSkillId(), &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
}

SkillLadyLuck::SkillLadyLuck() : SkillImpl(DC_FORTUNEKISS) {
}

void SkillLadyLuck::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillLadyLuck::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	// Ammo should be deleted right away.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillLandMine::SkillLandMine() : SkillImpl(HT_LANDMINE) {
}

void SkillLandMine::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillLandMine::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_STUN, 10, skill_lv, skill_get_time2(getSkillId(), skill_lv), 1000);
}

SkillLeradsDew::SkillLeradsDew() : SkillImpl(WM_LERADS_DEW) {
}

void SkillLeradsDew::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( flag&1 ) {	// These affect all party members near the caster.
		if( sc && sc->getSCE(type) ) {
			sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv));
		}
	} else if( sd ) {
		if( sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv)) )
			party_foreachsamemap(skill_area_sub,sd,skill_get_splash(getSkillId(),skill_lv),src,getSkillId(),skill_lv,tick,flag|BCT_PARTY|1,skill_castend_nodamage_id);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillLongingForFreedom::SkillLongingForFreedom() : SkillImpl(CG_LONGINGFREEDOM) {
}

void SkillLongingForFreedom::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;

	if (tsc && !tsce && (tsce=tsc->getSCE(SC_DANCING)) && tsce->val4
		&& (tsce->val1&0xFFFF) != CG_MOONLIT) //Can't use Longing for Freedom while under Moonlight Petals. [Skotlex]
	{
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	}
#endif
}

SkillLullaby::SkillLullaby() : SkillImpl(BD_LULLABY) {
}

void SkillLullaby::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
#ifndef RENEWAL
	status_change *sc = status_get_sc(src);
	status_data* sstatus = status_get_status_data(*src);

	if (sc != nullptr && sc->getSCE(SC_DANCING) != nullptr) {
		block_list* partner = map_id2bl(sc->getSCE(SC_DANCING)->val4);
		if (partner == nullptr)
			return;
		status_data* pstatus = status_get_status_data(*partner);
		if (pstatus == nullptr)
			return;
		status_change_start(src, target, skill_get_sc(getSkillId()), (sstatus->int_ + pstatus->int_ + rnd_value(100, 300)) * 10, skill_lv, 0, 0, 0, skill_get_time2(getSkillId(), skill_lv), SCSTART_NONE);
	}
#else
	// In renewal the chance is simply 100% and uses the original song duration as sleep duration
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
#endif
}

void SkillLullaby::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillLullaby::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillMagentaTrap::SkillMagentaTrap() : SkillImpl(RA_MAGENTATRAP) {
}

void SkillMagentaTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillMagicStrings::SkillMagicStrings() : SkillImpl(BA_POEMBRAGI) {
}

void SkillMagicStrings::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillMagicStrings::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillMaizeTrap::SkillMaizeTrap() : SkillImpl(RA_MAIZETRAP) {
}

void SkillMaizeTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillMakingArrow::SkillMakingArrow() : SkillImpl(AC_MAKINGARROW)
{
}

void SkillMakingArrow::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const
{
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd != nullptr)
	{
		clif_arrow_create_list(*sd);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillMarionetteControl::SkillMarionetteControl() : SkillImpl(CG_MARIONETTE) {
}

void SkillMarionetteControl::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	map_session_data *dstsd = BL_CAST(BL_PC, target);
	status_change *sc = status_get_sc(src);
	status_change *tsc = status_get_sc(target);

	if ((sd && dstsd && (dstsd->class_ & MAPID_SECONDMASK) == MAPID_BARDDANCER && dstsd->status.sex == sd->status.sex) ||
		(tsc && (tsc->getSCE(SC_CURSE) || tsc->getSCE(SC_QUAGMIRE)))) {
		// Cannot cast on another bard/dancer-type class of the same gender as caster, or targets under Curse/Quagmire
		if (sd != nullptr) {
			clif_skill_fail(*sd, getSkillId());
		}
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	if (sc && tsc) {
		if (!sc->getSCE(SC_MARIONETTE) && !tsc->getSCE(SC_MARIONETTE2)) {
			sc_start(src, src, SC_MARIONETTE, 100, target->id, skill_get_time(getSkillId(), skill_lv));
			sc_start(src, target, SC_MARIONETTE2, 100, src->id, skill_get_time(getSkillId(), skill_lv));
			clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		} else if (sc->getSCE(SC_MARIONETTE) && sc->getSCE(SC_MARIONETTE)->val1 == target->id &&
			tsc->getSCE(SC_MARIONETTE2) && tsc->getSCE(SC_MARIONETTE2)->val1 == src->id) {
			status_change_end(src, SC_MARIONETTE);
			status_change_end(target, SC_MARIONETTE2);
		} else {
			if (sd != nullptr) {
				clif_skill_fail(*sd, getSkillId());
			}
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
	}
}

SkillMelodyOfSink::SkillMelodyOfSink() : SkillImpl(WM_MELODYOFSINK) {
}

void SkillMelodyOfSink::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( flag&1 ) {
		sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
	} else {	// These affect to all targets around the caster.
		if( rnd()%100 < 5 + 5 * skill_lv + pc_checkskill(sd, WM_LESSON) ) { // !TODO: What's the Lesson bonus?
			map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(),skill_lv),BL_PC, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
			clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		}
	}
}

SkillMelodyStrike::SkillMelodyStrike() : WeaponSkillImpl(BA_MUSICALSTRIKE) {
}

void SkillMelodyStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 10 + 40 * skill_lv;
#else
	base_skillratio += -40 + 40 * skill_lv;
#endif
}

SkillMentalSensing::SkillMentalSensing() : SkillImpl(BD_RICHMANKIM) {
}

void SkillMentalSensing::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillMentalSensing::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillMetallicFury::SkillMetallicFury() : SkillImplRecursiveDamageSplash(TR_METALIC_FURY) {
}

void SkillMetallicFury::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* tsc = status_get_sc(target);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 3850 * skill_lv;
	// !Todo: skill affected by SPL (without SC_SOUNDBLEND) as well?
	if (tsc && tsc->getSCE(SC_SOUNDBLEND)) {
		skillratio += 800 * skill_lv;
		skillratio += 2 * pc_checkskill(sd, TR_STAGE_MANNER) * sstatus->spl;
	}
	RE_LVL_DMOD(100);
}

void SkillMetallicFury::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr)
		element = sd->bonus.arrow_ele;
}

SkillMetallicSound::SkillMetallicSound() : SkillImpl(WM_METALICSOUND) {
}

void SkillMetallicSound::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change_end(target, SC_SOUNDBLEND);
}

void SkillMetallicSound::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *tsc = status_get_sc(target);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 120 * skill_lv + 60 * ((sd) ? pc_checkskill(sd, WM_LESSON) : 1);
	if (tsc && tsc->getSCE(SC_SLEEP))
		skillratio += 100; // !TODO: Confirm target sleeping bonus
	RE_LVL_DMOD(100);
	if (tsc && tsc->getSCE(SC_SOUNDBLEND))
		skillratio += skillratio * 50 / 100;
}

void SkillMetallicSound::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillMoonlitSerenade::SkillMoonlitSerenade() : SkillImpl(WA_MOONLIT_SERENADE) {
}

void SkillMoonlitSerenade::castendNoDamageId(block_list* src, block_list* bl, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	sc_type type = skill_get_sc(getSkillId());

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
		sc_start2(src, bl, type, 100, skill_lv, ((sd) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON)), skill_get_time(getSkillId(), skill_lv));
	} else if (sd) {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
		sc_start2(src, bl, type, 100, skill_lv, ((sd) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON)), skill_get_time(getSkillId(), skill_lv));
		clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
	}
}

SkillMusicalInterlude::SkillMusicalInterlude() : SkillImpl(TR_MUSICAL_INTERLUDE) {
}

void SkillMusicalInterlude::castendNoDamageId(block_list* src, block_list* bl, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
		sc_start4(src, bl, skill_get_sc(getSkillId()), 100, skill_lv, 0, flag, 0, skill_get_time(getSkillId(), skill_lv));
	else if (sd) {
		clif_skill_nodamage(bl, *bl, getSkillId(), skill_lv);

		sd->skill_id_song = getSkillId();
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, getSkillId(), &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
}

SkillNipelheimRequiem::SkillNipelheimRequiem() : SkillImpl(TR_NIPELHEIM_REQUIEM) {
}

void SkillNipelheimRequiem::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1) { // Need official success chances.
		uint16 success_chance = 5 * skill_lv;

		if (flag & 2)
			success_chance *= 2;

		// Is it a chance to inflect so and so, or seprate chances for inflicting each status? [Rytech]
		sc_start(src, target, SC_CURSE, 4 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
		sc_start(src, target, SC_HANDICAPSTATE_DEPRESSION, success_chance, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	} else if (sd) {
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv);

		sd->skill_id_song = getSkillId();
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, getSkillId(), &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
}

SkillPangVoice::SkillPangVoice() : SkillImpl(BA_PANGVOICE) {
}

void SkillPangVoice::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	// In Renewal it causes Confusion and Bleeding to 100% base chance
	sc_start(src, target, SC_CONFUSION, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	sc_start(src, target, SC_BLEEDING, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
#else
	// In Pre-renewal it causes Confusion to 70% base chance
	sc_start(src, target, SC_CONFUSION, 70, skill_lv, skill_get_time(getSkillId(), skill_lv));
#endif
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillPerfectTablature::SkillPerfectTablature() : SkillImpl(BA_WHISTLE) {
}

void SkillPerfectTablature::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillPerfectTablature::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillPhantasmicArrow::SkillPhantasmicArrow() : WeaponSkillImpl(HT_PHANTASMIC) {
}

void SkillPhantasmicArrow::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 400;
#else
	base_skillratio += 50;
#endif
}

SkillPoemOfTheNetherworld::SkillPoemOfTheNetherworld() : SkillImpl(WM_POEMOFNETHERWORLD) {
}

void SkillPoemOfTheNetherworld::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillPowerChord::SkillPowerChord() : SkillImpl(BD_INTOABYSS) {
}

void SkillPowerChord::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillPowerChord::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillPronMarch::SkillPronMarch() : SkillImpl(TR_PRON_MARCH) {
}

void SkillPronMarch::castendNoDamageId(block_list* src, block_list* bl, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
		sc_start4(src, bl, skill_get_sc(getSkillId()), 100, skill_lv, 0, flag, 0, skill_get_time(getSkillId(), skill_lv));
	else if (sd) {
		clif_skill_nodamage(bl, *bl, getSkillId(), skill_lv);

		sd->skill_id_song = getSkillId();
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, getSkillId(), &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
}

SkillRemoveTrap::SkillRemoveTrap() : SkillImpl(HT_REMOVETRAP) {
}

void SkillRemoveTrap::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd == nullptr ){
		return;
	}

	skill_unit* su = BL_CAST(BL_SKILL, target);
	std::shared_ptr<s_skill_unit_group> sg;
	std::shared_ptr<s_skill_db> skill_group;

	// Players can only remove their own traps or traps on Vs maps.
	if( su && (sg = su->group) && (sg->src_id == src->id || map_flag_vs(target->m)) && ( skill_group = skill_db.find(sg->skill_id) ) && skill_group->inf2[INF2_ISTRAP] )
	{
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		if( !(sg->unit_id == UNT_USED_TRAPS || (sg->unit_id == UNT_ANKLESNARE && sg->val2 != 0 )) )
		{ // prevent picking up expired traps
			if( battle_config.skill_removetrap_type )
			{ // get back all items used to deploy the trap
				for( int32 i = 0; i < MAX_SKILL_ITEM_REQUIRE; i++ )
				{
					if( skill_group->require.itemid[i] > 0 )
					{
						int32 flag2;
						struct item item_tmp;
						memset(&item_tmp,0,sizeof(item_tmp));
						item_tmp.nameid = skill_group->require.itemid[i];
						item_tmp.identify = 1;
						item_tmp.amount = skill_group->require.amount[i];
						if( item_tmp.nameid && (flag2=pc_additem(sd,&item_tmp,item_tmp.amount,LOG_TYPE_OTHER)) ){
							clif_additem(sd,0,0,flag2);
							if (battle_config.skill_drop_items_full)
								map_addflooritem(&item_tmp,item_tmp.amount,sd->m,sd->x,sd->y,0,0,0,4,0);
						}
					}
				}
			}
			else
			{ // get back 1 trap
				struct item item_tmp;
				memset(&item_tmp,0,sizeof(item_tmp));
				item_tmp.nameid = su->group->item_id?su->group->item_id:ITEMID_TRAP;
				item_tmp.identify = 1;
				if( item_tmp.nameid && (flag=pc_additem(sd,&item_tmp,1,LOG_TYPE_OTHER)) )
				{
					clif_additem(sd,0,0,flag);
					if (battle_config.skill_drop_items_full)
						map_addflooritem(&item_tmp,1,sd->m,sd->x,sd->y,0,0,0,4,0);
				}
			}
		}
		skill_delunit(su);
	}else
		clif_skill_fail( *sd, getSkillId() );
}

SkillRetrospection::SkillRetrospection() : SkillImpl(TR_RETROSPECTION) {
}

void SkillRetrospection::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	if (sd)
		unit_skilluse_id(src, src->id, sd->skill_id_song, sd->skill_lv_song);
}

SkillReverberation::SkillReverberation() : SkillImpl(WM_REVERBERATION) {
}

void SkillReverberation::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change_end(target, SC_SOUNDBLEND);
}

void SkillReverberation::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *tsc = status_get_sc(target);

	// MATK [{(Skill Level x 300) + 400} x Casters Base Level / 100] %
	skillratio += -100 + 700 + 300 * skill_lv;
	RE_LVL_DMOD(100);
	if (tsc && tsc->getSCE(SC_SOUNDBLEND))
		skillratio += skillratio * 50 / 100;
}

void SkillReverberation::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|SD_SPLASH|1, skill_castend_damage_id);
		battle_consume_ammo(sd, getSkillId(), skill_lv); // Consume here since Magic/Misc attacks reset arrow_atk
	}
}

void SkillReverberation::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr)
		element = sd->bonus.arrow_ele;
}

SkillRhythmicalWave::SkillRhythmicalWave() : SkillImpl(TR_RHYTHMICAL_WAVE) {
}

void SkillRhythmicalWave::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|SD_SPLASH|1, skill_castend_damage_id);
		battle_consume_ammo(sd, getSkillId(), skill_lv); // Consume here since Magic/Misc attacks reset arrow_atk
	}
}

void SkillRhythmicalWave::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 250 + 3650 * skill_lv;
	skillratio += pc_checkskill(sd, TR_STAGE_MANNER) * 25; // !TODO: check Stage Manner ratio
	skillratio += 5 * sstatus->spl;	// !TODO: check SPL ratio

	if (sc != nullptr && sc->hasSCE(SC_MYSTIC_SYMPHONY))
		skillratio += 200 + 1000 * skill_lv;

	RE_LVL_DMOD(100);
}

void SkillRhythmicalWave::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr)
		element = sd->bonus.arrow_ele;
}

SkillRhythmShooting::SkillRhythmShooting() : WeaponSkillImpl(TR_RHYTHMSHOOTING) {
}

void SkillRhythmShooting::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillRhythmShooting::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_change* tsc = status_get_sc(target);
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 550 + 950 * skill_lv;

	if (sd && pc_checkskill(sd, TR_STAGE_MANNER) > 0)
		skillratio += 5 * sstatus->con;

	if (tsc && tsc->getSCE(SC_SOUNDBLEND)) {
		skillratio += 300 + 100 * skill_lv;
		skillratio += 2 * sstatus->con;
	}

	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_MYSTIC_SYMPHONY)) {
		skillratio *= 2;

		if (tstatus->race == RC_FISH || tstatus->race == RC_DEMIHUMAN)
			skillratio += skillratio * 50 / 100;
	}
}

SkillRokiCapriccio::SkillRokiCapriccio() : SkillImpl(TR_ROKI_CAPRICCIO) {
}

void SkillRokiCapriccio::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1) { // Need official success chances.
		uint16 success_chance = 5 * skill_lv;

		if (flag & 2)
			success_chance *= 2;

		// Is it a chance to inflect so and so, or seprate chances for inflicting each status? [Rytech]
		sc_start(src, target, SC_CONFUSION, 4 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
		sc_start(src, target, SC_HANDICAPSTATE_MISFORTUNE, success_chance, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	}
	else if (sd) {
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv);

		sd->skill_id_song = getSkillId();
		sd->skill_lv_song = skill_lv;

		if (skill_check_pc_partner(sd, getSkillId(), &skill_lv, AREA_SIZE, 0) > 0)
			flag |= 2;

		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	}
}

SkillRoseBlossom::SkillRoseBlossom() : WeaponSkillImpl(TR_ROSEBLOSSOM) {
}

void SkillRoseBlossom::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillRoseBlossom::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_change* tsc = status_get_sc(target);
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 200 + 2000 * skill_lv;

	if (sd && pc_checkskill(sd, TR_STAGE_MANNER) > 0)
		skillratio += 3 * sstatus->con;

	if( tsc != nullptr && tsc->getSCE( SC_SOUNDBLEND ) ){
		skillratio += 200 * skill_lv;
	}

	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_MYSTIC_SYMPHONY)) {
		skillratio *= 2;

		if (tstatus->race == RC_FISH || tstatus->race == RC_DEMIHUMAN)
			skillratio += skillratio * 50 / 100;
	}
}

void SkillRoseBlossom::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Rose blossom seed can only bloom if the target is hit.
	sc_start4(src, target, SC_ROSEBLOSSOM, 100, skill_lv, TR_ROSEBLOSSOM_ATK, src->id, 0, skill_get_time(getSkillId(), skill_lv));
	status_change_end(target, SC_SOUNDBLEND);
}

SkillRoseBlossomAttack::SkillRoseBlossomAttack() : SkillImplRecursiveDamageSplash(TR_ROSEBLOSSOM_ATK) {
}

void SkillRoseBlossomAttack::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_change* tsc = status_get_sc(target);
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 550 + 3850 * skill_lv;

	if (sd && pc_checkskill(sd, TR_STAGE_MANNER) > 0)
		skillratio += 3 * sstatus->con;

	if (tsc != nullptr && tsc->getSCE(SC_SOUNDBLEND)) {
		skillratio += 200 * skill_lv;
	}

	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_MYSTIC_SYMPHONY)) {
		skillratio *= 2;

		if (tstatus->race == RC_FISH || tstatus->race == RC_DEMIHUMAN)
			skillratio += skillratio * 50 / 100;
	}
}

SkillSandman::SkillSandman() : SkillImpl(HT_SANDMAN) {
}

void SkillSandman::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillSandman::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_SLEEP, (10 * skill_lv + 40), skill_lv, skill_get_time2(getSkillId(), skill_lv), 1000);
}

SkillSaturdayNightFever::SkillSaturdayNightFever() : SkillImpl(WM_SATURDAY_NIGHT_FEVER) {
}

void SkillSaturdayNightFever::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* sstatus = status_get_status_data(*src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( flag&1 ) {
		sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
	} else if (sd) {
		if( rnd()%100 < sstatus->int_ / 6 + sd->status.job_level / 5 + skill_lv * 4 + pc_checkskill(sd, WM_LESSON) ) { // !TODO: What's the Lesson bonus?
			map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(),skill_lv),BL_PC, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
			clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		}
	}
}

SkillSensitiveKeen::SkillSensitiveKeen() : WeaponSkillImpl(RA_SENSITIVEKEEN) {
}

void SkillSensitiveKeen::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 50 * skill_lv;
}

void SkillSensitiveKeen::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if( target->type != BL_SKILL ) { // Only Hits Invisible Targets
		if (tsc && ((tsc->option&(OPTION_HIDE|OPTION_CLOAK|OPTION_CHASEWALK)) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC_STEALTHFIELD))) {
			status_change_end(target, SC_CLOAKINGEXCEED);
			WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
		}
		if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
			status_change_end(target, SC__SHADOWFORM); // Should only end, no damage dealt.
	} else {
		skill_unit *su = BL_CAST(BL_SKILL, target);
		std::shared_ptr<s_skill_unit_group> sg;

		if (su && (sg = su->group) && skill_get_inf2(sg->skill_id, INF2_ISTRAP)) {
			if( !(sg->unit_id == UNT_USED_TRAPS || (sg->unit_id == UNT_ANKLESNARE && sg->val2 != 0 )) )
			{
				struct item item_tmp;
				memset(&item_tmp,0,sizeof(item_tmp));
				item_tmp.nameid = sg->item_id?sg->item_id:ITEMID_TRAP;
				item_tmp.identify = 1;
				if( item_tmp.nameid )
					map_addflooritem(&item_tmp,1,target->m,target->x,target->y,0,0,0,4,0);
			}
			skill_delunit(su);
		}
	}
}

void SkillSensitiveKeen::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	map_foreachinrange(skill_area_sub,src,skill_get_splash(getSkillId(),skill_lv),BL_CHAR|BL_SKILL,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY,skill_castend_damage_id);
}

void SkillSensitiveKeen::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	if( rnd()%100 < 8 * skill_lv ) {
		map_session_data* sd = BL_CAST( BL_PC, src );
	
		skill_castend_damage_id(src, target, RA_WUGBITE, ((sd) ? pc_checkskill(sd, RA_WUGBITE) : skill_get_max(RA_WUGBITE)), tick, SD_ANIMATION);
	}
}

// WM_SEVERE_RAINSTORM
SkillSevereRainstorm::SkillSevereRainstorm() : SkillImpl(WM_SEVERE_RAINSTORM) {
}

void SkillSevereRainstorm::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	flag |= 1;
	if (sd)
		sd->canequip_tick = tick + skill_get_time(getSkillId(), skill_lv); // Can't switch equips for the duration of the skill.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}


// WM_SEVERE_RAINSTORM_MELEE
SkillSevereRainstormMelee::SkillSevereRainstormMelee() : WeaponSkillImpl(WM_SEVERE_RAINSTORM_MELEE) {
}

void SkillSevereRainstormMelee::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	//ATK [{(Caster DEX / 300 + AGI / 200)} x Caster Base Level / 100] %
	skillratio += -100 + 100 * skill_lv + (sstatus->dex / 300 + sstatus->agi / 200);
	if (wd->miscflag&4) // Whip/Instrument equipped
		skillratio += 20 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillShelteringBliss::SkillShelteringBliss() : SkillImpl(CG_MOONLIT) {
}

void SkillShelteringBliss::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= 1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

SkillShockwaveTrap::SkillShockwaveTrap() : SkillImpl(HT_SHOCKWAVE) {
}

void SkillShockwaveTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillShockwaveTrap::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_percent_damage(src, target, 0, -(15*skill_lv+5), false);
}

SkillSkidTrap::SkillSkidTrap() : SkillImpl(HT_SKIDTRAP) {
}

void SkillSkidTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillSkilledSpecialSinger::SkillSkilledSpecialSinger() : SkillImpl(CG_SPECIALSINGER) {
}

void SkillSkilledSpecialSinger::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if (tsc && tsc->getSCE(SC_ENSEMBLEFATIGUE)) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		status_change_end(target, SC_ENSEMBLEFATIGUE);
	}
}

SkillSlingingArrow::SkillSlingingArrow() : WeaponSkillImpl(DC_THROWARROW) {
}

void SkillSlingingArrow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 10 + 40 * skill_lv;
#else
	base_skillratio += -40 + 40 * skill_lv;
#endif
}

SkillSlowGrace::SkillSlowGrace() : SkillImpl(DC_DONTFORGETME) {
}

void SkillSlowGrace::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillSlowGrace::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	// Ammo should be deleted right away.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillSolidTrap::SkillSolidTrap() : SkillImpl(WH_SOLIDTRAP) {
}

void SkillSolidTrap::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 850 * skill_lv + 5 * sstatus->con;
	RE_LVL_DMOD(100);
	skillratio += skillratio * (20 * (sd ? pc_checkskill(sd, WH_ADVANCED_TRAP) : 5)) / 100;
}

void SkillSolidTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillSongofLutie::SkillSongofLutie() : SkillImpl(BA_APPLEIDUN) {
}

void SkillSongofLutie::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillSongofLutie::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillSongOfMana::SkillSongOfMana() : SkillImpl(WM_SONG_OF_MANA) {
}

void SkillSongOfMana::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( flag&1 ) {	// These affect all party members near the caster.
		if( sc && sc->getSCE(type) ) {
			sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv));
		}
	} else if( sd ) {
		if( sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv)) )
			party_foreachsamemap(skill_area_sub,sd,skill_get_splash(getSkillId(),skill_lv),src,getSkillId(),skill_lv,tick,flag|BCT_PARTY|1,skill_castend_nodamage_id);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillSoundBlend::SkillSoundBlend() : SkillImpl(TR_SOUNDBLEND) {
}

void SkillSoundBlend::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, 0);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv)));
}

void SkillSoundBlend::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillSoundBlend::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 120 * skill_lv + 5 * sstatus->spl;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_MYSTIC_SYMPHONY)) {
		skillratio += skillratio * 100 / 100;

		if (tstatus->race == RC_FISH || tstatus->race == RC_DEMIHUMAN)
			skillratio += skillratio * 50 / 100;
	}
}

void SkillSoundBlend::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr)
		element = sd->bonus.arrow_ele;
}

SkillSoundOfDestruction::SkillSoundOfDestruction() : SkillImpl(WM_SOUND_OF_DESTRUCTION) {
}

void SkillSoundOfDestruction::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag&1) {
		sc_start(src, target, type, 100, skill_lv, (sd ? pc_checkskill(sd, WM_LESSON) * 500 : 0) + skill_get_time(getSkillId(), skill_lv)); // !TODO: Confirm Lesson increase
	} else {
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv),BL_PC, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillSpringTrap::SkillSpringTrap() : SkillImpl(HT_SPRINGTRAP) {
}

void SkillSpringTrap::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);

	skill_unit *su=nullptr;
	if((target->type==BL_SKILL) && (su=(skill_unit *)target) && (su->group) ){
		switch(su->group->unit_id){
			case UNT_ANKLESNARE:	// ankle snare
				if (su->group->val2 != 0)
					// if it is already trapping something don't spring it,
					// remove trap should be used instead
					break;
				[[fallthrough]];
			case UNT_BLASTMINE:
			case UNT_SKIDTRAP:
			case UNT_LANDMINE:
			case UNT_SHOCKWAVE:
			case UNT_SANDMAN:
			case UNT_FLASHER:
			case UNT_FREEZINGTRAP:
			case UNT_CLAYMORETRAP:
			case UNT_TALKIEBOX:
				su->group->unit_id = UNT_USED_TRAPS;
				clif_changetraplook(target, UNT_USED_TRAPS);
				su->group->limit=DIFF_TICK(tick+1500,su->group->tick);
				su->limit=DIFF_TICK(tick+1500,su->group->tick);
		}
	}
}

SkillSwiftTrap::SkillSwiftTrap() : SkillImpl(WH_SWIFTTRAP) {
}

void SkillSwiftTrap::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 850 * skill_lv + 5 * sstatus->con;
	RE_LVL_DMOD(100);
	skillratio += skillratio * (20 * (sd ? pc_checkskill(sd, WH_ADVANCED_TRAP) : 5)) / 100;
}

void SkillSwiftTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillSwingDance::SkillSwingDance() : SkillImpl(WA_SWING_DANCE) {
}

void SkillSwingDance::castendNoDamageId(block_list* src, block_list* bl, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	sc_type type = skill_get_sc(getSkillId());

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
		sc_start2(src, bl, type, 100, skill_lv, ((sd) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON)), skill_get_time(getSkillId(), skill_lv));
	} else if (sd) {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
		sc_start2(src, bl, type, 100, skill_lv, ((sd) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON)), skill_get_time(getSkillId(), skill_lv));
		clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
	}
}

SkillSymphonyOfLovers::SkillSymphonyOfLovers() : SkillImpl(WA_SYMPHONY_OF_LOVER) {
}

void SkillSymphonyOfLovers::castendNoDamageId(block_list* src, block_list* bl, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	sc_type type = skill_get_sc(getSkillId());

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
		sc_start2(src, bl, type, 100, skill_lv, ((sd) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON)), skill_get_time(getSkillId(), skill_lv));
	} else if (sd) {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
		sc_start2(src, bl, type, 100, skill_lv, ((sd) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON)), skill_get_time(getSkillId(), skill_lv));
		clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
	}
}

SkillTalkieBox::SkillTalkieBox() : SkillImpl(HT_TALKIEBOX) {
}

void SkillTalkieBox::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

int32 skill_tarotcard(block_list* src, block_list* target, uint16 skill_id, uint16 skill_lv, t_tick tick);


SkillTarotCardOfFate::SkillTarotCardOfFate() : SkillImpl(CG_TAROTCARD) {
}

void SkillTarotCardOfFate::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	mob_data *dstmd = BL_CAST(BL_MOB, target);
	status_change *tsc = status_get_sc(target);

	if (tsc && tsc->getSCE(SC_TAROTCARD)) {
		// Target currently has the SUN tarot card effect and is immune to any other effect.
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	if (rnd() % 100 > skill_lv * 8 ||
#ifndef RENEWAL
		(tsc && tsc->getSCE(SC_BASILICA)) ||
#endif
		(dstmd && ((dstmd->guardian_data && dstmd->mob_id == MOBID_EMPERIUM) || status_get_class_(target) == CLASS_BATTLEFIELD))) {
		if (sd != nullptr)
			clif_skill_fail(*sd, getSkillId());
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	status_zap(src, 0, skill_get_sp(getSkillId(), skill_lv)); // Consume SP only on success.
	int32 card = skill_tarotcard(src, target, getSkillId(), skill_lv, tick); // Actual effect is executed here.
	clif_specialeffect((card == 6) ? src : target, EF_TAROTCARD1 + card - 1, AREA);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}


/*========================================== [Playtester]
* Process tarot card's effects
* @param src: Source of the tarot card effect
* @param target: Target of the tartor card effect
* @param skill_id: ID of the skill used
* @param skill_lv: Level of the skill used
* @param tick: Processing tick time
* @return Card number
*------------------------------------------*/
int32 skill_tarotcard(block_list* src, block_list *target, uint16 skill_id, uint16 skill_lv, t_tick tick)
{
	int32 card = 0;

	if (battle_config.tarotcard_equal_chance) {
		//eAthena equal chances
		card = rnd() % 14 + 1;
	}
	else {
		//Official chances
		int32 rate = rnd() % 100;
		if (rate < 10) card = 1; // THE FOOL
		else if (rate < 20) card = 2; // THE MAGICIAN
		else if (rate < 30) card = 3; // THE HIGH PRIESTESS
		else if (rate < 37) card = 4; // THE CHARIOT
		else if (rate < 47) card = 5; // STRENGTH
		else if (rate < 62) card = 6; // THE LOVERS
		else if (rate < 63) card = 7; // WHEEL OF FORTUNE
		else if (rate < 69) card = 8; // THE HANGED MAN
		else if (rate < 74) card = 9; // DEATH
		else if (rate < 82) card = 10; // TEMPERANCE
		else if (rate < 83) card = 11; // THE DEVIL
		else if (rate < 85) card = 12; // THE TOWER
		else if (rate < 90) card = 13; // THE STAR
		else card = 14; // THE SUN
	}

	switch (card) {
	case 1: // THE FOOL - heals SP to 0
	{
		status_percent_damage(src, target, 0, 100, false);
		break;
	}
	case 2: // THE MAGICIAN - matk halved
	{
		sc_start(src, target, SC_INCMATKRATE, 100, -50, skill_get_time2(skill_id, skill_lv));
		break;
	}
	case 3: // THE HIGH PRIESTESS - all buffs removed
	{
		status_change_clear_buffs(target, SCCB_BUFFS | SCCB_CHEM_PROTECT);
		break;
	}
	case 4: // THE CHARIOT - 1000 damage, random armor destroyed
	{
		battle_fix_damage(src, target, 1000, 0, skill_id);
		clif_damage(*src, *target, tick, 0, 0, 1000, 0, DMG_NORMAL, 0, false);
		if (!status_isdead(*target))
		{
			uint16 where[] = { EQP_ARMOR, EQP_SHIELD, EQP_HELM };
			skill_break_equip(src, target, where[rnd() % 3], 10000, BCT_ENEMY);
		}
		break;
	}
	case 5: // STRENGTH - atk halved
	{
		sc_start(src, target, SC_INCATKRATE, 100, -50, skill_get_time2(skill_id, skill_lv));
		break;
	}
	case 6: // THE LOVERS - 2000HP heal, random teleported
	{
		status_heal(target, 2000, 0, 0);
		if (!map_flag_vs(target->m))
			unit_warp(target, -1, -1, -1, CLR_TELEPORT);
		break;
	}
	case 7: // WHEEL OF FORTUNE - random 2 other effects
	{
		// Recursive call
		skill_tarotcard(src, target, skill_id, skill_lv, tick);
		skill_tarotcard(src, target, skill_id, skill_lv, tick);
		break;
	}
	case 8: // THE HANGED MAN - ankle, freeze or stoned
	{
		enum sc_type sc[] = { SC_ANKLE, SC_FREEZE, SC_STONEWAIT };
		uint8 rand_eff = rnd() % 3;
		int32 time = ((rand_eff == 0) ? skill_get_time2(skill_id, skill_lv) : skill_get_time2(status_db.getSkill(sc[rand_eff]), 1));

		if (sc[rand_eff] == SC_STONEWAIT)
			sc_start2(src, target, SC_STONEWAIT, 100, skill_lv, src->id, time, skill_get_time(status_db.getSkill(SC_STONEWAIT), 1));
		else
			sc_start(src, target, sc[rand_eff], 100, skill_lv, time);
		break;
	}
	case 9: // DEATH - curse, coma and poison
	{
		status_change_start(src, target, SC_COMA, 10000, skill_lv, 0, src->id, 0, 0, SCSTART_NONE);
		sc_start(src, target, SC_CURSE, 100, skill_lv, skill_get_time2(status_db.getSkill(SC_CURSE), 1));
		sc_start2(src, target, SC_POISON, 100, skill_lv, src->id, skill_get_time2(status_db.getSkill(SC_POISON), 1));
		break;
	}
	case 10: // TEMPERANCE - confusion
	{
		sc_start(src, target, SC_CONFUSION, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
		break;
	}
	case 11: // THE DEVIL - 6666 damage, atk and matk halved, cursed
	{
		battle_fix_damage(src, target, 6666, 0, skill_id);
		clif_damage(*src, *target, tick, 0, 0, 6666, 0, DMG_NORMAL, 0, false);
		sc_start(src, target, SC_INCATKRATE, 100, -50, skill_get_time2(skill_id, skill_lv));
		sc_start(src, target, SC_INCMATKRATE, 100, -50, skill_get_time2(skill_id, skill_lv));
		sc_start(src, target, SC_CURSE, skill_lv, 100, skill_get_time2(status_db.getSkill(SC_CURSE), 1));
		break;
	}
	case 12: // THE TOWER - 4444 damage
	{
		battle_fix_damage(src, target, 4444, 0, skill_id);
		clif_damage(*src, *target, tick, 0, 0, 4444, 0, DMG_NORMAL, 0, false);
		break;
	}
	case 13: // THE STAR - stun
	{
		sc_start(src, target, SC_STUN, 100, skill_lv, skill_get_time2(status_db.getSkill(SC_STUN), 1));
		break;
	}
	default: // THE SUN - atk, matk, hit, flee and def reduced, immune to more tarot card effects
	{
#ifdef RENEWAL
		//In renewal, this card gives the SC_TAROTCARD status change which makes you immune to other cards
		sc_start(src, target, SC_TAROTCARD, 100, skill_lv, skill_get_time2(skill_id, skill_lv));
#endif
		sc_start(src, target, SC_INCATKRATE, 100, -20, skill_get_time2(skill_id, skill_lv));
		sc_start(src, target, SC_INCMATKRATE, 100, -20, skill_get_time2(skill_id, skill_lv));
		sc_start(src, target, SC_INCHITRATE, 100, -20, skill_get_time2(skill_id, skill_lv));
		sc_start(src, target, SC_INCFLEERATE, 100, -20, skill_get_time2(skill_id, skill_lv));
		sc_start(src, target, SC_INCDEFRATE, 100, -20, skill_get_time2(skill_id, skill_lv));
		return 14; //To make sure a valid number is returned
	}
	}

	return card;
}

SkillUnbarringOctave::SkillUnbarringOctave() : SkillImpl(BA_FROSTJOKER) {
}

void SkillUnbarringOctave::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	int32 rate = 150 + 50 * skill_lv; // Aegis accuracy (1000 = 100%)
	int32 duration = skill_get_time2(getSkillId(), skill_lv);
	if (battle_check_target(src, target, BCT_PARTY) > 0) {
		// On party members: Chance is divided by 4 and duration is fixed to 15000ms
		rate /= 4;
		duration = skill_get_time(getSkillId(), skill_lv);
	}
	status_change_start(src, target, skill_get_sc(getSkillId()), rate*10, skill_lv, 0, 0, 0, duration, SCSTART_NONE);
}

void SkillUnbarringOctave::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data *md = BL_CAST(BL_MOB, src);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_addtimerskill(src,tick+3000,target->id,src->x,src->y,getSkillId(),skill_lv,0,flag);

	if (md) {
		// custom hack to make the mob display the skill, because these skills don't show the skill use text themselves
		//NOTE: mobs don't have the sprite animation that is used when performing this skill (will cause glitches)
		char temp[70];
		snprintf(temp, sizeof(temp), "%s : %s !!",md->name,skill_get_desc(getSkillId()));
		clif_disp_overhead(md,temp);
	}
}

SkillUnchainedSerenade::SkillUnchainedSerenade() : WeaponSkillImpl(BA_DISSONANCE) {
}

void SkillUnchainedSerenade::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST( BL_PC, src );

	base_skillratio += 10 + skill_lv * 50;
	if (sd != nullptr)
		base_skillratio = base_skillratio * sd->status.job_level / 10;
#endif
}

void SkillUnchainedSerenade::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillUnchainedSerenade::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	// Ammo should be deleted right away.
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
#endif
}

SkillUnlimitedHummingVoice::SkillUnlimitedHummingVoice() : SkillImpl(WM_UNLIMITED_HUMMING_VOICE) {
}

void SkillUnlimitedHummingVoice::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( flag&1 ) {	// These affect all party members near the caster.
		if( sc && sc->getSCE(type) ) {
			sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv));
		}
	} else if( sd ) {
		if( sc_start2(src,target,type,100,skill_lv,pc_checkskill(sd, WM_LESSON),skill_get_time(getSkillId(),skill_lv)) )
			party_foreachsamemap(skill_area_sub,sd,skill_get_splash(getSkillId(),skill_lv),src,getSkillId(),skill_lv,tick,flag|BCT_PARTY|1,skill_castend_nodamage_id);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillValleyOfDeath::SkillValleyOfDeath() : SkillImpl(WM_DEADHILLHERE) {
}

void SkillValleyOfDeath::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	if( target->type == BL_PC ) {
		if( !status_isdead(*target) )
			return;

		tstatus->hp = max(tstatus->sp, 1);
		tstatus->sp -= tstatus->sp * ( 60 - 10 * skill_lv ) / 100;
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		pc_revive(reinterpret_cast<map_session_data*>(target),true,true);
		clif_resurrection( *target );
	}
}

SkillVerdureTrap::SkillVerdureTrap() : SkillImpl(RA_VERDURETRAP) {
}

void SkillVerdureTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillVoiceOfSiren::SkillVoiceOfSiren() : SkillImpl(WM_VOICEOFSIREN) {
}

void SkillVoiceOfSiren::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag&1)
		sc_start2(src,target,type,skill_area_temp[5],skill_lv,src->id,skill_area_temp[6]);
	else {
		// Success chance: (Skill Level x 6) + (Voice Lesson Skill Level x 2) + (Caster's Job Level / 2) %
		skill_area_temp[5] = skill_lv * 6 + ((sd) ? pc_checkskill(sd, WM_LESSON) : 1) * 2 + (sd ? sd->status.job_level : 50) / 2;
		skill_area_temp[6] = skill_get_time(getSkillId(),skill_lv);
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(),skill_lv), BL_CHAR|BL_SKILL, src, getSkillId(), skill_lv, tick, flag|BCT_ALL|BCT_WOS|1, skill_castend_nodamage_id);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillVulcanArrow::SkillVulcanArrow() : WeaponSkillImpl(CG_ARROWVULCAN) {
}

void SkillVulcanArrow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	skillratio += 400 + 100 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += 100 + 100 * skill_lv;
#endif
}

SkillWandOfHermode::SkillWandOfHermode() : SkillImpl(CG_HERMODE) {
}

void SkillWandOfHermode::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_castend_song(src, getSkillId(), skill_lv, tick);
#endif
}

void SkillWandOfHermode::castendPos2(block_list *src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32 &flag) const {
#ifndef RENEWAL
	skill_clear_unitgroup(src);
	if (auto sg = skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0); sg != nullptr)
		sc_start4(src, src, SC_DANCING, 100, getSkillId(), 0, skill_lv, sg->group_id, skill_get_time(getSkillId(), skill_lv));
	flag |= 1;
#endif
}

SkillWarcryOfBeyond::SkillWarcryOfBeyond() : SkillImpl(WM_BEYOND_OF_WARCRY) {
}

void SkillWarcryOfBeyond::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( flag&1 ) {
		sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
	} else {	// These affect to all targets around the caster.
		if( rnd()%100 < 12 + 3 * skill_lv + (sd ? pc_checkskill(sd, WM_LESSON) : 0) ) { // !TODO: What's the Lesson bonus?
			map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(),skill_lv),BL_PC, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
			clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		}
	}
}

SkillWargBite::SkillWargBite() : WeaponSkillImpl(RA_WUGBITE) {
}

void SkillWargBite::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 300 + 200 * skill_lv;
	if (skill_lv == 5)
		base_skillratio += 100;
}

void SkillWargBite::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( path_search(nullptr,src->m,src->x,src->y,target->x,target->y,1,CELL_CHKNOREACH) ) {
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	}else if( sd )
		clif_skill_fail( *sd, getSkillId() );
}

void SkillWargBite::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	int32 wug_rate = (50 + 10 * skill_lv) + 2 * ((sd) ? pc_checkskill(sd,RA_TOOTHOFWUG)*2 : skill_get_max(RA_TOOTHOFWUG)) - (status_get_agi(target) / 4);
	if (wug_rate < 50)
		wug_rate = 50;
	sc_start(src,target, SC_BITE, wug_rate, skill_lv, (skill_get_time(getSkillId(),skill_lv) + ((sd) ? pc_checkskill(sd,RA_TOOTHOFWUG)*500 : skill_get_max(RA_TOOTHOFWUG))) );
}

SkillWargDash::SkillWargDash() : SkillImplRecursiveDamageSplash(RA_WUGDASH) {
}

void SkillWargDash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	// ATK 300%
	base_skillratio += 200;
}

void SkillWargDash::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc && type != SC_NONE)?tsc->getSCE(type):nullptr;

	if( tsce ) {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,status_change_end(target, type));
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	if( sd && pc_isridingwug(sd) ) {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,sc_start4(src,target,type,100,skill_lv,unit_getdir(target),0,0,0));
		clif_walkok(*sd);
	}
}

SkillWargMastery::SkillWargMastery() : SkillImpl(RA_WUGMASTERY) {
}

void SkillWargMastery::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd ) {
		if( !pc_iswug(sd) )
			pc_setoption(sd,sd->sc.option|OPTION_WUG);
		else
			pc_setoption(sd,sd->sc.option&~OPTION_WUG);
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillWargRider::SkillWargRider() : SkillImpl(RA_WUGRIDER) {
}

void SkillWargRider::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd ) {
		if( !pc_isridingwug(sd) && pc_iswug(sd) ) {
			pc_setoption(sd,sd->sc.option&~OPTION_WUG);
			pc_setoption(sd,sd->sc.option|OPTION_WUGRIDER);
		} else if( pc_isridingwug(sd) ) {
			pc_setoption(sd,sd->sc.option&~OPTION_WUGRIDER);
			pc_setoption(sd,sd->sc.option|OPTION_WUG);
		}
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillWargStrike::SkillWargStrike() : WeaponSkillImpl(RA_WUGSTRIKE) {
}

void SkillWargStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 200 * skill_lv;
}

void SkillWargStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd && pc_isridingwug(sd) ){
		uint8 dir = map_calc_dir(target, src->x, src->y);

		if( unit_movepos(src, target->x+dirx[dir], target->y+diry[dir], 1, 1) ) {
			clif_blown(src);
			WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
		}
		return;
	}
	if( path_search(nullptr,src->m,src->x,src->y,target->x,target->y,1,CELL_CHKNOREACH) ) {
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	}
}

SkillWildWalk::SkillWildWalk() : WeaponSkillImpl(WH_WILD_WALK) {
}

void SkillWildWalk::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	skillratio += -100 + 1800 + 2800 * skill_lv;
	// !TODO: unknown con and WH_NATUREFRIENDLY/HT_STEELCROW skills ratio
	skillratio += 5 * sstatus->con;
	skillratio += skillratio * pc_checkskill(sd, WH_NATUREFRIENDLY) / 10;
	skillratio += skillratio * pc_checkskill(sd, HT_STEELCROW) / 10;
	RE_LVL_DMOD(100);
}

void SkillWildWalk::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(),skill_lv));
}

SkillWindmillRushAttack::SkillWindmillRushAttack() : SkillImpl(MI_RUSH_WINDMILL) {
}

void SkillWindmillRushAttack::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	uint16 lesson_lv = (sd != nullptr) ? pc_checkskill(sd, WM_LESSON) : skill_get_max(WM_LESSON);

	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) ) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		sc_start2(src, target, type, 100, skill_lv, lesson_lv, skill_get_time(getSkillId(), skill_lv));
	} else {
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
		sc_start2(src, target, type, 100, skill_lv, lesson_lv, skill_get_time(getSkillId(), skill_lv));
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillWindWalker::SkillWindWalker() : SkillImpl(SN_WINDWALK) {
}

void SkillWindWalker::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) )
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillWinkofCharm::SkillWinkofCharm() : SkillImpl(DC_WINKCHARM) {
}

void SkillWinkofCharm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if( dstsd ) {
#ifdef RENEWAL
		// In Renewal it causes Confusion and Hallucination to 100% base chance
		sc_start(src, target, SC_CONFUSION, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
		sc_start(src, target, SC_HALLUCINATION, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
#else
		// In Pre-Renewal it only causes Wink Charm, if Confusion was successfully started
		if (sc_start(src, target, SC_CONFUSION, 10, skill_lv, skill_get_time(getSkillId(), skill_lv)))
			sc_start(src, target, type, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
#endif
	} else
	if( dstmd )
	{
		// For monsters it causes Wink Charm with a chance depending on the level difference
		if (sc_start2(src, target, type, (status_get_lv(src) - status_get_lv(target)) + 40, skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv))) {
			// This triggers a 0 damage event and might make the monster switch target to caster
			battle_damage(src, target, 0, 1, skill_lv, 0, ATK_DEF, BF_WEAPON|BF_LONG|BF_NORMAL, true, tick, false);
		}
	}
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

std::unique_ptr<const SkillImpl> SkillFactoryArcher::create(const e_skill skill_id) const {
	switch( skill_id ){
		case AC_CHARGEARROW:
			return std::make_unique<SkillChargeArrow>();
		case AC_CONCENTRATION:
			return std::make_unique<SkillConcentration>();
		case AC_DOUBLE:
			return std::make_unique<SkillDoubleStrafe>();
		case AC_MAKINGARROW:
			return std::make_unique<SkillMakingArrow>();
		case AC_SHOWER:
			return std::make_unique<SkillArrowShower>();
		case BA_APPLEIDUN:
			return std::make_unique<SkillSongofLutie>();
		case BA_ASSASSINCROSS:
			return std::make_unique<SkillImpressiveRiff>();
		case BA_DISSONANCE:
			return std::make_unique<SkillUnchainedSerenade>();
		case BA_FROSTJOKER:
			return std::make_unique<SkillUnbarringOctave>();
		case BA_MUSICALSTRIKE:
			return std::make_unique<SkillMelodyStrike>();
		case BA_PANGVOICE:
			return std::make_unique<SkillPangVoice>();
		case BA_POEMBRAGI:
			return std::make_unique<SkillMagicStrings>();
		case BA_WHISTLE:
			return std::make_unique<SkillPerfectTablature>();
		case BD_ADAPTATION:
			return std::make_unique<SkillAmp>();
		case BD_DRUMBATTLEFIELD:
			return std::make_unique<SkillBattleTheme>();
		case BD_ENCORE:
			return std::make_unique<SkillEncore>();
		case BD_ETERNALCHAOS:
			return std::make_unique<SkillDownTempo>();
		case BD_INTOABYSS:
			return std::make_unique<SkillPowerChord>();
		case BD_LULLABY:
			return std::make_unique<SkillLullaby>();
		case BD_RICHMANKIM:
			return std::make_unique<SkillMentalSensing>();
		case BD_RINGNIBELUNGEN:
			return std::make_unique<SkillHarmonicLick>();
		case BD_ROKISWEIL:
			return std::make_unique<SkillClassicalPluck>();
		case BD_SIEGFRIED:
			return std::make_unique<SkillAcousticRhythm>();
		case CG_ARROWVULCAN:
			return std::make_unique<SkillVulcanArrow>();
		case CG_HERMODE:
			return std::make_unique<SkillWandOfHermode>();
		case CG_LONGINGFREEDOM:
			return std::make_unique<SkillLongingForFreedom>();
		case CG_MARIONETTE:
			return std::make_unique<SkillMarionetteControl>();
		case CG_MOONLIT:
			return std::make_unique<SkillShelteringBliss>();
		case CG_SPECIALSINGER:
			return std::make_unique<SkillSkilledSpecialSinger>();
		case CG_TAROTCARD:
			return std::make_unique<SkillTarotCardOfFate>();
		case DC_DONTFORGETME:
			return std::make_unique<SkillSlowGrace>();
		case DC_FORTUNEKISS:
			return std::make_unique<SkillLadyLuck>();
		case DC_HUMMING:
			return std::make_unique<SkillFocusBallet>();
		case DC_SCREAM:
			return std::make_unique<SkillDazzler>();
		case DC_SERVICEFORYOU:
			return std::make_unique<SkillGypsysKiss>();
		case DC_THROWARROW:
			return std::make_unique<SkillSlingingArrow>();
		case DC_UGLYDANCE:
			return std::make_unique<SkillHipShaker>();
		case DC_WINKCHARM:
			return std::make_unique<SkillWinkofCharm>();
		case HT_ANKLESNARE:
			return std::make_unique<SkillAnkleSnare>();
		case HT_BLASTMINE:
			return std::make_unique<SkillBlastMine>();
		case HT_BLITZBEAT:
			return std::make_unique<SkillBlitzBeat>();
		case HT_CLAYMORETRAP:
			return std::make_unique<SkillClaymoreTrap>();
		case HT_DETECTING:
			return std::make_unique<SkillDetect>();
		case HT_FLASHER:
			return std::make_unique<SkillFlasher>();
		case HT_FREEZINGTRAP:
			return std::make_unique<SkillFreezingTrap>();
		case HT_LANDMINE:
			return std::make_unique<SkillLandMine>();
		case HT_PHANTASMIC:
			return std::make_unique<SkillPhantasmicArrow>();
		case HT_POWER:
			return std::make_unique<SkillBeastStrafing>();
		case HT_REMOVETRAP:
			return std::make_unique<SkillRemoveTrap>();
		case HT_SANDMAN:
			return std::make_unique<SkillSandman>();
		case HT_SHOCKWAVE:
			return std::make_unique<SkillShockwaveTrap>();
		case HT_SKIDTRAP:
			return std::make_unique<SkillSkidTrap>();
		case HT_SPRINGTRAP:
			return std::make_unique<SkillSpringTrap>();
		case HT_TALKIEBOX:
			return std::make_unique<SkillTalkieBox>();
		case MI_ECHOSONG:
			return std::make_unique<SkillEchoSong>();
		case MI_HARMONIZE:
			return std::make_unique<SkillHarmonize>();
		case MI_RUSH_WINDMILL:
			return std::make_unique<SkillWindmillRushAttack>();
		case RA_AIMEDBOLT:
			return std::make_unique<SkillAimedBolt>();
		case RA_ARROWSTORM:
			return std::make_unique<SkillArrowStorm>();
		case RA_CAMOUFLAGE:
			return std::make_unique<SkillCamouflage>();
		case RA_CLUSTERBOMB:
			return std::make_unique<SkillClusterBomb>();
		case RA_COBALTTRAP:
			return std::make_unique<SkillCobaltTrap>();
		case RA_DETONATOR:
			return std::make_unique<SkillDetonator>();
		case RA_ELECTRICSHOCKER:
			return std::make_unique<SkillElectricShocker>();
		case RA_FEARBREEZE:
			return std::make_unique<SkillFearBreeze>();
		case RA_FIRINGTRAP:
			return std::make_unique<SkillFiringTrap>();
		case RA_ICEBOUNDTRAP:
			return std::make_unique<SkillIceboundTrap>();
		case RA_MAGENTATRAP:
			return std::make_unique<SkillMagentaTrap>();
		case RA_MAIZETRAP:
			return std::make_unique<SkillMaizeTrap>();
		case RA_SENSITIVEKEEN:
			return std::make_unique<SkillSensitiveKeen>();
		case RA_UNLIMIT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case RA_VERDURETRAP:
			return std::make_unique<SkillVerdureTrap>();
		case RA_WUGBITE:
			return std::make_unique<SkillWargBite>();
		case RA_WUGDASH:
			return std::make_unique<SkillWargDash>();
		case RA_WUGMASTERY:
			return std::make_unique<SkillWargMastery>();
		case RA_WUGRIDER:
			return std::make_unique<SkillWargRider>();
		case RA_WUGSTRIKE:
			return std::make_unique<SkillWargStrike>();
		case SN_FALCONASSAULT:
			return std::make_unique<SkillFalconAssault>();
		case SN_SHARPSHOOTING:
			return std::make_unique<SkillFocusedArrowStrike>();
		case SN_SIGHT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SN_WINDWALK:
			return std::make_unique<SkillWindWalker>();
		case TR_AIN_RHAPSODY:
			return std::make_unique<SkillAinRhapsody>();
		case TR_GEF_NOCTURN:
			return std::make_unique<SkillGeffeniaNocturn>();
		case TR_JAWAII_SERENADE:
			return std::make_unique<SkillJawaiiSerenade>();
		case TR_KVASIR_SONATA:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case TR_METALIC_FURY:
			return std::make_unique<SkillMetallicFury>();
		case TR_MUSICAL_INTERLUDE:
			return std::make_unique<SkillMusicalInterlude>();
		case TR_MYSTIC_SYMPHONY:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case TR_NIPELHEIM_REQUIEM:
			return std::make_unique<SkillNipelheimRequiem>();
		case TR_PRON_MARCH:
			return std::make_unique<SkillPronMarch>();
		case TR_RETROSPECTION:
			return std::make_unique<SkillRetrospection>();
		case TR_RHYTHMICAL_WAVE:
			return std::make_unique<SkillRhythmicalWave>();
		case TR_RHYTHMSHOOTING:
			return std::make_unique<SkillRhythmShooting>();
		case TR_ROKI_CAPRICCIO:
			return std::make_unique<SkillRokiCapriccio>();
		case TR_ROSEBLOSSOM:
			return std::make_unique<SkillRoseBlossom>();
		case TR_ROSEBLOSSOM_ATK:
			return std::make_unique<SkillRoseBlossomAttack>();
		case TR_SOUNDBLEND:
			return std::make_unique<SkillSoundBlend>();
		case WA_MOONLIT_SERENADE:
			return std::make_unique<SkillMoonlitSerenade>();
		case WA_SWING_DANCE:
			return std::make_unique<SkillSwingDance>();
		case WA_SYMPHONY_OF_LOVER:
			return std::make_unique<SkillSymphonyOfLovers>();
		case WH_CALAMITYGALE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case WH_CRESCIVE_BOLT:
			return std::make_unique<SkillCresciveBolt>();
		case WH_DEEPBLINDTRAP:
			return std::make_unique<SkillDeepBlindTrap>();
		case WH_FLAMETRAP:
			return std::make_unique<SkillFlameTrap>();
		case WH_GALESTORM:
			return std::make_unique<SkillGaleStorm>();
		case WH_HAWKBOOMERANG:
			return std::make_unique<SkillHawkBoomerang>();
		case WH_HAWKRUSH:
			return std::make_unique<SkillHawkRush>();
		case WH_HAWK_M:
			return std::make_unique<SkillHawkMastery>();
		case WH_SOLIDTRAP:
			return std::make_unique<SkillSolidTrap>();
		case WH_SWIFTTRAP:
			return std::make_unique<SkillSwiftTrap>();
		case WH_WILD_WALK:
			return std::make_unique<SkillWildWalk>();
		case WH_WIND_SIGN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case WM_BEYOND_OF_WARCRY:
			return std::make_unique<SkillWarcryOfBeyond>();
		case WM_DANCE_WITH_WUG:
			return std::make_unique<SkillDanceWithAWarg>();
		case WM_DEADHILLHERE:
			return std::make_unique<SkillValleyOfDeath>();
		case WM_DOMINION_IMPULSE:
			return std::make_unique<SkillDominionImpulse>();
		case WM_FRIGG_SONG:
			return std::make_unique<SkillFriggsSong>();
		case WM_GLOOMYDAY:
			return std::make_unique<SkillGloomyDay>();
		case WM_GREAT_ECHO:
			return std::make_unique<SkillGreatEcho>();
		case WM_LERADS_DEW:
			return std::make_unique<SkillLeradsDew>();
		case WM_LULLABY_DEEPSLEEP:
			return std::make_unique<SkillDeepSleepLullaby>();
		case WM_MELODYOFSINK:
			return std::make_unique<SkillMelodyOfSink>();
		case WM_METALICSOUND:
			return std::make_unique<SkillMetallicSound>();
		case WM_POEMOFNETHERWORLD:
			return std::make_unique<SkillPoemOfTheNetherworld>();
		case WM_RANDOMIZESPELL:
			return std::make_unique<SkillImprovisedSong>();
		case WM_REVERBERATION:
			return std::make_unique<SkillReverberation>();
		case WM_SATURDAY_NIGHT_FEVER:
			return std::make_unique<SkillSaturdayNightFever>();
		case WM_SEVERE_RAINSTORM:
			return std::make_unique<SkillSevereRainstorm>();
		case WM_SEVERE_RAINSTORM_MELEE:
			return std::make_unique<SkillSevereRainstormMelee>();
		case WM_SIRCLEOFNATURE:
			return std::make_unique<SkillCircleOfNaturesSound>();
		case WM_SONG_OF_MANA:
			return std::make_unique<SkillSongOfMana>();
		case WM_SOUND_OF_DESTRUCTION:
			return std::make_unique<SkillSoundOfDestruction>();
		case WM_UNLIMITED_HUMMING_VOICE:
			return std::make_unique<SkillUnlimitedHummingVoice>();
		case WM_VOICEOFSIREN:
			return std::make_unique<SkillVoiceOfSiren>();

		default:
			return nullptr;
	}
	return nullptr;
}

#endif
