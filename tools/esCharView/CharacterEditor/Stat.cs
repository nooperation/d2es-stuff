using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using BKSystem.IO;

//TODO: Read information directly from ItemStatCost.txt so it's more compatible
namespace CharacterEditor
{
	/// <summary>
	/// Controls access to character's stats
	/// </summary>
	public class Stat : IEnumerable<KeyValuePair<Stat.StatTypes, uint>>
	{
		/// <summary>
		/// Raw stat data from save file
		/// </summary>
		private byte[] statsBytes;
		/// <summary>
		/// Decoded stat values
		/// </summary>
		private Dictionary<StatTypes, uint> statValues = new Dictionary<StatTypes, uint>();
		/// <summary>
		/// Number of bits each stat requires
		/// </summary>
		private Dictionary<StatTypes, int> statValueBitCounts = new Dictionary<StatTypes, int>();
		/// <summary>
		/// Specific stats that need to be shifted by the specified amount
		/// </summary>
		private Dictionary<StatTypes, int> statValueBitShifts = new Dictionary<StatTypes, int>();
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
			//TODO: Read from file
			statValueBitCounts.Add(StatTypes.Strength, 11);
			statValueBitCounts.Add(StatTypes.Energy, 11);
			statValueBitCounts.Add(StatTypes.Dexterity, 11);
			statValueBitCounts.Add(StatTypes.Vitality, 11);
			statValueBitCounts.Add(StatTypes.StatPoints, 10);
			statValueBitCounts.Add(StatTypes.SkillPoints, 8);
			statValueBitCounts.Add(StatTypes.Hitpoints, 21);
			statValueBitCounts.Add(StatTypes.BaseHitpoints, 21);
			statValueBitCounts.Add(StatTypes.Mana, 21);
			statValueBitCounts.Add(StatTypes.BaseMana, 21);
			statValueBitCounts.Add(StatTypes.Stamina, 21);
			statValueBitCounts.Add(StatTypes.BaseStamina, 21);
			statValueBitCounts.Add(StatTypes.Level, 7);
			statValueBitCounts.Add(StatTypes.Experience, 32);
			statValueBitCounts.Add(StatTypes.Gold, 25);
			statValueBitCounts.Add(StatTypes.GoldBank, 25);
			statValueBitCounts.Add(StatTypes.KillCount, 32);
			statValueBitCounts.Add(StatTypes.DeathCount, 32);

			statValueBitShifts.Add(StatTypes.Hitpoints, 8);
			statValueBitShifts.Add(StatTypes.BaseHitpoints, 8);
			statValueBitShifts.Add(StatTypes.Mana, 8);
			statValueBitShifts.Add(StatTypes.BaseMana, 8);
			statValueBitShifts.Add(StatTypes.Stamina, 8);
			statValueBitShifts.Add(StatTypes.BaseStamina, 8);

			this.statsBytes = statsBytes;
			ReadStats();
		}

