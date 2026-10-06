// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MAP_GENERATOR

#include "skill_elemental.hpp"

#include "map/clif.hpp"
#include "map/elemental.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include <common/random.hpp>
#include "map/map.hpp"
#include <config/core.hpp>
#include "skill_impl.hpp"

SkillAgeOfIce::SkillAgeOfIce() : SkillImplRecursiveDamageSplash(EM_EL_AGE_OF_ICE) {
}

void SkillAgeOfIce::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const s_elemental_data* ed = BL_CAST(BL_ELEM, src);

	base_skillratio += -100 + 3700;
	if (ed)
		base_skillratio += base_skillratio * status_get_lv(ed->master) / 100;
}

void SkillAgeOfIce::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillAquaPlay::SkillAquaPlay() : SkillImpl(EL_AQUAPLAY) {
}

void SkillAquaPlay::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillAvalanche::SkillAvalanche() : SkillImplRecursiveDamageSplash(EM_EL_AVALANCHE) {
}

void SkillAvalanche::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const s_elemental_data* ed = BL_CAST(BL_ELEM, src);

	base_skillratio += -100 + 450;
	if (ed)
		base_skillratio += base_skillratio * status_get_lv(ed->master) / 100;
}

void SkillAvalanche::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillBlast::SkillBlast() : SkillImpl(EL_BLAST) {
}

void SkillBlast::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillCircleOfFire::SkillCircleOfFire() : SkillImpl(EL_CIRCLE_OF_FIRE) {
}

void SkillCircleOfFire::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 200;
}

void SkillCircleOfFire::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillColdForce::SkillColdForce() : SkillImpl(EM_EL_COLD_FORCE) {
}

void SkillColdForce::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	s_elemental_data *ele = BL_CAST(BL_ELEM, src);

	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillCoolAir::SkillCoolAir() : SkillImpl(EL_CHILLY_AIR) {
}

void SkillCoolAir::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillCooler::SkillCooler() : SkillImpl(EL_COOLER) {
}

void SkillCooler::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillCrystalArmor::SkillCrystalArmor() : SkillImpl(EM_EL_CRYSTAL_ARMOR) {
}

void SkillCrystalArmor::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	s_elemental_data *ele = BL_CAST(BL_ELEM, src);

	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillCursedSoil::SkillCursedSoil() : SkillImpl(EL_CURSED_SOIL) {
}

void SkillCursedSoil::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillDeadlyPoison::SkillDeadlyPoison() : SkillImplRecursiveDamageSplash(EM_EL_DEADLY_POISON) {
}

void SkillDeadlyPoison::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const s_elemental_data* ed = BL_CAST(BL_ELEM, src);

	base_skillratio += -100 + 700;
	if (ed)
		base_skillratio += base_skillratio * status_get_lv(ed->master) / 100;
}

void SkillDeadlyPoison::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillDeepPoisoning::SkillDeepPoisoning() : SkillImpl(EM_EL_DEEP_POISONING) {
}

void SkillDeepPoisoning::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillEarthCare::SkillEarthCare() : SkillImpl(EM_EL_EARTH_CARE) {
}

void SkillEarthCare::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillEyesOfStorm::SkillEyesOfStorm() : SkillImpl(EM_EL_EYES_OF_STORM) {
}

void SkillEyesOfStorm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillFireArrow::SkillFireArrow() : SkillImpl(EL_FIRE_ARROW) {
}

void SkillFireArrow::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 200;
}

void SkillFireArrow::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

// EL_FIRE_BOMB
SkillFireBomb::SkillFireBomb() : SkillImpl(EL_FIRE_BOMB) {
}

void SkillFireBomb::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 400;
}

void SkillFireBomb::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 )
		skill_attack(skill_get_type(EL_FIRE_BOMB_ATK),src,src,target,EL_FIRE_BOMB_ATK,skill_lv,tick,flag);
	else {
		int32 i = skill_get_splash(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
		clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
		if( rnd()%100 < 30 )
			map_foreachinrange(skill_area_sub,target,i,BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
		else
			skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
	}
}


// EL_FIRE_BOMB_ATK
SkillFireBombAttack::SkillFireBombAttack() : SkillImpl(EL_FIRE_BOMB_ATK) {
}

void SkillFireBombAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 200;
}

SkillFireCloak::SkillFireCloak() : SkillImpl(EL_FIRE_CLOAK) {
}

void SkillFireCloak::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillFireMantle::SkillFireMantle() : SkillImpl(EL_FIRE_MANTLE) {
}

void SkillFireMantle::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 900;
}

