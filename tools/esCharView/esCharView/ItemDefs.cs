using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace esCharView
{
	class ItemDefs
	{
		private static Dictionary<string, string> ItemDescriptions = new Dictionary<string, string>();
		private static Dictionary<string, string> SetDescriptions = new Dictionary<string, string>();
		private static HashSet<string> ArmorCodes = new HashSet<string>();
		private static HashSet<string> WeaponCodes = new HashSet<string>();
		private static HashSet<string> StackableCodes = new HashSet<string>();

		public static string GetItemDescription(string itemCode)
		{
			if (ItemDescriptions.ContainsKey(itemCode))
			{
				return ItemDescriptions[itemCode];
			}

			return "UNKNOWN";
		}

		public static string GetUniqueSetName(string itemCode)
		{
			if (!SetDescriptions.ContainsKey(itemCode))
			{
				return "UNKNOWN SET";
			}

			return SetDescriptions[itemCode];
		}

		public static bool IsArmor(string itemCode)
		{
			return ArmorCodes.Contains(itemCode);
		}

		public static bool IsWeapon(string itemCode)
		{
			return WeaponCodes.Contains(itemCode);
		}

		public static bool IsStackable(string itemCode)
		{
			return StackableCodes.Contains(itemCode);
		}

		static ItemDefs()
		{
			if (!File.Exists("AllItems.txt"))
			{
				MessageBox.Show("Failed to locate AllItems.txt, unable to get item descriptions", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}

			if (!File.Exists("Armor.txt"))
			{
				MessageBox.Show("Failed to locate Armor.txt", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}

			if (!File.Exists("Weapons.txt"))
			{
				MessageBox.Show("Failed to locate Weapons.txt", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}

			if (!File.Exists("Stackable.txt"))
			{
				MessageBox.Show("Failed to locate Stackable.txt", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}
			if (!File.Exists("Sets.txt"))
			{
				MessageBox.Show("Failed to locate Sets.txt", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
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

				ItemDescriptions.Add(cleanStr.Substring(0, 3), cleanStr.Substring(4));
			}

			foreach (string str in File.ReadAllLines("Sets.txt"))
			{
				string itemCode = str.Substring(0, 3);

				if(!SetDescriptions.ContainsKey(itemCode))
					SetDescriptions.Add(itemCode, str.Substring(4));
			}
			foreach (string str in File.ReadAllLines("Armor.txt"))
			{
				ArmorCodes.Add(str.Substring(0, 3));
			}
			foreach (string str in File.ReadAllLines("Weapons.txt"))
			{
				WeaponCodes.Add(str.Substring(0, 3));
			}
			foreach (string str in File.ReadAllLines("Stackable.txt"))
			{
				StackableCodes.Add(str.Substring(0, 3));
			}
		}


	}
}