		/// <summary>
		/// Character Stat Types
		/// </summary>
		public enum StatTypes
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
			KillCount = 192,
			DeathCount = 249,
		}

		/// <summary>
		/// Get or set specified stat
		/// </summary>
		/// <param name="index">Type of stat to access. Non case sensitive string of a StatTypes type.</param>
		/// <returns>Value of specified stat</returns>
		public uint this[string index]
		{
			get
			{
				return this[(StatTypes)Enum.Parse(typeof(StatTypes), index, true)];
			}
			set
			{
				this[(StatTypes)Enum.Parse(typeof(StatTypes), index, true)] = value;
			}
		}

		/// <summary>
		/// Get or set specified stat
		/// </summary>
		/// <param name="index">Type of stat to access</param>
		/// <returns>Value of specified stat</returns>
		public uint this[StatTypes index]
		{
			get
			{
				if (statValues.ContainsKey(index))
				{
					return statValues[index];
				}
				else
				{
					return 0;
				}
			}
			set
			{
				if (!statValues.ContainsKey(index))
				{
					statValues.Add(index, value);
				}
				else
				{
					statValues[index] = value;
				}
			}
		}

		// TODO: Get rid of all of these individual properties?
		/// <summary>
		/// Base value of strength
		/// </summary>
		public uint Strength
		{
			get { return this[StatTypes.Strength]; }
			set { this[StatTypes.Strength] = value; }
		}
		/// <summary>
		/// Base value of energy
		/// </summary>
		public uint Energy
		{
			get { return this[StatTypes.Energy]; }
			set { this[StatTypes.Energy] = value; }
		}
		/// <summary>
		/// Base value of dexterity
		/// </summary>
		public uint Dexterity
		{
			get { return this[StatTypes.Dexterity]; }
			set { this[StatTypes.Dexterity] = value; }
		}
		/// <summary>
		/// Base value of vitality
		/// </summary>
		public uint Vitality
		{
			get { return this[StatTypes.Vitality]; }
			set { this[StatTypes.Vitality] = value; }
		}
		/// <summary>
		/// Number of unallocated stat points
		/// </summary>
		public uint StatPoints
		{
			get { return this[StatTypes.StatPoints]; }
			set { this[StatTypes.StatPoints] = value; }
		}
		/// <summary>
		/// Number of unallocated skill points
		/// </summary>
		public uint SkillPoints
		{
			get { return this[StatTypes.SkillPoints]; }
			set { this[StatTypes.SkillPoints] = value; }
		}
		/// <summary>
		/// Current value of hitpoints
		/// </summary>
		/// <remarks>Current value is usually higher than base value</remarks>
		public uint Hitpoints
		{
			get { return this[StatTypes.Hitpoints]; }
			set { this[StatTypes.Hitpoints] = value; }
		}
		/// <summary>
		/// Base value of hitpoints
		/// </summary>
		public uint BaseHitpoints
		{
			get { return this[StatTypes.BaseHitpoints]; }
			set { this[StatTypes.BaseHitpoints] = value; }
		}
		/// <summary>
		/// Current value of mana
		/// </summary>
		/// <remarks>Current value is usually higher than base value</remarks>
		public uint Mana
		{
			get { return this[StatTypes.Mana]; }
			set { this[StatTypes.Mana] = value; }
		}
		/// <summary>
		/// Base value of mana
		/// </summary>
		public uint BaseMana
		{
			get { return this[StatTypes.BaseMana]; }
			set { this[StatTypes.BaseMana] = value; }
		}
		/// <summary>
		/// Current value of stamina.
		/// </summary>
		/// <remarks>Current value is usually higher than base value</remarks>
		public uint Stamina
		{
			get { return this[StatTypes.Stamina]; }
			set { this[StatTypes.Stamina] = value; }
		}
		/// <summary>
		/// Base value of stamina
		/// </summary>
		public uint BaseStamina
		{
			get { return this[StatTypes.BaseStamina]; }
			set { this[StatTypes.BaseStamina] = value; }
		}
		/// <summary>
		/// Character's level
		/// </summary>
		public uint Level
		{
			get { return this[StatTypes.Level]; }
			set { this[StatTypes.Level] = value; }
		}
		/// <summary>
		/// Number of experience points character has
		/// </summary>
		public uint Experience
		{
			get { return this[StatTypes.Experience]; }
			set { this[StatTypes.Experience] = value; }
		}
		/// <summary>
		/// Amount of gold character has in inventory
		/// </summary>
		public uint Gold
		{
			get { return this[StatTypes.Gold]; }
			set { this[StatTypes.Gold] = value; }
		}
		/// <summary>
		/// Amount of gold character has in the bank
		/// </summary>
		public uint GoldBank
		{
			get { return this[StatTypes.GoldBank]; }
			set { this[StatTypes.GoldBank] = value; }
		}
		/// <summary>
		/// Number of kills
		/// </summary>
		public uint KillCount
		{
			get { return this[StatTypes.KillCount]; }
			set { this[StatTypes.KillCount] = value; }
		}
		/// <summary>
		/// Times character has died
		/// </summary>
		public uint DeathCount
		{
			get { return this[StatTypes.DeathCount]; }
			set { this[StatTypes.DeathCount] = value; }
		}

		/// <summary>
		/// Parses raw character stat data
		/// </summary>
		/// <param name="headerBytes">Raw characte stat data found between "gf" and "if" near offset 765 in save file</param>
		/// <remarks>Bit lengths of stat types are found in the CSvBits column of ItemStatCost.txt</remarks>
		/// Source: http://phrozenkeep.hugelaser.com/forum/viewtopic.php?f=8&t=9011&start=50
		private void ReadStats()
		{
			BitReader br = new BitReader(statsBytes);

			// Skip header bytes
			br.SkipBits(8);
			br.SkipBits(8);

			while (br.Position < br.BitCount - 16)
			{
				// ID of stat (See ItemStatCost.txt)
				StatTypes statIndex = (StatTypes)br.Read(9);
				// Value contains this many bits (See CSvBits in ItemStatCost.txt)
				int statValueBits = 0;
				// Value needs to be shifted by this amount
				int valShift = 0;

				if(!statValueBitCounts.ContainsKey(statIndex))
				{
					br.Position -= 9;
					remainingBytes = new byte[(((br.BitCount-16) - br.Position) / 8)];

					for (int i = 0; i < remainingBytes.Length; i++)
					{
						remainingBytes[i] = br.ReadByte();
					}

					remainingBitsCount = (int)((br.BitCount-16) - br.Position);
					if (remainingBitsCount > 0)
					{
						remainingBits = (byte)br.Read(remainingBitsCount);
					}

					break;
				}

				statValueBits = statValueBitCounts[statIndex];
				if (statValueBits == 0)
				{
					break;
				}

				// Get the shift value if it exists (Used on health/mana/stamina stats)
				if (statValueBitShifts.ContainsKey(statIndex))
				{
					valShift = statValueBitShifts[statIndex];
				}

				uint statValue = br.Read(statValueBits);
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

			bits.Write(Utils.ReverseBits('g', 8), 0, 8);
			bits.Write(Utils.ReverseBits('f', 8), 0, 8);

			foreach (var stat in statValues)
			{
				bits.Write(Utils.ReverseBits((uint)stat.Key, 9), 0, 9);

				int valShift = 0;
				int bitCount = statValueBitCounts[stat.Key];

				if (statValueBitShifts.ContainsKey(stat.Key))
				{
					valShift = statValueBitShifts[stat.Key];
				}

				bits.Write(Utils.ReverseBits((uint)((stat.Value << valShift)), bitCount), 0, bitCount);
			}

			// These last 2 bytes seem to be some sort of terminator?
			bits.Write(Utils.ReverseByteArrayBits(remainingBytes));

			if (remainingBitsCount > 0)
			{
				bits.Write(Utils.ReverseBits(remainingBits, remainingBitsCount), 0, remainingBitsCount);
			}

			// This is actually the skill header, but it's easier to just handle the header here
			//  and keep the skill data as raw bytes containing skill levels, no additional processing needed
			bits.Write(Utils.ReverseBits('i', 8), 0, 8);
			bits.Write(Utils.ReverseBits('f', 8), 0, 8);

			return Utils.ReverseByteArrayBits(bits.ToByteArray());
		}

		#region IEnumerable<KeyValuePair<StatTypes,uint>> Members

		public IEnumerator<KeyValuePair<Stat.StatTypes, uint>> GetEnumerator()
		{
			return statValues.GetEnumerator();
		}

		#endregion

		#region IEnumerable Members

		IEnumerator IEnumerable.GetEnumerator()
		{
			return statValues.GetEnumerator();
		}

		#endregion
	}
}