void SkillFireMantle::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	skill_unitsetting(src,getSkillId(),skill_lv,target->x,target->y,0);
}

// EL_FIRE_WAVE
SkillFireWave::SkillFireWave() : SkillImpl(EL_FIRE_WAVE) {
}

void SkillFireWave::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 1100;
}

void SkillFireWave::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 )
		skill_attack(skill_get_type(EL_FIRE_WAVE_ATK),src,src,target,EL_FIRE_WAVE_ATK,skill_lv,tick,flag);
	else {
		int32 i = skill_get_splash(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
		clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
		if( rnd()%100 < 30 )
			map_foreachinrange(skill_area_sub,target,i,BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
		else
			skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
	}
}


// EL_FIRE_WAVE_ATK
SkillFireWaveAttack::SkillFireWaveAttack() : SkillImpl(EL_FIRE_WAVE_ATK) {
}

void SkillFireWaveAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 500;
}

SkillFlameArmor::SkillFlameArmor() : SkillImpl(EM_EL_FLAMEARMOR) {
}

void SkillFlameArmor::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	s_elemental_data *ele = BL_CAST(BL_ELEM, src);

	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillFlameRock::SkillFlameRock() : SkillImplRecursiveDamageSplash(EM_EL_FLAMEROCK) {
}

void SkillFlameRock::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const s_elemental_data* ed = BL_CAST(BL_ELEM, src);

	base_skillratio += -100 + 2400;
	if (ed)
		base_skillratio += base_skillratio * status_get_lv(ed->master) / 100;
}

void SkillFlameRock::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillFlameTechnic::SkillFlameTechnic() : SkillImpl(EM_EL_FLAMETECHNIC) {
}

void SkillFlameTechnic::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());
	s_elemental_data *ele = BL_CAST(BL_ELEM, src);

	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillGraceBreeze::SkillGraceBreeze() : SkillImpl(EM_EL_GRACE_BREEZE) {
}

void SkillGraceBreeze::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillGust::SkillGust() : SkillImpl(EL_GUST) {
}

void SkillGust::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillHeater::SkillHeater() : SkillImpl(EL_HEATER) {
}

void SkillHeater::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

// EL_HURRICANE
SkillHurricaneRage::SkillHurricaneRage() : SkillImpl(EL_HURRICANE) {
}

void SkillHurricaneRage::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 600;
}

void SkillHurricaneRage::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 )
		skill_attack(skill_get_type(EL_HURRICANE_ATK),src,src,target,EL_HURRICANE_ATK,skill_lv,tick,flag);
	else {
		int32 i = skill_get_splash(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
		clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
		if( rnd()%100 < 30 )
			map_foreachinrange(skill_area_sub,target,i,BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
		else
			skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
	}
}


// EL_HURRICANE_ATK
SkillHurricaneRageAttack::SkillHurricaneRageAttack() : SkillImpl(EL_HURRICANE_ATK) {
}

void SkillHurricaneRageAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 400;
}

SkillIceNeedle::SkillIceNeedle() : SkillImpl(EL_ICE_NEEDLE) {
}

void SkillIceNeedle::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 400;
}

void SkillIceNeedle::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillPetrology::SkillPetrology() : SkillImpl(EL_PETROLOGY) {
}

void SkillPetrology::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillPoisonShield::SkillPoisonShield() : SkillImpl(EM_EL_POISON_SHIELD) {
}

void SkillPoisonShield::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillPowerOfGaia::SkillPowerOfGaia() : SkillImpl(EL_POWER_OF_GAIA) {
}

void SkillPowerOfGaia::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	skill_unitsetting(src,getSkillId(),skill_lv,target->x,target->y,0);
}

SkillPyrotechnic::SkillPyrotechnic() : SkillImpl(EL_PYROTECHNIC) {
}

void SkillPyrotechnic::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

// EL_ROCK_CRUSHER
SkillRockLauncher::SkillRockLauncher() : SkillImpl(EL_ROCK_CRUSHER) {
}

void SkillRockLauncher::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target, SC_ROCK_CRUSHER,50,skill_lv,skill_get_time(EL_ROCK_CRUSHER,skill_lv));
}

void SkillRockLauncher::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 700;
}

void SkillRockLauncher::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
	clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	if( rnd()%100 < 50 )
		skill_attack(BF_MAGIC,src,src,target,getSkillId(),skill_lv,tick,flag);
	else
		skill_attack(BF_WEAPON,src,src,target,EL_ROCK_CRUSHER_ATK,skill_lv,tick,flag);
}


