// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_thief.hpp"

#include <config/core.hpp>
#include "map/clif.hpp"
#include "map/status.hpp"
#include "map/pc.hpp"
#include "map/unit.hpp"
#include "map/map.hpp"
#include "map/path.hpp"
#include "map/battle.hpp"
#include "map/log.hpp"
#include <common/random.hpp>
#include "map/mob.hpp"
#include <common/utils.hpp>
#include "skill_impl.hpp"

SkillAbyssDagger::SkillAbyssDagger() : SkillImplRecursiveDamageSplash(ABC_ABYSS_DAGGER) {
}

void SkillAbyssDagger::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillAbyssDagger::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 350 + 1400 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

SkillAbyssFlame::SkillAbyssFlame() : SkillImpl(ABC_ABYSS_FLAME) {
}

void SkillAbyssFlame::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE);
		skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
	} else {
		map_foreachinrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR | BL_SKILL, src, getSkillId(), skill_lv, tick, (flag | BCT_ENEMY | SD_SPLASH | 1 ) & ~BCT_SELF, skill_castend_damage_id);
		skill_castend_damage_id(src, target, ABC_ABYSS_FLAME_ATK, skill_lv, tick, flag);
	}
}

void SkillAbyssFlame::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 820 * skill_lv;
	skillratio += 10 * sstatus->spl;
	skillratio += 30 * skill_lv * pc_checkskill(sd, ABC_MAGIC_SWORD_M);
	RE_LVL_DMOD(100);
}

SkillAbyssFlameAttack::SkillAbyssFlameAttack() : SkillImplRecursiveDamageSplash(ABC_ABYSS_FLAME_ATK) {
}

void SkillAbyssFlameAttack::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 500 * skill_lv;
	skillratio += 10 * sstatus->spl;
	skillratio += 15 * skill_lv * pc_checkskill(sd, ABC_MAGIC_SWORD_M);
	RE_LVL_DMOD(100);
}

void SkillAbyssFlameAttack::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillAbyssSquare::SkillAbyssSquare() : SkillImpl(ABC_ABYSS_SQUARE) {
}

void SkillAbyssSquare::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	if (dmg.miscflag == 2)
		dmg.div_ = 2;
}

void SkillAbyssSquare::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).

	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillAbyssSquare::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 900 * skill_lv;
	skillratio += 50 * pc_checkskill(sd, ABC_MAGIC_SWORD_M) * skill_lv;
	skillratio += 5 * sstatus->spl;

	RE_LVL_DMOD(100);
}

SkillAntidote::SkillAntidote() : SkillImpl(GC_ANTIDOTE) {
}

void SkillAntidote::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	if( tsc )
	{
		status_change_end(target, SC_PARALYSE);
		status_change_end(target, SC_PYREXIA);
		status_change_end(target, SC_DEATHHURT);
		status_change_end(target, SC_LEECHESEND);
		status_change_end(target, SC_VENOMBLEED);
		status_change_end(target, SC_MAGICMUSHROOM);
		status_change_end(target, SC_TOXIN);
		status_change_end(target, SC_OBLIVIONCURSE);
	}
}

SkillAutoShadowSpell::SkillAutoShadowSpell() : SkillImpl(SC_AUTOSHADOWSPELL) {
}

void SkillAutoShadowSpell::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		if( (sd->reproduceskill_idx > 0 && sd->status.skill[sd->reproduceskill_idx].id) ||
			(sd->cloneskill_idx > 0 && sd->status.skill[sd->cloneskill_idx].id) )
		{
			sc_start(src,src,SC_STOP,100,skill_lv,INFINITE_TICK);// The skill_lv is stored in val1 used in skill_select_menu to determine the used skill lvl [Xazax]
			clif_autoshadowspell_list( *sd );
			clif_skill_nodamage(src,*target,getSkillId(),1);
		}
		else
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_IMITATION_SKILL_NONE );
	}
}

SkillBackSlide::SkillBackSlide() : SkillImpl(TF_BACKSLIDING) {
}

void SkillBackSlide::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32 &flag) const {
	//This is the correct implementation as per packet logging information. [Skotlex]

	// Backsliding makes you immune to being stopped for 200ms, but only if you don't have the endure effect yet
	if (unit_data *ud = unit_bl2ud(bl); ud != nullptr && !status_isendure(*bl, tick, true))
		ud->endure_tick = tick + 200;

#ifdef RENEWAL
	int16 blew_count = skill_blown(src, bl, skill_get_blewcount(getSkillId(), skill_lv), unit_getdir(bl),
	                               static_cast<enum e_skill_blown>(BLOWN_IGNORE_NO_KNOCKBACK | BLOWN_DONT_SEND_PACKET));
	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);

	if (blew_count > 0)
		clif_blown(src); // Always blow, otherwise it shows a casting animation. [Lemongrass]
#else
	int16 blew_count = skill_blown(src, bl, skill_get_blewcount(getSkillId(), skill_lv), unit_getdir(bl), BLOWN_IGNORE_NO_KNOCKBACK);
	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
	clif_slide(*bl, bl->x, bl->y); //Show the casting animation on pre-re
#endif
}

SkillBackStab::SkillBackStab() : SkillImpl(RG_BACKSTAP) {
}

void SkillBackStab::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->status.weapon == W_DAGGER)
		dmg.div_ = 2;
#endif
}

void SkillBackStab::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST( BL_PC, src );

	if(sd && sd->status.weapon == W_BOW && battle_config.backstab_bow_penalty)
		base_skillratio += (200 + 40 * skill_lv) / 2;
	else
		base_skillratio += 200 + 40 * skill_lv;
}

void SkillBackStab::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

#ifdef RENEWAL
	uint8 dir = map_calc_dir(src, target->x, target->y);
	int16 x, y;

	if (dir > 0 && dir < 4)
		x = -1;
	else if (dir > 4)
		x = 1;
	else
		x = 0;

	if (dir > 2 && dir < 6)
		y = -1;
	else if (dir == 7 || dir < 2)
		y = 1;
	else
		y = 0;

	if (battle_check_target(src, target, BCT_ENEMY) > 0 && unit_movepos(src, target->x + x, target->y + y, 2, true)) { // Display movement + animation.
#else
	if (check_distance_bl(src, target, 0))
		return;

	uint8 dir = map_calc_dir(src, target->x, target->y), t_dir = unit_getdir(target);

	if (!map_check_dir(dir, t_dir) || target->type == BL_SKILL) {
#endif
		status_change_end(src, SC_HIDING);
		dir = dir < 4 ? dir+4 : dir-4; // change direction [Celest]
		unit_setdir(target,dir);
#ifdef RENEWAL
		clif_blown(src);
#endif
		skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
	}
	else if (sd)
		clif_skill_fail( *sd, getSkillId() );
}

void SkillBackStab::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
#ifdef RENEWAL
	sc_start(src,target,SC_STUN,(5+2*skill_lv),skill_lv,skill_get_time(getSkillId(),skill_lv));
#endif
}

void SkillBackStab::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
#ifdef RENEWAL
	hit_rate += skill_lv; // !TODO: What's the rate increase?
#endif
}

SkillBloodyLust::SkillBloodyLust() : SkillImpl(SC_BLOODYLUST) {
}

void SkillBloodyLust::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillBodyPainting::SkillBodyPainting() : SkillImpl(SC_BODYPAINT) {
}

void SkillBodyPainting::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	if( flag&1 ) {
		if (tsc && ((tsc->option&(OPTION_HIDE|OPTION_CLOAK)) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC_STEALTHFIELD))) {
			status_change_end(target,SC_HIDING);
			status_change_end(target,SC_CLOAKING);
			status_change_end(target,SC_CLOAKINGEXCEED);
			status_change_end(target,SC_CAMOUFLAGE);
			status_change_end(target,SC_NEWMOON);
			if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
				status_change_end(target, SC__SHADOWFORM);
		}
		// Attack Speed decrease and Blind happen to everyone around caster, not just hidden targets.
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
		sc_start(src, target, SC_BLIND, 53 + 2 * skill_lv, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), 0);
		map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR,
			src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
	}
}

SkillChainReactionShot::SkillChainReactionShot() : SkillImplRecursiveDamageSplash(ABC_CHAIN_REACTION_SHOT) {
}

