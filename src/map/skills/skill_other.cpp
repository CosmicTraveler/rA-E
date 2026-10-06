// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_other.hpp"

#include "map/clif.hpp"
#include "map/path.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include "map/map.hpp"
#include "map/battle.hpp"
#include "map/party.hpp"
#include "skill_impl.hpp"

SkillBaby::SkillBaby() : SkillImpl(WE_BABY) {
}

void SkillBaby::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr)
		return;

	map_session_data* f_sd = pc_get_father(sd);
	map_session_data* m_sd = pc_get_mother(sd);

	// Neither was found
	if (f_sd == nullptr && m_sd == nullptr) {
		clif_skill_fail(*sd, getSkillId());
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	// Not in same party
	// TODO : no check if sd->status.party_id == 0 ?
	if (sd->status.party_id != 0 && (f_sd == nullptr || sd->status.party_id != f_sd->status.party_id) && (m_sd == nullptr || sd->status.party_id != m_sd->status.party_id)) {
		clif_skill_fail(*sd, getSkillId());
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	// Not in same screen
	if ((f_sd == nullptr || !check_distance_bl(sd, f_sd, AREA_SIZE)) && (m_sd == nullptr || !check_distance_bl(sd, m_sd, AREA_SIZE))) {
		clif_skill_fail(*sd, getSkillId());
		flag |= SKILL_NOCONSUME_REQ;
		return;
	}

	status_change_start(src, target, SC_STUN, 10000, skill_lv, 0, 0, 0, skill_get_time2(getSkillId(), skill_lv), SCSTART_NORATEDEF);

	sc_type type = skill_get_sc(getSkillId());

	if (f_sd != nullptr)
		sc_start(src, f_sd, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
	if (m_sd != nullptr)
		sc_start(src, m_sd, type, 100, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

SkillBattleBuster::SkillBattleBuster() : WeaponSkillImpl(ABR_BATTLE_BUSTER) {
}

void SkillBattleBuster::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	// TODO: Need official formula.
	base_skillratio += -100 + 8000;
}

SkillCallAllFamily::SkillCallAllFamily() : SkillImpl(WE_CALLALLFAMILY) {
}

void SkillCallAllFamily::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd) {
		map_session_data *p_sd = pc_get_partner(sd);
		map_session_data *c_sd = pc_get_child(sd);

		if (!p_sd && !c_sd) { // Fail if no family members are found
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		// Partner must be on the same map and in same party
		if (p_sd && !status_isdead(*p_sd) && p_sd->m == sd->m && p_sd->status.party_id == sd->status.party_id)
			pc_setpos(p_sd, map_id2index(sd->m), sd->x, sd->y, CLR_TELEPORT);
		// Child must be on the same map and in same party as the parent casting
		if (c_sd && !status_isdead(*c_sd) && c_sd->m == sd->m && c_sd->status.party_id == sd->status.party_id)
			pc_setpos(c_sd, map_id2index(sd->m), sd->x, sd->y, CLR_TELEPORT);
	}
}

SkillCallBaby::SkillCallBaby() : SkillImpl(WE_CALLBABY) {
}

void SkillCallBaby::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillCallParent::SkillCallParent() : SkillImpl(WE_CALLPARENT) {
}

void SkillCallParent::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillCatCry::SkillCatCry() : SkillImpl(ALL_CATCRY) {
}

void SkillCatCry::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillCheerUp::SkillCheerUp() : StatusSkillImpl(WE_CHEERUP) {
}

void SkillCheerUp::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (sd) {
		map_session_data *f_sd = pc_get_father(sd);
		map_session_data *m_sd = pc_get_mother(sd);

		if (!f_sd && !m_sd && !dstsd) { // Fail if no family members are found
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
		if (flag&1) { // Buff can only be given to parents in 7x7 AoE around baby
			if (dstsd == f_sd || dstsd == m_sd)
				StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
		} else
			map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_PC, src, getSkillId(), skill_lv, tick, flag|BCT_ALL|1, skill_castend_nodamage_id);
	}
}

SkillChristmasCarol::SkillChristmasCarol() : SkillImpl(ALL_WEWISH) {
}

void SkillChristmasCarol::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillDualCannonFire::SkillDualCannonFire() : WeaponSkillImpl(ABR_DUAL_CANNON_FIRE) {
}

void SkillDualCannonFire::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	// TODO: Need official formula.
	base_skillratio += -100 + 8000;
}

