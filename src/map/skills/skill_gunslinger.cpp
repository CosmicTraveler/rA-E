// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_gunslinger.hpp"

#include "map/status.hpp"
#include <common/random.hpp>
#include <config/core.hpp>
#include "map/battle.hpp"
#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/unit.hpp"
#include "map/map.hpp"
#include "map/path.hpp"
#include <common/nullpo.hpp>
#include "skill_impl.hpp"

SkillAntiMaterialBlast::SkillAntiMaterialBlast() : WeaponSkillImpl(RL_AM_BLAST) {
}

void SkillAntiMaterialBlast::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_ANTI_M_BLAST, 20 + 10 * skill_lv, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillAntiMaterialBlast::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += -100 + 3500 + 300 * skill_lv;
}

SkillBanishingBuster::SkillBanishingBuster() : WeaponSkillImpl(RL_BANISHING_BUSTER) {
}

void SkillBanishingBuster::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += -100 + 1000 + 200 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillBanishingBuster::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change* tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (tsc == nullptr || tsc->empty()) {
		return;
	}

	if (status_isimmune(target)) {
		return;
	}

	if ((dstsd && (dstsd->class_ & MAPID_SECONDMASK) == MAPID_SOUL_LINKER) || rnd() % 100 >= 50 + 5 * skill_lv) {
		if (sd) {
			clif_skill_fail(*sd, getSkillId());
		}
		return;
	}

	uint16 n = skill_lv;

	for (const auto& it : status_db) {
		sc_type status = static_cast<sc_type>(it.first);
		status_change_entry* sce = tsc->getSCE(status);

		if (n <= 0) {
			break;
		}
		if (sce == nullptr) {
			continue;
		}
		if (it.second->flag[SCF_NOBANISHINGBUSTER]) {
			continue;
		}

		switch (status) {
			case SC_WHISTLE: case SC_ASSNCROS: case SC_POEMBRAGI:
			case SC_APPLEIDUN: case SC_HUMMING: case SC_DONTFORGETME:
			case SC_FORTUNE: case SC_SERVICE4U:
				if (!battle_config.dispel_song || sce->val4 == 0) {
					//If in song area don't end it, even if config enabled
					continue;
				}
				break;
			case SC_ASSUMPTIO:
				if (target->type == BL_MOB) {
					continue;
				}
				break;
		}

		if (status == SC_BERSERK || status == SC_SATURDAYNIGHTFEVER) {
			sce->val2 = 0;
		}
		status_change_end(target, status);
		n--;
	}

	if (dstsd) {
		//Remove bonus_script by Banishing Buster
		pc_bonus_script_clear(dstsd, BSF_REM_ON_BANISHING_BUSTER);
	}
}

SkillBasicGrenade::SkillBasicGrenade() : WeaponSkillImpl(NW_BASIC_GRENADE) {
}

void SkillBasicGrenade::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub, src->m, x - i, y - i, x + i, y + i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
}

void SkillBasicGrenade::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1500 + 2100 * skill_lv;
	skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 50;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

void SkillBasicGrenade::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	// Night Watch Grenade Fragment elementals
	if( sc != nullptr ){
		if( sc->hasSCE( SC_GRENADE_FRAGMENT_1 ) ){
			element = ELE_WATER;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_2 ) ){
			element = ELE_WIND;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_3 ) ){
			element = ELE_EARTH;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_4 ) ){
			element = ELE_FIRE;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_5 ) ){
			element = ELE_DARK;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_6 ) ){
			element = ELE_HOLY;
		}
	}
}

SkillBindTrap::SkillBindTrap() : SkillImpl(RL_B_TRAP) {
}

void SkillBindTrap::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillBindTrap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillBullseye::SkillBullseye() : WeaponSkillImpl(GS_BULLSEYE) {
}

void SkillBullseye::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_data *tstatus = status_get_status_data(*target);

	// Only works well against brute/demihumans non bosses.
	if ((tstatus->race == RC_BRUTE || tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_PLAYER_HUMAN || tstatus->race == RC_PLAYER_DORAM) && !status_has_mode(
		    tstatus, MD_STATUSIMMUNE))
		base_skillratio += 400;
}

void SkillBullseye::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data *tstatus = status_get_status_data(*target);

	// 0.1% coma rate.
	if (tstatus->race == RC_BRUTE || tstatus->race == RC_DEMIHUMAN || tstatus->race == RC_PLAYER_HUMAN || tstatus->race == RC_PLAYER_DORAM)
		status_change_start(src, target, SC_COMA, 10, skill_lv, 0, src->id, 0, 0, SCSTART_NONE);
}

SkillChainAction::SkillChainAction() : WeaponSkillImpl(GS_CHAINACTION) {
}

void SkillChainAction::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	// For NPC used skill.
	dmg.type = DMG_MULTI_HIT;
}

