using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BatchSaveTester
{
	struct ItemPropData
	{
		public enum ComparisonTypes
		{
			LessThan,
			LessThanOrEqualTo,
			GreaterThan,
			GreatherThanOrEqualTo,
			EqualTo,
			NotEqualTo,
			AlwaysTrue,
			AlwaysFalse,
		}

		public string PropertyName;
		public int Value;
		public ComparisonTypes ComparisonType;

		public string ComparisonString
		{
			get
			{
				return ComparisonType.ToString();
			}
			set
			{
				ComparisonType = ParseComparisonString(value);
			}
		}

		private ComparisonTypes ParseComparisonString(string str)
		{
			str = str.Trim();

			if (str == "!=" || str == "<>")
			{
				return ComparisonTypes.NotEqualTo;
			}
			if (str == ">")
			{
				return ComparisonTypes.GreaterThan;
			}
			switch (str)
			{
				case "!=":
				case "<>":
				case "~=":
				case "/=":
					return ComparisonTypes.NotEqualTo;
				case ">":
					return ComparisonTypes.GreaterThan;
				case "<":
					return ComparisonTypes.LessThan;
				case ">=":
					return ComparisonTypes.GreatherThanOrEqualTo;
				case "<=":
					return ComparisonTypes.LessThanOrEqualTo;
				case "=":
				case "==":
					return ComparisonTypes.EqualTo;
				case "<=>":
					return ComparisonTypes.AlwaysTrue;
				case "><":
				case "!<=>":
				case "~<=>":
					return ComparisonTypes.AlwaysFalse;
				default:
					throw new Exception("ParseComparisonString: Failed to parse comparison string");
			}
		}

		public bool TestValue(int n)
		{
			switch (ComparisonType)
			{
				case ComparisonTypes.LessThan:
					return n < Value;
				case ComparisonTypes.LessThanOrEqualTo:
					return n <= Value;
				case ComparisonTypes.GreaterThan:
					return n > Value;
				case ComparisonTypes.GreatherThanOrEqualTo:
					return n >= Value;
				case ComparisonTypes.EqualTo:
					return n == Value;
				case ComparisonTypes.NotEqualTo:
					return n != Value;
				case ComparisonTypes.AlwaysTrue:
					return true;
				case ComparisonTypes.AlwaysFalse:
					return false;
				default:
					throw new Exception("TestValue: Invalid condition specified");
			}
		}

		public override string ToString()
		{
			return string.Format("{0} {1} {2}", PropertyName, ComparisonType.ToString(), Value);
		}
	}
}