void SkillChainReactionShot::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 850 * skill_lv;
	skillratio += 15 * sstatus->con;
	RE_LVL_DMOD(100);
}

void SkillChainReactionShot::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	map_foreachinrange(skill_area_sub, target, skill_get_splash(ABC_CHAIN_REACTION_SHOT_ATK, skill_lv), BL_CHAR | BL_SKILL, src, ABC_CHAIN_REACTION_SHOT_ATK, skill_lv, tick + (200 + status_get_amotion(src)), flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillChainReactionShotAttack::SkillChainReactionShotAttack() : WeaponSkillImpl(ABC_CHAIN_REACTION_SHOT_ATK) {
}

void SkillChainReactionShotAttack::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	if (skill_lv == 4)
		skillratio += -100 + 11500;
	else
		skillratio += -100 + 950 + 2650 * skill_lv;
	skillratio += 15 * sstatus->con;
	if (sc != nullptr && sc->hasSCE(SC_CHASING))
		skillratio += 1100 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillChaosPanic::SkillChaosPanic() : SkillImpl(SC_CHAOSPANIC) {
}

void SkillChaosPanic::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillChasingBreak::SkillChasingBreak() : SkillImplRecursiveDamageSplash(ABC_CHASING_BREAK) {
}

void SkillChasingBreak::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_CHASING))
		dmg.div_ = 7;
}

void SkillChasingBreak::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1550 + 450 * skill_lv;
	skillratio += 5 * sstatus->pow;
	if (sc != nullptr && sc->hasSCE(SC_CHASING))
		skillratio += 200 + 50 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillChasingBreak::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	uint8 dir = DIR_NORTHEAST;

	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);

	if (skill_check_unit_movepos(0, src, target->x + dirx[dir], target->y + diry[dir], 1, 1))
		clif_blown(src);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, 1);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillChasingShot::SkillChasingShot() : SkillImplRecursiveDamageSplash(ABC_CHASING_SHOT) {
}

void SkillChasingShot::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_CHASING))
		dmg.div_ = 3;
}

void SkillChasingShot::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1750 + 850 * skill_lv;
	skillratio += 5 * sstatus->con;
	if (sc != nullptr && sc->hasSCE(SC_CHASING))
		skillratio += 250 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillChasingShot::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	uint8 dir = DIR_NORTHEAST;

	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);

	if (skill_check_unit_movepos(0, src, target->x + dirx[dir], target->y + diry[dir], 1, 1))
		clif_blown(src);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, 1);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillCloaking::SkillCloaking() : SkillImpl(AS_CLOAKING) {
}

void SkillCloaking::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc && type != SC_NONE)?tsc->getSCE(type):nullptr;
	int32 i = 0;

	if (tsce) {
		i = status_change_end(target, type);
		if( i )
			clif_skill_nodamage(src,*target,getSkillId(),-1,i);
		else if( sd )
			clif_skill_fail( *sd, getSkillId() );
		return;
	}
	i = sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
	if( i )
		clif_skill_nodamage(src,*target,getSkillId(),-1,i);
	else if( sd )
		clif_skill_fail( *sd, getSkillId(),  USESKILL_FAIL_LEVEL );
}

SkillCloakingExceed::SkillCloakingExceed() : SkillImpl(GC_CLOAKINGEXCEED) {
}

void SkillCloakingExceed::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;
	map_session_data* sd = BL_CAST( BL_PC, src );
	int32 i = 0;

	if (tsce) {
		i = status_change_end(target, type);
		if( i )
			clif_skill_nodamage(src,*target,getSkillId(),-1,i);
		else if( sd )
			clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	i = sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
	if( i )
		clif_skill_nodamage(src,*target,getSkillId(),-1,i);
	else if( sd )
		clif_skill_fail( *sd, getSkillId(),  USESKILL_FAIL_LEVEL );
}

SkillCloseConfine::SkillCloseConfine() : SkillImpl(RG_CLOSECONFINE) {
}

void SkillCloseConfine::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start4(src,target,type,100,skill_lv,src->id,0,0,skill_get_time(getSkillId(),skill_lv)));
}

SkillCounterInstinct::SkillCounterInstinct() : StatusSkillImpl(ST_REJECTSWORD) {
}

void SkillCounterInstinct::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_AUTOCOUNTER,(skill_lv*15),skill_lv,skill_get_time(getSkillId(),skill_lv));
}

SkillCounterSlash::SkillCounterSlash() : SkillImplRecursiveDamageSplash(GC_COUNTERSLASH) {
}

void SkillCounterSlash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST( BL_PC, src );

	//ATK [{(Skill Level x 150) + 300} x Caster's Base Level / 120]% + ATK [(AGI x 2) + (Caster's Job Level x 4)]%
	skillratio += -100 + 300 + 150 * skill_lv;
	RE_LVL_DMOD(120);
	skillratio += sstatus->agi * 2;
	// If 4th job, job level of your 3rd job counts
	skillratio += (sd ? (sd->class_&JOBL_FOURTH ? sd->change_level_4th : sd->status.job_level) * 4 : 0);
}

void SkillCounterSlash::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillCreateDeadlyPoison::SkillCreateDeadlyPoison() : SkillImpl(ASC_CDP) {
}

void SkillCreateDeadlyPoison::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if(sd) {
		if(skill_produce_mix(sd, getSkillId(), ITEMID_POISON_BOTTLE, 0, 0, 0, 1, -1)) //Produce a Poison Bottle.
			clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		else
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_STUFF_INSUFFICIENT );
	}
}

SkillCreateNewPoison::SkillCreateNewPoison() : SkillImpl(GC_CREATENEWPOISON) {
}

void SkillCreateNewPoison::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd )
	{
		clif_skill_produce_mix_list( *sd, getSkillId(), 25 );
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillCrossImpact::SkillCrossImpact() : WeaponSkillImpl(GC_CROSSIMPACT) {
}

void SkillCrossImpact::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 1400 + 150 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillCrossImpact::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	uint8 dir = DIR_NORTHEAST;

	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);	// dir based on target as we move player based on target location

	if (skill_check_unit_movepos(0, src, target->x + dirx[dir], target->y + diry[dir], 1, 1)) {
		clif_blown(src);
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	} else {
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
	}
}

SkillCrossRipperSlasher::SkillCrossRipperSlasher() : WeaponSkillImpl(GC_CROSSRIPPERSLASHER) {
}

void SkillCrossRipperSlasher::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 80 * skill_lv + (sstatus->agi * 3);
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_ROLLINGCUTTER))
		skillratio += sc->getSCE(SC_ROLLINGCUTTER)->val1 * 200;
}

void SkillCrossRipperSlasher::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd && !(sc && sc->getSCE(SC_ROLLINGCUTTER)) )
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_CONDITION );
	else
	{
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	}
}

SkillCrossSlash::SkillCrossSlash() : SkillImplRecursiveDamageSplash(SHC_CROSS_SLASH) {
}

void SkillCrossSlash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 300 * skill_lv;
	skillratio += 5 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_SHADOW_EXCEED ) ) {
		skillratio += 60 * skill_lv;
		skillratio += 2 * sstatus->pow;
	}
	RE_LVL_DMOD(100);
}

void SkillCrossSlash::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillDancingKnife::SkillDancingKnife() : SkillImplRecursiveDamageSplash(SHC_DANCING_KNIFE) {
}

void SkillDancingKnife::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 200 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillDancingKnife::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	if (flag & 1) {
		skill_area_temp[1] = 0;

		// Note: doesn't force player to stand before attacking
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR | BL_SKILL, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_LEVEL | SD_SPLASH, skill_castend_damage_id);
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	}
}

SkillDarkClaw::SkillDarkClaw() : WeaponSkillImpl(GC_DARKCROW) {
}

void SkillDarkClaw::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv - 1);
}

void SkillDarkClaw::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	sc_start(src, target, SC_DARKCROW, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)); // Should be applied even on miss
}

SkillDarkIllusion::SkillDarkIllusion() : WeaponSkillImpl(GC_DARKILLUSION) {
}