SkillCracker::SkillCracker() : SkillImpl(GS_CRACKER) {
}

void SkillCracker::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	map_session_data *dstsd = BL_CAST(BL_PC, target);
	mob_data *dstmd = BL_CAST(BL_MOB, target);

	/* per official standards, this skill works on players and mobs. */
	if (sd && (dstsd || dstmd)) {
		int32 i = 65 - 5 * distance_bl(src, target); // Base rate
		if (i < 30)
			i = 30;
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		sc_start(src, target, SC_STUN, i, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	}
}

SkillCrimsonMarker::SkillCrimsonMarker() : SkillImpl(RL_C_MARKER) {
}

void SkillCrimsonMarker::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	sc_type type = skill_get_sc(getSkillId());
	status_change* tsc = status_get_sc(target);
	status_change_entry* tsce = (tsc && type != SC_NONE) ? tsc->getSCE(type) : nullptr;
	int32 i;

	if (sd) {
		// If marked by someone else remove it
		if (tsce && tsce->val2 != src->id) {
			status_change_end(target, type);
		}

		// Check if marked before
		ARR_FIND(0, MAX_SKILL_CRIMSON_MARKER, i, sd->c_marker[i] == target->id);
		if (i == MAX_SKILL_CRIMSON_MARKER) {
			// Find empty slot
			ARR_FIND(0, MAX_SKILL_CRIMSON_MARKER, i, !sd->c_marker[i]);
			if (i == MAX_SKILL_CRIMSON_MARKER) {
				clif_skill_fail(*sd, getSkillId());
				return;
			}
		}

		sd->c_marker[i] = target->id;
		status_change_start(src, target, type, 10000, skill_lv, src->id, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NOAVOID | SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	} else {
		// If mob casts this, at least SC_C_MARKER as debuff
		status_change_start(src, target, type, 10000, skill_lv, src->id, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NOAVOID | SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillDesperado::SkillDesperado() : SkillImpl(GS_DESPERADO) {
}

void SkillDesperado::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	base_skillratio += 50 * (skill_lv - 1);
	if (sc && sc->getSCE(SC_FALLEN_ANGEL))
		base_skillratio *= 2;
}

void SkillDesperado::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

SkillDisarm::SkillDisarm() : WeaponSkillImpl(GS_DISARM) {
}

void SkillDisarm::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	skill_strip_equip(src, target, getSkillId(), skill_lv);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillDragonTail::SkillDragonTail() : SkillImplRecursiveDamageSplash(RL_D_TAIL) {
}

int32 SkillDragonTail::getSplashTarget(block_list* src) const {
	return BL_CHAR;
}

void SkillDragonTail::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	const status_change* tsc = status_get_sc(target);

	// TODO: do we need to check the src id?
	if (tsc != nullptr && tsc->hasSCE(SC_C_MARKER) && tsc->getSCE(SC_C_MARKER)->val2 == src->id) {
		skill_area_temp[0] |= SKILL_ALTDMG_FLAG;
	}

	// Disable skill animation
	skill_area_temp[1] = 0;

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

void SkillDragonTail::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += -100 + 500 + 200 * skill_lv;

	if (wd->miscflag & SKILL_ALTDMG_FLAG) {
		skillratio *= 2;
	}

	RE_LVL_DMOD(100);
}

SkillDust::SkillDust() : WeaponSkillImpl(GS_DUST) {
}

void SkillDust::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 50 * skill_lv;
}

SkillFallenAngel::SkillFallenAngel() : SkillImpl(RL_FALLEN_ANGEL) {
}

void SkillFallenAngel::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	sc_type type = skill_get_sc(getSkillId());

	if (unit_movepos(src, x, y, 1, 1)) {
		clif_snap(src, src->x, src->y);
		sc_start(src, src, type, 100, getSkillId(), skill_get_time(getSkillId(), skill_lv));
	} else if (sd != nullptr) {
		clif_skill_fail(*sd, getSkillId());
	}
}

SkillFireDance::SkillFireDance() : SkillImplRecursiveDamageSplash(RL_FIREDANCE) {
}

void SkillFireDance::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += 100 + 100 * skill_lv;
	skillratio += (sd ? pc_checkskill(sd, GS_DESPERADO) * 20 : 0);
	RE_LVL_DMOD(100);
}

void SkillFireDance::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillFireRain::SkillFireRain() : SkillImpl(RL_FIRE_RAIN) {
}

void SkillFireRain::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 wave = skill_lv + 5;
	int32 dir = map_calc_dir(src, x, y);
	int32 sx = src->x;
	int32 sy = src->y;

	x = src->x;
	y = src->y;

	for (int32 w = 0; w <= wave; ++w) {
		switch (dir) {
			case DIR_NORTH:
			case DIR_NORTHWEST:
			case DIR_NORTHEAST:
				sy = y + w;
				break;
			case DIR_WEST:
				sx = x - w;
				break;
			case DIR_SOUTHWEST:
			case DIR_SOUTH:
			case DIR_SOUTHEAST:
				sy = y - w;
				break;
			case DIR_EAST:
				sx = x + w;
				break;
		}
		skill_addtimerskill(src, gettick() + (80 * w), 0, sx, sy, getSkillId(), skill_lv, dir, flag);
	}
}

