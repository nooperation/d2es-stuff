///////////////////////////////////////////////////////////////
// Unit affection information retrieval
//
// Written by Abin
///////////////////////////////////////////////////////////////
#include "AffectionDef.h"
#include <tchar.h>
#include <string.h>
#include "Constants.h"

DWORD D2GetAffectionName(BYTE iAffectionID, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (dwMaxChars == 0)
		return 0;

	switch (iAffectionID)
	{
	case AFFECT_FREEZE:
		_tcsncpy(lpszBuffer, _T("Freeze"), dwMaxChars);
		break;

	case AFFECT_POISON:
		_tcsncpy(lpszBuffer, _T("Poison"), dwMaxChars);
		break;

	case AFFECT_RESISTFIRE:
		_tcsncpy(lpszBuffer, _T("Resist Fire"), dwMaxChars);
		break;

	case AFFECT_RESISTCOLD:
		_tcsncpy(lpszBuffer, _T("Resist Cold"), dwMaxChars);
		break;

	case AFFECT_RESISTLIGHT:
		_tcsncpy(lpszBuffer, _T("Resist Lightning"), dwMaxChars);
		break;

	case AFFECT_RESISTMAGIC:
		_tcsncpy(lpszBuffer, _T("Resist Magic"), dwMaxChars);
		break;

	case AFFECT_RESISTALL:
		_tcsncpy(lpszBuffer, _T("Salvation"), dwMaxChars);
		break;

	case AFFECT_AMPLIFYDAMAGE:
		_tcsncpy(lpszBuffer, _T("Amplify Damage"), dwMaxChars);
		break;

	case AFFECT_FROZENARMOR:
		_tcsncpy(lpszBuffer, _T("Frozen Armor"), dwMaxChars);
		break;

	case AFFECT_COLD:
		_tcsncpy(lpszBuffer, _T("Cold"), dwMaxChars);
		break;

	case AFFECT_INFERNO:
		_tcsncpy(lpszBuffer, _T("Inferno"), dwMaxChars);
		break;

	case AFFECT_BLAZE:
		_tcsncpy(lpszBuffer, _T("Blaze"), dwMaxChars);
		break;

	case AFFECT_BONEARMOR:
		_tcsncpy(lpszBuffer, _T("Bone Armor"), dwMaxChars);
		break;

	case AFFECT_CONCENTRATE:
		_tcsncpy(lpszBuffer, _T("Concentrate"), dwMaxChars);
		break;

	case AFFECT_ENCHANT:
		_tcsncpy(lpszBuffer, _T("Enchant"), dwMaxChars);
		break;

	case AFFECT_INNERSIGHT:
		_tcsncpy(lpszBuffer, _T("Inner Sight"), dwMaxChars);
		break;

	case AFFECT_SKILL_MOVE:
		_tcsncpy(lpszBuffer, _T("Skill Move"), dwMaxChars);
		break;

	case AFFECT_WEAKEN:
		_tcsncpy(lpszBuffer, _T("Weaken"), dwMaxChars);
		break;

	case AFFECT_CHILLINGARMOR:
		_tcsncpy(lpszBuffer, _T("Chilling Armor"), dwMaxChars);
		break;

	case AFFECT_STUNNED:
		_tcsncpy(lpszBuffer, _T("Stunned"), dwMaxChars);
		break;

	case AFFECT_SPIDERLAY:
		_tcsncpy(lpszBuffer, _T("Spider Lay"), dwMaxChars);
		break;

	case AFFECT_DIMVISION:
		_tcsncpy(lpszBuffer, _T("Dim Vision"), dwMaxChars);
		break;

	case AFFECT_SLOWED:
		_tcsncpy(lpszBuffer, _T("Slowed"), dwMaxChars);
		break;

	case AFFECT_FETISHAURA:
		_tcsncpy(lpszBuffer, _T("Fetish Aura"), dwMaxChars);
		break;

	case AFFECT_SHOUT:
		_tcsncpy(lpszBuffer, _T("Shout"), dwMaxChars);
		break;

	case AFFECT_TAUNT:
		_tcsncpy(lpszBuffer, _T("Taunt"), dwMaxChars);
		break;

	case AFFECT_CONVICTION:
		_tcsncpy(lpszBuffer, _T("Conviction"), dwMaxChars);
		break;

	case AFFECT_CONVICTED:
		_tcsncpy(lpszBuffer, _T("Convicted"), dwMaxChars);
		break;

	case AFFECT_ENERGYSHIELD:
		_tcsncpy(lpszBuffer, _T("Energy Shield"), dwMaxChars);
		break;

	case AFFECT_VENOMCLAWS:
		_tcsncpy(lpszBuffer, _T("Venom Claw"), dwMaxChars);
		break;

	case AFFECT_BATTLEORDERS:
		_tcsncpy(lpszBuffer, _T("Battle Orders"), dwMaxChars);
		break;

	case AFFECT_MIGHT:
		_tcsncpy(lpszBuffer, _T("Might"), dwMaxChars);
		break;

	case AFFECT_PRAYER:
		_tcsncpy(lpszBuffer, _T("Prayer"), dwMaxChars);
		break;

	case AFFECT_HOLYFIRE:
		_tcsncpy(lpszBuffer, _T("Holy Fire"), dwMaxChars);
		break;

	case AFFECT_THORNS:
		_tcsncpy(lpszBuffer, _T("Thorns"), dwMaxChars);
		break;

	case AFFECT_DEFIANCE:
		_tcsncpy(lpszBuffer, _T("Defiance"), dwMaxChars);
		break;

	case AFFECT_THUNDERSTORM:
		_tcsncpy(lpszBuffer, _T("Thunder Storm"), dwMaxChars);
		break;

	case AFFECT_LIGHTNINGBOLT:
		_tcsncpy(lpszBuffer, _T("Lightning Bolt"), dwMaxChars);
		break;

	case AFFECT_BLESSEDAIM:
		_tcsncpy(lpszBuffer, _T("Blessed Aim"), dwMaxChars);
		break;

	case AFFECT_VIGOR: // vigor
		_tcsncpy(lpszBuffer, _T("Vigor"), dwMaxChars);
		break;

	case AFFECT_CONCENTRATION:
		_tcsncpy(lpszBuffer, _T("Concentration"), dwMaxChars);
		break;

	case AFFECT_HOLYFREEZE: // Holy freeze
		_tcsncpy(lpszBuffer, _T("Holy Freeze"), dwMaxChars);
		break;

	case AFFECT_HOLYFREEZECHILL:
		_tcsncpy(lpszBuffer, _T("Holy Freeze Chilled"), dwMaxChars);
		break;

	case AFFECT_CLEANSING:
		_tcsncpy(lpszBuffer, _T("Cleansing"), dwMaxChars);
		break;

	case AFFECT_HOLYSHOCK:
		_tcsncpy(lpszBuffer, _T("Holy Shock"), dwMaxChars);
		break;

	case AFFECT_SANCTUARY:
		_tcsncpy(lpszBuffer, _T("Sanctuary"), dwMaxChars);
		break;

	case AFFECT_MEDITATION:
		_tcsncpy(lpszBuffer, _T("Meditation"), dwMaxChars);
		break;

	case AFFECT_FANATICISM:
		_tcsncpy(lpszBuffer, _T("Fanaticism"), dwMaxChars);
		break;

	case AFFECT_REDEMPTION:
		_tcsncpy(lpszBuffer, _T("Redemption"), dwMaxChars);
		break;

	case AFFECT_BATTLECOMMAND:
		_tcsncpy(lpszBuffer, _T("Battle Command"), dwMaxChars);
		break;

	case AFFECT_PREVENTHEAL:
		_tcsncpy(lpszBuffer, _T("Prevent Heal"), dwMaxChars);
		break;

	case AFFECT_UNINTERRUPTABLE:
		_tcsncpy(lpszBuffer, _T("Uninterruptable"), dwMaxChars);
		break;

	case AFFECT_IRONMAIDEN:
		_tcsncpy(lpszBuffer, _T("Iron Maiden"), dwMaxChars);
		break;

	case AFFECT_TERROR:
		_tcsncpy(lpszBuffer, _T("Terror"), dwMaxChars);
		break;

	case AFFECT_ATTRACT:
		_tcsncpy(lpszBuffer, _T("Attract"), dwMaxChars);
		break;

	case AFFECT_LIFETAP:
		_tcsncpy(lpszBuffer, _T("Life Tap"), dwMaxChars);
		break;

	case AFFECT_CONFUSE:
		_tcsncpy(lpszBuffer, _T("Confuse"), dwMaxChars);
		break;

	case AFFECT_DECREPIFY:
		_tcsncpy(lpszBuffer, _T("Decrepify"), dwMaxChars);
		break;

	case AFFECT_LOWERRESIST:
		_tcsncpy(lpszBuffer, _T("Lower Resist"), dwMaxChars);
		break;

	case AFFECT_OPENWOUNDS:
		_tcsncpy(lpszBuffer, _T("Open Wounds"), dwMaxChars);
		break;

	case AFFECT_DOPPLEZON:
		_tcsncpy(lpszBuffer, _T("Dopplezon"), dwMaxChars);
		break;

	case AFFECT_CRITICALSTRIKE:
		_tcsncpy(lpszBuffer, _T("Critical Strike"), dwMaxChars);
		break;

	case AFFECT_DODGE:
		_tcsncpy(lpszBuffer, _T("Dodge"), dwMaxChars);
		break;

	case AFFECT_AVOID:
		_tcsncpy(lpszBuffer, _T("Avoid"), dwMaxChars);
		break;

	case AFFECT_PENETRATE:
		_tcsncpy(lpszBuffer, _T("Penetrate"), dwMaxChars);
		break;

	case AFFECT_EVADE:
		_tcsncpy(lpszBuffer, _T("Evade"), dwMaxChars);
		break;

	case AFFECT_PIERCE:
		_tcsncpy(lpszBuffer, _T("Pierce"), dwMaxChars);
		break;

	case AFFECT_WARMTH:
		_tcsncpy(lpszBuffer, _T("Warmth"), dwMaxChars);
		break;

	case AFFECT_FIREMASTERY:
		_tcsncpy(lpszBuffer, _T("Fire Mastery"), dwMaxChars);
		break;

	case AFFECT_LIGHTNINGMASTERY:
		_tcsncpy(lpszBuffer, _T("Lightning Mastery"), dwMaxChars);
		break;

	case AFFECT_COLDMASTERY:
		_tcsncpy(lpszBuffer, _T("Cold Mastery"), dwMaxChars);
		break;

	case AFFECT_SWORDMASTERY:
		_tcsncpy(lpszBuffer, _T("Shadow Mastery"), dwMaxChars);
		break;

	case AFFECT_AXEMASTERY:
		_tcsncpy(lpszBuffer, _T("Axe Mastery"), dwMaxChars);
		break;

	case AFFECT_MACEMASTERY:
		_tcsncpy(lpszBuffer, _T("Mace Mastery"), dwMaxChars);
		break;

	case AFFECT_POLEARMMASTERY:
		_tcsncpy(lpszBuffer, _T("Polearm Mastery"), dwMaxChars);
		break;

	case AFFECT_THROWINGMASTERY:
		_tcsncpy(lpszBuffer, _T("Throwing Mastery"), dwMaxChars);
		break;

	case AFFECT_SPEARMASTERY:
		_tcsncpy(lpszBuffer, _T("Spear Mastery"), dwMaxChars);
		break;

	case AFFECT_INCREASEDSTAMINA:
		_tcsncpy(lpszBuffer, _T("Increased Stamina"), dwMaxChars);
		break;

	case AFFECT_IRONSKIN:
		_tcsncpy(lpszBuffer, _T("Iron Skin"), dwMaxChars);
		break;

	case AFFECT_INCREASEDSPEED:
		_tcsncpy(lpszBuffer, _T("Increased Speed"), dwMaxChars);
		break;

	case AFFECT_NATURALRESISTANCE:
		_tcsncpy(lpszBuffer, _T("Natural Resistance"), dwMaxChars);
		break;

	case AFFECT_FINGERMAGECURSE:
		_tcsncpy(lpszBuffer, _T("Finger Mage Curse"), dwMaxChars);
		break;

	case AFFECT_NOMANAREGEN:
		_tcsncpy(lpszBuffer, _T("No Mana Regen"), dwMaxChars);
		break;

	case AFFECT_JUSTHIT:
		_tcsncpy(lpszBuffer, _T("Just Hit"), dwMaxChars);
		break;

	case AFFECT_SLOWMISSILES:
		_tcsncpy(lpszBuffer, _T("Slow Missles"), dwMaxChars);
		break;

	case AFFECT_SHIVERARMOR:
		_tcsncpy(lpszBuffer, _T("Shiver Armor"), dwMaxChars);
		break;

	case AFFECT_BATTLECRY:
		_tcsncpy(lpszBuffer, _T("Battle Cry"), dwMaxChars);
		break;

	case AFFECT_DEATH_DELAY:
		_tcsncpy(lpszBuffer, _T("Death Delay"), dwMaxChars);
		break;

	case AFFECT_VALKYRIE:
		_tcsncpy(lpszBuffer, _T("Valkyrie"), dwMaxChars);
		break;

	case AFFECT_FRENZY:
		_tcsncpy(lpszBuffer, _T("Frenzy"), dwMaxChars);
		break;

	case AFFECT_BERSERK:
		_tcsncpy(lpszBuffer, _T("Berserk"), dwMaxChars);
		break;

	case AFFECT_REVIVE:
		_tcsncpy(lpszBuffer, _T("Revive"), dwMaxChars);
		break;

	case AFFECT_ITEMFULLSET:
		_tcsncpy(lpszBuffer, _T("Item Full Set"), dwMaxChars);
		break;

	case AFFECT_SOURCEUNIT:
		_tcsncpy(lpszBuffer, _T("Source Unit"), dwMaxChars);
		break;

	case AFFECT_REDEEMED:
		_tcsncpy(lpszBuffer, _T("Redeemed"), dwMaxChars);
		break;

	case AFFECT_HEALTHPOT:
		_tcsncpy(lpszBuffer, _T("Health Pot"), dwMaxChars);
		break;

	case AFFECT_HOLYSHIELD:
		_tcsncpy(lpszBuffer, _T("Holy Shield"), dwMaxChars);
		break;

	case AFFECT_JUST_PORTALED:
		_tcsncpy(lpszBuffer, _T("Just Portaled"), dwMaxChars);
		break;

	case AFFECT_MONFRENZY:
		_tcsncpy(lpszBuffer, _T("MonFrenzy"), dwMaxChars);
		break;

	case AFFECT_CORPSE_NODRAW:
		_tcsncpy(lpszBuffer, _T("Corpse No Draw"), dwMaxChars);
		break;

	case AFFECT_ALIGNMENT:
		_tcsncpy(lpszBuffer, _T("Alignment"), dwMaxChars);
		break;

	case AFFECT_MANAPOT:
		_tcsncpy(lpszBuffer, _T("Mana Pot"), dwMaxChars);
		break;

	case AFFECT_SHATTER:
		_tcsncpy(lpszBuffer, _T("Shatter"), dwMaxChars);
		break;

	case AFFECT_SYNC_WARPED:
		_tcsncpy(lpszBuffer, _T("Sync Warped"), dwMaxChars);
		break;

	case AFFECT_CONVERSION_SAVE:
		_tcsncpy(lpszBuffer, _T("Conversion Save"), dwMaxChars);
		break;

	case AFFECT_PREGNANT:
		_tcsncpy(lpszBuffer, _T("Pregnant"), dwMaxChars); // LOL
		break;

	case AFFECT_RABIES:
		_tcsncpy(lpszBuffer, _T("Rabies"), dwMaxChars);
		break;

	case AFFECT_DEFENSE_CURSE:
		_tcsncpy(lpszBuffer, _T("Defense Curse"), dwMaxChars);
		break;

	case AFFECT_BLOOD_MANA:
		_tcsncpy(lpszBuffer, _T("Blood Mana"), dwMaxChars);
		break;

	case AFFECT_BURNING:
		_tcsncpy(lpszBuffer, _T("Burning"), dwMaxChars);
		break;

	case AFFECT_DRAGONFLIGHT:
		_tcsncpy(lpszBuffer, _T("Dragon Flight"), dwMaxChars);
		break;

	case AFFECT_MAUL:
		_tcsncpy(lpszBuffer, _T("Maul"), dwMaxChars);
		break;

	case AFFECT_CORPSE_NOSELECT:
		_tcsncpy(lpszBuffer, _T("Corpse No Select"), dwMaxChars);
		break;

	case AFFECT_SHADOWWARRIOR:
		_tcsncpy(lpszBuffer, _T("Shadow Warrior"), dwMaxChars);
		break;

	case AFFECT_FERALRAGE:
		_tcsncpy(lpszBuffer, _T("Feral Rage"), dwMaxChars);
		break;

	case AFFECT_SKILLDELAY:
		_tcsncpy(lpszBuffer, _T("Skill Delay"), dwMaxChars);
		break;

	case AFFECT_PROGRESSIVE_DAMAGE:
		_tcsncpy(lpszBuffer, _T("Progressive Damage"), dwMaxChars);
		break;

	case AFFECT_PROGRESSIVE_STEAL:
		_tcsncpy(lpszBuffer, _T("Progressive Steal"), dwMaxChars);
		break;

	case AFFECT_PROGRESSIVE_OTHER:
		_tcsncpy(lpszBuffer, _T("Progressive Other"), dwMaxChars);
		break;

	case AFFECT_PROGRESSIVE_FIRE:
		_tcsncpy(lpszBuffer, _T("Progressive Fire"), dwMaxChars);
		break;

	case AFFECT_PROGRESSIVE_COLD:
		_tcsncpy(lpszBuffer, _T("Progressive Cold"), dwMaxChars);
		break;

	case AFFECT_PROGRESSIVE_LIGHTNING:
		_tcsncpy(lpszBuffer, _T("Progressive Lightning"), dwMaxChars);
		break;

	case AFFECT_SHRINE_ARMOR:
		_tcsncpy(lpszBuffer, _T("Shrine Armor"), dwMaxChars);
		break;

	case AFFECT_SHRINE_COMBAT:
		_tcsncpy(lpszBuffer, _T("Shrine Combat"), dwMaxChars);
		break;

	case AFFECT_SHRINE_RESIST_LIGHTNING:
		_tcsncpy(lpszBuffer, _T("Shrine Resist Lightning"), dwMaxChars);
		break;

	case AFFECT_SHRINE_RESIST_FIRE:
		_tcsncpy(lpszBuffer, _T("Shrine Resist Fire"), dwMaxChars);
		break;

	case AFFECT_SHRINE_RESIST_POISON:
		_tcsncpy(lpszBuffer, _T("Shrine Resist Poison"), dwMaxChars);
		break;

	case AFFECT_SHRINE_SKILL:
		_tcsncpy(lpszBuffer, _T("Shrine Skill"), dwMaxChars);
		break;

	case AFFECT_SHRINE_MANA_REGEN:
		_tcsncpy(lpszBuffer, _T("Shrine Mana Regen"), dwMaxChars);
		break;

	case AFFECT_SHRINE_STAMINA:
		_tcsncpy(lpszBuffer, _T("Shrine Stamina"), dwMaxChars);
		break;

	case AFFECT_SHRINE_EXPERIENCE:
		_tcsncpy(lpszBuffer, _T("Shrine Experience"), dwMaxChars);
		break;

	case AFFECT_FENRIS_RAGE:
		_tcsncpy(lpszBuffer, _T("Fenris Rage"), dwMaxChars);
		break;

	case AFFECT_WOLF:
		_tcsncpy(lpszBuffer, _T("Wolf"), dwMaxChars);
		break;

	case AFFECT_BEAR:
		_tcsncpy(lpszBuffer, _T("Bear"), dwMaxChars);
		break;

	case AFFECT_BLOODLUST:
		_tcsncpy(lpszBuffer, _T("Blood Lust"), dwMaxChars);
		break;

	case AFFECT_CHANGECLASS:
		_tcsncpy(lpszBuffer, _T("Chance Class"), dwMaxChars);
		break;

	case AFFECT_ATTACHED:
		_tcsncpy(lpszBuffer, _T("Attached"), dwMaxChars);
		break;

	case AFFECT_HURRICANE:
		_tcsncpy(lpszBuffer, _T("Hurricane"), dwMaxChars);
		break;

	case AFFECT_ARMAGEDDON:
		_tcsncpy(lpszBuffer, _T("Armageddon"), dwMaxChars);
		break;

	case AFFECT_INVIS:
		_tcsncpy(lpszBuffer, _T("Invis"), dwMaxChars);
		break;

	case AFFECT_BARBS:
		_tcsncpy(lpszBuffer, _T("Barbs"), dwMaxChars);
		break;

	case AFFECT_WOLVERINE:
		_tcsncpy(lpszBuffer, _T("Wolverine"), dwMaxChars);
		break;

	case AFFECT_OAKSAGE:
		_tcsncpy(lpszBuffer, _T("Oak Sage"), dwMaxChars);
		break;

	case AFFECT_VINE_BEAST:
		_tcsncpy(lpszBuffer, _T("Battle Cry"), dwMaxChars);
		break;

	case AFFECT_CYCLONEARMOR:
		_tcsncpy(lpszBuffer, _T("Cyclone Armor"), dwMaxChars);
		break;

	case AFFECT_CLAWMASTERY:
		_tcsncpy(lpszBuffer, _T("Claw Mastery"), dwMaxChars);
		break;

	case AFFECT_CLOAK_OF_SHADOWS:
		_tcsncpy(lpszBuffer, _T("Cloak of Shadows"), dwMaxChars);
		break;

	case AFFECT_RECYCLED:
		_tcsncpy(lpszBuffer, _T("Recycled"), dwMaxChars);
		break;

	case AFFECT_WEAPONBLOCK:
		_tcsncpy(lpszBuffer, _T("Weapon Block"), dwMaxChars);
		break;

	case AFFECT_CLOAKED:
		_tcsncpy(lpszBuffer, _T("Cloaked"), dwMaxChars);
		break;

	case AFFECT_BURSTOFSPEED:
		_tcsncpy(lpszBuffer, _T("Burst of Speed"), dwMaxChars);
		break;

	case AFFECT_BLADESHIELD:
		_tcsncpy(lpszBuffer, _T("Blade Shield"), dwMaxChars);
		break;

	case AFFECT_FADE:
		_tcsncpy(lpszBuffer, _T("Fade"), dwMaxChars);
		break;

	default:
		break;
	}
	
	return _tcslen(lpszBuffer);
}

BOOL D2IsAura(BYTE iAffectionID)
{
	BOOL bResult = FALSE;
	switch (iAffectionID)
	{
	case AFFECT_RESISTFIRE:
	case AFFECT_RESISTCOLD:
	case AFFECT_RESISTLIGHT:
	case AFFECT_RESISTALL: // salvation
	case AFFECT_CONVICTION:
	case AFFECT_MIGHT:
	case AFFECT_PRAYER:
	case AFFECT_HOLYFIRE:
	case AFFECT_HOLYFREEZE: // holy freeze
	case AFFECT_THORNS:
	case AFFECT_DEFIANCE:
	case AFFECT_BLESSEDAIM:
	case AFFECT_VIGOR: // vigor
	case AFFECT_CONCENTRATION:
	case AFFECT_CLEANSING:
	case AFFECT_HOLYSHOCK:
	case AFFECT_SANCTUARY:
	case AFFECT_MEDITATION:
	case AFFECT_FANATICISM:
	case AFFECT_REDEMPTION:
		bResult = TRUE;
		break;

	default:
		break;
	}

	return bResult;
}
