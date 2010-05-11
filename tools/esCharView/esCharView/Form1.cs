using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;

namespace esCharView
{
	// This whole form is temporary and poorly thrown together
	public partial class Form1 : Form
	{
		private SaveReader playerData = null;

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
			playerData = new SaveReader();

			playerData.Read(filePath);

			listBoxInventory.Items.Clear();
			listBoxCorpseInventory.Items.Clear();
			listBoxGolemInventory.Items.Clear();
			listBoxMercInventory.Items.Clear();
			textBoxGeneral.Clear();

			UpdateInventoryList();

			checkBoxDied.Checked = playerData.Character.Died;
			checkBoxExpansion.Checked = playerData.Character.Expansion;
			checkBoxHardcore.Checked = playerData.Character.Hardcore;
			textBoxUnknownFlags.Text = playerData.Character.UnknownFlags.ToString();
			textBoxName.Text = playerData.Character.Name;

			StringBuilder sb = new StringBuilder();

			sb.AppendLine(string.Format("Class: {0}", playerData.Character.Class));
			foreach (var item in playerData.Stat)
			{
				sb.AppendLine(string.Format("{0}: {1}", item.Key, item.Value));
			}

			sb.AppendLine("Skills: ");

			foreach (var item in playerData.Skill)
			{
				sb.Append(string.Format("{0} ", item));
			}
			
			textBoxGeneral.Text = sb.ToString();
		}

		private void UpdateInventoryList()
		{
			int selected = listBoxInventory.SelectedIndex;

			listBoxInventory.Items.Clear();
			listBoxCorpseInventory.Items.Clear();
			listBoxMercInventory.Items.Clear();
			listBoxGolemInventory.Items.Clear();
			ClearItemEditor();

			foreach (Item item in playerData.Inventory.PlayerItems)
			{
				listBoxInventory.Items.Add(item.ToString());
			}

			foreach (Item item in playerData.Inventory.CorpseItems)
			{
				listBoxCorpseInventory.Items.Add(item.ToString());
			}

			foreach (Item item in playerData.Inventory.MercItems)
			{
				listBoxMercInventory.Items.Add(item.ToString());
			}

			foreach (Item item in playerData.Inventory.GolemItems)
			{
				listBoxGolemInventory.Items.Add(item.ToString());
			}

			if (selected > 0)
			{
				listBoxInventory.SelectedIndex = selected - 1;
			}
		}