void SkillFireRain::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += -100 + 3500 + 300 * skill_lv;
}

static int32 skill_bind_trap(block_list* bl, va_list ap);

SkillFlicker::SkillFlicker() : SkillImpl(RL_FLICKER) {
}

void SkillFlicker::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		sd->flicker = true;
		skill_area_temp[1] = 0;
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		// Detonate RL_B_TRAP
		if (pc_checkskill(sd, RL_B_TRAP)) {
			map_foreachinallrange(skill_bind_trap, src, AREA_SIZE, BL_SKILL, src);
		}
		// Detonate RL_H_MINE
		if (int32 mine_lv = pc_checkskill(sd, RL_H_MINE)) {
			map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, RL_H_MINE, mine_lv, tick, flag | BCT_ENEMY | SD_SPLASH, skill_castend_damage_id);
		}
		sd->flicker = false;
	}
}

/**
 * Rebellion's Bind Trap explosion
 * @author [Cydh]
 */
static int32 skill_bind_trap(block_list *bl, va_list ap) {
	skill_unit *su = nullptr;
	block_list *src = nullptr;

	nullpo_ret(bl);

	src = va_arg(ap,block_list *);

	if (bl->type != BL_SKILL || !(su = (skill_unit *)bl) || !(su->group))
		return 0;
	if (su->group->unit_id != UNT_B_TRAP || su->group->src_id != src->id)
		return 0;

	map_foreachinallrange(skill_trap_splash, bl, su->range, BL_CHAR, bl,su->group->tick);
	clif_changetraplook(bl, UNT_USED_TRAPS);
	su->group->unit_id = UNT_USED_TRAPS;
	su->group->limit = DIFF_TICK(gettick(), su->group->tick) + 500;
	return 1;
}

SkillFling::SkillFling() : SkillImpl(GS_FLING) {
}

void SkillFling::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillFling::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *sd = BL_CAST(BL_PC, src);

	sc_start(src, target, SC_FLING, 100, sd ? sd->spiritball_old : 5, skill_get_time(getSkillId(), skill_lv));
}

SkillFullBuster::SkillFullBuster() : WeaponSkillImpl(GS_FULLBUSTER) {
}

void SkillFullBuster::applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const {
	sc_start(src, src, SC_BLIND, 2 * skill_lv, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillFullBuster::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv + 2);
}

SkillGatlingfever::SkillGatlingfever() : SkillImpl(GS_GATLINGFEVER) {
}

void SkillGatlingfever::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	status_change_entry *tsce = (tsc) ? tsc->getSCE(type) : nullptr;

	if (tsce) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, status_change_end(target, type));
		return;
	}

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillGlittering::SkillGlittering() : SkillImpl(GS_GLITTERING) {
}

void SkillGlittering::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		if (rnd() % 100 < (20 + 10 * skill_lv))
			pc_addspiritball(sd, skill_get_time(getSkillId(), skill_lv), 10);
		else if (sd->spiritball > 0 && !pc_checkskill(sd, RL_RICHS_COIN))
			pc_delspiritball(sd, 1, 0);
	}
}

SkillGrenadeFragment::SkillGrenadeFragment() : SkillImpl(NW_GRENADE_FRAGMENT) {
}

void SkillGrenadeFragment::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(src, skill_get_sc(getSkillId()));
	if (skill_lv < 7)
		sc_start(src, target, (sc_type)(SC_GRENADE_FRAGMENT_1 -1 + skill_lv), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	else if (skill_lv == 7) {
		status_change_end(src, SC_GRENADE_FRAGMENT_1);
		status_change_end(src, SC_GRENADE_FRAGMENT_2);
		status_change_end(src, SC_GRENADE_FRAGMENT_3);
		status_change_end(src, SC_GRENADE_FRAGMENT_4);
		status_change_end(src, SC_GRENADE_FRAGMENT_5);
		status_change_end(src, SC_GRENADE_FRAGMENT_6);
	}
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
}

SkillGrenadesDropping::SkillGrenadesDropping() : SkillImpl(NW_GRENADES_DROPPING) {
}

void SkillGrenadesDropping::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	uint16 splash = skill_get_splash(getSkillId(), skill_lv);
	uint16 tmpx = rnd_value(x - splash, x + splash);
	uint16 tmpy = rnd_value(y - splash, y + splash);
	skill_unitsetting(src, getSkillId(), skill_lv, tmpx, tmpy, flag);
	for (int32 i = 0; i <= (skill_get_time(getSkillId(), skill_lv) / skill_get_unit_interval(getSkillId())); i++) {
		skill_addtimerskill(src, tick + (t_tick)i * skill_get_unit_interval(getSkillId()), 0, x, y, getSkillId(), skill_lv, 0, flag);
	}
}

