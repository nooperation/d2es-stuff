using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace esCharView
{
	class ItemDefs
	{
		public static Dictionary<string, string> ItemDescriptions = new Dictionary<string, string>();

		public static string GetItemDescription(string itemCode)
		{
			if (ItemDescriptions.ContainsKey(itemCode))
			{
				return ItemDescriptions[itemCode];
			}

			return "UNKNOWN";
		}

		static ItemDefs()
		{
			if (!File.Exists("AllItems.txt"))
			{
				MessageBox.Show("Failed to find AllItems.txt, unable to get item descriptions", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
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
		}
	}
}
