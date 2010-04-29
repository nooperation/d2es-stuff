using System;
using System.Collections.Generic;
using System.Text;

namespace esCharView
{
	public class Inventory
	{
		public Inventory(byte[] inventoryBytes)
		{
			ProcessInventoryBytes(inventoryBytes);
		}

		private int hardItemCount = 0;
		private int originalItemCount = 0;

		public int HardItemCount 
		{
			get
			{
				return hardItemCount;
			}
		}
		
		private void ProcessInventoryBytes(byte[] inventoryBytes)
		{
			items = new List<Item>();
			corpseItems = new List<Item>();

			hardItemCount = BitConverter.ToInt16(inventoryBytes, 2);

			int itemBegin = 4;

			Item lastItem = null;
			int socketedItems = 0;
			bool inCorpse = false;
			int corpseItemCount = 0;

			for (int i = 4; i < inventoryBytes.Length - 1; i++)
			{
				// Reading all data upto the next JM tag. All inventory lists will have an extra JM at the end,
				// for corpse item list?
				if ((inventoryBytes[i] == 'J' && inventoryBytes[i + 1] == 'M') || i == inventoryBytes.Length-2)
				{
					int itemDataSize = i - itemBegin;

					// Corpse item list doesn't end with JM\0\0 so we need to treat the remainder of inventoryBytes
					//   as the last item in the corpse.
					if (inCorpse && i == inventoryBytes.Length - 2)
					{
						itemDataSize = inventoryBytes.Length - itemBegin;
					}

					if (!inCorpse && items.Count == hardItemCount && socketedItems == 0)
					{
						// Corpse data size should be somewhere around 0x0E bytes, if it's 0
						//   then we're at the end of an item list of a player without a corpse
						if (itemDataSize == 0)
						{
							continue;
						}

						corpseData = new byte[itemDataSize];
						Array.Copy(inventoryBytes, itemBegin, corpseData, 0, itemDataSize);

						i += 2;
						corpseItemCount = BitConverter.ToInt16(inventoryBytes, i);
						inCorpse = true;

						//i += 2;
						itemBegin = i+2;

						continue;
					}

					if (itemDataSize > 0)
					{
						Item item = new Item();

						byte[] itemData = new byte[itemDataSize];
						Array.Copy(inventoryBytes, itemBegin, itemData, 0, itemDataSize);

						BitReader br = new BitReader(itemData);

						br.Position = 27 - 16;
						item.Socketed = br.ReadBoolean();

						br.Position = 76 - 16;
						item.ItemData = itemData;
						item.ItemCode = br.ReadString(3, 8);
						item.Location = itemBegin;

						if (socketedItems > 0)
						{
							lastItem.Sockets.Add(item);
							socketedItems--;
						}
						else
						{
							if (item.Socketed)
							{
								br.Position = 108 - 16;
								socketedItems = br.ReadByte(3);
								lastItem = item;
							}

							if (inCorpse)
							{
								corpseItems.Add(item);
							}
							else
							{
								items.Add(item);
							}
						}
					}

					itemBegin = i + 2;
				}
			}
		}

		public byte[] GetInventoryBytes()
		{
			List<Byte> inventoryBytes = new List<byte>();
			byte[] magic = new byte[] { 0x4A, 0x4D };

			inventoryBytes.AddRange(magic);
			inventoryBytes.AddRange(BitConverter.GetBytes((short)items.Count));

			// Dump inventory data
			foreach (Item item in items)
			{
				inventoryBytes.AddRange(magic);
				inventoryBytes.AddRange(item.ItemData);

				foreach (Item itemSocket in item.Sockets)
				{
					inventoryBytes.AddRange(magic);
					inventoryBytes.AddRange(itemSocket.ItemData);
				}
			}

			// Dump corpse data
			if (corpseItems.Count > 0)
			{
				inventoryBytes.AddRange(magic);
				inventoryBytes.AddRange(corpseData);
				inventoryBytes.AddRange(magic);
				inventoryBytes.AddRange(BitConverter.GetBytes((short)corpseItems.Count));

				foreach (Item item in corpseItems)
				{
					inventoryBytes.AddRange(magic);
					inventoryBytes.AddRange(item.ItemData);

					foreach (Item itemSocket in item.Sockets)
					{
						inventoryBytes.AddRange(magic);
						inventoryBytes.AddRange(itemSocket.ItemData);
					}
				}
			}
			else
			{
				// Only non corpse inventory data ends in JM\0\0
				inventoryBytes.AddRange(magic);
				inventoryBytes.Add(0x00);
				inventoryBytes.Add(0x00);
			}

			return inventoryBytes.ToArray();
		}

		private byte[] corpseData = new byte[12];

		private List<Item> items;

		public List<Item> Items
		{
			get { return items; }
			set { items = value; }
		}

		private List<Item> corpseItems;

		public List<Item> CorpseItems
		{
			get { return corpseItems; }
			set { corpseItems = value; }
		}
	}
}