void SkillGrenadesDropping::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 550 + 850 * skill_lv;
	skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 30;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

void SkillGrenadesDropping::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	// Night Watch Grenade Fragment elementals
	if( sc != nullptr ){
		if( sc->hasSCE( SC_GRENADE_FRAGMENT_1 ) ){
			element = ELE_WATER;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_2 ) ){
			element = ELE_WIND;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_3 ) ){
			element = ELE_EARTH;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_4 ) ){
			element = ELE_FIRE;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_5 ) ){
			element = ELE_DARK;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_6 ) ){
			element = ELE_HOLY;
		}
	}
}

SkillGroundDrift::SkillGroundDrift() : SkillImpl(GS_GROUNDDRIFT) {
}

void SkillGroundDrift::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_data* sstatus = status_get_status_data(src);

	dmg.amotion = sstatus->amotion;
	dmg.blewcount = 0;
}

void SkillGroundDrift::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 100 + 20 * skill_lv;
#endif
}

void SkillGroundDrift::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Ammo should be deleted right away.
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillGroundDrift::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	element = dmg.miscflag; // element comes in flag.
}

SkillHammerOfGod::SkillHammerOfGod() : SkillImplRecursiveDamageSplash(RL_HAMMER_OF_GOD) {
}


void SkillHammerOfGod::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* tsc = status_get_sc(target);

	if (flag & 1) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag | SD_ANIMATION);
		return;
	}

	if (sd && tsc && tsc->getSCE(SC_C_MARKER)) {
		int32 i = 0;

		ARR_FIND(0, MAX_SKILL_CRIMSON_MARKER, i, sd->c_marker[i] == target->id);
		if (i < MAX_SKILL_CRIMSON_MARKER) {
			flag |= 8;
		}
	}

	clif_skill_poseffect(*src, getSkillId(), 1, target->x, target->y, gettick());
	map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
}

void SkillHammerOfGod::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 100 * skill_lv;
	if (sd) {
		if (wd->miscflag & 8) {
			skillratio += 400 * sd->spiritball_old;
		} else {
			skillratio += 150 * sd->spiritball_old;
		}
	}
	RE_LVL_DMOD(100);
}

SkillHastyFireInTheHole::SkillHastyFireInTheHole() : WeaponSkillImpl(NW_HASTY_FIRE_IN_THE_HOLE) {
}

void SkillHastyFireInTheHole::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	if (flag & 1){
		i++;
	}
	if (flag & 2){
		i++;
	}
	map_foreachinallarea(skill_area_sub,
		src->m, x - i, y - i, x + i, y + i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1,
		skill_castend_damage_id);
	if (!(flag & 1)) {
		skill_addtimerskill(src, tick + 300, 0, x, y, getSkillId(), skill_lv, 0, flag | 1 | SKILL_NOCONSUME_REQ);
		skill_addtimerskill(src, tick + 600, 0, x, y, getSkillId(), skill_lv, 0, flag | 3 | SKILL_NOCONSUME_REQ);
	}
}

void SkillHastyFireInTheHole::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1500 + 1500 * skill_lv;
	skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 20;
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

void SkillHastyFireInTheHole::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	// Night Watch Grenade Fragment elementals
	if( sc != nullptr ){
		if( sc->hasSCE( SC_GRENADE_FRAGMENT_1 ) ){
			element = ELE_WATER;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_2 ) ){
			element = ELE_WIND;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_3 ) ){
			element = ELE_EARTH;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_4 ) ){
			element = ELE_FIRE;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_5 ) ){
			element = ELE_DARK;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_6 ) ){
			element = ELE_HOLY;
		}
	}
}

// TODO: Refactor to SkillImplRecursiveDamageSplash
SkillHowlingMine::SkillHowlingMine() : SkillImpl(RL_H_MINE) {
}

void SkillHowlingMine::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	status_change* tsc = status_get_sc(target);

	if (!(flag & 1)) {
		// Direct attack
		if (!sd || !sd->flicker) {
			if (skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag)) {
				status_change_start(src, target, SC_H_MINE, 10000, getSkillId(), 0, 0, 0, skill_get_time(getSkillId(), skill_lv), SCSTART_NOAVOID | SCSTART_NOTICKDEF | SCSTART_NORATEDEF);
			}
			return;
		}

		// Triggered by RL_FLICKER
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR | BL_SKILL,
			src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
		flag |= 1; // Don't consume requirement

		if (tsc && tsc->getSCE(SC_H_MINE) && tsc->getSCE(SC_H_MINE)->val2 == src->id) {
			status_change_end(target, SC_H_MINE);
			sc_start4(src, target, SC_BURNING, 10 * skill_lv, skill_lv, 1000, src->id, 0, skill_get_time2(getSkillId(), skill_lv));
		}
	} else {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	}

	if (sd && sd->flicker) {
		flag |= 1; // Don't consume requirement
	}
}

