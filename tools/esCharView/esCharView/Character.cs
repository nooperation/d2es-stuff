using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Collections;

namespace esCharView
{
	public class Character
	{
		private byte[] characterBytes;
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
		}

		/// <summary>
		/// Character's name
		/// </summary>
		public String Name
		{
			get
			{
				return ASCIIEncoding.ASCII.GetString(characterBytes, 0x14, 16);
			}
			set
			{
				byte[] paddedName = new byte[16];
				byte[] newName = ASCIIEncoding.ASCII.GetBytes(value);

				//15 instead of 16 to keep trailing null character
				Array.Copy(newName, paddedName, newName.Length < 15 ? newName.Length : 15);
				Array.Copy(paddedName, 0, characterBytes, 0x14, paddedName.Length);
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
			set
			{
				if (value)
				{
					characterFlags |= 0x04;
				}
				else
				{
					characterFlags &= 0xfb;
				}
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
			set
			{
				if (value)
				{
					characterFlags |= 0x08;
				}
				else
				{
					characterFlags &= 0xf7;
				}
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
			set
			{
				if (value)
				{
					characterFlags |= 0x20;
				}
				else
				{
					characterFlags &= 0xDF;
				}
			}
		}

		/// <summary>
		/// Collection of unknown flags
		/// </summary>
		public byte UnknownFlags
		{
			get
			{
				return (byte)(characterFlags & (byte)0xd3);
			}
			set
			{
				characterFlags &= 0x2C;
				characterFlags |= value;
			}
		}

		/// <summary>
		/// Character's title / progression
		/// </summary>
		public byte Progression
		{
			get { return characterBytes[37]; }
			set { characterBytes[37] = value; }
		}

		/// <summary>
		/// Character's class
		/// </summary>
		public CharacterClass Class
		{
			get { return (CharacterClass)characterBytes[40]; }
			set { characterBytes[40] = (byte)value; }
		}

		/// <summary>
		/// Level displayed on character list ?
		/// </summary>
		public byte LevelDisplay
		{
			get { return characterBytes[43]; }
			set { characterBytes[43] = value; }
		}

		/// <summary>
		/// Chracter has a mercenary
		/// </summary>
		public bool HasMercenary
		{
			get
			{
				return BitConverter.ToUInt32(characterBytes, 179) != 0;
			}
		}

		/// <summary>
		/// ID Of mercenary's name
		/// </summary>
		public ushort MercenaryNameId
		{
			get
			{
				return BitConverter.ToUInt16(characterBytes, 183);
			}
		}

		/// <summary>
		/// Mercenary type
		/// </summary>
		public ushort MercenaryType
		{
			get
			{
				return BitConverter.ToUInt16(characterBytes, 185);
			}
		}

		/// <summary>
		/// Mercenary's experience points
		/// </summary>
		public uint MercenaryExp
		{
			get
			{
				return BitConverter.ToUInt32(characterBytes, 185);
			}
		}

		/// <summary>
		/// Creates a new character reader with specified header
		/// </summary>
		/// <param name="characterBytes">Raw character bytes from save file</param>
		public Character(byte[] characterBytes)
		{
			this.characterBytes = characterBytes;
			characterFlags = characterBytes[0x24];
		}

		/// <summary>
		/// Returns modified character bytes for use in save file
		/// </summary>
		/// <returns>Raw character bytes</returns>
		public byte[] GetCharacterBytes()
		{
			characterBytes[0x24] = characterFlags;

			return characterBytes;
		}
	}
}
