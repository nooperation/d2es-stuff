using System;
using System.Collections.Generic;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using CharacterEditor;

namespace esCharView.Silverlight
{
	public partial class MainPage : UserControl
	{
		private SaveReader playerData = null;
		private Item itemToEdit = null;

		public MainPage()
		{
			InitializeComponent();

			comboBoxResourceSets.ItemsSource = CharacterEditor.Resources.GetResourceSets();
			comboBoxResourceSets.SelectedIndex = 1;
		}

		/// <summary>
		/// Load and process specified save file
		/// </summary>
		/// <param name="saveFile"></param>
		private void LoadSave(FileInfo saveFile)
		{
			byte[] rawCharacterbytes;
			using (BinaryReader br = new BinaryReader(saveFile.OpenRead()))
			{
				rawCharacterbytes = br.ReadBytes((int)br.BaseStream.Length);
			}

			ProcessCharacter(rawCharacterbytes);
		}

		/// <summary>
		/// Save character data to file (specified with dialog)
		/// </summary>
		private void SaveCharacter()
		{
			if (playerData == null)
			{
				return;
			}

			SaveFileDialog saveFileDialog1 = new SaveFileDialog();
			saveFileDialog1.DefaultExt = "d2s";
			saveFileDialog1.Filter = "Diablo II Save Files (*.d2s)|*.d2s|All Files (*.*)|*.*";

			if (saveFileDialog1.ShowDialog() == true)
			{
				playerData.Write(saveFileDialog1.OpenFile(), checkBoxSkipFailedData.IsChecked == true);
			}
		}

		/// <summary>
		/// Process raw character bytes from save file
		/// </summary>
		/// <param name="rawCharacterbytes">Raw data from save file</param>
		private void ProcessCharacter(byte[] rawCharacterbytes)
		{
			ClearPlayerData();

			CharacterEditor.Resources.Instance.ResourceSet = (string)comboBoxResourceSets.SelectedItem;
			playerData = new SaveReader(comboBoxResourceSets.SelectedItem.ToString());

			try
			{
				playerData.Read(rawCharacterbytes);
			}
			catch (Exception ex)
			{
				new ErrorWindow("Unable to read character, most likely wrong version: " + ex.Message, true);
				return;
			}

			string errorString = "";

			if (playerData.FailedCharacterDecoding)
			{
				errorString += "Failed character decoding\n";
			}
			if (playerData.FailedInventoryDecoding)
			{
				errorString += "Failed inventory decoding";
			}
			if (playerData.FailedSkillDecoding)
			{
				errorString += "Failed skill decoding";
			}

			if (errorString.Length > 0)
			{
				ErrorWindow errorWindow = new ErrorWindow(errorString, true);
			}

			if (playerData.Inventory.FailedItemCount > 0)
			{
				new ErrorWindow(string.Format("Failed to read {0} items. These items will not be included when saving character", playerData.Inventory.FailedItemCount));
			}

			RefreshCharacter();
			RefreshInventory();
		}

		/// <summary>
		/// Deletes selected items
		/// </summary>
		/// <param name="listBox"></param>
		/// <param name="items"></param>
		private void RemoveSelectedItems(ListBox listBox, List<Item> items)
		{
			List<Item> toRemove = new List<Item>();

			// Listbox data is bound directly to character item list, when removing items the listbox 
			//  collection gets modified.
			foreach (var item in listBox.SelectedItems)
			{
				toRemove.Add(item as Item);
			}

			foreach (var item in toRemove)
			{
				items.Remove(item);
			}

			RefreshInventory();
			ClearItemEditor();
		}

		/// <summary>
		/// Deletes selected items
		/// </summary>
		private void RemoveSelectedItems()
		{
			if (playerData == null)
			{
				return;
			}

			if (tabControlInventory.SelectedItem == tabPageItemEditor)
			{
				RemoveSelectedSocket();
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryPlayer)
			{
				RemoveSelectedItems(listBoxInventory, playerData.Inventory.PlayerItems);
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryCorpse)
			{
				RemoveSelectedItems(listBoxCorpseInventory, playerData.Inventory.CorpseItems);
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryMerc)
			{
				RemoveSelectedItems(listBoxMercInventory, playerData.Inventory.MercItems);
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryGolem)
			{
				RemoveSelectedItems(listBoxGolemInventory, playerData.Inventory.GolemItems);
			}
		}

