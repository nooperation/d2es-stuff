using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

//TODO: Read information directly from ItemStatCost.txt so it's more compatible
namespace esCharView
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

		public Stat(byte[] statsBytes)
		{
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
			protected set { this[StatTypes.Strength] = value; }
		}
		/// <summary>
		/// Base value of energy
		/// </summary>
		public uint Energy
		{
			get { return this[StatTypes.Energy]; }
			protected set { this[StatTypes.Energy] = value; }
		}
		/// <summary>
		/// Base value of dexterity
		/// </summary>
		public uint Dexterity
		{
			get { return this[StatTypes.Dexterity]; }
			protected set { this[StatTypes.Dexterity] = value; }
		}
		/// <summary>
		/// Base value of vitality
		/// </summary>
		public uint Vitality
		{
			get { return this[StatTypes.Vitality]; }
			protected set { this[StatTypes.Vitality] = value; }
		}
		/// <summary>
		/// Number of unallocated stat points
		/// </summary>
		public uint StatPoints
		{
			get { return this[StatTypes.StatPoints]; }
			protected set { this[StatTypes.StatPoints] = value; }
		}
		/// <summary>
		/// Number of unallocated skill points
		/// </summary>
		public uint SkillPoints
		{
			get { return this[StatTypes.SkillPoints]; }
			protected set { this[StatTypes.SkillPoints] = value; }
		}
		/// <summary>
		/// Current value of hitpoints
		/// </summary>
		/// <remarks>Current value is usually higher than base value</remarks>
		public uint Hitpoints
		{
			get { return this[StatTypes.Hitpoints]; }
			protected set { this[StatTypes.Hitpoints] = value; }
		}
		/// <summary>
		/// Base value of hitpoints
		/// </summary>
		public uint BaseHitpoints
		{
			get { return this[StatTypes.BaseHitpoints]; }
			protected set { this[StatTypes.BaseHitpoints] = value; }
		}
		/// <summary>
		/// Current value of mana
		/// </summary>
		/// <remarks>Current value is usually higher than base value</remarks>
		public uint Mana
		{
			get { return this[StatTypes.Mana]; }
			protected set { this[StatTypes.Mana] = value; }
		}
		/// <summary>
		/// Base value of mana
		/// </summary>
		public uint BaseMana
		{
			get { return this[StatTypes.BaseMana]; }
			protected set { this[StatTypes.BaseMana] = value; }
		}
		/// <summary>
		/// Current value of stamina.
		/// </summary>
		/// <remarks>Current value is usually higher than base value</remarks>
		public uint Stamina
		{
			get { return this[StatTypes.Stamina]; }
			protected set { this[StatTypes.Stamina] = value; }
		}
		/// <summary>
		/// Base value of stamina
		/// </summary>
		public uint BaseStamina
		{
			get { return this[StatTypes.BaseStamina]; }
			protected set { this[StatTypes.BaseStamina] = value; }
		}
		/// <summary>
		/// Character's level
		/// </summary>
		public uint Level
		{
			get { return this[StatTypes.Level]; }
			protected set { this[StatTypes.Level] = value; }
		}
		/// <summary>
		/// Number of experience points character has
		/// </summary>
		public uint Experience
		{
			get { return this[StatTypes.Experience]; }
			protected set { this[StatTypes.Experience] = value; }
		}
		/// <summary>
		/// Amount of gold character has in inventory
		/// </summary>
		public uint Gold
		{
			get { return this[StatTypes.Gold]; }
			protected set { this[StatTypes.Gold] = value; }
		}
		/// <summary>
		/// Amount of gold character has in the bank
		/// </summary>
		public uint GoldBank
		{
			get { return this[StatTypes.GoldBank]; }
			protected set { this[StatTypes.GoldBank] = value; }
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
					case StatTypes.KillCount:
						statValueBits = 32;
						break;
					case StatTypes.DeathCount:
						statValueBits = 32;
						break;
					default:
						// There are still atleast 2 more unknowns, including 511
						return;
				}

				if (statValueBits == 0)
				{
					break;
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
			return statsBytes;
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
