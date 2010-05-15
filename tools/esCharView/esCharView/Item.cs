using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Linq;
using BKSystem.IO;

namespace esCharView
{
	public class Item
	{
		public enum ItemQuality
		{
			Unknown, // Shouldn't happen
			Inferior,
			Normal,
			Superior,
			Magic,
			Set,
			Rare,
			Unique,
			Craft
		}

		public enum EquipmentLocation
		{
			None, // Not equipped
			Head,
			Amulet,
			Body,
			RightPrimary,
			LeftPrimary,
			RightRing,
			LeftRing,
			Belt,
			Feet,
			Gloves,
			RightSecondary,
			LeftSecondary
		}

		public enum ItemLocation
		{
			Stored,
			Equipped,
			Belt,
			Ground,
			Tohand,
			Dropping,
			Socketed
		}

		public enum StorageType
		{
			Unknown,
			Inventory,
			UnknownB,
			UnknownC,
			Cube,
			Stash,
		}

		private class ItemData
		{
			public ItemData()
			{

			}

			public ItemData(string name, uint value, int bitCount, int index)
			{
				this.Index = index;
				this.Name = name;
				this.Value = value;
				this.BitCount = bitCount;
			}

			public int Index;
			public string Name;
			public object Value;
			public int BitCount;
		}

		private Dictionary<string, ItemData> dataEntries = new Dictionary<string, ItemData>();
		private int dataEntryIndex = 0;
		private BitReader br;
		private byte[] remainingBytes;

		private List<Item> sockets = new List<Item>();
		public List<Item> Sockets
		{
			get { return sockets; }
		}

		#region SimpleProperties

		// All items must contain these properties, if they don't it's an invalid item and should be caught
		public bool IsEquipped
		{
			get { return GetDataBoolean("IsEquipped"); }
			set { SetData("IsEquipped", value); }
		}
		public bool IsInSocket
		{
			get { return GetDataBoolean("IsInSocket"); }
			set { SetData("IsInSocket", value); }
		}
		public bool IsIdentified
		{
			get { return GetDataBoolean("IsIdentified"); }
			set { SetData("IsIdentified", value); }
		}
		public bool IsSwitchIn
		{
			get { return GetDataBoolean("IsSwitchIn"); }
			set { SetData("IsSwitchIn", value); }
		}
		public bool IsSwitchOut
		{
			get { return GetDataBoolean("IsSwitchOut"); }
			set { SetData("IsSwitchOut", value); }
		}
		public bool IsBroken
		{
			get { return GetDataBoolean("IsBroken"); }
			set { SetData("IsBroken", value); }
		}
		public bool IsSocketed
		{
			get { return GetDataBoolean("IsSocketed"); }
			set { SetData("IsSocketed", value); }
		}
		public bool IsStoreItem
		{
			get { return GetDataBoolean("IsStoreItem"); }
			set { SetData("IsStoreItem", value); }
		}
		public bool IsEar
		{
			get { return GetDataBoolean("IsEar"); }
			set { SetData("IsEar", value); }
		}
		public bool IsStarterItem
		{
			get { return GetDataBoolean("IsStarterItem"); }
			set { SetData("IsStarterItem", value); }
		}
		public bool IsSimpleItem
		{
			get { return GetDataBoolean("IsSimpleItem"); }
			set { SetData("IsSimpleItem", value); }
		}
		public bool IsEthereal
		{
			get { return GetDataBoolean("IsEthereal"); }
			set { SetData("IsEthereal", value); }
		}
		public bool IsPersonalized
		{
			get { return GetDataBoolean("IsPersonalized"); }
			set { SetData("IsPersonalized", value); }
		}
		public bool IsGamble
		{
			get { return GetDataBoolean("IsGamble"); }
			set { SetData("IsGamble", value); }
		}
		public bool IsRuneword
		{
			get { return GetDataBoolean("IsRuneword"); }
			set { SetData("IsRuneword", value); }
		}
		public ItemLocation Location
		{
			get { return (ItemLocation)GetDataValue("Location"); }
			set { SetData("Location", value); }
		}
		public EquipmentLocation PositionOnBody
		{
			get { return (EquipmentLocation)GetDataValue("PositionOnBody"); }
			set { SetData("PositionOnBody", value); }
		}
		public uint PositionX
		{
			get { return GetDataValue("PositionX"); }
			set { SetData("PositionOnBody", value); }
		}
		public uint PositionY
		{
			get { return GetDataValue("PositionY"); }
			set { SetData("PositionOnBody", value); }
		}
		public StorageType StorageId
		{
			get { return (StorageType)GetDataValue("StorageId"); }
			set { SetData("StorageId", value); }
		}
		#endregion

