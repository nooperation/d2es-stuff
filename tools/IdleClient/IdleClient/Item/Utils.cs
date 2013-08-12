using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

#if SILVERLIGHT
using System.Windows.Resources;
using System.Windows;
#endif

// Utils for the silverlight app
namespace CharacterEditor
{
	public class ResourceUtils
	{
		public static Stream OpenResource(string assemblyName, string resourcePath)
		{
#if SILVERLIGHT
			Uri resourceUri = new Uri(string.Format("{0};component/{1}", assemblyName, resourcePath), UriKind.Relative);
			StreamResourceInfo sri = Application.GetResourceStream(resourceUri);

			if (sri == null)
			{
				throw new FileNotFoundException("Failed to read file: " + resourceUri);
			}

			return sri.Stream;
#else
			return File.Open(resourcePath, FileMode.Open);
#endif
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