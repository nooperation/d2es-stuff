using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using BKSystem.IO;

namespace CharacterEditor
{
	/// <summary>
	/// Controls access to character's stats
	/// </summary>
	public class Stat
	{
		/// <summary>
		/// Raw stat data from save file
		/// </summary>
		private byte[] statsBytes;
		/// <summary>
		/// Decoded stat values
		/// </summary>
		private Dictionary<int, int> statValues = new Dictionary<int, int>();
		/// <summary>
		/// Remaining bits that need to be written (Each a full byte)
		/// </summary>
		private byte[] remainingBytes;
		/// <summary>
		/// Remaining bits that need to be written after remainingBytes (Not a full byte)
		/// </summary>
		private byte remainingBits;
		/// <summary>
		/// Total number of bits in remainingBits
		/// </summary>
		private int remainingBitsCount;

		public Stat(byte[] statsBytes)
		{
			this.statsBytes = statsBytes;
			ReadStats();
		}

		// TODO: Get rid of all of these individual properties?
		/// <summary>
		/// Base value of strength
		/// </summary>
		public int Strength
		{
			get { return GetStatValue("strength"); }
			set { SetStatValue("strength", value); }
		}
		/// <summary>
		/// Base value of energy
		/// </summary>
		public int Energy
		{
			get { return GetStatValue("energy"); }
			set { SetStatValue("energy", value); }
		}
		/// <summary>
		/// Base value of dexterity
		/// </summary>
		public int Dexterity
		{
			get { return GetStatValue("dexterity"); }
			set { SetStatValue("dexterity", value); }
		}
		/// <summary>
		/// Base value of vitality
		/// </summary>
		public int Vitality
		{
			get { return GetStatValue("vitality"); }
			set { SetStatValue("vitality", value); }
		}
		/// <summary>
		/// Number of unallocated stat points
		/// </summary>
		public int StatPoints
		{
			get { return GetStatValue("statpts"); }
			set { SetStatValue("statpts", value); }
		}
		/// <summary>
		/// Number of unallocated skill points
		/// </summary>
		public int SkillPoints
		{
			get { return GetStatValue("newskills"); }
			set { SetStatValue("newskills", value); }
		}
		/// <summary>
		/// Current value of hitpoints
		/// </summary>
		/// <remarks>Current value is usually higher than base value</remarks>
		public int Hitpoints
		{
			get { return GetStatValue("hitpoints"); }
			set { SetStatValue("hitpoints", value); }
		}
		/// <summary>
		/// Base value of hitpoints
		/// </summary>
		public int BaseHitpoints
		{
			get { return GetStatValue("maxhp"); }
			set { SetStatValue("maxhp", value); }
		}
		/// <summary>
		/// Current value of mana
		/// </summary>
		/// <remarks>Current value is usually higher than base value</remarks>
		public int Mana
		{
			get { return GetStatValue("mana"); }
			set { SetStatValue("mana", value); }
		}
		/// <summary>
		/// Base value of mana
		/// </summary>
		public int BaseMana
		{
			get { return GetStatValue("maxmana"); }
			set { SetStatValue("maxmana", value); }
		}
		/// <summary>
		/// Current value of stamina.
		/// </summary>
		/// <remarks>Current value is usually higher than base value</remarks>
		public int Stamina
		{
			get { return GetStatValue("stamina"); }
			set { SetStatValue("stamina", value); }
		}
		/// <summary>
		/// Base value of stamina
		/// </summary>
		public int BaseStamina
		{
			get { return GetStatValue("maxstamina"); }
			set { SetStatValue("maxstamina", value); }
		}
		/// <summary>
		/// Character's level
		/// </summary>
		public int Level
		{
			get { return GetStatValue("level"); }
			set { SetStatValue("level", value); }
		}
		/// <summary>
		/// Number of experience points character has
		/// </summary>
		public uint Experience
		{
			get { return (uint)GetStatValue("experience"); }
			set { SetStatValue("experience", (int)value); }
		}
		/// <summary>
		/// Amount of gold character has in inventory
		/// </summary>
		public uint Gold
		{
			get { return (uint)GetStatValue("gold"); }
			set { SetStatValue("gold", (int)value); }
		}
		/// <summary>
		/// Amount of gold character has in the bank
		/// </summary>
		public uint GoldBank
		{
			get { return (uint)GetStatValue("goldbank"); }
			set { SetStatValue("goldbank", (int)value); }
		}
		/// <summary>
		/// Number of kills
		/// </summary>
		public int KillCount
		{
			get { return GetStatValue("kill_counter"); }
			set { SetStatValue("kill_counter", value); }
		}
		/// <summary>
		/// Times character has died
		/// </summary>
		public int DeathCount
		{
			get { return GetStatValue("death_counter"); }
			set { SetStatValue("death_counter", value); }
		}

