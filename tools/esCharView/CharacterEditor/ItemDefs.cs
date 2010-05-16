using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace CharacterEditor
{
	//TODO: Move all specific groups of items into a single file like Autostocker.ini has
	class ItemDefs
	{
		private static Dictionary<string, string> itemDescriptions = new Dictionary<string, string>();
		private static Dictionary<string, string> setDescriptions = new Dictionary<string, string>();
		private static Dictionary<string, HashSet<string>> itemCodeSets = new Dictionary<string, HashSet<string>>();

		public static string GetItemDescription(string itemCode)
		{
			if (itemDescriptions.ContainsKey(itemCode))
			{
				return itemDescriptions[itemCode];
			}

			return "UNKNOWN";
		}

		public static string GetUniqueSetName(string itemCode)
		{
			if (!setDescriptions.ContainsKey(itemCode))
			{
				return "UNKNOWN SET";
			}

			return setDescriptions[itemCode];
		}

		public static bool IsArmor(string itemCode)
		{
			return itemCodeSets["armor"].Contains(itemCode);
		}

		public static bool IsWeapon(string itemCode)
		{
			return itemCodeSets["weapons"].Contains(itemCode);
		}

		public static bool IsStackable(string itemCode)
		{
			return itemCodeSets["stackable"].Contains(itemCode);
		}

		public static bool IsMonsterPart(string itemCode)
		{
			return itemCodeSets["monsterparts"].Contains(itemCode);
		}

		public static bool IsScrollOrTome(string itemCode)
		{
			return itemCodeSets["scrolltome"].Contains(itemCode);
		}

		public static bool IsCharm(string itemCode)
		{
			return itemCodeSets["charms"].Contains(itemCode);
		}

		static ItemDefs()
		{
			if (!File.Exists("AllItems.txt"))
			{
				throw new ApplicationException("Failed to locate AllItems.txt, unable to get item descriptions");
			}

			if (!File.Exists("ItemGroups.txt"))
			{
				throw new ApplicationException("Failed to locate ItemGroups.txt");
			}

			if (!File.Exists("Sets.txt"))
			{
				throw new ApplicationException("Failed to locate Sets.txt");
			}

			String[] fileContents = File.ReadAllLines("AllItems.txt");

			foreach (String str in fileContents)
			{
				string cleanStr = str;

				while (true)
				{
					// SpecialChar is the color character (followed by 2 other characters), this
					//  is just stripping it since it looks bad
					int specialCharLocation = cleanStr.IndexOf('\xfffd');

					if (specialCharLocation == -1)
						break;

					cleanStr = cleanStr.Remove(specialCharLocation, 3);
				}

				itemDescriptions.Add(cleanStr.Substring(0, 3), cleanStr.Substring(4));
			}

			// Just so it's guaranteed that these sections exist
			itemCodeSets.Add("weapons", new HashSet<string>());
			itemCodeSets.Add("armor", new HashSet<string>());
			itemCodeSets.Add("stackable", new HashSet<string>());
			itemCodeSets.Add("monsterparts", new HashSet<string>());
			itemCodeSets.Add("scrolltome", new HashSet<string>());
			itemCodeSets.Add("charms", new HashSet<string>());

			ReadItemCodeSet(itemCodeSets, "ItemGroups.txt");

			foreach (string str in File.ReadAllLines("Sets.txt"))
			{
				string itemCode = str.Substring(0, 3);

				if(!setDescriptions.ContainsKey(itemCode))
					setDescriptions.Add(itemCode, str.Substring(4));
			}
		}

		/// <summary>
		/// Reads multiple sets of item codes from disk. Each set has a unique section name defined in brackets (eg. [sectionName]).
		/// If multiple identical section names are exist, then the data for these sections are combined into the same set.
		/// </summary>
		/// <param name="itemCodeSets">Table of sets to store data</param>
		/// <param name="filePath">Path containing sets of item codes</param>
		/// <example>
		/// 
		/// [armor]
		/// cap Cap/hat
		/// skp Skull Cap
		/// 
		/// [weapons]
		/// hax Hand Axe 
		/// axe Axe 
		/// 
		/// </example>
		public static void ReadItemCodeSet(Dictionary<string, HashSet<string>> itemCodeSets, string filePath)
		{
			string[] lines = File.ReadAllLines(filePath);
			HashSet<string> currentSection = null;

			foreach (var line in lines)
			{
				if (line.Length <= 2)
				{
					continue;
				}

				if (line[0] == '[' && line[line.Length - 1] == ']')
				{
					string sectionName = line.Substring(1, line.Length - 2).ToLower();

					if (!itemCodeSets.ContainsKey(sectionName))
					{
						itemCodeSets.Add(sectionName, new HashSet<string>());
					}

					currentSection = itemCodeSets[sectionName];
				}
				else if (currentSection != null)
				{
					string itemCode = line.Substring(0, 3).ToLower();

					currentSection.Add(itemCode);
				}
			}
		}
	}
}
