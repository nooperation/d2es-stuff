using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace esCharView
{
	class SaveReader
	{
		private Inventory inventory;
		private Character character;
		private string filePath;

		/// <summary>
		/// Character's inventory
		/// </summary>
		public Inventory Inventory
		{
			get
			{
				return inventory;
			}
		}

		public Character Character
		{
			get
			{
				return character;
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
		/// Saves player data to specified path
		/// </summary>
		/// <param name="filePath">Path to save character data as</param>
		public void Write(string filePath)
		{
			byte[] characterBytes = Character.GetCharacterBytes();
			byte[] inventoryBytes = Inventory.GetInventoryBytes(Character.HasMercenary);

			byte[] rawCharacterData = new byte[characterBytes.Length + inventoryBytes.Length];

			Array.Copy(characterBytes, rawCharacterData, characterBytes.Length);
			Array.Copy(inventoryBytes, 0, rawCharacterData, characterBytes.Length, inventoryBytes.Length);

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

			byte[] characterBytes = new byte[itemListBegin];
			byte[] inventoryBytes = new byte[rawCharacterData.Length - itemListBegin];

			Array.Copy(rawCharacterData, 0, characterBytes, 0, characterBytes.Length);
			Array.Copy(rawCharacterData, itemListBegin, inventoryBytes, 0, inventoryBytes.Length);

			inventory = new Inventory(inventoryBytes);
			character = new Character(characterBytes);
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