		#region ExtendedProperties

		public string ItemCode
		{
			get
			{
				if (IsEar)
				{
					return "ear";
				}

				return dataEntries["ItemCode"].Value as string;
			}
			set
			{
				dataEntries["ItemCode"].Value = value;
			}
		}

		public uint GoldAmount
		{
			get { return GetDataValue("GoldAmount"); }
			set { SetData("GoldAmount", value); }
		}
		public uint SocketsFilled
		{
			get { return GetDataValue("SocketsFilled"); }
			set { SetData("SocketsFilled", value); }
		}
		public uint Id
		{
			get { return GetDataValue("Id"); }
			set { SetData("Id", value); }
		}
		public uint Level
		{
			get
			{
				if (IsEar)
				{
					return EarLevel;
				}

				return GetDataValue("Level");
			}
			set
			{
				SetData("Level", value);
			}
		}
		public ItemQuality Quality
		{
			get { return (ItemQuality)GetDataValue("Quality"); }
			set { SetData("Quality", value); }
		}
		public bool HasGraphic
		{
			get { return GetDataBoolean("HasGraphic"); }
			set { SetData("HasGraphic", value); }
		}
		public uint Graphic
		{
			get { return GetDataValue("Graphic"); }
			set { SetData("Graphic", value); }
		}
		public uint UniqueSetId
		{
			get { return GetDataValue("UniqueSetId"); }
			set { SetData("UniqueSetId", value); }
		}
		public uint Defense
		{
			get { return GetDataValue("Defense"); }
			set { SetData("Defense", value); }
		}
		public uint MaxDurability
		{
			get { return GetDataValue("MaxDurability"); }
			set { SetData("MaxDurability", value); }
		}
		public uint Durability
		{
			get { return GetDataValue("Durability"); }
			set { SetData("Durability", value); }
		}
		public bool IsIndestructable
		{
			get { return GetDataBoolean("IsIndestructable"); }
			set { SetData("IsIndestructable", value); }
		}
		public uint SocketCount
		{
			get { return GetDataValue("SocketCount"); }
			set { SetData("SocketCount", value); }
		}
		public uint Quantity
		{
			get { return GetDataValue("Quantity"); }
			set { SetData("Quantity", value); }
		}
		public bool RandomFlag
		{
			get { return GetDataBoolean("RandomFlag"); }
			set { SetData("RandomFlag", value); }
		}
		public bool UnknownGoldFlag
		{
			get { return GetDataBoolean("UnknownGoldFlag"); }
			set { SetData("UnknownGoldFlag", value); }
		}
		public bool ClassFlag
		{
			get { return GetDataBoolean("ClassFlag"); }
			set { SetData("ClassFlag", value); }
		}
		public uint EarClass
		{
			get { return GetDataValue("EarClass"); }
			set { SetData("EarClass", value); }
		}
		public uint EarLevel
		{
			get { return GetDataValue("EarLevel"); }
			set { SetData("EarLevel", value); }
		}
		#endregion

		#region Affix

		public bool PrefixFlag0
		{
			get { return GetDataBoolean("PrefixFlag0"); }
			set { SetData("PrefixFlag0", value); }
		}
		public bool PrefixFlag1
		{
			get { return GetDataBoolean("PrefixFlag1"); }
			set { SetData("PrefixFlag1", value); }
		}
		public bool PrefixFlag2
		{
			get { return GetDataBoolean("PrefixFlag2"); }
			set { SetData("PrefixFlag2", value); }
		}
		public bool SuffixFlag0
		{
			get { return GetDataBoolean("SuffixFlag0"); }
			set { SetData("SuffixFlag0", value); }
		}
		public bool SuffixFlag1
		{
			get { return GetDataBoolean("SuffixFlag1"); }
			set { SetData("SuffixFlag1", value); }
		}
		public bool SuffixFlag2
		{
			get { return GetDataBoolean("SuffixFlag2"); }
			set { SetData("SuffixFlag2", value); }
		}

		public uint Prefix0
		{
			get { return GetDataValue("Prefix0"); }
			set { SetData("Prefix0", value); }
		}
		public uint Prefix1
		{
			get { return GetDataValue("Prefix1"); }
			set { SetData("Prefix1", value); }
		}
		public uint Prefix2
		{
			get { return GetDataValue("Prefix2"); }
			set { SetData("Prefix2", value); }
		}
		public uint Suffix0
		{
			get { return GetDataValue("Suffix0"); }
			set { SetData("Suffix0", value); }
		}
		public uint Suffix1
		{
			get { return GetDataValue("Suffix1"); }
			set { SetData("Suffix1", value); }
		}
		public uint Suffix2
		{
			get { return GetDataValue("Suffix2"); }
			set { SetData("Suffix2", value); }
		}
		#endregion

