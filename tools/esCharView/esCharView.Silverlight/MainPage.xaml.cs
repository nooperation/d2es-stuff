using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using CharacterEditor;
using System.IO;
using System.Collections.ObjectModel;

namespace esCharView.Silverlight
{
	public partial class MainPage : UserControl
	{
		private SaveReader playerData = null;
		private Item itemToEdit = null;

		public MainPage()
		{
			InitializeComponent();
		}

		private void LoadSave(FileInfo saveFile)
		{
			byte[] rawCharacterbytes;
			using (BinaryReader br = new BinaryReader(saveFile.OpenRead()))
			{
				rawCharacterbytes = br.ReadBytes((int)br.BaseStream.Length);
			}
			ProcessCharacter(rawCharacterbytes);
		}

		private void ProcessCharacter(byte[] rawCharacterbytes)
		{
			playerData = new SaveReader();

			playerData.Read(rawCharacterbytes);

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

			dataGridViewCharacter.ItemsSource = new List<Character>() { playerData.Character };
			dataGridViewStats.ItemsSource = new List<Stat>() { playerData.Stat };
			textBoxPlayerName.DataContext = playerData.Character;
			textBoxUnknownFlags.DataContext = playerData.Character;

			ClearItemEditor();
			RefreshInventory();
		}

		private void removeSelectedItems(ListBox listBox, List<Item> items)
		{
			foreach (var item in listBox.SelectedItems)
			{
				items.Remove(item as Item);
			}

			RefreshInventory();
			ClearItemEditor();
		}

		private void removeSelectedItems()
		{
			if (tabControlInventory.SelectedItem == tabPageInventoryPlayer)
			{
				removeSelectedItems(listBoxInventory, playerData.Inventory.PlayerItems);
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryCorpse)
			{
				removeSelectedItems(listBoxCorpseInventory, playerData.Inventory.CorpseItems);
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryMerc)
			{
				removeSelectedItems(listBoxMercInventory, playerData.Inventory.MercItems);
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryGolem)
			{
				removeSelectedItems(listBoxGolemInventory, playerData.Inventory.GolemItems);
			}
		}

		private void OpenItemProperties()
		{
			if (tabControl1.SelectedItem == tabPageItemEditor)
			{
				if (listBoxItemEditorSockets.SelectedIndex == -1)
				{
					return;
				}

				OpenItemProperties(itemToEdit.Sockets[listBoxItemEditorSockets.SelectedIndex]);
			}
			else
			{
				if (tabControlInventory.SelectedItem == tabPageInventoryPlayer)
				{
					if (listBoxInventory.SelectedIndex == -1)
					{
						return;
					}

					OpenItemProperties(playerData.Inventory.PlayerItems[listBoxInventory.SelectedIndex]);
				}
				else if (tabControlInventory.SelectedItem == tabPageInventoryCorpse)
				{
					if (listBoxCorpseInventory.SelectedIndex == -1)
					{
						return;
					}

					OpenItemProperties(playerData.Inventory.CorpseItems[listBoxCorpseInventory.SelectedIndex]);
				}
				else if (tabControlInventory.SelectedItem == tabPageInventoryMerc)
				{
					if (listBoxMercInventory.SelectedIndex == -1)
					{
						return;
					}

					OpenItemProperties(playerData.Inventory.MercItems[listBoxMercInventory.SelectedIndex]);
				}
				else if (tabControlInventory.SelectedItem == tabPageInventoryGolem)
				{
					if (listBoxGolemInventory.SelectedIndex == -1)
					{
						return;
					}

					OpenItemProperties(playerData.Inventory.GolemItems[listBoxGolemInventory.SelectedIndex]);
				}
			}
		}

		private void OpenItemProperties(Item itemToEdit)
		{
			ClearItemEditor();

			this.itemToEdit = itemToEdit;
			
			RefreshItemEditor();

			tabControl1.SelectedItem = tabPageItemEditor;
		}

		private void AddItemProperty()
		{
			if (tabControlItemProperties.SelectedItem == tabPageItemProperties)
			{
				itemToEdit.Properties.Insert(itemToEdit.Properties.Count - 1, new Item.PropertyInfo());
			}
			else if (tabControlItemProperties.SelectedItem == tabPageItemSetProperties)
			{
				itemToEdit.PropertiesSet.Insert(itemToEdit.PropertiesSet.Count - 1, new Item.PropertyInfo());
			}
			else if (tabControlItemProperties.SelectedItem == tabPageItemRunewordProperties)
			{
				itemToEdit.PropertiesRuneword.Insert(itemToEdit.PropertiesRuneword.Count - 1, new Item.PropertyInfo());
			}
		}