SkillEquipSwitch::SkillEquipSwitch() : SkillImpl(ALL_EQSWITCH) {
}

void SkillEquipSwitch::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd ){
		clif_equipswitch_reply( sd, false );

		for( int32 i = 0, position = 0; i < EQI_MAX; i++ ){
			if( sd->equip_switch_index[i] >= 0 && !( position & equip_bitmask[i] ) ){
				position |= pc_equipswitch( sd, sd->equip_switch_index[i] );
			}
		}
	}
}

SkillGmSandman::SkillGmSandman() : SkillImpl(GM_SANDMAN) {
}

void SkillGmSandman::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);

	if( tsc ) {
		if( tsc->opt1 == OPT1_SLEEP )
			tsc->opt1 = 0;
		else
			tsc->opt1 = OPT1_SLEEP;
		clif_changeoption(target);
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}

SkillGuardiansRecall::SkillGuardiansRecall() : SkillImpl(ALL_GUARDIAN_RECALL) {
}

void SkillGuardiansRecall::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd != nullptr ){
		// Destination position.
		uint16 x = 44;
		uint16 y = 151;
		uint16 mapindex  = mapindex_name2id(MAP_MORA);

		sc_start( src, target, type, 100, skill_lv, skill_get_cooldown( getSkillId(), skill_lv ) );

		if(!mapindex)
		{ //Given map not found?
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		pc_setpos(sd, mapindex, x, y, CLR_TELEPORT);
	}
}

SkillILookUpToYou::SkillILookUpToYou() : SkillImpl(WE_FEMALE) {
}

void SkillILookUpToYou::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	uint8 sp_rate = abs(skill_get_sp_rate(getSkillId(), skill_lv));

	if (sp_rate && status_get_sp(src) > status_get_max_sp(src) / sp_rate) {
		int32 gain_sp = tstatus->max_sp * sp_rate / 100; // The earned is the same % of the target SP than it costed the caster. [Skotlex]

		clif_skill_nodamage(src,*target,getSkillId(),status_heal(target, 0, gain_sp, 0));
	}
}

SkillIMissYou::SkillIMissYou() : SkillImpl(WE_CALLPARTNER) {
}

void SkillIMissYou::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1; // Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}

SkillInfinityBuster::SkillInfinityBuster() : WeaponSkillImpl(ABR_INFINITY_BUSTER) {
}

void SkillInfinityBuster::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	// TODO: Need official formula.
	base_skillratio += -100 + 50000;
}

SkillIWillProtectYou::SkillIWillProtectYou() : SkillImpl(WE_MALE) {
}

void SkillIWillProtectYou::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	uint8 hp_rate = abs(skill_get_hp_rate(getSkillId(), skill_lv));

	if (hp_rate && status_get_hp(src) > status_get_max_hp(src) / hp_rate) {
		int32 gain_hp = tstatus->max_hp * hp_rate / 100; // The earned is the same % of the target HP than it costed the caster. [Skotlex]

		clif_skill_nodamage(src,*target,getSkillId(),status_heal(target, gain_hp, 0, 0));
	}
}

SkillNetRepair::SkillNetRepair() : SkillImpl(ABR_NET_REPAIR) {
}

void SkillNetRepair::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	if (flag & 1) {
		int32 heal_amount = tstatus->max_hp * 10 / 100;
		clif_skill_nodamage(nullptr, *target, AL_HEAL, heal_amount);
		status_heal(target, heal_amount, 0, 0);
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ALLY | SD_SPLASH | 1, skill_castend_nodamage_id);
	}
}

SkillNetSupport::SkillNetSupport() : SkillImpl(ABR_NET_SUPPORT) {
}

