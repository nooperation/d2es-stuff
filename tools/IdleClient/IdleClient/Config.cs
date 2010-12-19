using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Serialization;
using System.Xml;

namespace IdleClient
{
	[XmlRootAttribute]
	public class Config
	{
		/// <summary>
		/// Gets or sets the chat server address.
		/// </summary>
		public string Address { get; set; }

		/// <summary>
		/// Gets or sets the chat server port.
		/// </summary>
		public int Port { get; set; }

		/// <summary>
		/// Gets or sets the client version. 0x0A = 1.10, 0x0B+ = 1.11+
		/// </summary>
		public uint ClientVersion { get; set; }

		/// <summary>
		/// Gets or sets the CDKey Owner
		/// </summary>
		public string Owner { get; set; }

		/// <summary>
		/// Gets or sets the palyer name to accept commands from
		/// </summary>
		public string MasterName { get; set; }

		/// <summary>
		/// Gets or sets the account username
		/// </summary>
		public string Username { get; set; }

		/// <summary>
		/// Gets or sets the account password
		/// </summary>
		public string Password { get; set; }

		/// <summary>
		/// Gets or sets the game name
		/// </summary>
		public string GameName { get; set; }

		/// <summary>
		/// Gets or sets the game pass
		/// </summary>
		public string GamePass { get; set; }

		/// <summary>
		/// Gets or sets the game description. Difficulty name by default.
		/// </summary>
		public string GameDescription { get; set; }

		/// <summary>
		/// Gets or sets the game difficulty. Normal mode by default.
		/// </summary>
		public DifficultyType GameDifficulty { get; set; }

		/// <summary>
		/// Milliseconds to wait before starting connection
		/// </summary>
		public int JoinDelay { get; set; }

		/// <summary>
		/// Gets or sets a value indicating whether packets is shown.
		/// </summary>
		public bool ShowPackets { get; set; }

		/// <summary>
		/// Gets or sets a value indicating whether packet data is shown.!
		/// </summary>
		public bool ShowPacketData { get; set; }

		/// <summary>
		/// Gets or sets a list of the bot names.
		/// </summary>
		public List<string> BotNames { get; set; }

		// TODO: Temporary
		private object configLock = new object();

		/// <summary>
		/// Creates a new Config with default options (Only for faster testing)
		/// </summary>
		public Config()
		{
			Address = "localhost";
			Port = 6112;
			Username = "";
			Password = "";
			MasterName = "";
			JoinDelay = 3000;

#if OLDVERSION
			ClientVersion = 0x0a;
#else
			ClientVersion = 0x0c;
#endif
			GameName = "test";
			GamePass = "";

			Owner = "IdleClient";
			GameDifficulty = DifficultyType.Normal;
			GameDescription = GameDifficulty.ToString();
			ShowPackets = false;
			ShowPacketData = false;

			BotNames = new List<string>();
		}

		/// <summary>
		/// Writes configuration file
		/// </summary>
		/// <param name="configFile">The configuration file path</param>
		public void WriteConfig(string configFile)
		{
			lock (configLock)
			{
				XmlSerializer serializer = new XmlSerializer(this.GetType());

				using (FileStream outStream = new FileStream(configFile, FileMode.Create))
				{
					serializer.Serialize(outStream, this);
					outStream.Flush();
				}
			}
		}

		/// <summary>
		/// Reads a configuration file. 
		/// </summary>
		/// <param name="configFile">The configuration file path</param>
		public static Config ReadConfig(string configFile)
		{
			XmlSerializer serializer = new XmlSerializer(typeof(Config));

			try
			{
				using (FileStream inStream = new FileStream(configFile, FileMode.Open))
				{
					return serializer.Deserialize(inStream) as Config;
				}
			}
			catch (FileNotFoundException)
			{
				return new Config();
			}
		}
	}
}
