// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_acolyte.hpp"

#include "map/clif.hpp"
#include "map/mob.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include <config/core.hpp>
#include "map/map.hpp"
#include "map/party.hpp"
#include "map/unit.hpp"
#include "map/battle.hpp"
#include <common/random.hpp>
#include "skill_impl.hpp"

SkillAbsorbSpiritSphere::SkillAbsorbSpiritSphere() : SkillImpl(MO_ABSORBSPIRITS) {
}

void SkillAbsorbSpiritSphere::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_data* tstatus = status_get_status_data(*target);

	int32 i = 0;
	if (dstsd && (battle_check_target(src, target, BCT_SELF) > 0 || battle_check_target(src, target, BCT_ENEMY) > 0) && // Only works on self and enemies
		(dstsd->class_&MAPID_FIRSTMASK) != MAPID_GUNSLINGER ) { // split the if for readability, and included gunslingers in the check so that their coins cannot be removed [Reddozen]
		if (dstsd->spiritball > 0) {
			i = dstsd->spiritball * 7;
			pc_delspiritball(dstsd,dstsd->spiritball,0);
		}
		if (dstsd->spiritcharm_type != CHARM_TYPE_NONE && dstsd->spiritcharm > 0) {
			i += dstsd->spiritcharm * 7;
			pc_delspiritcharm(dstsd,dstsd->spiritcharm,dstsd->spiritcharm_type);
		}
	} else if (dstmd && !status_has_mode(tstatus,MD_STATUSIMMUNE) && rnd() % 100 < 20) { // check if target is a monster and not status immune, for the 20% chance to absorb 2 SP per monster's level [Reddozen]
		i = 2 * dstmd->level;
		mob_target(dstmd,src,0);
	} else {
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		return;
	}
	if (i) status_heal(src, 0, i, 3);
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv,i != 0);
}

SkillAdoramus::SkillAdoramus() : SkillImplRecursiveDamageSplash(AB_ADORAMUS) {
}

void SkillAdoramus::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	sc_start(src,target, SC_ADORAMUS, skill_lv * 4 + (sd ? sd->status.job_level : 50) / 2, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}

void SkillAdoramus::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += - 100 + 300 + 250 * skill_lv;
	RE_LVL_DMOD(100);
}

int64 SkillAdoramus::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	if (map_getcell(target->m, target->x, target->y, CELL_CHKLANDPROTECTOR))
		return 0; // No damage should happen if the target is on Land Protector

	return SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag);
}

void SkillAdoramus::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_ANCILLA))
		element = ELE_NEUTRAL;
}

SkillAncilla::SkillAncilla() : SkillImpl(AB_ANCILLA) {
}

void SkillAncilla::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( sd ) {
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		skill_produce_mix(sd, getSkillId(), ITEMID_ANCILLA, 0, 0, 0, 1, -1);
	}
}

SkillAngelus::SkillAngelus() : SkillImpl(AL_ANGELUS)
{
}

void SkillAngelus::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1))
	{
		sc_type type = skill_get_sc(getSkillId());

		// Animations don't play when outside visible range
		if (check_distance_bl(src, bl, AREA_SIZE))
			clif_skill_nodamage(bl, *bl, getSkillId(), skill_lv);


		sc_start(src, bl, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
	else if (sd != nullptr)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
	}
}

SkillArbitrium::SkillArbitrium() : SkillImplRecursiveDamageSplash(CD_ARBITRIUM) {
}

void SkillArbitrium::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1200 * skill_lv;
	skillratio += 10 * sstatus->spl;	// TODO : spl ratio has changed ?
	skillratio += 35 * pc_checkskill(sd, CD_FIDUS_ANIMUS) * skill_lv;

	RE_LVL_DMOD(100);
}

// TODO : CD_ARBITRIUM_ATK is no longer used ?
SkillArbitriumAttack::SkillArbitriumAttack() : SkillImplRecursiveDamageSplash(CD_ARBITRIUM_ATK) {
}

void SkillArbitriumAttack::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1750 * skill_lv + 10 * sstatus->spl;
	skillratio += 50 * pc_checkskill(sd, CD_FIDUS_ANIMUS) * skill_lv;
	RE_LVL_DMOD(100);
}

SkillAspersio::SkillAspersio() : SkillImpl(PR_ASPERSIO) {
}

void SkillAspersio::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if (sd && dstmd) {
		clif_skill_nodamage(src,*target,getSkillId(), skill_lv, false);
		return;
	}
	clif_skill_nodamage(src,*target, getSkillId(),skill_lv,
		sc_start(src,target,skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

void SkillAspersio::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillAssimilatePower::SkillAssimilatePower() : SkillImpl(SR_ASSIMILATEPOWER) {
}

void SkillAssimilatePower::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (flag & 1) {
		int32 amount = 0;

		if (dstsd && (sd == dstsd || map_flag_vs(src->m)) && (dstsd->class_ & MAPID_FIRSTMASK) != MAPID_GUNSLINGER) {
			if (dstsd->spiritball > 0) {
				amount = dstsd->spiritball;
				pc_delspiritball(dstsd, dstsd->spiritball, 0);
			}
			if (dstsd->spiritcharm_type != CHARM_TYPE_NONE && dstsd->spiritcharm > 0) {
				amount += dstsd->spiritcharm;
				pc_delspiritcharm(dstsd, dstsd->spiritcharm, dstsd->spiritcharm_type);
			}
		}

		if (amount)
			status_percent_heal(src, 0, amount);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, amount != 0);
	} else {
		clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE);
		map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), splash_target(src), src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | BCT_SELF | SD_SPLASH | 1, skill_castend_nodamage_id);
	}
}

SkillAssumptio::SkillAssumptio() : StatusSkillImpl(HP_ASSUMPTIO) {
}

void SkillAssumptio::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if( sd && dstmd )
		clif_skill_fail( *sd, getSkillId() );
	else
		StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillAsuraStrike::SkillAsuraStrike() : WeaponSkillImpl(MO_EXTREMITYFIST) {
}

void SkillAsuraStrike::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int16 x, y, i = 3; // Move 3 cells (From caster)
	int16 dir = map_calc_dir(src,target->x,target->y);

#ifdef RENEWAL
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd && sd->spiritball_old > 5)
		flag |= 1; // Give +100% damage increase
#endif
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);

	status_set_sp(src, 0, 0);
	sc_start(src, src, SC_EXTREMITYFIST, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	status_change_end(src, SC_EXPLOSIONSPIRITS);
	status_change_end(src, SC_BLADESTOP);

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

	if (unit_movepos(src, src->x + x, src->y + y, 1, 1)) {
		clif_blown(src);
		clif_spiritball(src);
	}
}

void SkillAsuraStrike::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += 700 + sstatus->sp * 10;
#ifdef RENEWAL
	if (wd->miscflag&1)
		base_skillratio *= 2; // More than 5 spirit balls active
#endif
	base_skillratio = min(500000,base_skillratio); //We stop at roughly 50k SP for overflow protection
}

SkillBasilica::SkillBasilica() : StatusSkillImpl(HP_BASILICA) {
}

void SkillBasilica::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
#endif
}

void SkillBasilica::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifndef RENEWAL
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( status_change *sc = status_get_sc(src); sc && sc->getSCE(SC_BASILICA) ) {
		status_change_end(src, SC_BASILICA); // Cancel Basilica and return so requirement isn't consumed again
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	if( map_getcell(src->m, x, y, CELL_CHKLANDPROTECTOR) ) {
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}
	
	// Create Basilica
	skill_clear_unitgroup(src);
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
	flag|=1;
#endif
}

SkillBlazingFlameBlast::SkillBlazingFlameBlast() : WeaponSkillImpl(IQ_BLAZING_FLAME_BLAST) {
}

void SkillBlazingFlameBlast::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillBlazingFlameBlast::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* sc = status_get_sc(src);

	skillratio += -100 + 2000 + 3800 * skill_lv;
	skillratio += 10 * sstatus->pow;	// !TODO: unknown ratio
	if (sc != nullptr && sc->hasSCE(SC_MASSIVE_F_BLASTER))
		skillratio += 1500 + 400 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillBlessing::SkillBlessing() : SkillImpl(AL_BLESSING)
{
}

void SkillBlessing::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	map_session_data *dstsd = BL_CAST(BL_PC, bl);
	status_change *tsc = status_get_sc(bl);
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
	if (dstsd != nullptr && tsc && tsc->getSCE(SC_CHANGEUNDEAD))
	{
		status_data* tstatus = status_get_status_data(*bl);
		if (tstatus->hp > 1)
			skill_attack(BF_MISC, src, src, bl, getSkillId(), skill_lv, tick, flag);
		return;
	}
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillBenedictioSanctissimiSacramenti::SkillBenedictioSanctissimiSacramenti() : SkillImpl(PR_BENEDICTIO) {
}

void SkillBenedictioSanctissimiSacramenti::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	if (!battle_check_undead(tstatus->race, tstatus->def_ele) && tstatus->race != RC_DEMON)
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

void SkillBenedictioSanctissimiSacramenti::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	//Should attack undead and demons. [Skotlex]
	if (battle_check_undead(tstatus->race, tstatus->def_ele) || tstatus->race == RC_DEMON)
		skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillBenedictioSanctissimiSacramenti::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_area_temp[1] = src->id;
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub,
		src->m, x-i, y-i, x+i, y+i, BL_PC,
		src, getSkillId(), skill_lv, tick, flag|BCT_ALL|1,
		skill_castend_nodamage_id);
	map_foreachinallarea(skill_area_sub,
		src->m, x-i, y-i, x+i, y+i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1,
		skill_castend_damage_id);
}

SkillCantoCandidus::SkillCantoCandidus() : SkillImpl(AB_CANTO) {
}

void SkillCantoCandidus::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	int32 agi_lv = ((sd) ? pc_checkskill(sd,AL_INCAGI) : skill_get_max(AL_INCAGI)) + (((sd) ? sd->status.job_level : 50) / 10);
	if( sd == nullptr || sd->status.party_id == 0 || flag&1 )
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,agi_lv, skill_get_time(getSkillId(),skill_lv)));
	else if( sd )
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillChainCrushCombo::SkillChainCrushCombo() : WeaponSkillImpl(CH_CHAINCRUSH) {
}

void SkillChainCrushCombo::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
#ifdef RENEWAL
	skillratio += -100 + 200 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += 300 + 100 * skill_lv;
#endif
	if (const status_change* sc = status_get_sc(src); sc != nullptr && sc->getSCE(SC_GT_ENERGYGAIN))
		skillratio += skillratio * 50 / 100;
}

SkillClearance::SkillClearance() : SkillImpl(AB_CLEARANCE) {
}

