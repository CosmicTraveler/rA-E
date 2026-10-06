// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "skill_factory.hpp"
#include "skill_impl.hpp"
#include <config/core.hpp>

class SkillFactoryAcolyte : public SkillFactory {
public:
	virtual std::unique_ptr<const SkillImpl> create(const e_skill skill_id) const override;
};

class SkillAbsorbSpiritSphere : public SkillImpl {
public:
	SkillAbsorbSpiritSphere();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAdoramus : public SkillImplRecursiveDamageSplash {
public:
	SkillAdoramus();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	int64 splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillAncilla : public SkillImpl {
public:
	SkillAncilla();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAngelus : public SkillImpl {
public:
	SkillAngelus();
	
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillArbitrium : public SkillImplRecursiveDamageSplash {
public:
	SkillArbitrium();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillArbitriumAttack : public SkillImplRecursiveDamageSplash {
public:
	SkillArbitriumAttack();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillAspersio : public SkillImpl {
public:
	SkillAspersio();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAssimilatePower : public SkillImpl {
public:
	SkillAssimilatePower();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAssumptio : public StatusSkillImpl {
public:
	SkillAssumptio();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillAsuraStrike : public WeaponSkillImpl {
public:
	SkillAsuraStrike();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillBasilica : public StatusSkillImpl {
public:
	SkillBasilica();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBlazingFlameBlast : public WeaponSkillImpl {
public:
	SkillBlazingFlameBlast();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillBlessing : public SkillImpl {
public:
	SkillBlessing();
	
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillBenedictioSanctissimiSacramenti : public SkillImpl {
public:
	SkillBenedictioSanctissimiSacramenti();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCantoCandidus : public SkillImpl {
public:
	SkillCantoCandidus();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillChainCrushCombo : public WeaponSkillImpl {
public:
	SkillChainCrushCombo();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillClearance : public SkillImpl {
public:
	SkillClearance();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillColuceoHeal : public SkillImpl {
public:
	SkillColuceoHeal();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCompetentia : public SkillImpl {
public:
	SkillCompetentia();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillConvenio : public SkillImpl {
public:
	SkillConvenio();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCrementia : public SkillImpl {
public:
	SkillCrementia();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCrucis : public SkillImpl {
public:
	SkillCrucis();
	
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCure : public SkillImpl {
public:
	SkillCure();
	
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillCursedCircle : public SkillImpl {
public:
	SkillCursedCircle();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDecreaseAgi : public SkillImpl {
public:
	SkillDecreaseAgi();
	
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDilectioHeal : public SkillImpl {
public:
	SkillDilectioHeal();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillDivinusFlos : public SkillImplRecursiveDamageSplash {
public:
	SkillDivinusFlos();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	void modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const override;
};

class SkillDragonCombo : public WeaponSkillImpl {
public:
	SkillDragonCombo();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

// AB_DUPLELIGHT_MAGIC
class SkillDupleLightMagic : public SkillImpl {
public:
	SkillDupleLightMagic();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};


// AB_DUPLELIGHT_MELEE
class SkillDupleLightMelee : public WeaponSkillImpl {
public:
	SkillDupleLightMelee();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const override;
};

class SkillEarthShaker : public WeaponSkillImpl {
public:
	SkillEarthShaker();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillEffligo : public WeaponSkillImpl {
public:
	SkillEffligo();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillEpiclesis : public SkillImpl {
public:
	SkillEpiclesis();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillExplosionBlaster : public SkillImplRecursiveDamageSplash {
public:
	SkillExplosionBlaster();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillFallenEmpire : public WeaponSkillImpl {
public:
	SkillFallenEmpire();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const override;
};

class SkillFirstBrand : public SkillImplRecursiveDamageSplash {
public:
	SkillFirstBrand();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillFlashCombo : public SkillImpl {
public:
	SkillFlashCombo();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillFramen : public SkillImplRecursiveDamageSplash {
public:
	SkillFramen();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillGateOfHell : public WeaponSkillImpl {
public:
	SkillGateOfHell();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillGentleTouchCure : public SkillImpl {
public:
	SkillGentleTouchCure();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillGentleTouchQuiet : public WeaponSkillImpl {
public:
	SkillGentleTouchQuiet();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillGlacierFist : public WeaponSkillImpl {
public:
	SkillGlacierFist();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillGloria : public SkillImpl {
public:
	SkillGloria();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHeal : public SkillImpl {
public:
	SkillHeal();
	
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHighnessHeal : public SkillImpl {
public:
	SkillHighnessHeal();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHolyLight : public SkillImpl {
public:
	SkillHolyLight();
	
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillHolyWater : public SkillImpl {
public:
	SkillHolyWater();
	
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillHowlingOfLion : public SkillImplRecursiveDamageSplash {
public:
	SkillHowlingOfLion();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	int64 splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	int32 getSplashTarget(block_list* src) const override;
};

#ifdef RENEWAL
class SkillImpositioManus : public SkillImpl {
public:
	SkillImpositioManus();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
#else
class SkillImpositioManus : public StatusSkillImpl {
public:
	SkillImpositioManus();
};
#endif

class SkillIncreaseAgi : public SkillImpl {
public:
	SkillIncreaseAgi();
	
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillJudex : public SkillImplRecursiveDamageSplash {
public:
	SkillJudex();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
};

class SkillKiExplosion : public SkillImpl {
public:
	SkillKiExplosion();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillKiTranslation : public SkillImpl {
public:
	SkillKiTranslation();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillKnuckleArrow : public SkillImpl {
public:
	SkillKnuckleArrow();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillKyrieEleison : public SkillImpl {
public:
	SkillKyrieEleison();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillLaudaAgnus : public SkillImpl {
public:
	SkillLaudaAgnus();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillLaudaRamus : public SkillImpl {
public:
	SkillLaudaRamus();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillLexDivina : public SkillImpl {
public:
	SkillLexDivina();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMagnificat : public SkillImpl {
public:
	SkillMagnificat();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillMagnusExorcismus : public SkillImpl {
public:
	SkillMagnusExorcismus();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillMassiveFlameBlaster : public SkillImplRecursiveDamageSplash {
public:
	SkillMassiveFlameBlaster();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillMedialeVotum : public StatusSkillImpl {
public:
	SkillMedialeVotum();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillOccultImpaction : public WeaponSkillImpl {
public:
	SkillOccultImpaction();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillOleumSanctum : public SkillImplRecursiveDamageSplash {
public:
	SkillOleumSanctum();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
};

class SkillOratio : public SkillImpl {
public:
	SkillOratio();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPetitio : public SkillImplRecursiveDamageSplash {
public:
	SkillPetitio();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillPneuma : public SkillImpl {
public:
	SkillPneuma();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPneumaticusProcella : public SkillImpl {
public:
	SkillPneumaticusProcella();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillPowerVelocity : public SkillImpl {
public:
	SkillPowerVelocity();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillPraefatio : public SkillImpl {
public:
	SkillPraefatio();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRagingPalmStrike : public SkillImpl {
public:
	SkillRagingPalmStrike();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillRagingQuadrupleBlow : public WeaponSkillImpl {
public:
	SkillRagingQuadrupleBlow();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillRagingThrust : public WeaponSkillImpl {
public:
	SkillRagingThrust();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillRagingTrifectaBlow : public WeaponSkillImpl {
public:
	SkillRagingTrifectaBlow();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillRaisingDragon : public StatusSkillImpl {
public:
	SkillRaisingDragon();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRampageBlaster : public SkillImplRecursiveDamageSplash {
public:
	SkillRampageBlaster();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRedemptio : public SkillImpl {
public:
	SkillRedemptio();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRenovatio : public StatusSkillImpl {
public:
	SkillRenovatio();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillReparatio : public SkillImpl {
public:
	SkillReparatio();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillResurrection : public SkillImpl {
public:
	SkillResurrection();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRideInLightening : public SkillImplRecursiveDamageSplash {
public:
	SkillRideInLightening();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillRuwach : public SkillImpl {
public:
	SkillRuwach();
	
	void castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillSanctuary : public SkillImpl {
public:
	SkillSanctuary();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSecondFaith : public SkillImplRecursiveDamageSplash {
public:
	SkillSecondFaith();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillSecondFlame : public SkillImplRecursiveDamageSplash {
public:
	SkillSecondFlame();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillSecondJudgement : public SkillImplRecursiveDamageSplash {
public:
	SkillSecondJudgement();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillSilentium : public SkillImpl {
public:
	SkillSilentium();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSkyNetBlow : public SkillImplRecursiveDamageSplash {
public:
	SkillSkyNetBlow();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillSnap : public SkillImpl {
public:
	SkillSnap();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillStatusRecovery : public SkillImpl {
public:
	SkillStatusRecovery();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

#ifdef RENEWAL
class SkillSuffragium : public SkillImpl {
public:
	SkillSuffragium();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
#else
class SkillSuffragium : public StatusSkillImpl {
public:
	SkillSuffragium();
};
#endif

class SkillSummoningSpiritSphere : public SkillImpl {
public:
	SkillSummoningSpiritSphere();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTeleport : public SkillImpl {
public:
	SkillTeleport();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillThirdConsecration : public SkillImplRecursiveDamageSplash {
public:
	SkillThirdConsecration();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillThirdFlameBomb : public SkillImplRecursiveDamageSplash {
public:
	SkillThirdFlameBomb();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillThirdPunish : public SkillImplRecursiveDamageSplash {
public:
	SkillThirdPunish();

	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const override;
};

class SkillThrowSpiritSphere : public WeaponSkillImpl {
public:
	SkillThrowSpiritSphere();

	void modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const override;
	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const override;
};

class SkillTigerCannon : public WeaponSkillImpl {
public:
	SkillTigerCannon();

	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillTurnUndead : public SkillImpl {
public:
	SkillTurnUndead();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillVituperatum : public StatusSkillImpl {
public:
	SkillVituperatum();

	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWarpPortal : public SkillImpl {
public:
	SkillWarpPortal();

	void castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillWindmill : public SkillImplRecursiveDamageSplash {
public:
	SkillWindmill();

	void applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const override;
	void calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const override;
	void castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};

class SkillZen : public SkillImpl {
public:
	SkillZen();

	void castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
