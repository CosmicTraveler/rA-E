// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "skill_factory.hpp"
#include "skill_impl.hpp"
#include "map/battle.hpp"

class SkillFactoryMerchant : public SkillFactory {
public:
	virtual std::unique_ptr<const SkillImpl> create(const e_skill skill_id) const override;
};

class SkillAbrBattleWarrior : public SkillImpl {
public:
	SkillAbrBattleWarrior();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAbrDualCannon : public SkillImpl {
public:
	SkillAbrDualCannon();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAbrInfinity : public SkillImpl {
public:
	SkillAbrInfinity();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAbrMotherNet : public SkillImpl {
public:
	SkillAbrMotherNet();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAcidDemonstration : public WeaponSkillImpl {
public:
	SkillAcidDemonstration();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// BO_ACIDIFIED_ZONE_FIRE
class SkillAcidifiedZoneFire : public SkillImplRecursiveDamageSplash {
public:
	SkillAcidifiedZoneFire();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};


// BO_ACIDIFIED_ZONE_FIRE_ATK
class SkillActifiedZoneFireAttack : public WeaponSkillImpl {
public:
	SkillActifiedZoneFireAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

// BO_ACIDIFIED_ZONE_GROUND
class SkillAcidifiedZoneGround : public SkillImplRecursiveDamageSplash {
public:
	SkillAcidifiedZoneGround();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};


// BO_ACIDIFIED_ZONE_GROUND_ATK
class SkillActifiedZoneGroundAttack : public WeaponSkillImpl {
public:
	SkillActifiedZoneGroundAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

// BO_ACIDIFIED_ZONE_WATER
class SkillAcidifiedZoneWater : public SkillImplRecursiveDamageSplash {
public:
	SkillAcidifiedZoneWater();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};


// BO_ACIDIFIED_ZONE_WATER_ATK
class SkillActifiedZoneWaterAttack : public WeaponSkillImpl {
public:
	SkillActifiedZoneWaterAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

// BO_ACIDIFIED_ZONE_WIND
class SkillAcidifiedZoneWind : public SkillImplRecursiveDamageSplash {
public:
	SkillAcidifiedZoneWind();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};


// BO_ACIDIFIED_ZONE_WIND_ATK
class SkillActifiedZoneWindAttack : public WeaponSkillImpl {
public:
	SkillActifiedZoneWindAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillAcidTerror : public WeaponSkillImpl {
public:
	SkillAcidTerror();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillAdrenalineRush : public SkillImpl {
public:
	SkillAdrenalineRush();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAdvancedAdrenalineRush : public SkillImpl {
public:
	SkillAdvancedAdrenalineRush();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAdvanceProtection : public StatusSkillImpl {
public:
	SkillAdvanceProtection();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAidBerserkPotion : public SkillImpl {
public:
	SkillAidBerserkPotion();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAidCondensedPotion : public SkillImpl {
public:
	SkillAidCondensedPotion();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAidPotion : public SkillImpl {
public:
	SkillAidPotion();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAlchemicalWeapon : public SkillImpl {
public:
	SkillAlchemicalWeapon();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAnalyze : public SkillImpl {
public:
	SkillAnalyze();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillArmCannon : public SkillImplRecursiveDamageSplash {
public:
	SkillArmCannon();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	int32 getSplashTarget(block_list* src) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillAttackMachine : public SkillImplRecursiveDamageSplash {
public:
	SkillAttackMachine();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillAxeBoomerang : public WeaponSkillImpl {
public:
	SkillAxeBoomerang();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillAxeStomp : public SkillImplRecursiveDamageSplash {
public:
	SkillAxeStomp();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillAxeTornado : public SkillImplRecursiveDamageSplash {
public:
	SkillAxeTornado();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBackSideSlide : public SkillImpl {
public:
	SkillBackSideSlide();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBiochemicalHelm : public SkillImpl {
public:
	SkillBiochemicalHelm();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBionicPharmacy : public SkillImpl {
public:
	SkillBionicPharmacy();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBomb : public SkillImpl {
public:
	SkillBomb();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillBoostKnuckle : public WeaponSkillImpl {
public:
	SkillBoostKnuckle();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillCallHomunculus : public SkillImpl {
public:
	SkillCallHomunculus();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCartCannon : public SkillImplRecursiveDamageSplash {
public:
	SkillCartCannon();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillCartRevolution : public SkillImplRecursiveDamageSplash {
public:
	SkillCartRevolution();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void modifyHitRate(int16& hit_rate, const block_list *src, const block_list *target, uint16 skill_lv) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCartTermination : public WeaponSkillImpl {
public:
	SkillCartTermination();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillCartTornado : public SkillImplRecursiveDamageSplash {
public:
	SkillCartTornado();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const override;
};

class SkillChangeCart : public SkillImpl {
public:
	SkillChangeCart();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillChangeMaterial : public SkillImpl {
public:
	SkillChangeMaterial();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillColdSlower : public SkillImplRecursiveDamageSplash {
public:
	SkillColdSlower();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCrazyUproar : public StatusSkillImpl {
public:
	SkillCrazyUproar();

#ifdef RENEWAL
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
#endif
};

// GN_CRAZYWEED
class SkillCrazyWeed : public SkillImpl {
public:
	SkillCrazyWeed();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// GN_CRAZYWEED_ATK
class SkillCrazyWeedAttack : public SkillImpl {
public:
	SkillCrazyWeedAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillCreateBomb : public SkillImpl {
public:
	SkillCreateBomb();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCreeper : public SkillImpl {
public:
	SkillCreeper();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDecorateCart : public SkillImpl {
public:
	SkillDecorateCart();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDemonicFire : public SkillImplRecursiveDamageSplash {
public:
	SkillDemonicFire();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDustExplosion : public SkillImplRecursiveDamageSplash {
public:
	SkillDustExplosion();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillEmergencyCool : public SkillImpl {
public:
	SkillEmergencyCool();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEnergyCannonade : public SkillImplRecursiveDamageSplash {
public:
	SkillEnergyCannonade();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillExplosivePowder : public SkillImplRecursiveDamageSplash {
public:
	SkillExplosivePowder();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFawMagicDecoy : public SkillImpl {
public:
	SkillFawMagicDecoy();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFawRemoval : public SkillImpl {
public:
	SkillFawRemoval();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFawSilverSniper : public SkillImpl {
public:
	SkillFawSilverSniper();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// GN_FIRE_EXPANSION
class SkillFireExpansion : public SkillImpl {
public:
	SkillFireExpansion();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// GN_FIRE_EXPANSION_ACID
class SkillFireExpansionAcid : public SkillImplRecursiveDamageSplash {
public:
	SkillFireExpansionAcid();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillFlameLauncher : public SkillImpl {
public:
	SkillFlameLauncher();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFrontSideSlide : public SkillImpl {
public:
	SkillFrontSideSlide();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFullProtection : public SkillImpl {
public:
	SkillFullProtection();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGreed : public SkillImpl {
public:
	SkillGreed();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHammerFall : public SkillImpl {
public:
	SkillHammerFall();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// GN_HELLS_PLANT
class SkillHellsPlant : public StatusSkillImpl {
public:
	SkillHellsPlant();
};


// GN_HELLS_PLANT_ATK
class SkillHellsPlantAttack : public SkillImplRecursiveDamageSplash {
public:
	SkillHellsPlantAttack();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillHellTree : public SkillImpl {
public:
	SkillHellTree();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHomunculusResurrection : public SkillImpl {
public:
	SkillHomunculusResurrection();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHowlingOfMandragora : public SkillImpl {
public:
	SkillHowlingOfMandragora();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillIllusionDoping : public SkillImplRecursiveDamageSplash {
public:
	SkillIllusionDoping();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillInfraredScan : public SkillImpl {
public:
	SkillInfraredScan();

	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillItemAppraisal : public SkillImpl {
public:
	SkillItemAppraisal();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// NC_MAGMA_ERUPTION
class SkillMagmaEruption : public WeaponSkillImpl {
public:
	SkillMagmaEruption();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// NC_MAGMA_ERUPTION_DOTDAMAGE
class SkillMagmaEruptionDotDamage : public SkillImpl {
public:
	SkillMagmaEruptionDotDamage();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillMagneticField : public SkillImpl {
public:
	SkillMagneticField();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMammonite : public WeaponSkillImpl {
public:
	SkillMammonite();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillManufactureMachine : public SkillImpl {
public:
	SkillManufactureMachine();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMayhemicThorns : public SkillImplRecursiveDamageSplash {
public:
	SkillMayhemicThorns();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillMightySmash : public SkillImplRecursiveDamageSplash {
public:
	SkillMightySmash();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillMixCooking : public SkillImpl {
public:
	SkillMixCooking();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMysteryPowder : public SkillImplRecursiveDamageSplash {
public:
	SkillMysteryPowder();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillNeutralBarrier : public SkillImpl {
public:
	SkillNeutralBarrier();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPileBunker : public WeaponSkillImpl {
public:
	SkillPileBunker();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillPlantCultivation : public SkillImpl {
public:
	SkillPlantCultivation();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPowerfulSwing : public SkillImplRecursiveDamageSplash {
public:
	SkillPowerfulSwing();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillPowerSwing : public WeaponSkillImpl {
public:
	SkillPowerSwing();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillPowerThrust : public SkillImpl {
public:
	SkillPowerThrust();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPreparePotion : public SkillImpl {
public:
	SkillPreparePotion();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRepair : public SkillImpl {
public:
	SkillRepair();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRushQuake : public SkillImplRecursiveDamageSplash {
public:
	SkillRushQuake();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillRushStrike : public SkillImplRecursiveDamageSplash {
public:
	SkillRushStrike();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillSelfDestruction : public SkillImplRecursiveDamageSplash {
public:
	SkillSelfDestruction();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillVending : public SkillImpl {
public:
	SkillVending();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// GN_SLINGITEM
class SkillSlingItem : public SkillImpl {
public:
	SkillSlingItem();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// GN_SLINGITEM_RANGEMELEEATK
class SkillSlingItemAttack : public WeaponSkillImpl {
public:
	SkillSlingItemAttack();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillSparkBlaster : public SkillImplRecursiveDamageSplash {
public:
	SkillSparkBlaster();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillSpecialPharmacy : public SkillImpl {
public:
	SkillSpecialPharmacy();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSporeExplosion : public SkillImplRecursiveDamageSplash {
public:
	SkillSporeExplosion();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillStealthField : public SkillImpl {
public:
	SkillStealthField();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummonFlora : public SkillImpl {
public:
	SkillSummonFlora();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummonMarineSphere : public SkillImpl {
public:
	SkillSummonMarineSphere();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSynthesizedShield : public SkillImpl {
public:
	SkillSynthesizedShield();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSyntheticArmor : public SkillImpl {
public:
	SkillSyntheticArmor();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTheWholeProtection : public SkillImpl {
public:
	SkillTheWholeProtection();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillThornTrap : public SkillImpl {
public:
	SkillThornTrap();

	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTripleLaser : public WeaponSkillImpl {
public:
	SkillTripleLaser();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillTwilightAlchemy1 : public SkillImpl {
public:
	SkillTwilightAlchemy1();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTwilightAlchemy2 : public SkillImpl {
public:
	SkillTwilightAlchemy2();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTwilightAlchemy3 : public SkillImpl {
public:
	SkillTwilightAlchemy3();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillUpgradeWeapon : public SkillImpl {
public:
	SkillUpgradeWeapon();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillVaporize : public SkillImpl {
public:
	SkillVaporize();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillVulcanArm : public SkillImplRecursiveDamageSplash {
public:
	SkillVulcanArm();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillWallOfThorns : public SkillImpl {
public:
	SkillWallOfThorns();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWeaponPerfection : public SkillImpl {
public:
	SkillWeaponPerfection();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWeaponRepair : public SkillImpl {
public:
	SkillWeaponRepair();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWoodenFairy : public SkillImpl {
public:
	SkillWoodenFairy();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWoodenWarrior : public SkillImpl {
public:
	SkillWoodenWarrior();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