void SkillHowlingMine::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && sd->flicker) {
		// Flicker explosion damage: 500 + 300 * SkillLv
		skillratio += -100 + 500 + 300 * skill_lv;
	} else {
		// Direct trigger damage: 200 + 200 * SkillLv
		skillratio += -100 + 200 + 200 * skill_lv;
	}
}

void SkillHowlingMine::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->flicker) //Force RL_H_MINE deals fire damage if activated by RL_FLICKER
		element = ELE_FIRE;
}

SkillIntensiveAim::SkillIntensiveAim() : SkillImpl(NW_INTENSIVE_AIM) {
}

void SkillIntensiveAim::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	enum sc_type type = skill_get_sc(getSkillId());
	status_change* tsc = status_get_sc(target);

	if (tsc && tsc->getSCE(type)) {
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
		status_change_end(target, type);
	} else {
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
		sc_start(src, target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
}

SkillMagazineForOne::SkillMagazineForOne() : WeaponSkillImpl(NW_MAGAZINE_FOR_ONE) {
}

void SkillMagazineForOne::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->weapontype1 == W_GATLING)
		dmg.div_ += 4;
}

void SkillMagazineForOne::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* sc = status_get_sc(src);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);

	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
}

void SkillMagazineForOne::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 250 + 500 * skill_lv;
	skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 100 * skill_lv;
	if (sd && sd->weapontype1 == W_REVOLVER)
		skillratio += 50 + 300 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillMassSpiral::SkillMassSpiral() : WeaponSkillImpl(RL_MASS_SPIRAL) {
}

void SkillMassSpiral::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += -100 + 200 * skill_lv;
}

void SkillMassSpiral::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start2(src, target, SC_BLEEDING, 30 + 10 * skill_lv, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv));
}

SkillMidnightFallen::SkillMidnightFallen() : WeaponSkillImpl(NW_MIDNIGHT_FALLEN) {
}

void SkillMidnightFallen::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	int32 splash = skill_get_splash(getSkillId(), skill_lv);
	if (sd != nullptr) {
		if (sd->status.weapon == W_GATLING)
			splash += 1;
		else if (sd->status.weapon == W_GRENADE)
			splash += 2;
	}
	map_foreachinallarea(skill_area_sub, src->m, x - splash, y - splash, x + splash, y + splash, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
}

void SkillMidnightFallen::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 2500 + 850 * skill_lv;
	if (sd != nullptr && sc != nullptr && sc->hasSCE(SC_HIDDEN_CARD)) {
		if (sd->weapontype1 == W_GATLING)
			skillratio += 200 * skill_lv;
		else if (sd->weapontype1 == W_GRENADE)
			skillratio += 340 * skill_lv;
		else if (sd->weapontype1 == W_SHOTGUN)
			skillratio += 400 * skill_lv;
	}
	skillratio += 5 * sstatus->con; //!TODO: check con ratio
	RE_LVL_DMOD(100);
}

SkillMissionBombard::SkillMissionBombard() : WeaponSkillImpl(NW_MISSION_BOMBARD) {
}

void SkillMissionBombard::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinarea(skill_area_sub, src->m, x - i, y - i, x + i, y + i, BL_CHAR | BL_SKILL, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SKILL_ALTDMG_FLAG | 1, skill_castend_damage_id);
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, flag);

	for (i = 1; i <= (skill_get_time(getSkillId(), skill_lv) / skill_get_unit_interval(getSkillId())); i++) {
		skill_addtimerskill(src, tick + (t_tick)i * skill_get_unit_interval(getSkillId()), 0, x, y, getSkillId(), skill_lv, 0, flag);
	}
}

void SkillMissionBombard::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	if (wd->miscflag & SKILL_ALTDMG_FLAG) {
		skillratio += -100 + 5000 + 1800 * skill_lv;
		skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 100;
	}
	else {
		skillratio += -100 + 800 + 200 * skill_lv;
		skillratio += pc_checkskill(sd, NW_GRENADE_MASTERY) * 30;
	}
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

void SkillMissionBombard::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	// Night Watch Grenade Fragment elementals
	if( sc != nullptr ){
		if( sc->hasSCE( SC_GRENADE_FRAGMENT_1 ) ){
			element = ELE_WATER;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_2 ) ){
			element = ELE_WIND;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_3 ) ){
			element = ELE_EARTH;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_4 ) ){
			element = ELE_FIRE;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_5 ) ){
			element = ELE_DARK;
		}else if( sc->hasSCE( SC_GRENADE_FRAGMENT_6 ) ){
			element = ELE_HOLY;
		}
	}
}

