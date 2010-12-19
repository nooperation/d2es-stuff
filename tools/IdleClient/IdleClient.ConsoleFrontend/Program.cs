using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace IdleClient.ConsoleFrontend
{
	class Program
	{
		private readonly string settingsPath = "IdleClient.xml";
		Driver driver = new Driver();
		Config settings = new Config();

		static void Main(string[] args)
		{
			new Program();
		}

		public Program()
		{
			try
			{
				settings = Config.ReadConfig(settingsPath);
			}
			catch (Exception ex)
			{
				Output("Initalization failed: " + ex.Message);
				throw;
			}

			driver.Initalize(settings);
			driver.OnOutput = Output;
			driver.OnPlayerCountChange = null;

			driver.Start();			

			while (driver.IsRunning)
			{
				Thread.Sleep(10);
			}
		}

		private void Output(string message)
		{
			Console.WriteLine(message);
		}
	}
}
