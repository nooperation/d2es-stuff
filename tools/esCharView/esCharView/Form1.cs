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
	// This whole form is temporary and poorly thrown together
	public partial class Form1 : Form
	{
		private Character playerData = null;

		public Form1()
		{
			InitializeComponent();
			ItemDefs.GetItemDescription("dongs");
		}

		private void LoadSave(string savePath)
		{
			if (!File.Exists(savePath))
			{
				return;
			}

			this.Text = "D2ES Save Fix - " + Path.GetFileName(savePath);
			ProcessCharacter(savePath);
			buttonSave.Enabled = true;
		}

		private void button1_Click(object sender, EventArgs e)
		{		
			if (openFileDialog2.ShowDialog() == DialogResult.OK)
			{
				LoadSave(openFileDialog2.FileName);
			}
		}

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
			listBoxCorpseInventory.Items.Clear();
			listBoxMercInventory.Items.Clear();
			listBoxGolemInventory.Items.Clear();
			textBoxItemLocation.Clear();

			foreach (Item item in playerData.Inventory.PlayerItems)
			{
				listBoxInventory.Items.Add(String.Format("{0:X}: {1}", item.Location + playerData.HeaderSize, item.ToString()));
			}

			foreach (Item item in playerData.Inventory.CorpseItems)
			{
				listBoxCorpseInventory.Items.Add(String.Format("{0:X}: {1}", item.Location + playerData.HeaderSize, item.ToString()));
			}

			foreach (Item item in playerData.Inventory.MercItems)
			{
				listBoxMercInventory.Items.Add(String.Format("{0:X}: {1}", item.Location + playerData.HeaderSize, item.ToString()));
			}

			foreach (Item item in playerData.Inventory.GolemItems)
			{
				listBoxGolemInventory.Items.Add(String.Format("{0:X}: {1}", item.Location + playerData.HeaderSize, item.ToString()));
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

		private void removeSelectedItems(ListBox listBox, List<Item> items)
		{
			if (listBox.SelectedIndices.Count > 0)
			{
				for (int i = listBox.SelectedIndices.Count - 1; i >= 0; i--)
				{
					items.RemoveAt(listBox.SelectedIndices[i]);
					listBox.Items.RemoveAt(i);
				}
			}

			playerData.UpdateInventoryHeaders();
			UpdateInventoryList();
		}

		private void removeSelectedItems()
		{
			if (tabControlInventory.SelectedTab == tabPageInventoryPlayer)
			{
				removeSelectedItems(listBoxInventory, playerData.Inventory.PlayerItems);
			}
			else if (tabControlInventory.SelectedTab == tabPageInventoryCorpse)
			{
				removeSelectedItems(listBoxCorpseInventory, playerData.Inventory.CorpseItems);
			}
			else if (tabControlInventory.SelectedTab == tabPageInventoryMerc)
			{
				removeSelectedItems(listBoxMercInventory, playerData.Inventory.MercItems);
			}
			else if (tabControlInventory.SelectedTab == tabPageInventoryGolem)
			{
				removeSelectedItems(listBoxGolemInventory, playerData.Inventory.GolemItems);
			}
		}

		private void buttonRemoveItem_Click(object sender, EventArgs e)
		{
			removeSelectedItems();
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