SkillOnlyOneBullet::SkillOnlyOneBullet() : WeaponSkillImpl(NW_ONLY_ONE_BULLET) {
}

void SkillOnlyOneBullet::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* sc = status_get_sc(src);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);

	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
}

void SkillOnlyOneBullet::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1200 + 3000 * skill_lv;
	skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 350 * skill_lv;
	if (sd && sd->weapontype1 == W_REVOLVER) {
		skillratio += 400 * skill_lv;
	}
	RE_LVL_DMOD(100);
}

SkillPiercingShot::SkillPiercingShot() : WeaponSkillImpl(GS_PIERCINGSHOT) {
}

void SkillPiercingShot::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && sd->weapontype1 == W_RIFLE)
		base_skillratio += 150 + 30 * skill_lv;
	else
		base_skillratio += 100 + 20 * skill_lv;
#else
	base_skillratio += 20 * skill_lv;
#endif
}

void SkillPiercingShot::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start2(src, target, SC_BLEEDING, (skill_lv * 3), skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv));
}

SkillQuickDrawShot::SkillQuickDrawShot() : SkillImpl(RL_QD_SHOT) {
}

void SkillQuickDrawShot::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	dmg.div_ = 1;
	
	if (sd != nullptr) {
		dmg.div_ += sd->status.job_level / 20;
	}
}

void SkillQuickDrawShot::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Remember main target as it will always be hit by this skill
	skill_area_temp[1] = target->id;
	// Iterate through all enemies in the area
	map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_damage_id);
	// End here to prevent spamming of the skill onto the target
	status_change_end(src, SC_QD_SHOT_READY);
	skill_area_temp[1] = 0;
}

void SkillQuickDrawShot::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* tsc = status_get_sc(target);

	// Except for main target, only units marked with crimson marker are valid targets
	if (skill_area_temp[1] == target->id || (tsc != nullptr && tsc->getSCE(SC_C_MARKER) != nullptr)) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	}
}

SkillRapidShower::SkillRapidShower() : WeaponSkillImpl(GS_RAPIDSHOWER) {
}

void SkillRapidShower::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 400 + 50 * skill_lv;
}

SkillRichsCoin::SkillRichsCoin() : SkillImpl(RL_RICHS_COIN) {
}

void SkillRichsCoin::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		for (int32 i = 0; i < 10; i++) {
			pc_addspiritball(sd, skill_get_time(getSkillId(), skill_lv), 10);
		}
	}
}

// RL_R_TRIP
SkillRoundTrip::SkillRoundTrip() : SkillImplRecursiveDamageSplash(RL_R_TRIP) {
}

void SkillRoundTrip::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 350 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillRoundTrip::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}


// RL_R_TRIP_PLUSATK
SkillRoundTripPlusAttack::SkillRoundTripPlusAttack() : SkillImpl(RL_R_TRIP_PLUSATK) {
}

void SkillRoundTripPlusAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 300 + 300 * skill_lv;
}

SkillShatterStorm::SkillShatterStorm() : SkillImplRecursiveDamageSplash(RL_S_STORM) {
}

void SkillShatterStorm::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data* sstatus = status_get_status_data(*src);
	status_data* tstatus = status_get_status_data(*target);

	//kRO update 2014-02-12. Break a headgear by minimum chance 5%/10%/15%/20%/25%
	//! TODO: Figure out break chance formula
	skill_break_equip(src, target, EQP_HEAD_TOP, max(skill_lv * 500, (sstatus->dex * skill_lv * 10) - (tstatus->agi * 20)), BCT_ENEMY);
}

void SkillShatterStorm::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += -100 + 1700 + 200 * skill_lv;
}

SkillSlugShot::SkillSlugShot() : WeaponSkillImpl(RL_SLUGSHOT) {
}

void SkillSlugShot::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_STUN, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillSlugShot::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* tstatus = status_get_status_data(*target);

	if (target->type == BL_MOB) {
		skillratio += -100 + 1200 * skill_lv;
	} else {
		skillratio += -100 + 2000 * skill_lv;
	}
	skillratio *= 2 + tstatus->size;
}

void SkillSlugShot::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	int8 dist = distance_bl(src, target);

	if (dist > 3) {
		// Reduce n hitrate for each cell after initial 3 cells. Different each level
		// -10:-9:-8:-7:-6
		dist -= 3;
		hit_rate -= ((11 - skill_lv) * dist);
	}
}

SkillSpiralShooting::SkillSpiralShooting() : SkillImpl(NW_SPIRAL_SHOOTING) {
}

void SkillSpiralShooting::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->weapontype1 == W_GRENADE)
		dmg.div_ += 1;
}

