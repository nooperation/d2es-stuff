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

			string[] fileNames = Directory.GetFiles(path);

			for (int i = 0; i < fileNames.Length; i++)
			{
				SaveReader currentSave = null;

				try
				{
					currentSave = new SaveReader(fileNames[i]);
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
