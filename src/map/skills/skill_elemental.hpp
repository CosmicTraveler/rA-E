// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "skill_factory.hpp"
#include "skill_impl.hpp"

class SkillFactoryElemental : public SkillFactory {
public:
	virtual std::unique_ptr<const SkillImpl> create(const e_skill skill_id) const override;
};

class SkillAgeOfIce : public SkillImplRecursiveDamageSplash {
public:
	SkillAgeOfIce();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillAquaPlay : public SkillImpl {
public:
	SkillAquaPlay();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAvalanche : public SkillImplRecursiveDamageSplash {
public:
	SkillAvalanche();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillBlast : public SkillImpl {
public:
	SkillBlast();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCircleOfFire : public SkillImpl {
public:
	SkillCircleOfFire();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillColdForce : public SkillImpl {
public:
	SkillColdForce();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCoolAir : public SkillImpl {
public:
	SkillCoolAir();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCooler : public SkillImpl {
public:
	SkillCooler();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCrystalArmor : public SkillImpl {
public:
	SkillCrystalArmor();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCursedSoil : public SkillImpl {
public:
	SkillCursedSoil();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDeadlyPoison : public SkillImplRecursiveDamageSplash {
public:
	SkillDeadlyPoison();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillDeepPoisoning : public SkillImpl {
public:
	SkillDeepPoisoning();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEarthCare : public SkillImpl {
public:
	SkillEarthCare();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEyesOfStorm : public SkillImpl {
public:
	SkillEyesOfStorm();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFireArrow : public SkillImpl {
public:
	SkillFireArrow();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// EL_FIRE_BOMB
class SkillFireBomb : public SkillImpl {
public:
	SkillFireBomb();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// EL_FIRE_BOMB_ATK
class SkillFireBombAttack : public SkillImpl {
public:
	SkillFireBombAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillFireCloak : public SkillImpl {
public:
	SkillFireCloak();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFireMantle : public SkillImpl {
public:
	SkillFireMantle();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// EL_FIRE_WAVE
class SkillFireWave : public SkillImpl {
public:
	SkillFireWave();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// EL_FIRE_WAVE_ATK
class SkillFireWaveAttack : public SkillImpl {
public:
	SkillFireWaveAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillFlameArmor : public SkillImpl {
public:
	SkillFlameArmor();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFlameRock : public SkillImplRecursiveDamageSplash {
public:
	SkillFlameRock();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillFlameTechnic : public SkillImpl {
public:
	SkillFlameTechnic();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGraceBreeze : public SkillImpl {
public:
	SkillGraceBreeze();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGust : public SkillImpl {
public:
	SkillGust();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHeater : public SkillImpl {
public:
	SkillHeater();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// EL_HURRICANE
class SkillHurricaneRage : public SkillImpl {
public:
	SkillHurricaneRage();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// EL_HURRICANE_ATK
class SkillHurricaneRageAttack : public SkillImpl {
public:
	SkillHurricaneRageAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillIceNeedle : public SkillImpl {
public:
	SkillIceNeedle();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPetrology : public SkillImpl {
public:
	SkillPetrology();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPoisonShield : public SkillImpl {
public:
	SkillPoisonShield();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPowerOfGaia : public SkillImpl {
public:
	SkillPowerOfGaia();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPyrotechnic : public SkillImpl {
public:
	SkillPyrotechnic();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// EL_ROCK_CRUSHER
class SkillRockLauncher : public SkillImpl {
public:
	SkillRockLauncher();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// EL_ROCK_CRUSHER_ATK
class SkillRockLauncherAttack : public SkillImpl {
public:
	SkillRockLauncherAttack();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillSolidSkin : public SkillImpl {
public:
	SkillSolidSkin();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStoneHammer : public SkillImpl {
public:
	SkillStoneHammer();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStoneRain : public SkillImpl {
public:
	SkillStoneRain();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStoneShield : public SkillImpl {
public:
	SkillStoneShield();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStormWind : public SkillImplRecursiveDamageSplash {
public:
	SkillStormWind();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillStrongProtection : public SkillImpl {
public:
	SkillStrongProtection();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTidalWeapon : public SkillImpl {
public:
	SkillTidalWeapon();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTropic : public SkillImpl {
public:
	SkillTropic();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// EL_TYPOON_MIS
class SkillTyphoonMissile : public SkillImpl {
public:
	SkillTyphoonMissile();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// EL_TYPOON_MIS_ATK
class SkillTyphoonMissileAttack : public SkillImpl {
public:
	SkillTyphoonMissileAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillUpheaval : public SkillImpl {
public:
	SkillUpheaval();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWaterBarrier : public SkillImpl {
public:
	SkillWaterBarrier();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWaterDrop : public SkillImpl {
public:
	SkillWaterDrop();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWaterScreen : public SkillImpl {
public:
	SkillWaterScreen();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// EL_WATER_SCREW
class SkillWaterScrew : public SkillImpl {
public:
	SkillWaterScrew();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// EL_WATER_SCREW_ATK
class SkillWaterScrewAttack : public SkillImpl {
public:
	SkillWaterScrewAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillWildStorm : public SkillImpl {
public:
	SkillWildStorm();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWindCurtain : public SkillImpl {
public:
	SkillWindCurtain();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWindSlasher : public SkillImpl {
public:
	SkillWindSlasher();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWindStep : public SkillImpl {
public:
	SkillWindStep();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillZephyr : public SkillImpl {
public:
	SkillZephyr();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