void SkillClearance::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );
	int32 i = 0;

	if( flag&1 || (i = skill_get_splash(getSkillId(), skill_lv)) < 1 ) { // As of the behavior in official server Clearance is just a super version of Dispell skill. [Jobbie]

		if( target->type != BL_MOB && battle_check_target(src,target,BCT_PARTY) <= 0 ) // Only affect mob or party.
			return;

		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);

		if(rnd()%100 >= 60 + 8 * skill_lv) {
			if (sd)
				clif_skill_fail( *sd, getSkillId() );
			return;
		}

		if(status_isimmune(target))
			return;

		//Remove bonus_script by Clearance
		if (dstsd)
			pc_bonus_script_clear(dstsd,BSF_REM_ON_CLEARANCE);

		if(tsc == nullptr || tsc->empty())
			return;

		//Statuses change that can't be removed by Cleareance
		for (const auto &it : status_db) {
			sc_type status = static_cast<sc_type>(it.first);

			if (!tsc->getSCE(status))
				continue;

			if (it.second->flag[SCF_NOCLEARANCE])
				continue;

			switch (status) {
				case SC_WHISTLE:		case SC_ASSNCROS:		case SC_POEMBRAGI:
				case SC_APPLEIDUN:		case SC_HUMMING:		case SC_DONTFORGETME:
				case SC_FORTUNE:		case SC_SERVICE4U:
					if (!battle_config.dispel_song || tsc->getSCE(status)->val4 == 0)
						continue; //If in song area don't end it, even if config enatargeted
					break;
				case SC_ASSUMPTIO:
					if (target->type == BL_MOB)
						continue;
					break;
			}
			if (status == SC_BERSERK || status == SC_SATURDAYNIGHTFEVER)
				tsc->getSCE(status)->val2 = 0; //Mark a dispelled berserk to avoid setting hp to 100 by setting hp penalty to 0.
			status_change_end(target,status);
		}
		return;
	}

	map_foreachinallrange(skill_area_sub, target, i, BL_CHAR, src, getSkillId(), skill_lv, tick, flag|1, skill_castend_damage_id);
}

SkillColuceoHeal::SkillColuceoHeal() : SkillImpl(AB_CHEAL) {
}

void SkillColuceoHeal::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( !sd || sd->status.party_id == 0 || flag&1 ) {
		if( sd && tstatus && !battle_check_undead(tstatus->race, tstatus->def_ele) && !tsc->getSCE(SC_BERSERK) ) {
			int32 partycount = (sd->status.party_id ? party_foreachsamemap(party_sub_count, sd, 0) : 0);

			int32 i = skill_calc_heal(src, target, AL_HEAL, pc_checkskill(sd, AL_HEAL), true);

			if( partycount > 1 )
				i += (i / 100) * (partycount * 10) / 4;
			if (status_isimmune(target))
				i = 0; // Should heal by 0 or won't do anything?? in iRO it breaks the healing to members.. [malufett]

			clif_skill_nodamage(src, *target, getSkillId(), i);
			if( tsc && tsc->getSCE(SC_AKAITSUKI) && i )
				i = ~i + 1;
			status_heal(target, i, 0, 0);
		}
	} else if( sd )
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillCompetentia::SkillCompetentia() : SkillImpl(CD_COMPETENTIA) {
}

void SkillCompetentia::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_data* tstatus = status_get_status_data(*target);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {
		int32 hp_amount = tstatus->max_hp * (20 * skill_lv) / 100;
		int32 sp_amount = tstatus->max_sp * (20 * skill_lv) / 100;

		clif_skill_nodamage(nullptr, *target, AL_HEAL, hp_amount);
		status_heal(target, hp_amount, 0, 0);

		clif_skill_nodamage(nullptr, *target, MG_SRECOVERY, sp_amount);
		status_heal(target, 0, sp_amount, 0);

		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	} else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillConvenio::SkillConvenio() : SkillImpl(AB_CONVENIO) {
}

void SkillConvenio::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd) {
		party_data *p = party_search(sd->status.party_id);
		int32 i = 0, count = 0;

		// Only usable in party
		if (p == nullptr) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}

		// Only usable as party leader.
		ARR_FIND(0, MAX_PARTY, i, p->data[i].sd == sd);
		if (i == MAX_PARTY || !p->party.member[i].leader) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}

		// Do the teleport part
		for (i = 0; i < MAX_PARTY; ++i) {
			map_session_data *pl_sd = p->data[i].sd;

			if (pl_sd == nullptr || pl_sd == sd || pl_sd->status.party_id != p->party.party_id || pc_isdead(pl_sd) ||
				sd->m != pl_sd->m)
				continue;

			// Respect /call configuration
			if( pl_sd->status.disable_call ){
				continue;
			}

			if (!(map_getmapflag(sd->m, MF_NOTELEPORT) || map_getmapflag(sd->m, MF_PVP) || map_getmapflag(sd->m, MF_BATTLEGROUND) || map_flag_gvg2(sd->m))) {
				pc_setpos(pl_sd, map_id2index(sd->m), sd->x, sd->y, CLR_TELEPORT);
				count++;
			}
		}
		if (!count)
			clif_skill_fail( *sd, getSkillId() );
	}
}

SkillCrementia::SkillCrementia() : SkillImpl(AB_CLEMENTIA) {
}

void SkillCrementia::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	int32 bless_lv = ((sd) ? pc_checkskill(sd,AL_BLESSING) : skill_get_max(AL_BLESSING)) + (((sd) ? sd->status.job_level : 50) / 10);
	if( sd == nullptr || sd->status.party_id == 0 || flag&1 )
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,bless_lv, skill_get_time(getSkillId(),skill_lv)));
	else if( sd )
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillCrucis::SkillCrucis() : SkillImpl(AL_CRUCIS)
{
}

void SkillCrucis::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	sc_type type = skill_get_sc(getSkillId());

	if (flag & 1)
		sc_start(src, bl, type, 25 + skill_lv * 4 + status_get_lv(src) - status_get_lv(bl), skill_lv, skill_get_time(getSkillId(), skill_lv));
	else
	{
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
	}
}

SkillCure::SkillCure() : SkillImpl(AL_CURE)
{
}

void SkillCure::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	if (status_isimmune(bl))
	{
		clif_skill_nodamage(src, *bl, getSkillId(), skill_lv, false);
		return;
	}
	status_change_end(bl, SC_SILENCE);
	status_change_end(bl, SC_BLIND);
	status_change_end(bl, SC_CONFUSION);
	status_change_end(bl, SC_BITESCAR);
	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
}

SkillCursedCircle::SkillCursedCircle() : SkillImpl(SR_CURSEDCIRCLE) {
}

void SkillCursedCircle::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	sc_type type = skill_get_sc(getSkillId());

	if (flag & 1) {
		if (status_get_class_(target) == CLASS_BOSS)
			return;
		if (sc_start2(src, target, type, 100, skill_lv, src->id, skill_get_time(getSkillId(), skill_lv))) {
			if (target->type == BL_MOB)
				mob_unlocktarget((TBL_MOB*)target, gettick());
			clif_bladestop(*src, target->id, true);
			flag |= SKILL_NOCONSUME_REQ;
		}
		return;
	}

	clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE);
	int32 count = map_forcountinrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), (sd) ? sd->spiritball_old : 15, // Assume 15 spiritballs in non-characters
		BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1, skill_castend_nodamage_id);
	if (sd)
		pc_delspiritball(sd, count, 0);
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv, sc_start2(src, src, SC_CURSEDCIRCLE_ATKER, 100, skill_lv, count, skill_get_time(getSkillId(), skill_lv)));
}

SkillDecreaseAgi::SkillDecreaseAgi() : SkillImpl(AL_DECAGI)
{
}

void SkillDecreaseAgi::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	sc_type type = skill_get_sc(getSkillId());
	status_data *sstatus = status_get_status_data(*src);

	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv, sc_start(src, bl, type, (50 + skill_lv * 3 + (status_get_lv(src) + sstatus->int_) / 5), skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillDilectioHeal::SkillDilectioHeal() : SkillImpl(CD_DILECTIO_HEAL) {
}

void SkillDilectioHeal::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1) {
		if (sd == nullptr || sd->status.party_id == 0 || (flag & 2)) {
			int32 heal_amount = skill_calc_heal(src, target, getSkillId(), skill_lv, 1);

			clif_skill_nodamage(nullptr, *target, AL_HEAL, heal_amount);
			status_heal(target, heal_amount, 0, 0);
		} else if (sd)
			party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 3, skill_castend_nodamage_id);
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv); // Placed here to display animation on target only.
		skill_castend_nodamage_id(target, target, getSkillId(), skill_lv, tick, 1);
	}
}

SkillDivinusFlos::SkillDivinusFlos() : SkillImplRecursiveDamageSplash(CD_DIVINUS_FLOS) {
}

void SkillDivinusFlos::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 4000 * skill_lv;
	skillratio += 70 * pc_checkskill(sd, CD_FIDUS_ANIMUS);
	skillratio += 10 * sstatus->spl;
	RE_LVL_DMOD(100);
}

void SkillDivinusFlos::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

void SkillDivinusFlos::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if (sc != nullptr && sc->hasSCE(SC_ANCILLA))
		element = ELE_NEUTRAL;
}

SkillDragonCombo::SkillDragonCombo() : WeaponSkillImpl(SR_DRAGONCOMBO) {
}

void SkillDragonCombo::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target, SC_STUN, 1 + skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillDragonCombo::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += 100 + 80 * skill_lv;
	RE_LVL_DMOD(100);
}

// AB_DUPLELIGHT_MAGIC
SkillDupleLightMagic::SkillDupleLightMagic() : SkillImpl(AB_DUPLELIGHT_MAGIC) {
}

void SkillDupleLightMagic::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 300 + 40 * skill_lv;
}

void SkillDupleLightMagic::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}


// AB_DUPLELIGHT_MELEE
SkillDupleLightMelee::SkillDupleLightMelee() : WeaponSkillImpl(AB_DUPLELIGHT_MELEE) {
}

void SkillDupleLightMelee::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 50 + 15 * skill_lv;
}

SkillEarthShaker::SkillEarthShaker() : WeaponSkillImpl(SR_EARTHSHAKER) {
}

void SkillEarthShaker::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if (dstmd != nullptr && dstmd->guardian_data == nullptr) // Target is a mob (boss included) and not a guardian type. [Atemo]
		sc_start(src, target, SC_EARTHSHAKER, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	sc_start(src,target,SC_STUN, 25 + 5 * skill_lv,skill_lv,skill_get_time(getSkillId(),skill_lv));
	status_change_end(target, SC_SV_ROOTTWIST);
}