		private void RemoveItemProperty()
		{
			if (tabControlItemProperties.SelectedItem == tabPageItemProperties)
			{
				foreach (var item in dataGridViewItemProperties.SelectedItems)
				{
					itemToEdit.Properties.Remove(item as Item.PropertyInfo);
				}
			}
			else if (tabControlItemProperties.SelectedItem == tabPageItemSetProperties)
			{
				foreach (var item in dataGridViewItemSetProperties.SelectedItems)
				{
					itemToEdit.PropertiesSet.Remove(item as Item.PropertyInfo);
				}
			}
			else if (tabControlItemProperties.SelectedItem == tabPageItemRunewordProperties)
			{
				foreach (var item in dataGridViewItemRunewordProperties.SelectedItems)
				{
					itemToEdit.PropertiesRuneword.Remove(item as Item.PropertyInfo);
				}
			}
		}

		private void RefreshInventory()
		{
			listBoxInventory.ItemsSource = null;
			listBoxCorpseInventory.ItemsSource = null;
			listBoxMercInventory.ItemsSource = null;
			listBoxGolemInventory.ItemsSource = null;

			listBoxInventory.ItemsSource = playerData.Inventory.PlayerItems;
			listBoxCorpseInventory.ItemsSource = playerData.Inventory.CorpseItems;
			listBoxMercInventory.ItemsSource = playerData.Inventory.MercItems;
			listBoxGolemInventory.ItemsSource = playerData.Inventory.GolemItems;
		}

		private void ClearItemEditor()
		{
			itemToEdit = null;
			RefreshItemEditor();		
		}

		private void RefreshItemEditor()
		{
			dataGridViewItem.ItemsSource = null;
			dataGridViewItemProperties.ItemsSource = null;
			dataGridViewItemRunewordProperties.ItemsSource = null;
			dataGridViewItemSetProperties.ItemsSource = null;
			listBoxItemEditorSockets.ItemsSource = null;

			if (itemToEdit == null)
			{
				return;
			}

			dataGridViewItem.ItemsSource = new List<Item>() { itemToEdit };
			dataGridViewItemProperties.ItemsSource = itemToEdit.Properties;
			dataGridViewItemRunewordProperties.ItemsSource = itemToEdit.PropertiesRuneword;
			dataGridViewItemSetProperties.ItemsSource = itemToEdit.PropertiesSet;
			listBoxItemEditorSockets.ItemsSource = itemToEdit.Sockets;
		}

		private void buttonLoadCharacter_Click(object sender, RoutedEventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();
			if (openFileDialog.ShowDialog() == true)
			{
				LoadSave(openFileDialog.File);
			}
		}

		private void buttonRemoveItem_Click(object sender, RoutedEventArgs e)
		{
			removeSelectedItems();
		}

		private void buttonSave_Click(object sender, RoutedEventArgs e)
		{
			SaveFileDialog saveFileDialog1 = new SaveFileDialog();

			if (saveFileDialog1.ShowDialog() == true)
			{
				playerData.Write(saveFileDialog1.OpenFile(), checkBoxSkipFailedData.IsChecked == true);
			}
		}

		private void buttonRemoveSocket_Click(object sender, RoutedEventArgs e)
		{
			if (listBoxItemEditorSockets.SelectedIndex < 0)
			{
				return;
			}

			if (itemToEdit == null)
			{
				return;
			}

			itemToEdit.RemoveSocketedItem(listBoxItemEditorSockets.SelectedIndex);
			RefreshItemEditor();
			RefreshInventory();
		}

		private void buttonItemAddProperty_Click(object sender, RoutedEventArgs e)
		{
			AddItemProperty();
			RefreshItemEditor();
		}

		private void buttonItemDeleteProperty_Click(object sender, RoutedEventArgs e)
		{
			if (itemToEdit == null)
			{
				return;
			}

			RemoveItemProperty();
		}

		private void buttonViewItem_Click(object sender, RoutedEventArgs e)
		{
			OpenItemProperties();
		}

		private void buttonItemViewerRefresh_Click(object sender, RoutedEventArgs e)
		{
			RefreshItemEditor();
		}

		private void onInventoryKeyUp(object sender, KeyEventArgs e)
		{
			ListBox source = sender as ListBox;

			if (source == null || source.SelectedItems == null)
			{
				return;
			}

			if (e.Key == Key.Enter)
			{
				OpenItemProperties(source.SelectedItem as Item);
			}
			else if (e.Key == Key.Delete)
			{
				removeSelectedItems();
			}
		}
	}
}
