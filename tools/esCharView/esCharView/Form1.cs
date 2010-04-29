using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;

namespace esCharView
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
			ItemDefs.GetItemDescription("dongs");
		}

		private void button1_Click(object sender, EventArgs e)
		{		
			if (openFileDialog2.ShowDialog() == DialogResult.OK)
			{
				this.Text = "D2ES Save Fix - " + Path.GetFileName(openFileDialog2.FileName);
				ProcessCharacter(openFileDialog2.FileName);
				buttonSave.Enabled = true;
			}
		}

		Character playerData = null;

		private void ProcessCharacter(string filePath)
		{
			playerData = new Character();

			playerData.Read(filePath);

			listBoxInventory.Items.Clear();

			UpdateInventoryList();

			checkBoxDied.Checked = playerData.Died;
			checkBoxExpansion.Checked = playerData.Expansion;
			checkBoxHardcore.Checked = playerData.Hardcore;
			textBoxUnknownFlags.Text = playerData.UnknownFlags.ToString();
			textBoxName.Text = playerData.Name;
		}

		private void UpdateInventoryList()
		{
			int selected = listBoxInventory.SelectedIndex;

			listBoxInventory.Items.Clear();
			textBoxItemLocation.Clear();

			foreach (Item item in playerData.Inventory.Items)
			{
				listBoxInventory.Items.Add(String.Format("{0:X}: {1}", item.Location + playerData.HeaderSize, item.ToString()));
			}

			foreach (Item item in playerData.Inventory.CorpseItems)
			{
				listBoxInventory.Items.Add(String.Format("{0:X}: (Corpse) {1}", item.Location + playerData.HeaderSize, item.ToString()));
			}

			if (selected > 0)
			{
				listBoxInventory.SelectedIndex = selected - 1;
			}
		}

		private void listBoxInventory_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (listBoxInventory.SelectedIndices.Count > 0)
			{
				int lastSelectedIndex = listBoxInventory.SelectedIndices[listBoxInventory.SelectedIndices.Count - 1];
				string selectedItemText = listBoxInventory.Items[lastSelectedIndex].ToString();
				textBoxItemLocation.Text = selectedItemText.Substring(0, selectedItemText.IndexOf(':'));
			}
		}

		private void buttonRemoveItem_Click(object sender, EventArgs e)
		{
			if (listBoxInventory.SelectedIndices.Count > 0)
			{
				int itemsCount = playerData.Inventory.Items.Count;
				for (int i = listBoxInventory.SelectedIndices.Count-1; i >= 0; i--)
				{
					if (listBoxInventory.SelectedIndices[i] >= itemsCount)
					{
						playerData.Inventory.CorpseItems.RemoveAt(listBoxInventory.SelectedIndices[i] - itemsCount);
					}
					else
					{
						playerData.Inventory.Items.RemoveAt(listBoxInventory.SelectedIndices[i]);
					}
				}
	
				playerData.UpdateInventoryHeaders();
				UpdateInventoryList();
			}
		}

		private void buttonSave_Click(object sender, EventArgs e)
		{
			byte unknownFlags = 0;

			try
			{
				unknownFlags = byte.Parse(textBoxUnknownFlags.Text);
			}
			catch (Exception)
			{
				unknownFlags = 0;
			}

			textBoxUnknownFlags.Text = unknownFlags.ToString();

			playerData.Name = textBoxName.Text;
			playerData.SetCharacterFlags(checkBoxExpansion.Checked, checkBoxDied.Checked, checkBoxHardcore.Checked, unknownFlags); 
			saveFileDialog1.FileName = playerData.Name + ".d2s";

			if (saveFileDialog1.ShowDialog() == DialogResult.OK)
			{
				playerData.Write(saveFileDialog1.FileName);
			}
		}

		private void textBoxUnknownFlags_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (!char.IsNumber(e.KeyChar) && !char.IsControl(e.KeyChar))
			{
				e.Handled = true;
			}
		}
	}
}