void SkillEarthShaker::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *tsc = status_get_sc(target);

	if (tsc && ((tsc->option&(OPTION_HIDE|OPTION_CLOAK|OPTION_CHASEWALK)) || tsc->getSCE(SC_CAMOUFLAGE) || tsc->getSCE(SC_STEALTHFIELD) || tsc->getSCE(SC__SHADOWFORM))) {
		//[(Skill Level x 300) x (Caster Base Level / 100) + (Caster STR x 3)] %
		skillratio += -100 + 300 * skill_lv;
		RE_LVL_DMOD(100);
		skillratio += status_get_str(src) * 3;
	} else { //[(Skill Level x 400) x (Caster Base Level / 100) + (Caster STR x 2)] %
		skillratio += -100 + 400 * skill_lv;
		RE_LVL_DMOD(100);
		skillratio += status_get_str(src) * 2;
	}
}

void SkillEarthShaker::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if( flag&1 ) { //by default cloaking skills are remove by aoe skills so no more checking/removing except hiding and cloaking exceed.
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
		status_change_end(target, SC_CLOAKINGEXCEED);
		if (tsc && tsc->getSCE(SC__SHADOWFORM) && rnd() % 100 < 100 - tsc->getSCE(SC__SHADOWFORM)->val1 * 10) // [100 - (Skill Level x 10)] %
			status_change_end(target, SC__SHADOWFORM);
	} else {
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|SD_SPLASH|1, skill_castend_damage_id);
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	}
}

void SkillEarthShaker::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

SkillEffligo::SkillEffligo() : WeaponSkillImpl(CD_EFFLIGO) {
}

void SkillEffligo::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillEffligo::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 1800 * skill_lv;
	skillratio += 7 * sstatus->pow;
	skillratio += 8 * pc_checkskill(sd, CD_MACE_BOOK_M);

	if (tstatus->race == RC_UNDEAD || tstatus->race == RC_DEMON) {
		skillratio += 200 * skill_lv;
		skillratio += 7 * pc_checkskill(sd, CD_MACE_BOOK_M);
	}

	RE_LVL_DMOD(100);
}

SkillEpiclesis::SkillEpiclesis() : SkillImpl(AB_EPICLESIS) {
}

void SkillEpiclesis::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	std::shared_ptr<s_skill_unit_group> sg;

	if( (sg = skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0)) ) {
		int32 i = skill_get_splash(getSkillId(), skill_lv);
		map_foreachinallarea(skill_area_sub, src->m, x - i, y - i, x + i, y + i, BL_CHAR, src, ALL_RESURRECTION, 1, tick, flag|BCT_NOENEMY|1,skill_castend_nodamage_id);
	}
}

SkillExplosionBlaster::SkillExplosionBlaster() : SkillImplRecursiveDamageSplash(IQ_EXPOSION_BLASTER) {
}

void SkillExplosionBlaster::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillExplosionBlaster::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change* tsc = status_get_sc(target);

	skillratio += -100 + 450 + 2600 * skill_lv;
	skillratio += 10 * sstatus->pow;

	if (tsc != nullptr && tsc->getSCE(SC_HOLY_OIL)) {
		skillratio += 950 * skill_lv;
	}

	RE_LVL_DMOD(100);
}

SkillFallenEmpire::SkillFallenEmpire() : WeaponSkillImpl(SR_FALLENEMPIRE) {
}

void SkillFallenEmpire::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	// ATK [(Skill Level x 300 + 100) x Caster Base Level / 150] %
	skillratio += 300 * skill_lv;
	RE_LVL_DMOD(150);
}

SkillFirstBrand::SkillFirstBrand() : SkillImplRecursiveDamageSplash(IQ_FIRST_BRAND) {
}

void SkillFirstBrand::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1200 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillFirstBrand::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_FIRST_BRAND, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillFirstBrand::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillFlashCombo::SkillFlashCombo() : SkillImpl(SR_FLASHCOMBO) {
}

void SkillFlashCombo::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	const int32 combo[] = { SR_DRAGONCOMBO, SR_FALLENEMPIRE, SR_TIGERCANNON };
	const int32 delay[] = { 0, 750, 1250 };

	if (sd) // Disable attacking/acting/moving for skill's duration.
		sd->ud.attackabletime = sd->canuseitem_tick = sd->ud.canact_tick = tick + delay[2];

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv,
		sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));

	for (int32 i = 0; i < ARRAYLENGTH(combo); i++)
		skill_addtimerskill(src,tick + delay[i],target->id,0,0,combo[i],skill_lv,BF_WEAPON,flag|SD_LEVEL);
}

SkillFramen::SkillFramen() : SkillImplRecursiveDamageSplash(CD_FRAMEN) {
}

void SkillFramen::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 1550 * skill_lv;
	skillratio += 5 * pc_checkskill(sd, CD_FIDUS_ANIMUS) * skill_lv;
	skillratio += 5 * sstatus->spl;
	if (tstatus->race == RC_UNDEAD || tstatus->race == RC_DEMON)
		skillratio += 50 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillFramen::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillGateOfHell::SkillGateOfHell() : WeaponSkillImpl(SR_GATEOFHELL) {
}

void SkillGateOfHell::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);

	if (sc && sc->getSCE(SC_COMBO) && sc->getSCE(SC_COMBO)->val1 == SR_FALLENEMPIRE)
		skillratio += -100 + 800 * skill_lv;
	else
		skillratio += -100 + 500 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc->getSCE(SC_GT_REVITALIZE))
		skillratio += skillratio * 30 / 100;
}

SkillGentleTouchCure::SkillGentleTouchCure() : SkillImpl(SR_GENTLETOUCH_CURE) {
}

void SkillGentleTouchCure::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_change *tsc = status_get_sc(target);

	uint32 heal;

	if (dstmd && (dstmd->mob_id == MOBID_EMPERIUM || status_get_class_(target) == CLASS_BATTLEFIELD))
		heal = 0;
	else {
		heal = (120 * skill_lv) + (status_get_max_hp(target) * skill_lv / 100);
		status_heal(target, heal, 0, 0);
	}

	if( tsc != nullptr && !tsc->empty() && rnd_chance( ( skill_lv * 5 + ( status_get_dex( src ) + status_get_lv( src ) ) / 4 ) - rnd_value( 1, 10 ), 100 ) ){
		status_change_end(target, SC_STONE);
		status_change_end(target, SC_FREEZE);
		status_change_end(target, SC_STUN);
		status_change_end(target, SC_POISON);
		status_change_end(target, SC_SILENCE);
		status_change_end(target, SC_BLIND);
		status_change_end(target, SC_HALLUCINATION);
	}

	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillGentleTouchQuiet::SkillGentleTouchQuiet() : WeaponSkillImpl(SR_GENTLETOUCH_QUIET) {
}

void SkillGentleTouchQuiet::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// [(Skill Level x 5) + (Caster?s DEX + Caster?s Base Level) / 10]
	sc_start(src,target, SC_SILENCE, 5 * skill_lv + (status_get_dex(src) + status_get_lv(src)) / 10, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillGentleTouchQuiet::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 100 * skill_lv + sstatus->dex;
	RE_LVL_DMOD(100);
}

SkillGlacierFist::SkillGlacierFist() : WeaponSkillImpl(CH_TIGERFIST) {
}

void SkillGlacierFist::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
#ifdef RENEWAL
	skillratio += 400 + 150 * skill_lv;
	RE_LVL_DMOD(100);
#else
	skillratio += -60 + 100 * skill_lv;
#endif
	if (const status_change* sc = status_get_sc(src); sc != nullptr && sc->getSCE(SC_GT_ENERGYGAIN))
		skillratio += skillratio * 50 / 100;
}

void SkillGlacierFist::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	t_tick basetime = skill_get_time(getSkillId(), skill_lv);
	t_tick mintime = 15 * (status_get_lv(src) + 100);

	if (status_bl_has_mode(target, MD_STATUSIMMUNE))
		basetime /= 5;
	basetime = std::max((basetime * status_get_agi(target)) / -200 + basetime, mintime);
	sc_start(src, target, SC_ANKLE, (1 + skill_lv) * 10, 0, basetime);
}

SkillGloria::SkillGloria() : SkillImpl(PR_GLORIA) {
}

void SkillGloria::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {

		// Animations don't play when outside visible range
		if (check_distance_bl(src, target, AREA_SIZE))
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv);

		sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
}

SkillHeal::SkillHeal() : SkillImpl(AL_HEAL)
{
}

void SkillHeal::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	status_change *tsc = status_get_sc(bl);
	map_session_data *sd = BL_CAST(BL_PC, src);
	map_session_data *dstsd = nullptr;
	status_data* sstatus = status_get_status_data(*src);
	mob_data *dstmd = BL_CAST(BL_MOB, bl);

	int32 heal = skill_calc_heal(src, bl, getSkillId(), skill_lv, true);

	if (status_isimmune(bl) || (dstmd && (status_get_class(bl) == MOBID_EMPERIUM || status_get_class_(bl) == CLASS_BATTLEFIELD)))
		heal = 0;

	if (tsc != nullptr && !tsc->empty())
	{
		if (tsc->getSCE(SC_KAITE) && !status_has_mode(sstatus, MD_STATUSIMMUNE))
		{ // Bounce back heal
			if (--tsc->getSCE(SC_KAITE)->val2 <= 0)
				status_change_end(bl, SC_KAITE);
			if (src == bl)
				heal = 0; // When you try to heal yourself under Kaite, the heal is voided.
			else
			{
				bl = src;
				dstsd = sd;
			}
		}
		else if (tsc->getSCE(SC_BERSERK) || tsc->getSCE(SC_SATURDAYNIGHTFEVER))
		{
			heal = 0; // Needed so that it actually displays 0 when healing.
		}
	}

	status_change_end(bl, SC_BITESCAR);
	clif_skill_nodamage(src, *bl, getSkillId(), heal);
	if (tsc && tsc->getSCE(SC_AKAITSUKI) && heal)
		heal = ~heal + 1;
	t_exp heal_get_jobexp = status_heal(bl, heal, 0, 0);

	if (sd && dstsd && heal > 0 && sd != dstsd && battle_config.heal_exp > 0)
	{
		heal_get_jobexp = heal_get_jobexp * battle_config.heal_exp / 100;
		if (heal_get_jobexp <= 0)
			heal_get_jobexp = 1;
		pc_gainexp(sd, bl, 0, heal_get_jobexp, 0);
	}
}

