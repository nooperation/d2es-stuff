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
		private Stat stat;
		private Skill skill;
		private string filePath;

		/// <summary>
		/// Character's inventory
		/// </summary>
		public Inventory Inventory
		{
			get { return inventory; }
		}

		/// <summary>
		/// General character information
		/// </summary>
		public Character Character
		{
			get { return character; }
		}

		/// <summary>
		/// Character's stats (Str, dex, etc)
		/// </summary>
		public Stat Stat
		{
			get { return stat; }
		}

		/// <summary>
		/// Character's skills
		/// </summary>
		public Skill Skill
		{
			get { return skill; }
		}

		/// <summary>
		/// Position of the start of stat information (This should be hardcoded in all save files)
		/// </summary>
		private static int StatListBegin
		{
			get { return 765; }
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
			byte[] statsBytes = Stat.GetStatBytes();
			byte[] skillBytes = Skill.GetSkillBytes();
			byte[] inventoryBytes = Inventory.GetInventoryBytes(Character.HasMercenary);
			byte[] rawCharacterData = new byte[characterBytes.Length + statsBytes.Length + skillBytes.Length + inventoryBytes.Length];

			Array.Copy(characterBytes, rawCharacterData, characterBytes.Length);
			Array.Copy(statsBytes, 0, rawCharacterData, characterBytes.Length, statsBytes.Length);
			Array.Copy(skillBytes, 0, rawCharacterData, characterBytes.Length + statsBytes.Length, skillBytes.Length);
			Array.Copy(inventoryBytes, 0, rawCharacterData, characterBytes.Length + statsBytes.Length + skillBytes.Length, inventoryBytes.Length);

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

			byte[] statBytes = GetStatBytes(rawCharacterData);
			byte[] characterBytes = GetCharacterBytes(rawCharacterData);
			byte[] skillBytes = GetSkillBytes(rawCharacterData);
			byte[] inventoryBytes = GetInventoryBytes(rawCharacterData);

			inventory = new Inventory(inventoryBytes);
			character = new Character(characterBytes);
			stat = new Stat(statBytes);
			skill = new Skill(skillBytes);
		}

		/// <summary>
		/// Obtains general character information from raw save file data
		/// </summary>
		/// <param name="rawCharacterData"></param>
		/// <returns></returns>
		private static byte[] GetCharacterBytes(byte[] rawCharacterData)
		{
			byte[] characterBytes = new byte[StatListBegin];
			Array.Copy(rawCharacterData, characterBytes, characterBytes.Length);

			return characterBytes;
		}

		/// <summary>
		/// Copies character's raw stat data from raw header bytes
		/// </summary>
		/// <param name="rawCharacterBytes">Raw header data from save file</param>
		/// <returns>Raw stat data</returns>
		private static byte[] GetStatBytes(byte[] rawCharacterBytes)
		{
			byte[] statsSection;
			int statsSectionLength = FindStatListEnd(rawCharacterBytes) - StatListBegin;

			statsSection = new byte[statsSectionLength];
			Array.Copy(rawCharacterBytes, StatListBegin, statsSection, 0, statsSection.Length);

			return statsSection;
		}

		/// <summary>
		/// Obtains skill information from raw save file data
		/// </summary>
		/// <param name="rawCharacterData"></param>
		/// <returns></returns>
		private static byte[] GetSkillBytes(byte[] rawCharacterData)
		{
			int itemListBegin = FindItemListBegin(rawCharacterData);
			int statListEnd = FindStatListEnd(rawCharacterData);

			byte[] skillBytes = new byte[itemListBegin - statListEnd];
			Array.Copy(rawCharacterData, statListEnd, skillBytes, 0, skillBytes.Length);

			return skillBytes;
		}

		/// <summary>
		/// Obtains inventory information from raw save file data
		/// </summary>
		/// <param name="rawCharacterData"></param>
		/// <returns></returns>
		private static byte[] GetInventoryBytes(byte[] rawCharacterData)
		{
			int itemListBegin = FindItemListBegin(rawCharacterData);

			byte[] inventoryBytes = new byte[rawCharacterData.Length - itemListBegin];
			Array.Copy(rawCharacterData, itemListBegin, inventoryBytes, 0, inventoryBytes.Length);

			return inventoryBytes;
		}

		/// <summary>
		/// Corrects checksum of new player data
		/// </summary>
		/// <param name="rawCharacterData">Raw player save data</param>
		private void FixHeaders(ref byte[] rawCharacterData)
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
			for (int i = StatListBegin; i < rawCharacterData.Length - 1; i++)
			{
				if (rawCharacterData[i] == 'J' && rawCharacterData[i + 1] == 'M')
				{
					return i;
				}
			}

			return 0;
		}

		/// <summary>
		/// Returns location right after the end of the character's stat list
		/// </summary>
		/// <param name="rawCharacterBytes">Raw data from save file</param>
		/// <returns>Location of the end of character's stat list</returns>
		private static int FindStatListEnd(byte[] rawCharacterBytes)
		{
			for (int i = StatListBegin; i < rawCharacterBytes.Length; i++)
			{
				if (rawCharacterBytes[i] == 0x69 && rawCharacterBytes[i + 1] == 0x66)
				{
					return i + 2;
				}
			}

			throw new ApplicationException("End of stat list not found!");
		}
	}
}