void SkillDarkIllusion::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int16 x, y;
	int16 dir = map_calc_dir(src,target->x,target->y);

	if( dir > 0 && dir < 4) x = 2;
	else if( dir > 4 ) x = -2;
	else x = 0;
	if( dir > 2 && dir < 6 ) y = 2;
	else if( dir == 7 || dir < 2 ) y = -2;
	else y = 0;

	if( unit_movepos(src, target->x+x, target->y+y, 1, 1) ) {
		clif_blown(src);
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);

		if( rnd()%100 < 4 * skill_lv )
			skill_castend_damage_id(src,target,GC_CROSSIMPACT,skill_lv,tick,flag);
	}
}

SkillDeftStab::SkillDeftStab() : SkillImplRecursiveDamageSplash(ABC_DEFT_STAB) {
}

void SkillDeftStab::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 700 + 550 * skill_lv;
	skillratio += 7 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillDeftStab::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillDetoxify::SkillDetoxify() : SkillImpl(TF_DETOXIFY) {
}

void SkillDetoxify::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32 &flag) const {
	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
	status_change_end(bl, SC_POISON);
	status_change_end(bl, SC_DPOISON);
}

SkillDimensionDoor::SkillDimensionDoor() : SkillImpl(SC_DIMENSIONDOOR) {
}

void SkillDimensionDoor::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillDivestAll::SkillDivestAll() : SkillImpl(ST_FULLSTRIP) {
}

void SkillDivestAll::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );

	bool i;

	//Special message when trying to use strip on FCP [Jobbie]
	if( sd && tsc && tsc->getSCE(SC_CP_WEAPON) && tsc->getSCE(SC_CP_HELM) && tsc->getSCE(SC_CP_ARMOR) && tsc->getSCE(SC_CP_SHIELD))
	{
		clif_gospel_info( *sd, 0x28 );
		return;
	}

	if( i = skill_strip_equip(src, target, getSkillId(), skill_lv) )
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i);

	//Nothing stripped.
	if( sd && !i )
		clif_skill_fail( *sd, getSkillId() );
}

SkillDivestArmor::SkillDivestArmor() : SkillImpl(RG_STRIPARMOR) {
}

void SkillDivestArmor::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	bool i = skill_strip_equip(src, target, getSkillId(), skill_lv);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i);

	//Nothing stripped.
	if( sd && !i )
		clif_skill_fail( *sd, getSkillId() );
}

SkillDivestHelm::SkillDivestHelm() : SkillImpl(RG_STRIPHELM) {
}

void SkillDivestHelm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	bool i = skill_strip_equip(src, target, getSkillId(), skill_lv);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i);

	//Nothing stripped.
	if( sd && !i )
		clif_skill_fail( *sd, getSkillId() );
}

SkillDivestShield::SkillDivestShield() : SkillImpl(RG_STRIPSHIELD) {
}

void SkillDivestShield::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	bool i = skill_strip_equip(src, target, getSkillId(), skill_lv);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i);

	//Nothing stripped.
	if( sd && !i )
		clif_skill_fail( *sd, getSkillId() );
}

SkillDivestWeapon::SkillDivestWeapon() : SkillImpl(RG_STRIPWEAPON) {
}

void SkillDivestWeapon::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	bool i = skill_strip_equip(src, target, getSkillId(), skill_lv);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i);

	//Nothing stripped.
	if( sd && !i )
		clif_skill_fail( *sd, getSkillId() );
}

SkillDoubleAttack::SkillDoubleAttack() : WeaponSkillImpl(TF_DOUBLE) {
}

void SkillDoubleAttack::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	// For NPC used skill.
	dmg.type = DMG_MULTI_HIT;
}

SkillEmergencyEscape::SkillEmergencyEscape() : SkillImpl(SC_ESCAPE) {
}

void SkillEmergencyEscape::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
	skill_blown(src, src, skill_get_blewcount(getSkillId(), skill_lv), unit_getdir(src), BLOWN_IGNORE_NO_KNOCKBACK); // Don't stop the caster from backsliding if special_state.no_knockback is active
	clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
	flag |= 1;
}

SkillEnchantDeadlyPoison::SkillEnchantDeadlyPoison() : StatusSkillImpl(ASC_EDP) {
}

void SkillEnchantDeadlyPoison::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// EDP also give +25% WATK poison pseudo element to user.
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);

#ifdef RENEWAL
	sc_start4(src, src, SC_SUB_WEAPONPROPERTY, 100, ELE_POISON, 25, getSkillId(), 0, skill_get_time(getSkillId(), skill_lv));
#else
	sc_start4(src, src, SC_WATK_ELEMENT, 100, ELE_POISON, 25, 0, 0, skill_get_time(getSkillId(), skill_lv));
#endif
}

SkillEnchantPoison::SkillEnchantPoison() : SkillImpl(AS_ENCHANTPOISON) {
}

void SkillEnchantPoison::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sc_start( src, target, type, 100, skill_lv, skill_get_time( getSkillId(), skill_lv ) ) ){
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}else{
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, false );

		if( sd != nullptr ){
			clif_skill_fail( *sd, getSkillId() );
		}
	}
}

SkillEnvenom::SkillEnvenom() : WeaponSkillImpl(TF_POISON) {
}

void SkillEnvenom::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	if (!sc_start2(src, target, SC_POISON, (4 * skill_lv + 10), skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv)) && sd)
		clif_skill_fail(*sd, getSkillId());
}

SkillEternalSlash::SkillEternalSlash() : WeaponSkillImpl(SHC_ETERNAL_SLASH) {
}

void SkillEternalSlash::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_change *sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_E_SLASH_COUNT))
		dmg.div_ = sc->getSCE(SC_E_SLASH_COUNT)->val1;
}

void SkillEternalSlash::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 300 * skill_lv + 2 * sstatus->pow;

	if( sc != nullptr && sc->getSCE( SC_SHADOW_EXCEED ) ){
		skillratio += 120 * skill_lv + sstatus->pow;
	}

	RE_LVL_DMOD(100);
}

void SkillEternalSlash::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);

	if( sc && sc->getSCE(SC_E_SLASH_COUNT) )
		sc_start(src, src, SC_E_SLASH_COUNT, 100, min( 5, 1 + sc->getSCE(SC_E_SLASH_COUNT)->val1 ), skill_get_time(getSkillId(), skill_lv));
	else
		sc_start(src, src, SC_E_SLASH_COUNT, 100, 1, skill_get_time(getSkillId(), skill_lv));
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillFatalMenace::SkillFatalMenace() : WeaponSkillImpl(SC_FATALMENACE) {
}

void SkillFatalMenace::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->weapontype1 == W_DAGGER)
		dmg.div_++;
}

void SkillFatalMenace::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += 120 * skill_lv + sstatus->agi; // !TODO: What's the AGI bonus?

	if( sc != nullptr && sc->getSCE( SC_ABYSS_DAGGER ) ){
		skillratio += 30 * skill_lv;
	}

	RE_LVL_DMOD(100);
}

void SkillFatalMenace::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 )
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	else {
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), splash_target(src), src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_damage_id);
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	}
}

void SkillFatalMenace::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	if (skill_lv < 6)
		hit_rate -= 35 - 5 * skill_lv;
	else if (skill_lv > 6)
		hit_rate += 5 * skill_lv - 30;
}

SkillFatalShadowCrow::SkillFatalShadowCrow() : SkillImplRecursiveDamageSplash(SHC_FATAL_SHADOW_CROW) {
}

void SkillFatalShadowCrow::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	sc_start( src, target, SC_DARKCROW, 100, max( 1, pc_checkskill( sd, GC_DARKCROW ) ), skill_get_time( getSkillId(), skill_lv ) );
}

void SkillFatalShadowCrow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 1300 * skill_lv + 10 * sstatus->pow;
	if (tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_DRAGON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillFatalShadowCrow::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	uint8 dir = DIR_NORTHEAST;

	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);	// dir based on target as we move player based on target location

	// Move the player 1 cell near the target, between the target and the player
	if (skill_check_unit_movepos(5, src, target->x + dirx[dir], target->y + diry[dir], 0, 1))
		clif_blown(src);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);// Trigger animation

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillFeintBomb::SkillFeintBomb() : WeaponSkillImpl(SC_FEINTBOMB) {
}

