///////////////////////////////////////////////
// Spell Information Retrieval
//
// Written by Abin
///////////////////////////////////////////////

#include "SpellDef.h"
#include "D2HackIt.h"
#include "Constants.h"
#include <tchar.h>
#include <string.h>

DWORD D2GetClassName(int nClassID, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL || dwMaxChars == 0)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars * sizeof(TCHAR));
	switch (nClassID)
	{
	case CLASS_AMA:
		_tcsncpy(lpszBuffer, _T("Amazon"), dwMaxChars);
		break;

	case CLASS_SOR:
		_tcsncpy(lpszBuffer, _T("Sorceress"), dwMaxChars);
		break;

	case CLASS_NEC:
		_tcsncpy(lpszBuffer, _T("Necromancer"), dwMaxChars);
		break;

	case CLASS_PAL:
		_tcsncpy(lpszBuffer, _T("Paladin"), dwMaxChars);
		break;

	case CLASS_BAR:
		_tcsncpy(lpszBuffer, _T("Barbarian"), dwMaxChars);
		break;

	case CLASS_DRU:
		_tcsncpy(lpszBuffer, _T("Druid"), dwMaxChars);
		break;

	case CLASS_ASN:
		_tcsncpy(lpszBuffer, _T("Assassin"), dwMaxChars);
		break;

	default:
		break;
	}

	return _tcslen(lpszBuffer);
}

BOOL D2GetSpellInfo(WORD wSpell, LPD2SPELLINFO lpBuffer)
{
	if (lpBuffer == NULL)
		return FALSE;
	
	::memset(lpBuffer, 0, sizeof(D2SPELLINFO));
	lpBuffer->wSpellID = wSpell;
	lpBuffer->nClass = CLASS_NA;
	
	switch (wSpell)
	{
		// Common Skills 		
		case D2S_ATTACK:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Attack"), SPELL_MAX_NAME);
			break;

		case D2S_THROW:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
						
			_tcsncpy(lpBuffer->szSpellName, _T("Throw"), SPELL_MAX_NAME);
			break;

		case D2S_UNSUMMON:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Unsummon"), SPELL_MAX_NAME);
			break;

		case D2S_TOMEOFIDENTIFY:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Tome of Identify"), SPELL_MAX_NAME);
			break;

		case D2S_SCROLLOFIDENTIFY:
			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Scroll of Identify"), SPELL_MAX_NAME);
			break;

		case D2S_TOMEOFTOWNPORTAL:
						
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
						
			_tcsncpy(lpBuffer->szSpellName, _T("Tome of Town Portal"), SPELL_MAX_NAME);
			break;

		case D2S_SCROLLOFTOWNPORTAL:			
			
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
						
			_tcsncpy(lpBuffer->szSpellName, _T("Scroll of Town Portal"), SPELL_MAX_NAME);
			break;

///////////////////////////////////////////////////////////////
//    Amazon Skills
///////////////////////////////////////////////////////////////
		
// Javelin and Spear Skills
		case D2S_JAB:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
						
			_tcsncpy(lpBuffer->szSpellName, _T("Jab"), SPELL_MAX_NAME);
			break;

		case D2S_IMPALE:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
						
			_tcsncpy(lpBuffer->szSpellName, _T("Impale"), SPELL_MAX_NAME);
			break;

		case D2S_FEND:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;			
						
			_tcsncpy(lpBuffer->szSpellName, _T("Fend"), SPELL_MAX_NAME);
			break;

		case D2S_POWERSTRIKE:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Power Strike"), SPELL_MAX_NAME);
			break;

		case D2S_CHARGEDSTRIKE:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Charged Strike"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNINGSTRIKE:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Strike"), SPELL_MAX_NAME);
			break;

		case D2S_POISONJAVELIN:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Javelin"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNINGBOLT:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_PLAGUEJAVELIN:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Plague Javelin"), SPELL_MAX_NAME);
			break;
			
		case D2S_LIGHTNINGFURY:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Fury"), SPELL_MAX_NAME);
			break;


