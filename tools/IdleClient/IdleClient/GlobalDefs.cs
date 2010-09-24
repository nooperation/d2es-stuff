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
}
