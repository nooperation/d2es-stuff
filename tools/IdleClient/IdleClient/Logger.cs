using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IdleClient
{
	public class Logger
	{

		/// <summary>
		/// Values that represent MessageType. 
		/// </summary>
		private enum MessageType
		{
			Driver,
			Server,
			Debug,
			Game,
			Error
		}

		/// <summary>
		/// Gets the logger instance.
		/// </summary>
		public static Logger Instance { get; protected set; }

		public Action<string, string> OnServerMessage;
		public Action<string, string> OnDebugMessage;
		public Action<string, string> OnGameMessage;
		public Action<string, string> OnErrorMessage;
		public Action<string, string> OnDriverMessage;

		List<string> serverMessageFilter = new List<string>();
		List<string> debugMessageFilter = new List<string>();
		List<string> gameMessageFilter = new List<string>();
		List<string> errorMessageFilter = new List<string>();
		List<string> driverMessageFilter = new List<string>();

		/// <summary>
		/// Gets the server message filter.
		/// </summary>
		public List<string> ServerMessageFilter { get { return serverMessageFilter; } }

		/// <summary>
		/// Gets the debug message filter.
		/// </summary>
		public List<string> DebugMessageFilter { get { return debugMessageFilter; } }

		/// <summary>
		/// Gets the game message filter.
		/// </summary>
		public List<string> GameMessageFilter { get { return gameMessageFilter; } }

		/// <summary>
		/// Gets the error message filter.
		/// </summary>
		public List<string> ErrorMessageFilter { get { return errorMessageFilter; } }

		/// <summary>
		/// Gets the driver message filter.
		/// </summary>
		public List<string> DriverMessageFilter { get { return driverMessageFilter; } }

		static Logger()
		{
			Instance = new Logger();
		}

		private Logger()
		{

		}

		/// <summary>
		/// Logs a server message. 
		/// </summary>
		/// <param name="source">Source for the message.</param>
		/// <param name="message">The message.</param>
		public void LogServer(object source, object message)
		{
			Log(MessageType.Server, source, message);
		}

		/// <summary>
		/// Logs a game message. 
		/// </summary>
		/// <param name="source">Source for the message.</param>
		/// <param name="message">The message.</param>
		public void LogGame(object source, object message)
		{
			Log(MessageType.Game, source, message);
		}

		/// <summary>
		/// Logs a debug message. 
		/// </summary>
		/// <param name="source">Source for the message.</param>
		/// <param name="message">The message.</param>
		public void LogDebug(object source, object message)
		{
			Log(MessageType.Debug, source, message);
		}

		/// <summary>
		/// Logs an error message. 
		/// </summary>
		/// <param name="source">Source for the message.</param>
		/// <param name="message">The message.</param>
		public void LogError(object source, object message)
		{
			Log(MessageType.Error, source, message);
		}

		/// <summary>
		/// Logs a driver message. 
		/// </summary>
		/// <param name="source">Source for the message.</param>
		/// <param name="message">The message.</param>
		public void LogDriver(object source, object message)
		{
			Log(MessageType.Driver, source, message);
		}

		/// <summary>
		/// Logs a message. 
		/// </summary>
		/// <exception cref="ApplicationException">Thrown on invalid message type.</exception>
		/// <param name="type">The type of message.</param>
		/// <param name="source">Source for the message.</param>
		/// <param name="message">The message.</param>
		private void Log(MessageType type, object source, object message)
		{
			// Filter out messages that need to be filtered
			if (source is ClientBase)
			{
				ClientBase clientBase = source as ClientBase;

				switch (type)
				{
					case MessageType.Driver:
						if (IsFiltered(DriverMessageFilter, clientBase.characterName))
						{
							return;
						}
						break;
					case MessageType.Server:
						if (IsFiltered(ServerMessageFilter, clientBase.characterName))
						{
							return;
						}
						break;
					case MessageType.Debug:
						if (IsFiltered(DebugMessageFilter, clientBase.characterName))
						{
							return;
						}
						break;
					case MessageType.Game:
						if (IsFiltered(GameMessageFilter, clientBase.characterName))
						{
							return;
						}
						break;
					case MessageType.Error:
						if (IsFiltered(ErrorMessageFilter, clientBase.characterName))
						{
							return;
						}
						break;
				}
			}

			Action<string, string> tempHandler;

			switch (type)
			{
				case MessageType.Server:
					tempHandler = OnServerMessage;
					break;
				case MessageType.Debug:
					tempHandler = OnDebugMessage;
					break;
				case MessageType.Game:
					tempHandler = OnGameMessage;
					break;
				case MessageType.Error:
					tempHandler = OnErrorMessage;
					break;
				case MessageType.Driver:
					tempHandler = OnDriverMessage;
					break;
				default:
					throw new ApplicationException("Log(): Invalid MessageType specified");
			}

			if (tempHandler != null)
			{
				tempHandler.BeginInvoke(source.ToString(), message.ToString(), null, null);
			}
		}

		/// <summary>
		/// Check if 'characterName' should be filtered. 
		/// </summary>
		/// <param name="filteredNames">List of names that should not be filtered.</param>
		/// <param name="characterName">Name of the character.</param>
		/// <returns>true if filtered, false if not.</returns>
		private bool IsFiltered(List<string> filteredNames, string characterName)
		{
			if (filteredNames.Count > 0)
			{
				if (!filteredNames.Contains(characterName))
				{
					return true;
				}
			}

			return false;
		}

	}
}
