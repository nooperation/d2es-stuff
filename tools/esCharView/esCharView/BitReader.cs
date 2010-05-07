using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

//http://forum.valhallalegends.com/index.php?PHPSESSID=ba8cac6978590e8cbdb57f03a569e775&topic=16354.msg165191#msg165191
// E.T -> http://forum.valhallalegends.com/index.php?action=profile;u=4447
namespace esCharView
{
	/// <summary>
	/// Utility class to parse an octet stream from a byte arrays, supporting bit packing.
	/// </summary>
	public class BitReader
	{
		public const int BYTE_MASK = 0xFF;
		public const int BYTE_BITS = 8;
		public const int WORD_BITS = 16;
		public const int DWORD_BITS = 32;
		public const int QWORD_BITS = 64;

		protected byte[] data;
		protected int byteOffset;
		protected int bitOffset;

		#region Constructors
		public BitReader(int length)
		{
			this.data = new byte[length];
		}
		public BitReader(int length, long position)
		{
			this.data = new byte[length];
			this.Position = position;
		}
		public BitReader(int length, int byteOffset)
		{
			this.data = new byte[length];
			this.byteOffset = byteOffset;
		}
		public BitReader(int length, int byteOffset, int bitOffset)
		{
			this.data = new byte[length];
			this.byteOffset = byteOffset;
			this.bitOffset = bitOffset;
		}

		public BitReader(byte[] data)
		{
			this.data = data;
		}
		public BitReader(byte[] data, long position)
		{
			this.data = data;
			this.Position = position;
		}
		public BitReader(byte[] data, int byteOffset)
		{
			this.data = data;
			this.byteOffset = byteOffset;
		}
		public BitReader(byte[] data, int byteOffset, int bitOffset)
		{
			this.data = data;
			this.byteOffset = byteOffset;
			this.bitOffset = bitOffset;
		}

		#endregion Constructors

		/// <summary>
		/// Byte array associated with the current instance.
		/// </summary>
		public byte[] Data
		{
			get { return this.data; }
			set
			{
				if (this.data.Length < value.Length)
				{
					this.byteOffset = 0;
					this.bitOffset = 0;
				}
				this.data = value;
			}
		}
		/// <summary>
		/// Length of the associated byte array, in bytes.
		/// </summary>
		public int ByteCount
		{
			get { return this.data.Length; }
			set
			{
				byte[] cData = this.data;
				this.data = new byte[value];
				Array.Copy(cData, this.data, Math.Min(this.ByteCount, cData.Length));
				if (this.data.Length < cData.Length)
				{
					this.byteOffset = 0;
					this.bitOffset = 0;
				}
			}
		}
		/// <summary>
		/// Length of the associated byte array, in bits.
		/// </summary>
		public long BitCount
		{
			get { return this.data.Length << 3; }
			set
			{
				byte[] cData = this.data;
				this.data = new byte[(int)(value >> 3) + (value & 7) != 0 ? 1 : 0];
				Array.Copy(cData, this.data, Math.Min(this.ByteCount, cData.Length));
				if (this.data.Length < cData.Length)
				{
					this.byteOffset = 0;
					this.bitOffset = 0;
				}
			}
		}

		/// <summary>
		/// Offset of the current byte.
		/// </summary>
		public int ByteOffset
		{
			get { return this.byteOffset; }
			set { this.byteOffset = value; }
		}
		/// <summary>
		/// Offset of the current bit, relative to the current byte.
		/// </summary>
		public int BitOffset
		{
			get { return this.bitOffset; }
			set { this.bitOffset = value; }
		}
		/// <summary>
		/// Absolute offset of the current bit (in bits).
		/// </summary>
		public long Position
		{
			get { return ((uint)this.byteOffset << 3) | (uint)this.bitOffset; }
			set
			{
				this.byteOffset = (int)(value >> 3);
				this.bitOffset = (int)value & 7;
			}
		}


		protected void CheckLength(int bits, int maxBits)
		{
			if (bits > maxBits || bits < 1)
				throw new ArgumentOutOfRangeException("bits", bits,
					String.Concat("Number of bits must be higher than 0 and equal to or less than ", maxBits));
		}
		/*
		protected void CheckOffset(int bits)
		{
			if (bits + this.Offset > this.BitCount)
				throw new OverflowException(String.Format("Less than {0} bits remain! Offset: {1}, Length: {2}", 
					bits, this.Offset, this.BitCount));
		}
		*/

		/// <summary>
		/// Increments the byte offset by the specified number.
		/// </summary>
		public void SkipBytes(int bytes)
		{
			this.byteOffset += bytes;
		}
		/// <summary>
		/// Increments the current position in array by the specified number of bits.
		/// </summary>
		public void SkipBits(int bits)
		{
			this.bitOffset += bits & 7;
			if ((this.bitOffset & BYTE_BITS) == BYTE_BITS)
			{
				this.bitOffset ^= BYTE_BITS;
				this.byteOffset++;
			}
			this.byteOffset += bits >> 3;
		}