		/// <summary>
		/// Sets currently selected item as the item being edted and opens the item editor panel
		/// </summary>
		private void OpenItemProperties()
		{
			ListBox source = GetCurrentItemListbox();
			if (source == null)
			{
				return;
			}

			OpenItemProperties(source.SelectedItem as Item);
		}

		/// <summary>
		/// Sets specified item as the item being edted and opens the item editor panel
		/// </summary>
		/// <param name="itemToEdit">Item to open in item editor panel</param>
		private void OpenItemProperties(Item itemToEdit)
		{
			this.itemToEdit = itemToEdit;

			RefreshItemEditor();

			tabControl1.SelectedItem = tabPageItemEditor;
		}

		/// <summary>
		/// Adds a new item property to item being edited
		/// </summary>
		private void AddItemProperty()
		{
			if (itemToEdit == null)
			{
				return;
			}

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

			RefreshItemEditor();
		}

		/// <summary>
		/// Removes specified item property from item being edited
		/// </summary>
		private void RemoveItemProperty()
		{
			if (itemToEdit == null)
			{
				return;
			}

			// Item property lists are bound directly to item data, when removing property from item the listbox 
			//  collection gets modified.
			List<Item.PropertyInfo> toRemove = new List<Item.PropertyInfo>();
			List<Item.PropertyInfo> source = null;

			if (tabControlItemProperties.SelectedItem == tabPageItemProperties)
			{
				source = itemToEdit.Properties;
				foreach (var item in dataGridViewItemProperties.SelectedItems)
				{
					toRemove.Add(item as Item.PropertyInfo);
				}
			}
			else if (tabControlItemProperties.SelectedItem == tabPageItemSetProperties)
			{
				source = itemToEdit.PropertiesSet;
				foreach (var item in dataGridViewItemSetProperties.SelectedItems)
				{
					toRemove.Add(item as Item.PropertyInfo);
				}
			}
			else if (tabControlItemProperties.SelectedItem == tabPageItemRunewordProperties)
			{
				source = itemToEdit.PropertiesRuneword;
				foreach (var item in dataGridViewItemRunewordProperties.SelectedItems)
				{
					toRemove.Add(item as Item.PropertyInfo);
				}
			}

			foreach (var item in toRemove)
			{
				source.Remove(item);
			}

			RefreshItemEditor();
		}

		/// <summary>
		/// Deletes selected socketed item.
		/// </summary>
		private void RemoveSelectedSocket()
		{
			if (listBoxItemEditorSockets.SelectedIndex < 0)
			{
				return;
			}

			if (itemToEdit == null)
			{
				return;
			}

			Item socketedItem = itemToEdit.RemoveSocketedItem(listBoxItemEditorSockets.SelectedIndex);
			playerData.Inventory.CorpseItems.Add(socketedItem);

			RefreshItemEditor();
			RefreshInventory();
		}

		/// <summary>
		/// Exports currently selected item to an item file
		/// </summary>
		private void ExportSelectedItem()
		{
			ListBox source = GetCurrentItemListbox();

			if (source == null || source.SelectedItems == null || playerData == null)
			{
				return;
			}

			Item item = source.SelectedItem as Item;
			SaveFileDialog sfd = new SaveFileDialog();
			sfd.DefaultExt = "d2i";
			sfd.Filter = "Diablo II Item Files (*.d2i)|*.d2i|All Files (*.*)|*.*";

			if (sfd.ShowDialog() == true)
			{
				using (BinaryWriter bw = new BinaryWriter(sfd.OpenFile()))
				{
					bw.Write(item.GetItemBytes());
				}
			}
		}

		/// <summary>
		/// Imports an item file and stores it in the player's corpse
		/// </summary>
		private void ImportItem()
		{
			if (playerData == null)
			{
				return;
			}

			OpenFileDialog ofd = new OpenFileDialog();
			ofd.Filter = "Diablo II Item Files (*.d2i)|*.d2i|All Files (*.*)|*.*";

			if (ofd.ShowDialog() == true)
			{
				byte[] itemData = new byte[ofd.File.Length];

				using (BinaryReader br = new BinaryReader(ofd.File.OpenRead()))
				{
					br.Read(itemData, 0, itemData.Length);
				}

				playerData.Inventory.CorpseItems.Add(new Item(itemData));
			}

			RefreshInventory();
		}

