// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "skill_factory.hpp"
#include "skill_impl.hpp"
#include "map/battle.hpp"

class SkillFactoryMercenary : public SkillFactory {
public:
	virtual std::unique_ptr<const SkillImpl> create(const e_skill skill_id) const override;
};

class SkillMercenaryArrowRepel : public WeaponSkillImpl {
public:
	SkillMercenaryArrowRepel();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillMercenaryArrowShower : public SkillImplRecursiveDamageSplash {
public:
	SkillMercenaryArrowShower();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillMercenaryBash : public WeaponSkillImpl {
public:
	SkillMercenaryBash();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const override;
};

class SkillMercenaryBenediction : public SkillImpl {
public:
	SkillMercenaryBenediction();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryBlessing : public SkillImpl {
public:
	SkillMercenaryBlessing();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryBowlingBash : public WeaponSkillImpl {
public:
	SkillMercenaryBowlingBash();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryBrandishSpear : public SkillImpl {
public:
	SkillMercenaryBrandishSpear();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryCompress : public SkillImpl {
public:
	SkillMercenaryCompress();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryCrash : public WeaponSkillImpl {
public:
	SkillMercenaryCrash();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillMercenaryDecreaseAgi : public SkillImpl {
public:
	SkillMercenaryDecreaseAgi();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryDoubleStrafe : public WeaponSkillImpl {
public:
	SkillMercenaryDoubleStrafe();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillMercenaryFocusedArrowStrike : public SkillImpl {
public:
	SkillMercenaryFocusedArrowStrike();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryFreezingTrap : public SkillImpl {
public:
	SkillMercenaryFreezingTrap();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryIncreaseAgility : public SkillImpl {
public:
	SkillMercenaryIncreaseAgility();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryKyrieEleison : public SkillImpl {
public:
	SkillMercenaryKyrieEleison();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryLandMine : public SkillImpl {
public:
	SkillMercenaryLandMine();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryLexDivina : public SkillImpl {
public:
	SkillMercenaryLexDivina();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryMagnificat : public SkillImpl {
public:
	SkillMercenaryMagnificat();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryMagnumBreak : public SkillImpl {
public:
	SkillMercenaryMagnumBreak();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const override;
};

class SkillMercenaryMentalCure : public SkillImpl {
public:
	SkillMercenaryMentalCure();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryMindBlaster : public SkillImpl {
public:
	SkillMercenaryMindBlaster();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryPierce : public WeaponSkillImpl {
public:
	SkillMercenaryPierce();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const override;
};

class SkillMercenaryProvoke : public SkillImpl {
public:
	SkillMercenaryProvoke();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryRecuperate : public SkillImpl {
public:
	SkillMercenaryRecuperate();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryRegain : public SkillImpl {
public:
	SkillMercenaryRegain();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryRemoveTrap : public SkillImpl {
public:
	SkillMercenaryRemoveTrap();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenarySacrifice : public SkillImpl {
public:
	SkillMercenarySacrifice();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenarySandman : public SkillImpl {
public:
	SkillMercenarySandman();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryScapegoat : public SkillImpl {
public:
	SkillMercenaryScapegoat();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenarySense : public SkillImpl {
public:
	SkillMercenarySense();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenaryShieldReflect : public StatusSkillImpl {
public:
	SkillMercenaryShieldReflect();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenarySight : public SkillImpl {
public:
	SkillMercenarySight();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenarySkidTrap : public SkillImpl {
public:
	SkillMercenarySkidTrap();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMercenarySpiralPierce : public WeaponSkillImpl {
public:
	SkillMercenarySpiralPierce();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillMercenaryTender : public SkillImpl {
public:
	SkillMercenaryTender();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
