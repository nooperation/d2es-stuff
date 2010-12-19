using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace IdleClient.ConsoleFrontend
{
	class Program
	{
		Driver driver = new Driver();

		static void Main(string[] args)
		{
			new Program();
		}

		public Program()
		{
			driver.Initalize();
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
