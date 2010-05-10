using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Collections;

namespace esCharView
{
	class Character
	{
		private byte[] headerBytes;
		private uint[] statValues = new uint[16];
		private byte characterFlags;

		/// <summary>
		/// Character classes
		/// </summary>
		public enum CharacterClass
		{
			Amazon,
			Sorceress,
			Necromancer,
			Paladin,
			Barbarian,
			Druid,
			Assassin,
			Unknown
		}

		/// <summary>
		/// Character's name
		/// </summary>
		public String Name
		{
			get
			{
				return ASCIIEncoding.ASCII.GetString(headerBytes, 0x14, 16);
			}
			set
			{
				byte[] paddedName = new byte[16];
				byte[] newName = ASCIIEncoding.ASCII.GetBytes(value);

				//15 instead of 16 to keep trailing null character
				Array.Copy(newName, paddedName, newName.Length < 15 ? newName.Length : 15);
				Array.Copy(paddedName, 0, headerBytes, 0x14, paddedName.Length);
			}
		}

		/// <summary>
		/// Character Stat Types
		/// </summary>
		enum StatTypes
		{
			Strength,
			Energy,
			Dexterity,
			Vitality,
			StatPoints,
			SkillPoints,
			Hitpoints,
			BaseHitpoints,
			Mana,
			BaseMana,
			Stamina,
			BaseStamina,
			Level,
			Experience,
			Gold,
			GoldBank,
		}

		public uint Strength
		{
			get { return statValues[(int)StatTypes.Strength]; }
			set { statValues[(int)StatTypes.Strength] = value; }
		}
		public uint Energy
		{
			get { return statValues[(int)StatTypes.Energy]; }
			set { statValues[(int)StatTypes.Energy] = value; }
		}
		public uint Dexterity
		{
			get { return statValues[(int)StatTypes.Dexterity]; }
			set { statValues[(int)StatTypes.Dexterity] = value; }
		}
		public uint Vitality
		{
			get { return statValues[(int)StatTypes.Vitality]; }
			set { statValues[(int)StatTypes.Vitality] = value; }
		}
		public uint StatPoints
		{
			get { return statValues[(int)StatTypes.StatPoints]; }
			set { statValues[(int)StatTypes.StatPoints] = value; }
		}
		public uint SkillPoints
		{
			get { return statValues[(int)StatTypes.SkillPoints]; }
			set { statValues[(int)StatTypes.SkillPoints] = value; }
		}
		public uint Hitpoints
		{
			get { return statValues[(int)StatTypes.Hitpoints]; }
			set { statValues[(int)StatTypes.Hitpoints] = value; }
		}
		public uint BaseHitpoints
		{
			get { return statValues[(int)StatTypes.BaseHitpoints]; }
			set { statValues[(int)StatTypes.BaseHitpoints] = value; }
		}
		public uint Mana
		{
			get { return statValues[(int)StatTypes.Mana]; }
			set { statValues[(int)StatTypes.Mana] = value; }
		}
		public uint BaseMana
		{
			get { return statValues[(int)StatTypes.BaseMana]; }
			set { statValues[(int)StatTypes.BaseMana] = value; }
		}
		public uint Stamina
		{
			get { return statValues[(int)StatTypes.Stamina]; }
			set { statValues[(int)StatTypes.Stamina] = value; }
		}
		public uint BaseStamina
		{
			get { return statValues[(int)StatTypes.BaseStamina]; }
			set { statValues[(int)StatTypes.BaseStamina] = value; }
		}
		public uint Level
		{
			get { return statValues[(int)StatTypes.Level]; }
			set { statValues[(int)StatTypes.Level] = value; }
		}
		public uint Experience
		{
			get { return statValues[(int)StatTypes.Experience]; }
			set { statValues[(int)StatTypes.Experience] = value; }
		}
		public uint Gold
		{
			get { return statValues[(int)StatTypes.Gold]; }
			set { statValues[(int)StatTypes.Gold] = value; }
		}
		public uint GoldBank
		{
			get { return statValues[(int)StatTypes.GoldBank]; }
			set { statValues[(int)StatTypes.GoldBank] = value; }
		}

		public CharacterClass Class
		{
			get
			{
				switch (headerBytes[40])
				{
					case 0:
						return CharacterClass.Amazon;
					case 1:
						return CharacterClass.Sorceress;
					case 2:
						return CharacterClass.Necromancer;
					case 3:
						return CharacterClass.Paladin;
					case 4:
						return CharacterClass.Barbarian;
					case 5:
						return CharacterClass.Druid;
					case 6:
						return CharacterClass.Assassin;
					default:
						return CharacterClass.Unknown;
				}
			}
			set
			{
				if (value == CharacterClass.Unknown)
				{
					return;
				}

				headerBytes[40] = (byte)value;
			}
		}


