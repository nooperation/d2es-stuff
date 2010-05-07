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
		private Inventory inventory;

		private byte characterFlags;
		private string filePath;

		public int HeaderSize { get { return headerBytes.Length; } }

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
			MaxHitpoints,
			Mana,
			MaxMana,
			Stamina,
			MaxStamina,
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
		public uint MaxHitpoints
		{
			get { return statValues[(int)StatTypes.MaxHitpoints]; }
			set { statValues[(int)StatTypes.MaxHitpoints] = value; }
		}
		public uint Mana
		{
			get { return statValues[(int)StatTypes.Mana]; }
			set { statValues[(int)StatTypes.Mana] = value; }
		}
		public uint MaxMana
		{
			get { return statValues[(int)StatTypes.MaxMana]; }
			set { statValues[(int)StatTypes.MaxMana] = value; }
		}
		public uint Stamina
		{
			get { return statValues[(int)StatTypes.Stamina]; }
			set { statValues[(int)StatTypes.Stamina] = value; }
		}
		public uint MaxStamina
		{
			get { return statValues[(int)StatTypes.MaxStamina]; }
			set { statValues[(int)StatTypes.MaxStamina] = value; }
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

		/// <summary>
		/// Character's inventory
		/// </summary>
		public Inventory Inventory
		{
			get
			{
				return inventory;
			}
			set
			{
				inventory = value;
			}
		}


		public Character()
		{

		}

		/// <summary>
		/// Read character save from disk
		/// </summary>
		/// <param name="filePath">Path of save file</param>
		public void Read(string filePath)
		{
			this.filePath = filePath;

			ReadHeaders();
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
		/// Saves player data to specified path
		/// </summary>
		/// <param name="filePath">Path to save character data as</param>
		public void Write(string filePath)
		{
			byte[] inventoryBytes = Inventory.GetInventoryBytes(HasMercenary);
			///UpdateInventoryHeaders();

			byte[] rawCharacterData = new byte[headerBytes.Length + inventoryBytes.Length];

			Array.Copy(headerBytes, rawCharacterData, headerBytes.Length);
			Array.Copy(inventoryBytes, 0, rawCharacterData, headerBytes.Length, inventoryBytes.Length);

			FixHeaders(ref rawCharacterData);

			File.WriteAllBytes(filePath, rawCharacterData);
		}

		/// <summary>
		/// Saves the player data to original file
		/// </summary>
		public void Write()
		{
			Write(filePath);
		}

		/// <summary>
		/// Splits character data into several sections for easier parsing
		/// </summary>
		private void ReadHeaders()
		{
			byte[] rawCharacterData = File.ReadAllBytes(filePath);
			int itemListBegin = FindItemListBegin(rawCharacterData);

			headerBytes = new byte[itemListBegin];
			byte[] inventoryBytes = new byte[rawCharacterData.Length - itemListBegin];

			Array.Copy(rawCharacterData, 0, headerBytes, 0, headerBytes.Length);
			Array.Copy(rawCharacterData, itemListBegin, inventoryBytes, 0, inventoryBytes.Length);

			ReadStats(headerBytes);

			inventory = new Inventory(inventoryBytes);
			characterFlags = headerBytes[0x24];
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
					case StatTypes.MaxHitpoints:
					case StatTypes.Mana:
					case StatTypes.MaxMana:
					case StatTypes.Stamina:
					case StatTypes.MaxStamina:
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

		/// <summary>
		/// Corrects checksum of new player data
		/// </summary>
		/// <param name="rawCharacterData">Raw player save data</param>
		public void FixHeaders(ref byte[] rawCharacterData)
		{
			byte[] fileSizeBytes = BitConverter.GetBytes(rawCharacterData.Length);
			Array.Copy(fileSizeBytes, 0, rawCharacterData, 8, 4);

			uint checksum = CalculateChecksum(rawCharacterData);

			byte[] checksumBytes = BitConverter.GetBytes(checksum);
			Array.Copy(checksumBytes, 0, rawCharacterData, 12, 4);
		}

		// 
		/// <summary>
		/// Calculates a new checksum for specified data
		/// </summary>
		/// <param name="fileBytes">Raw character data</param>
		/// <returns>Checksum for specified data</returns>
		/// <remarks>Source: ehertlein ( http://forums.diii.net/showthread.php?t=532037&page=41 )</remarks>
		private static uint CalculateChecksum(byte[] fileBytes)
		{
			uint hexTest = 0x80000000;
			uint checksum = 0;

			// clear out the old checksum
			fileBytes[12] = 0;
			fileBytes[13] = 0;
			fileBytes[14] = 0;
			fileBytes[15] = 0;

			foreach (byte currentByte in fileBytes)
			{
				if ((checksum & hexTest) == hexTest)
				{
					checksum = checksum << 1;
					checksum = checksum + 1;
				}
				else
				{
					checksum = checksum << 1;
				}

				checksum += currentByte;
			}

			return checksum;
		}

		/// <summary>
		/// Returns the location of the inventory data
		/// </summary>
		/// <param name="rawCharacterData">Raw bytes from save file</param>
		/// <returns>Location of inventory data</returns>
		private static int FindItemListBegin(byte[] rawCharacterData)
		{
			for (int i = 768; i < rawCharacterData.Length - 1; i++)
			{
				if (rawCharacterData[i] == 'J' && rawCharacterData[i + 1] == 'M')
				{
					return i;
				}
			}

			return 0;
		}
	}
}