void SkillSpiralShooting::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_change* sc = status_get_sc(src);

	if (flag & 1) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	} else {
		int32 splash = skill_get_splash(getSkillId(), skill_lv);

		if (sd && sd->weapontype1 == W_GRENADE)
			splash += 2;
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinrange(skill_area_sub, target, splash, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
		if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
			status_change_end(src, SC_INTENSIVE_AIM_COUNT);
	}
}

void SkillSpiralShooting::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1200 + 1700 * skill_lv;
	skillratio += 5 * sstatus->con;
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 150 * skill_lv;
	if (sd && sd->weapontype1 == W_RIFLE)
		skillratio += 200 + 1100 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillSpreadAttack::SkillSpreadAttack() : SkillImplRecursiveDamageSplash(GS_SPREADATTACK) {
}

void SkillSpreadAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 30 * skill_lv;
#else
	base_skillratio += 20 * (skill_lv - 1);
#endif
}

SkillTheVigilanteAtNight::SkillTheVigilanteAtNight() : SkillImpl(NW_THE_VIGILANTE_AT_NIGHT) {
}

void SkillTheVigilanteAtNight::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->weapontype1 == W_GATLING)
		dmg.div_ += 3;
}

void SkillTheVigilanteAtNight::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_change* sc = status_get_sc(src);

	int32 i = skill_get_splash(getSkillId(), skill_lv);
	skill_area_temp[0] = 0;
	skill_area_temp[1] = target->id;
	skill_area_temp[2] = 0;

	if (sd && sd->weapontype1 == W_GATLING) {
		i = 5; // 11x11
		clif_skill_nodamage(src, *target, NW_THE_VIGILANTE_AT_NIGHT_GUN_GATLING, skill_lv);
	} else
		clif_skill_nodamage(src, *target, NW_THE_VIGILANTE_AT_NIGHT_GUN_SHOTGUN, skill_lv);
	map_foreachinrange(skill_area_sub, target, i, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
}

void SkillTheVigilanteAtNight::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillTheVigilanteAtNight::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	if (sd && sd->weapontype1 == W_GATLING) {
		skillratio += -100 + 350 * skill_lv;
		if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
			skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 100 * skill_lv;
	} else {
		skillratio += -100 + 850 + 800 * skill_lv;
		if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
			skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 200 * skill_lv;
	}
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}

SkillTracking::SkillTracking() : WeaponSkillImpl(GS_TRACKING) {
}

void SkillTracking::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100 * (skill_lv + 1);
}

SkillTripleAction::SkillTripleAction() : WeaponSkillImpl(GS_TRIPLEACTION) {
}

void SkillTripleAction::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 50 * skill_lv;
}

SkillWildFire::SkillWildFire() : WeaponSkillImpl(NW_WILD_FIRE) {
}

void SkillWildFire::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_change* sc = status_get_sc(src);

	int32 i = skill_get_splash(getSkillId(), skill_lv);
	if (sd && sd->status.weapon == W_GRENADE)
		i += 2;
	map_foreachinallarea(skill_area_sub,
		src->m, x - i, y - i, x + i, y + i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1,
		skill_castend_damage_id);
	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		status_change_end(src, SC_INTENSIVE_AIM_COUNT);
}

void SkillWildFire::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1500 + 3450 * skill_lv;
	skillratio += 5 * sstatus->con;

	if (sc && sc->getSCE(SC_INTENSIVE_AIM_COUNT))
		skillratio += sc->getSCE(SC_INTENSIVE_AIM_COUNT)->val1 * 500 * skill_lv;

	if (sd && sd->weapontype1 == W_SHOTGUN)
		skillratio += 100 * skill_lv;

	RE_LVL_DMOD(100);
}

SkillWildShot::SkillWildShot() : SkillImpl(NW_WILD_SHOT) {
}

void SkillWildShot::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
	} else {
		int32 splash = skill_get_splash(getSkillId(), skill_lv);

		if (sd != nullptr && sd->weapontype1 == W_RIFLE)
			splash += 1;
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, 1);
		map_foreachinrange(skill_area_sub, target, splash, BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);

	}
}

void SkillWildShot::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_change* sc = status_get_sc(src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 850 + 230 * skill_lv;
	if (sd != nullptr && sc != nullptr && sc->hasSCE(SC_HIDDEN_CARD)) {
		if (sd->weapontype1 == W_REVOLVER)
			skillratio += 100 * skill_lv;
		else if (sd->weapontype1 == W_RIFLE)
			skillratio += 150 * skill_lv;
	}
	skillratio += 5 * sstatus->con; //!TODO: check con ratio
	RE_LVL_DMOD(100);
}