void SkillFeintBomb::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + (skill_lv + 1) * sstatus->dex / 2 * ((sd) ? sd->status.job_level / 10 : 1);
	RE_LVL_DMOD(120);
}

void SkillFeintBomb::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	std::shared_ptr<s_skill_unit_group> group = skill_unitsetting(src,getSkillId(),skill_lv,x,y,0); // Set bomb on current Position

	if( group == nullptr || group->unit == nullptr ) {
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	map_foreachinallrange(unit_changetarget, src, AREA_SIZE, BL_MOB, src, group->unit); // Release all targets against the caster
	skill_blown(src, src, skill_get_blewcount(getSkillId(), skill_lv), unit_getdir(src), BLOWN_IGNORE_NO_KNOCKBACK); // Don't stop the caster from backsliding if special_state.no_knockback is active
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv, false);
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillFindStone::SkillFindStone() : SkillImpl(TF_PICKSTONE) {
}

void SkillFindStone::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32 &flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd) {
		unsigned char eflag;
		item item_tmp;
		block_list tbl;
		clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
		memset(&item_tmp, 0, sizeof(item_tmp));
		memset(&tbl, 0, sizeof(tbl)); // [MouseJstr]
		item_tmp.nameid = ITEMID_STONE;
		item_tmp.identify = 1;
		tbl.id = 0;
		// Commented because of duplicate animation [Lemongrass]
		// At the moment this displays the pickup animation a second time
		// If this is required in older clients, we need to add a version check here
		// clif_takeitem(*sd,tbl);
		eflag = pc_additem(sd, &item_tmp, 1, LOG_TYPE_PRODUCE);
		if (eflag) {
			clif_additem(sd, 0, 0, eflag);
			if (battle_config.skill_drop_items_full)
				map_addflooritem(&item_tmp, 1, sd->m, sd->x, sd->y, 0, 0, 0, 4, 0);
		}
	}
}

SkillFrenzyShot::SkillFrenzyShot() : WeaponSkillImpl(ABC_FRENZY_SHOT) {
}

void SkillFrenzyShot::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	if (rnd_chance(5 * skill_lv, 100)) {
		dmg.div_ = 3;
	}
}

void SkillFrenzyShot::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillFrenzyShot::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 350 + 825 * skill_lv;
	skillratio += 15 * sstatus->con;

	RE_LVL_DMOD(100);
}

SkillFromTheAbyss::SkillFromTheAbyss() : SkillImpl(ABC_FROM_THE_ABYSS) {
}

void SkillFromTheAbyss::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start2(src, target, skill_get_sc(getSkillId()), 100, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv)));
}

SkillFromTheAbyssAttack::SkillFromTheAbyssAttack() : SkillImplRecursiveDamageSplash(ABC_FROM_THE_ABYSS_ATK) {
}

void SkillFromTheAbyssAttack::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 150 + 650 * skill_lv;
	skillratio += 5 * sstatus->spl;
	RE_LVL_DMOD(100);
}

SkillGrimtooth::SkillGrimtooth() : SkillImplRecursiveDamageSplash(AS_GRIMTOOTH) {
}

void SkillGrimtooth::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 20 * skill_lv;
}

void SkillGrimtooth::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag |= SD_PREAMBLE; // a fake packet will be sent for the first target to be hit

	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillGrimtooth::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data* tstatus = status_get_status_data(*target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if (dstmd && !status_has_mode(tstatus,MD_STATUSIMMUNE))
		sc_start(src,target,SC_QUAGMIRE,100,0,skill_get_time2(getSkillId(),skill_lv));
}

SkillHallucinationWalk::SkillHallucinationWalk() : StatusSkillImpl(GC_HALLUCINATIONWALK) {
}

void SkillHallucinationWalk::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	int32 heal = status_get_max_hp(target) / 10;
	if( status_get_hp(target) < heal ) { // if you haven't enough HP skill fails.
		if( sd ) clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_HP_INSUFFICIENT );
		return;
	}
	if( !status_charge(target,heal,0) )
	{
		if( sd ) clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_HP_INSUFFICIENT );
		return;
	}
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillHiding::SkillHiding() : SkillImpl(TF_HIDING) {
}

void SkillHiding::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32 &flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(bl);
	status_change_entry *tsce = tsc ? tsc->getSCE(SC_HIDING) : nullptr;

	if (tsce) {
		clif_skill_nodamage(src, *bl, getSkillId(), -1, status_change_end(bl, type)); // Hide skill-scream animation.
		return;
	}

	clif_skill_nodamage(src, *bl, getSkillId(), -1, sc_start(src, bl, SC_HIDING, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillHitAndSliding::SkillHitAndSliding() : WeaponSkillImpl(ABC_HIT_AND_SLIDING) {
}

void SkillHitAndSliding::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->status.weapon == W_BOW)
		dmg.flag |= BF_LONG;
}

void SkillHitAndSliding::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	uint8 dir = DIR_NORTHEAST;

	// Total backslide = skill level + distance between player and target
	int32 total_backslide = skill_lv + distance_bl(src, target);

	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);

	if (skill_check_unit_movepos(0, src, target->x + dirx[dir] * total_backslide, target->y + diry[dir] * total_backslide, 1, 1)) {
		clif_blown(src);
		unit_setdir(src, map_calc_dir(src, target->x, target->y)); // Set the player's direction to face the target
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	} else { //Is this the right behavior? [Haydrich]
		if (sd != nullptr)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
	}

	// Trigger skill animation
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, 1);
}

void SkillHitAndSliding::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 3500 * skill_lv;
	skillratio += 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillHitAndSliding::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillImpactCrater::SkillImpactCrater() : SkillImplRecursiveDamageSplash(SHC_IMPACT_CRATER) {
}

void SkillImpactCrater::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 200 * skill_lv;
	skillratio += 5 * sstatus->pow;

	RE_LVL_DMOD(100);
}

void SkillImpactCrater::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillInvisibility::SkillInvisibility() : SkillImpl(SC_INVISIBILITY) {
}

void SkillInvisibility::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 i = 0;

	if (tsce) {
		i = status_change_end(target, type);
		if( i )
			clif_skill_nodamage(src,*target,getSkillId(),-1,i);
		else if( sd )
			clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	i = sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
	if( i )
		clif_skill_nodamage(src,*target,getSkillId(),-1,i);
	else if( sd )
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_LEVEL );
}

SkillMaelstrom::SkillMaelstrom() : SkillImpl(SC_MAELSTROM) {
}

void SkillMaelstrom::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillManHole::SkillManHole() : SkillImpl(SC_MANHOLE) {
}

void SkillManHole::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillMasqueradeEnervation::SkillMasqueradeEnervation() : SkillImpl(SC_ENERVATION) {
}