void SkillNetSupport::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);

	if (flag & 1) {
		int32 heal_amount = tstatus->max_sp * 3 / 100;
		clif_skill_nodamage(nullptr, *target, MG_SRECOVERY, heal_amount);
		status_heal(target, 0, heal_amount, 0);
	} else {
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, flag | BCT_ALLY | SD_SPLASH | 1, skill_castend_nodamage_id);
	}
}

SkillNiflheimRecall::SkillNiflheimRecall() : SkillImpl(ALL_NIFLHEIM_RECALL) {
}

void SkillNiflheimRecall::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd != nullptr ){
		// Destination position.
		uint16 x = 193;
		uint16 y = 186;
		uint16 mapindex = mapindex_name2id( MAP_NIFLHEIM );

		sc_start( src, target, type, 100, skill_lv, skill_get_cooldown( getSkillId(), skill_lv ) );

		if(!mapindex)
		{ //Given map not found?
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		pc_setpos(sd, mapindex, x, y, CLR_TELEPORT);
	}
}

SkillOdinsRecall::SkillOdinsRecall() : SkillImpl(ALL_ODINS_RECALL) {
}

void SkillOdinsRecall::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if(sd != nullptr)
	{
		if (map_getmapflag(target->m, MF_NOTELEPORT) && skill_lv <= 2) {
			clif_skill_teleportmessage( *sd, NOTIFY_MAPINFO_CANT_TP );
			return;
		}
		if(!battle_config.duel_allow_teleport && sd->duel_group && skill_lv <= 2) { // duel restriction [LuzZza]
			char output[128]; sprintf(output, msg_txt(sd,365), skill_get_name(ALL_ODINS_RECALL));
			clif_displaymessage(sd->fd, output); //"Duel: Can't use %s in duel."
			return;
		}

		if( sd->state.autocast || ( (sd->skillitem == AL_TELEPORT || battle_config.skip_teleport_lv1_menu) && skill_lv == 1 ) || skill_lv == 3 )
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

		maps.push_back( sd->status.save_point.map );

		clif_skill_warppoint( *sd, getSkillId(), skill_lv, maps );
	} else
		unit_warp(target,-1,-1,-1,CLR_TELEPORT);
}

SkillOneForever::SkillOneForever() : SkillImpl(WE_ONEFOREVER) {
}

void SkillOneForever::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_data* tstatus = status_get_status_data(*target);
	status_change *tsc = status_get_sc(target);
	map_session_data* sd = BL_CAST(BL_PC, src);
	map_session_data* dstsd = BL_CAST(BL_PC, target);

	if (sd) {
		map_session_data *p_sd = pc_get_partner(sd);
		map_session_data *c_sd = pc_get_child(sd);

		if (!p_sd && !c_sd && !dstsd) { // Fail if no family members are found
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}
		if (map_flag_gvg2(target->m) || map_getmapflag(target->m, MF_BATTLEGROUND)) { // No reviving in WoE grounds!
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		if (status_isdead(*target)) {
			int32 per = 30, sper = 0;

			if (battle_check_undead(tstatus->race, tstatus->def_ele))
				return;
			if (tsc && tsc->getSCE(SC_HELLPOWER))
				return;
			if (map_getmapflag(target->m, MF_PVP) && dstsd->pvp_point < 0)
				return;
			if (dstsd->special_state.restart_full_recover)
				per = sper = 100;
			if ((dstsd == p_sd || dstsd == c_sd) && status_revive(target, per, sper)) // Only family members can be revived
				clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
		}
	}
}

SkillOpenBuyingStore::SkillOpenBuyingStore() : SkillImpl(ALL_BUYING_STORE) {
}

void SkillOpenBuyingStore::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd )
	{// players only, skill allows 5 buying slots
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, buyingstore_setup(sd, MAX_BUYINGSTORE_SLOTS) == 0);
	}
}

SkillPartyAssumptio::SkillPartyAssumptio() : SkillImpl(CASH_ASSUMPTIO) {
}

