using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CharacterEditor;
using System.Text.RegularExpressions;
using System.Reflection;

namespace BatchSaveTester
{
	class Program
	{
		private static StreamWriter sw; // TEMP!

		static void Main(string[] args)
		{
			bool detectDuplicateIds = false;
			bool detectFlaggedItems = false;
			bool detectFlaggedProperties = false;

			if (args.Length < 2)
			{
				PrintUsage();
				return;
			}

			string specifiedCharSaveDirectory = args[0];
			string specifiedMod = args[1].ToLower();
			List<string> modList = Resources.GetResourceSets();

			if (!Directory.Exists(specifiedCharSaveDirectory))
			{
				Console.WriteLine("Invalid charSaveDirectory specified");
				return;
			}

			if (!modList.Any(n => { return n.ToLower() == specifiedMod; }))
			{
				Console.WriteLine("Invalid mod specified");
				return;
			}

			for (int i = 0; i < args.Length; i++)
			{
				if (args[i].ToLower() == "-dupes")
				{
					detectDuplicateIds = true;
				}
				else if (args[i].ToLower() == "-items")
				{
					detectFlaggedItems = true;
				}
				else if (args[i].ToLower() == "-properties")
				{
					detectFlaggedProperties = true;
				}
			}

			//// TEMP!
			//using(sw = new StreamWriter(File.OpenWrite(@"d:\\goontopiaStats.txt")))
			//{
			//	DumpCharacterData(null);
			//	TestAllFiles(specifiedCharSaveDirectory, specifiedMod, detectDuplicateIds, detectFlaggedItems, detectFlaggedProperties);
			//}
			//TestAllFiles(@"D:\files\usr\var\charbackup");
			// 
			BatchConvertToD2s(@"D:\charsaves", "es300_R6D");
			Console.WriteLine("Complete");
		}

		private static void BatchConvertToD2s(string path, string resourceSet)
		{
			string convertedSavePath = path + "\\d2s\\";
			string[] fileNames = Directory.GetFiles(path);

			if (!Directory.Exists(convertedSavePath))
			{
				Directory.CreateDirectory(convertedSavePath);
			}
			for (int i = 0; i < fileNames.Length; i++)
			{
				SaveReader currentSave = new SaveReader(resourceSet);

				try
				{
					currentSave.Read(File.ReadAllBytes(fileNames[i]));
					if (currentSave.Character.UnknownFlags != 64)
					{
						Console.WriteLine("Flags = {0}", currentSave.Character.UnknownFlags);
					}
					currentSave.Character.UnknownFlags = 0;

					string savePath = convertedSavePath + Path.GetFileNameWithoutExtension(fileNames[i]) + ".d2s";

					using (FileStream saveStream = File.OpenWrite(savePath))
					{
						currentSave.Write(saveStream, false);
					}
					
				}
				catch (Exception ex)
				{
					Console.WriteLine("Failed to convert {0}: {1}", fileNames[i], ex.Message);
				}
			}
		}

		private static void PrintUsage()
		{
			List<string> modList = Resources.GetResourceSets();

			StringBuilder sb = new StringBuilder();
			foreach (var item in modList)
			{
				sb.AppendFormat("\"{0}\" ", item);
			}

			Console.WriteLine("BatchSaveTester \"charSavePath\" \"mod\" [-dupes] [-items] [-properties]");
			Console.WriteLine();
			Console.WriteLine("  charSavePath    Path containing save files");
			Console.WriteLine("  mod             Name of mod to use from Resources directory");
			Console.WriteLine("  -dupes          Detect duplicate item IDs");
			Console.WriteLine("  -items          Detect flagged items");
			Console.WriteLine("  -properties     Detect flagged item properties");
			Console.WriteLine();
			Console.WriteLine("Available mods: " + sb.ToString());
			Console.WriteLine();
		}

		struct ItemData
		{
			public string FileName;
			public Item Item;
		}

		private static void TestAllFiles(string path, string resourceSet, bool detectDuplicateIds, bool detectFlaggedItems, bool detectFlaggedProperties)
		{
			TestAllFiles(path, resourceSet, detectDuplicateIds, detectFlaggedItems, detectFlaggedProperties, "flaggedItems.txt", "flaggedProperties.txt");
		}

