using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FileHelpers;

namespace CharacterEditor
{
	[DelimitedRecord("\t")]
	[IgnoreFirst()]
	class ItemStatCost
	{
		public string Stat;

		[FieldNullValue(0)]
		public int ID;

		[FieldNullValue(false)]
		public bool SendOther;

		[FieldNullValue(false)]
		public bool Signed;

		[FieldNullValue(0)]
		public int SendBits;

		[FieldNullValue(0)]
		public int SendParamBits;

		[FieldNullValue(false)]
		public bool UpdateAnimRate;

		[FieldNullValue(false)]
		public bool Saved;

		[FieldNullValue(false)]
		public bool CSvSigned;

		[FieldNullValue(0)]
		public int CSvBits;

		[FieldNullValue(0)]
		public int CSvParam;

		[FieldNullValue(false)]
		public bool FCallback;

		[FieldNullValue(false)]
		public bool FMin;

		[FieldNullValue(0)]
		public int MinAccr;

		[FieldNullValue(0)]
		public int Encode;

		[FieldNullValue(0)]
		public int Add;

		[FieldNullValue(0)]
		public int Multiply;

		[FieldNullValue(0)]
		public int Divide;

		[FieldNullValue(0)]
		public int ValShift;

		[FieldNullValue(0)]
		public int OldSaveBits;

		[FieldNullValue(0)]
		public int OldSaveAdd;

		[FieldNullValue(0)]
		public int SaveBits;

		[FieldNullValue(0)]
		public int SaveAdd;

		[FieldNullValue(0)]
		public int SaveParamBits;

		[FieldNullValue(0)]
		public int Keepzero;

		[FieldNullValue(0)]
		public int Op;

		[FieldNullValue(0)]
		public int OpParam;

		public string OpBase;

		public string OpStat1;

		public string OpStat2;

		public string OpStat3;

		[FieldNullValue(false)]
		public bool Direct;

		public string Maxstat;

		[FieldNullValue(false)]
		public bool Itemspecific;

		[FieldNullValue(false)]
		public bool Damagerelated;

		public string Itemevent1;

		[FieldNullValue(0)]
		public int Itemeventfunc1;

		public string Itemevent2;

		[FieldNullValue(0)]
		public int Itemeventfunc2;

		[FieldNullValue(0)]
		public int Descpriority;

		[FieldNullValue(0)]
		public int Descfunc;

		[FieldNullValue(0)]
		public int Descval;

		public string Descstrpos;

		public string Descstrneg;

		public string Descstr2;

		[FieldNullValue(0)]
		public int Dgrp;

		[FieldNullValue(0)]
		public int Dgrpfunc;

		[FieldNullValue(0)]
		public int Dgrpval;

		public string Dgrpstrpos;

		public string Dgrpstrneg;

		public string Dgrpstr2;

		[FieldNullValue(0)]
		public int Stuff;

		[FieldNullValue(false)]
		public bool Eol;


		public override string ToString()
		{
			return string.Format("[{0}] {1}", ID, Stat);
		}

		/// <summary>
		/// Only to suppress the warnings about unused variables. They're all assigned by FileHelpers class
		/// </summary>
		private void Reset()
		{
			Stat = null;
			ID = 0;
			SendOther = false;
			Signed = false;
			SendBits = 0;
			SendParamBits = 0;
			UpdateAnimRate = false;
			Saved = false;
			CSvSigned = false;
			CSvBits = 0;
			CSvParam = 0;
			FCallback = false;
			FMin = false;
			MinAccr = 0;
			Encode = 0;
			Add = 0;
			Multiply = 0;
			Divide = 0;
			ValShift = 0;
			OldSaveBits = 0;
			OldSaveAdd = 0;
			SaveBits = 0;
			SaveAdd = 0;
			SaveParamBits = 0;
			Keepzero = 0;
			Op = 0;
			OpParam = 0;
			OpBase = null;
			OpStat1 = null;
			OpStat2 = null;
			OpStat3 = null;
			Direct = false;
			Maxstat = null;
			Itemspecific = false;
			Damagerelated = false;
			Itemevent1 = null;
			Itemeventfunc1 = 0;
			Itemevent2 = null;
			Itemeventfunc2 = 0;
			Descpriority = 0;
			Descfunc = 0;
			Descval = 0;
			Descstrpos = null;
			Descstrneg = null;
			Descstr2 = null;
			Dgrp = 0;
			Dgrpfunc = 0;
			Dgrpval = 0;
			Dgrpstrpos = null;
			Dgrpstrneg = null;
			Dgrpstr2 = null;
			Stuff = 0;
			Eol = false;
		}
	}
}