		/// <summary>
		/// Gets the specified stat's value
		/// </summary>
		/// <param name="name">Name of stat</param>
		/// <returns>Value of stat or 0 if stat is not present</returns>
		private int GetStatValue(string name)
		{
			int statId = ItemDefs.ItemStatCostsByName[name].ID;

			if(!statValues.ContainsKey(statId))
			{
				return 0;
			}

			return statValues[statId];
		}

		/// <summary>
		/// Sets the specified stat to a given value or do nothing if stat is not present
		/// </summary>
		/// <param name="name">Name of stat</param>
		/// <param name="value">Value to set stat to</param>
		private void SetStatValue(string name, int value)
		{
			int statId = ItemDefs.ItemStatCostsByName[name].ID;

			if (!statValues.ContainsKey(statId))
			{
				return;
			}

			statValues[statId] = value;
		}

		/// <summary>
		/// Parses raw character stat data
		/// </summary>
		/// <param name="headerBytes">Raw characte stat data found between "gf" and "if" near offset 765 in save file</param>
		/// <remarks>Bit lengths of stat types are found in the CSvBits column of ItemStatCost.txt</remarks>
		/// Source: http://phrozenkeep.hugelaser.com/forum/viewtopic.php?f=8&t=9011&start=50
		private void ReadStats()
		{
			BitStream bs = new BitStream(statsBytes);

			// Skip header bytes
			bs.SkipBits(8);
			bs.SkipBits(8);

			while (bs.RemainingBits >= 9)
			{
				// ID of stat (See ItemStatCost.txt)
				int statIndex = (int)bs.ReadReversed(9);
				// Value contains this many bits (See CSvBits in ItemStatCost.txt)
				int statValueBits = 0;
				// Value needs to be shifted by this amount
				int valShift = 0;

				if (statIndex == 0x1ff)
				{
					bs.Position -= 9;
					remainingBytes = new byte[(bs.RemainingBits / 8)];

					for (int i = 0; i < remainingBytes.Length; i++)
					{
						remainingBytes[i] = bs.ReadReversedByte();
					}

					remainingBitsCount = (int)(bs.RemainingBits);
					if (remainingBitsCount > 0)
					{
						remainingBits = (byte)bs.ReadReversed(remainingBitsCount);
					}

					break;
				}

				statValueBits = ItemDefs.ItemStatCostsById[statIndex].CSvBits;
				if (statValueBits == 0)
				{
					break;
				}

				valShift = ItemDefs.ItemStatCostsById[(int)statIndex].ValShift; 

				int statValue = (int)bs.ReadReversed(statValueBits);
				if (!statValues.ContainsKey(statIndex))
				{
					statValues.Add(statIndex, (statValue >> valShift));
				}
			}
		}

		/// <summary>
		/// Converts all stat data into raw data for save file
		/// </summary>
		/// <returns>Raw stat data ready for insertion into save file</returns>
		public byte[] GetStatBytes()
		{
			BitStream bits = new BitStream();

			bits.WriteReversed('g', 8);
			bits.WriteReversed('f', 8);

			foreach (var stat in statValues)
			{
				bits.WriteReversed(stat.Key, 9);

				int valShift = 0;
				int bitCount = ItemDefs.ItemStatCostsById[stat.Key].CSvBits;

				if (ItemDefs.ItemStatCostsById.ContainsKey(stat.Key))
				{
					valShift = ItemDefs.ItemStatCostsById[stat.Key].ValShift;
				}

				bits.WriteReversed((uint)((stat.Value << valShift)), bitCount);
			}

			// These last 2 bytes seem to be some sort of terminator?
			if (remainingBytes != null)
			{
				bits.WriteReversed(remainingBytes);
			}

			if (remainingBitsCount > 0)
			{
				bits.WriteReversed(remainingBits, remainingBitsCount);
			}

			return bits.ToReversedByteArray();
		}
	}
}