void SkillHeal::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const
{
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

SkillHighnessHeal::SkillHighnessHeal() : SkillImpl(AB_HIGHNESSHEAL) {
}

void SkillHighnessHeal::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillHighnessHeal::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	status_data* sstatus = status_get_status_data(*src);
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST( BL_PC, src );
	map_session_data* dstsd = BL_CAST( BL_PC, target );

	int32 heal = skill_calc_heal(src, target, getSkillId(), skill_lv, true);

	if (status_isimmune(target) || (dstmd && (status_get_class(target) == MOBID_EMPERIUM || status_get_class_(target) == CLASS_BATTLEFIELD)))
		heal = 0;

	if( tsc != nullptr && !tsc->empty() ) {
		if( tsc->getSCE(SC_KAITE) && !status_has_mode(sstatus,MD_STATUSIMMUNE) ) { //Bounce back heal
			if (--tsc->getSCE(SC_KAITE)->val2 <= 0)
				status_change_end(target, SC_KAITE);
			if (src == target)
				heal=0; //When you try to heal yourself under Kaite, the heal is voided.
			else {
				target = src;
				dstsd = sd;
			}
		}
		else if (tsc->getSCE(SC_BERSERK) || tsc->getSCE(SC_SATURDAYNIGHTFEVER))
			heal = 0; //Needed so that it actually displays 0 when healing.
	}
	clif_skill_nodamage(src, *target, getSkillId(), heal);
	if( tsc && tsc->getSCE(SC_AKAITSUKI) && heal )
		heal = ~heal + 1;
	t_exp heal_get_jobexp = status_heal(target,heal,0,0);

	if(sd && dstsd && heal > 0 && sd != dstsd && battle_config.heal_exp > 0){
		heal_get_jobexp = heal_get_jobexp * battle_config.heal_exp / 100;
		if (heal_get_jobexp <= 0)
			heal_get_jobexp = 1;
		pc_gainexp (sd, target, 0, heal_get_jobexp, 0);
	}
}

SkillHolyLight::SkillHolyLight() : SkillImpl(AL_HOLYLIGHT) {
}

void SkillHolyLight::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_P_ALTER);
	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillHolyLight::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += 25;
	if (sd && sd->sc.getSCE(SC_SPIRIT) && sd->sc.getSCE(SC_SPIRIT)->val2 == SL_PRIEST)
		base_skillratio *= 5; //Does 5x damage include bonuses from other skills?
}

SkillHolyWater::SkillHolyWater() : SkillImpl(AL_HOLYWATER)
{
}

void SkillHolyWater::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd)
	{
		if (skill_produce_mix(sd, getSkillId(), ITEMID_HOLY_WATER, 0, 0, 0, 1, -1))
		{
			if (skill_unit* su = map_find_skill_unit_oncell(bl, bl->x, bl->y, NJ_SUITON, nullptr, 0); su != nullptr)
				skill_delunit(su);
			clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
		}
		else
			clif_skill_fail(*sd, getSkillId());
	}
}

SkillHowlingOfLion::SkillHowlingOfLion() : SkillImplRecursiveDamageSplash(SR_HOWLINGOFLION) {
}

void SkillHowlingOfLion::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 500 * skill_lv;
	RE_LVL_DMOD(100);
}

int64 SkillHowlingOfLion::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	status_change_end(target, SC_SWINGDANCE);
	status_change_end(target, SC_SYMPHONYOFLOVER);
	status_change_end(target, SC_MOONLITSERENADE);
	status_change_end(target, SC_RUSHWINDMILL);
	status_change_end(target, SC_ECHOSONG);
	status_change_end(target, SC_HARMONIZE);
	status_change_end(target, SC_NETHERWORLD);
	status_change_end(target, SC_VOICEOFSIREN);
	status_change_end(target, SC_DEEPSLEEP);
	status_change_end(target, SC_SIRCLEOFNATURE);
	status_change_end(target, SC_GLOOMYDAY);
	status_change_end(target, SC_GLOOMYDAY_SK);
	status_change_end(target, SC_SONGOFMANA);
	status_change_end(target, SC_DANCEWITHWUG);
	status_change_end(target, SC_SATURDAYNIGHTFEVER);
	status_change_end(target, SC_LERADSDEW);
	status_change_end(target, SC_MELODYOFSINK);
	status_change_end(target, SC_BEYONDOFWARCRY);
	status_change_end(target, SC_UNLIMITEDHUMMINGVOICE);

	int32 sflag = flag|SD_ANIMATION;
	return SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, sflag);
}

void SkillHowlingOfLion::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

int32 SkillHowlingOfLion::getSplashTarget(block_list* src) const {
	return splash_target(src);
}

#ifdef RENEWAL
SkillImpositioManus::SkillImpositioManus() : SkillImpl(PR_IMPOSITIO) {
}

void SkillImpositioManus::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {

		// Animations don't play when outside visible range
		if (check_distance_bl(src, target, AREA_SIZE))
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv);

		sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
}
#else
SkillImpositioManus::SkillImpositioManus() : StatusSkillImpl(PR_IMPOSITIO) {
}
#endif

SkillIncreaseAgi::SkillIncreaseAgi() : SkillImpl(AL_INCAGI)
{
}

void SkillIncreaseAgi::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	map_session_data *dstsd = BL_CAST(BL_PC, bl);
	status_change *tsc = status_get_sc(bl);
	enum sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);
	if (dstsd != nullptr && tsc && tsc->getSCE(SC_CHANGEUNDEAD))
	{
		status_data *tstatus = status_get_status_data(*bl);
		if (tstatus->hp > 1)
		{
			skill_attack(BF_MISC, src, src, bl, getSkillId(), skill_lv, tick, flag);
		}
		return;
	}
	sc_start(src, bl, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillJudex::SkillJudex() : SkillImplRecursiveDamageSplash(AB_JUDEX) {
}

void SkillJudex::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	skillratio += -100 + 300 + 70 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillKiExplosion::SkillKiExplosion() : SkillImpl(MO_BALKYOUNG) {
}

void SkillKiExplosion::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.blewcount = 0;
}

void SkillKiExplosion::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Passive part of the attack. Splash knock-back+stun. [Skotlex]
	if (skill_area_temp[1] != target->id) {
		skill_blown(src,target,skill_get_blewcount(getSkillId(),skill_lv),-1,BLOWN_NONE);
		skill_additional_effect(src,target,getSkillId(),skill_lv,BF_MISC,ATK_DEF,tick); //Use Misc rather than weapon to signal passive pushback
	}
}

void SkillKiExplosion::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Active part of the attack. Skill-attack [Skotlex]
	skill_area_temp[1] = target->id; //NOTE: This is used in skill_castend_nodamage_id to avoid affecting the target.
	if (skill_attack(BF_WEAPON,src,src,target,getSkillId(),skill_lv,tick,flag))
		map_foreachinallrange(skill_area_sub,target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR,
			src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,
			skill_castend_nodamage_id);
}

void SkillKiExplosion::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 700;
#else
	base_skillratio += 200;
#endif
}

void SkillKiExplosion::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	//Note: attack_type is passed as BF_WEAPON for the actual target, BF_MISC for the splash-affected mobs.
	if(attack_type&BF_MISC) //70% base stun chance...
		sc_start(src,target,SC_STUN,70,skill_lv,skill_get_time2(getSkillId(),skill_lv));
}

SkillKiTranslation::SkillKiTranslation() : SkillImpl(MO_KITRANSLATION) {
}

void SkillKiTranslation::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if(dstsd && ((dstsd->class_&MAPID_FIRSTMASK) != MAPID_GUNSLINGER && (dstsd->class_&MAPID_SECONDMASK) != MAPID_REBELLION) && dstsd->spiritball < 5) {
		//Require will define how many spiritballs will be transferred
		struct s_skill_condition require;
		require = skill_get_requirement(sd,getSkillId(),skill_lv);
		pc_delspiritball(sd,require.spiritball,0);
		for (int32 i = 0; i < require.spiritball; i++)
			pc_addspiritball(dstsd,skill_get_time(getSkillId(),skill_lv),5);
	} else {
		if(sd)
			clif_skill_fail( *sd, getSkillId() );
		flag |= SKILL_NOCONSUME_REQ;
	}
}

SkillKnuckleArrow::SkillKnuckleArrow() : SkillImpl(SR_KNUCKLEARROW) {
}

void SkillKnuckleArrow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_change *sc = status_get_sc(src);
	const map_session_data* tsd = BL_CAST(BL_PC, target);

	if (wd->miscflag&4) { // ATK [(Skill Level x 150) + (1000 x Target current weight / Maximum weight) + (Target Base Level x 5) x (Caster Base Level / 150)] %
		skillratio += -100 + 150 * skill_lv + status_get_lv(target) * 5;
		if (tsd && tsd->weight)
			skillratio += pc_getpercentweight(*tsd);
		RE_LVL_DMOD(150);
	} else {
		if (status_get_class_(target) == CLASS_BOSS)
			skillratio += 400 + 200 * skill_lv;
		else // ATK [(Skill Level x 100 + 500) x Caster Base Level / 100] %
			skillratio += 400 + 100 * skill_lv;
		RE_LVL_DMOD(100);
	}
	if (sc != nullptr && sc->hasSCE(SC_GT_CHANGE))
		skillratio += skillratio * 30 / 100;
}

void SkillKnuckleArrow::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Holds current direction of bl/target to src/attacker before the src is moved to bl location
	dir_ka = map_calc_dir(target, src->x, src->y);
	// Has slide effect
	if (skill_check_unit_movepos(5, src, target->x, target->y, 1, 1))
		skill_blown(src, src, 1, (dir_ka + 4) % 8, BLOWN_NONE); // Target position is actually one cell next to the target
	skill_addtimerskill(src, tick + 300, target->id, 0, 0, getSkillId(), skill_lv, BF_WEAPON, flag|SD_LEVEL|2);
}

SkillKyrieEleison::SkillKyrieEleison() : SkillImpl(PR_KYRIE) {
}

void SkillKyrieEleison::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(target,*target,getSkillId(), skill_lv,
			sc_start(src,target,skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
}

SkillLaudaAgnus::SkillLaudaAgnus() : SkillImpl(AB_LAUDAAGNUS) {
}

void SkillLaudaAgnus::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( flag&1 || !sd || !sd->status.party_id ) {
		if( tsc && (tsc->getSCE(SC_FREEZE) || tsc->getSCE(SC_STONE) || tsc->getSCE(SC_BLIND) ||
			tsc->getSCE(SC_BURNING) || tsc->getSCE(SC_FREEZING) || tsc->getSCE(SC_CRYSTALIZE))) {
			// Success Chance: (60 + 10 * Skill Level) %
			if( rnd()%100 > 60+10*skill_lv ) return;
			status_change_end(target, SC_FREEZE);
			status_change_end(target, SC_STONE);
			status_change_end(target, SC_BLIND);
			status_change_end(target, SC_BURNING);
			status_change_end(target, SC_FREEZING);
			status_change_end(target, SC_CRYSTALIZE);
		} else //Success rate only applies to the curing effect and not stat bonus. Bonus status only applies to non infected targets
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv,
				sc_start(src,target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	} else if( sd )
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv),
			src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillLaudaRamus::SkillLaudaRamus() : SkillImpl(AB_LAUDARAMUS) {
}

void SkillLaudaRamus::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( flag&1 || !sd || !sd->status.party_id ) {
		if( tsc && (tsc->getSCE(SC_SLEEP) || tsc->getSCE(SC_STUN) || tsc->getSCE(SC_MANDRAGORA) || tsc->getSCE(SC_SILENCE) || tsc->getSCE(SC_DEEPSLEEP)) ){
			// Success Chance: (60 + 10 * Skill Level) %
			if( rnd()%100 > 60+10*skill_lv )  return;
			status_change_end(target, SC_SLEEP);
			status_change_end(target, SC_STUN);
			status_change_end(target, SC_MANDRAGORA);
			status_change_end(target, SC_SILENCE);
			status_change_end(target, SC_DEEPSLEEP);
		} else // Success rate only applies to the curing effect and not stat bonus. Bonus status only applies to non infected targets
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv,
				sc_start(src,target, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv)));
	} else if( sd )
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv),
			src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillLexDivina::SkillLexDivina() : SkillImpl(PR_LEXDIVINA) {
}