// Passive and Magic Skills
		case D2S_INNERSIGHT:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Inner Sight"), SPELL_MAX_NAME);
			break;

		case D2S_SLOWMISSILES:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Slow Missiles"), SPELL_MAX_NAME);
			break;

		case D2S_DECOY:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

			_tcsncpy(lpBuffer->szSpellName, _T("Decoy"), SPELL_MAX_NAME);
			break;

		case D2S_VALKYRIE:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

			_tcsncpy(lpBuffer->szSpellName, _T("Valkyrie"), SPELL_MAX_NAME);
			break;

		case D2S_DODGE:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dodge"), SPELL_MAX_NAME);
			break;

		case D2S_AVOID:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Avoid"), SPELL_MAX_NAME);
			break;

		case D2S_EVADE:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Evade"), SPELL_MAX_NAME);
			break;

		case D2S_CRITICALSTRIKE:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Critical Strike"), SPELL_MAX_NAME);
			break;

		case D2S_PENETRATE:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Penetrate"), SPELL_MAX_NAME);
			break;

		case D2S_PIERCE:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Pierce"), SPELL_MAX_NAME);
			break;

// Bow and Crossbow Skills
		case D2S_COLDARROW:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cold Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_ICEARROW:			
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;				
			
			_tcsncpy(lpBuffer->szSpellName, _T("Ice Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_FREEZINGARROW:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Freezing Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_MAGICARROW:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Magic Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_MULTIPLESHOT:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Multiple Shot"), SPELL_MAX_NAME);
			break;

		case D2S_GUIDEDARROW:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Guided Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_STRAFE:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Strafe"), SPELL_MAX_NAME);
			break;

		case D2S_FIREARROW:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_EXPLODINGARROW:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Exploding Arrow"), SPELL_MAX_NAME);
			break;

		case D2S_IMMOLATIONARROW:
			lpBuffer->nClass = CLASS_AMA;
			lpBuffer->dwCoolDown = 1000;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Immolation Arrow"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Assassin Skills
///////////////////////////////////////////////////////////////
		
// Martial Arts
		case D2S_FISTSOFFIRE:
			lpBuffer->nClass = CLASS_ASN;
			//lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			//lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fists of Fire"), SPELL_MAX_NAME);
			break;

		case D2S_CLAWSOFTHUNDER:
			lpBuffer->nClass = CLASS_ASN;
			//lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			//lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Claws of Thunder"), SPELL_MAX_NAME);
			break;

		case D2S_BLADESOFICE:
			lpBuffer->nClass = CLASS_ASN;
			//lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			//lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blades of Ice"), SPELL_MAX_NAME);
			break;

		case D2S_TIGERSTRIKE:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Tiger Strike"), SPELL_MAX_NAME);
			break;

		case D2S_COBRASTRIKE:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cobra Strike"), SPELL_MAX_NAME);
			break;

		case D2S_PHOENIXSTRIKE:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Phoenix Strike"), SPELL_MAX_NAME);
			break;

		case D2S_DRAGONTALON:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dragon Talon"), SPELL_MAX_NAME);
			break;

		case D2S_DRAGONCLAW:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dragon Claw"), SPELL_MAX_NAME);
			break;

		case D2S_DRAGONTAIL:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dragon Tail"), SPELL_MAX_NAME);
			break;

		case D2S_DRAGONFLIGHT:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwCoolDown = 2000;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			//lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dragon Flight"), SPELL_MAX_NAME);
			break;


// Shadow Disciplines
		case D2S_BURSTOFSPEED:			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

			_tcsncpy(lpBuffer->szSpellName, _T("Burst of Speed"), SPELL_MAX_NAME);
			break;

		case D2S_FADE:			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

			_tcsncpy(lpBuffer->szSpellName, _T("Fade"), SPELL_MAX_NAME);
			break;

		case D2S_VENOM:
			lpBuffer->nClass = CLASS_ASN;	
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

			_tcsncpy(lpBuffer->szSpellName, _T("Venom"), SPELL_MAX_NAME);
			break;

		case D2S_CLAWMASTERY:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
	
			_tcsncpy(lpBuffer->szSpellName, _T("Claw Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_WEAPONBLOCK:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Weapon Block"), SPELL_MAX_NAME);
			break;

		case D2S_SHADOWWARRIOR:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

			_tcsncpy(lpBuffer->szSpellName, _T("Shadow Warrior"), SPELL_MAX_NAME);
			break;

		case D2S_SHADOWMASTER:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shadow Master"), SPELL_MAX_NAME);
			break;

		case D2S_PSYCHICHAMMER:			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Psychic Hammer"), SPELL_MAX_NAME);
			break;

		case D2S_CLOAKOFSHADOWS:			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cloak of Shadows"), SPELL_MAX_NAME);
			break;

		case D2S_MINDBLAST:			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Mind Blast"), SPELL_MAX_NAME);
			break;