		public Item(byte[] itemData)
		{
			br = new BitReader(itemData);

			ReadItemData();
			ReadRemainingBits();
		}

		/// <summary>
		/// Decodes raw item data
		/// </summary>
		private void ReadItemData()
		{
			ReadItemDataSimple();

			if (IsEar)
			{
				ReadItemDataEar();
				return;
			}

			ReadString("ItemCode", 3, true);

			// Read gold data if it's gold
			// TODO: Should this come later on? Not sure how to test this...
			if (ItemCode.ToLower() == "gld")
			{
				ReadItemDataGold();
				return;
			}

			// Simple items don't have extended data
			if (!IsSimpleItem)
			{
				ReadItemDataExtended();
			}

			// TODO: Not sure what this bit is, I can't find any items with it set
			// Extend.txt says it's some sort of random flag followed by 40 bits

			ReadData("RandomFlag", 1);
			if (RandomFlag)
			{
				ReadData(8);
				ReadData(32);
			}

			// Type specific extended data
			if (!IsSimpleItem)
			{
				ReadItemDataExtendedSpecific();
			}
		}

		/// <summary>
		/// Decodes basic item data. All items have basic data
		/// </summary>
		private void ReadItemDataSimple()
		{
			br.SkipBits(16); // "JM" header

			ReadData("IsEquipped", 1);
			ReadData(2);
			ReadData("IsInSocket", 1);
			ReadData("IsIdentified", 1);
			ReadData(1); // Unknown 2
			ReadData("IsSwitchIn", 1);
			ReadData("IsSwitchOut", 1);
			ReadData("IsBroken", 1);
			ReadData(1); // Unknown 3 +
			ReadData(1); // Potion (?)
			ReadData("IsSocketed", 1);
			ReadData(1); // Unknown 4
			ReadData("IsStoreItem", 1);
			ReadData(1); // NotInSocket (?)
			ReadData(1); // Unknown 5
			ReadData("IsEar", 1);
			ReadData("IsStarterItem", 1);
			ReadData(3); // Unknown 6
			ReadData("IsSimpleItem", 1);
			ReadData("IsEthereal", 1);
			ReadData(1); // Any (?)
			ReadData("IsPersonalized", 1);
			ReadData("IsGamble", 1);
			ReadData("IsRuneword", 1);
			ReadData(15);
			ReadData("Location", 3);
			ReadData("PositionOnBody", 4);
			ReadData("PositionX", 4);
			ReadData("PositionY", 4);
			ReadData("StorageId", 3);
		}

		/// <summary>
		/// Decodes ear data
		/// </summary>
		private void ReadItemDataEar()
		{
			ReadData("EarClass", 3);
			ReadData("EarLevel", 7);
			// earName is the rest of the packet

			//ItemCode = "ear";

			return;
		}

		/// <summary>
		/// Decodes gold data
		/// </summary>
		private void ReadItemDataGold()
		{
			ReadData("UnknownGoldFlag", 1);

			// Not sure if this is correct
			if (UnknownGoldFlag)
				ReadData("GoldAmount", 32);
			else
				ReadData("GoldAmount", 12);

			return;
		}

		/// <summary>
		/// Decodes extended item data
		/// </summary>
		private void ReadItemDataExtended()
		{
			ReadData("SocketsFilled", 3);
			ReadData("Id", 32);
			ReadData("Level", 7);
			ReadData("Quality", 4);
			ReadData("HasGraphic", 1);
			if (HasGraphic)
			{
				ReadData("Graphic", 3);
			}

			// No idea what this flag is. Some say Class data others say Color data
			ReadData("ClassFlag", 1);
			if (ClassFlag)
			{
				ReadData(11);
			}

			switch (Quality)
			{
				case ItemQuality.Inferior:
				case ItemQuality.Superior:
					ReadData(3);
					break;

				case ItemQuality.Normal:
					if (ItemDefs.IsCharm(ItemCode))
					{
						ReadData(12); // Not sure
					}
					else if (ItemDefs.IsScrollOrTome(ItemCode))
					{
						ReadData(5); // Spell Id?
					}
					else if (ItemDefs.IsMonsterPart(ItemCode))
					{
						ReadData(10); // Monster id?
					}
					break;

				case ItemQuality.Magic:
					ReadData("Prefix0", 11);
					ReadData("Suffix0", 11);
					break;

				case ItemQuality.Rare:
				case ItemQuality.Craft:
					ReadData(8); // Prefix name id?
					ReadData(8); // Suffix name id?

					for (int i = 0; i < 3; i++)
					{
						string prefixIndex = string.Format("PrefixFlag{0}", i);

						ReadData(prefixIndex, 1);
						if (dataEntries.ContainsKey(prefixIndex))
						{
							ReadData(string.Format("Prefix{0}", i), 11);
						}

						string suffixIndex = string.Format("SuffixFlag{0}", i);

						ReadData(suffixIndex, 1);
						if (dataEntries.ContainsKey(suffixIndex))
						{
							ReadData(string.Format("Suffix{0}", i), 11);
						}
					}
					break;

				case ItemQuality.Unique:
				case ItemQuality.Set:
					ReadData("UniqueSetId", 12);
					break;

				default:
					break;
			}

			// TODO: check order of Runeword and personalized
			if (IsRuneword)
			{
				ReadData(16);
			}
			// TODO: check order of Runeword and personalized
			if (IsPersonalized)
			{
				ReadData(15);
			}
		}