		private void removeSelectedItems(ListBox listBox, List<Item> items)
		{
			if (listBox.SelectedIndices.Count > 0)
			{
				for (int i = listBox.SelectedIndices.Count - 1; i >= 0; i--)
				{
					if (listBox == listBoxItemEditorSockets)
					{
						itemToEdit.RemoveSocketedItem(listBox.SelectedIndices[i]);
					}
					else
					{
						items.RemoveAt(listBox.SelectedIndices[i]);
					}
					listBox.Items.RemoveAt(i);
				}
			}

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

			playerData.Character.Name = textBoxName.Text;
			playerData.Character.SetCharacterFlags(checkBoxExpansion.Checked, checkBoxDied.Checked, checkBoxHardcore.Checked, unknownFlags);
			saveFileDialog1.FileName = playerData.Character.Name + ".d2s";

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

		private void listBoxInventory_DoubleClick(object sender, EventArgs e)
		{
			OpenItemProperties();
		}

		private void OpenItemProperties()
		{
			if (tabControl1.SelectedTab == tabPageItemEditor)
			{
				if (listBoxItemEditorSockets.SelectedIndex == -1)
				{
					return;
				}

				OpenItemProperties(itemToEdit.Sockets[listBoxItemEditorSockets.SelectedIndex]);
			}
			else
			{
				if (tabControlInventory.SelectedTab == tabPageInventoryPlayer)
				{
					if (listBoxInventory.SelectedIndex == -1)
					{
						return;
					}

					OpenItemProperties(playerData.Inventory.PlayerItems[listBoxInventory.SelectedIndex]);
				}
				else if (tabControlInventory.SelectedTab == tabPageInventoryCorpse)
				{
					if (listBoxCorpseInventory.SelectedIndex == -1)
					{
						return;
					}

					OpenItemProperties(playerData.Inventory.CorpseItems[listBoxCorpseInventory.SelectedIndex]);
				}
				else if (tabControlInventory.SelectedTab == tabPageInventoryMerc)
				{
					if (listBoxMercInventory.SelectedIndex == -1)
					{
						return;
					}

					OpenItemProperties(playerData.Inventory.MercItems[listBoxMercInventory.SelectedIndex]);
				}
				else if (tabControlInventory.SelectedTab == tabPageInventoryGolem)
				{
					if (listBoxGolemInventory.SelectedIndex == -1)
					{
						return;
					}

					OpenItemProperties(playerData.Inventory.GolemItems[listBoxGolemInventory.SelectedIndex]);
				}
			}
		}


		private Item itemToEdit;

		private void OpenItemProperties(Item itemToEdit)
		{
			ClearItemEditor();

			this.itemToEdit = itemToEdit;
			StringBuilder sb = new StringBuilder();

			labelItemName.Text = itemToEdit.ToString();

			sb.AppendLine(string.Format("IsEquipped: {0}", itemToEdit.IsEquipped));
			sb.AppendLine(string.Format("IsInSocket: {0}", itemToEdit.IsInSocket));
			sb.AppendLine(string.Format("IsIdentified: {0}", itemToEdit.IsIdentified));
			sb.AppendLine(string.Format("IsSwitchIn: {0}", itemToEdit.IsSwitchIn));
			sb.AppendLine(string.Format("IsSwitchOut: {0}", itemToEdit.IsSwitchOut));
			sb.AppendLine(string.Format("IsBroken: {0}", itemToEdit.IsBroken));
			sb.AppendLine(string.Format("IsSocketed: {0}", itemToEdit.IsSocketed));
			sb.AppendLine(string.Format("IsStoreItem: {0}", itemToEdit.IsStoreItem));
			sb.AppendLine(string.Format("IsEar: {0}", itemToEdit.IsEar));
			sb.AppendLine(string.Format("IsStarterItem: {0}", itemToEdit.IsStarterItem));
			sb.AppendLine(string.Format("IsSimpleItem: {0}", itemToEdit.IsSimpleItem));
			sb.AppendLine(string.Format("IsEthereal: {0}", itemToEdit.IsEthereal));
			sb.AppendLine(string.Format("IsPersonalized: {0}", itemToEdit.IsPersonalized));
			sb.AppendLine(string.Format("IsGamble: {0}", itemToEdit.IsGamble));
			sb.AppendLine(string.Format("IsRuneword: {0}", itemToEdit.IsRuneword));
			sb.AppendLine(string.Format("Location: {0}", itemToEdit.Location));
			sb.AppendLine(string.Format("PositionOnBody: {0}", itemToEdit.PositionOnBody));
			sb.AppendLine(string.Format("PositionX: {0}", itemToEdit.PositionX));
			sb.AppendLine(string.Format("PositionY: {0}", itemToEdit.PositionY));
			sb.AppendLine(string.Format("StorageId: {0}", itemToEdit.StorageId));
			sb.AppendLine(string.Format("GoldAmount: {0}", itemToEdit.GoldAmount));
			sb.AppendLine(string.Format("SocketsFilled: {0}", itemToEdit.SocketsFilled));
			sb.AppendLine(string.Format("Id: {0:X}", itemToEdit.Id));
			sb.AppendLine(string.Format("Level: {0}", itemToEdit.Level));
			sb.AppendLine(string.Format("Quality: {0}", itemToEdit.Quality));
			sb.AppendLine(string.Format("HasGraphic: {0}", itemToEdit.HasGraphic));
			sb.AppendLine(string.Format("Graphic: {0}", itemToEdit.Graphic));

			sb.AppendLine("Prefix: ");
			foreach (var item in itemToEdit.Prefix)
			{
				sb.AppendLine(string.Format("   {0}", item));
			}
			sb.AppendLine("Suffix: ");
			foreach (var item in itemToEdit.Suffix)
			{
				sb.AppendLine(string.Format("   {0}", item));
			}

			sb.AppendLine(string.Format("UniqueSetId: {0}", itemToEdit.UniqueSetId));
			sb.AppendLine(string.Format("Defense: {0}", itemToEdit.Defense));
			sb.AppendLine(string.Format("MaxDurability: {0}", itemToEdit.MaxDurability));
			sb.AppendLine(string.Format("Durability: {0}", itemToEdit.Durability));
			sb.AppendLine(string.Format("IsIndestructable: {0}", itemToEdit.IsIndestructable));
			sb.AppendLine(string.Format("SocketCount: {0}", itemToEdit.SocketCount));
			sb.AppendLine(string.Format("Quantity: {0}", itemToEdit.Quantity));
			sb.AppendLine(string.Format("RemainingBits: {0}", itemToEdit.RemainingBits));

			foreach (var socket in itemToEdit.Sockets)
			{
				listBoxItemEditorSockets.Items.Add(socket);
			}

			textBoxItemEditor.Text = sb.ToString();
			textBoxItemEditor.Select(0, 0);

			tabControl1.SelectTab(2);
		}

		private void ClearItemEditor()
		{
			itemToEdit = null;
			textBoxItemEditor.Clear();
			listBoxItemEditorSockets.Items.Clear();
		}

		private void buttonRemoveSocket_Click(object sender, EventArgs e)
		{
			if (listBoxItemEditorSockets.SelectedIndex < 0)
			{
				return;
			}

			if (itemToEdit == null)
			{
				return;
			}

			removeSelectedItems(listBoxItemEditorSockets, itemToEdit.Sockets);
		}
	}
}