		/// <summary>
		/// Reads a number of bits, incrementing offsets, and write them to dest.
		/// <para>Be very careful not to overflow when using this function!</para>
		/// <para>Normally you shouldn't have to use it directly...</para>
		/// </summary>
		/// <param name="dest">Pointer to the location where the data will be written.</param>
		/// <param name="bits">Number of bits to read (and copy.)</param>
		unsafe public void Read(byte* dest, int bits)
		{
			int mBits, cBits, destByte = 0, destBit = 0;
			while (true)
			{
				mBits = BYTE_BITS - (destBit > this.bitOffset ? destBit : this.bitOffset);
				if ((cBits = bits < mBits ? bits : mBits) == BYTE_BITS)
				{
					dest[destByte++] = this.data[this.byteOffset++];
				}
				else
				{
					dest[destByte] = (byte)(
						(dest[destByte] & ((BYTE_MASK >> (BYTE_BITS - destBit)) | (BYTE_MASK << (destBit + cBits))))
						| (((this.data[this.byteOffset] >> this.bitOffset) & (BYTE_MASK >> (BYTE_BITS - cBits)))
						   << destBit)
						);
					destBit += cBits;
					if ((destBit & BYTE_BITS) == BYTE_BITS)
					{
						destBit ^= BYTE_BITS;
						destByte++;
					}
					this.bitOffset += cBits;
					if ((this.bitOffset & BYTE_BITS) == BYTE_BITS)
					{
						this.bitOffset ^= BYTE_BITS;
						this.byteOffset++;
					}
				}
				if ((bits -= cBits) == 0)
					break;
			}
		}

		unsafe public bool ReadBoolean()
		{
			byte retVal;
			this.Read((byte*)&retVal, BYTE_BITS);
			return retVal != 0;
		}
		unsafe public bool ReadBoolean(int bits)
		{
			this.CheckLength(bits, DWORD_BITS);
			int retVal;
			this.Read((byte*)&retVal, bits);
			return retVal != 0;
		}

		unsafe public byte ReadByte()
		{
			byte retVal;
			this.Read((byte*)&retVal, BYTE_BITS);
			return retVal;
		}
		unsafe public byte ReadByte(int bits)
		{
			this.CheckLength(bits, BYTE_BITS);
			byte retVal;
			this.Read((byte*)&retVal, bits);
			return retVal;
		}

		unsafe public sbyte ReadSByte()
		{
			sbyte retVal;
			this.Read((byte*)&retVal, BYTE_BITS);
			return retVal;
		}
		unsafe public sbyte ReadSByte(int bits)
		{
			this.CheckLength(bits, BYTE_BITS);
			sbyte retVal;
			this.Read((byte*)&retVal, bits);
			return retVal;
		}

		unsafe public short ReadInt16()
		{
			short retVal;
			this.Read((byte*)&retVal, WORD_BITS);
			return retVal;
		}
		unsafe public short ReadInt16(int bits)
		{
			this.CheckLength(bits, WORD_BITS);
			short retVal;
			this.Read((byte*)&retVal, bits);
			return retVal;
		}

		unsafe public ushort ReadUInt16()
		{
			ushort retVal;
			this.Read((byte*)&retVal, WORD_BITS);
			return retVal;
		}
		unsafe public ushort ReadUInt16(int bits)
		{
			this.CheckLength(bits, WORD_BITS);
			ushort retVal;
			this.Read((byte*)&retVal, bits);
			return retVal;
		}

		unsafe public int ReadInt32()
		{
			int retVal;
			this.Read((byte*)&retVal, DWORD_BITS);
			return retVal;
		}
		unsafe public int ReadInt32(int bits)
		{
			this.CheckLength(bits, DWORD_BITS);
			int retVal;
			this.Read((byte*)&retVal, bits);
			return retVal;
		}

		unsafe public uint ReadUInt32()
		{
			uint retVal;
			this.Read((byte*)&retVal, DWORD_BITS);
			return retVal;
		}
		unsafe public uint ReadUInt32(int bits)
		{
			this.CheckLength(bits, DWORD_BITS);
			uint retVal;
			this.Read((byte*)&retVal, bits);
			return retVal;
		}

		unsafe public long ReadInt64()
		{
			long retVal;
			this.Read((byte*)&retVal, QWORD_BITS);
			return retVal;
		}
		unsafe public long ReadInt64(int bits)
		{
			this.CheckLength(bits, QWORD_BITS);
			long retVal;
			this.Read((byte*)&retVal, bits);
			return retVal;
		}

