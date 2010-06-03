using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

// Utils for the silverlight app
namespace Utils
{
	public class StringUtils
	{
		public static object ConvertFromString(string value, Type t)
		{
			if (value.Length == 0)
			{
				if (t.IsValueType)
				{
					return Activator.CreateInstance(t);
				}
				else
				{
					if (t == typeof(string))
					{
						return "";
					}

					return null;
				}
			}

			if (t == typeof(bool) && value.Length == 1)
			{
				return value == "1";
			}

			return Convert.ChangeType(value, t, null);
		}
	}

	public class ResourceUtils
	{
		public static Stream OpenResource(string assemblyName, string resourcePath)
		{
			return File.Open(resourcePath, FileMode.Open);
		}

		public static StreamReader OpenResourceText(string assemblyName, string resourcePath)
		{
			return new StreamReader(OpenResource(assemblyName, resourcePath));
		}

		public static BinaryReader OpenResourceBinary(string assemblyName, string resourcePath)
		{
			return new BinaryReader(OpenResource(assemblyName, resourcePath));
		}

		public static List<string> ReadAllLines(string assemblyName, string resourcePath)
		{
			List<string> lines = new List<string>();

			using (StreamReader sr = OpenResourceText(assemblyName, resourcePath))
			{
				while (!sr.EndOfStream)
				{
					lines.Add(sr.ReadLine());
				}
			}

			return lines;
		}

		public static string ReadToEnd(string assemblyName, string resourcePath)
		{
			using (StreamReader sr = OpenResourceText(assemblyName, resourcePath))
			{
				return sr.ReadToEnd();
			}
		}

		public static byte[] ReadAllBytes(string assemblyName, string resourcePath)
		{
			using (BinaryReader br = OpenResourceBinary(assemblyName, resourcePath))
			{
				return br.ReadBytes((int)br.BaseStream.Length);
			}
		}
	}
}

// Very temporary until the silverlight frontend stops using Auto generated fields
namespace CharacterEditor
{
	class DisplayAttribute : Attribute
	{
		public bool AutoGenerateField { get; set; }
	}
}
