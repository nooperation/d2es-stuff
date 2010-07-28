using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CharacterEditor;

namespace BatchSaveTester
{
	class Program
	{
		static void Main(string[] args)
		{
			TestAllFiles(@"D:\files\usr\var\charsave");
			//TestAllFiles(@"D:\files\usr\var\charbackup");
		}

		struct ItemData
		{
			public string FileName;
			public Item Item;
		}

		private static void TestAllFiles(string path)
		{
			string invalidSavePath = path + "\\invalid\\";
			string invalidItemPath = path + "\\baditems\\";
			string invalidPropsPath = path + "\\badprops\\";
			string invalidDecodePath = path + "\\undecoded\\";

			List<string> invlaidSaves = new List<string>();
			List<string> invalidItemSaves = new List<string>();
			List<string> invalidPropsSaves = new List<string>();
			List<string> invalidDecodeSaves = new List<string>();

			Dictionary<uint, List<ItemData>> itemIds = new Dictionary<uint, List<ItemData>>();

			string[] fileNames = Directory.GetFiles(path);

			for (int i = 0; i < fileNames.Length; i++)
			{
				SaveReader currentSave = new SaveReader("es300_r6d");

				try
				{
					currentSave.Read(File.ReadAllBytes(fileNames[i]));

					foreach (var item in currentSave.Inventory.PlayerItems)
					{
						if (!itemIds.ContainsKey(item.Id))
						{
							itemIds[item.Id] = new List<ItemData>();
						}

						itemIds[item.Id].Add(new ItemData() { FileName = fileNames[i], Item = item });
					}
				}
				catch (IndexOutOfRangeException)
				{
					invlaidSaves.Add(fileNames[i]);
					continue;
				}
				catch (KeyNotFoundException)
				{
					invalidPropsSaves.Add(fileNames[i]);
					continue;
				}

				if (currentSave != null)
				{
					if (currentSave.Inventory.FailedItemCount > 0)
					{
						invalidItemSaves.Add(fileNames[i]);
					}
					else if (currentSave.FailedCharacterDecoding || currentSave.FailedInventoryDecoding || currentSave.FailedSkillDecoding)
					{
						invalidDecodeSaves.Add(fileNames[i]);
					}
				}

				if (i % 100 == 0)
				{
					Console.Write("\r");
					Console.Write(string.Format("Progress: {0,7:0.00%}", i / (double)fileNames.Length));
				}
			}
			Console.Write("\r");
			Console.Write(string.Format("Progress: {0,7:0.00%}", 1.0));
			Console.WriteLine();

			MoveInvalidSaves(invlaidSaves, invalidSavePath);
			MoveInvalidSaves(invalidItemSaves, invalidItemPath);
			MoveInvalidSaves(invalidPropsSaves, invalidPropsPath);
			MoveInvalidSaves(invalidDecodeSaves, invalidDecodePath);

			Console.WriteLine("Searching for duplicate ids:");
			var dupes = (from n in itemIds where n.Key > 0 && n.Value.Count > 1 select n).ToList();

			foreach (var item in dupes)
			{
				Console.WriteLine("Item {0}:", item.Key);
				foreach (var dupe in item.Value)
				{
					Console.WriteLine("\t{0} -> {1}", dupe.FileName, dupe.Item.ToString());
				}
				Console.WriteLine();
			}
		}

		private static void MoveInvalidSaves(List<string> invalidSaves, string path)
		{
			if (invalidSaves.Count == 0)
			{
				return;
			}

			if (!Directory.Exists(path))
			{
				Directory.CreateDirectory(path);
			}

			foreach (var item in invalidSaves)
			{
				Console.WriteLine(item);
				//	File.Move(item, path + Path.GetFileName(item));
			}
		}
	}
}