void SkillPartyAssumptio::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) )
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillPartyBlessing::SkillPartyBlessing() : SkillImpl(CASH_BLESSING) {
}

void SkillPartyBlessing::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) )
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillPartyFlee::SkillPartyFlee() : StatusSkillImpl(ALL_PARTYFLEE) {
}

void SkillPartyFlee::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd  && !(flag&1) ) {
		if( !sd->status.party_id ) {
			clif_skill_fail( *sd, getSkillId() );
			return;
		}
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	} else
		StatusSkillImpl::castendNoDamageId(src, target, skill_lv, tick, flag);
}

SkillPartyIncreaseAgi::SkillPartyIncreaseAgi() : SkillImpl(CASH_INCAGI) {
}

void SkillPartyIncreaseAgi::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd == nullptr || sd->status.party_id == 0 || (flag & 1) )
		clif_skill_nodamage(target, *target, getSkillId(), skill_lv, sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
	else if (sd)
	{
		party_foreachsamemap(skill_area_sub, sd, skill_get_splash(getSkillId(), skill_lv), src, getSkillId(), skill_lv, tick, flag|BCT_PARTY|1, skill_castend_nodamage_id);
	}
}

SkillPeonyMamy::SkillPeonyMamy() : SkillImpl(ECL_PEONYMAMY) {
}

void SkillPeonyMamy::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_FREEZE);
	status_change_end(target, SC_FREEZING);
	status_change_end(target, SC_CRYSTALIZE);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), 1, DMG_SINGLE );
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillPronteraRecall::SkillPronteraRecall() : SkillImpl(ALL_PRONTERA_RECALL) {
}

void SkillPronteraRecall::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd != nullptr ){
		// Destination position.
		uint16 x;
		uint16 y;

		if(skill_lv == 1) {
			x = 115;
			y = 72;
		}
		else if(skill_lv == 2) {
			x = 159;
			y = 192;
		}
		uint16 mapindex  = mapindex_name2id(MAP_PRONTERA);

		sc_start( src, target, type, 100, skill_lv, skill_get_cooldown( getSkillId(), skill_lv ) );

		if(!mapindex)
		{ //Given map not found?
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		pc_setpos(sd, mapindex, x, y, CLR_TELEPORT);
	}
}

SkillRayOfProtection::SkillRayOfProtection() : SkillImpl(ALL_RAY_OF_PROTECTION) {
}

void SkillRayOfProtection::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(target,*target,getSkillId(),skill_lv,
		sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv)));
}

SkillReturnToEclage::SkillReturnToEclage() : SkillImpl(ECLAGE_RECALL) {
}

void SkillReturnToEclage::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd != nullptr ){
		// Destination position.
		uint16 x = 47;
		uint16 y = 31;
		uint16 mapindex  = mapindex_name2id(MAP_ECLAGE_IN);

		sc_start( src, target, type, 100, skill_lv, skill_get_cooldown( getSkillId(), skill_lv ) );

		if(!mapindex)
		{ //Given map not found?
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		pc_setpos(sd, mapindex, x, y, CLR_TELEPORT);
	}
}

SkillReturnToEldicastes::SkillReturnToEldicastes() : SkillImpl(RETURN_TO_ELDICASTES) {
}

void SkillReturnToEldicastes::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd != nullptr ){
		// Destination position.
		uint16 x = 198;
		uint16 y = 187;
		uint16 mapindex  = mapindex_name2id(MAP_DICASTES);

		sc_start( src, target, type, 100, skill_lv, skill_get_cooldown( getSkillId(), skill_lv ) );

		if(!mapindex)
		{ //Given map not found?
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		pc_setpos(sd, mapindex, x, y, CLR_TELEPORT);
	}
}

SkillReturnToGlastHeim::SkillReturnToGlastHeim() : SkillImpl(ALL_GLASTHEIM_RECALL) {
}