		#region Misc GUI functions

		private void RefreshCharacter()
		{
			dataGridViewCharacter.ItemsSource = null;
			dataGridViewStats.ItemsSource = null;
			textBoxPlayerName.DataContext = null;
			textBoxUnknownFlags.DataContext = null;

			if (playerData == null)
			{
				return;
			}

			dataGridViewCharacter.ItemsSource = new List<Character>() { playerData.Character };
			dataGridViewStats.ItemsSource = new List<Stat>() { playerData.Stat };
			textBoxPlayerName.DataContext = playerData.Character;
			textBoxUnknownFlags.DataContext = playerData.Character;
		}

		private void RefreshInventory()
		{
			listBoxInventory.ItemsSource = null;
			listBoxCorpseInventory.ItemsSource = null;
			listBoxMercInventory.ItemsSource = null;
			listBoxGolemInventory.ItemsSource = null;

			if (playerData == null)
			{
				return;
			}

			listBoxInventory.ItemsSource = playerData.Inventory.PlayerItems;
			listBoxCorpseInventory.ItemsSource = playerData.Inventory.CorpseItems;
			listBoxMercInventory.ItemsSource = playerData.Inventory.MercItems;
			listBoxGolemInventory.ItemsSource = playerData.Inventory.GolemItems;
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

		private void ClearItemEditor()
		{
			itemToEdit = null;
			RefreshItemEditor();
		}

		private void ClearPlayerData()
		{
			playerData = null;
			itemToEdit = null;

			RefreshCharacter();
			RefreshInventory();
			RefreshItemEditor();
		}

		private ListBox GetCurrentItemListbox()
		{
			if (tabControlInventory.SelectedItem == tabPageInventoryPlayer)
			{
				return listBoxInventory;
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryCorpse)
			{
				return listBoxCorpseInventory;
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryMerc)
			{
				return listBoxMercInventory;
			}
			else if (tabControlInventory.SelectedItem == tabPageInventoryGolem)
			{
				return listBoxGolemInventory;
			}
			else if (tabControl1.SelectedItem == tabPageItemEditor)
			{
				return listBoxItemEditorSockets;
			}

			throw new Exception("Invalid state for tabControlInventory");
		}

		#endregion

		#region Event Handlers

		private void buttonLoadCharacter_Click(object sender, RoutedEventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();
			openFileDialog.Filter = "Diablo II Save Files (*.d2s)|*.d2s|All Files (*.*)|*.*";
			if (openFileDialog.ShowDialog() == true)
			{
				LoadSave(openFileDialog.File);
			}
		}

		private void buttonRemoveItem_Click(object sender, RoutedEventArgs e)
		{
			RemoveSelectedItems();
		}

		private void buttonSave_Click(object sender, RoutedEventArgs e)
		{
			try
			{
				SaveCharacter();
			}
			catch (Exception ex)
			{
				new ErrorWindow("Failed to save character: " + ex.Message, true);
			}
		}

		private void buttonRemoveSocket_Click(object sender, RoutedEventArgs e)
		{
			try
			{
				RemoveSelectedSocket();
			}
			catch (Exception ex)
			{
				new ErrorWindow("Failed to remove socket: " + ex.Message, true);
			}
		}

		private void buttonItemAddProperty_Click(object sender, RoutedEventArgs e)
		{
			AddItemProperty();
		}

		private void buttonItemDeleteProperty_Click(object sender, RoutedEventArgs e)
		{
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

		private void buttonExportItem_Click(object sender, RoutedEventArgs e)
		{
			try
			{
				ExportSelectedItem();
			}
			catch (Exception ex)
			{
				new ErrorWindow("Failed to export item: " + ex.Message, true);
			}
		}

		private void buttonImportItem_Click(object sender, RoutedEventArgs e)
		{
			try
			{
				ImportItem();
			}
			catch (Exception ex)
			{
				new ErrorWindow("Failed to import item: " + ex.Message, true);
			}
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
				RemoveSelectedItems();
			}
		}
		#endregion
	}
}