// Traps
		case D2S_SHOCKWEB:			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shock Web"), SPELL_MAX_NAME);
			break;

		case D2S_CHARGEDBOLTSENTRY:
			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Charged Bolt Sentry"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNINGSENTRY:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Sentry"), SPELL_MAX_NAME);
			break;

		case D2S_DEATHSENTRY:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Death Sentry"), SPELL_MAX_NAME);
			break;

		case D2S_FIREBLAST:			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Blast"), SPELL_MAX_NAME);
			break;

		case D2S_WAKEOFFIRE:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Wake of Fire"), SPELL_MAX_NAME);
			break;

		case D2S_WAKEOFINFERNO:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Wake of Inferno"), SPELL_MAX_NAME);
			break;

		case D2S_BLADESENTINEL:			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blade Sentinel"), SPELL_MAX_NAME);
			break;

		case D2S_BLADEFURY:
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blade Fury"), SPELL_MAX_NAME);
			break;

		case D2S_BLADESHIELD:			
			lpBuffer->nClass = CLASS_ASN;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;				
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

			_tcsncpy(lpBuffer->szSpellName, _T("Blade Shield"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Barbarian Skills
///////////////////////////////////////////////////////////////
		
// Warcries
		case D2S_HOWL:			
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Howl"), SPELL_MAX_NAME);
			break;

		case D2S_TAUNT:			
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Taunt"), SPELL_MAX_NAME);
			break;

		case D2S_BATTLECRY:			
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Battle Cry"), SPELL_MAX_NAME);
			break;

		case D2S_WARCRY:			
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;

			_tcsncpy(lpBuffer->szSpellName, _T("War Cry"), SPELL_MAX_NAME);
			break;

		case D2S_SHOUT:			
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shout"), SPELL_MAX_NAME);
			break;

		case D2S_BATTLEORDERS:			
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Battle Orders"), SPELL_MAX_NAME);
			break;

		case D2S_BATTLECOMMAND:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Battle Command"), SPELL_MAX_NAME);
			break;

		case D2S_FINDPOTION:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Find Potion"), SPELL_MAX_NAME);
			break;

		case D2S_FINDITEM:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Find Item"), SPELL_MAX_NAME);
			break;

		case D2S_GRIMWARD:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Grim Ward"), SPELL_MAX_NAME);
			break;


// Combat Masteries
		case D2S_ES_BARBTHUNDERSTORM:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Thunder Storm (BAR)"), SPELL_MAX_NAME);
			break;

		case D2S_SWORDMASTERY:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Sword Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_POLEARMMASTERY:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Pole Arm Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_INCREASEDSTAMINA:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Increased Stamin"), SPELL_MAX_NAME);
			break;

		case D2S_INCREASEDSPEED:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Increased Speed"), SPELL_MAX_NAME);
			break;

		case D2S_AXEMASTERY:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Axe Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_THROWINGMASTERY:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Throwing Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_MACEMASTERY:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

			_tcsncpy(lpBuffer->szSpellName, _T("Mace Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_SPEARMASTERY:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Spear Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_IRONSKIN:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Iron Skin"), SPELL_MAX_NAME);
			break;

		case D2S_NATURALRESISTANCE:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Natural Resistance"), SPELL_MAX_NAME);
			break;

// Combat Skills
		case D2S_LEAP:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Leap"), SPELL_MAX_NAME);
			break;

		case D2S_LEAPATTACK:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Leap Attack"), SPELL_MAX_NAME);
			break;

		case D2S_WHIRLWIND:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Whirlwind"), SPELL_MAX_NAME);
			break;

		case D2S_BASH:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bash"), SPELL_MAX_NAME);
			break;

		case D2S_STUN:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Stun"), SPELL_MAX_NAME);
			break;

		case D2S_CONCENTRATE:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Concentrate"), SPELL_MAX_NAME);
			break;

		case D2S_BERSERK:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Berserk"), SPELL_MAX_NAME);
			break;

		case D2S_DOUBLESWING:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Double Swing"), SPELL_MAX_NAME);
			break;

		case D2S_DOUBLETHROW:			
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Double Throw"), SPELL_MAX_NAME);
			break;

		case D2S_FRENZY:
			lpBuffer->nClass = CLASS_BAR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Frenzy"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Druid Skills
///////////////////////////////////////////////////////////////
		
// Elemental
		case D2S_FIRESTORM:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Firestorm"), SPELL_MAX_NAME);
			break;

		case D2S_MOLTENBOULDER:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwCoolDown = 2000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Molten Boulder"), SPELL_MAX_NAME);
			break;

		case D2S_FISSURE:			
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwCoolDown = 2000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fissure"), SPELL_MAX_NAME);
			break;

		case D2S_VOLCANO:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwCoolDown = 4000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Volcano"), SPELL_MAX_NAME);
			break;

		case D2S_ARMAGEDDON:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwCoolDown = 6000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Armageddon"), SPELL_MAX_NAME);
			break;

		case D2S_TWISTER:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Twister"), SPELL_MAX_NAME);
			break;

		case D2S_TORNADO:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Tornado"), SPELL_MAX_NAME);
			break;

		case D2S_HURRICANE:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwCoolDown = 6000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Hurricane"), SPELL_MAX_NAME);
			break;

		case D2S_ARCTICBLAST:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Arctic Blast"), SPELL_MAX_NAME);
			break;

		case D2S_CYCLONEARMOR:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cyclone Armor"), SPELL_MAX_NAME);
			break;


// Shape Shifting
		case D2S_WEREWOLF:			
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			lpBuffer->dwCoolDown = 1000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Werewolf"), SPELL_MAX_NAME);
			break;

		case D2S_FERALRAGE:			
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Feral Rage"), SPELL_MAX_NAME);
			break;

		case D2S_RABIES:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Rabies"), SPELL_MAX_NAME);
			break;

		case D2S_FURY:			
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fury"), SPELL_MAX_NAME);
			break;

		case D2S_LYCANTHROPY:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lycanthropy"), SPELL_MAX_NAME);
			break;

		case D2S_FIRECLAWS:			
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Claws"), SPELL_MAX_NAME);
			break;

		case D2S_HUNGER:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Hunger"), SPELL_MAX_NAME);
			break;

		case D2S_WEREBEAR:			
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
			lpBuffer->dwCoolDown = 1000;		
			
			_tcsncpy(lpBuffer->szSpellName, _T("Werebear"), SPELL_MAX_NAME);
			break;

		case D2S_MAUL:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Maul"), SPELL_MAX_NAME);
			break;

		case D2S_SHOCKWAVE:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shock Wave"), SPELL_MAX_NAME);
			break;


// Summoning
		case D2S_OAKSAGE:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Oak Sage"), SPELL_MAX_NAME);
			break;

		case D2S_HEARTOFWOLVERINE:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Heart of Wolverine"), SPELL_MAX_NAME);
			break;

		case D2S_SPIRITOFBARBS:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Spirit of Barbs"), SPELL_MAX_NAME);
			break;

		case D2S_RAVEN:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Raven"), SPELL_MAX_NAME);
			break;

		case D2S_SUMMONSPIRITWOLF:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Summon Spirit Wolf"), SPELL_MAX_NAME);
			break;

		case D2S_SUMMONDIREWOLF:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Summon Dire Wolf"), SPELL_MAX_NAME);
			break;

		case D2S_SUMMONGRIZZLY:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Summon Grizzly"), SPELL_MAX_NAME);
			break;

		case D2S_POISONCREEPER:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Creeper"), SPELL_MAX_NAME);
			break;

		case D2S_CARRIONVINE:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Carrion Vine"), SPELL_MAX_NAME);
			break;

		case D2S_SOLARCREEPER:
			lpBuffer->nClass = CLASS_DRU;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Solar Creeper"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Necromaner Skills
///////////////////////////////////////////////////////////////
		
// Summoning Spells
		case D2S_SKELETONMASTERY:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Skeleton Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_GOLEMMASTERY:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Golem Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_SUMMONRESIST:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Summon Resist"), SPELL_MAX_NAME);
			break;

		case D2S_CLAYGOLEM:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Clay Golem"), SPELL_MAX_NAME);
			break;

		case D2S_BLOODGOLEM:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blood Golem"), SPELL_MAX_NAME);
			break;

		case D2S_IRONGOLEM:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Iron Golem"), SPELL_MAX_NAME);
			break;

		case D2S_FIREGOLEM:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Golem"), SPELL_MAX_NAME);
			break;

		case D2S_RAISESKELETON:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Raise Skeleton"), SPELL_MAX_NAME);
			break;

		case D2S_RAISESKELETALMAGE:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Raise Skeletal Mage"), SPELL_MAX_NAME);
			break;

		case D2S_REVIVE:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Revive"), SPELL_MAX_NAME);
			break;


// Poison and Bone Spells
		case D2S_POISONDAGGER:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Dagger"), SPELL_MAX_NAME);
			break;

		case D2S_POISONEXPLOSION:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Explosion"), SPELL_MAX_NAME);
			break;

		case D2S_POISONNOVA:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_POISON;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Poison Nova"), SPELL_MAX_NAME);
			break;

		case D2S_TEETH:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Teeth"), SPELL_MAX_NAME);
			break;

		case D2S_CORPOSEEXPLOSION:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Corpose Explosion"), SPELL_MAX_NAME);
			break;

		case D2S_BONESPEAR:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Spear"), SPELL_MAX_NAME);
			break;

		case D2S_BONESPIRIT:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Spirit"), SPELL_MAX_NAME);
			break;

		case D2S_BONEARMOR:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Armor"), SPELL_MAX_NAME);
			break;

		case D2S_BONEWALL:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Wall"), SPELL_MAX_NAME);
			break;

		case D2S_BONEPRISON:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Bone Prison"), SPELL_MAX_NAME);
			break;


// Curses
		case D2S_DIMVISION:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Dim Vision"), SPELL_MAX_NAME);
			break;

		case D2S_CONFUSE:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Confuse"), SPELL_MAX_NAME);
			break;

		case D2S_ATTRACT:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Attract"), SPELL_MAX_NAME);
			break;

		case D2S_AMPLIFYDAMAGE:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Amplify Damage"), SPELL_MAX_NAME);
			break;

		case D2S_IRONMAIDEN:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Iron Maiden"), SPELL_MAX_NAME);
			break;

		case D2S_LIFETAP:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Life Tap"), SPELL_MAX_NAME);
			break;

		case D2S_LOWERRESIST:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lower Resist"), SPELL_MAX_NAME);
			break;

		case D2S_WEAKEN:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Weaken"), SPELL_MAX_NAME);
			break;

		case D2S_TERROR:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Terror"), SPELL_MAX_NAME);
			break;

		case D2S_DECREPIFY:
			lpBuffer->nClass = CLASS_NEC;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Decrepify"), SPELL_MAX_NAME);
			break;



///////////////////////////////////////////////////////////////
//    Paladin Skills
///////////////////////////////////////////////////////////////
		
// Defensive Auras
		case D2S_PRAYER:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Prayer"), SPELL_MAX_NAME);
			break;

		case D2S_CLEANSING:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cleansing"), SPELL_MAX_NAME);
			break;

		case D2S_MEDITATION:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Meditation"), SPELL_MAX_NAME);
			break;

		case D2S_DEFIANCE:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Defiance"), SPELL_MAX_NAME);
			break;

		case D2S_VIGOR:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Vigor"), SPELL_MAX_NAME);
			break;

		case D2S_REDEMPTION:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Redemption"), SPELL_MAX_NAME);
			break;

		case D2S_RESISTFIRE:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Resist Fire"), SPELL_MAX_NAME);
			break;

		case D2S_RESISTCOLD:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Resist Cold"), SPELL_MAX_NAME);
			break;

		case D2S_RESISTLIGHTNING:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Resist Lightning"), SPELL_MAX_NAME);
			break;

		case D2S_SALVATION:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Salvation"), SPELL_MAX_NAME);
			break;


// Offensive Auras
		case D2S_MIGHT:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Might"), SPELL_MAX_NAME);
			break;

		case D2S_BLESSEDAIM:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blessed Aim"), SPELL_MAX_NAME);
			break;

		case D2S_CONCENTRATION:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Concentration"), SPELL_MAX_NAME);
			break;

		case D2S_FANATICISM:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fanaticism"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYFIRE:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Fire"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYFREEZE:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Freeze"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYSHOCK:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Shock"), SPELL_MAX_NAME);
			break;

		case D2S_THORNS:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Thorns"), SPELL_MAX_NAME);
			break;

		case D2S_SANCTUARY:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Sanctuary"), SPELL_MAX_NAME);
			break;

		case D2S_CONVICTION:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Conviction"), SPELL_MAX_NAME);
			break;