		/// <summary>
		/// Chracter has a mercenary
		/// </summary>
		public bool HasMercenary
		{
			get
			{
				return BitConverter.ToInt32(headerBytes, 179) != 0;
			}
		}

		/// <summary>
		/// Character is in hardcore mode
		/// </summary>
		public bool Hardcore
		{
			get
			{
				return (characterFlags & 0x04) > 0;
			}
		}

		/// <summary>
		/// Character has died before
		/// </summary>
		public bool Died
		{
			get
			{
				return (characterFlags & 0x08) > 0;
			}
		}

		/// <summary>
		/// Character is expansion character
		/// </summary>
		public bool Expansion
		{
			get
			{
				return (characterFlags & 0x20) > 0;
			}
		}

		/// <summary>
		/// Collection of unknown flags
		/// </summary>
		public int UnknownFlags
		{
			get
			{
				return (characterFlags & 0xd3);
			}
		}


		public Character(byte[] characterBytes)
		{
			headerBytes = characterBytes;
			ReadStats(characterBytes);
			characterFlags = characterBytes[0x24];
		}

		/// <summary>
		/// Sets various character flags
		/// </summary>
		/// <param name="expansion">Character is expansion character</param>
		/// <param name="died">Character has died before</param>
		/// <param name="hardcore">Hardcore mode</param>
		/// <param name="unknownFlags">Other flags - 64 is common value for realm characters</param>
		public void SetCharacterFlags(bool expansion, bool died, bool hardcore, byte unknownFlags)
		{
			byte characterFlags = unknownFlags;

			if (expansion)
			{
				characterFlags |= 0x20;
			}
			if (died)
			{
				characterFlags |= 0x08;
			}
			if (hardcore)
			{
				characterFlags |= 0x04;
			}

			headerBytes[0x24] = characterFlags;
		}


		/// <summary>
		/// Copies character's raw stat data from raw header bytes
		/// </summary>
		/// <param name="headerBytes">Raw header data from save file</param>
		/// <returns>Raw stat data</returns>
		private byte[] GetStatsData(byte[] headerBytes)
		{
			byte[] statsSection;
			int statsSectionLength = 0;

			for (int i = 767; i < headerBytes.Length; i++)
			{
				if (headerBytes[i] == 0x69 && headerBytes[i + 1] == 0x66)
				{
					break;
				}

				statsSectionLength++;
			}

			statsSection = new byte[statsSectionLength];
			Array.Copy(headerBytes, 767, statsSection, 0, statsSection.Length);

			return statsSection;
		}

		/// <summary>
		/// Parses raw character stat data
		/// </summary>
		/// <param name="headerBytes">Raw characte stat data found between "gf" and "if" near offset 765 in save file</param>
		/// <remarks>Bit lengths of stat types are found in the CSvBits column of ItemStatCost.txt</remarks>
		/// Source: http://phrozenkeep.hugelaser.com/forum/viewtopic.php?f=8&t=9011&start=50
		private void ReadStats(byte[] headerBytes)
		{
			byte[] statsData = GetStatsData(headerBytes);
			BitReader br = new BitReader(statsData);

			while (br.Position < br.BitCount)
			{
				// ID of stat (See ItemStatCost.txt)
				byte statIndex = (byte)br.Read(9);
				// Value contains this many bits (See CSvBits in ItemStatCost.txt)
				int statValueBits = 0;
				// Value needs to be shifted by this amount
				int valShift = 0;

				switch ((StatTypes)statIndex)
				{
					case StatTypes.Strength:
					case StatTypes.Energy:
					case StatTypes.Dexterity:
					case StatTypes.Vitality:
						statValueBits = 11;
						break;
					case StatTypes.StatPoints:
						statValueBits = 10;
						break;
					case StatTypes.SkillPoints:
						statValueBits = 8;
						break;
					case StatTypes.Hitpoints:
					case StatTypes.BaseHitpoints:
					case StatTypes.Mana:
					case StatTypes.BaseMana:
					case StatTypes.Stamina:
					case StatTypes.BaseStamina:
						statValueBits = 21;
						valShift = 8;
						break;
					case StatTypes.Level:
						statValueBits = 7;
						break;
					case StatTypes.Experience:
						statValueBits = 32;
						break;
					case StatTypes.Gold:
					case StatTypes.GoldBank:
						statValueBits = 25;
						break;
					default:
						return;
				}

				if (statValueBits == 0)
				{
					break;
				}

				uint statValue = br.Read(statValueBits);

				statValues[statIndex] = (statValue >> valShift);
			}
		}

		public byte[] GetCharacterBytes()
		{
			return headerBytes;
		}
	}
}
