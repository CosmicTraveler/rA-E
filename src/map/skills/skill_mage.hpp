// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "skill_factory.hpp"
#include "skill_impl.hpp"
#include "map/battle.hpp"

class SkillFactoryMage : public SkillFactory {
public:
	virtual std::unique_ptr<const SkillImpl> create(const e_skill skill_id) const override;
};

class SkillActivityBurn : public SkillImpl {
public:
	SkillActivityBurn();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// AG_ALL_BLOOM
class SkillAllBloom : public SkillImpl {
public:
	SkillAllBloom();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// AG_ALL_BLOOM_ATK
class SkillAllBloomAttack : public SkillImpl {
public:
	SkillAllBloomAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};


// AG_ALL_BLOOM_ATK2
class SkillAllBloomAttack2 : public SkillImpl {
public:
	SkillAllBloomAttack2();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillArrullo : public SkillImpl {
public:
	SkillArrullo();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// AG_ASTRAL_STRIKE
class SkillAstralStrike : public SkillImpl {
public:
	SkillAstralStrike();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// AG_ASTRAL_STRIKE_ATK
class SkillAstralStrikeAttack : public SkillImpl {
public:
	SkillAstralStrikeAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBeastlyHypnosis : public SkillImpl {
public:
	SkillBeastlyHypnosis();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBlindingMist : public SkillImpl {
public:
	SkillBlindingMist();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCastCancel : public SkillImpl {
public:
	SkillCastCancel();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// WL_CHAINLIGHTNING
class SkillChainLightning : public SkillImpl {
public:
	SkillChainLightning();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// WL_CHAINLIGHTNING_ATK
class SkillChainLightningAttack : public SkillImpl {
public:
	SkillChainLightningAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillClassChange : public SkillImpl {
public:
	SkillClassChange();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCloudKill : public SkillImpl {
public:
	SkillCloudKill();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillColdBolt : public SkillImpl {
public:
	SkillColdBolt();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
};

class SkillComa : public SkillImpl {
public:
	SkillComa();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillComet : public SkillImpl {
public:
	SkillComet();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillConflagration : public SkillImpl {
public:
	SkillConflagration();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCreateElementalConverter : public SkillImpl {
public:
	SkillCreateElementalConverter();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// AG_CRIMSON_ARROW
class SkillCrimsonArrow : public SkillImpl {
public:
	SkillCrimsonArrow();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// AG_CRIMSON_ARROW_ATK
class SkillCrimsonArrowAttack : public SkillImplRecursiveDamageSplash {
public:
	SkillCrimsonArrowAttack();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillCrimsonRock : public SkillImplRecursiveDamageSplash {
public:
	SkillCrimsonRock();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

// AG_CRYSTAL_IMPACT
class SkillCrystalImpact : public SkillImplRecursiveDamageSplash {
public:
	SkillCrystalImpact();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// AG_CRYSTAL_IMPACT_ATK
class SkillCrystalImpactAttack : public SkillImplRecursiveDamageSplash {
public:
	SkillCrystalImpactAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	int16 getSearchSize(block_list* src, uint16 skill_lv) const override;
	int16 getSplashSearchSize(block_list* src, uint16 skill_lv) const override;
};

class SkillDeadlyProjection : public SkillImpl {
public:
	SkillDeadlyProjection();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDeluge : public SkillImpl {
public:
	SkillDeluge();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// AG_DESTRUCTIVE_HURRICANE
class SkillDestructiveHurricane : public SkillImplRecursiveDamageSplash {
public:
	SkillDestructiveHurricane();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// AG_DESTRUCTIVE_HURRICANE_CLIMAX
class SkillDestructiveHurricaneClimax : public SkillImpl {
public:
	SkillDestructiveHurricaneClimax();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDiamondDust : public SkillImpl {
public:
	SkillDiamondDust();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDiamondStorm : public SkillImpl {
public:
	SkillDiamondStorm();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDispell : public SkillImpl {
public:
	SkillDispell();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDrainLife : public SkillImpl {
public:
	SkillDrainLife();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEarthGrave : public SkillImpl {
public:
	SkillEarthGrave();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEarthInsignia : public SkillImpl {
public:
	SkillEarthInsignia();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEarthSpike : public SkillImpl {
public:
	SkillEarthSpike();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillEarthStrain : public SkillImpl {
public:
	SkillEarthStrain();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillElectricWalk : public SkillImpl {
public:
	SkillElectricWalk();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillElementalAction : public SkillImpl {
public:
	SkillElementalAction();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// EM_ELEMENTAL_BUSTER
class SkillElementalBuster : public SkillImpl {
public:
	SkillElementalBuster();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// EM_ELEMENTAL_BUSTER_FIRE
class SkillElementalBusterFire : public SkillImplRecursiveDamageSplash {
public:
	SkillElementalBusterFire();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};


// EM_ELEMENTAL_BUSTER_GROUND
class SkillElementalBusterGround : public SkillImplRecursiveDamageSplash {
public:
	SkillElementalBusterGround();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};


// EM_ELEMENTAL_BUSTER_POISON
class SkillElementalBusterPoison : public SkillImplRecursiveDamageSplash {
public:
	SkillElementalBusterPoison();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};


// EM_ELEMENTAL_BUSTER_WATER
class SkillElementalBusterWater : public SkillImplRecursiveDamageSplash {
public:
	SkillElementalBusterWater();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};


// EM_ELEMENTAL_BUSTER_WIND
class SkillElementalBusterWind : public SkillImplRecursiveDamageSplash {
public:
	SkillElementalBusterWind();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillElementalChangeEarth : public SkillImpl {
public:
	SkillElementalChangeEarth();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillElementalChangeFire : public SkillImpl {
public:
	SkillElementalChangeFire();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillElementalChangeWater : public SkillImpl {
public:
	SkillElementalChangeWater();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillElementalChangeWind : public SkillImpl {
public:
	SkillElementalChangeWind();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillElementalShield : public SkillImpl {
public:
	SkillElementalShield();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillElementalVeil : public SkillImpl {
public:
	SkillElementalVeil();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEndowBlaze : public SkillImpl {
public:
	SkillEndowBlaze();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEndowQuake : public SkillImpl {
public:
	SkillEndowQuake();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEndowTornado : public SkillImpl {
public:
	SkillEndowTornado();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEndowTsunami : public SkillImpl {
public:
	SkillEndowTsunami();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEnergyCoat : public StatusSkillImpl {
public:
	SkillEnergyCoat();
};

class SkillEnergyConversion : public SkillImpl {
public:
	SkillEnergyConversion();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFiberLock : public SkillImpl {
public:
	SkillFiberLock();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFireBall : public SkillImplRecursiveDamageSplash {
public:
	SkillFireBall();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
	int64 splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillFireBolt : public SkillImpl {
public:
	SkillFireBolt();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
};

class SkillFireInsignia : public SkillImpl {
public:
	SkillFireInsignia();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFirePillar : public SkillImpl {
public:
	SkillFirePillar();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
};

class SkillFireWalk : public SkillImpl {
public:
	SkillFireWalk();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillFireWall : public SkillImpl {
public:
	SkillFireWall();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
};

class SkillFloralFlareRoad : public SkillImpl {
public:
	SkillFloralFlareRoad();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFourSpiritAnalysis : public SkillImpl {
public:
	SkillFourSpiritAnalysis();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFrostDiver : public SkillImpl {
public:
	SkillFrostDiver();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
	void applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillFrostNova : public SkillImpl {
public:
	SkillFrostNova();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillFrostyMisty : public SkillImpl {
public:
	SkillFrostyMisty();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFrozenSlash : public SkillImplRecursiveDamageSplash {
public:
	SkillFrozenSlash();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGanbantein : public SkillImpl {
public:
	SkillGanbantein();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGoldDigger : public SkillImpl {
public:
	SkillGoldDigger();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGravitationField : public SkillImpl {
public:
	SkillGravitationField();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGravity : public SkillImpl {
public:
	SkillGravity();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGrimReaper : public SkillImpl {
public:
	SkillGrimReaper();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHeavensDrive : public SkillImpl {
public:
	SkillHeavensDrive();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillHellInferno : public SkillImpl {
public:
	SkillHellInferno();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillHindsight : public SkillImpl {
public:
	SkillHindsight();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHocusPocus : public SkillImpl {
public:
	SkillHocusPocus();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillIceWall : public SkillImpl {
public:
	SkillIceWall();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillIncreasingActivity : public SkillImpl {
public:
	SkillIncreasingActivity();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillIndulge : public SkillImpl {
public:
	SkillIndulge();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillJackFrost : public SkillImplRecursiveDamageSplash {
public:
	SkillJackFrost();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillJupitelThunder : public SkillImpl {
public:
	SkillJupitelThunder();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillLeveling : public SkillImpl {
public:
	SkillLeveling();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillLightningBolt : public SkillImpl {
public:
	SkillLightningBolt();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
};

class SkillLightningLand : public SkillImpl {
public:
	SkillLightningLand();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillLordOfVermilion : public SkillImpl {
public:
	SkillLordOfVermilion();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillMagicRod : public SkillImpl {
public:
	SkillMagicRod();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMagneticEarth : public SkillImpl {
public:
	SkillMagneticEarth();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMeteorStorm : public SkillImpl {
public:
	SkillMeteorStorm();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillMindBreaker : public SkillImpl {
public:
	SkillMindBreaker();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMonocell : public SkillImpl {
public:
	SkillMonocell();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMonsterChant : public SkillImpl {
public:
	SkillMonsterChant();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMysteryIllusion : public SkillImpl {
public:
	SkillMysteryIllusion();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillNapalmBeat : public SkillImplRecursiveDamageSplash {
public:
	SkillNapalmBeat();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillNapalmVulcan : public SkillImplRecursiveDamageSplash {
public:
	SkillNapalmVulcan();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillPoisonBuster : public SkillImplRecursiveDamageSplash {
public:
	SkillPoisonBuster();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillPsychicStream : public SkillImplRecursiveDamageSplash {
public:
	SkillPsychicStream();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillPsychicWave : public SkillImpl {
public:
	SkillPsychicWave();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillQuagmire : public SkillImpl {
public:
	SkillQuagmire();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillQuestioning : public SkillImpl {
public:
	SkillQuestioning();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRainOfCrystal : public SkillImpl {
public:
	SkillRainOfCrystal();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillReadingSpellbook : public SkillImpl {
public:
	SkillReadingSpellbook();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRejuvenation : public SkillImpl {
public:
	SkillRejuvenation();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRelease : public SkillImpl {
public:
	SkillRelease();

	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRockDown : public SkillImplRecursiveDamageSplash {
public:
	SkillRockDown();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillSafetyWall : public SkillImpl {
public:
	SkillSafetyWall();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSense : public SkillImpl {
public:
	SkillSense();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSiennaExecrate : public SkillImpl {
public:
	SkillSiennaExecrate();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSight : public SkillImpl {
public:
	SkillSight();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillSightBlaster : public SkillImpl {
public:
	SkillSightBlaster();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillSightRasher : public SkillImpl {
public:
	SkillSightRasher();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillSoulExhale : public SkillImpl {
public:
	SkillSoulExhale();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulExpansion : public SkillImplRecursiveDamageSplash {
public:
	SkillSoulExpansion();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillSoulSiphon : public SkillImpl {
public:
	SkillSoulSiphon();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulStrike : public SkillImpl {
public:
	SkillSoulStrike();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillSoulVulcanStrike : public SkillImplRecursiveDamageSplash {
public:
	SkillSoulVulcanStrike();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillSpellBreaker : public SkillImpl {
public:
	SkillSpellBreaker();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpellFist : public SkillImpl {
public:
	SkillSpellFist();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritControl : public SkillImpl {
public:
	SkillSpiritControl();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritRecovery : public SkillImpl {
public:
	SkillSpiritRecovery();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStasis : public SkillImpl {
public:
	SkillStasis();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStoneCurse : public SkillImpl {
public:
	SkillStoneCurse();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillStormCannon : public SkillImpl {
public:
	SkillStormCannon();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStormGust : public SkillImpl {
public:
	SkillStormGust();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillStrantumTremor : public SkillImpl {
public:
	SkillStrantumTremor();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStriking : public SkillImpl {
public:
	SkillStriking();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSuicide : public SkillImpl {
public:
	SkillSuicide();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummonEarthSpiritTera : public SkillImpl {
public:
	SkillSummonEarthSpiritTera();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummonElementalArdor : public SkillImpl {
public:
	SkillSummonElementalArdor();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummonElementalDiluvio : public SkillImpl {
public:
	SkillSummonElementalDiluvio();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummonElementalProcella : public SkillImpl {
public:
	SkillSummonElementalProcella();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummonElementalSerpens : public SkillImpl {
public:
	SkillSummonElementalSerpens();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummonElementalTerremotus : public SkillImpl {
public:
	SkillSummonElementalTerremotus();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// WL_SUMMONFB
class SkillSummonFireBall : public SkillImpl {
public:
	SkillSummonFireBall();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// WL_SUMMON_ATK_FIRE
class SkillSummonAttackFire : public SkillImpl {
public:
	SkillSummonAttackFire();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillSummonFireSpiritAgni : public SkillImpl {
public:
	SkillSummonFireSpiritAgni();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// WL_SUMMONBL
class SkillSummonLightningBall : public SkillImpl {
public:
	SkillSummonLightningBall();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// WL_SUMMON_ATK_WIND
class SkillSummonAttackWind : public SkillImpl {
public:
	SkillSummonAttackWind();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

// WL_SUMMONSTONE
class SkillSummonStone : public SkillImpl {
public:
	SkillSummonStone();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// WL_SUMMON_ATK_GROUND
class SkillSummonAttackEarth : public SkillImpl {
public:
	SkillSummonAttackEarth();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

// WL_SUMMONWB
class SkillSummonWaterBall : public SkillImpl {
public:
	SkillSummonWaterBall();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// WL_SUMMON_ATK_WATER
class SkillSummonAttackWater : public SkillImpl {
public:
	SkillSummonAttackWater();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillSummonWaterSpiritAqua : public SkillImpl {
public:
	SkillSummonWaterSpiritAqua();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummonWindSpiritVentus : public SkillImpl {
public:
	SkillSummonWindSpiritVentus();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTerraDrive : public SkillImpl {
public:
	SkillTerraDrive();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// WL_TETRAVORTEX
class SkillTetraVortex : public SkillImpl {
public:
	SkillTetraVortex();

	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// WL_TETRAVORTEX_GROUND
class SkillTetraVortexEarth : public SkillImpl {
public:
	SkillTetraVortexEarth();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// WL_TETRAVORTEX_FIRE
class SkillTetraVortexFire : public SkillImpl {
public:
	SkillTetraVortexFire();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// WL_TETRAVORTEX_WATER
class SkillTetraVortexWater : public SkillImpl {
public:
	SkillTetraVortexWater();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// WL_TETRAVORTEX_WIND
class SkillTetraVortexWind : public SkillImpl {
public:
	SkillTetraVortexWind();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillThunderStorm : public SkillImpl {
public:
	SkillThunderStorm();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillTornadoStorm : public SkillImpl {
public:
	SkillTornadoStorm();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillVacuumExtreme : public SkillImpl {
public:
	SkillVacuumExtreme();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillVaretyrSpear : public SkillImplRecursiveDamageSplash {
public:
	SkillVaretyrSpear();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillVenomSwamp : public SkillImpl {
public:
	SkillVenomSwamp();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// AG_VIOLENT_QUAKE
class SkillViolentQuake : public SkillImpl {
public:
	SkillViolentQuake();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// AG_VIOLENT_QUAKE_ATK
class SkillViolentQuakeAttack : public SkillImpl {
public:
	SkillViolentQuakeAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillVolcano : public SkillImpl {
public:
	SkillVolcano();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWarmer : public SkillImpl {
public:
	SkillWarmer();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWaterBall : public SkillImpl {
public:
	SkillWaterBall();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillWaterInsignia : public SkillImpl {
public:
	SkillWaterInsignia();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWhirlwind : public SkillImpl {
public:
	SkillWhirlwind();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWhiteImprison : public SkillImpl {
public:
	SkillWhiteImprison();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWindInsignia : public SkillImpl {
public:
	SkillWindInsignia();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