// Combat Skills
		case D2S_SACRIFICE:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Sacrifice"), SPELL_MAX_NAME);
			break;

		case D2S_ZEAL:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Zeal"), SPELL_MAX_NAME);
			break;

		case D2S_VENGEANCE:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Vengeance"), SPELL_MAX_NAME);
			break;

		case D2S_CONVERSION:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;				
			
			_tcsncpy(lpBuffer->szSpellName, _T("Conversion"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYBOLT:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_BLESSEDHAMMER:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blessed Hammer"), SPELL_MAX_NAME);
			break;

		case D2S_FISTOFTHEHEAVENS:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwCoolDown = 1000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fist of The Heavens"), SPELL_MAX_NAME);
			break;

		case D2S_SMITE:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Smite"), SPELL_MAX_NAME);
			break;

		case D2S_CHARGE:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
			
			_tcsncpy(lpBuffer->szSpellName, _T("Charge"), SPELL_MAX_NAME);
			break;

		case D2S_HOLYSHIELD:
			lpBuffer->nClass = CLASS_PAL;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Holy Shield"), SPELL_MAX_NAME);
			break;


///////////////////////////////////////////////////////////////
//    Sorceress Skills
///////////////////////////////////////////////////////////////
		
// Cold Spells
		case D2S_FROSTNOVA:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Frost Nova"), SPELL_MAX_NAME);
			break;

		case D2S_BLIZZARD:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwCoolDown = 1800;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blizzard"), SPELL_MAX_NAME);
			break;

		case D2S_FROZENORB:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwCoolDown = 1000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Frozen Orb"), SPELL_MAX_NAME);
			break;

		case D2S_ICEBOLT:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Ice Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_ICEBLAST:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Ice Blast"), SPELL_MAX_NAME);
			break;

		case D2S_GLACIALSPIKE:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_COLD;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Glacial Spike"), SPELL_MAX_NAME);
			break;

		case D2S_COLDMASTERY:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Cold Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_FROZENARMOR:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Frozen Armor"), SPELL_MAX_NAME);
			break;

		case D2S_SHIVERARMOR:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Shiver Armor"), SPELL_MAX_NAME);
			break;

		case D2S_CHILLINGARMOR:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Chilling Armor"), SPELL_MAX_NAME);
			break;

// Lightning Spells
		case D2S_STATICFIELD:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Static Field"), SPELL_MAX_NAME);
			break;

		case D2S_NOVA:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Nova"), SPELL_MAX_NAME);
			break;

		case D2S_THUNDERSTORM:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Thunder Storm"), SPELL_MAX_NAME);
			break;

		case D2S_CHARGEDBOLT:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Charged Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNING:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning"), SPELL_MAX_NAME);
			break;

		case D2S_CHAINLIGHTNING:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Chain Lightning"), SPELL_MAX_NAME);
			break;

		case D2S_LIGHTNINGMASTERY:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Lightning Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_TELEKINESIS:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
			lpBuffer->dwSpellInfoFlag |= DSI_STUN;
			lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
			
			_tcsncpy(lpBuffer->szSpellName, _T("Telekinesis"), SPELL_MAX_NAME);
			break;

		case D2S_TELEPORT:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Teleport"), SPELL_MAX_NAME);
			break;

		case D2S_ENERGYSHIELD:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Energy Shield"), SPELL_MAX_NAME);
			break;


// Fire Spells
		case D2S_INFERNO:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Inferno"), SPELL_MAX_NAME);
			break;

		case D2S_BLAZE:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Blaze"), SPELL_MAX_NAME);
			break;

		case D2S_FIREWALL:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwCoolDown = 1400;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Wall"), SPELL_MAX_NAME);
			break;

		case D2S_FIREBOLT:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Bolt"), SPELL_MAX_NAME);
			break;

		case D2S_FIREBALL:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Ball"), SPELL_MAX_NAME);
			break;

		case D2S_METEOR:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
			lpBuffer->dwCoolDown = 1200;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Meteor"), SPELL_MAX_NAME);
			break;

		case D2S_FIREMASTERY:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Fire Mastery"), SPELL_MAX_NAME);
			break;

		case D2S_WARMTH:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Warmth"), SPELL_MAX_NAME);
			break;

		case D2S_ENCHANT:
			lpBuffer->nClass = CLASS_SOR;
			//lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN; // no longer in 1.10
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Enchant"), SPELL_MAX_NAME);
			break;

		case D2S_HYDRA:
			lpBuffer->nClass = CLASS_SOR;
			lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
			lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
			lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
			lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
			lpBuffer->dwCoolDown = 2000;
			
			_tcsncpy(lpBuffer->szSpellName, _T("Hydra"), SPELL_MAX_NAME);
			break;


		default:
			lpBuffer->nClass = CLASS_NA;
			lpBuffer->wSpellID = D2S_INVALID;
			return FALSE;
			break;
	}

	return TRUE;
}
