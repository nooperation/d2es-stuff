using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace esCharView
{
	class Character
	{
		private byte[] headerBytes;
		private byte[] inventoryBytes;
		private byte[] footerBytes;
		private byte characterFlags;
		private string filePath;
		private Inventory inventory;

		public Character()
		{

		}

		public int HeaderSize { get { return headerBytes.Length; } }
		public int InventorySize { get { return inventoryBytes.Length; } }
		public int FooterSize { get { return footerBytes.Length; } }

		public String Name
		{
			get
			{
				return ASCIIEncoding.ASCII.GetString(headerBytes, 0x14, 16);
			}
			set
			{
				byte[] paddedName = new byte[16];
				byte[] newName = ASCIIEncoding.ASCII.GetBytes(value);

				//15 instead of 16 to keep trailing null character
				Array.Copy(newName, paddedName, newName.Length < 15 ? newName.Length : 15); 
				Array.Copy(paddedName, 0, headerBytes, 0x14, paddedName.Length);
			}
		}

		public bool Hardcore
		{
			get
			{
				return (characterFlags & 0x04) > 0;
			}
		}
		public bool Died 
		{ 
			get
			{
				return (characterFlags & 0x08) > 0;
			}
		}
		public bool Expansion
		{
			get
			{
				return (characterFlags & 0x20) > 0;
			}
		}
		public int UnknownFlags
		{
			get
			{
				return (characterFlags & 0xd3);
			}
		}

		public Inventory Inventory
		{
			get
			{
				return inventory;
			}
			set
			{
				inventory = value;
			}
		}

		public void Read(string filePath)
		{
			this.filePath = filePath;

			ReadHeaders();
		}

		public void UpdateInventoryHeaders()
		{
			inventoryBytes = Inventory.GetInventoryBytes();
		}

		public void SetCharacterFlags(bool expansion, bool died, bool hardcore, byte unknownFlags)
		{
			byte characterFlags = unknownFlags;

			if (expansion)
			{
				characterFlags |= 0x20;
			}
			if (died)
			{
				characterFlags |= 0x08;
			}
			if (hardcore)
			{
				characterFlags |= 0x04;
			}

			headerBytes[0x24] = characterFlags;
		}

		public void Write(string filePath)
		{
			UpdateInventoryHeaders();

			byte[] rawCharacterData = new byte[headerBytes.Length + inventoryBytes.Length + footerBytes.Length];

			Array.Copy(headerBytes, rawCharacterData, headerBytes.Length);
			Array.Copy(inventoryBytes, 0, rawCharacterData, headerBytes.Length, inventoryBytes.Length);
			Array.Copy(footerBytes, 0, rawCharacterData, headerBytes.Length + inventoryBytes.Length, footerBytes.Length);

			FixHeaders(ref rawCharacterData);

			File.WriteAllBytes(filePath, rawCharacterData);
		}

		public void Write()
		{
			Write(filePath);
		}

		private void ReadHeaders()
		{
			byte[] rawCharacterData = File.ReadAllBytes(filePath);
			int itemListBegin = FindItemListBegin(rawCharacterData);
			int itemListEnd = FindItemListEnd(rawCharacterData);

			headerBytes = new byte[itemListBegin];
			inventoryBytes = new byte[itemListEnd - itemListBegin];
			footerBytes = new byte[rawCharacterData.Length - itemListEnd];

			Array.Copy(rawCharacterData, 0, headerBytes, 0, headerBytes.Length);
			Array.Copy(rawCharacterData, itemListBegin, inventoryBytes, 0, inventoryBytes.Length);
			Array.Copy(rawCharacterData, itemListEnd, footerBytes, 0, footerBytes.Length);

			inventory = new Inventory(inventoryBytes);
			characterFlags = headerBytes[0x24];
		}

		public void FixHeaders(ref byte[] rawCharacterData)
		{
			byte[] fileSizeBytes = BitConverter.GetBytes(rawCharacterData.Length);
			Array.Copy(fileSizeBytes, 0, rawCharacterData, 8, 4);

			uint checksum = CalculateChecksum(rawCharacterData);

			byte[] checksumBytes = BitConverter.GetBytes(checksum);
			Array.Copy(checksumBytes, 0, rawCharacterData, 12, 4);
		}

		// Source: ehertlein ( http://forums.diii.net/showthread.php?t=532037&page=41 )
		private static uint CalculateChecksum(byte[] fileBytes)
		{
			uint hexTest = 0x80000000;
			uint checksum = 0;

			// clear out the old checksum
			fileBytes[12] = 0;
			fileBytes[13] = 0;
			fileBytes[14] = 0;
			fileBytes[15] = 0;

			foreach (byte currentByte in fileBytes)
			{
				if ((checksum & hexTest) == hexTest)
				{
					checksum = checksum << 1;
					checksum = checksum + 1;
				}
				else
				{
					checksum = checksum << 1;
				}

				checksum += currentByte;
			}

			return checksum;
		}

		private static int FindItemListBegin(byte[] rawCharacterData)
		{
			for (int i = 768; i < rawCharacterData.Length - 1; i++)
			{
				if (rawCharacterData[i] == 'J' && rawCharacterData[i + 1] == 'M')
				{
					return i;
				}
			}

			return 0;
		}

		private static int FindItemListEnd(byte[] rawCharacterData)
		{
			for (int i = rawCharacterData.Length - 1; i > 768; i--)
			{
				if (rawCharacterData[i - 1] == 'j' && rawCharacterData[i] == 'f')
				{
					return i - 1;
				}
			}

			return 0;
		}
	}
}