void SkillMasqueradeEnervation::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( !(tsc && tsc->getSCE(type)) ) {
		status_data* sstatus = status_get_status_data(*src);
		status_data* tstatus = status_get_status_data(*target);
		map_session_data* dstsd = BL_CAST(BL_PC, target);

		int32 rate;

		if (status_get_class_(target) == CLASS_BOSS)
			return;
		rate = status_get_lv(src) / 10 + rnd_value(sstatus->dex / 12, sstatus->dex / 4) + ( sd ? sd->status.job_level : 50 ) + 10 * skill_lv
				   - (status_get_lv(target) / 10 + rnd_value(tstatus->agi / 6, tstatus->agi / 3) + tstatus->luk / 10 + ( dstsd ? (dstsd->max_weight / 10 - dstsd->weight / 10 ) / 100 : 0));
		rate = cap_value(rate, skill_lv + sstatus->dex / 20, 100);
		clif_skill_nodamage(src,*target,getSkillId(),0,sc_start(src,target,type,rate,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	} else if( sd )
		 clif_skill_fail( *sd, getSkillId() );
}

SkillMasqueradeGloomy::SkillMasqueradeGloomy() : SkillImpl(SC_GROOMY) {
}

void SkillMasqueradeGloomy::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( !(tsc && tsc->getSCE(type)) ) {
		status_data* sstatus = status_get_status_data(*src);
		status_data* tstatus = status_get_status_data(*target);
		map_session_data* dstsd = BL_CAST(BL_PC, target);

		int32 rate;

		if (status_get_class_(target) == CLASS_BOSS)
			return;
		rate = status_get_lv(src) / 10 + rnd_value(sstatus->dex / 12, sstatus->dex / 4) + ( sd ? sd->status.job_level : 50 ) + 10 * skill_lv
				   - (status_get_lv(target) / 10 + rnd_value(tstatus->agi / 6, tstatus->agi / 3) + tstatus->luk / 10 + ( dstsd ? (dstsd->max_weight / 10 - dstsd->weight / 10 ) / 100 : 0));
		rate = cap_value(rate, skill_lv + sstatus->dex / 20, 100);
		clif_skill_nodamage(src,*target,getSkillId(),0,sc_start(src,target,type,rate,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	} else if( sd )
		 clif_skill_fail( *sd, getSkillId() );
}

SkillMasqueradeIgnorance::SkillMasqueradeIgnorance() : SkillImpl(SC_IGNORANCE) {
}

void SkillMasqueradeIgnorance::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( !(tsc && tsc->getSCE(type)) ) {
		mob_data* dstmd = BL_CAST(BL_MOB, target);
		status_data* sstatus = status_get_status_data(*src);
		status_data* tstatus = status_get_status_data(*target);
		map_session_data* dstsd = BL_CAST(BL_PC, target);

		int32 rate;

		if (status_get_class_(target) == CLASS_BOSS)
			return;
		rate = status_get_lv(src) / 10 + rnd_value(sstatus->dex / 12, sstatus->dex / 4) + ( sd ? sd->status.job_level : 50 ) + 10 * skill_lv
				   - (status_get_lv(target) / 10 + rnd_value(tstatus->agi / 6, tstatus->agi / 3) + tstatus->luk / 10 + ( dstsd ? (dstsd->max_weight / 10 - dstsd->weight / 10 ) / 100 : 0));
		rate = cap_value(rate, skill_lv + sstatus->dex / 20, 100);
		if (clif_skill_nodamage(src,*target,getSkillId(),0,sc_start(src,target,type,rate,skill_lv,skill_get_time(getSkillId(),skill_lv)))) {
			int32 sp = 100 * skill_lv;

			if( dstmd )
				sp = dstmd->level;
			if( !dstmd )
				status_zap(target, 0, sp);

			status_heal(src, 0, sp / 2, 3);
		} else if( sd )
			clif_skill_fail( *sd, getSkillId() );
	} else if( sd )
		clif_skill_fail( *sd, getSkillId() );
}

SkillMasqueradeLaziness::SkillMasqueradeLaziness() : SkillImpl(SC_LAZINESS) {
}

void SkillMasqueradeLaziness::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( !(tsc && tsc->getSCE(type)) ) {
		status_data* sstatus = status_get_status_data(*src);
		status_data* tstatus = status_get_status_data(*target);
		map_session_data* dstsd = BL_CAST(BL_PC, target);

		int32 rate;

		if (status_get_class_(target) == CLASS_BOSS)
			return;
		rate = status_get_lv(src) / 10 + rnd_value(sstatus->dex / 12, sstatus->dex / 4) + ( sd ? sd->status.job_level : 50 ) + 10 * skill_lv
				   - (status_get_lv(target) / 10 + rnd_value(tstatus->agi / 6, tstatus->agi / 3) + tstatus->luk / 10 + ( dstsd ? (dstsd->max_weight / 10 - dstsd->weight / 10 ) / 100 : 0));
		rate = cap_value(rate, skill_lv + sstatus->dex / 20, 100);
		clif_skill_nodamage(src,*target,getSkillId(),0,sc_start(src,target,type,rate,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	} else if( sd )
		 clif_skill_fail( *sd, getSkillId() );
}

SkillMasqueradeUnlucky::SkillMasqueradeUnlucky() : SkillImpl(SC_UNLUCKY) {
}

void SkillMasqueradeUnlucky::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( !(tsc && tsc->getSCE(type)) ) {
		status_data* sstatus = status_get_status_data(*src);
		status_data* tstatus = status_get_status_data(*target);
		map_session_data* dstsd = BL_CAST(BL_PC, target);

		int32 rate;

		if (status_get_class_(target) == CLASS_BOSS)
			return;
		rate = status_get_lv(src) / 10 + rnd_value(sstatus->dex / 12, sstatus->dex / 4) + ( sd ? sd->status.job_level : 50 ) + 10 * skill_lv
				   - (status_get_lv(target) / 10 + rnd_value(tstatus->agi / 6, tstatus->agi / 3) + tstatus->luk / 10 + ( dstsd ? (dstsd->max_weight / 10 - dstsd->weight / 10 ) / 100 : 0));
		rate = cap_value(rate, skill_lv + sstatus->dex / 20, 100);
		clif_skill_nodamage(src,*target,getSkillId(),0,sc_start(src,target,type,rate,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	} else if( sd )
		 clif_skill_fail( *sd, getSkillId() );
}

SkillMasqueradeWeakness::SkillMasqueradeWeakness() : SkillImpl(SC_WEAKNESS) {
}

void SkillMasqueradeWeakness::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( !(tsc && tsc->getSCE(type)) ) {
		status_data* sstatus = status_get_status_data(*src);
		status_data* tstatus = status_get_status_data(*target);
		map_session_data* dstsd = BL_CAST(BL_PC, target);

		int32 rate;

		if (status_get_class_(target) == CLASS_BOSS)
			return;
		rate = status_get_lv(src) / 10 + rnd_value(sstatus->dex / 12, sstatus->dex / 4) + ( sd ? sd->status.job_level : 50 ) + 10 * skill_lv
				   - (status_get_lv(target) / 10 + rnd_value(tstatus->agi / 6, tstatus->agi / 3) + tstatus->luk / 10 + ( dstsd ? (dstsd->max_weight / 10 - dstsd->weight / 10 ) / 100 : 0));
		rate = cap_value(rate, skill_lv + sstatus->dex / 20, 100);
		clif_skill_nodamage(src,*target,getSkillId(),0,sc_start(src,target,type,rate,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	} else if( sd )
		 clif_skill_fail( *sd, getSkillId() );
}

SkillMeteorAssault::SkillMeteorAssault() : SkillImplRecursiveDamageSplash(ASC_METEORASSAULT) {
}

void SkillMeteorAssault::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	skillratio += 100 + 120 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += -60 + 40 * skill_lv;
#endif
}

void SkillMeteorAssault::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}
	
void SkillMeteorAssault::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	//Any enemies hit by this skill will receive Stun, Darkness, or external bleeding status ailment with a 5%+5*skill_lv% chance.
	switch(rnd()%3) {
		case 0:
			sc_start(src,target,SC_BLIND,(5+skill_lv*5),skill_lv,skill_get_time2(getSkillId(),1));
			break;
		case 1:
			sc_start(src,target,SC_STUN,(5+skill_lv*5),skill_lv,skill_get_time2(getSkillId(),2));
			break;
		default:
			sc_start2(src,target,SC_BLEEDING,(5+skill_lv*5),skill_lv,src->id,skill_get_time2(getSkillId(),3));
	}
}

SkillMug::SkillMug() : SkillImpl(RG_STEALCOIN) {
}

void SkillMug::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	mob_data *dstmd = BL_CAST(BL_MOB, target);

	if (sd == nullptr || dstmd == nullptr)
		return;

	int32 target_lv = status_get_lv(target);
	int32 rate = 10 * pc_checkskill(sd, RG_STEALCOIN);
	rate += sd->battle_status.dex / 2;
	rate += sd->battle_status.luk / 2;
	rate += 2 * (sd->status.base_level - target_lv);

	if (!rnd_chance_official(rate, 1000))
	{
		clif_skill_fail(*sd, getSkillId());
		return;
	}

	dstmd->state.steal_coin_flag = 1;

	// Zeny Steal Amount
	int32 amount = rnd_value(8 * target_lv, 10 * target_lv);
	amount += (skill_lv * target_lv) / 10;

	pc_getzeny(sd, amount, LOG_TYPE_STEAL);

	// This triggers a 0 damage event and might make the monster switch target to caster
	battle_damage(src, target, 0, 1, skill_lv, 0, ATK_DEF, BF_WEAPON|BF_LONG|BF_NORMAL, true, tick, false);

	// Client uses skill_lv to show how many Zeny were stolen
	clif_skill_nodamage(src, *target, getSkillId(), amount);		
}