void SkillLexDivina::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	status_change* tsc = status_get_sc(target);
	status_change_entry* tsce = (tsc && type != SC_NONE) ? tsc->getSCE(type) : nullptr;

	if (tsce)
		status_change_end(target, type);
	else
		skill_addtimerskill(src, tick+1000, target->id, 0, 0, getSkillId(), skill_lv, 100, flag);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillMagnificat::SkillMagnificat() : SkillImpl(PR_MAGNIFICAT) {
}

void SkillMagnificat::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {

		// Animations don't play when outside visible range
		if (check_distance_bl(src, target, AREA_SIZE))
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv);

		sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
}

SkillMagnusExorcismus::SkillMagnusExorcismus() : SkillImpl(PR_MAGNUS) {
}

void SkillMagnusExorcismus::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillMagnusExorcismus::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_data* tstatus = status_get_status_data(*target);

	if (battle_check_undead(tstatus->race, tstatus->def_ele) || tstatus->race == RC_DEMON)
		base_skillratio += 30;
}

SkillMassiveFlameBlaster::SkillMassiveFlameBlaster() : SkillImplRecursiveDamageSplash(IQ_MASSIVE_F_BLASTER) {
}

void SkillMassiveFlameBlaster::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillMassiveFlameBlaster::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 2500 * skill_lv;
	skillratio += 15 * sstatus->pow;
	if (tstatus->race == RC_BRUTE || tstatus->race == RC_DEMON)
		skillratio += 150 * skill_lv;
	RE_LVL_DMOD(100);
}

SkillMedialeVotum::SkillMedialeVotum() : StatusSkillImpl(CD_MEDIALE_VOTUM) {
}

void SkillMedialeVotum::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1) {
		if (sd == nullptr || sd->status.party_id == 0 || (flag & 2)) {
			int32 heal_amount = skill_calc_heal(src, target, getSkillId(), skill_lv, 1);

			clif_skill_nodamage(nullptr, *target, AL_HEAL, heal_amount);
			status_heal(target, heal_amount, 0, 0);
		} else if (sd)
			party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 3, skill_castend_nodamage_id);
	} else {
		StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
	}
}

SkillOccultImpaction::SkillOccultImpaction() : WeaponSkillImpl(MO_INVESTIGATE) {
}

void SkillOccultImpaction::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	status_change_end(src, SC_BLADESTOP);
}

void SkillOccultImpaction::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_change* tsc = status_get_sc(target);

	base_skillratio += -100 + 100 * skill_lv;
	if (tsc && tsc->getSCE(SC_BLADESTOP))
		base_skillratio += base_skillratio / 2;
#else
	base_skillratio += 75 * skill_lv;
#endif
}

SkillOleumSanctum::SkillOleumSanctum() : SkillImplRecursiveDamageSplash(IQ_OLEUM_SANCTUM) {
}

void SkillOleumSanctum::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillOleumSanctum::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 500 + 2000 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillOleumSanctum::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_HOLY_OIL, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillOratio::SkillOratio() : SkillImpl(AB_ORATIO) {
}

void SkillOratio::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	if( flag&1 )
		sc_start(src,target, type, 40 + 5 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
	else {
		map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR,
			src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillPetitio::SkillPetitio() : SkillImplRecursiveDamageSplash(CD_PETITIO) {
}

void SkillPetitio::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1375 * skill_lv;
	skillratio += pc_checkskill(sd, CD_MACE_BOOK_M) * 50 * skill_lv;
	skillratio += 5 * sstatus->pow;

	RE_LVL_DMOD(100);
}

void SkillPetitio::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillPneuma::SkillPneuma() : SkillImpl(AL_PNEUMA) {
}

void SkillPneuma::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;

	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillPneumaticusProcella::SkillPneumaticusProcella() : SkillImpl(CD_PNEUMATICUS_PROCELLA) {
}

void SkillPneumaticusProcella::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag|=1;

	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

void SkillPneumaticusProcella::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);
	const status_data* sstatus = status_get_status_data(*src);
	const status_data* tstatus = status_get_status_data(*target);

	skillratio += -100 + 150 + 2100 * skill_lv + 10 * sstatus->spl;
	skillratio += 3 * pc_checkskill( sd, CD_FIDUS_ANIMUS );
	if (tstatus->race == RC_UNDEAD || tstatus->race == RC_DEMON) {
		skillratio += 50 + 150 * skill_lv;
		skillratio += 2 * pc_checkskill( sd, CD_FIDUS_ANIMUS );
	}
	RE_LVL_DMOD(100);
}

SkillPowerVelocity::SkillPowerVelocity() : SkillImpl(SR_POWERVELOCITY) {
}

void SkillPowerVelocity::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (!dstsd)
		return;

	if (sd && dstsd->spiritball <= 5) {
		for (int32 i = 0; i <= 5; i++) {
			pc_addspiritball(dstsd, skill_get_time(MO_CALLSPIRITS, pc_checkskill(sd, MO_CALLSPIRITS)), i);
			pc_delspiritball(sd, sd->spiritball, 0);
		}
	}
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillPraefatio::SkillPraefatio() : SkillImpl(AB_PRAEFATIO) {
}

void SkillPraefatio::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( !sd || sd->status.party_id == 0 || flag&1 ) {
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start4(src, target, type, 100, skill_lv, 0, 0, (sd && sd->status.party_id ? party_foreachsamemap(party_sub_count, sd, 0) : 1 ), skill_get_time(getSkillId(), skill_lv)));
	} else if( sd )
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillRagingPalmStrike::SkillRagingPalmStrike() : SkillImpl(CH_PALMSTRIKE) {
}

void SkillRagingPalmStrike::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	//	Palm Strike takes effect 1sec after casting. [Skotlex]
	// clif_skill_nodamage(src,*target,getSkillId(),skill_lv,false); //Can't make this one display the correct attack animation delay :/
	clif_damage(*src, *target, tick, status_get_amotion(src), 0, -1, 1, DMG_ENDURE, 0, false); //Display an absorbed damage attack.
	skill_addtimerskill(src, tick + (1000 + status_get_amotion(src)), target->id, 0, 0, getSkillId(), skill_lv, BF_WEAPON, flag);
}

void SkillRagingPalmStrike::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += 100 + 100 * skill_lv + sstatus->str; // !TODO: How does STR play a role?
	RE_LVL_DMOD(100);
#else
	skillratio += 100 + 100 * skill_lv;
#endif
}

SkillRagingQuadrupleBlow::SkillRagingQuadrupleBlow() : WeaponSkillImpl(MO_CHAINCOMBO) {
}

void SkillRagingQuadrupleBlow::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->status.weapon == W_KNUCKLE)
		dmg.div_ = -6;
#endif
}

void SkillRagingQuadrupleBlow::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	status_change_end(src, SC_BLADESTOP);
}

void SkillRagingQuadrupleBlow::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST(BL_PC, src);

	base_skillratio += 150 + 50 * skill_lv;
	if (sd && sd->status.weapon == W_KNUCKLE)
		base_skillratio *= 2;
#else
	base_skillratio += 50 + 50 * skill_lv;
#endif
}

SkillRagingThrust::SkillRagingThrust() : WeaponSkillImpl(MO_COMBOFINISH) {
}

void SkillRagingThrust::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* sc = status_get_sc(src);

	if (!(flag&1) && sc && sc->getSCE(SC_SPIRIT) && sc->getSCE(SC_SPIRIT)->val2 == SL_MONK)
	{	//Becomes a splash attack when Soul Linked.
		map_foreachinshootrange(skill_area_sub, target,
			skill_get_splash(getSkillId(), skill_lv),BL_CHAR|BL_SKILL,
			src,getSkillId(),skill_lv,tick, flag|BCT_ENEMY|1,
			skill_castend_damage_id);
	} else
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillRagingThrust::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += 450 + 50 * skill_lv + sstatus->str; // !TODO: How does STR play a role?
#else
	base_skillratio += 140 + 60 * skill_lv;
#endif

	if (const status_change* sc = status_get_sc(src); sc != nullptr && sc->getSCE(SC_GT_ENERGYGAIN))
		base_skillratio += base_skillratio * 50 / 100;
}

SkillRagingTrifectaBlow::SkillRagingTrifectaBlow() : WeaponSkillImpl(MO_TRIPLEATTACK) {
}

void SkillRagingTrifectaBlow::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 sflag = flag|SD_ANIMATION;

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, sflag);
}

void SkillRagingTrifectaBlow::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 20 * skill_lv;
}

SkillRaisingDragon::SkillRaisingDragon() : StatusSkillImpl(SR_RAISINGDRAGON) {
}

void SkillRaisingDragon::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ) {
		int16 max = 5 + skill_lv;
		sc_start(src,target, SC_EXPLOSIONSPIRITS, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
		for( int16 i = 0; i < max; i++ ) // Don't call more than max available spheres.
			pc_addspiritball(sd, skill_get_time(getSkillId(), skill_lv), max);

		StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
	}
}

SkillRampageBlaster::SkillRampageBlaster() : SkillImplRecursiveDamageSplash(SR_RAMPAGEBLASTER) {
}

void SkillRampageBlaster::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);
	const status_change* tsc = status_get_sc(target);

	if (tsc && tsc->getSCE(SC_EARTHSHAKER)) {
		skillratio += 1400 + 550 * skill_lv;
		RE_LVL_DMOD(120);
	} else {
		skillratio += 900 + 350 * skill_lv;
		RE_LVL_DMOD(150);
	}

	if (sc != nullptr && sc->hasSCE(SC_GT_CHANGE))
		skillratio += skillratio * 30 / 100;
}

void SkillRampageBlaster::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);
}

SkillRedemptio::SkillRedemptio() : SkillImpl(PR_REDEMPTIO) {
}

