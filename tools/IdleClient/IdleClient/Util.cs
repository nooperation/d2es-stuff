using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net.Sockets;

namespace IdleClient
{
	/// <summary>Utilities class.</summary>
	class Util
	{
		/// <summary>
		/// Gets a string representation of a byte array in hex format. 
		/// </summary>
		/// <param name="data">Data to convert.</param>
		/// <param name="offset">Where to begin conversion of data.</param>
		/// <param name="length">Number of bytes to convert.</param>
		/// <returns>Representation of a byte array in hex format ([0xde 0xad 0xbe 0xef] -> "DE AD BE EF").</returns>
		public static string GetStringOfBytes(byte[] data, int offset, int length)
		{
			StringBuilder sb = new StringBuilder();

			for (int i = offset; i < length && i < data.Length; i++)
			{
				sb.AppendFormat("{0:X2} ", data[i]);
			}

			return sb.ToString();
		}

		/// <summary>
		/// Removes the beginning bytes of a byte array.
		/// </summary>
		/// <param name="data">[in,out] The data to modify.</param>
		/// <param name="count">Number of elements to remove.</param>
		public static void RemoveBeginningBytes(ref byte[] data, int count)
		{
			Array.Copy(data, count, data, 0, data.Length - count);
			Array.Resize(ref data, data.Length - count);
		}

		/// <summary>
		/// Reads a special string from a BinaryReader. These strings are either terminated by a null
		/// terminator (\0) or a string representation of the null terminator ("\0")
		/// </summary>
		/// <param name="br">The BinaryReader to read the string from.</param>
		/// <returns>The string read from br.</returns>
		public static string ReadSpecialString(BinaryReader br)
		{
			StringBuilder sb = new StringBuilder();

			while (true)
			{
				char ch = br.ReadChar();
				if (ch == '\0' || (ch == '\\' && br.PeekChar() == '0'))
				{
					return sb.ToString();
				}
				else
				{
					sb.Append(ch);
				}
			}
		}

		/// <summary>
		/// Reads a string padded with null terminator from a stream and resizes it so only
		/// the string portion remains.
		/// </summary>
		/// <param name="br">The BinaryReader to read the string from.</param>
		/// <param name="byteCount">Total fixed size of string.</param>
		/// <returns>The unpadded string</returns>
		public static string ReadAndUnpadString(BinaryReader br, int byteCount)
		{
			byte[] temp = br.ReadBytes(byteCount);
			int paddingStart = temp.Length;

			for (int i = 0; i < temp.Length; i++)
			{
				if (temp[i] == 0)
				{
					paddingStart = i;
					break;
				}
			}

			return ASCIIEncoding.ASCII.GetString(temp, 0, paddingStart);
		}

		/// <summary>
		/// Writes a null terminated string to a binary stream. 
		/// </summary>
		/// <param name="bw">The binary stream.</param>
		/// <param name="str">The string to write.</param>
		public static void WriteString(BinaryWriter bw, string str)
		{
			bw.Write(ASCIIEncoding.ASCII.GetBytes(str));
			bw.Write((byte)0x00);
		}

		/// <summary>
		/// Receives data from a NetworkStream. Storage container will only contain bytes read from stream.
		/// </summary>
		/// <param name="ns">The stream to read from.</param>
		/// <param name="data">[in,out] The stream data.</param>
		public static void Receive(NetworkStream ns, ref byte[] data)
		{
			byte[] readBuff = new byte[4096];
			int previousDataLength = data.Length;


			int readSize = ns.Read(readBuff, 0, readBuff.Length);

			Array.Resize(ref data, data.Length + readSize);
			Array.Copy(readBuff, 0, data, previousDataLength, readSize);
		}
	}

}
