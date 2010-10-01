using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IdleClient
{
	/// <summary>
	/// Interface for packets that are sent to the server. 
	/// </summary>
	interface IOutPacket
	{
		byte[] GetBytes();
		byte Id { get; }
	}

	/// <summary>
	/// Values that represent game difficulties. 
	/// </summary>
	public enum DifficultyType
	{
		Normal = 0x0000,
		Nightmare = 0x1000,
		Hell = 0x2000,
	}

	/// <summary>
	/// Values that represent character classes. 
	/// </summary>
	public enum CharacterClassType
	{
		Amazon,
		Sorceress,
		Necromancer,
		Paladin,
		Barbarian,
		Druid,
		Assassin
	}

	/// <summary>
	/// Arguments for failure event. Occurs on unrecoverable errors.
	/// </summary>
	public class FailureArgs : EventArgs
	{
		public enum FailureTypes
		{
			// General
			UnableToConnect,
			FailedToSend,
			FailedToReceive,

			// Chat server
			LoginFailed,
			NoRealmsAvailable,
			RealmLoginFailed,
			FailedAuthCheck,

			// Realm server
			FailedToJoinGame,
			FailedToCreateGame,
			FailedToLoginToChat,
			CharacterNotFound,
			ServerDeniedConnection,

			// Game server
			UnknownPacketSize,
			BadCharacterData,
		}

		public FailureTypes Type { get; set; }
		public string Message { get; set; }

		public FailureArgs(FailureTypes type, string message)
		{
			this.Type = type;
			this.Message = message;
		}

		public override string ToString()
		{
			return Type + ": " + Message;
		}
	}


	/// <summary>
	/// Arguments for player count event. Contains the updated player count.
	/// </summary>
	public class PlayerCountArgs : EventArgs
	{
		public int PlayerCount { get; set; }

		public PlayerCountArgs(int count)
		{
			PlayerCount = count;
		}

		public override string ToString()
		{
			return PlayerCount.ToString();
		}
	}
}
