using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

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
		/// Gets or sets the account username
		/// </summary>
		public string Username { get; set; }

		/// <summary>
		/// Gets or sets the account password
		/// </summary>
		public string password { get; set; }

		/// <summary>
		/// Gets or sets the character name to login as
		/// </summary>
		public string CharacterName { get; set; }

		/// <summary>
		/// Gets or sets the palyer name to accept commands from
		/// </summary>
		public string MasterName { get; set; }

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
		/// Creates a new Config with default options (Only for faster testing)
		/// </summary>
		public Config()
		{
			Address = "localhost";
			Port = 6112;
			Owner = "IdleClient";
			Username = "brahmin";
			password = "asdf123";
#if OLDVERSION
			CharacterName = "es";
			ClientVersion = 0x0a;
#else
			CharacterName = "test";
			ClientVersion = 0x0c;
#endif
			GameName = "foobarrr";
			GamePass = "";
			GameDifficulty = DifficultyType.Normal;
			GameDescription = GameDifficulty.ToString();

			MasterName = "aaa";
			ShowPackets = true;
			ShowPacketData = false;
		}

		/// <summary>
		/// Creates a new Config and reads settings from specific file
		/// </summary>
		/// <param name="configFile">The configuration file.</param>
		public Config(string configFile)
		{
			ReadConfig(configFile);
		}

		/// <summary>
		/// Reads a configuration file. 
		/// </summary>
		/// <param name="configFile">The configuration file.</param>
		public void ReadConfig(string configFile)
		{

		}
	}
}