void SkillRedemptio::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);
	status_change* tsc = status_get_sc(target);

	if (sd && !(flag&1)) {
		if (sd->status.party_id == 0) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		skill_area_temp[0] = 0;
		party_foreachsamemap(skill_area_sub,
			sd,skill_get_splash(getSkillId(), skill_lv),
			src,getSkillId(),skill_lv,tick, flag|BCT_PARTY|1,
			skill_castend_nodamage_id);
		if (skill_area_temp[0] == 0) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
#ifndef RENEWAL
		skill_area_temp[0] = battle_config.exp_cost_redemptio_limit - skill_area_temp[0]; // The actual penalty...
		if (skill_area_temp[0] > 0 && !map_getmapflag(src->m, MF_NOEXPPENALTY) && battle_config.exp_cost_redemptio) { //Apply penalty
			//If total penalty is 1% => reduced 0.2% penalty per each revived player
			pc_lostexp(sd, u64min(sd->status.base_exp, (pc_nextbaseexp(sd) * skill_area_temp[0] * battle_config.exp_cost_redemptio / battle_config.exp_cost_redemptio_limit) / 100), 0);
		}
		status_set_sp(src, 0, 0);
#endif
		status_set_hp(src, 1, 0);
		return;
	} else if (!(status_isdead(*target) && flag&1)) { 
		//Invalid target, skip resurrection.
		return;
	}
	//Revive
	skill_area_temp[0]++; //Count it in, then fall-through to the Resurrection code.
	skill_lv = 3; //Resurrection level 3 is used

	if(sd && (map_flag_gvg2(target->m) || map_getmapflag(target->m, MF_BATTLEGROUND)))
	{	//No reviving in WoE grounds!
		clif_skill_fail( *sd, getSkillId() );
		return;
	}
	if (!status_isdead(*target))
		return;

	int32 per = 0, sper = 0;
	if (tsc && tsc->getSCE(SC_HELLPOWER)) {
		clif_skill_nodamage(src, *target, ALL_RESURRECTION, skill_lv);
		return;
	}

	if (map_getmapflag(target->m, MF_PVP) && dstsd && dstsd->pvp_point < 0)
		return;

	switch(skill_lv){
	case 1: per=10; break;
	case 2: per=30; break;
	case 3: per=50; break;
	case 4: per=80; break;
	}
	if(dstsd && dstsd->special_state.restart_full_recover)
		per = sper = 100;
	if (status_revive(target, per, sper))
	{
		clif_skill_nodamage(src,*target,ALL_RESURRECTION,skill_lv); //Both Redemptio and Res show this skill-animation.
		if(sd && dstsd && battle_config.resurrection_exp > 0)
		{
			t_exp exp = 0,jexp = 0;
			int32 lv = dstsd->status.base_level - sd->status.base_level, jlv = dstsd->status.job_level - sd->status.job_level;
			if(lv > 0 && pc_nextbaseexp(dstsd)) {
				exp = (t_exp)(dstsd->status.base_exp * lv * battle_config.resurrection_exp / 1000000.);
				if (exp < 1) exp = 1;
			}
			if(jlv > 0 && pc_nextjobexp(dstsd)) {
				jexp = (t_exp)(dstsd->status.job_exp * lv * battle_config.resurrection_exp / 1000000.);
				if (jexp < 1) jexp = 1;
			}
			if(exp > 0 || jexp > 0)
				pc_gainexp (sd, target, exp, jexp, 0);
		}
	}
}

SkillRenovatio::SkillRenovatio() : StatusSkillImpl(AB_RENOVATIO) {
}

void SkillRenovatio::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if( !sd || sd->status.party_id == 0 || flag&1 ) {
		StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
	} else if( sd )
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
}

SkillReparatio::SkillReparatio() : SkillImpl(CD_REPARATIO) {
}

void SkillReparatio::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_data* tstatus = status_get_status_data(*target);

	if (target->type != BL_PC) { // Only works on players.
		if (sd)
			clif_skill_fail( *sd, getSkillId() );
		return;
	}

	int32 heal_amount = 0;

	if (!status_isimmune(target))
		heal_amount = tstatus->max_hp;

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	clif_skill_nodamage(nullptr, *target, AL_HEAL, heal_amount);
	status_heal(target, heal_amount, 0, 0);
}

SkillResurrection::SkillResurrection() : SkillImpl(ALL_RESURRECTION) {
}

void SkillResurrection::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	if (!battle_check_undead(tstatus->race, tstatus->def_ele))
		return;
	skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
}

void SkillResurrection::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if(map_flag_gvg2(target->m) || map_getmapflag(target->m, MF_BATTLEGROUND))
	{	//No reviving in WoE grounds!
		if( sd != nullptr ){
			clif_skill_fail( *sd, getSkillId() );
		}
		return;
	}
	if (!status_isdead(*target))
		return;

	int32 per = 0, sper = 0;
	if (tsc && tsc->getSCE(SC_HELLPOWER)) {
		clif_skill_nodamage(src, *target, ALL_RESURRECTION, skill_lv);
		return;
	}

	if (map_getmapflag(target->m, MF_PVP) && dstsd && dstsd->pvp_point < 0)
		return;

	switch(skill_lv){
	case 1: per=10; break;
	case 2: per=30; break;
	case 3: per=50; break;
	case 4: per=80; break;
	}
	if(dstsd && dstsd->special_state.restart_full_recover)
		per = sper = 100;
	if (status_revive(target, per, sper))
	{
		clif_skill_nodamage(src,*target,ALL_RESURRECTION,skill_lv); //Both Redemptio and Res show this skill-animation.
		if(sd && dstsd && battle_config.resurrection_exp > 0)
		{
			t_exp exp = 0,jexp = 0;
			int32 lv = dstsd->status.base_level - sd->status.base_level, jlv = dstsd->status.job_level - sd->status.job_level;
			if(lv > 0 && pc_nextbaseexp(dstsd)) {
				exp = (t_exp)(dstsd->status.base_exp * lv * battle_config.resurrection_exp / 1000000.);
				if (exp < 1) exp = 1;
			}
			if(jlv > 0 && pc_nextjobexp(dstsd)) {
				jexp = (t_exp)(dstsd->status.job_exp * lv * battle_config.resurrection_exp / 1000000.);
				if (jexp < 1) jexp = 1;
			}
			if(exp > 0 || jexp > 0)
				pc_gainexp (sd, target, exp, jexp, 0);
		}
	}
}

SkillRideInLightening::SkillRideInLightening() : SkillImplRecursiveDamageSplash(SR_RIDEINLIGHTNING) {
}

void SkillRideInLightening::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr) {
		dmg.div_ = max(1, skill_lv);
	}else {
		dmg.div_ = 1;
	}
}

void SkillRideInLightening::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	skillratio += -100 + 40 * skill_lv;
	if (sd && sd->status.weapon == W_KNUCKLE)
		skillratio += 50 * skill_lv;
	RE_LVL_DMOD(100);
}

void SkillRideInLightening::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);
	map_foreachinallarea(skill_area_sub, src->m, x-i, y-i, x+i, y+i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_damage_id);
}

SkillRuwach::SkillRuwach() : SkillImpl(AL_RUWACH)
{
}

void SkillRuwach::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const
{
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv, sc_start2(src, bl, type, 100, skill_lv, getSkillId(), skill_get_time(getSkillId(), skill_lv)));
}

void SkillRuwach::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 45;
}

SkillSanctuary::SkillSanctuary() : SkillImpl(PR_SANCTUARY) {
}

void SkillSanctuary::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	flag |= 1;
	skill_unitsetting(src, getSkillId(), skill_lv, x, y, 0);
}

SkillSecondFaith::SkillSecondFaith() : SkillImplRecursiveDamageSplash(IQ_SECOND_FAITH) {
}

void SkillSecondFaith::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 100 + 2300 * skill_lv + 5 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillSecondFaith::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_SECOND_BRAND, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillSecondFaith::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillSecondFlame::SkillSecondFlame() : SkillImplRecursiveDamageSplash(IQ_SECOND_FLAME) {
}

void SkillSecondFlame::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 200 + 2900 * skill_lv + 9 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillSecondFlame::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_SECOND_BRAND, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillSecondFlame::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillSecondJudgement::SkillSecondJudgement() : SkillImplRecursiveDamageSplash(IQ_SECOND_JUDGEMENT) {
}

void SkillSecondJudgement::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 2000 + 500 * skill_lv;
	skillratio += 7 * sstatus->pow;	// TODO : pow ratio has changed ?

	RE_LVL_DMOD(100);
}

void SkillSecondJudgement::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_SECOND_BRAND, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillSecondJudgement::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillSilentium::SkillSilentium() : SkillImpl(AB_SILENTIUM) {
}

void SkillSilentium::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Should the level of Lex Divina be equivalent to the level of Silentium or should the highest level learned be used? [LimitLine]
	map_foreachinallrange(skill_area_sub, src, skill_get_splash(getSkillId(), skill_lv), BL_CHAR,
		src, PR_LEXDIVINA, skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
}

SkillSkyNetBlow::SkillSkyNetBlow() : SkillImplRecursiveDamageSplash(SR_SKYNETBLOW) {
}

void SkillSkyNetBlow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	//ATK [{(Skill Level x 200) + (Caster AGI)} x Caster Base Level / 100] %
	skillratio += -100 + 200 * skill_lv + sstatus->agi / 6; // !TODO: Confirm AGI bonus
	RE_LVL_DMOD(100);
}

void SkillSkyNetBlow::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, target, getSkillId(), skill_lv, tick, flag);

	if (skill_area_temp[2] == 0) {
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	}
}

SkillSnap::SkillSnap() : SkillImpl(MO_BODYRELOCATION) {
}

void SkillSnap::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (unit_movepos(src, x, y, 2, 1)) {
#if PACKETVER >= 20111005
		clif_snap(src, src->x, src->y);
#else
		clif_skill_poseffect( *src, getSkillId(), skill_lv, src->x, src->y, tick );
#endif
		if (sd)
			skill_blockpc_start (*sd, MO_EXTREMITYFIST, 2000);
	}
}

SkillStatusRecovery::SkillStatusRecovery() : SkillImpl(PR_STRECOVERY) {
}

void SkillStatusRecovery::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change* tsc = status_get_sc(target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if(status_isimmune(target)) {
		clif_skill_nodamage(src,*target,getSkillId(), skill_lv, false);
		return;
	}
	if (battle_check_undead(tstatus->race, tstatus->def_ele))
		skill_addtimerskill(src, tick + 1000, target->id, 0, 0, getSkillId(), skill_lv, 100, flag);
	else {
		// Bodystate is reset to "normal" for non-undead
		if (tsc) {
			// The following are bodystate status changes
			status_change_end(target, SC_STONE);
			status_change_end(target, SC_FREEZE);
			status_change_end(target, SC_STUN);
			status_change_end(target, SC_SLEEP);
			status_change_end(target, SC_STONEWAIT);
			status_change_end(target, SC_BURNING);
			status_change_end(target, SC_WHITEIMPRISON);
		}
		// Resetting bodystate to normal always also resets the monster AI to idle
		if (dstmd)
			mob_unlocktarget(dstmd, tick);
	}
	if (tsc) {
		// Ends SC_NETHERWORLD and SC_NORECOVER_STATE (even on undead)
		status_change_end(target, SC_NETHERWORLD);
		status_change_end(target, SC_NORECOVER_STATE);
	}
	clif_skill_nodamage(src,*target, getSkillId(),skill_lv);
}