SkillOmegaAbyssStrike::SkillOmegaAbyssStrike() : SkillImpl(ABC_ABYSS_STRIKE) {
}

void SkillOmegaAbyssStrike::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

void SkillOmegaAbyssStrike::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 2650 * skill_lv;
	skillratio += 10 * sstatus->spl;
	if (tstatus->race == RC_DEMON || tstatus->race == RC_ANGEL)
		skillratio += 200 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillPhantomMenace::SkillPhantomMenace() : WeaponSkillImpl(GC_PHANTOMMENACE) {
}

void SkillPhantomMenace::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 200;
}

void SkillPhantomMenace::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if (flag&1) { // Only Hits Invisitargete Targets
		if(tsc && (tsc->option&(OPTION_HIDE|OPTION_CLOAK|OPTION_CHASEWALK) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC_STEALTHFIELD))) {
			status_change_end(target, SC_CLOAKINGEXCEED);
			WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
		}
		if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
			status_change_end(target, SC__SHADOWFORM); // Should only end, no damage dealt.
	}
}

void SkillPhantomMenace::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_damage( *src, *target,tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	map_foreachinrange(skill_area_sub,src,skill_get_splash(getSkillId(),skill_lv),BL_CHAR,
		src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
}

SkillPoisoningWeapon::SkillPoisoningWeapon() : SkillImpl(GC_POISONINGWEAPON) {
}

void SkillPoisoningWeapon::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd ) {
		clif_poison_list( *sd, skill_lv );
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	}
}

SkillPoisonSmoke::SkillPoisonSmoke() : SkillImpl(GC_POISONSMOKE) {
}

void SkillPoisonSmoke::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( !(sc && sc->getSCE(SC_POISONINGWEAPON)) ) {
		if( sd )
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_GC_POISONINGWEAPON );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, flag);
}

SkillRemover::SkillRemover() : SkillImpl(RG_CLEANER) {
}

void SkillRemover::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

void SkillRemover::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_graffitiremover,src->m,x-i,y-i,x+i,y+i,BL_SKILL,1);
}

SkillReproduce::SkillReproduce() : SkillImpl(SC_REPRODUCE) {
}

void SkillReproduce::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	status_change_entry *tsce = (tsc != nullptr && type != SC_NONE) ? tsc->getSCE(type) : nullptr;
	map_session_data* sd = BL_CAST(BL_PC, src);
	int32 i = 0;

	if (tsce) {
		i = status_change_end(target, type);
		if( i )
			clif_skill_nodamage(src,*target,getSkillId(),-1,i);
		else if( sd )
			clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	i = sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
	if( i )
		clif_skill_nodamage(src,*target,getSkillId(),-1,i);
	else if( sd )
		clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL_LEVEL );
}

SkillRollingCutter::SkillRollingCutter() : SkillImplRecursiveDamageSplash(GC_ROLLINGCUTTER) {
}

void SkillRollingCutter::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 50 + 80 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillRollingCutter::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	int16 count = 1;
	skill_area_temp[2] = 0;
	map_foreachinrange(skill_area_sub,src,skill_get_splash(getSkillId(),skill_lv),BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|SD_PREAMBLE|SD_SPLASH|1,skill_castend_damage_id);
	if( tsc && tsc->getSCE(SC_ROLLINGCUTTER) )
	{ // Every time the skill is casted the status change is reseted adding a counter.
		count += (int16)tsc->getSCE(SC_ROLLINGCUTTER)->val1;
		if( count > 10 )
			count = 10; // Max coounter
		status_change_end(target, SC_ROLLINGCUTTER);
	}
	sc_start(src,target,SC_ROLLINGCUTTER,100,count,skill_get_time(getSkillId(),skill_lv));
	clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
}

SkillSandAttack::SkillSandAttack() : WeaponSkillImpl(TF_SPRINKLESAND) {
}

void SkillSandAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 30;
}

void SkillSandAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	sc_start(src, target, SC_BLIND, (sd != nullptr) ? 20 : 15, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

SkillSavageImpact::SkillSavageImpact() : SkillImplRecursiveDamageSplash(SHC_SAVAGE_IMPACT) {
}

void SkillSavageImpact::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.div_ = dmg.div_ + dmg.miscflag;
}

void SkillSavageImpact::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);

	skillratio += -100 + 130 * skill_lv;
	skillratio += 5 * sstatus->pow;

	if( sc != nullptr && sc->hasSCE( SC_SHADOW_EXCEED ) ){
		skillratio += 30 * skill_lv;
		skillratio += 2 * sstatus->pow;
	}

	RE_LVL_DMOD(100);
}

void SkillSavageImpact::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	if( status_change *sc = status_get_sc(src); sc != nullptr && sc->hasSCE( SC_CLOAKINGEXCEED ) ){
		skill_area_temp[0] = 2;
		status_change_end( src, SC_CLOAKINGEXCEED );
	}

	uint8 dir = DIR_NORTHEAST;	// up-right when src is on the same cell of target

	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);	// dir based on target as we move player based on target location

	// Move the player 1 cell near the target, between the target and the player
	if (skill_check_unit_movepos(5, src, target->x + dirx[dir], target->y + diry[dir], 0, 1))
		clif_blown(src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillScribble::SkillScribble() : SkillImpl(RG_GRAFFITI) {
}

void SkillScribble::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
	flag|=1;
}

SkillShadowForm::SkillShadowForm() : SkillImpl(SC_SHADOWFORM) {
}

void SkillShadowForm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if( sd && dstsd && src != target && !dstsd->shadowform_id ) {
		if( clif_skill_nodamage(src,*target,getSkillId(),skill_lv,sc_start4(src,src,type,100,skill_lv,target->id,4+skill_lv,0,skill_get_time(getSkillId(), skill_lv))) )
			dstsd->shadowform_id = src->id;
	}
	else if( sd )
		clif_skill_fail( *sd, getSkillId() );
}

SkillShadowStab::SkillShadowStab() : WeaponSkillImpl(SHC_SHADOW_STAB) {
}

void SkillShadowStab::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 650 * skill_lv;
	skillratio += 5 * sstatus->pow;	// TODO : check pow ratio

	RE_LVL_DMOD(100);
}

void SkillShadowStab::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(src, SC_CLOAKING);
	status_change_end(src, SC_CLOAKINGEXCEED);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

SkillSightlessMind::SkillSightlessMind() : SkillImplRecursiveDamageSplash(RG_RAID) {
}

void SkillSightlessMind::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += -100 + 50 + skill_lv * 150;
#else
	base_skillratio += 40 * skill_lv;
#endif
}

void SkillSightlessMind::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = 0;
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	map_foreachinrange(skill_area_sub, target,
		skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL,
		src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|1,
		skill_castend_damage_id);
	status_change_end(src, SC_HIDING);
}

void SkillSightlessMind::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_STUN,(10+3*skill_lv),skill_lv,skill_get_time(getSkillId(),skill_lv));
	sc_start(src,target,SC_BLIND,(10+3*skill_lv),skill_lv,skill_get_time2(getSkillId(),skill_lv));
#ifdef RENEWAL
	sc_start(src, target, SC_RAID, 100, skill_lv, 10000); // Hardcoded to 10 seconds since Duration1 and Duration2 are used
#endif
}

SkillSnatch::SkillSnatch() : WeaponSkillImpl(RG_INTIMIDATE) {
}

void SkillSnatch::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 30 * skill_lv;
}

SkillSonicBlow::SkillSonicBlow() : WeaponSkillImpl(AS_SONICBLOW) {
}

void SkillSonicBlow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_data* tstatus = status_get_status_data(*target);

	base_skillratio += 100 + 100 * skill_lv;
	if (tstatus->hp < (tstatus->max_hp / 2))
		base_skillratio += base_skillratio / 2;
#else
	const map_session_data* sd = BL_CAST( BL_PC, src );

	base_skillratio += 200 + 50 * skill_lv;
	if (sd && pc_checkskill(sd, AS_SONICACCEL) > 0)
		base_skillratio += base_skillratio / 10;
