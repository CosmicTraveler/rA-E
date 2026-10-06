// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "skill_factory.hpp"
#include "skill_impl.hpp"

class SkillFactoryOther : public SkillFactory {
public:
	virtual std::unique_ptr<const SkillImpl> create(const e_skill skill_id) const override;
};

class SkillBaby : public SkillImpl {
public:
	SkillBaby();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBattleBuster : public WeaponSkillImpl {
public:
	SkillBattleBuster();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillCallAllFamily : public SkillImpl {
public:
	SkillCallAllFamily();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCallBaby : public SkillImpl {
public:
	SkillCallBaby();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCallParent : public SkillImpl {
public:
	SkillCallParent();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCatCry : public SkillImpl {
public:
	SkillCatCry();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCheerUp : public StatusSkillImpl {
public:
	SkillCheerUp();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillChristmasCarol : public SkillImpl {
public:
	SkillChristmasCarol();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDualCannonFire : public WeaponSkillImpl {
public:
	SkillDualCannonFire();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillEquipSwitch : public SkillImpl {
public:
	SkillEquipSwitch();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGmSandman : public SkillImpl {
public:
	SkillGmSandman();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGuardiansRecall : public SkillImpl {
public:
	SkillGuardiansRecall();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillILookUpToYou : public SkillImpl {
public:
	SkillILookUpToYou();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillIMissYou : public SkillImpl {
public:
	SkillIMissYou();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillInfinityBuster : public WeaponSkillImpl {
public:
	SkillInfinityBuster();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillIWillProtectYou : public SkillImpl {
public:
	SkillIWillProtectYou();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillNetRepair : public SkillImpl {
public:
	SkillNetRepair();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillNetSupport : public SkillImpl {
public:
	SkillNetSupport();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillNiflheimRecall : public SkillImpl {
public:
	SkillNiflheimRecall();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillOdinsRecall : public SkillImpl {
public:
	SkillOdinsRecall();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillOneForever : public SkillImpl {
public:
	SkillOneForever();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillOpenBuyingStore : public SkillImpl {
public:
	SkillOpenBuyingStore();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPartyAssumptio : public SkillImpl {
public:
	SkillPartyAssumptio();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPartyBlessing : public SkillImpl {
public:
	SkillPartyBlessing();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPartyFlee : public StatusSkillImpl {
public:
	SkillPartyFlee();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPartyIncreaseAgi : public SkillImpl {
public:
	SkillPartyIncreaseAgi();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPeonyMamy : public SkillImpl {
public:
	SkillPeonyMamy();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPronteraRecall : public SkillImpl {
public:
	SkillPronteraRecall();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRayOfProtection : public SkillImpl {
public:
	SkillRayOfProtection();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillReturnToEclage : public SkillImpl {
public:
	SkillReturnToEclage();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillReturnToEldicastes : public SkillImpl {
public:
	SkillReturnToEldicastes();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillReturnToGlastHeim : public SkillImpl {
public:
	SkillReturnToGlastHeim();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillReturnToLighthalzen : public SkillImpl {
public:
	SkillReturnToLighthalzen();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillReturnToThanatos : public SkillImpl {
public:
	SkillReturnToThanatos();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRo20thAnniversaryFirecracker : public SkillImpl {
public:
	SkillRo20thAnniversaryFirecracker();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSadagui : public SkillImpl {
public:
	SkillSadagui();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSequoiaDust : public SkillImpl {
public:
	SkillSequoiaDust();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSnowFlip : public SkillImpl {
public:
	SkillSnowFlip();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSummerNightDream : public SkillImpl {
public:
	SkillSummerNightDream();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWeaponEnchantment : public SkillImpl {
public:
	SkillWeaponEnchantment();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