void SkillReturnToGlastHeim::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd != nullptr ){
		// Destination position.
		uint16 x = 200;
		uint16 y = 268;
		uint16 mapindex  = mapindex_name2id(MAP_GLASTHEIM);

		sc_start( src, target, type, 100, skill_lv, skill_get_cooldown( getSkillId(), skill_lv ) );

		if(!mapindex)
		{ //Given map not found?
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		pc_setpos(sd, mapindex, x, y, CLR_TELEPORT);
	}
}

SkillReturnToLighthalzen::SkillReturnToLighthalzen() : SkillImpl(ALL_LIGHTHALZEN_RECALL) {
}

void SkillReturnToLighthalzen::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd != nullptr ){
		// Destination position.
		uint16 x = 307;
		uint16 y = 307;
		uint16 mapindex  = mapindex_name2id(MAP_LIGHTHALZEN);

		sc_start( src, target, type, 100, skill_lv, skill_get_cooldown( getSkillId(), skill_lv ) );

		if(!mapindex)
		{ //Given map not found?
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		pc_setpos(sd, mapindex, x, y, CLR_TELEPORT);
	}
}

SkillReturnToThanatos::SkillReturnToThanatos() : SkillImpl(ALL_THANATOS_RECALL) {
}

void SkillReturnToThanatos::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());
	map_session_data* sd = BL_CAST(BL_PC, src);

	if( sd != nullptr ){
		// Destination position.
		uint16 x = 139;
		uint16 y = 156;
		uint16 mapindex  = mapindex_name2id(MAP_THANATOS);

		sc_start( src, target, type, 100, skill_lv, skill_get_cooldown( getSkillId(), skill_lv ) );

		if(!mapindex)
		{ //Given map not found?
			clif_skill_fail( *sd, getSkillId() );
			flag |= SKILL_NOCONSUME_REQ;
			return;
		}

		pc_setpos(sd, mapindex, x, y, CLR_TELEPORT);
	}
}

SkillRo20thAnniversaryFirecracker::SkillRo20thAnniversaryFirecracker() : SkillImpl(ALL_EVENT_20TH_ANNIVERSARY) {
}

void SkillRo20thAnniversaryFirecracker::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src, *src, getSkillId(), skill_lv);
}

SkillSadagui::SkillSadagui() : SkillImpl(ECL_SADAGUI) {
}

void SkillSadagui::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_STUN);
	status_change_end(target, SC_CONFUSION);
	status_change_end(target, SC_HALLUCINATION);
	status_change_end(target, SC_FEAR);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), 1, DMG_SINGLE );
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillSequoiaDust::SkillSequoiaDust() : SkillImpl(ECL_SEQUOIADUST) {
}

void SkillSequoiaDust::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_STONE);
	status_change_end(target, SC_POISON);
	status_change_end(target, SC_CURSE);
	status_change_end(target, SC_BLIND);
	status_change_end(target, SC_ORCISH);
	status_change_end(target, SC_DECREASEAGI);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), 1, DMG_SINGLE );
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillSnowFlip::SkillSnowFlip() : SkillImpl(ECL_SNOWFLIP) {
}

void SkillSnowFlip::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(target, SC_SLEEP);
	status_change_end(target, SC_BLEEDING);
	status_change_end(target, SC_BURNING);
	status_change_end(target, SC_DEEPSLEEP);

	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), 1, DMG_SINGLE );
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillSummerNightDream::SkillSummerNightDream() : SkillImpl(ALL_DREAM_SUMMERNIGHT) {
}

void SkillSummerNightDream::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*target,getSkillId(),skill_lv);
}

SkillWeaponEnchantment::SkillWeaponEnchantment() : SkillImpl(ITEM_ENCHANTARMS) {
}

void SkillWeaponEnchantment::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_get_ele(getSkillId(), skill_lv), skill_get_time(getSkillId(), skill_lv)));
}