#endif
}

void SkillSonicBlow::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change *sc = status_get_sc(src);

	if (!map_flag_gvg2(target->m) && !map_getmapflag(target->m, MF_BATTLEGROUND) && sc && sc->getSCE(SC_SPIRIT) && sc->getSCE(SC_SPIRIT)->val2 == SL_ASSASIN)
		sc_start(src, target, SC_STUN, (4 * skill_lv + 20), skill_lv, skill_get_time2(getSkillId(), skill_lv)); //Link gives double stun chance outside GVG/BG
	else
		sc_start(src, target, SC_STUN, (2 * skill_lv + 10), skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillSonicBlow::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST( BL_PC, src );

	if(sd && pc_checkskill(sd,AS_SONICACCEL) > 0)
#ifdef RENEWAL
		hit_rate += hit_rate * 90 / 100;
#else
		hit_rate += hit_rate * 50 / 100;
#endif
}

SkillSoulDestroyer::SkillSoulDestroyer() : WeaponSkillImpl(ASC_BREAKER) {
}

void SkillSoulDestroyer::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 150 * skill_lv + sstatus->str + sstatus->int_; // !TODO: Confirm stat modifier
	RE_LVL_DMOD(100);
#else
	// Pre-Renewal: skill ratio for weapon part of damage [helvetica]
	skillratio += -100 + 100 * skill_lv;
#endif
}

SkillSteal::SkillSteal() : SkillImpl(TF_STEAL) {
}

void SkillSteal::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32 &flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd) {
		if (pc_steal_item(sd, bl, skill_lv))
			clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
		else
			clif_skill_fail(*sd, getSkillId(), USESKILL_FAIL);
	}
}

SkillStealth::SkillStealth() : SkillImpl(ST_CHASEWALK) {
}

void SkillStealth::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change *tsc = status_get_sc(target);
	status_change_entry *tsce = (tsc && type != SC_NONE)?tsc->getSCE(type):nullptr;

	if (tsce)
	{
		clif_skill_nodamage(src,*target,getSkillId(),-1,status_change_end(target, type)); //Hide skill-scream animation.
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	clif_skill_nodamage(src,*target,getSkillId(),-1,sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
}

SkillStoneFling::SkillStoneFling() : SkillImpl(TF_THROWSTONE) {
}

void SkillStoneFling::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	if (sd != nullptr) {
		// Only blind if used by player and stun failed
		if (!sc_start(src, target, SC_STUN, 3, skill_lv, skill_get_time(getSkillId(), skill_lv)))
			sc_start(src, target, SC_BLIND, 3, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	} else {
		// 5% stun chance and no blind chance when used by monsters
		sc_start(src, target, SC_STUN, 5, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
}

void SkillStoneFling::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillStripAccessory::SkillStripAccessory() : SkillImpl(SC_STRIPACCESSARY) {
}

void SkillStripAccessory::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	bool strip_success = skill_strip_equip(src, target, getSkillId(), skill_lv);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,strip_success);

	//Nothing stripped.
	if( sd && !strip_success )
		clif_skill_fail( *sd, getSkillId() );
}

SkillStripShadow::SkillStripShadow() : SkillImpl(ABC_STRIP_SHADOW) {
}

void SkillStripShadow::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	bool strip_success = skill_strip_equip(src, target, getSkillId(), skill_lv);

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,strip_success);

	//Nothing stripped.
	if( sd && !strip_success )
		clif_skill_fail( *sd, getSkillId() );
}

SkillThrowVenomKnife::SkillThrowVenomKnife() : WeaponSkillImpl(AS_VENOMKNIFE) {
}

void SkillThrowVenomKnife::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 400;
#endif
}

void SkillThrowVenomKnife::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start2(src, target, SC_POISON, 100, skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv));
}

SkillTriangleShot::SkillTriangleShot() : WeaponSkillImpl(SC_TRIANGLESHOT) {
}

void SkillTriangleShot::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 230 * skill_lv + 3 * sstatus->agi;
	RE_LVL_DMOD(100);
}

SkillUnluckyRush::SkillUnluckyRush() : WeaponSkillImpl(ABC_UNLUCKY_RUSH) {
}

void SkillUnluckyRush::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Jump to the target before attacking.
	if (skill_check_unit_movepos(5, src, target->x, target->y, 0, 1))
		skill_blown(src, src, 1, (map_calc_dir(target, src->x, src->y) + 4) % 8, BLOWN_NONE);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillUnluckyRush::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 100 + 300 * skill_lv + 5 * sstatus->pow;
	if (sc != nullptr && sc->hasSCE(SC_CHASING))
		skillratio += 2500 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillUnluckyRush::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_HANDICAPSTATE_MISFORTUNE, 30 + 10 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillVenomDust::SkillVenomDust() : SkillImpl(AS_VENOMDUST) {
}

void SkillVenomDust::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillVenomPressure::SkillVenomPressure() : WeaponSkillImpl(GC_VENOMPRESSURE) {
}

void SkillVenomPressure::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 900;
}

void SkillVenomPressure::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += 10 + 4 * skill_lv;
}

SkillVenomSplasher::SkillVenomSplasher() : SkillImplRecursiveDamageSplash(AS_SPLASHER) {
}

void SkillVenomSplasher::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST( BL_PC, src );

#ifdef RENEWAL
	base_skillratio += -100 + 400 + 100 * skill_lv;
#else
	base_skillratio += 400 + 50 * skill_lv;
#endif
	if(sd)
		base_skillratio += 20 * pc_checkskill(sd,AS_POISONREACT);
}

void SkillVenomSplasher::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( status_has_mode(tstatus,MD_STATUSIMMUNE)
	// Renewal dropped the 3/4 hp requirement
#ifndef RENEWAL
		|| tstatus-> hp > tstatus->max_hp*3/4
#endif
			) {
		if (sd) {
			clif_skill_fail( *sd, getSkillId() );
		}
		return;
	}
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,
		sc_start4(src,target,type,100,skill_lv,getSkillId(),src->id,skill_get_time(getSkillId(),skill_lv),1000));
}

void SkillVenomSplasher::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);

	if (!(flag & 1)) {
		// Don't consume a second gemstone.
		flag |= SKILL_NOCONSUME_REQ;
	}
}

int16 SkillVenomSplasher::getSearchSize(block_list* src, uint16 skill_lv) const {
	// Venom Splasher uses a different range for searching than for splashing
	return 1;
}

void SkillVenomSplasher::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start2(src, target, SC_POISON, 100, skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv));
}

SkillWeaponCrush::SkillWeaponCrush() : WeaponSkillImpl(GC_WEAPONCRUSH) {
}

void SkillWeaponCrush::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );
	std::shared_ptr<s_skill_unit_group> sg;

	bool i;

	if( (i = skill_strip_equip(src, target, getSkillId(), skill_lv)) )
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i);

	//Nothing stripped.
	if( sd && !i )
		clif_skill_fail( *sd, getSkillId() );
}

void SkillWeaponCrush::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	skill_castend_nodamage_id(src,target,getSkillId(),skill_lv,tick,BCT_ENEMY);
}