#ifdef RENEWAL
SkillSuffragium::SkillSuffragium() : SkillImpl(PR_SUFFRAGIUM) {
}

void SkillSuffragium::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr || sd->status.party_id == 0 || (flag & 1)) {

		// Animations don't play when outside visible range
		if (check_distance_bl(src, target, AREA_SIZE))
			clif_skill_nodamage(target, *target, getSkillId(), skill_lv);

		sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
	else if (sd)
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag | BCT_PARTY | 1, skill_castend_nodamage_id);
}
#else
SkillSuffragium::SkillSuffragium() : StatusSkillImpl(PR_SUFFRAGIUM) {
}
#endif

SkillSummoningSpiritSphere::SkillSummoningSpiritSphere() : SkillImpl(MO_CALLSPIRITS) {
}

void SkillSummoningSpiritSphere::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd) {
		int32 limit = skill_lv;
		if( sd->sc.getSCE(SC_RAISINGDRAGON) )
			limit += sd->sc.getSCE(SC_RAISINGDRAGON)->val1;
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		pc_addspiritball(sd,skill_get_time(getSkillId(),skill_lv),limit);
	}
}

SkillTeleport::SkillTeleport() : SkillImpl(AL_TELEPORT) {
}

void SkillTeleport::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd != nullptr)
	{
		if (map_getmapflag(target->m, MF_NOTELEPORT) && skill_lv <= 2) {
			clif_skill_teleportmessage( *sd, NOTIFY_MAPINFO_CANT_TP );
			return;
		}
		if(!battle_config.duel_allow_teleport && sd->duel_group && skill_lv <= 2) { // duel restriction [LuzZza]
			char output[128]; sprintf(output, msg_txt(sd,365), skill_get_name(getSkillId()));
			clif_displaymessage(sd->fd, output); //"Duel: Can't use %s in duel."
			return;
		}

		if( sd->state.autocast || ( (sd->skillitem == getSkillId() || battle_config.skip_teleport_lv1_menu) && skill_lv == 1 ) || skill_lv == 3 )
		{
			if( skill_lv == 1 )
				pc_randomwarp(sd,CLR_TELEPORT);
			else
				pc_setpos( sd, mapindex_name2id( sd->status.save_point.map ), sd->status.save_point.x, sd->status.save_point.y, CLR_TELEPORT );
			return;
		}

		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);

		std::vector<std::string> maps = {
			"Random"
		};

		if( skill_lv == 1 ){
			clif_skill_warppoint( *sd, getSkillId(), skill_lv, maps );
		}else{
			maps.push_back( sd->status.save_point.map );

			clif_skill_warppoint( *sd, getSkillId(), skill_lv, maps );
		}
	} else
		unit_warp(target,-1,-1,-1,CLR_TELEPORT);
}

SkillThirdConsecration::SkillThirdConsecration() : SkillImplRecursiveDamageSplash(IQ_THIRD_CONSECRATION) {
}

void SkillThirdConsecration::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 1250 * skill_lv;
	skillratio += 10 * sstatus->pow;

	RE_LVL_DMOD(100);
}

void SkillThirdConsecration::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change_end(target, SC_SECOND_BRAND);
}

void SkillThirdConsecration::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillThirdFlameBomb::SkillThirdFlameBomb() : SkillImplRecursiveDamageSplash(IQ_THIRD_FLAME_BOMB) {
}

void SkillThirdFlameBomb::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.div_ = min(dmg.div_ + dmg.miscflag, 3); // Number of hits doesn't go above 3.
}

void SkillThirdFlameBomb::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 650 * skill_lv + 10 * sstatus->pow;
	skillratio += sstatus->max_hp * 20 / 100;
	RE_LVL_DMOD(100);
}

void SkillThirdFlameBomb::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change_end(target, SC_SECOND_BRAND);
}

void SkillThirdFlameBomb::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if (sd && sd->spiritball / 5 > 1)
		skill_area_temp[0] = sd->spiritball / 5 - 1;

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillThirdPunish::SkillThirdPunish() : SkillImplRecursiveDamageSplash(IQ_THIRD_PUNISH) {
}

void SkillThirdPunish::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 450 + 1800 * skill_lv;
	skillratio += 10 * sstatus->pow;
	RE_LVL_DMOD(100);
}

void SkillThirdPunish::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_change_end(target, SC_SECOND_BRAND);
}

void SkillThirdPunish::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	if (sd) {
		uint8 limit = 5;
		status_change* sc = status_get_sc(src);

		if (sc && sc->getSCE(SC_RAISINGDRAGON))
			limit += sc->getSCE(SC_RAISINGDRAGON)->val1;
		for (uint8 i = 0; i < limit; i++)
			pc_addspiritball(sd, skill_get_time(getSkillId(), skill_lv), limit);
	}

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillThrowSpiritSphere::SkillThrowSpiritSphere() : WeaponSkillImpl(MO_FINGEROFFENSIVE) {
}

void SkillThrowSpiritSphere::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr) {
		if (battle_config.finger_offensive_type)
			dmg.div_ = 1;
#ifndef RENEWAL
		else if ((sd->spiritball + sd->spiritball_old) < dmg.div_)
			dmg.div_ = sd->spiritball + sd->spiritball_old;
#endif
	}
}

void SkillThrowSpiritSphere::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	if (battle_config.finger_offensive_type && sd) {
		for (int32 i = 1; i < sd->spiritball_old; i++)
			skill_addtimerskill(src, tick + i * 200, target->id, 0, 0, getSkillId(), skill_lv, BF_WEAPON, flag);
	}
	status_change_end(src, SC_BLADESTOP);
}

void SkillThrowSpiritSphere::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_change* tsc = status_get_sc(target);

	base_skillratio += 500 + skill_lv * 200;
	if (tsc && tsc->getSCE(SC_BLADESTOP))
		base_skillratio += base_skillratio / 2;
#else
	base_skillratio += 50 * skill_lv;
#endif
}

SkillTigerCannon::SkillTigerCannon() : WeaponSkillImpl(SR_TIGERCANNON) {
}

void SkillTigerCannon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);
	const status_change *sc = status_get_sc(src);
	uint32 hp = sstatus->max_hp * (10 + (skill_lv * 2)) / 100;
	uint32 sp = sstatus->max_sp * (5 + skill_lv) / 100;

	if (wd->miscflag&8)
		// Base_Damage = [((Caster consumed HP + SP) / 2) x Caster Base Level / 100] %
		skillratio += -100 + (hp + sp) / 2;
	else
		// Base_Damage = [((Caster consumed HP + SP) / 4) x Caster Base Level / 100] %
		skillratio += -100 + (hp + sp) / 4;
	RE_LVL_DMOD(100);

	if (sc != nullptr && sc->hasSCE(SC_GT_REVITALIZE))
		skillratio += skillratio * 30 / 100;
}

void SkillTigerCannon::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *sc = status_get_sc(src);
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (flag & 1) {
		int32 sflag = flag|SD_ANIMATION;
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, sflag);
	} else if (sd) {
		if (sc && sc->getSCE(SC_COMBO) && sc->getSCE(SC_COMBO)->val1 == SR_FALLENEMPIRE && !sc->getSCE(SC_FLASHCOMBO))
			flag |= 8; // Only apply Combo bonus when Tiger Cannon is not used through Flash Combo
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR | BL_SKILL, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
}

void SkillTigerCannon::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

SkillTurnUndead::SkillTurnUndead() : SkillImpl(PR_TURNUNDEAD) {
}

void SkillTurnUndead::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	if (!battle_check_undead(tstatus->race, tstatus->def_ele))
		return;
	skill_attack(BF_MAGIC,src,src,target,getSkillId(), skill_lv, tick, flag);
}

SkillVituperatum::SkillVituperatum() : StatusSkillImpl(AB_VITUPERATUM) {
}

void SkillVituperatum::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag&1)
		StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
	else {
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY|1, skill_castend_nodamage_id);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillWarpPortal::SkillWarpPortal() : SkillImpl(AL_WARP) {
}

void SkillWarpPortal::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_change* sc = status_get_sc(src);

	if(sd != nullptr) {
		std::vector<std::string> maps( MAX_MEMOPOINTS + 1 );

		maps.push_back( sd->status.save_point.map );

		if( skill_lv >= 2 ){
			maps.push_back( sd->status.memo_point[0].map );

			if( skill_lv >= 3 ){
				maps.push_back( sd->status.memo_point[1].map );

				if( skill_lv >= 4 ){
					maps.push_back( sd->status.memo_point[2].map );
				}
			}
		}

		clif_skill_warppoint( *sd, getSkillId(), skill_lv, maps );
	}
	if( sc && sc->getSCE(SC_CURSEDCIRCLE_ATKER) ) //Should only remove after the skill has been casted.
		status_change_end(src,SC_CURSEDCIRCLE_ATKER);
	// not to consume item.
	flag |= SKILL_NOCONSUME_REQ;
}

SkillWindmill::SkillWindmill() : SkillImplRecursiveDamageSplash(SR_WINDMILL) {
}

void SkillWindmill::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	mob_data* dstmd = BL_CAST(BL_MOB, target);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if( dstsd )
		skill_addtimerskill(src,tick+status_get_amotion(src),target->id,0,0,getSkillId(),skill_lv,BF_WEAPON,0);
	else if( dstmd )
		sc_start(src,target, SC_STUN, 100, skill_lv, 1000 + 1000 * (rnd() %3));
}

void SkillWindmill::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	// ATK [(Caster Base Level + Caster DEX) x Caster Base Level / 100] %
	skillratio += -100 + status_get_lv(src) + sstatus->dex;
	RE_LVL_DMOD(100);
}

void SkillWindmill::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
	skill_castend_damage_id(src, src, getSkillId(), skill_lv, tick, flag);
}

SkillZen::SkillZen() : SkillImpl(CH_SOULCOLLECT) {
}

void SkillZen::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd) {
		int32 limit = 5;
		if( sd->sc.getSCE(SC_RAISINGDRAGON) )
			limit += sd->sc.getSCE(SC_RAISINGDRAGON)->val1;
		clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
		for (int32 i = 0; i < limit; i++)
			pc_addspiritball(sd,skill_get_time(getSkillId(),skill_lv),limit);
	}
}

