using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;
using System.Reflection;

using CharacterEditor;

namespace esCharView
{
	// This whole form is temporary and poorly thrown together
	public partial class Form1 : Form
	{
		private SaveReader playerData = null;

		public Form1()
		{
			InitializeComponent();
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

			if (playerData.FailedCharacterDecoding)
			{
				MessageBox.Show("Failed character decoding");
			}
			if (playerData.FailedInventoryDecoding)
			{
				MessageBox.Show("Failed inventory decoding");
			}
			if (playerData.FailedSkillDecoding)
			{
				MessageBox.Show("Failed skill decoding");
			}

			if (playerData.Inventory.FailedItemCount > 0)
			{
				MessageBox.Show(string.Format("Failed to read {0} items. These items will not be included when saving character", playerData.Inventory.FailedItemCount));
			}

			listBoxInventory.Items.Clear();
			listBoxCorpseInventory.Items.Clear();
			listBoxGolemInventory.Items.Clear();
			listBoxMercInventory.Items.Clear();

			UpdateInventoryList();

			textBoxUnknownFlags.DataBindings.Clear();
			textBoxName.DataBindings.Clear();
			characterBindingSource.Clear();
			statBindingSource.Clear();
			itemBindingSource.Clear();
			dataGridViewItemProperties.DataSource = null;
			dataGridViewItemRunewordProperties.DataSource = null;
			dataGridViewItemSetProperties.DataSource = null;

			textBoxUnknownFlags.DataBindings.Add("Text", playerData.Character, "UnknownFlags");
			textBoxName.DataBindings.Add("Text", playerData.Character, "Name");

			characterBindingSource.Add(playerData.Character);
			statBindingSource.Add(playerData.Stat);
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
				listBoxInventory.Items.Add(item);
			}

			foreach (Item item in playerData.Inventory.CorpseItems)
			{
				listBoxCorpseInventory.Items.Add(item);
			}

			foreach (Item item in playerData.Inventory.MercItems)
			{
				listBoxMercInventory.Items.Add(item);
			}

			foreach (Item item in playerData.Inventory.GolemItems)
			{
				listBoxGolemInventory.Items.Add(item);
			}

			if (selected > 0)
			{
				listBoxInventory.SelectedIndex = selected - 1;
			}
		}

		private void removeSelectedItems(ListBox listBox, List<Item> items)
		{
			while (listBox.SelectedItems.Count > 0)
			{
				items.Remove(listBox.SelectedItem as Item);
				listBox.Items.Remove(listBox.SelectedItem);
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

			playerData.Character.Name = textBoxName.Text;
			playerData.Character.UnknownFlags = unknownFlags;

			saveFileDialog1.FileName = playerData.Character.Name + ".d2s";

			if (saveFileDialog1.ShowDialog() == DialogResult.OK)
			{
				playerData.Write(saveFileDialog1.FileName, checkBoxSkipFailedData.Checked);
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

			// It's just debug output only, so just grab all the primitive+string properties of the items
			//  and output them
			PropertyInfo[] itemProperties = typeof(Item).GetProperties();

			foreach (var item in itemProperties)
			{
				if (item.PropertyType.IsPrimitive || item.PropertyType == typeof(string))
				{
					sb.AppendLine(string.Format("{0}: {1}", item.Name, item.GetValue(itemToEdit, null)));
				}
			}

			foreach (var socket in itemToEdit.Sockets)
			{
				listBoxItemEditorSockets.Items.Add(socket);
			}

			itemBindingSource.Clear();
			itemBindingSource.Add(itemToEdit);

			RefreshItemPropertiesGrid();

			tabControl1.SelectTab(2);
		}

		private void ClearItemEditor()
		{
			itemToEdit = null;
			dataGridViewItemProperties.DataSource = null;
			dataGridViewItemRunewordProperties.DataSource = null;
			dataGridViewItemSetProperties.DataSource = null;
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

		private void buttonItemViewerRefresh_Click(object sender, EventArgs e)
		{
			OpenItemProperties(itemToEdit);
		}


		private void AddItemProperty()
		{
			if (tabControlItemProperties.SelectedTab == tabPageItemProperties)
			{
				itemToEdit.Properties.Insert(itemToEdit.Properties.Count - 1, new Item.PropertyInfo());
			}
			else if (tabControlItemProperties.SelectedTab == tabPageItemSetProperties)
			{
				itemToEdit.PropertiesSet.Insert(itemToEdit.PropertiesSet.Count - 1, new Item.PropertyInfo());
			}
			else if (tabControlItemProperties.SelectedTab == tabPageItemRunewordProperties)
			{
				itemToEdit.PropertiesRuneword.Insert(itemToEdit.PropertiesRuneword.Count - 1, new Item.PropertyInfo());
			}
		}

		private void RemoveItemProperty()
		{
			if (tabControlItemProperties.SelectedTab == tabPageItemProperties)
			{
				foreach (DataGridViewRow item in dataGridViewItemProperties.SelectedRows)
				{
					itemToEdit.Properties.Remove(item.DataBoundItem as Item.PropertyInfo);
				}
			}
			else if (tabControlItemProperties.SelectedTab == tabPageItemSetProperties)
			{
				foreach (DataGridViewRow item in dataGridViewItemSetProperties.SelectedRows)
				{
					itemToEdit.PropertiesSet.Remove(item.DataBoundItem as Item.PropertyInfo);
				}
			}
			else if (tabControlItemProperties.SelectedTab == tabPageItemRunewordProperties)
			{
				foreach (DataGridViewRow item in dataGridViewItemRunewordProperties.SelectedRows)
				{
					itemToEdit.PropertiesRuneword.Remove(item.DataBoundItem as Item.PropertyInfo);
				}
			}			
		}

		private void buttonItemAddProperty_Click(object sender, EventArgs e)
		{
			AddItemProperty();
			RefreshItemPropertiesGrid();
		}

		private void RefreshItemPropertiesGrid()
		{
			dataGridViewItemProperties.DataSource = null;
			dataGridViewItemRunewordProperties.DataSource = null;
			dataGridViewItemSetProperties.DataSource = null;

			dataGridViewItemProperties.DataSource = itemToEdit.Properties;
			dataGridViewItemRunewordProperties.DataSource = itemToEdit.PropertiesRuneword;
			dataGridViewItemSetProperties.DataSource = itemToEdit.PropertiesSet;
		}

		private void buttonItemDeleteProperty_Click(object sender, EventArgs e)
		{
			if (itemToEdit == null)
			{
				return;
			}

			RemoveItemProperty();
			RefreshItemPropertiesGrid();
		}
	}
}
