using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace esCharView
{
	static public class Utils
	{
		/// <summary>
		/// Reverse bits of a value up to 32 bits
		/// </summary>
		/// <param name="val">Value to reverse</param>
		/// <param name="bitCount">Number of bits in val</param>
		/// <returns>Reversed value in 32 bits</returns>
		/// <see cref="http://www.cs.utk.edu/~vose/c-stuff/bithacks.html#IntegerLog"/>
		public static uint ReverseBits(uint val, int bitCount)
		{
			uint v = val; // 32-bit word to reverse bit order

			// swap odd and even bits
			v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);
			// swap consecutive pairs
			v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
			// swap nibbles ... 
			v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
			// swap bytes
			v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);
			// swap 2-byte long pairs
			v = (v >> 16) | (v << 16);

			return (v >> (32 - bitCount));
		}

		/// <summary>
		/// Reverse the order of bits in a byte ( 10001101 -> 10110001 )
		/// </summary>
		/// <param name="val">Byte to reverse bits of</param>
		/// <returns>Byte with reversed bits</returns>
		/// <see cref="http://www.cs.utk.edu/~vose/c-stuff/bithacks.html#ReverseByteWith32Bits"/>
		public static byte ReverseByteBits(byte val)
		{
			return (byte)(((val * 0x0802LU & 0x22110LU) | (val * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);
		}

		/// <summary>
		/// Reverse the bits in each byte of the array
		/// </summary>
		/// <param name="bytes">Array of bytes to reverse</param>
		/// <returns>New array containing modified bytes</returns>
		public static byte[] ReverseByteArrayBits(byte[] bytes)
		{
			byte[] reversedBits = new byte[bytes.Length];

			for (int i = 0; i < bytes.Length; i++)
			{
				reversedBits[i] = ReverseByteBits(bytes[i]);
			}

			return reversedBits;
		}
	}
}