// EL_ROCK_CRUSHER_ATK
SkillRockLauncherAttack::SkillRockLauncherAttack() : SkillImpl(EL_ROCK_CRUSHER_ATK) {
}

void SkillRockLauncherAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_ROCK_CRUSHER_ATK,50,skill_lv,skill_get_time(EL_ROCK_CRUSHER,skill_lv));
}

void SkillRockLauncherAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 200;
}

SkillSolidSkin::SkillSolidSkin() : SkillImpl(EL_SOLID_SKIN) {
}

void SkillSolidSkin::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillStoneHammer::SkillStoneHammer() : SkillImpl(EL_STONE_HAMMER) {
}

void SkillStoneHammer::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_STUN, 10 * skill_lv, skill_lv, skill_get_time(getSkillId(), skill_lv));
}

void SkillStoneHammer::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 400;
}

void SkillStoneHammer::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillStoneRain::SkillStoneRain() : SkillImpl(EL_STONE_RAIN) {
}

void SkillStoneRain::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 200;
}

void SkillStoneRain::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 )
		skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
	else {
		int32 i = skill_get_splash(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
		if( rnd()%100 < 30 )
			map_foreachinrange(skill_area_sub,target,i,BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
		else
			skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
	}
}

SkillStoneShield::SkillStoneShield() : SkillImpl(EL_STONE_SHIELD) {
}

void SkillStoneShield::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillStormWind::SkillStormWind() : SkillImplRecursiveDamageSplash(EM_EL_STORM_WIND) {
}

void SkillStormWind::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	const s_elemental_data* ed = BL_CAST(BL_ELEM, src);

	base_skillratio += -100 + 2600;
	if (ed)
		base_skillratio += base_skillratio * status_get_lv(ed->master) / 100;
}

void SkillStormWind::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);

	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
}

SkillStrongProtection::SkillStrongProtection() : SkillImpl(EM_EL_STRONG_PROTECTION) {
}

void SkillStrongProtection::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillTidalWeapon::SkillTidalWeapon() : SkillImpl(EL_TIDAL_WEAPON) {
}

void SkillTidalWeapon::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 1400;
}

void SkillTidalWeapon::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( src->type == BL_ELEM ) {
		status_change *tsc = status_get_sc(target);
		s_elemental_data *ele = BL_CAST(BL_ELEM,src);
		status_change *tsc_ele = status_get_sc(ele);
		sc_type type = SC_TIDAL_WEAPON_OPTION;
		sc_type type2 = SC_TIDAL_WEAPON;

		clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
		clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
		if( (tsc_ele && tsc_ele->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(battle_get_master(src),type);
			status_change_end(src,type2);
		}
		if( rnd()%100 < 50 )
			skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
		else {
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,battle_get_master(src),type,100,ele->id,skill_get_time(getSkillId(),skill_lv));
		}
		clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
	}
}

SkillTropic::SkillTropic() : SkillImpl(EL_TROPIC) {
}

void SkillTropic::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

// EL_TYPOON_MIS
SkillTyphoonMissile::SkillTyphoonMissile() : SkillImpl(EL_TYPOON_MIS) {
}

void SkillTyphoonMissile::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src,target,SC_SILENCE,10*skill_lv,skill_lv,skill_get_time(getSkillId(),skill_lv));
}

void SkillTyphoonMissile::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 900;
}

void SkillTyphoonMissile::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 )
		skill_attack(skill_get_type(EL_TYPOON_MIS_ATK),src,src,target,EL_TYPOON_MIS_ATK,skill_lv,tick,flag);
	else {
		int32 i = skill_get_splash(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
		clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
		if( rnd()%100 < 30 )
			map_foreachinrange(skill_area_sub,target,i,BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
		else
			skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
	}
}


// EL_TYPOON_MIS_ATK
SkillTyphoonMissileAttack::SkillTyphoonMissileAttack() : SkillImpl(EL_TYPOON_MIS_ATK) {
}

void SkillTyphoonMissileAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 1100;
}

SkillUpheaval::SkillUpheaval() : SkillImpl(EL_UPHEAVAL) {
}

void SkillUpheaval::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillWaterBarrier::SkillWaterBarrier() : SkillImpl(EL_WATER_BARRIER) {
}

void SkillWaterBarrier::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	skill_unitsetting(src,getSkillId(),skill_lv,target->x,target->y,0);
}

SkillWaterDrop::SkillWaterDrop() : SkillImpl(EL_WATER_DROP) {
}

