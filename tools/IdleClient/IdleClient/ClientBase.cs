using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;

namespace IdleClient
{
	class ClientBase
	{
		/// <summary>
		/// Client has been marked as disconnecting. Errors from loss of connection should be silenced
		/// until main loop dies.
		/// </summary>
		public bool IsDisconnecting { get; protected set; }

		/// <summary>
		/// Client has failed and requested to disconnect. Failure event will be rasied after main loop ends
		/// instead of disconnect event.
		/// </summary>
		public bool HasFailed { get; protected set; }

		/// <summary> 
		/// Raised when the client disconnects. 
		/// </summary>
		public event EventHandler OnDisconnect;

		/// <summary> 
		/// Raised when a failure occurs. This is most likely nonrecoverable. 
		/// </summary>
		public event EventHandler<FailureArgs> OnFailure;

		/// <summary>
		/// Name of the character we want to log in as
		/// </summary>
		public string characterName { get; protected set; }

		/// <summary>
		/// Name of the client.
		/// </summary>
		public string ClientName { get; protected set; }

		/// <summary> The failure arguments if a failure has occured</summary>
		protected FailureArgs failureArgs;
		protected TcpClient client = new TcpClient();
		protected Config settings;
		protected string address;
		protected int port;


		/// <summary>
		/// Creates a new client.
		/// </summary>
		/// <param name="settings">Options for controlling the operation.</param>
		/// <param name="characterName">Name of the character.</param>
		public ClientBase(Config settings, string characterName)
		{
			this.settings = settings;
			this.characterName = characterName;
			ClientName = "NONE";
		}

		/// <summary>
		/// Initialises this object. Make sure to assign server address and port here or in the
		/// constructor. This must be overridden.
		/// </summary>
		/// <param name="args">The arguments to pass to this client.</param>
		protected virtual void Init(object args)
		{
			throw new NotImplementedException();
		}

		/// <summary>
		/// The main loop. This must be overridden. 
		/// </summary>
		protected virtual void MainLoop()
		{
			throw new NotImplementedException();
		}

		/// <summary>
		/// Entrypoint for this client thread
		/// </summary>
		/// <param name="args">The arguments to pass to this client.</param>
		public void Run(object args)
		{
			Init(args);

			LogServer("Connecting to " + settings.Address + ":" + settings.Port);

			try
			{
				client = new TcpClient(address, port);
			}
			catch (SocketException ex)
			{
				Fail(FailureArgs.FailureTypes.UnableToConnect, "Failed to connect to server: " + ex.Message);
				FireOnFailureEvent(failureArgs.Type, failureArgs.Message);
				return;
			}

			using (client)
			{
				MainLoop();
			}

			LogServer("Disconnected");
			if (HasFailed)
			{
				FireOnFailureEvent(failureArgs.Type, failureArgs.Message);
			}
			else
			{
				FireOnDisconnectEvent();
			}
		}

		/// <summary>
		/// Forcefully disconnects from the server.
		/// </summary>
		public void Disconnect()
		{
			if (client != null)
			{
				IsDisconnecting = true;

				if (client.Connected)
				{
					LogServer("Disconnect requested");
					client.Close();
				}
			}
		}

		/// <summary>
		/// Handles failure and disconnects.
		/// </summary>
		/// <param name="failureType">Type of the failure.</param>
		/// <param name="message">The error message.</param>
		internal void Fail(FailureArgs.FailureTypes failureType, string message)
		{
			LogError(message);

			if (HasFailed)
			{
				failureArgs.Message += ". " + message;
			}
			else
			{
				failureArgs = new FailureArgs(failureType, message);
			}

			HasFailed = true;
			IsDisconnecting = true;
			Disconnect();
		}

		/// <summary>
		/// Raises the on disconnect event. 
		/// </summary>
		private void FireOnDisconnectEvent()
		{
			EventHandler tempHandler = OnDisconnect;
			if (tempHandler != null)
			{
				tempHandler(this, new EventArgs());
			}
		}

		/// <summary>
		/// Raises the on failure event. 
		/// </summary>
		/// <param name="failureTypes">Type of failures.</param>
		/// <param name="message">The error message.</param>
		private void FireOnFailureEvent(FailureArgs.FailureTypes failureTypes, string message)
		{
			EventHandler<FailureArgs> tempHandler = OnFailure;
			if (tempHandler != null)
			{
				tempHandler(this, new FailureArgs(failureTypes, message));
			}
		}

		/// <summary>
		/// Logs a server message.
		/// </summary>
		/// <param name="message">The message.</param>
		protected void LogServer(object message)
		{
			Logger.Instance.LogServer(this, characterName + " -> " + message);
		}

		/// <summary>
		/// Logs a debug message. 
		/// </summary>
		/// <param name="message">The message.</param>
		protected void LogDebug(object message)
		{
			Logger.Instance.LogDebug(this, characterName + " -> " + message);
		}

		/// <summary>
		/// Logs a general message
		/// </summary>
		/// <param name="message">The message.</param>
		protected void Log(object message)
		{
			Logger.Instance.LogGame(this, characterName + " -> " + message);
		}

		/// <summary>
		/// Logs an error message. 
		/// </summary>
		/// <param name="message">The error message.</param>
		protected void LogError(object message)
		{
			Logger.Instance.LogError(this, characterName + " -> " + message);
		}

		public override string ToString()
		{
			return ClientName;
		}
	}
}