std::unique_ptr<const SkillImpl> SkillFactoryThief::create(const e_skill skill_id) const {
	switch (skill_id) {
		case ABC_ABYSS_DAGGER:
			return std::make_unique<SkillAbyssDagger>();
		case ABC_ABYSS_FLAME:
			return std::make_unique<SkillAbyssFlame>();
		case ABC_ABYSS_FLAME_ATK:
			return std::make_unique<SkillAbyssFlameAttack>();
		case ABC_ABYSS_SLAYER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case ABC_ABYSS_SQUARE:
			return std::make_unique<SkillAbyssSquare>();
		case ABC_ABYSS_STRIKE:
			return std::make_unique<SkillOmegaAbyssStrike>();
		case ABC_CHAIN_REACTION_SHOT:
			return std::make_unique<SkillChainReactionShot>();
		case ABC_CHAIN_REACTION_SHOT_ATK:
			return std::make_unique<SkillChainReactionShotAttack>();
		case ABC_CHASING_BREAK:
			return std::make_unique<SkillChasingBreak>();
		case ABC_CHASING_SHOT:
			return std::make_unique<SkillChasingShot>();
		case ABC_DEFT_STAB:
			return std::make_unique<SkillDeftStab>();
		case ABC_FRENZY_SHOT:
			return std::make_unique<SkillFrenzyShot>();
		case ABC_FROM_THE_ABYSS:
			return std::make_unique<SkillFromTheAbyss>();
		case ABC_FROM_THE_ABYSS_ATK:
			return std::make_unique<SkillFromTheAbyssAttack>();
		case ABC_HIT_AND_SLIDING:
			return std::make_unique<SkillHitAndSliding>();
		case ABC_STRIP_SHADOW:
			return std::make_unique<SkillStripShadow>();
		case ABC_UNLUCKY_RUSH:
			return std::make_unique<SkillUnluckyRush>();
		case ASC_BREAKER:
			return std::make_unique<SkillSoulDestroyer>();
		case ASC_CDP:
			return std::make_unique<SkillCreateDeadlyPoison>();
		case ASC_EDP:
			return std::make_unique<SkillEnchantDeadlyPoison>();
		case ASC_METEORASSAULT:
			return std::make_unique<SkillMeteorAssault>();
		case AS_CLOAKING:
			return std::make_unique<SkillCloaking>();
		case AS_ENCHANTPOISON:
			return std::make_unique<SkillEnchantPoison>();
		case AS_GRIMTOOTH:
			return std::make_unique<SkillGrimtooth>();
		case AS_POISONREACT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case AS_SONICBLOW:
			return std::make_unique<SkillSonicBlow>();
		case AS_SPLASHER:
			return std::make_unique<SkillVenomSplasher>();
		case AS_VENOMDUST:
			return std::make_unique<SkillVenomDust>();
		case AS_VENOMKNIFE:
			return std::make_unique<SkillThrowVenomKnife>();
		case GC_ANTIDOTE:
			return std::make_unique<SkillAntidote>();
		case GC_CLOAKINGEXCEED:
			return std::make_unique<SkillCloakingExceed>();
		case GC_COUNTERSLASH:
			return std::make_unique<SkillCounterSlash>();
		case GC_CREATENEWPOISON:
			return std::make_unique<SkillCreateNewPoison>();
		case GC_CROSSIMPACT:
			return std::make_unique<SkillCrossImpact>();
		case GC_CROSSRIPPERSLASHER:
			return std::make_unique<SkillCrossRipperSlasher>();
		case GC_DARKCROW:
			return std::make_unique<SkillDarkClaw>();
		case GC_DARKILLUSION:
			return std::make_unique<SkillDarkIllusion>();
		case GC_HALLUCINATIONWALK:
			return std::make_unique<SkillHallucinationWalk>();
		case GC_PHANTOMMENACE:
			return std::make_unique<SkillPhantomMenace>();
		case GC_POISONINGWEAPON:
			return std::make_unique<SkillPoisoningWeapon>();
		case GC_POISONSMOKE:
			return std::make_unique<SkillPoisonSmoke>();
		case GC_ROLLINGCUTTER:
			return std::make_unique<SkillRollingCutter>();
		case GC_VENOMIMPRESS:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case GC_VENOMPRESSURE:
			return std::make_unique<SkillVenomPressure>();
		case GC_WEAPONBLOCKING:
			return std::make_unique<StatusSkillImpl>(skill_id, true);
		case GC_WEAPONCRUSH:
			return std::make_unique<SkillWeaponCrush>();
		case RG_BACKSTAP:
			return std::make_unique<SkillBackStab>();
		case RG_CLEANER:
			return std::make_unique<SkillRemover>();
		case RG_CLOSECONFINE:
			return std::make_unique<SkillCloseConfine>();
		case RG_GRAFFITI:
			return std::make_unique<SkillScribble>();
		case RG_INTIMIDATE:
			return std::make_unique<SkillSnatch>();
		case RG_RAID:
			return std::make_unique<SkillSightlessMind>();
		case RG_STEALCOIN:
			return std::make_unique<SkillMug>();
		case RG_STRIPARMOR:
			return std::make_unique<SkillDivestArmor>();
		case RG_STRIPHELM:
			return std::make_unique<SkillDivestHelm>();
		case RG_STRIPSHIELD:
			return std::make_unique<SkillDivestShield>();
		case RG_STRIPWEAPON:
			return std::make_unique<SkillDivestWeapon>();
		case SC_AUTOSHADOWSPELL:
			return std::make_unique<SkillAutoShadowSpell>();
		case SC_BLOODYLUST:
			return std::make_unique<SkillBloodyLust>();
		case SC_BODYPAINT:
			return std::make_unique<SkillBodyPainting>();
		case SC_CHAOSPANIC:
			return std::make_unique<SkillChaosPanic>();
		case SC_DEADLYINFECT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SC_DIMENSIONDOOR:
			return std::make_unique<SkillDimensionDoor>();
		case SC_ENERVATION:
			return std::make_unique<SkillMasqueradeEnervation>();
		case SC_ESCAPE:
			return std::make_unique<SkillEmergencyEscape>();
		case SC_FATALMENACE:
			return std::make_unique<SkillFatalMenace>();
		case SC_FEINTBOMB:
			return std::make_unique<SkillFeintBomb>();
		case SC_GROOMY:
			return std::make_unique<SkillMasqueradeGloomy>();
		case SC_IGNORANCE:
			return std::make_unique<SkillMasqueradeIgnorance>();
		case SC_INVISIBILITY:
			return std::make_unique<SkillInvisibility>();
		case SC_LAZINESS:
			return std::make_unique<SkillMasqueradeLaziness>();
		case SC_MAELSTROM:
			return std::make_unique<SkillMaelstrom>();
		case SC_MANHOLE:
			return std::make_unique<SkillManHole>();
		case SC_REPRODUCE:
			return std::make_unique<SkillReproduce>();
		case SC_SHADOWFORM:
			return std::make_unique<SkillShadowForm>();
		case SC_STRIPACCESSARY:
			return std::make_unique<SkillStripAccessory>();
		case SC_TRIANGLESHOT:
			return std::make_unique<SkillTriangleShot>();
		case SC_UNLUCKY:
			return std::make_unique<SkillMasqueradeUnlucky>();
		case SC_WEAKNESS:
			return std::make_unique<SkillMasqueradeWeakness>();
		case SHC_CROSS_SLASH:
			return std::make_unique<SkillCrossSlash>();
		case SHC_DANCING_KNIFE:
			return std::make_unique<SkillDancingKnife>();
		case SHC_ENCHANTING_SHADOW:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SHC_ETERNAL_SLASH:
			return std::make_unique<SkillEternalSlash>();
		case SHC_FATAL_SHADOW_CROW:
			return std::make_unique<SkillFatalShadowCrow>();
		case SHC_IMPACT_CRATER:
			return std::make_unique<SkillImpactCrater>();
		case SHC_POTENT_VENOM:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SHC_SAVAGE_IMPACT:
			return std::make_unique<SkillSavageImpact>();
		case SHC_SHADOW_EXCEED:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SHC_SHADOW_STAB:
			return std::make_unique<SkillShadowStab>();
		case ST_CHASEWALK:
			return std::make_unique<SkillStealth>();
		case ST_FULLSTRIP:
			return std::make_unique<SkillDivestAll>();
		case ST_PRESERVE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case ST_REJECTSWORD:
			return std::make_unique<SkillCounterInstinct>();
		case TF_BACKSLIDING:
			return std::make_unique<SkillBackSlide>();
		case TF_DETOXIFY:
			return std::make_unique<SkillDetoxify>();
		case TF_DOUBLE:
			return std::make_unique<SkillDoubleAttack>();
		case TF_HIDING:
			return std::make_unique<SkillHiding>();
		case TF_PICKSTONE:
			return std::make_unique<SkillFindStone>();
		case TF_POISON:
			return std::make_unique<SkillEnvenom>();
		case TF_SPRINKLESAND:
			return std::make_unique<SkillSandAttack>();
		case TF_STEAL:
			return std::make_unique<SkillSteal>();
		case TF_THROWSTONE:
			return std::make_unique<SkillStoneFling>();

		default:
			return nullptr;
	}
}

#endif