void SkillWaterDrop::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillWaterScreen::SkillWaterScreen() : SkillImpl(EL_WATER_SCREEN) {
}

void SkillWaterScreen::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		status_change *esc = status_get_sc(ele);
		sc_type type2 = (sc_type)(type-1);

		clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(target,type);
			status_change_end(src,type2);
		} else {
			// This not heals at the end.
			clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,src->id,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

// EL_WATER_SCREW
SkillWaterScrew::SkillWaterScrew() : SkillImpl(EL_WATER_SCREW) {
}

void SkillWaterScrew::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 900;
}

void SkillWaterScrew::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if( flag&1 )
		skill_attack(skill_get_type(EL_WATER_SCREW_ATK),src,src,target,EL_WATER_SCREW_ATK,skill_lv,tick,flag);
	else {
		int32 i = skill_get_splash(getSkillId(),skill_lv);
		clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
		clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
		if( rnd()%100 < 30 )
			map_foreachinrange(skill_area_sub,target,i,BL_CHAR,src,getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
		else
			skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
	}
}


// EL_WATER_SCREW_ATK
SkillWaterScrewAttack::SkillWaterScrewAttack() : SkillImpl(EL_WATER_SCREW_ATK) {
}

void SkillWaterScrewAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 900;
}

SkillWildStorm::SkillWildStorm() : SkillImpl(EL_WILD_STORM) {
}

void SkillWildStorm::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *src, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillWindCurtain::SkillWindCurtain() : SkillImpl(EL_WIND_CURTAIN) {
}

void SkillWindCurtain::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillWindSlasher::SkillWindSlasher() : SkillImpl(EL_WIND_SLASH) {
}

void SkillWindSlasher::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Non confirmed rate.
	sc_start2(src,target, SC_BLEEDING, 25, skill_lv, src->id, skill_get_time(getSkillId(),skill_lv));
}

void SkillWindSlasher::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 100;
}

void SkillWindSlasher::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_nodamage(src,*battle_get_master(src),getSkillId(),skill_lv);
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	skill_attack(skill_get_type(getSkillId()),src,src,target,getSkillId(),skill_lv,tick,flag);
}

SkillWindStep::SkillWindStep() : SkillImpl(EL_WIND_STEP) {
}

void SkillWindStep::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change *tsc = status_get_sc(target);
	sc_type type = skill_get_sc(getSkillId());

	s_elemental_data *ele = BL_CAST(BL_ELEM, src);
	if( ele ) {
		sc_type type2 = (sc_type)(type-1);
		status_change *esc = status_get_sc(ele);

		if( (esc && esc->getSCE(type2)) || (tsc && tsc->getSCE(type)) ) {
			status_change_end(src,type);
			status_change_end(target,type2);
		} else {
			clif_skill_nodamage(src,*src,getSkillId(),skill_lv);
			clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
			// There aren't teleport, just push the master away.
			skill_blown(src,target,(rnd()%skill_get_blewcount(getSkillId(),skill_lv))+1,rnd()%8,BLOWN_NONE);
			sc_start(src,src,type2,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
			sc_start(src,target,type,100,skill_lv,skill_get_time(getSkillId(),skill_lv));
		}
	}
}

SkillZephyr::SkillZephyr() : SkillImpl(EL_ZEPHYR) {
}

void SkillZephyr::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	clif_skill_damage( *src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, getSkillId(), skill_lv, DMG_SINGLE );
	skill_unitsetting(src,getSkillId(),skill_lv,target->x,target->y,0);
}

