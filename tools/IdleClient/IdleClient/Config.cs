using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace IdleClient
{
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


		/// <summary>
		/// Creates a new Config with default options (Only for faster testing)
		/// </summary>
		public Config()
		{
			Address = "localhost";
			Port = 6112;
			Owner = "IdleClient";
			Username = "brahmin";
			Password = "asdf123";

#if OLDVERSION
			ClientVersion = 0x0a;
#else
			ClientVersion = 0x0c;
#endif
			GameName = "test";
			GamePass = "";
			GameDifficulty = DifficultyType.Normal;
			GameDescription = GameDifficulty.ToString();

			MasterName = "aaa";
			ShowPackets = false;
			ShowPacketData = false;
		}

		/// <summary>
		/// Creates a new Config and reads settings from specific file
		/// </summary>
		/// <param name="configFile">The configuration file.</param>
		public Config(string configFile)
		{
			Address = "localhost";
			Port = 6112;
			Owner = "IdleClient";

#if OLDVERSION
			ClientVersion = 0x0a;
#else
			ClientVersion = 0x0c;
#endif
			GameName = "foobarrr";
			GamePass = "";
			GameDifficulty = DifficultyType.Normal;
			GameDescription = GameDifficulty.ToString();

			MasterName = "aaa";
			ShowPackets = false;
			ShowPacketData = false;

			ReadConfig(configFile);
		}

		/// <summary>
		/// Reads a configuration file. 
		/// </summary>
		/// <param name="configFile">The configuration file.</param>
		public void ReadConfig(string configFile)
		{
			string[] lines = File.ReadAllLines(configFile);

			BotNames = new List<string>();

			foreach (var line in lines)
			{
				string[] parts = line.Split(new char[] { '=' });
				parts[0] = parts[0].Trim().ToLower();
				parts[1] = parts[1].Trim();

				switch (parts[0])
				{
					case "server":
						Address = parts[1];
						break;
					case "port":
						Port = int.Parse(parts[1]);
						break;
					case "showpackets":
						ShowPackets = bool.Parse(parts[1]);
						break;
					case "showpacketData":
						ShowPacketData = bool.Parse(parts[1]);
						break;
					case "gamedifficulty":
						GameDifficulty = (DifficultyType)Enum.Parse(typeof(DifficultyType), parts[1], true);
						break;
					case "account":
						Username = parts[1];
						break;
					case "password":
						Password = parts[1];
						break;
					case "master":
						MasterName = parts[1].ToLower();
						break;
					case "bot":
						if (!BotNames.Contains(parts[1].ToLower()))
						{
							BotNames.Add(parts[1]);
						}
						break;
				}
			}
		}
	}
}
