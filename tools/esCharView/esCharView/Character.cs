using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace esCharView
{
	class Character
	{
		private byte[] headerBytes;
		private byte[] inventoryBytes;
		private byte characterFlags;
		private string filePath;
		private Inventory inventory;

		public Character()
		{

		}

		public int HeaderSize { get { return headerBytes.Length; } }
		public int InventorySize { get { return inventoryBytes.Length; } }

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
		/// Updates inventory data (mainly after user deletes an item)
		/// </summary>
		public void UpdateInventoryHeaders()
		{
			inventoryBytes = Inventory.GetInventoryBytes(HasMercenary);
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
			UpdateInventoryHeaders();

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
			inventoryBytes = new byte[rawCharacterData.Length - itemListBegin];

			Array.Copy(rawCharacterData, 0, headerBytes, 0, headerBytes.Length);
			Array.Copy(rawCharacterData, itemListBegin, inventoryBytes, 0, inventoryBytes.Length);

			inventory = new Inventory(inventoryBytes);
			characterFlags = headerBytes[0x24];
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
