using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Linq;
using BKSystem.IO;
using System.IO;
using System.ComponentModel;

namespace CharacterEditor
{
	public class Item : INotifyPropertyChanged
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

			public int Index { get; set; }
			public string Name { get; set; }
			public object Value { get; set; }
			public int BitCount { get; set; }

			public override string ToString()
			{
				return Value.ToString();
			}
		}

		public class PropertyInfo : INotifyPropertyChanged
		{
			private int id;

			public int ID
			{
				get { return id; }
				set
				{
					id = value;
					OnPropertyChange("ID");
					OnPropertyChange("PropertyName");
				}
			}

			public string PropertyName
			{
				get
				{
					return ItemDefs.GetPropertyName(ID);
				}
			}

			public int Value { get; set; }
			public int ParamValue { get; set; }
			public bool IsAdditionalProperty { get; set; }

			public override string ToString()
			{
				return string.Format("[{0}] {1} -> {2} [{3}]", ID, PropertyName, Value, ParamValue);
			}

			#region INotifyPropertyChanged Members

			public event PropertyChangedEventHandler PropertyChanged;

			private void OnPropertyChange(string propertyName)
			{
				if (PropertyChanged != null)
				{
					PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
				}
			}

			#endregion
		}

		private List<PropertyInfo> properties = new List<PropertyInfo>();
		private List<PropertyInfo> propertiesSet = new List<PropertyInfo>();
		private List<PropertyInfo> propertiesRuneword = new List<PropertyInfo>();
		private List<Item> sockets = new List<Item>();
		private static Dictionary<string, int> dataIndicies = new Dictionary<string, int>();
		private Dictionary<string, ItemData> dataEntries = new Dictionary<string, ItemData>();
		private BitStream bs;
		private byte[] remainingBytes;

		/// <summary>
		/// List of items stored in item's sockets
		/// </summary>
		public List<Item> Sockets
		{
			get { return sockets; }
		}
		/// <summary>
		/// Item properties
		/// </summary>
		public List<PropertyInfo> Properties
		{
			get { return properties; }
		}
		/// <summary>
		/// Set bonuses for wearing multiple parts
		/// </summary>
		public List<PropertyInfo> PropertiesSet
		{
			get { return propertiesSet; }
		}
		/// <summary>
		/// Runeword properties
		/// </summary>
		public List<PropertyInfo> PropertiesRuneword
		{
			get { return propertiesRuneword; }
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
		public bool IsPotion
		{
			get { return GetDataBoolean("IsPotion"); }
			set { SetData("IsPotion", value); }
		}
		public bool IsStoreItem
		{
			get { return GetDataBoolean("IsStoreItem"); }
			set { SetData("IsStoreItem", value); }
		}
		public bool IsNotInSocket
		{
			get { return GetDataBoolean("IsNotInSocket"); }
			set { SetData("IsNotInSocket", value); }
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

		// TODO: use SetData ?
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
				dataEntries["ItemCode"].Value = value.Substring(0, 3);
			}
		}

		public uint GoldAmountSmall
		{
			get { return GetDataValue("GoldAmountSmall"); }
			set { SetData("GoldAmountSmall", value); }
		}
		public uint GoldAmountLarge
		{
			get { return GetDataValue("GoldAmountLarge"); }
			set { SetData("GoldAmountLarge", value); }
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
		public uint ClassInfo
		{
			get { return GetDataValue("ClassInfo"); }
			set { SetData("ClassInfo", value); }
		}
		public uint InferiorQualityType
		{
			get { return GetDataValue("InferiorQualityType"); }
			set { SetData("InferiorQualityType", value); }
		}
		public uint SuperiorQualityType
		{
			get { return GetDataValue("SuperiorQualityType"); }
			set { SetData("SuperiorQualityType", value); }
		}
		public uint CharmData
		{
			get { return GetDataValue("CharmData"); }
			set { SetData("CharmData", value); }
		}
		public uint SpellId
		{
			get { return GetDataValue("SpellId"); }
			set { SetData("SpellId", value); }
		}
		public uint MonsterId
		{
			get { return GetDataValue("MonsterId"); }
			set { SetData("MonsterId", value); }
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
		public string EarName
		{
			get { return (string)GetDataObject("EarName"); }
			protected set { SetData("EarName", value.Length > 17 ? value.Substring(0, 17) : value); }
		}
		public string PersonalizedName
		{
			get { return (string)GetDataObject("PersonalizedName"); }
			protected set { SetData("PersonalizedName", value.Length > 17 ? value.Substring(0, 17) : value); }
		}
		public uint RunewordId
		{
			get { return GetDataValue("RunewordId"); }
			set { SetData("RunewordId", value); }
		}

		#endregion

		#region Affix

		public uint PrefixNameId
		{
			get { return GetDataValue("PrefixNameId"); }
			set { SetData("PrefixNameId", value); }
		}
		public uint SuffixNameId
		{
			get { return GetDataValue("SuffixNameId"); }
			set { SetData("SuffixNameId", value); }
		}
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
		public uint MagicPrefix
		{
			get { return GetDataValue("MagicPrefix"); }
			set { SetData("MagicPrefix", value); }
		}
		public uint MagicSuffix
		{
			get { return GetDataValue("MagicSuffix"); }
			set { SetData("MagicSuffix", value); }
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


		static Item()
		{
			CreateDataIndicies();
		}

		public Item(byte[] itemData)
		{
			bs = new BitStream(itemData);

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

			ReadString("ItemCode", 3, 8, true);

			// Read gold data if it's gold
			// TODO: Should this come later on? Not sure how to test this...
			if (ItemCode.ToLower() == "gld")
			{
				ReadItemDataGold();
				return;
			}

			if (IsSimpleItem)
			{
				return;
			}

			ReadItemDataExtended();

			// TODO: Not sure what this bit is, I can't find any items with it set
			// Extend.txt says it's some sort of random flag followed by 40 bits
			ReadData("RandomFlag", 1);
			if (RandomFlag)
			{
				ReadData("Unknown8", 8);
				ReadData("Unknown9", 32);
			}

		//	if (Resources.Instance.ResourceSet == "es300_r6d")
			{
				// Type specific extended data
				ReadItemDataExtendedSpecific();
			}
		}

		/// <summary>
		/// Decodes basic item data. All items have basic data
		/// </summary>
		private void ReadItemDataSimple()
		{
			bs.SkipBits(16); // "JM" header

			ReadData("IsEquipped", 1);
			ReadData("Unknown0", 2);
			ReadData("IsInSocket", 1);
			ReadData("IsIdentified", 1);
			ReadData("Unknown1", 1);
			ReadData("IsSwitchIn", 1);
			ReadData("IsSwitchOut", 1);
			ReadData("IsBroken", 1);
			ReadData("Unknown2", 1);
			ReadData("IsPotion", 1);
			ReadData("IsSocketed", 1);
			ReadData("Unknown3", 1);
			ReadData("IsStoreItem", 1);
			ReadData("IsNotInSocket", 1);
			ReadData("Unknown4", 1);
			ReadData("IsEar", 1);
			ReadData("IsStarterItem", 1);
			ReadData("Unknown5", 3);
			ReadData("IsSimpleItem", 1);
			ReadData("IsEthereal", 1);
			ReadData("Unknown6", 1);
			ReadData("IsPersonalized", 1);
			ReadData("IsGamble", 1);
			ReadData("IsRuneword", 1);
			ReadData("Unknown7", 15);
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
			ReadString("EarName", 7);

			if (EarName.Trim().Length == 0)
			{
				throw new Exception("Invalid Ear: Blank ear name");
			}

			foreach (char ch in EarName)
			{
				if (!Char.IsLetterOrDigit(ch) && ch != '-' && ch != '_')
				{
					throw new Exception("Invalid Ear: Ear name contains invalid characters");
				}
			}

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
				ReadData("GoldAmountLarge", 32);
			else
				ReadData("GoldAmountSmall", 12);

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
				ReadData("ClassInfo", 11);
			}

			switch (Quality)
			{
				case ItemQuality.Inferior:
					ReadData("InferiorQualityType", 3);
					break;
				case ItemQuality.Superior:
					ReadData("SuperiorQualityType", 3);
					break;

				case ItemQuality.Normal:
					if (ItemDefs.IsCharm(ItemCode))
					{
						ReadData("CharmData", 12);
					}
					if (ItemDefs.IsScrollOrTome(ItemCode))
					{
						ReadData("SpellId", 5);
					}
					else if (ItemDefs.IsMonsterPart(ItemCode))
					{
						ReadData("MonsterId", 10);
					}
					break;

				case ItemQuality.Magic:
					ReadData("MagicPrefix", 11);
					ReadData("MagicSuffix", 11);
					break;

				case ItemQuality.Rare:
				case ItemQuality.Craft:
					ReadData("PrefixNameId", 8);
					ReadData("SuffixNameId", 8);

					for (int i = 0; i < 3; i++)
					{
						string prefixIndex = string.Format("PrefixFlag{0}", i);

						ReadData(prefixIndex, 1);
						if (GetDataBoolean(prefixIndex))
						{
							ReadData(string.Format("Prefix{0}", i), 11);
						}

						string suffixIndex = string.Format("SuffixFlag{0}", i);

						ReadData(suffixIndex, 1);
						if (GetDataBoolean(suffixIndex))
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

			if (IsRuneword)
			{
				ReadData("RunewordId", 16);
			}
			if (IsPersonalized)
			{
				ReadString("PersonalizedName", 7); //TODO: 15 in ROT?
			}
		}

		/// <summary>
		/// Decodes item specific data
		/// </summary>
		private void ReadItemDataExtendedSpecific()
		{
			if (ItemDefs.IsArmor(ItemCode))
			{
				ReadData("Defense", ItemDefs.ItemStatCostsByName["armorclass"].SaveBits); // TODO: Changed for ROT
				Defense -= (uint)ItemDefs.ItemStatCostsByName["armorclass"].SaveAdd;
			}

			if (ItemDefs.IsWeapon(ItemCode) || ItemDefs.IsArmor(ItemCode))
			{
				ReadData("MaxDurability", ItemDefs.ItemStatCostsByName["maxdurability"].SaveBits);
				MaxDurability -= (uint)ItemDefs.ItemStatCostsByName["maxdurability"].SaveAdd;

				if (MaxDurability != 0)
				{
					ReadData("Durability", ItemDefs.ItemStatCostsByName["durability"].SaveBits);
					Durability -= (uint)ItemDefs.ItemStatCostsByName["durability"].SaveAdd;
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

			if (Quality == ItemQuality.Set)
			{
				ReadData("NumberOfSetProperties", 5);
			}

			ReadPropertyList(properties);

			if (Quality == ItemQuality.Set)
			{
				int numberOfSetProperties = (int)GetDataValue("NumberOfSetProperties");

				while (bs.RemainingBits > 9)
				{
					ReadPropertyList(propertiesSet);
				}
			}

			if (IsRuneword)
			{
				ReadPropertyList(propertiesRuneword);
			}
		}

		/// <summary>
		/// Reads a list of properties to specified property list from the BitReader
		/// </summary>
		/// <param name="propertyList">List of properties to read properties into</param>
		private void ReadPropertyList(List<PropertyInfo> propertyList)
		{
			while (true)
			{
				int currentPropertyID = (int)bs.ReadReversed(9);
				if (currentPropertyID == 0x1ff)
				{
					propertyList.Add(new PropertyInfo() { ID = currentPropertyID });
					break;
				}

				ReadPropertyData(propertyList, currentPropertyID);
			}
		}

		/// <summary>
		/// Reads property data for a specified ID from BitReader
		/// </summary>
		/// <param name="propertyList">List of properties to add data to</param>
		/// <param name="currentPropertyID">ID of property to read from BitReader</param>
		/// <param name="isAdditional">Property to read has no header. Found in damage type properties</param>
		private void ReadPropertyData(List<PropertyInfo> propertyList, int currentPropertyID, bool isAdditional = false)
		{
			ItemStatCost statCost = ItemDefs.ItemStatCostsById[currentPropertyID];
			PropertyInfo currentPropertyInfo = new PropertyInfo();

			currentPropertyInfo.IsAdditionalProperty = isAdditional;
			currentPropertyInfo.ID = currentPropertyID;
			currentPropertyInfo.Value = (int)bs.ReadReversed(statCost.SaveBits) - statCost.SaveAdd;

			if (statCost.SaveParamBits > 0)
			{
				currentPropertyInfo.ParamValue = (int)bs.ReadReversed(statCost.SaveParamBits);
			}

			propertyList.Add(currentPropertyInfo);

			switch (statCost.Stat)
			{
				case "item_maxdamage_percent":
					ReadPropertyData(propertyList, ItemDefs.ItemStatCostsByName["item_mindamage_percent"].ID, true);
					break;
				case "firemindam":
					ReadPropertyData(propertyList, ItemDefs.ItemStatCostsByName["firemaxdam"].ID, true);
					break;
				case "lightmindam":
					ReadPropertyData(propertyList, ItemDefs.ItemStatCostsByName["lightmaxdam"].ID, true);
					break;
				case "magicmindam":
					ReadPropertyData(propertyList, ItemDefs.ItemStatCostsByName["magicmaxdam"].ID, true);
					break;
				case "coldmindam":
					ReadPropertyData(propertyList, ItemDefs.ItemStatCostsByName["coldmaxdam"].ID, true);
					ReadPropertyData(propertyList, ItemDefs.ItemStatCostsByName["coldlength"].ID, true);
					break;
				case "poisonmindam":
					ReadPropertyData(propertyList, ItemDefs.ItemStatCostsByName["poisonmaxdam"].ID, true);
					ReadPropertyData(propertyList, ItemDefs.ItemStatCostsByName["poisonlength"].ID, true);
					break;
				default:
					break;
			}

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
			int paddingBitCount = GetPaddingBitCount();
			int remainingByteCount = (int)((int)(bs.RemainingBits) - paddingBitCount) / 8;

			if (remainingByteCount > 0)
			{
				remainingBytes = bs.ReadReversedBytes(remainingByteCount);
			}

			if ((bs.RemainingBits) - paddingBitCount > 0)
			{
				ReadData("LAST", (int)(bs.RemainingBits) - paddingBitCount);
			}
		}

		/// <summary>
		/// Reads a value (up to 32 bits) from the BitReader
		/// </summary>
		/// <param name="name">Name of the property to create to store the data</param>
		/// <param name="bitCount">Number of bits to read</param>
		private void ReadData(string name, int bitCount)
		{
			ItemData data = new ItemData();

			if (!dataIndicies.ContainsKey(name))
			{
				throw new Exception("ReadData: Invalid property name specified");
			}

			data.Index = dataIndicies[name];
			data.BitCount = bitCount;
			data.Value = bs.ReadReversed(bitCount);
			data.Name = name;

			dataEntries.Add(data.Name, data);
		}

		/// <summary>
		/// Reads a sequence of characters from the BitStream
		/// </summary>
		/// <param name="name">Name of property to store data in</param>
		/// <param name="characterCount">Number of characters to read</param>
		/// <param name="skipFollowingByte">Skip an additional byte after reading the characters</param>
		private void ReadString(string name, int characterCount, int bitsPerChar, bool skipFollowingByte)
		{
			ItemData data = new ItemData();

			if (!dataIndicies.ContainsKey(name))
			{
				throw new Exception("ReadString: Invalid property name specified");
			}

			data.Index = dataIndicies[name];
			data.Value = bs.ReadReversedString(characterCount, bitsPerChar);
			data.BitCount = (data.Value as string).Length * bitsPerChar;
			data.Name = name;

			if (skipFollowingByte)
			{
				bs.ReadReversedByte();
			}

			dataEntries.Add(data.Name, data);
		}

		/// <summary>
		/// Reads a null terminated string from the BitStream
		/// </summary>
		/// <param name="name">Name of property to store data in</param>
		/// <param name="characterCount">Number of characters to read</param>
		private void ReadString(string name, int bitsPerChar)
		{
			ItemData data = new ItemData();

			if (!dataIndicies.ContainsKey(name))
			{
				throw new Exception("ReadString: Invalid property name specified");
			}

			data.Index = dataIndicies[name];
			data.Value = bs.ReadReversedString(bitsPerChar);
			data.BitCount = (data.Value as string).Length * bitsPerChar;
			data.Name = name;

			dataEntries.Add(data.Name, data);
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
			OnPropertyChange(name);
		}

		/// <summary>
		/// Gets the number of bits used for 0 padding to set the remaining bits in last byte to 8
		/// </summary>
		/// <returns>Number of bits used for padding</returns>
		private int GetPaddingBitCount()
		{
			long brPos = bs.Position;

			if (!IsSimpleItem)
			{
				bs.Position -= 9;
				while (bs.Position > 0)
				{
					if (bs.ReadReversed(9) == 0x1ff)
					{
						bs.Position = brPos;
						return (int)(bs.RemainingBits);
					}
					bs.Position -= 10;
				}
			}

			bs.Position = brPos;
			return 0;
		}

		/// <summary>
		/// Converts specified item into item format for save file
		/// </summary>
		/// <returns>Byte representation of item for save file</returns>
		public byte[] GetItemBytes()
		{
			BitStream bs = new BitStream();

			bs.WriteReversed('J', 8);
			bs.WriteReversed('M', 8);

			var ordered = dataEntries.OrderBy(n => n.Value.Index);

			foreach (var item in ordered)
			{
				if (item.Value.Value is string)
				{
					string value = item.Value.Value as string;

					if (item.Key == "ItemCode")
					{
						foreach (var ch in value)
						{
							bs.WriteReversed(ch, 8);
						}
						bs.WriteReversed(' ', 8);
					}
					else if (item.Key == "EarName" || item.Key == "PersonalizedName")
					{
						foreach (var ch in value)
						{
							bs.WriteReversed(ch, 7);
						}
						bs.WriteReversed(0, 7);
					}
					else
					{
						throw new Exception("Unknown string type in item data");
					}
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

						if (item.Key == "Defense" )
						{
							value += (uint)ItemDefs.ItemStatCostsByName["armorclass"].SaveAdd;
						}
						else if (item.Key == "MaxDurability")
						{
							value += (uint)ItemDefs.ItemStatCostsByName["maxdurability"].SaveAdd;
						}
						else if (item.Key == "Durability")
						{
							value += (uint)ItemDefs.ItemStatCostsByName["durability"].SaveAdd;
						}

						bs.WriteReversed(value, item.Value.BitCount);
					}
					else if (valueType == TypeCode.Int32)
					{
						bs.WriteReversed((uint)((int)item.Value.Value), item.Value.BitCount);
					}
					else if (valueType == TypeCode.Boolean)
					{
						bs.Write((bool)item.Value.Value);
					}
					else
					{
						throw new Exception("Invalid ValueType!");
					}
				}
				else
				{
					throw new Exception("Invalid data type in item dataEntries");
				}
			}

			foreach (var item in properties)
			{
				WriteItemProperty(bs, item);
			}

			foreach (var item in propertiesSet)
			{
				WriteItemProperty(bs, item);
			}

			foreach (var item in propertiesRuneword)
			{
				WriteItemProperty(bs, item);
			}

			// Some simple items do have remaining data such as the soulstone
			if (IsSimpleItem)
			{
				//TODO: Enable renaming of ear and personlized names?
				if (remainingBytes != null)
				{
					bs.WriteReversed(remainingBytes);
				}

				if (dataEntries.ContainsKey("LAST"))
				{
					var lastEntry = dataEntries["LAST"];
					bs.WriteReversed((uint)lastEntry.Value, lastEntry.BitCount);
				}
			}
			else
			{
				// Fill the last byte with 0 if it's not already full
				if ((bs.Position % 8) != 0)
				{
					int bitsToAdd = 8 - (int)(bs.Position % 8);
					if (bitsToAdd > 0)
					{
						bs.WriteReversed(0, bitsToAdd);
					}
				}
			}

			return bs.ToReversedByteArray();
		}

		/// <summary>
		/// Writes an item property to the BitStream
		/// </summary>
		/// <param name="bs">Bitstream to write property to</param>
		/// <param name="property">Property to write</param>
		private void WriteItemProperty(BitStream bs, PropertyInfo property)
		{
			if (property.ID == 0x1ff)
			{
				bs.WriteReversed(property.ID, 9);
				return;
			}

			ItemStatCost statCost = ItemDefs.ItemStatCostsById[property.ID];

			int fixedValue = property.Value + statCost.SaveAdd;

			if (!property.IsAdditionalProperty)
			{
				bs.WriteReversed(property.ID, 9);
			}

			bs.WriteReversed(fixedValue, statCost.SaveBits);

			if (statCost.SaveParamBits > 0)
			{
				bs.WriteReversed(property.ParamValue, statCost.SaveParamBits);
			}
		}

		/// <summary>
		/// Creates a list of indicies for each data entry name
		/// </summary>
		private static void CreateDataIndicies()
		{
			int index = 0;

			// Simple data
			dataIndicies.Add("IsEquipped", index++);
			dataIndicies.Add("Unknown0", index++);
			dataIndicies.Add("IsInSocket", index++);
			dataIndicies.Add("IsIdentified", index++);
			dataIndicies.Add("Unknown1", index++);
			dataIndicies.Add("IsSwitchIn", index++);
			dataIndicies.Add("IsSwitchOut", index++);
			dataIndicies.Add("IsBroken", index++);
			dataIndicies.Add("Unknown2", index++);
			dataIndicies.Add("IsPotion", index++);
			dataIndicies.Add("IsSocketed", index++);
			dataIndicies.Add("Unknown3", index++);
			dataIndicies.Add("IsStoreItem", index++);
			dataIndicies.Add("IsNotInSocket", index++);
			dataIndicies.Add("Unknown4", index++);
			dataIndicies.Add("IsEar", index++);
			dataIndicies.Add("IsStarterItem", index++);
			dataIndicies.Add("Unknown5", index++);
			dataIndicies.Add("IsSimpleItem", index++);
			dataIndicies.Add("IsEthereal", index++);
			dataIndicies.Add("Unknown6", index++);
			dataIndicies.Add("IsPersonalized", index++);
			dataIndicies.Add("IsGamble", index++);
			dataIndicies.Add("IsRuneword", index++);
			dataIndicies.Add("Unknown7", index++);
			dataIndicies.Add("Location", index++);
			dataIndicies.Add("PositionOnBody", index++);
			dataIndicies.Add("PositionX", index++);
			dataIndicies.Add("PositionY", index++);
			dataIndicies.Add("StorageId", index++);

			// Ear data
			dataIndicies.Add("EarClass", index++);
			dataIndicies.Add("EarLevel", index++);
			dataIndicies.Add("EarName", index++);

			// Extended data
			dataIndicies.Add("ItemCode", index++);

			// Gold Data
			dataIndicies.Add("UnknownGoldFlag", index++);
			dataIndicies.Add("GoldAmountLarge", index++);
			dataIndicies.Add("GoldAmountSmall", index++);

			// Extended data
			dataIndicies.Add("SocketsFilled", index++);
			dataIndicies.Add("Id", index++);
			dataIndicies.Add("Level", index++);
			dataIndicies.Add("Quality", index++);
			dataIndicies.Add("HasGraphic", index++);
			dataIndicies.Add("Graphic", index++);
			dataIndicies.Add("ClassFlag", index++);
			dataIndicies.Add("ClassInfo", index++);
			dataIndicies.Add("InferiorQualityType", index++);
			dataIndicies.Add("SuperiorQualityType", index++);
			dataIndicies.Add("CharmData", index++);
			dataIndicies.Add("SpellId", index++);
			dataIndicies.Add("MonsterId", index++);
			dataIndicies.Add("MagicPrefix", index++);
			dataIndicies.Add("MagicSuffix", index++);
			dataIndicies.Add("PrefixNameId", index++);
			dataIndicies.Add("SuffixNameId", index++);
			dataIndicies.Add("PrefixFlag0", index++);
			dataIndicies.Add("Prefix0", index++);
			dataIndicies.Add("SuffixFlag0", index++);
			dataIndicies.Add("Suffix0", index++);
			dataIndicies.Add("PrefixFlag1", index++);
			dataIndicies.Add("Prefix1", index++);
			dataIndicies.Add("SuffixFlag1", index++);
			dataIndicies.Add("Suffix1", index++);
			dataIndicies.Add("PrefixFlag2", index++);
			dataIndicies.Add("Prefix2", index++);
			dataIndicies.Add("SuffixFlag2", index++);
			dataIndicies.Add("Suffix2", index++);
			dataIndicies.Add("UniqueSetId", index++);
			dataIndicies.Add("RunewordId", index++);
			dataIndicies.Add("PersonalizedName", index++);

			// Extended data
			dataIndicies.Add("RandomFlag", index++);
			dataIndicies.Add("Unknown8", index++);
			dataIndicies.Add("Unknown9", index++);

			// Extended specific data
			dataIndicies.Add("Defense", index++);
			dataIndicies.Add("MaxDurability", index++);
			dataIndicies.Add("Durability", index++);
			dataIndicies.Add("Quantity", index++);
			dataIndicies.Add("SocketCount", index++);



			dataIndicies.Add("NumberOfSetProperties", index++);

			// Properties are managed in lists

			// Very last data
			dataIndicies.Add("LAST", int.MaxValue);
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

		#region INotifyPropertyChanged Members

		public event PropertyChangedEventHandler PropertyChanged;

		private void OnPropertyChange(string propertyName)
		{
			if (PropertyChanged != null)
			{
				PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
			}
		}

		#endregion
	}
}
