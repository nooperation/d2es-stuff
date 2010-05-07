using System;
using System.Collections.Generic;
using System.Text;

namespace esCharView
{
	public class Item
	{
		private List<Item> sockets = new List<Item>();

		public string ItemCode { get; set; }
		public byte[] ItemData { get; set; }
		public int Location { get; set; }
		public bool Socketed { get; set; }
		public int SocketCount { get; set; }

		public List<Item> Sockets
		{
			get { return sockets; }
		}

		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();

			sb.Append(String.Format("{0} {1}", ItemCode, ItemDefs.GetItemDescription(ItemCode)));

			if (sockets.Count > 0)
			{
				sb.Append(" { ");
				for (int i = 0; i < sockets.Count; i++)
				{
					sb.Append(ItemDefs.GetItemDescription(sockets[i].ItemCode));

					if (i < sockets.Count - 1)
					{
						sb.Append(", ");
					}
				}
				sb.Append(" }");
			}

			return sb.ToString();
		}
	}
}
