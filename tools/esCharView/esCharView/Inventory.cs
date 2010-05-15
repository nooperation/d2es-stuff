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

		private byte[] unknownCorpseData = new byte[12];
		private List<Item> playerItems;
		private List<Item> corpseItems;
		private List<Item> mercItems;
		private List<Item> golemItems;

		/// <summary>
		/// All player items
		/// </summary>
		public List<Item> PlayerItems
		{
			get { return playerItems; }
			set { playerItems = value; }
		}

		/// <summary>
		/// Items in corpse
		/// </summary>
		public List<Item> CorpseItems
		{
			get { return corpseItems; }
			set { corpseItems = value; }
		}

		/// <summary>
		/// Mercenary equipment
		/// </summary>
		public List<Item> MercItems
		{
			get { return mercItems; }
			set { mercItems = value; }
		}

		/// <summary>
		/// Item(s) golem was created from
		/// </summary>
		public List<Item> GolemItems
		{
			get { return golemItems; }
			set { golemItems = value; }
		}

		/// <summary>
		/// Obtains the length of next Item in raw inventory data at specified position
		/// </summary>
		/// <param name="inventoryBytes">Raw inventory data</param>
		/// <param name="begin">Position in raw inventory data to start looking for item</param>
		/// <returns>Length in bytes of next item</returns>
		private int GetNextItemSize(byte[] inventoryBytes, int begin)
		{
			for (int i = begin + 2; i < inventoryBytes.Length - 1; i++)
			{
				// Header of next item record or end of inventory list reached
				if (inventoryBytes[i] == 'J' && inventoryBytes[i + 1] == 'M')
				{
					return i - begin;
				}

				// End of corpse inventory reached
				if (inventoryBytes[i] == 'j' && inventoryBytes[i + 1] == 'f')
				{
					return i - begin;
				}

				// End of merc inventory reached
				if (inventoryBytes[i] == 'k' && inventoryBytes[i + 1] == 'f')
				{
					return i - begin;
				}
			}

			return inventoryBytes.Length - begin;
		}

		/// <summary>
		/// Retrieves the next item from specified position in raw inventory data
		/// </summary>
		/// <param name="inventoryBytes">Raw inventory data</param>
		/// <param name="begin">Position in raw inventory data to start looking for item</param>
		/// <returns>Next item in raw inventory data</returns>
		private Item GetNextItem(byte[] inventoryBytes, ref int begin)
		{
			int itemDataSize = GetNextItemSize(inventoryBytes, begin);
			byte[] itemData = new byte[itemDataSize];
			Array.Copy(inventoryBytes, begin, itemData, 0, itemDataSize);

			Item item = new Item(itemData);

			begin += itemDataSize;
			if (item.IsSocketed)
			{
				for (int i = 0; i < item.SocketsFilled; i++)
				{
					item.Sockets.Add(GetNextItem(inventoryBytes, ref begin));
				}
			}

			return item;
		}

		/// <summary>
		/// Separates raw inventory data into more specific chunks of data
		/// </summary>
		/// <param name="inventoryBytes">Raw inventory data from save file</param>
		/// <param name="playerInventoryBytes">Inventory data for player</param>
		/// <param name="unknownCorpseDataBytes">Unknown data for corpse</param>
		/// <param name="corpseInventoryBytes">Inventory data for corpse</param>
		/// <param name="mercInventoryBytes">Inventory data for mercenary</param>
		/// <param name="golemInventoryBytes">Inventory data for golem</param>
		private void SplitInventoryBytes(byte[] inventoryBytes,
			out byte[] playerInventoryBytes,
			out byte[] unknownCorpseDataBytes,
			out byte[] corpseInventoryBytes,
			out byte[] mercInventoryBytes,
			out byte[] golemInventoryBytes)
		{
			int playerInventoryLength = 0;
			int unknownCorpseDataLength = 0;
			int corpseInventoryLength = 0;
			int mercInventoryLength = 0;
			int golemInventoryLength = 0;

			int playerInventoryStart = 0;
			int unknownCorpseDataStart = 0;
			int corpseInventoryStart = 0;
			int mercInventoryStart = 0;
			int golemInventoryStart = 0;

			bool hasCorpseData = false;

			// Player inventory data ends with "JM\x00\x00" or "JM\x01\x00"
			playerInventoryStart = 0;
			for (int i = playerInventoryStart + 5; i < inventoryBytes.Length; i++)
			{
				// End of player inventory list
				if (inventoryBytes[i] == 'J' && inventoryBytes[i + 1] == 'M' && inventoryBytes[i + 3] == 0x00)
				{
					// Corpse exists if JM\x01\x00 followed by 12 unknown bytes and JM (the first corpse item entry)
					if(inventoryBytes[i + 2] == 0x01 && inventoryBytes[i+16] == 'J' && inventoryBytes[i+17] == 'M')
					{
						hasCorpseData = true;
						playerInventoryLength = i - playerInventoryStart;
						break;
					}
					// No corpse exists if JM\x00\x00 followed by jf (ending tag of corpse data)
					else if (inventoryBytes[i + 2] == 0x00 && inventoryBytes[i + 4] == 'j' && inventoryBytes[i+5] == 'f')
					{
						playerInventoryLength = i - playerInventoryStart;
						break;
					}
				}
			}

			// When corpse data is present, 4 bytes from last player inventory JM header and 12 bytes
			//  of unknown corpse data exist. Must keep a copy of these 12 bytes for when we go to save
			//  the file
			unknownCorpseDataStart = playerInventoryStart + playerInventoryLength + 4;
			if (hasCorpseData)
			{
				for (int i = unknownCorpseDataStart; i < inventoryBytes.Length - 1; i++)
				{
					if (inventoryBytes[i] == 'J' && inventoryBytes[i + 1] == 'M')
					{
						unknownCorpseDataLength = i - unknownCorpseDataStart;
						break;
					}
				}
			}

			// Corpse inventory data exists between the end of player inventory list "JM\x0#\x00" and ends with "jf"
			//  12 bytes of unknown data are at the beginning of corpseInventoryBytes whenever corpse data is present
			corpseInventoryStart = unknownCorpseDataStart + unknownCorpseDataLength;
			if (!hasCorpseData)
			{
				corpseInventoryLength = 0;
			}
			else
			{
				for (int i = corpseInventoryStart + 4; i < inventoryBytes.Length - 1; i++)
				{
					if (inventoryBytes[i] == 'j' && inventoryBytes[i + 1] == 'f')
					{
						corpseInventoryLength = i - corpseInventoryStart;
						break;
					}
				}
			}

			// Merc inventory data is everything between "jf" and "kf"
			mercInventoryStart = corpseInventoryStart + corpseInventoryLength + 2;
			for (int i = mercInventoryStart; i < inventoryBytes.Length - 1; i++)
			{
				if (inventoryBytes[i] == 'k' && inventoryBytes[i + 1] == 'f')
				{
					mercInventoryLength = i - mercInventoryStart;
					break;
				}
			}

			// Golem inventory exists iff the byte after the end of Merc inventory is set to 1
			golemInventoryStart = mercInventoryStart + mercInventoryLength + 2;
			if (golemInventoryStart < inventoryBytes.Length && inventoryBytes[golemInventoryStart] == '\x01')
			{
				golemInventoryStart++; // Skip the 01 flag, useless to us
				golemInventoryLength = inventoryBytes.Length - golemInventoryStart;
			}

			playerInventoryBytes = new byte[playerInventoryLength];
			unknownCorpseDataBytes = new byte[unknownCorpseDataLength];
			corpseInventoryBytes = new byte[corpseInventoryLength];
			mercInventoryBytes = new byte[mercInventoryLength];
			golemInventoryBytes = new byte[golemInventoryLength];

			Array.Copy(inventoryBytes, playerInventoryStart, playerInventoryBytes, 0, playerInventoryBytes.Length);
			Array.Copy(inventoryBytes, unknownCorpseDataStart, unknownCorpseDataBytes, 0, unknownCorpseData.Length);
			Array.Copy(inventoryBytes, corpseInventoryStart, corpseInventoryBytes, 0, corpseInventoryBytes.Length);
			Array.Copy(inventoryBytes, mercInventoryStart, mercInventoryBytes, 0, mercInventoryBytes.Length);
			Array.Copy(inventoryBytes, golemInventoryStart, golemInventoryBytes, 0, golemInventoryBytes.Length);
		}

		/// <summary>
		/// Parses raw inventory data into Items
		/// </summary>
		/// <param name="inventoryBytes">Raw inventory data from save file</param>
		private void ProcessInventoryBytes(byte[] inventoryBytes)
		{
			playerItems = new List<Item>();
			corpseItems = new List<Item>();
			mercItems = new List<Item>();
			golemItems = new List<Item>();

			byte[] playerInventoryBytes;
			byte[] corpseInventoryBytes;
			byte[] mercInventoryBytes;
			byte[] golemInventoryBytes;

			SplitInventoryBytes(inventoryBytes, 
				out playerInventoryBytes,
				out unknownCorpseData,
				out corpseInventoryBytes,
				out mercInventoryBytes, 
				out golemInventoryBytes);

			int currentPosition;

			// Parse player's inventory items, skipping 4 byte header
			currentPosition = 4;
			while (currentPosition < playerInventoryBytes.Length)
			{
				Item currentItem = GetNextItem(playerInventoryBytes, ref currentPosition);
				playerItems.Add(currentItem);
			}

			// Parse corpse inventory, skipping 4 byte header and 12 byte unknown corpse data
			currentPosition = 4;
			while (currentPosition < corpseInventoryBytes.Length)
			{
				Item currentItem = GetNextItem(corpseInventoryBytes, ref currentPosition);
				corpseItems.Add(currentItem);
			}

			// Parse merc items
			currentPosition = 4;
			while (currentPosition < mercInventoryBytes.Length)
			{
				Item currentItem = GetNextItem(mercInventoryBytes, ref currentPosition);
				mercItems.Add(currentItem);
			}

			// Parse golem item(s)
			currentPosition = 0;
			while (currentPosition < golemInventoryBytes.Length)
			{
				Item currentItem = GetNextItem(golemInventoryBytes, ref currentPosition);
				golemItems.Add(currentItem);
			}
		}

		/// <summary>
		/// Adds raw data from an item to a list of raw data
		/// </summary>
		/// <param name="inventoryBytes">List of raw data to add item to</param>
		/// <param name="items">Item to add</param>
		private void AddItemsToInventoryBytes(ref List<Byte> inventoryBytes, List<Item> items)
		{
			foreach (Item item in items)
			{
				inventoryBytes.AddRange(item.GetItemBytes());

				foreach (Item itemSocket in item.Sockets)
				{
					inventoryBytes.AddRange(itemSocket.GetItemBytes());
				}
			}
		}

		/// <summary>
		/// Converts all item data into raw data for the save file
		/// </summary>
		/// <param name="hasMercenary">Character has a mercenary</param>
		/// <returns>Byte array containing raw inventory data</returns>
		public byte[] GetInventoryBytes(bool hasMercenary)
		{
			List<Byte> inventoryBytes = new List<byte>();
			byte[] magic = new byte[] { 0x4A, 0x4D };

			inventoryBytes.AddRange(magic);
			inventoryBytes.AddRange(BitConverter.GetBytes((short)playerItems.Count));

			// Dump inventory data
			AddItemsToInventoryBytes(ref inventoryBytes, playerItems);

			// Closing player inventory list header
			inventoryBytes.AddRange(magic);
			if (corpseItems.Count > 0)
			{
				inventoryBytes.Add(0x01);
			}
			else
			{
				inventoryBytes.Add(0x00);
			}
			inventoryBytes.Add(0x00);


			// Dump corpse data
			if (corpseItems.Count > 0)
			{
				inventoryBytes.AddRange(unknownCorpseData);
				inventoryBytes.AddRange(magic);
				inventoryBytes.AddRange(BitConverter.GetBytes((short)corpseItems.Count));

				AddItemsToInventoryBytes(ref inventoryBytes, corpseItems);
			}

			// Closing corpse inventory header
			inventoryBytes.Add(0x6a); // j
			inventoryBytes.Add(0x66); // f

			// Dump merc inventory
			if (hasMercenary)
			{
				inventoryBytes.AddRange(magic);
				inventoryBytes.AddRange(BitConverter.GetBytes((short)mercItems.Count));

				AddItemsToInventoryBytes(ref inventoryBytes, mercItems);
			}

			// Closing merc inventory header
			inventoryBytes.Add(0x6b); // k
			inventoryBytes.Add(0x66); // f

			if (golemItems.Count == 0)
			{
				// No iron golem, finish file off with 0x00
				inventoryBytes.Add(0x00);
			}
			else
			{
				// Yes iron golem, finish file off with 0x01 followed by item golem was
				//   spawned from
				inventoryBytes.Add((byte)golemItems.Count);
				AddItemsToInventoryBytes(ref inventoryBytes, golemItems);
			}

			return inventoryBytes.ToArray();
		}
	}
}