		private static void TestAllFiles(string path, string resourceSet, bool detectDuplicateIds, bool detectFlaggedItems, bool detectFlaggedProperties, string flaggedItemsPath, string flaggedPropertiesPath)
		{
			string invalidSavePath = path + "\\invalid\\";
			string invalidItemPath = path + "\\baditems\\";
			string invalidPropsPath = path + "\\badprops\\";
			string invalidDecodePath = path + "\\undecoded\\";
			string[] fileNames = Directory.GetFiles(path);

			List<string> invlaidSaves = new List<string>();
			List<string> invalidItemSaves = new List<string>();
			List<string> invalidPropsSaves = new List<string>();
			List<string> invalidDecodeSaves = new List<string>();

			Dictionary<uint, List<ItemData>> itemIds = new Dictionary<uint, List<ItemData>>();
			Dictionary<string, List<ItemData>> detectedFlaggedItems = new Dictionary<string, List<ItemData>>();
			Dictionary<string, List<ItemData>> detectedFlaggedProps = new Dictionary<string, List<ItemData>>();

			HashSet<string> flaggedItems = null;
			Dictionary<string, ItemPropData> flaggedProperties = null;

			if (detectFlaggedItems)
			{
				try
				{
					flaggedItems = ReadFlaggedItemList(flaggedItemsPath);
				}
				catch (Exception ex)
				{
					Console.WriteLine("Failed to read " + flaggedItemsPath + ": " + ex.Message);
					sw.Close();
					return;
				}
			}
			if (detectFlaggedProperties)
			{
				try
				{
					flaggedProperties = ReadFlaggedPropertyList(flaggedPropertiesPath);
				}
				catch (Exception ex)
				{
					Console.WriteLine("Failed to read " + flaggedPropertiesPath + ": " + ex.Message);
					return;
				}
			}

			for (int i = 0; i < fileNames.Length; i++)
			{
				SaveReader currentSave = new SaveReader(resourceSet);

				try
				{
					currentSave.Read(File.ReadAllBytes(fileNames[i]));

					foreach (var item in currentSave.Inventory.PlayerItems)
					{
						if (detectDuplicateIds)
						{
							if (!itemIds.ContainsKey(item.Id))
							{
								itemIds[item.Id] = new List<ItemData>();
							}

							itemIds[item.Id].Add(new ItemData() { FileName = fileNames[i], Item = item });
						}
						if (detectFlaggedItems)
						{
							if (flaggedItems.Contains(item.ItemCode))
							{
								if (!detectedFlaggedItems.ContainsKey(item.ItemCode))
								{
									detectedFlaggedItems[item.ItemCode] = new List<ItemData>();
								}

								detectedFlaggedItems[item.ItemCode].Add(new ItemData() { FileName = fileNames[i], Item = item });
							}
						}
						if (detectFlaggedProperties)
						{
							foreach (var prop in item.Properties)
							{
								if (flaggedProperties.ContainsKey(prop.PropertyName))
								{
									if (flaggedProperties[prop.PropertyName].TestValue(prop.Value))
									{
										if (!detectedFlaggedProps.ContainsKey(prop.PropertyName))
										{
											detectedFlaggedProps[prop.PropertyName] = new List<ItemData>();
										}

										detectedFlaggedProps[prop.PropertyName].Add(new ItemData() { FileName = fileNames[i], Item = item });
									}
								}
							}
						}
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
				catch (Exception)
				{
					invlaidSaves.Add(fileNames[i]);
					continue;
				}

				if (currentSave != null)
				{
					DumpCharacterData(currentSave);
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

			if (detectDuplicateIds)
			{
				Console.WriteLine();
				Console.WriteLine("Searching for duplicate ids:");
				Console.WriteLine();
				var dupes = (from n in itemIds where n.Key > 0 && n.Value.Count > 1 select n).ToList();

				foreach (var item in dupes)
				{
					Console.WriteLine("Item {0} - {1}:", item.Key, CharacterEditor.ItemDefs.GetItemDescription(item.Value[0].Item.ItemCode));
					foreach (var dupe in item.Value)
					{
						Console.WriteLine("\t{0}", dupe.FileName);
					}
					Console.WriteLine();
				}
			}

			if (detectFlaggedItems)
			{
				Console.WriteLine();
				Console.WriteLine("Flagged items detected:");
				Console.WriteLine();
				foreach (var item in detectedFlaggedItems)
				{
					Console.WriteLine("Item {0} - {1}:", item.Key, CharacterEditor.ItemDefs.GetItemDescription(item.Key));
					foreach (var dupe in item.Value)
					{
						Console.WriteLine("\t{0}", dupe.FileName);
					}
					Console.WriteLine();
				}
			}

			if (detectFlaggedProperties)
			{
				Console.WriteLine();
				Console.WriteLine("Flagged item properties detected:");
				Console.WriteLine();
				foreach (var item in detectedFlaggedProps)
				{
					Console.WriteLine("Property {0}:", flaggedProperties[item.Key]);
					foreach (var flagged in item.Value)
					{
						Console.Write("\t{0} -> ", flagged.FileName);
						foreach (var prop in flagged.Item.Properties)
						{
							if (prop.PropertyName == item.Key)
							{
								Console.WriteLine("[n = {0}] {1}", prop.Value, flagged.Item);
								break;
							}
						}
					}
					Console.WriteLine();
				}
			}
		}

		private static Dictionary<string, ItemPropData> ReadFlaggedPropertyList(string path)
		{
			Dictionary<string, ItemPropData> flaggedProperties = new Dictionary<string, ItemPropData>();

			using (StreamReader sr = new StreamReader(File.OpenRead(path)))
			{
				Regex reg = new Regex(@"(?<propName>\w+)\s+(?<comparer>\W+)\s+(?<value>[-]*\d+)");

				while (!sr.EndOfStream)
				{
					string currentLine = sr.ReadLine().Trim();

					Match match = reg.Match(currentLine);
					if (match.Success)
					{
						ItemPropData newPropData = new ItemPropData();
						newPropData.PropertyName = match.Groups["propName"].Value;
						newPropData.ComparisonString = match.Groups["comparer"].Value;
						newPropData.Value = int.Parse(match.Groups["value"].Value);
						flaggedProperties.Add(newPropData.PropertyName, newPropData);
					}
				}
			}

			return flaggedProperties;
		}

		private static HashSet<string> ReadFlaggedItemList(string path)
		{
			HashSet<string> flaggedItems = new HashSet<string>();

			using (StreamReader sr = new StreamReader(File.OpenRead(path)))
			{
				while (!sr.EndOfStream)
				{
					string currentLine = sr.ReadLine();
					if (currentLine.Length < 3)
					{
						continue;
					}

					flaggedItems.Add(currentLine.Trim().Substring(0, 3).ToLower());
				}
			}

			return flaggedItems;
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

		private static object outputLock = new object();

		private static void DumpCharacterData(SaveReader saveData)
		{
			lock (outputLock)
			{
				sw.WriteLine(GetCharacterCSVData(saveData));
			}
		}

		private static string GetCharacterCSVData(SaveReader saveData)
		{
			StringBuilder sb = new StringBuilder();

			PropertyInfo[] characterProperties = typeof(Character).GetProperties();
			PropertyInfo[] statProperties = typeof(Stat).GetProperties();

			foreach (var item in characterProperties)
			{
				if (item.PropertyType.IsPrimitive || item.PropertyType.IsEnum || item.PropertyType == typeof(string))
				{
					if (saveData != null)
					{
						sb.Append(item.GetValue(saveData.Character, null) + ",");
					}
					else
					{
						sb.Append(item.Name + ",");
					}
				}
			}
			foreach (var item in statProperties)
			{
				if (item.PropertyType.IsPrimitive || item.PropertyType.IsEnum || item.PropertyType == typeof(string))
				{
					if (saveData != null)
					{
						sb.Append(item.GetValue(saveData.Stat, null) + ",");
					}
					else
					{
						sb.Append(item.Name + ",");
					}
				}
			}

			sb.Remove(sb.Length - 1, 1);
			return sb.ToString();
		}
	}
}
