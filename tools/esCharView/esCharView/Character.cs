using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Collections;

namespace esCharView
{
	public class Character
	{
		private byte[] headerBytes;
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

		public byte[] GetCharacterBytes()
		{
			return headerBytes;
		}
	}
}