		/// <summary>
		/// Decodes item specific data
		/// </summary>
		private void ReadItemDataExtendedSpecific()
		{
			if (ItemDefs.IsArmor(ItemCode))
			{
				// 1095 from ItemStatCost.txt. It just allows for a minimum of -1095 defense
				ReadData("Defense", 12);
				Defense -= 1095;
			}

			if (ItemDefs.IsWeapon(ItemCode) || ItemDefs.IsArmor(ItemCode))
			{
				ReadData("MaxDurability", 8);

				if (MaxDurability != 0)
				{
					ReadData("Durability", 8);
				}
				else
				{
					IsIndestructable = true;
				}
			}

			// Not sure of the order of socketed+stackable. Socketed arrows seem to mess up
			if (ItemDefs.IsStackable(ItemCode))
			{
				ReadData("Quantity", 9);
			}
			if (IsSocketed)
			{
				ReadData("SocketCount", 4);
			}

			// TODO: Maybe read the rest of the item properties? Most likely won't be implemented.
		}

		/// <summary>
		/// Attempts to remove a socketed item, this will fail horribly if used on runewords
		/// </summary>
		/// <param name="index">Index of socket to clear</param>
		public void RemoveSocketedItem(int index)
		{
			if (index < 0 || index > SocketsFilled)
			{
				return;
			}

			if (IsRuneword)
			{
				return;
			}

			Sockets.RemoveAt(index);
			SocketsFilled--;
		}

		/// <summary>
		/// Reads the remaining bits to the remainingBytes array and LAST property
		/// </summary>
		private void ReadRemainingBits()
		{
			int remainingByteCount = (int)(br.BitCount - br.Position) / 8;

			if (remainingByteCount > 0)
			{
				remainingBytes = br.ReadBytes(remainingByteCount);
			}

			if (br.BitCount - br.Position > 0)
			{
				ReadData("LAST", (int)(br.BitCount - br.Position));
			}
		}

		/// <summary>
		/// Reads a value (up to 32 bits) from the BitReader
		/// </summary>
		/// <param name="bitCount">Number of bits to read</param>
		private void ReadData(int bitCount)
		{
			ReadData(string.Format("Unknown{0}", dataEntryIndex), bitCount);
		}

		/// <summary>
		/// Reads a value (up to 32 bits) from the BitReader
		/// </summary>
		/// <param name="name">Name of the property to create to store the data</param>
		/// <param name="bitCount">Number of bits to read</param>
		private void ReadData(string name, int bitCount)
		{
			ItemData data = new ItemData();

			data.Index = dataEntryIndex++;
			data.BitCount = bitCount;
			data.Value = br.Read(bitCount);
			data.Name = name;

			dataEntries.Add(data.Name, data);
		}

		/// <summary>
		/// Reads a sequence of characters from the BitStream
		/// </summary>
		/// <param name="name">Name of property to store data in</param>
		/// <param name="characterCount">Number of characters to read</param>
		/// <param name="skipFollowingByte">Skip an additional byte after reading the characters</param>
		private void ReadString(string name, int characterCount, bool skipFollowingByte)
		{
			ItemData data = new ItemData();

			data.Index = dataEntryIndex++;
			data.Value = br.ReadString(characterCount, 8);
			data.BitCount = (data.Value as string).Length * 8;
			data.Name = name;

			if (skipFollowingByte)
			{
				br.ReadByte();
			}

			dataEntries.Add(data.Name, data);
		}