std::unique_ptr<const SkillImpl> SkillFactoryElemental::create(const e_skill skill_id) const {
	switch (skill_id) {
		case EL_AQUAPLAY:
			return std::make_unique<SkillAquaPlay>();
		case EL_BLAST:
			return std::make_unique<SkillBlast>();
		case EL_CHILLY_AIR:
			return std::make_unique<SkillCoolAir>();
		case EL_CIRCLE_OF_FIRE:
			return std::make_unique<SkillCircleOfFire>();
		case EL_COOLER:
			return std::make_unique<SkillCooler>();
		case EL_CURSED_SOIL:
			return std::make_unique<SkillCursedSoil>();
		case EL_FIRE_ARROW:
			return std::make_unique<SkillFireArrow>();
		case EL_FIRE_BOMB:
			return std::make_unique<SkillFireBomb>();
		case EL_FIRE_BOMB_ATK:
			return std::make_unique<SkillFireBombAttack>();
		case EL_FIRE_CLOAK:
			return std::make_unique<SkillFireCloak>();
		case EL_FIRE_MANTLE:
			return std::make_unique<SkillFireMantle>();
		case EL_FIRE_WAVE:
			return std::make_unique<SkillFireWave>();
		case EL_FIRE_WAVE_ATK:
			return std::make_unique<SkillFireWaveAttack>();
		case EL_GUST:
			return std::make_unique<SkillGust>();
		case EL_HEATER:
			return std::make_unique<SkillHeater>();
		case EL_HURRICANE:
			return std::make_unique<SkillHurricaneRage>();
		case EL_HURRICANE_ATK:
			return std::make_unique<SkillHurricaneRageAttack>();
		case EL_ICE_NEEDLE:
			return std::make_unique<SkillIceNeedle>();
		case EL_PETROLOGY:
			return std::make_unique<SkillPetrology>();
		case EL_POWER_OF_GAIA:
			return std::make_unique<SkillPowerOfGaia>();
		case EL_PYROTECHNIC:
			return std::make_unique<SkillPyrotechnic>();
		case EL_ROCK_CRUSHER:
			return std::make_unique<SkillRockLauncher>();
		case EL_ROCK_CRUSHER_ATK:
			return std::make_unique<SkillRockLauncherAttack>();
		case EL_SOLID_SKIN:
			return std::make_unique<SkillSolidSkin>();
		case EL_STONE_HAMMER:
			return std::make_unique<SkillStoneHammer>();
		case EL_STONE_RAIN:
			return std::make_unique<SkillStoneRain>();
		case EL_STONE_SHIELD:
			return std::make_unique<SkillStoneShield>();
		case EL_TIDAL_WEAPON:
			return std::make_unique<SkillTidalWeapon>();
		case EL_TROPIC:
			return std::make_unique<SkillTropic>();
		case EL_TYPOON_MIS:
			return std::make_unique<SkillTyphoonMissile>();
		case EL_TYPOON_MIS_ATK:
			return std::make_unique<SkillTyphoonMissileAttack>();
		case EL_UPHEAVAL:
			return std::make_unique<SkillUpheaval>();
		case EL_WATER_BARRIER:
			return std::make_unique<SkillWaterBarrier>();
		case EL_WATER_DROP:
			return std::make_unique<SkillWaterDrop>();
		case EL_WATER_SCREEN:
			return std::make_unique<SkillWaterScreen>();
		case EL_WATER_SCREW:
			return std::make_unique<SkillWaterScrew>();
		case EL_WATER_SCREW_ATK:
			return std::make_unique<SkillWaterScrewAttack>();
		case EL_WILD_STORM:
			return std::make_unique<SkillWildStorm>();
		case EL_WIND_CURTAIN:
			return std::make_unique<SkillWindCurtain>();
		case EL_WIND_SLASH:
			return std::make_unique<SkillWindSlasher>();
		case EL_WIND_STEP:
			return std::make_unique<SkillWindStep>();
		case EL_ZEPHYR:
			return std::make_unique<SkillZephyr>();
		case EM_EL_AGE_OF_ICE:
			return std::make_unique<SkillAgeOfIce>();
		case EM_EL_AVALANCHE:
			return std::make_unique<SkillAvalanche>();
		case EM_EL_COLD_FORCE:
			return std::make_unique<SkillColdForce>();
		case EM_EL_CRYSTAL_ARMOR:
			return std::make_unique<SkillCrystalArmor>();
		case EM_EL_DEADLY_POISON:
			return std::make_unique<SkillDeadlyPoison>();
		case EM_EL_DEEP_POISONING:
			return std::make_unique<SkillDeepPoisoning>();
		case EM_EL_EARTH_CARE:
			return std::make_unique<SkillEarthCare>();
		case EM_EL_EYES_OF_STORM:
			return std::make_unique<SkillEyesOfStorm>();
		case EM_EL_FLAMEARMOR:
			return std::make_unique<SkillFlameArmor>();
		case EM_EL_FLAMEROCK:
			return std::make_unique<SkillFlameRock>();
		case EM_EL_FLAMETECHNIC:
			return std::make_unique<SkillFlameTechnic>();
		case EM_EL_GRACE_BREEZE:
			return std::make_unique<SkillGraceBreeze>();
		case EM_EL_POISON_SHIELD:
			return std::make_unique<SkillPoisonShield>();
		case EM_EL_STORM_WIND:
			return std::make_unique<SkillStormWind>();
		case EM_EL_STRONG_PROTECTION:
			return std::make_unique<SkillStrongProtection>();

		default:
			return nullptr;
	}
}

#endif
