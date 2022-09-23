static const char* StatNames[] = 
{
    "strength",
    "energy",
    "dexterity",
    "vitality",
    "statpts",
    "newskills",
    "hitpoints",
    "maxhp",
    "mana",
    "maxmana",
    "stamina",
    "maxstamina",
    "level",
    "experience",
    "gold",
    "goldbank",
    "item_armor_percent",
    "item_maxdamage_percent",
    "item_mindamage_percent",
    "tohit",
    "toblock",
    "mindamage",
    "maxdamage",
    "secondary_mindamage",
    "secondary_maxdamage",
    "damagepercent",
    "manarecovery",
    "manarecoverybonus",
    "staminarecoverybonus",
    "lastexp",
    "nextexp",
    "armorclass",
    "armorclass_vs_missile",
    "armorclass_vs_hth",
    "normal_damage_reduction",
    "magic_damage_reduction",
    "damageresist",
    "magicresist",
    "maxmagicresist",
    "fireresist",
    "maxfireresist",
    "lightresist",
    "maxlightresist",
    "coldresist",
    "maxcoldresist",
    "poisonresist",
    "maxpoisonresist",
    "damageaura",
    "firemindam",
    "firemaxdam",
    "lightmindam",
    "lightmaxdam",
    "magicmindam",
    "magicmaxdam",
    "coldmindam",
    "coldmaxdam",
    "coldlength",
    "poisonmindam",
    "poisonmaxdam",
    "poisonlength",
    "lifedrainmindam",
    "lifedrainmaxdam",
    "manadrainmindam",
    "manadrainmaxdam",
    "stamdrainmindam",
    "stamdrainmaxdam",
    "stunlength",
    "velocitypercent",
    "attackrate",
    "other_animrate",
    "quantity",
    "value",
    "durability",
    "maxdurability",
    "hpregen",
    "item_maxdurability_percent",
    "item_maxhp_percent",
    "item_maxmana_percent",
    "item_attackertakesdamage",
    "item_goldbonus",
    "item_magicbonus",
    "item_knockback",
    "item_timeduration",
    "item_addclassskills",
    "unsentparam1",
    "item_addexperience",
    "item_healafterkill",
    "item_reducedprices",
    "item_doubleherbduration",
    "item_lightradius",
    "item_lightcolor",
    "item_req_percent",
    "item_levelreq",
    "item_fasterattackrate",
    "item_levelreqpct",
    "lastblockframe",
    "item_fastermovevelocity",
    "item_nonclassskill",
    "state",
    "item_fastergethitrate",
    "monster_playercount",
    "skill_poison_override_length",
    "item_fasterblockrate",
    "skill_bypass_undead",
    "skill_bypass_demons",
    "item_fastercastrate",
    "skill_bypass_beasts",
    "item_singleskill",
    "item_restinpeace",
    "curse_resistance",
    "item_poisonlengthresist",
    "item_normaldamage",
    "item_howl",
    "item_stupidity",
    "item_damagetomana",
    "item_ignoretargetac",
    "item_fractionaltargetac",
    "item_preventheal",
    "item_halffreezeduration",
    "item_tohit_percent",
    "item_damagetargetac",
    "item_demondamage_percent",
    "item_undeaddamage_percent",
    "item_demon_tohit",
    "item_undead_tohit",
    "item_throwable",
    "item_elemskill",
    "item_allskills",
    "item_attackertakeslightdamage",
    "ironmaiden_level",
    "lifetap_level",
    "thorns_percent",
    "bonearmor",
    "bonearmormax",
    "item_freeze",
    "item_openwounds",
    "item_crushingblow",
    "item_kickdamage",
    "item_manaafterkill",
    "item_healafterdemonkill",
    "item_extrablood",
    "item_deadlystrike",
    "item_absorbfire_percent",
    "item_absorbfire",
    "item_absorblight_percent",
    "item_absorblight",
    "item_absorbmagic_percent",
    "item_absorbmagic",
    "item_absorbcold_percent",
    "item_absorbcold",
    "item_slow",
    "item_aura",
    "item_indesctructible",
    "item_cannotbefrozen",
    "item_staminadrainpct",
    "item_reanimate",
    "item_pierce",
    "item_magicarrow",
    "item_explosivearrow",
    "item_throw_mindamage",
    "item_throw_maxdamage",
    "skill_handofathena",
    "skill_staminapercent",
    "skill_passive_staminapercent",
    "skill_concentration",
    "skill_enchant",
    "skill_pierce",
    "skill_conviction",
    "skill_chillingarmor",
    "skill_frenzy",
    "skill_decrepify",
    "skill_armor_percent",
    "alignment",
    "target0",
    "target1",
    "goldlost",
    "conversion_level",
    "conversion_maxhp",
    "unit_dooverlay",
    "attack_vs_montype",
    "damage_vs_montype",
    "fade",
    "armor_override_percent",
    "junk_01",
    "junk_02",
    "junk_03",
    "junk_04",
    "junk_05",
    "item_addskill_tab",
    "junk_06",
    "item_tinkerflag",
    "item_magic_damagemax_perlevel",
    "kill_counter",
    "item_token",
    "item_numsockets",
    "item_skillonattack",
    "item_skillonkill",
    "item_skillondeath",
    "item_skillonhit",
    "item_skillonlevelup",
    "vitality_hidden",
    "item_skillongethit",
    "item_craftflag",
    "item_randomize",
    "item_charged_skill",
    "gem_v",
    "gem_y",
    "gem_b",
    "gem_g",
    "gem_r",
    "gem_w",
    "gem_s",
    "gem_k",
    "item_fear",
    "item_armor_perlevel",
    "item_armorpercent_perlevel",
    "item_hp_perlevel",
    "item_mana_perlevel",
    "item_maxdamage_perlevel",
    "item_maxdamage_percent_perlevel",
    "item_strength_perlevel",
    "item_dexterity_perlevel",
    "item_energy_perlevel",
    "item_vitality_perlevel",
    "item_tohit_perlevel",
    "item_tohitpercent_perlevel",
    "item_cold_damagemax_perlevel",
    "item_fire_damagemax_perlevel",
    "item_ltng_damagemax_perlevel",
    "item_pois_damagemax_perlevel",
    "item_resist_cold_perlevel",
    "item_resist_fire_perlevel",
    "item_resist_ltng_perlevel",
    "item_resist_pois_perlevel",
    "item_absorb_cold_perlevel",
    "item_absorb_fire_perlevel",
    "item_absorb_ltng_perlevel",
    "item_absorb_pois_perlevel",
    "item_thorns_perlevel",
    "item_find_gold_perlevel",
    "item_find_magic_perlevel",
    "item_regenstamina_perlevel",
    "item_stamina_perlevel",
    "item_damage_demon_perlevel",
    "item_damage_undead_perlevel",
    "item_tohit_demon_perlevel",
    "item_tohit_undead_perlevel",
    "item_crushingblow_perlevel",
    "item_openwounds_perlevel",
    "death_counter",
    "item_deadlystrike_perlevel",
    "item_meldflag",
    "item_replenish_durability",
    "item_replenish_quantity",
    "item_extra_stack",
    "tome_01",
    "tome_02",
    "tome_03",
    "tome_04",
    "tome_05",
    "tome_06",
    "tome_07",
    "tome_08",
    "tome_09",
    "tome_10",
    "tome_11",
    "tome_12",
    "tome_13",
    "tome_14",
    "tome_15",
    "tome_16",
    "tome_17",
    "tome_18",
    "tome_19",
    "tome_20",
    "tome_21",
    "tome_22",
    "tome_23",
    "tome_24",
    "tome_25",
    "tome_26",
    "tome_27",
    "tome_28",
    "tome_29",
    "tome_30",
    "tome_31",
    "tome_32",
    "tome_33",
    "tome_34",
    "tome_35",
    "tome_36",
    "tome_37",
    "tome_38",
    "tome_39",
    "tome_40",
    "tome_41",
    "tome_42",
    "tome_43",
    "tome_44",
    "tome_45",
    "tome_46",
    "tome_47",
    "tome_48",
    "tome_49",
    "tome_50",
    "item_pierce_cold",
    "item_pierce_fire",
    "item_pierce_ltng",
    "item_pierce_pois",
    "junk_11",
    "junk_12",
    "junk_13",
    "junk_14",
    "junk_15",
    "junk_16",
    "firelength",
    "burningmin",
    "burningmax",
    "progressive_damage",
    "progressive_steal",
    "progressive_other",
    "progressive_fire",
    "progressive_cold",
    "progressive_lightning",
    "item_extra_charges",
    "progressive_tohit",
    "poison_count",
    "damage_framerate",
    "pierce_idx",
    "passive_fire_mastery",
    "passive_ltng_mastery",
    "passive_cold_mastery",
    "passive_pois_mastery",
    "passive_fire_pierce",
    "passive_ltng_pierce",
    "passive_cold_pierce",
    "passive_pois_pierce",
    "passive_critical_strike",
    "passive_dodge",
    "passive_avoid",
    "passive_evade",
    "passive_warmth",
    "passive_mastery_melee_th",
    "passive_mastery_melee_dmg",
    "passive_mastery_melee_crit",
    "passive_mastery_throw_th",
    "passive_mastery_throw_dmg",
    "passive_mastery_throw_crit",
    "passive_weaponblock",
    "passive_summon_resist",
    "modifierlist_skill",
    "modifierlist_level",
    "last_sent_hp_pct",
    "source_unit_type",
    "source_unit_id",
    "shortparam1",
    "questitemdifficulty",
    "junk_17",
    "junk_18",
    "rune_01",
    "rune_02",
    "rune_03",
    "rune_04",
    "rune_05",
    "rune_06",
    "rune_07",
    "rune_08",
    "rune_09",
    "rune_10",
    "rune_11",
    "rune_12",
    "rune_13",
    "rune_14",
    "rune_15",
    "rune_16",
    "rune_17",
    "rune_18",
    "rune_19",
    "rune_20",
    "rune_21",
    "rune_22",
    "rune_23",
    "rune_24",
    "rune_25",
    "rune_26",
    "rune_27",
    "rune_28",
    "rune_29",
    "rune_30",
    "rune_31",
    "rune_32",
    "rune_33",
    "rune_34",
    "rune_35",
    "rune_36",
    "rune_37",
    "rune_38",
    "rune_39",
    "rune_40",
    "rune_41",
    "rune_42",
    "rune_43",
    "rune_44",
    "rune_45",
    "rune_46",
    "rune_51",
    "rune_52",
    "rune_53",
    "rune_54",
    "rune_55",
    "rune_56",
    "rune_57",
    "rune_58",
    "rune_59",
    "rune_60",
    "rune_61",
    "rune_62",
    "rune_63",
    "rune_64",
    "rune_65",
    "rune_66",
    "rune_67",
    "rune_68",
    "rune_69",
    "rune_70",
    "rune_71",
    "rune_72",
    "rune_73",
    "rune_74",
    "rune_75",
    "rune_76",
    "rune_77",
    "rune_78",
    "rune_79",
    "rune_80",
    "rune_81",
    "rune_82",
    "rune_83",
    "item_01",
    "item_02",
    "item_03",
    "item_04",
    "item_05",
    "item_06",
    "item_07",
    "item_08",
    "item_09",
    "item_10",
    "item_11",
    "item_12",
    "item_13",
    "item_14",
    "item_15",
    "item_16",
    "item_17",
    "item_18",
    "item_19",
    "item_20",
    "item_21",
    "item_22",
    "item_23",
    "junk_07",
    "junk_08",
    "item_craftflag2",
    "item_tinkerflag2",
    "item_24",
    "item_maxdamage_percent2",
    "item_mindamage_percent2",
    "junk_09",
    "rune_50",
    "xtal_00",
    "xtal_01",
    "xtal_02",
    "xtal_03",
    "xtal_04",
    "xtal_05",
    "xtal_06",
    "xtal_07",
    "xtal_08",
    "xtal_09",
    "xtal_10",
    "xtal_11",
    "passive_mag2_mastery",
    "passive_wind_mastery",
    "item_jewelflag",
    "item_mercflag",
    "passive_summ_mastery",
    "item_summskills",
    "item_extraflag",
};