std::unique_ptr<const SkillImpl> SkillFactoryOther::create(const e_skill skill_id) const {
	switch( skill_id ){
		case ABR_BATTLE_BUSTER:
			return std::make_unique<SkillBattleBuster>();
		case ABR_DUAL_CANNON_FIRE:
			return std::make_unique<SkillDualCannonFire>();
		case ABR_INFINITY_BUSTER:
			return std::make_unique<SkillInfinityBuster>();
		case ABR_NET_REPAIR:
			return std::make_unique<SkillNetRepair>();
		case ABR_NET_SUPPORT:
			return std::make_unique<SkillNetSupport>();
		case ALL_BUYING_STORE:
			return std::make_unique<SkillOpenBuyingStore>();
		case ALL_CATCRY:
			return std::make_unique<SkillCatCry>();
		case ALL_DREAM_SUMMERNIGHT:
			return std::make_unique<SkillSummerNightDream>();
		case ALL_EQSWITCH:
			return std::make_unique<SkillEquipSwitch>();
		case ALL_EVENT_20TH_ANNIVERSARY:
			return std::make_unique<SkillRo20thAnniversaryFirecracker>();
		case ALL_FULL_THROTTLE:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case ALL_GLASTHEIM_RECALL:
			return std::make_unique<SkillReturnToGlastHeim>();
		case ALL_GUARDIAN_RECALL:
			return std::make_unique<SkillGuardiansRecall>();
		case ALL_LIGHTHALZEN_RECALL:
			return std::make_unique<SkillReturnToLighthalzen>();
		case ALL_NIFLHEIM_RECALL:
			return std::make_unique<SkillNiflheimRecall>();
		case ALL_ODINS_POWER:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case ALL_ODINS_RECALL:
			return std::make_unique<SkillOdinsRecall>();
		case ALL_PARTYFLEE:
			return std::make_unique<SkillPartyFlee>();
		case ALL_PRONTERA_RECALL:
			return std::make_unique<SkillPronteraRecall>();
		case ALL_RAY_OF_PROTECTION:
			return std::make_unique<SkillRayOfProtection>();
		case ALL_REVERSEORCISH:
			return std::make_unique<StatusSkillImpl>(skill_id);
		case ALL_THANATOS_RECALL:
			return std::make_unique<SkillReturnToThanatos>();
		case ALL_WEWISH:
			return std::make_unique<SkillChristmasCarol>();
		case CASH_ASSUMPTIO:
			return std::make_unique<SkillPartyAssumptio>();
		case CASH_BLESSING:
			return std::make_unique<SkillPartyBlessing>();
		case CASH_INCAGI:
			return std::make_unique<SkillPartyIncreaseAgi>();
		case ECLAGE_RECALL:
			return std::make_unique<SkillReturnToEclage>();
		case ECL_PEONYMAMY:
			return std::make_unique<SkillPeonyMamy>();
		case ECL_SADAGUI:
			return std::make_unique<SkillSadagui>();
		case ECL_SEQUOIADUST:
			return std::make_unique<SkillSequoiaDust>();
		case ECL_SNOWFLIP:
			return std::make_unique<SkillSnowFlip>();
		case GM_SANDMAN:
			return std::make_unique<SkillGmSandman>();
		case ITEM_ENCHANTARMS:
			return std::make_unique<SkillWeaponEnchantment>();
		case ITM_TOMAHAWK:
			return std::make_unique<WeaponSkillImpl>(skill_id);
		case RETURN_TO_ELDICASTES:
			return std::make_unique<SkillReturnToEldicastes>();
		case WE_BABY:
			return std::make_unique<SkillBaby>();
		case WE_CALLALLFAMILY:
			return std::make_unique<SkillCallAllFamily>();
		case WE_CALLBABY:
			return std::make_unique<SkillCallBaby>();
		case WE_CALLPARENT:
			return std::make_unique<SkillCallParent>();
		case WE_CALLPARTNER:
			return std::make_unique<SkillIMissYou>();
		case WE_CHEERUP:
			return std::make_unique<SkillCheerUp>();
		case WE_FEMALE:
			return std::make_unique<SkillILookUpToYou>();
		case WE_MALE:
			return std::make_unique<SkillIWillProtectYou>();
		case WE_ONEFOREVER:
			return std::make_unique<SkillOneForever>();

		default:
			return nullptr;
	}
}

#endif
