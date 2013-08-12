using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;

namespace CharacterEditor
{
	public class ItemStatCost
	{
		public string Stat { get; set; }
		public int ID { get; set; }
		public bool SendOther { get; set; }
		public bool Signed { get; set; }
		public int SendBits { get; set; }
		public int SendParamBits { get; set; }
		public bool UpdateAnimRate { get; set; }
		public bool Saved { get; set; }
		public bool CSvSigned { get; set; }
		public int CSvBits { get; set; }
		public int CSvParam { get; set; }
		public bool FCallback { get; set; }
		public bool FMin { get; set; }
		public int MinAccr { get; set; }
		public int Encode { get; set; }
		public int Add { get; set; }
		public int Multiply { get; set; }
		public int Divide { get; set; }
		public int ValShift { get; set; }
		public int OldSaveBits { get; set; }
		public int OldSaveAdd { get; set; }
		public int SaveBits { get; set; }
		public int SaveAdd { get; set; }
		public int SaveParamBits { get; set; }
		public int Keepzero { get; set; }
		public int Op { get; set; }
		public int OpParam { get; set; }
		public string OpBase { get; set; }
		public string OpStat1 { get; set; }
		public string OpStat2 { get; set; }
		public string OpStat3 { get; set; }
		public bool Direct { get; set; }
		public string Maxstat { get; set; }
		public bool Itemspecific { get; set; }
		public bool Damagerelated { get; set; }
		public string Itemevent1 { get; set; }
		public int Itemeventfunc1 { get; set; }
		public string Itemevent2 { get; set; }
		public int Itemeventfunc2 { get; set; }
		public int Descpriority { get; set; }
		public int Descfunc { get; set; }
		public int Descval { get; set; }
		public string Descstrpos { get; set; }
		public string Descstrneg { get; set; }
		public string Descstr2 { get; set; }
		public int Dgrp { get; set; }
		public int Dgrpfunc { get; set; }
		public int Dgrpval { get; set; }
		public string Dgrpstrpos { get; set; }
		public string Dgrpstrneg { get; set; }
		public string Dgrpstr2 { get; set; }
		public int Stuff { get; set; }
		public bool Eol { get; set; }

		public ItemStatCost()
		{

		}

		private bool ToBoolean(string data)
		{
			if (string.IsNullOrWhiteSpace(data))
				return false;

			return data.Trim() == "1";
		}

		private int ToInt(string data)
		{
			if (string.IsNullOrWhiteSpace(data))
				return 0;

			return int.Parse(data);
		}

		public ItemStatCost(string[] data)
		{
			int i = 0;

			// TODO: Horrible cut+paste+replace mess
			Stat = data[i++];
			ID = ToInt(data[i++]);
			SendOther = ToBoolean(data[i++]);
			Signed = ToBoolean(data[i++]);
			SendBits = ToInt(data[i++]);
			SendParamBits = ToInt(data[i++]);
			UpdateAnimRate = ToBoolean(data[i++]);
			Saved = ToBoolean(data[i++]);
			CSvSigned = ToBoolean(data[i++]);
			CSvBits = ToInt(data[i++]);
			CSvParam = ToInt(data[i++]);
			FCallback = ToBoolean(data[i++]);
			FMin = ToBoolean(data[i++]);
			MinAccr = ToInt(data[i++]);
			Encode = ToInt(data[i++]);
			Add = ToInt(data[i++]);
			Multiply = ToInt(data[i++]);
			Divide = ToInt(data[i++]);
			ValShift = ToInt(data[i++]);
			OldSaveBits = ToInt(data[i++]);
			OldSaveAdd = ToInt(data[i++]);
			SaveBits = ToInt(data[i++]);
			SaveAdd = ToInt(data[i++]);
			SaveParamBits = ToInt(data[i++]);
			Keepzero = ToInt(data[i++]);
			Op = ToInt(data[i++]);
			OpParam = ToInt(data[i++]);
			OpBase = data[i++];
			OpStat1 = data[i++];
			OpStat2 = data[i++];
			OpStat3 = data[i++];
			Direct = ToBoolean(data[i++]);
			Maxstat = data[i++];
			Itemspecific = ToBoolean(data[i++]);
			Damagerelated = ToBoolean(data[i++]);
			Itemevent1 = data[i++];
			Itemeventfunc1 = ToInt(data[i++]);
			Itemevent2 = data[i++];
			Itemeventfunc2 = ToInt(data[i++]);
			Descpriority = ToInt(data[i++]);
			Descfunc = ToInt(data[i++]);
			Descval = ToInt(data[i++]);
			Descstrpos = data[i++];
			Descstrneg = data[i++];
			Descstr2 = data[i++];
			Dgrp = ToInt(data[i++]);
			Dgrpfunc = ToInt(data[i++]);
			Dgrpval = ToInt(data[i++]);
			Dgrpstrpos = data[i++];
			Dgrpstrneg = data[i++];
			Dgrpstr2 = data[i++];
			Stuff = ToInt(data[i++]);
			Eol = ToBoolean(data[i++]);
		}

		public static List<ItemStatCost> Read(StreamReader stream)
		{
			List<ItemStatCost> statCosts = new List<ItemStatCost>();
			PropertyInfo[] properties = typeof(ItemStatCost).GetProperties();

			using (stream)
			{
				stream.ReadLine();

				while (!stream.EndOfStream)
				{
					string currentLine = stream.ReadLine();
					string[] splitLine = currentLine.Split('\t');


					statCosts.Add(new ItemStatCost(splitLine));
				}
			}

			return statCosts;
		}

		public override string ToString()
		{
			return string.Format("{0} - {1}", ID, Stat);
		}
	}
}