		/// <summary>
		/// Writes the specified item property to a given BitStream
		/// </summary>
		/// <param name="name">Name of property to write</param>
		/// <param name="stream">BitStream to write property to</param>
		private void WriteData(string name, BitStream stream)
		{
			if (dataEntries.ContainsKey(name))
			{
				throw new ApplicationException("Key not found in item data");
			}

			ItemData data = dataEntries[name];

			if (data.Value is string)
			{
				stream.Write((data.Value as string).ToCharArray());
			}
			else if (data.Value is ValueType)
			{
				stream.Write((uint)data.Value, 0, data.BitCount);
			}
		}

		/// <summary>
		/// Gets an object item property
		/// </summary>
		/// <param name="name">Name of property to get</param>
		/// <returns>Value of property</returns>
		private object GetDataObject(string name)
		{
			if (!dataEntries.ContainsKey(name))
			{
				return null;
			}

			return dataEntries[name].Value;
		}

		/// <summary>
		/// Gets an unsigned integer item property
		/// </summary>
		/// <param name="name">Name of property to get</param>
		/// <returns>Value of property</returns>
		private uint GetDataValue(string name)
		{
			if (!dataEntries.ContainsKey(name))
			{
				return 0;
			}

			return (uint)dataEntries[name].Value;
		}

		/// <summary>
		/// Gets a boolean item property
		/// </summary>
		/// <param name="name">Name of property to get</param>
		/// <returns>Value of property</returns>
		private bool GetDataBoolean(string name)
		{
			if (!dataEntries.ContainsKey(name))
			{
				return false;
			}

			if (dataEntries[name].Value.GetType() == typeof(bool))
			{
				return (bool)dataEntries[name].Value;
			}
			else
			{
				return (uint)dataEntries[name].Value == 1;
			}
		}

		/// <summary>
		/// Sets an item's property if it exists
		/// </summary>
		/// <param name="name">Name of property to set</param>
		/// <param name="value">Value to set property to</param>
		private void SetData(string name, object value)
		{
			if (!dataEntries.ContainsKey(name))
			{
				return;
			}

			dataEntries[name].Value = value;
		}

		/// <summary>
		/// Converts specified item into item format for save file
		/// </summary>
		/// <returns>Byte representation of item for save file</returns>
		public byte[] GetItemBytes()
		{
			BitStream bs = new BitStream();

			bs.Write(Utils.ReverseBits('J', 8), 0, 8);
			bs.Write(Utils.ReverseBits('M', 8), 0, 8);

			var ordered = dataEntries.OrderBy(n => n.Value.Index);

			foreach (var item in ordered)
			{
				if (item.Value.Value is string)
				{
					string value = item.Value.Value as string;
					foreach (var ch in value)
					{
						bs.Write(Utils.ReverseBits(ch, 8), 0, 8);
					}
					// This is odd, Reverse(32) = 4, but file wants 2
					bs.Write(Utils.ReverseBits(0x10, 7), 0, 8);
				}
				else if (item.Value.Value is ValueType)
				{
					// LAST key is the very last property added to the item data
					if (item.Key == "LAST")
						continue;

					TypeCode valueType = Type.GetTypeCode(item.Value.Value.GetType());

					if (valueType == TypeCode.UInt32)
					{
						uint value = (uint)item.Value.Value;

						if (item.Key == "Defense")
						{
							value += 1095;
						}

						value = Utils.ReverseBits(value, item.Value.BitCount);
						bs.Write(value, 0, item.Value.BitCount);
					}
					else if(valueType == TypeCode.Boolean)
					{
						bs.Write((bool)item.Value.Value);
					}
					else
					{
						throw new ApplicationException("Invalid ValueType!");
					}
				}
				else
				{
					throw new ApplicationException("Invalid data type in item dataEntries");
				}
			}

			if (remainingBytes != null)
			{
				bs.Write(Utils.ReverseByteArrayBits(remainingBytes));
			}

			if (dataEntries.ContainsKey("LAST"))
			{
				var lastEntry = dataEntries["LAST"];
				uint value = Utils.ReverseBits((uint)lastEntry.Value, lastEntry.BitCount);
				bs.Write((uint)value, 0, lastEntry.BitCount);
			}

			return Utils.ReverseByteArrayBits(bs.ToByteArray());
		}

		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();

			sb.Append(String.Format("{0} {1}", ItemCode, ItemDefs.GetItemDescription(ItemCode)));

			if (sockets.Count > 0)
			{
				sb.Append(" { ");
				for (int i = 0; i < sockets.Count; i++)
				{
					sb.Append(ItemDefs.GetItemDescription(sockets[i].ItemCode));

					if (i < sockets.Count - 1)
					{
						sb.Append(", ");
					}
				}
				sb.Append(" }");
			}

			return sb.ToString();
		}
	}
}