std::unique_ptr<const SkillImpl> SkillFactoryAcolyte::create(const e_skill skill_id) const {
	switch( skill_id ){
		case AB_ADORAMUS:
			return std::make_unique<SkillAdoramus>();
		case AB_ANCILLA:
			return std::make_unique<SkillAncilla>();
		case AB_CANTO:
			return std::make_unique<SkillCantoCandidus>();
		case AB_CHEAL:
			return std::make_unique<SkillColuceoHeal>();
		case AB_CLEARANCE:
			return std::make_unique<SkillClearance>();
		case AB_CLEMENTIA:
			return std::make_unique<SkillCrementia>();
		case AB_CONVENIO:
			return std::make_unique<SkillConvenio>();
		case AB_DUPLELIGHT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case AB_DUPLELIGHT_MAGIC:
			return std::make_unique<SkillDupleLightMagic>();
		case AB_DUPLELIGHT_MELEE:
			return std::make_unique<SkillDupleLightMelee>();
		case AB_EPICLESIS:
			return std::make_unique<SkillEpiclesis>();
		case AB_EXPIATIO:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case AB_HIGHNESSHEAL:
			return std::make_unique<SkillHighnessHeal>();
		case AB_JUDEX:
			return std::make_unique<SkillJudex>();
		case AB_LAUDAAGNUS:
			return std::make_unique<SkillLaudaAgnus>();
		case AB_LAUDARAMUS:
			return std::make_unique<SkillLaudaRamus>();
		case AB_OFFERTORIUM:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case AB_ORATIO:
			return std::make_unique<SkillOratio>();
		case AB_PRAEFATIO:
			return std::make_unique<SkillPraefatio>();
		case AB_RENOVATIO:
			return std::make_unique<SkillRenovatio>();
		case AB_SECRAMENT:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case AB_SILENTIUM:
			return std::make_unique<SkillSilentium>();
		case AB_VITUPERATUM:
			return std::make_unique<SkillVituperatum>();
		case ALL_RESURRECTION:
			return std::make_unique<SkillResurrection>();
		case AL_ANGELUS:
			return std::make_unique<SkillAngelus>();
		case AL_BLESSING:
			return std::make_unique<SkillBlessing>();
		case AL_CRUCIS:
			return std::make_unique<SkillCrucis>();
		case AL_CURE:
			return std::make_unique<SkillCure>();
		case AL_DECAGI:
			return std::make_unique<SkillDecreaseAgi>();
		case AL_HEAL:
			return std::make_unique<SkillHeal>();
		case AL_HOLYLIGHT:
			return std::make_unique<SkillHolyLight>();
		case AL_HOLYWATER:
			return std::make_unique<SkillHolyWater>();
		case AL_INCAGI:
			return std::make_unique<SkillIncreaseAgi>();
		case AL_PNEUMA:
			return std::make_unique<SkillPneuma>();
		case AL_RUWACH:
			return std::make_unique<SkillRuwach>();
		case AL_TELEPORT:
			return std::make_unique<SkillTeleport>();
		case AL_WARP:
			return std::make_unique<SkillWarpPortal>();
		case CD_ARBITRIUM:
			return std::make_unique<SkillArbitrium>();
		case CD_ARBITRIUM_ATK:
			return std::make_unique<SkillArbitriumAttack>();
		case CD_ARGUTUS_TELUM:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case CD_ARGUTUS_VITA:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case CD_BENEDICTUM:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case CD_COMPETENTIA:
			return std::make_unique<SkillCompetentia>();
		case CD_DILECTIO_HEAL:
			return std::make_unique<SkillDilectioHeal>();
		case CD_DIVINUS_FLOS:
			return std::make_unique<SkillDivinusFlos>();
		case CD_EFFLIGO:
			return std::make_unique<SkillEffligo>();
		case CD_FRAMEN:
			return std::make_unique<SkillFramen>();
		case CD_MEDIALE_VOTUM:
			return std::make_unique<SkillMedialeVotum>();
		case CD_PETITIO:
			return std::make_unique<SkillPetitio>();
		case CD_PNEUMATICUS_PROCELLA:
			return std::make_unique<SkillPneumaticusProcella>();
		case CD_PRESENS_ACIES:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case CD_RELIGIO:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case CD_REPARATIO:
			return std::make_unique<SkillReparatio>();
		case CH_CHAINCRUSH:
			return std::make_unique<SkillChainCrushCombo>();
		case CH_PALMSTRIKE:
			return std::make_unique<SkillRagingPalmStrike>();
		case CH_SOULCOLLECT:
			return std::make_unique<SkillZen>();
		case CH_TIGERFIST:
			return std::make_unique<SkillGlacierFist>();
		case HP_ASSUMPTIO:
			return std::make_unique<SkillAssumptio>();
		case HP_BASILICA:
			return std::make_unique<SkillBasilica>();
		case IQ_BLAZING_FLAME_BLAST:
			return std::make_unique<SkillBlazingFlameBlast>();
		case IQ_EXPOSION_BLASTER:
			return std::make_unique<SkillExplosionBlaster>();
		case IQ_FIRM_FAITH:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case IQ_FIRST_BRAND:
			return std::make_unique<SkillFirstBrand>();
		case IQ_FIRST_FAITH_POWER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case IQ_JUDGE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case IQ_MASSIVE_F_BLASTER:
			return std::make_unique<SkillMassiveFlameBlaster>();
		case IQ_OLEUM_SANCTUM:
			return std::make_unique<SkillOleumSanctum>();
		case IQ_POWERFUL_FAITH:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case IQ_SECOND_FAITH:
			return std::make_unique<SkillSecondFaith>();
		case IQ_SECOND_FLAME:
			return std::make_unique<SkillSecondFlame>();
		case IQ_SECOND_JUDGEMENT:
			return std::make_unique<SkillSecondJudgement>();
		case IQ_SINCERE_FAITH:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case IQ_THIRD_CONSECRATION:
			return std::make_unique<SkillThirdConsecration>();
		case IQ_THIRD_EXOR_FLAME:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case IQ_THIRD_FLAME_BOMB:
			return std::make_unique<SkillThirdFlameBomb>();
		case IQ_THIRD_PUNISH:
			return std::make_unique<SkillThirdPunish>();
		case MO_ABSORBSPIRITS:
			return std::make_unique<SkillAbsorbSpiritSphere>();
		case MO_BALKYOUNG:
			return std::make_unique<SkillKiExplosion>();
		case MO_BLADESTOP:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case MO_BODYRELOCATION:
			return std::make_unique<SkillSnap>();
		case MO_CALLSPIRITS:
			return std::make_unique<SkillSummoningSpiritSphere>();
		case MO_CHAINCOMBO:
			return std::make_unique<SkillRagingQuadrupleBlow>();
		case MO_COMBOFINISH:
			return std::make_unique<SkillRagingThrust>();
		case MO_EXPLOSIONSPIRITS:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case MO_EXTREMITYFIST:
			return std::make_unique<SkillAsuraStrike>();
		case MO_FINGEROFFENSIVE:
			return std::make_unique<SkillThrowSpiritSphere>();
		case MO_INVESTIGATE:
			return std::make_unique<SkillOccultImpaction>();
		case MO_KITRANSLATION:
			return std::make_unique<SkillKiTranslation>();
		case MO_STEELBODY:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case MO_TRIPLEATTACK:
			return std::make_unique<SkillRagingTrifectaBlow>();
		case PR_ASPERSIO:
			return std::make_unique<SkillAspersio>();
		case PR_BENEDICTIO:
			return std::make_unique<SkillBenedictioSanctissimiSacramenti>();
		case PR_GLORIA:
			return std::make_unique<SkillGloria>();
		case PR_IMPOSITIO:
			return std::make_unique<SkillImpositioManus>();
		case PR_KYRIE:
			return std::make_unique<SkillKyrieEleison>();
		case PR_LEXAETERNA:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case PR_LEXDIVINA:
			return std::make_unique<SkillLexDivina>();
		case PR_MAGNIFICAT:
			return std::make_unique<SkillMagnificat>();
		case PR_MAGNUS:
			return std::make_unique<SkillMagnusExorcismus>();
		case PR_REDEMPTIO:
			return std::make_unique<SkillRedemptio>();
		case PR_SANCTUARY:
			return std::make_unique<SkillSanctuary>();
		case PR_SLOWPOISON:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case PR_STRECOVERY:
			return std::make_unique<SkillStatusRecovery>();
		case PR_SUFFRAGIUM:
			return std::make_unique<SkillSuffragium>();
		case PR_TURNUNDEAD:
			return std::make_unique<SkillTurnUndead>();
		case SR_ASSIMILATEPOWER:
			return std::make_unique<SkillAssimilatePower>();
		case SR_CRESCENTELBOW:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SR_CRESCENTELBOW_AUTOSPELL:
			return std::make_unique<WeaponSkillImpl>(skill_id);
		case SR_CURSEDCIRCLE:
			return std::make_unique<SkillCursedCircle>();
		case SR_DRAGONCOMBO:
			return std::make_unique<SkillDragonCombo>();
		case SR_EARTHSHAKER:
			return std::make_unique<SkillEarthShaker>();
		case SR_FALLENEMPIRE:
			return std::make_unique<SkillFallenEmpire>();
		case SR_FLASHCOMBO:
			return std::make_unique<SkillFlashCombo>();
		case SR_GATEOFHELL:
			return std::make_unique<SkillGateOfHell>();
		case SR_GENTLETOUCH_CHANGE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SR_GENTLETOUCH_CURE:
			return std::make_unique<SkillGentleTouchCure>();
		case SR_GENTLETOUCH_ENERGYGAIN:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SR_GENTLETOUCH_QUIET:
			return std::make_unique<SkillGentleTouchQuiet>();
		case SR_GENTLETOUCH_REVITALIZE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SR_HOWLINGOFLION:
			return std::make_unique<SkillHowlingOfLion>();
		case SR_KNUCKLEARROW:
			return std::make_unique<SkillKnuckleArrow>();
		case SR_LIGHTNINGWALK:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case SR_POWERVELOCITY:
			return std::make_unique<SkillPowerVelocity>();
		case SR_RAISINGDRAGON:
			return std::make_unique<SkillRaisingDragon>();
		case SR_RAMPAGEBLASTER:
			return std::make_unique<SkillRampageBlaster>();
		case SR_RIDEINLIGHTNING:
			return std::make_unique<SkillRideInLightening>();
		case SR_SKYNETBLOW:
			return std::make_unique<SkillSkyNetBlow>();
		case SR_TIGERCANNON:
			return std::make_unique<SkillTigerCannon>();
		case SR_WINDMILL:
			return std::make_unique<SkillWindmill>();

		default:
			return nullptr;
	}
	return nullptr;
}

#endif