		unsafe public ulong ReadUInt64()
		{
			ulong retVal;
			this.Read((byte*)&retVal, QWORD_BITS);
			return retVal;
		}
		unsafe public ulong ReadUInt64(int bits)
		{
			this.CheckLength(bits, QWORD_BITS);
			ulong retVal;
			this.Read((byte*)&retVal, bits);
			return retVal;
		}

		unsafe public char ReadChar()
		{
			char retVal;
			this.Read((byte*)&retVal, WORD_BITS);
			return retVal;
		}
		unsafe public char ReadChar(int bits)
		{
			this.CheckLength(bits, WORD_BITS);
			char retVal;
			this.Read((byte*)&retVal, bits);
			return retVal;
		}

		unsafe public byte[] ReadBytes(int bytes)
		{
			byte[] retVal = new byte[bytes];
			fixed (byte* dest = retVal)
			{
				this.Read(dest, BYTE_BITS * bytes);
			}
			return retVal;
		}
		unsafe public byte[] ReadBytes(int bytes, int byteBits)
		{
			this.CheckLength(byteBits, BYTE_BITS);
			byte[] retVal = new byte[bytes];
			fixed (byte* dest = retVal)
			{
				for (int i = 0; i < bytes; i++)
					this.Read(&dest[i], byteBits);
			}
			return retVal;
		}

		unsafe public char[] ReadChars(int chars)
		{
			char[] retVal = new char[chars];
			fixed (char* dest = retVal)
			{
				for (int i = 0; i < chars; i++)
					this.Read((byte*)&dest[i], BYTE_BITS);
			}
			return retVal;
		}
		unsafe public char[] ReadChars(int chars, int charBits)
		{
			this.CheckLength(charBits, WORD_BITS);
			char[] retVal = new char[chars];
			fixed (char* dest = retVal)
			{
				for (int i = 0; i < chars; i++)
					this.Read((byte*)&dest[i], charBits);
			}
			return retVal;
		}

		/// <summary>
		/// Reads the specified number of characters.
		/// </summary>
		/// <param name="chars">Number of chars to read.</param>
		/// <param name="charBits">The number of bits to read per character (max 16.)</param>
		/// <returns>The characters read as a string.</returns>
		unsafe public string ReadString(int chars, int charBits)
		{
			return new String(this.ReadChars(chars, charBits));
		}

		/// <summary>
		/// Reads characters until the specified terminator character or maximum length is reached.
		/// </summary>
		/// <param name="charBits">The number of bits to read per character (max 16.)</param>
		/// <param name="terminator">The character marking the end of the string.</param>
		/// <param name="maxLength">The maximum number of characters to read (also the buffer size...)</param>
		unsafe public string ReadString(int charBits, char terminator, int maxLength)
		{
			this.CheckLength(charBits, WORD_BITS);
			char[] retVal = new char[maxLength];
			int i = 0;
			fixed (char* dest = retVal)
			{
				for (; i < maxLength; i++)
				{
					this.Read((byte*)&dest[i], charBits);
					if (dest[i] == terminator)
						break;
				}
			}
			return new String(retVal, 0, i);
		}

		/// <summary>
		/// Reads 8 bit characters until a null character is reached.
		/// </summary>
		/// <returns>The characters read as a string, excluding the null character.</returns>
		unsafe public string ReadString()
		{
			return this.ReadString(BYTE_BITS, '\0');
		}
		/// <summary>
		/// Reads characters of the specified amount of bits until a null character is reached.
		/// </summary>
		/// <param name="charBits">The number of bits to read per character (max 16.)</param>
		/// <returns>The characters read as a string, excluding the terminator.</returns>
		unsafe public string ReadString(int charBits)
		{
			return this.ReadString(charBits, '\0');
		}
		/// <summary>
		/// Reads 8 bit characters until the specified terminator character is reached.
		/// </summary>
		/// <param name="terminator">The character marking the end of the string.</param>
		/// <returns>The characters read as a string, excluding the terminator.</returns>
		unsafe public string ReadString(char terminator)
		{
			return this.ReadString(BYTE_BITS, terminator);
		}
		/// <summary>
		/// Reads characters of the specified length until the specified terminator character is reached.
		/// </summary>
		/// <param name="charBits">The number of bits to read per character (max 16.)</param>
		/// <param name="terminator">The character marking the end of the string.</param>
		/// <returns>The characters read as a string, excluding the terminator.</returns>
		unsafe public string ReadString(int charBits, char terminator)
		{
			this.CheckLength(charBits, WORD_BITS);
			List<char> retVal = new List<char>();
			char dest;
			while (true)
			{
				this.Read((byte*)&dest, charBits);
				if (dest == terminator)
					break;
				retVal.Add(dest);
			}
			return new String(retVal.ToArray());
		}

	}
}
