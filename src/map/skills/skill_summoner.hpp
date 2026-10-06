// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "skill_factory.hpp"
#include "skill_impl.hpp"

class SkillFactorySummoner : public SkillFactory {
public:
	virtual std::unique_ptr<const SkillImpl> create(const e_skill skill_id) const override;
};

class SkillBite : public WeaponSkillImpl {
public:
	SkillBite();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillBlessingofMysticalCreatures : public SkillImpl {
public:
	SkillBlessingofMysticalCreatures();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBunchofShrimp : public SkillImpl {
public:
	SkillBunchofShrimp();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// SU_CN_METEOR
class SkillCatnipMeteor : public SkillImpl {
public:
	SkillCatnipMeteor();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};


// SU_CN_METEOR2
class SkillCatnipMeteor2 : public SkillImpl {
public:
	SkillCatnipMeteor2();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillCatnipPowdering : public SkillImpl {
public:
	SkillCatnipPowdering();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillChattering : public SkillImpl {
public:
	SkillChattering();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillChulhoBattering : public SkillImplRecursiveDamageSplash {
public:
	SkillChulhoBattering();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillChulhoSonicClaw : public WeaponSkillImpl {
public:
	SkillChulhoSonicClaw();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillColorsofHyunrok : public SkillImpl {
public:
	SkillColorsofHyunrok();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGrooming : public SkillImpl {
public:
	SkillGrooming();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHiss : public SkillImpl {
public:
	SkillHiss();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHogogongStrike : public SkillImpl {
public:
	SkillHogogongStrike();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHowlingofChulho : public SkillImpl {
public:
	SkillHowlingofChulho();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHyunrokBreeze : public SkillImpl {
public:
	SkillHyunrokBreeze();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillHyunrokCannon : public SkillImpl {
public:
	SkillHyunrokCannon();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillHyunrokSpiritPower : public SkillImplRecursiveDamageSplash {
public:
	SkillHyunrokSpiritPower();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillKisulRampage : public SkillImpl {
public:
	SkillKisulRampage();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillKisulWaterSpraying : public SkillImpl {
public:
	SkillKisulWaterSpraying();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillLope : public SkillImpl {
public:
	SkillLope();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// SU_LUNATICCARROTBEAT
class SkillLunaticCarrotBeat : public SkillImplRecursiveDamageSplash {
public:
	SkillLunaticCarrotBeat();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// SU_LUNATICCARROTBEAT2
class SkillLunaticCarrotBeat2 : public SkillImplRecursiveDamageSplash {
public:
	SkillLunaticCarrotBeat2();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillMarineFestivalofKisul : public SkillImpl {
public:
	SkillMarineFestivalofKisul();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMeowMeow : public SkillImpl {
public:
	SkillMeowMeow();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillNyangGrass : public SkillImpl {
public:
	SkillNyangGrass();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

// SU_PICKYPECK
class SkillPickyPeck : public WeaponSkillImpl {
public:
	SkillPickyPeck();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// SU_PICKYPECK_DOUBLE_ATK
class SkillPickyPeckDoubleAttack : public SkillImpl {
public:
	SkillPickyPeckDoubleAttack();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillPowerofFlock : public SkillImpl {
public:
	SkillPowerofFlock();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPurring : public SkillImpl {
public:
	SkillPurring();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSandyFestivalofKisul : public SkillImpl {
public:
	SkillSandyFestivalofKisul();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillScarofTarou : public WeaponSkillImpl {
public:
	SkillScarofTarou();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillScratch : public SkillImplRecursiveDamageSplash {
public:
	SkillScratch();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillSilvervineRootTwist : public SkillImpl {
public:
	SkillSilvervineRootTwist();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSilvervineStemSpear : public SkillImpl {
public:
	SkillSilvervineStemSpear();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSpiritofSavage : public SkillImpl {
public:
	SkillSpiritofSavage();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTastyShrimpParty : public SkillImpl {
public:
	SkillTastyShrimpParty();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTunaBelly : public SkillImpl {
public:
	SkillTunaBelly();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTunaParty : public SkillImpl {
public:
	SkillTunaParty();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