std::unique_ptr<const SkillImpl> SkillFactoryGunslinger::create(const e_skill skill_id) const {
	switch (skill_id) {
		case GS_ADJUSTMENT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case GS_BULLSEYE:
			return std::make_unique<SkillBullseye>();
		case GS_CHAINACTION:
			return std::make_unique<SkillChainAction>();
		case GS_CRACKER:
			return std::make_unique<SkillCracker>();
		case GS_DESPERADO:
			return std::make_unique<SkillDesperado>();
		case GS_DISARM:
			return std::make_unique<SkillDisarm>();
		case GS_DUST:
			return std::make_unique<SkillDust>();
		case GS_FLING:
			return std::make_unique<SkillFling>();
		case GS_FULLBUSTER:
			return std::make_unique<SkillFullBuster>();
		case GS_GATLINGFEVER:
			return std::make_unique<SkillGatlingfever>();
		case GS_GLITTERING:
			return std::make_unique<SkillGlittering>();
		case GS_GROUNDDRIFT:
			return std::make_unique<SkillGroundDrift>();
		case GS_INCREASING:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case GS_MADNESSCANCEL:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case GS_MAGICALBULLET:
#ifdef RENEWAL
			return std::make_unique<StatusSkillImpl>(skill_id);
#else
			return std::make_unique<WeaponSkillImpl>(skill_id);
#endif
		case GS_PIERCINGSHOT:
			return std::make_unique<SkillPiercingShot>();
		case GS_RAPIDSHOWER:
			return std::make_unique<SkillRapidShower>();
		case GS_SPREADATTACK:
			return std::make_unique<SkillSpreadAttack>();
		case GS_TRACKING:
			return std::make_unique<SkillTracking>();
		case GS_TRIPLEACTION:
			return std::make_unique<SkillTripleAction>();
		case NW_AUTO_FIRING_LAUNCHER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NW_BASIC_GRENADE:
			return std::make_unique<SkillBasicGrenade>();
		case NW_GRENADES_DROPPING:
			return std::make_unique<SkillGrenadesDropping>();
		case NW_GRENADE_FRAGMENT:
			return std::make_unique<SkillGrenadeFragment>();
		case NW_HASTY_FIRE_IN_THE_HOLE:
			return std::make_unique<SkillHastyFireInTheHole>();
		case NW_HIDDEN_CARD:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case NW_INTENSIVE_AIM:
			return std::make_unique<SkillIntensiveAim>();
		case NW_MAGAZINE_FOR_ONE:
			return std::make_unique<SkillMagazineForOne>();
		case NW_MIDNIGHT_FALLEN:
			return std::make_unique<SkillMidnightFallen>();
		case NW_MISSION_BOMBARD:
			return std::make_unique<SkillMissionBombard>();
		case NW_ONLY_ONE_BULLET:
			return std::make_unique<SkillOnlyOneBullet>();
		case NW_SPIRAL_SHOOTING:
			return std::make_unique<SkillSpiralShooting>();
		case NW_THE_VIGILANTE_AT_NIGHT:
			return std::make_unique<SkillTheVigilanteAtNight>();
		case NW_WILD_FIRE:
			return std::make_unique<SkillWildFire>();
		case NW_WILD_SHOT:
			return std::make_unique<SkillWildShot>();
		case RL_AM_BLAST:
			return std::make_unique<SkillAntiMaterialBlast>();
		case RL_BANISHING_BUSTER:
			return std::make_unique<SkillBanishingBuster>();
		case RL_B_TRAP:
			return std::make_unique<SkillBindTrap>();
		case RL_C_MARKER:
			return std::make_unique<SkillCrimsonMarker>();
		case RL_D_TAIL:
			return std::make_unique<SkillDragonTail>();
		case RL_E_CHAIN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case RL_FALLEN_ANGEL:
			return std::make_unique<SkillFallenAngel>();
		case RL_FIREDANCE:
			return std::make_unique<SkillFireDance>();
		case RL_FIRE_RAIN:
			return std::make_unique<SkillFireRain>();
		case RL_FLICKER:
			return std::make_unique<SkillFlicker>();
		case RL_HAMMER_OF_GOD:
			return std::make_unique<SkillHammerOfGod>();
		case RL_HEAT_BARREL:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case RL_H_MINE:
			return std::make_unique<SkillHowlingMine>();
		case RL_MASS_SPIRAL:
			return std::make_unique<SkillMassSpiral>();
		case RL_P_ALTER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case RL_QD_SHOT:
			return std::make_unique<SkillQuickDrawShot>();
		case RL_RICHS_COIN:
			return std::make_unique<SkillRichsCoin>();
		case RL_R_TRIP:
			return std::make_unique<SkillRoundTrip>();
		case RL_R_TRIP_PLUSATK:
			return std::make_unique<SkillRoundTripPlusAttack>();
		case RL_S_STORM:
			return std::make_unique<SkillShatterStorm>();
		case RL_SLUGSHOT:
			return std::make_unique<SkillSlugShot>();

		default:
			return nullptr;
	}
}

#endif
