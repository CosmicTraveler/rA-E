// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "skill_factory.hpp"
#include "skill_impl.hpp"
#include "map/battle.hpp"

class SkillFactoryTaekwon : public SkillFactory {
public:
	virtual std::unique_ptr<const SkillImpl> create(const e_skill skill_id) const override;
};

class SkillAllInTheSky : public SkillImpl {
public:
	SkillAllInTheSky();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
};

class SkillBookofCreatingStar : public SkillImpl {
public:
	SkillBookofCreatingStar();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCircleOfDirectionsAndElementals : public SkillImplRecursiveDamageSplash {
public:
	SkillCircleOfDirectionsAndElementals();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillCounter : public WeaponSkillImpl {
public:
	SkillCounter();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillCurseExplosion : public SkillImplRecursiveDamageSplash {
public:
	SkillCurseExplosion();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillDawnBreak : public SkillImplRecursiveDamageSplash {
public:
	SkillDawnBreak();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillDocumentofSunMoonAndStar : public SkillImpl {
public:
	SkillDocumentofSunMoonAndStar();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDownKick : public WeaponSkillImpl {
public:
	SkillDownKick();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillEsha : public SkillImplRecursiveDamageSplash {
public:
	SkillEsha();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	int64 splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillEska : public StatusSkillImpl {
public:
	SkillEska();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEske : public StatusSkillImpl {
public:
	SkillEske();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEsma : public SkillImpl {
public:
	SkillEsma();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEspa : public SkillImpl {
public:
	SkillEspa();

	void applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEstin : public SkillImpl {
public:
	SkillEstin();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEstun : public SkillImpl {
public:
	SkillEstun();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEswhoo : public SkillImplRecursiveDamageSplash {
public:
	SkillEswhoo();

	void applyCounterAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& attack_type) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	int64 splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillEswoo : public StatusSkillImpl {
public:
	SkillEswoo();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillExorcismOfMaliciousSoul : public SkillImplRecursiveDamageSplash {
public:
	SkillExorcismOfMaliciousSoul();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFairysSoul : public SkillImpl {
public:
	SkillFairysSoul();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFalconsSoul : public SkillImpl {
public:
	SkillFalconsSoul();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// SJ_FALLINGSTAR
class SkillFallingStar : public StatusSkillImpl {
public:
	SkillFallingStar();
};


// SJ_FALLINGSTAR_ATK2
class SkillFallingStarAttack : public SkillImplRecursiveDamageSplash {
public:
	SkillFallingStarAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	int64 splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// SJ_FALLINGSTAR_ATK2
class SkillFallingStarAttack2 : public SkillImplRecursiveDamageSplash {
public:
	SkillFallingStarAttack2();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillFeelingtheSunMoonandStars : public SkillImpl {
public:
	SkillFeelingtheSunMoonandStars();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFlashKick : public SkillImpl {
public:
	SkillFlashKick();

	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFullMoonKick : public SkillImplRecursiveDamageSplash {
public:
	SkillFullMoonKick();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGolemsSoul : public SkillImpl {
public:
	SkillGolemsSoul();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGravityControl : public SkillImpl {
public:
	SkillGravityControl();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHatredoftheSunMoonandStars : public SkillImpl {
public:
	SkillHatredoftheSunMoonandStars();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHighJump : public SkillImpl {
public:
	SkillHighJump();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillJumpKick : public SkillImpl {
public:
	SkillJumpKick();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillKaahi : public StatusSkillImpl {
public:
	SkillKaahi();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillKaite : public StatusSkillImpl {
public:
	SkillKaite();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillKaizel : public StatusSkillImpl {
public:
	SkillKaizel();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillKaupe : public StatusSkillImpl {
public:
	SkillKaupe();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillKaute : public SkillImpl {
public:
	SkillKaute();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMidnightKick : public SkillImplRecursiveDamageSplash {
public:
	SkillMidnightKick();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillMission : public SkillImpl {
public:
	SkillMission();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillNewMoonKick : public SkillImplRecursiveDamageSplash {
public:
	SkillNewMoonKick();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillNoonBlast : public SkillImplRecursiveDamageSplash {
public:
	SkillNoonBlast();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillNovaExplosion : public SkillImpl {
public:
	SkillNovaExplosion();

	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillProminenceKick : public SkillImplRecursiveDamageSplash {
public:
	SkillProminenceKick();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	int64 splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillRisingMoon : public SkillImplRecursiveDamageSplash {
public:
	SkillRisingMoon();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillRisingSun : public SkillImpl {
public:
	SkillRisingSun();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillRun : public SkillImpl {
public:
	SkillRun();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillSevenWind : public SkillImpl {
public:
	SkillSevenWind();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillShadowsSoul : public SkillImpl {
public:
	SkillShadowsSoul();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSkyMoon : public SkillImplRecursiveDamageSplash {
public:
	SkillSkyMoon();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillSkySun : public SkillImplRecursiveDamageSplash {
public:
	SkillSkySun();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillSolarBurst : public SkillImplRecursiveDamageSplash {
public:
	SkillSolarBurst();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulCollect : public SkillImpl {
public:
	SkillSoulCollect();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulCurse : public SkillImpl {
public:
	SkillSoulCurse();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulDivision : public StatusSkillImpl {
public:
	SkillSoulDivision();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulExplosion : public SkillImpl {
public:
	SkillSoulExplosion();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulGathering : public SkillImpl {
public:
	SkillSoulGathering();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulOfHeavenAndEarth : public SkillImpl {
public:
	SkillSoulOfHeavenAndEarth();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulRevolution : public SkillImpl {
public:
	SkillSoulRevolution();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSoulUnity : public SkillImpl {
public:
	SkillSoulUnity();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritofRebirth : public SkillImpl {
public:
	SkillSpiritofRebirth();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheAlchemist : public SkillImpl {
public:
	SkillSpiritoftheAlchemist();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheArtist : public SkillImpl {
public:
	SkillSpiritoftheArtist();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheAssasin : public SkillImpl {
public:
	SkillSpiritoftheAssasin();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheBlacksmith : public SkillImpl {
public:
	SkillSpiritoftheBlacksmith();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheCrusader : public SkillImpl {
public:
	SkillSpiritoftheCrusader();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheHunter : public SkillImpl {
public:
	SkillSpiritoftheHunter();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheKnight : public SkillImpl {
public:
	SkillSpiritoftheKnight();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheMonk : public SkillImpl {
public:
	SkillSpiritoftheMonk();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritofthePriest : public SkillImpl {
public:
	SkillSpiritofthePriest();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheRogue : public SkillImpl {
public:
	SkillSpiritoftheRogue();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheSage : public SkillImpl {
public:
	SkillSpiritoftheSage();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheSoulLinker : public SkillImpl {
public:
	SkillSpiritoftheSoulLinker();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheStarGladiator : public SkillImpl {
public:
	SkillSpiritoftheStarGladiator();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheSupernovice : public SkillImpl {
public:
	SkillSpiritoftheSupernovice();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritoftheWizard : public SkillImpl {
public:
	SkillSpiritoftheWizard();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStarBurst : public SkillImpl {
public:
	SkillStarBurst();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillStarCannon : public SkillImpl {
public:
	SkillStarCannon();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillStarEmperorAdvent : public SkillImplRecursiveDamageSplash {
public:
	SkillStarEmperorAdvent();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStarLightKick : public SkillImplRecursiveDamageSplash {
public:
	SkillStarLightKick();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillStormKick : public SkillImpl {
public:
	SkillStormKick();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillSunsetBlast : public SkillImplRecursiveDamageSplash {
public:
	SkillSunsetBlast();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillTalismanOfBlackTortoise : public SkillImpl {
public:
	SkillTalismanOfBlackTortoise();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTalismanOfBlueDragon : public SkillImpl {
public:
	SkillTalismanOfBlueDragon();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTalismanOfFiveElements : public SkillImpl {
public:
	SkillTalismanOfFiveElements();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTalismanOfFourBearingGod : public SkillImplRecursiveDamageSplash {
public:
	SkillTalismanOfFourBearingGod();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillTalismanOfMagician : public SkillImpl {
public:
	SkillTalismanOfMagician();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTalismanOfProtection : public SkillImpl {
public:
	SkillTalismanOfProtection();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTalismanOfRedPhoenix : public SkillImplRecursiveDamageSplash {
public:
	SkillTalismanOfRedPhoenix();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillTalismanOfSoulStealing : public SkillImpl {
public:
	SkillTalismanOfSoulStealing();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTalismanOfWarrior : public SkillImpl {
public:
	SkillTalismanOfWarrior();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTalismanOfWhiteTiger : public SkillImplRecursiveDamageSplash {
public:
	SkillTalismanOfWhiteTiger();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTotemOfTutelary : public SkillImpl {
public:
	SkillTotemOfTutelary();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTurnKick : public SkillImpl {
public:
	SkillTurnKick();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const override;
};

class SkillTwinklingGalaxy : public SkillImpl {
public:
	SkillTwinklingGalaxy();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillWarmthoftheMoon : public SkillImpl {
public:
	SkillWarmthoftheMoon();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWarmthoftheStars : public SkillImpl {
public:
	SkillWarmthoftheStars();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWarmthoftheSun : public SkillImpl {
public:
	SkillWarmthoftheSun();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
