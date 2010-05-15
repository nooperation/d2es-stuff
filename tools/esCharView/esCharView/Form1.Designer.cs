namespace esCharView
{
	partial class Form1
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.buttonLoadCharacter = new System.Windows.Forms.Button();
			this.listBoxInventory = new System.Windows.Forms.ListBox();
			this.openFileDialog2 = new System.Windows.Forms.OpenFileDialog();
			this.buttonRemoveItem = new System.Windows.Forms.Button();
			this.buttonSave = new System.Windows.Forms.Button();
			this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
			this.checkBoxExpansion = new System.Windows.Forms.CheckBox();
			this.checkBoxHardcore = new System.Windows.Forms.CheckBox();
			this.checkBoxDied = new System.Windows.Forms.CheckBox();
			this.textBoxName = new System.Windows.Forms.TextBox();
			this.textBoxUnknownFlags = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.tabControlInventory = new System.Windows.Forms.TabControl();
			this.tabPageInventoryPlayer = new System.Windows.Forms.TabPage();
			this.tabPageInventoryCorpse = new System.Windows.Forms.TabPage();
			this.listBoxCorpseInventory = new System.Windows.Forms.ListBox();
			this.tabPageInventoryMerc = new System.Windows.Forms.TabPage();
			this.listBoxMercInventory = new System.Windows.Forms.ListBox();
			this.tabPageInventoryGolem = new System.Windows.Forms.TabPage();
			this.listBoxGolemInventory = new System.Windows.Forms.ListBox();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPageGeneral = new System.Windows.Forms.TabPage();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.dataGridViewCharacter = new System.Windows.Forms.DataGridView();
			this.nameDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.hardcoreDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.diedDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.expansionDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.unknownFlagsDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.progressionDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.classDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.levelDisplayDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.hasMercenaryDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.mercenaryNameIdDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.mercenaryTypeDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.mercenaryExpDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.characterBindingSource = new System.Windows.Forms.BindingSource(this.components);
			this.dataGridViewStats = new System.Windows.Forms.DataGridView();
			this.strengthDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.energyDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.dexterityDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.vitalityDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.statPointsDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.skillPointsDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.hitpointsDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.baseHitpointsDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.manaDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.baseManaDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.staminaDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.baseStaminaDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.levelDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.experienceDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.goldDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.goldBankDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.killCountDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.deathCountDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.statBindingSource = new System.Windows.Forms.BindingSource(this.components);
			this.tabPageInventoryEditor = new System.Windows.Forms.TabPage();
			this.tabPageItemEditor = new System.Windows.Forms.TabPage();
			this.labelItemName = new System.Windows.Forms.Label();
			this.buttonRemoveSocket = new System.Windows.Forms.Button();
			this.listBoxItemEditorSockets = new System.Windows.Forms.ListBox();
			this.textBoxItemEditor = new System.Windows.Forms.TextBox();
			this.dataGridView1 = new System.Windows.Forms.DataGridView();
			this.itemBindingSource = new System.Windows.Forms.BindingSource(this.components);
			this.isEquippedDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isInSocketDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isIdentifiedDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isSwitchInDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isSwitchOutDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isBrokenDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isSocketedDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isStoreItemDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isEarDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isStarterItemDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isSimpleItemDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isEtherealDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isPersonalizedDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isGambleDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.isRunewordDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.locationDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.positionOnBodyDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.positionXDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.positionYDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.storageIdDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.itemCodeDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.goldAmountDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.socketsFilledDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.idDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.levelDataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.qualityDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.hasGraphicDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.graphicDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.uniqueSetIdDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.defenseDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.maxDurabilityDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.durabilityDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.isIndestructableDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.socketCountDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.quantityDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.randomFlagDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.unknownGoldFlagDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.classFlagDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.earClassDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.earLevelDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.prefixFlag0DataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.prefixFlag1DataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.prefixFlag2DataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.suffixFlag0DataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.suffixFlag1DataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.suffixFlag2DataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.prefix0DataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.prefix1DataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.prefix2DataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.suffix0DataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.suffix1DataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.suffix2DataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.buttonItemViewerRefresh = new System.Windows.Forms.Button();
			this.tabControlInventory.SuspendLayout();
			this.tabPageInventoryPlayer.SuspendLayout();
			this.tabPageInventoryCorpse.SuspendLayout();
			this.tabPageInventoryMerc.SuspendLayout();
			this.tabPageInventoryGolem.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.tabPageGeneral.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewCharacter)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.characterBindingSource)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewStats)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.statBindingSource)).BeginInit();
			this.tabPageInventoryEditor.SuspendLayout();
			this.tabPageItemEditor.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.itemBindingSource)).BeginInit();
			this.SuspendLayout();
			// 
			// buttonLoadCharacter
			// 
			this.buttonLoadCharacter.Location = new System.Drawing.Point(12, 12);
			this.buttonLoadCharacter.Name = "buttonLoadCharacter";
			this.buttonLoadCharacter.Size = new System.Drawing.Size(68, 23);
			this.buttonLoadCharacter.TabIndex = 0;
			this.buttonLoadCharacter.Text = "Load";
			this.buttonLoadCharacter.UseVisualStyleBackColor = true;
			this.buttonLoadCharacter.Click += new System.EventHandler(this.button1_Click);
			// 
			// listBoxInventory
			// 
			this.listBoxInventory.Dock = System.Windows.Forms.DockStyle.Fill;
			this.listBoxInventory.Font = new System.Drawing.Font("Lucida Console", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.listBoxInventory.FormattingEnabled = true;
			this.listBoxInventory.Location = new System.Drawing.Point(3, 3);
			this.listBoxInventory.Name = "listBoxInventory";
			this.listBoxInventory.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
			this.listBoxInventory.Size = new System.Drawing.Size(607, 194);
			this.listBoxInventory.TabIndex = 2;
			this.listBoxInventory.DoubleClick += new System.EventHandler(this.listBoxInventory_DoubleClick);
			// 
			// openFileDialog2
			// 
			this.openFileDialog2.Filter = "d2s files|*.d2s|All files|*.*";
			this.openFileDialog2.InitialDirectory = ".";
			// 
			// buttonRemoveItem
			// 
			this.buttonRemoveItem.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonRemoveItem.Location = new System.Drawing.Point(541, 247);
			this.buttonRemoveItem.Name = "buttonRemoveItem";
			this.buttonRemoveItem.Size = new System.Drawing.Size(75, 23);
			this.buttonRemoveItem.TabIndex = 4;
			this.buttonRemoveItem.Text = "Remove";
			this.buttonRemoveItem.UseVisualStyleBackColor = true;
			this.buttonRemoveItem.Click += new System.EventHandler(this.buttonRemoveItem_Click);
			// 
			// buttonSave
			// 
			this.buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonSave.Enabled = false;
			this.buttonSave.Location = new System.Drawing.Point(545, 12);
			this.buttonSave.Name = "buttonSave";
			this.buttonSave.Size = new System.Drawing.Size(75, 23);
			this.buttonSave.TabIndex = 6;
			this.buttonSave.Text = "Save";
			this.buttonSave.UseVisualStyleBackColor = true;
			this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
			// 
			// saveFileDialog1
			// 
			this.saveFileDialog1.DefaultExt = "d2s";
			this.saveFileDialog1.Filter = "d2s files|*.d2s";
			this.saveFileDialog1.InitialDirectory = ".";
			// 
			// checkBoxExpansion
			// 
			this.checkBoxExpansion.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.checkBoxExpansion.AutoSize = true;
			this.checkBoxExpansion.Location = new System.Drawing.Point(265, 15);
			this.checkBoxExpansion.Name = "checkBoxExpansion";
			this.checkBoxExpansion.Size = new System.Drawing.Size(75, 17);
			this.checkBoxExpansion.TabIndex = 7;
			this.checkBoxExpansion.Text = "Expansion";
			this.checkBoxExpansion.UseVisualStyleBackColor = true;
			// 
			// checkBoxHardcore
			// 
			this.checkBoxHardcore.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.checkBoxHardcore.AutoSize = true;
			this.checkBoxHardcore.Location = new System.Drawing.Point(346, 15);
			this.checkBoxHardcore.Name = "checkBoxHardcore";
			this.checkBoxHardcore.Size = new System.Drawing.Size(70, 17);
			this.checkBoxHardcore.TabIndex = 8;
			this.checkBoxHardcore.Text = "Hardcore";
			this.checkBoxHardcore.UseVisualStyleBackColor = true;
			// 
			// checkBoxDied
			// 
			this.checkBoxDied.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.checkBoxDied.AutoSize = true;
			this.checkBoxDied.Location = new System.Drawing.Point(417, 15);
			this.checkBoxDied.Name = "checkBoxDied";
			this.checkBoxDied.Size = new System.Drawing.Size(48, 17);
			this.checkBoxDied.TabIndex = 9;
			this.checkBoxDied.Text = "Died";
			this.checkBoxDied.UseVisualStyleBackColor = true;
			// 
			// textBoxName
			// 
			this.textBoxName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxName.Location = new System.Drawing.Point(103, 13);
			this.textBoxName.MaxLength = 16;
			this.textBoxName.Name = "textBoxName";
			this.textBoxName.Size = new System.Drawing.Size(146, 20);
			this.textBoxName.TabIndex = 11;
			// 
			// textBoxUnknownFlags
			// 
			this.textBoxUnknownFlags.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxUnknownFlags.Location = new System.Drawing.Point(512, 13);
			this.textBoxUnknownFlags.MaxLength = 2;
			this.textBoxUnknownFlags.Name = "textBoxUnknownFlags";
			this.textBoxUnknownFlags.Size = new System.Drawing.Size(27, 20);
			this.textBoxUnknownFlags.TabIndex = 12;
			this.textBoxUnknownFlags.Text = "00";
			this.textBoxUnknownFlags.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxUnknownFlags_KeyPress);
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(473, 17);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(33, 13);
			this.label2.TabIndex = 13;
			this.label2.Text = "Other";
			// 
			// tabControlInventory
			// 
			this.tabControlInventory.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tabControlInventory.Controls.Add(this.tabPageInventoryPlayer);
			this.tabControlInventory.Controls.Add(this.tabPageInventoryCorpse);
			this.tabControlInventory.Controls.Add(this.tabPageInventoryMerc);
			this.tabControlInventory.Controls.Add(this.tabPageInventoryGolem);
			this.tabControlInventory.Location = new System.Drawing.Point(3, 3);
			this.tabControlInventory.Name = "tabControlInventory";
			this.tabControlInventory.SelectedIndex = 0;
			this.tabControlInventory.Size = new System.Drawing.Size(621, 226);
			this.tabControlInventory.TabIndex = 14;
			// 
			// tabPageInventoryPlayer
			// 
			this.tabPageInventoryPlayer.Controls.Add(this.listBoxInventory);
			this.tabPageInventoryPlayer.Location = new System.Drawing.Point(4, 22);
			this.tabPageInventoryPlayer.Name = "tabPageInventoryPlayer";
			this.tabPageInventoryPlayer.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageInventoryPlayer.Size = new System.Drawing.Size(613, 200);
			this.tabPageInventoryPlayer.TabIndex = 0;
			this.tabPageInventoryPlayer.Text = "Player";
			this.tabPageInventoryPlayer.UseVisualStyleBackColor = true;
			// 
			// tabPageInventoryCorpse
			// 
			this.tabPageInventoryCorpse.Controls.Add(this.listBoxCorpseInventory);
			this.tabPageInventoryCorpse.Location = new System.Drawing.Point(4, 22);
			this.tabPageInventoryCorpse.Name = "tabPageInventoryCorpse";
			this.tabPageInventoryCorpse.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageInventoryCorpse.Size = new System.Drawing.Size(613, 200);
			this.tabPageInventoryCorpse.TabIndex = 1;
			this.tabPageInventoryCorpse.Text = "Corpse";
			this.tabPageInventoryCorpse.UseVisualStyleBackColor = true;
			// 
			// listBoxCorpseInventory
			// 
			this.listBoxCorpseInventory.Dock = System.Windows.Forms.DockStyle.Fill;
			this.listBoxCorpseInventory.Font = new System.Drawing.Font("Lucida Console", 9.75F);
			this.listBoxCorpseInventory.FormattingEnabled = true;
			this.listBoxCorpseInventory.Location = new System.Drawing.Point(3, 3);
			this.listBoxCorpseInventory.Name = "listBoxCorpseInventory";
			this.listBoxCorpseInventory.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
			this.listBoxCorpseInventory.Size = new System.Drawing.Size(607, 194);
			this.listBoxCorpseInventory.TabIndex = 0;
			this.listBoxCorpseInventory.DoubleClick += new System.EventHandler(this.listBoxInventory_DoubleClick);
			// 
			// tabPageInventoryMerc
			// 
			this.tabPageInventoryMerc.Controls.Add(this.listBoxMercInventory);
			this.tabPageInventoryMerc.Location = new System.Drawing.Point(4, 22);
			this.tabPageInventoryMerc.Name = "tabPageInventoryMerc";
			this.tabPageInventoryMerc.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageInventoryMerc.Size = new System.Drawing.Size(613, 200);
			this.tabPageInventoryMerc.TabIndex = 2;
			this.tabPageInventoryMerc.Text = "Mercenary";
			this.tabPageInventoryMerc.UseVisualStyleBackColor = true;
			// 
			// listBoxMercInventory
			// 
			this.listBoxMercInventory.Dock = System.Windows.Forms.DockStyle.Fill;
			this.listBoxMercInventory.Font = new System.Drawing.Font("Lucida Console", 9.75F);
			this.listBoxMercInventory.FormattingEnabled = true;
			this.listBoxMercInventory.Location = new System.Drawing.Point(3, 3);
			this.listBoxMercInventory.Name = "listBoxMercInventory";
			this.listBoxMercInventory.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
			this.listBoxMercInventory.Size = new System.Drawing.Size(607, 194);
			this.listBoxMercInventory.TabIndex = 0;
			this.listBoxMercInventory.DoubleClick += new System.EventHandler(this.listBoxInventory_DoubleClick);
			// 
			// tabPageInventoryGolem
			// 
			this.tabPageInventoryGolem.Controls.Add(this.listBoxGolemInventory);
			this.tabPageInventoryGolem.Location = new System.Drawing.Point(4, 22);
			this.tabPageInventoryGolem.Name = "tabPageInventoryGolem";
			this.tabPageInventoryGolem.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageInventoryGolem.Size = new System.Drawing.Size(613, 200);
			this.tabPageInventoryGolem.TabIndex = 3;
			this.tabPageInventoryGolem.Text = "Golem";
			this.tabPageInventoryGolem.UseVisualStyleBackColor = true;
			// 
			// listBoxGolemInventory
			// 
			this.listBoxGolemInventory.Dock = System.Windows.Forms.DockStyle.Fill;
			this.listBoxGolemInventory.Font = new System.Drawing.Font("Lucida Console", 9.75F);
			this.listBoxGolemInventory.FormattingEnabled = true;
			this.listBoxGolemInventory.Location = new System.Drawing.Point(3, 3);
			this.listBoxGolemInventory.Name = "listBoxGolemInventory";
			this.listBoxGolemInventory.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
			this.listBoxGolemInventory.Size = new System.Drawing.Size(607, 194);
			this.listBoxGolemInventory.TabIndex = 0;
			this.listBoxGolemInventory.DoubleClick += new System.EventHandler(this.listBoxInventory_DoubleClick);
			// 
			// tabControl1
			// 
			this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl1.Controls.Add(this.tabPageGeneral);
			this.tabControl1.Controls.Add(this.tabPageInventoryEditor);
			this.tabControl1.Controls.Add(this.tabPageItemEditor);
			this.tabControl1.Location = new System.Drawing.Point(0, 41);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(632, 302);
			this.tabControl1.TabIndex = 15;
			// 
			// tabPageGeneral
			// 
			this.tabPageGeneral.Controls.Add(this.splitContainer1);
			this.tabPageGeneral.Location = new System.Drawing.Point(4, 22);
			this.tabPageGeneral.Name = "tabPageGeneral";
			this.tabPageGeneral.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageGeneral.Size = new System.Drawing.Size(624, 276);
			this.tabPageGeneral.TabIndex = 0;
			this.tabPageGeneral.Text = "General";
			this.tabPageGeneral.UseVisualStyleBackColor = true;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(3, 3);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.dataGridViewCharacter);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.dataGridViewStats);
			this.splitContainer1.Size = new System.Drawing.Size(618, 270);
			this.splitContainer1.SplitterDistance = 128;
			this.splitContainer1.TabIndex = 2;
			// 
			// dataGridViewCharacter
			// 
			this.dataGridViewCharacter.AutoGenerateColumns = false;
			this.dataGridViewCharacter.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewCharacter.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.nameDataGridViewTextBoxColumn,
            this.hardcoreDataGridViewCheckBoxColumn,
            this.diedDataGridViewCheckBoxColumn,
            this.expansionDataGridViewCheckBoxColumn,
            this.unknownFlagsDataGridViewTextBoxColumn,
            this.progressionDataGridViewTextBoxColumn,
            this.classDataGridViewTextBoxColumn,
            this.levelDisplayDataGridViewTextBoxColumn,
            this.hasMercenaryDataGridViewCheckBoxColumn,
            this.mercenaryNameIdDataGridViewTextBoxColumn,
            this.mercenaryTypeDataGridViewTextBoxColumn,
            this.mercenaryExpDataGridViewTextBoxColumn});
			this.dataGridViewCharacter.DataSource = this.characterBindingSource;
			this.dataGridViewCharacter.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridViewCharacter.Location = new System.Drawing.Point(0, 0);
			this.dataGridViewCharacter.Name = "dataGridViewCharacter";
			this.dataGridViewCharacter.Size = new System.Drawing.Size(618, 128);
			this.dataGridViewCharacter.TabIndex = 0;
			// 
			// nameDataGridViewTextBoxColumn
			// 
			this.nameDataGridViewTextBoxColumn.DataPropertyName = "Name";
			this.nameDataGridViewTextBoxColumn.HeaderText = "Name";
			this.nameDataGridViewTextBoxColumn.Name = "nameDataGridViewTextBoxColumn";
			// 
			// hardcoreDataGridViewCheckBoxColumn
			// 
			this.hardcoreDataGridViewCheckBoxColumn.DataPropertyName = "Hardcore";
			this.hardcoreDataGridViewCheckBoxColumn.HeaderText = "Hardcore";
			this.hardcoreDataGridViewCheckBoxColumn.Name = "hardcoreDataGridViewCheckBoxColumn";
			// 
			// diedDataGridViewCheckBoxColumn
			// 
			this.diedDataGridViewCheckBoxColumn.DataPropertyName = "Died";
			this.diedDataGridViewCheckBoxColumn.HeaderText = "Died";
			this.diedDataGridViewCheckBoxColumn.Name = "diedDataGridViewCheckBoxColumn";
			// 
			// expansionDataGridViewCheckBoxColumn
			// 
			this.expansionDataGridViewCheckBoxColumn.DataPropertyName = "Expansion";
			this.expansionDataGridViewCheckBoxColumn.HeaderText = "Expansion";
			this.expansionDataGridViewCheckBoxColumn.Name = "expansionDataGridViewCheckBoxColumn";
			// 
			// unknownFlagsDataGridViewTextBoxColumn
			// 
			this.unknownFlagsDataGridViewTextBoxColumn.DataPropertyName = "UnknownFlags";
			this.unknownFlagsDataGridViewTextBoxColumn.HeaderText = "UnknownFlags";
			this.unknownFlagsDataGridViewTextBoxColumn.Name = "unknownFlagsDataGridViewTextBoxColumn";
			// 
			// progressionDataGridViewTextBoxColumn
			// 
			this.progressionDataGridViewTextBoxColumn.DataPropertyName = "Progression";
			this.progressionDataGridViewTextBoxColumn.HeaderText = "Progression";
			this.progressionDataGridViewTextBoxColumn.Name = "progressionDataGridViewTextBoxColumn";
			// 
			// classDataGridViewTextBoxColumn
			// 
			this.classDataGridViewTextBoxColumn.DataPropertyName = "Class";
			this.classDataGridViewTextBoxColumn.HeaderText = "Class";
			this.classDataGridViewTextBoxColumn.Name = "classDataGridViewTextBoxColumn";
			// 
			// levelDisplayDataGridViewTextBoxColumn
			// 
			this.levelDisplayDataGridViewTextBoxColumn.DataPropertyName = "LevelDisplay";
			this.levelDisplayDataGridViewTextBoxColumn.HeaderText = "LevelDisplay";
			this.levelDisplayDataGridViewTextBoxColumn.Name = "levelDisplayDataGridViewTextBoxColumn";
			// 
			// hasMercenaryDataGridViewCheckBoxColumn
			// 
			this.hasMercenaryDataGridViewCheckBoxColumn.DataPropertyName = "HasMercenary";
			this.hasMercenaryDataGridViewCheckBoxColumn.HeaderText = "HasMercenary";
			this.hasMercenaryDataGridViewCheckBoxColumn.Name = "hasMercenaryDataGridViewCheckBoxColumn";
			this.hasMercenaryDataGridViewCheckBoxColumn.ReadOnly = true;
			// 
			// mercenaryNameIdDataGridViewTextBoxColumn
			// 
			this.mercenaryNameIdDataGridViewTextBoxColumn.DataPropertyName = "MercenaryNameId";
			this.mercenaryNameIdDataGridViewTextBoxColumn.HeaderText = "MercenaryNameId";
			this.mercenaryNameIdDataGridViewTextBoxColumn.Name = "mercenaryNameIdDataGridViewTextBoxColumn";
			this.mercenaryNameIdDataGridViewTextBoxColumn.ReadOnly = true;
			// 
			// mercenaryTypeDataGridViewTextBoxColumn
			// 
			this.mercenaryTypeDataGridViewTextBoxColumn.DataPropertyName = "MercenaryType";
			this.mercenaryTypeDataGridViewTextBoxColumn.HeaderText = "MercenaryType";
			this.mercenaryTypeDataGridViewTextBoxColumn.Name = "mercenaryTypeDataGridViewTextBoxColumn";
			this.mercenaryTypeDataGridViewTextBoxColumn.ReadOnly = true;
			// 
			// mercenaryExpDataGridViewTextBoxColumn
			// 
			this.mercenaryExpDataGridViewTextBoxColumn.DataPropertyName = "MercenaryExp";
			this.mercenaryExpDataGridViewTextBoxColumn.HeaderText = "MercenaryExp";
			this.mercenaryExpDataGridViewTextBoxColumn.Name = "mercenaryExpDataGridViewTextBoxColumn";
			this.mercenaryExpDataGridViewTextBoxColumn.ReadOnly = true;
			// 
			// characterBindingSource
			// 
			this.characterBindingSource.DataSource = typeof(esCharView.Character);
			// 
			// dataGridViewStats
			// 
			this.dataGridViewStats.AutoGenerateColumns = false;
			this.dataGridViewStats.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewStats.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.strengthDataGridViewTextBoxColumn,
            this.energyDataGridViewTextBoxColumn,
            this.dexterityDataGridViewTextBoxColumn,
            this.vitalityDataGridViewTextBoxColumn,
            this.statPointsDataGridViewTextBoxColumn,
            this.skillPointsDataGridViewTextBoxColumn,
            this.hitpointsDataGridViewTextBoxColumn,
            this.baseHitpointsDataGridViewTextBoxColumn,
            this.manaDataGridViewTextBoxColumn,
            this.baseManaDataGridViewTextBoxColumn,
            this.staminaDataGridViewTextBoxColumn,
            this.baseStaminaDataGridViewTextBoxColumn,
            this.levelDataGridViewTextBoxColumn,
            this.experienceDataGridViewTextBoxColumn,
            this.goldDataGridViewTextBoxColumn,
            this.goldBankDataGridViewTextBoxColumn,
            this.killCountDataGridViewTextBoxColumn,
            this.deathCountDataGridViewTextBoxColumn});
			this.dataGridViewStats.DataSource = this.statBindingSource;
			this.dataGridViewStats.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridViewStats.Location = new System.Drawing.Point(0, 0);
			this.dataGridViewStats.Name = "dataGridViewStats";
			this.dataGridViewStats.Size = new System.Drawing.Size(618, 138);
			this.dataGridViewStats.TabIndex = 1;
			// 
			// strengthDataGridViewTextBoxColumn
			// 
			this.strengthDataGridViewTextBoxColumn.DataPropertyName = "Strength";
			this.strengthDataGridViewTextBoxColumn.HeaderText = "Strength";
			this.strengthDataGridViewTextBoxColumn.Name = "strengthDataGridViewTextBoxColumn";
			// 
			// energyDataGridViewTextBoxColumn
			// 
			this.energyDataGridViewTextBoxColumn.DataPropertyName = "Energy";
			this.energyDataGridViewTextBoxColumn.HeaderText = "Energy";
			this.energyDataGridViewTextBoxColumn.Name = "energyDataGridViewTextBoxColumn";
			// 
			// dexterityDataGridViewTextBoxColumn
			// 
			this.dexterityDataGridViewTextBoxColumn.DataPropertyName = "Dexterity";
			this.dexterityDataGridViewTextBoxColumn.HeaderText = "Dexterity";
			this.dexterityDataGridViewTextBoxColumn.Name = "dexterityDataGridViewTextBoxColumn";
			// 
			// vitalityDataGridViewTextBoxColumn
			// 
			this.vitalityDataGridViewTextBoxColumn.DataPropertyName = "Vitality";
			this.vitalityDataGridViewTextBoxColumn.HeaderText = "Vitality";
			this.vitalityDataGridViewTextBoxColumn.Name = "vitalityDataGridViewTextBoxColumn";
			// 
			// statPointsDataGridViewTextBoxColumn
			// 
			this.statPointsDataGridViewTextBoxColumn.DataPropertyName = "StatPoints";
			this.statPointsDataGridViewTextBoxColumn.HeaderText = "StatPoints";
			this.statPointsDataGridViewTextBoxColumn.Name = "statPointsDataGridViewTextBoxColumn";
			// 
			// skillPointsDataGridViewTextBoxColumn
			// 
			this.skillPointsDataGridViewTextBoxColumn.DataPropertyName = "SkillPoints";
			this.skillPointsDataGridViewTextBoxColumn.HeaderText = "SkillPoints";
			this.skillPointsDataGridViewTextBoxColumn.Name = "skillPointsDataGridViewTextBoxColumn";
			// 
			// hitpointsDataGridViewTextBoxColumn
			// 
			this.hitpointsDataGridViewTextBoxColumn.DataPropertyName = "Hitpoints";
			this.hitpointsDataGridViewTextBoxColumn.HeaderText = "Hitpoints";
			this.hitpointsDataGridViewTextBoxColumn.Name = "hitpointsDataGridViewTextBoxColumn";
			// 
			// baseHitpointsDataGridViewTextBoxColumn
			// 
			this.baseHitpointsDataGridViewTextBoxColumn.DataPropertyName = "BaseHitpoints";
			this.baseHitpointsDataGridViewTextBoxColumn.HeaderText = "BaseHitpoints";
			this.baseHitpointsDataGridViewTextBoxColumn.Name = "baseHitpointsDataGridViewTextBoxColumn";
			// 
			// manaDataGridViewTextBoxColumn
			// 
			this.manaDataGridViewTextBoxColumn.DataPropertyName = "Mana";
			this.manaDataGridViewTextBoxColumn.HeaderText = "Mana";
			this.manaDataGridViewTextBoxColumn.Name = "manaDataGridViewTextBoxColumn";
			// 
			// baseManaDataGridViewTextBoxColumn
			// 
			this.baseManaDataGridViewTextBoxColumn.DataPropertyName = "BaseMana";
			this.baseManaDataGridViewTextBoxColumn.HeaderText = "BaseMana";
			this.baseManaDataGridViewTextBoxColumn.Name = "baseManaDataGridViewTextBoxColumn";
			// 
			// staminaDataGridViewTextBoxColumn
			// 
			this.staminaDataGridViewTextBoxColumn.DataPropertyName = "Stamina";
			this.staminaDataGridViewTextBoxColumn.HeaderText = "Stamina";
			this.staminaDataGridViewTextBoxColumn.Name = "staminaDataGridViewTextBoxColumn";
			// 
			// baseStaminaDataGridViewTextBoxColumn
			// 
			this.baseStaminaDataGridViewTextBoxColumn.DataPropertyName = "BaseStamina";
			this.baseStaminaDataGridViewTextBoxColumn.HeaderText = "BaseStamina";
			this.baseStaminaDataGridViewTextBoxColumn.Name = "baseStaminaDataGridViewTextBoxColumn";
			// 
			// levelDataGridViewTextBoxColumn
			// 
			this.levelDataGridViewTextBoxColumn.DataPropertyName = "Level";
			this.levelDataGridViewTextBoxColumn.HeaderText = "Level";
			this.levelDataGridViewTextBoxColumn.Name = "levelDataGridViewTextBoxColumn";
			// 
			// experienceDataGridViewTextBoxColumn
			// 
			this.experienceDataGridViewTextBoxColumn.DataPropertyName = "Experience";
			this.experienceDataGridViewTextBoxColumn.HeaderText = "Experience";
			this.experienceDataGridViewTextBoxColumn.Name = "experienceDataGridViewTextBoxColumn";
			// 
			// goldDataGridViewTextBoxColumn
			// 
			this.goldDataGridViewTextBoxColumn.DataPropertyName = "Gold";
			this.goldDataGridViewTextBoxColumn.HeaderText = "Gold";
			this.goldDataGridViewTextBoxColumn.Name = "goldDataGridViewTextBoxColumn";
			// 
			// goldBankDataGridViewTextBoxColumn
			// 
			this.goldBankDataGridViewTextBoxColumn.DataPropertyName = "GoldBank";
			this.goldBankDataGridViewTextBoxColumn.HeaderText = "GoldBank";
			this.goldBankDataGridViewTextBoxColumn.Name = "goldBankDataGridViewTextBoxColumn";
			// 
			// killCountDataGridViewTextBoxColumn
			// 
			this.killCountDataGridViewTextBoxColumn.DataPropertyName = "KillCount";
			this.killCountDataGridViewTextBoxColumn.HeaderText = "KillCount";
			this.killCountDataGridViewTextBoxColumn.Name = "killCountDataGridViewTextBoxColumn";
			// 
			// deathCountDataGridViewTextBoxColumn
			// 
			this.deathCountDataGridViewTextBoxColumn.DataPropertyName = "DeathCount";
			this.deathCountDataGridViewTextBoxColumn.HeaderText = "DeathCount";
			this.deathCountDataGridViewTextBoxColumn.Name = "deathCountDataGridViewTextBoxColumn";
			// 
			// statBindingSource
			// 
			this.statBindingSource.DataSource = typeof(esCharView.Stat);
			// 
			// tabPageInventoryEditor
			// 
			this.tabPageInventoryEditor.Controls.Add(this.tabControlInventory);
			this.tabPageInventoryEditor.Controls.Add(this.buttonRemoveItem);
			this.tabPageInventoryEditor.Location = new System.Drawing.Point(4, 22);
			this.tabPageInventoryEditor.Name = "tabPageInventoryEditor";
			this.tabPageInventoryEditor.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageInventoryEditor.Size = new System.Drawing.Size(624, 276);
			this.tabPageInventoryEditor.TabIndex = 1;
			this.tabPageInventoryEditor.Text = "Inventory";
			this.tabPageInventoryEditor.UseVisualStyleBackColor = true;
			// 
			// tabPageItemEditor
			// 
			this.tabPageItemEditor.Controls.Add(this.buttonItemViewerRefresh);
			this.tabPageItemEditor.Controls.Add(this.dataGridView1);
			this.tabPageItemEditor.Controls.Add(this.labelItemName);
			this.tabPageItemEditor.Controls.Add(this.buttonRemoveSocket);
			this.tabPageItemEditor.Controls.Add(this.listBoxItemEditorSockets);
			this.tabPageItemEditor.Controls.Add(this.textBoxItemEditor);
			this.tabPageItemEditor.Location = new System.Drawing.Point(4, 22);
			this.tabPageItemEditor.Name = "tabPageItemEditor";
			this.tabPageItemEditor.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageItemEditor.Size = new System.Drawing.Size(624, 276);
			this.tabPageItemEditor.TabIndex = 2;
			this.tabPageItemEditor.Text = "ItemViewer";
			this.tabPageItemEditor.UseVisualStyleBackColor = true;
			// 
			// labelItemName
			// 
			this.labelItemName.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.labelItemName.AutoSize = true;
			this.labelItemName.Location = new System.Drawing.Point(251, 12);
			this.labelItemName.Name = "labelItemName";
			this.labelItemName.Size = new System.Drawing.Size(0, 13);
			this.labelItemName.TabIndex = 4;
			// 
			// buttonRemoveSocket
			// 
			this.buttonRemoveSocket.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonRemoveSocket.Location = new System.Drawing.Point(518, 12);
			this.buttonRemoveSocket.Name = "buttonRemoveSocket";
			this.buttonRemoveSocket.Size = new System.Drawing.Size(98, 23);
			this.buttonRemoveSocket.TabIndex = 3;
			this.buttonRemoveSocket.Text = "Remove socket";
			this.buttonRemoveSocket.UseVisualStyleBackColor = true;
			this.buttonRemoveSocket.Click += new System.EventHandler(this.buttonRemoveSocket_Click);
			// 
			// listBoxItemEditorSockets
			// 
			this.listBoxItemEditorSockets.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.listBoxItemEditorSockets.Font = new System.Drawing.Font("Lucida Console", 9.75F);
			this.listBoxItemEditorSockets.FormattingEnabled = true;
			this.listBoxItemEditorSockets.Location = new System.Drawing.Point(254, 42);
			this.listBoxItemEditorSockets.Name = "listBoxItemEditorSockets";
			this.listBoxItemEditorSockets.Size = new System.Drawing.Size(367, 134);
			this.listBoxItemEditorSockets.TabIndex = 2;
			this.listBoxItemEditorSockets.DoubleClick += new System.EventHandler(this.listBoxInventory_DoubleClick);
			// 
			// textBoxItemEditor
			// 
			this.textBoxItemEditor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxItemEditor.Font = new System.Drawing.Font("Lucida Console", 9.75F);
			this.textBoxItemEditor.Location = new System.Drawing.Point(8, 6);
			this.textBoxItemEditor.Multiline = true;
			this.textBoxItemEditor.Name = "textBoxItemEditor";
			this.textBoxItemEditor.ReadOnly = true;
			this.textBoxItemEditor.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.textBoxItemEditor.Size = new System.Drawing.Size(237, 170);
			this.textBoxItemEditor.TabIndex = 1;
			this.textBoxItemEditor.WordWrap = false;
			// 
			// dataGridView1
			// 
			this.dataGridView1.AutoGenerateColumns = false;
			this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.isEquippedDataGridViewCheckBoxColumn,
            this.isInSocketDataGridViewCheckBoxColumn,
            this.isIdentifiedDataGridViewCheckBoxColumn,
            this.isSwitchInDataGridViewCheckBoxColumn,
            this.isSwitchOutDataGridViewCheckBoxColumn,
            this.isBrokenDataGridViewCheckBoxColumn,
            this.isSocketedDataGridViewCheckBoxColumn,
            this.isStoreItemDataGridViewCheckBoxColumn,
            this.isEarDataGridViewCheckBoxColumn,
            this.isStarterItemDataGridViewCheckBoxColumn,
            this.isSimpleItemDataGridViewCheckBoxColumn,
            this.isEtherealDataGridViewCheckBoxColumn,
            this.isPersonalizedDataGridViewCheckBoxColumn,
            this.isGambleDataGridViewCheckBoxColumn,
            this.isRunewordDataGridViewCheckBoxColumn,
            this.locationDataGridViewTextBoxColumn,
            this.positionOnBodyDataGridViewTextBoxColumn,
            this.positionXDataGridViewTextBoxColumn,
            this.positionYDataGridViewTextBoxColumn,
            this.storageIdDataGridViewTextBoxColumn,
            this.itemCodeDataGridViewTextBoxColumn,
            this.goldAmountDataGridViewTextBoxColumn,
            this.socketsFilledDataGridViewTextBoxColumn,
            this.idDataGridViewTextBoxColumn,
            this.levelDataGridViewTextBoxColumn1,
            this.qualityDataGridViewTextBoxColumn,
            this.hasGraphicDataGridViewCheckBoxColumn,
            this.graphicDataGridViewTextBoxColumn,
            this.uniqueSetIdDataGridViewTextBoxColumn,
            this.defenseDataGridViewTextBoxColumn,
            this.maxDurabilityDataGridViewTextBoxColumn,
            this.durabilityDataGridViewTextBoxColumn,
            this.isIndestructableDataGridViewCheckBoxColumn,
            this.socketCountDataGridViewTextBoxColumn,
            this.quantityDataGridViewTextBoxColumn,
            this.randomFlagDataGridViewCheckBoxColumn,
            this.unknownGoldFlagDataGridViewCheckBoxColumn,
            this.classFlagDataGridViewCheckBoxColumn,
            this.earClassDataGridViewTextBoxColumn,
            this.earLevelDataGridViewTextBoxColumn,
            this.prefixFlag0DataGridViewCheckBoxColumn,
            this.prefixFlag1DataGridViewCheckBoxColumn,
            this.prefixFlag2DataGridViewCheckBoxColumn,
            this.suffixFlag0DataGridViewCheckBoxColumn,
            this.suffixFlag1DataGridViewCheckBoxColumn,
            this.suffixFlag2DataGridViewCheckBoxColumn,
            this.prefix0DataGridViewTextBoxColumn,
            this.prefix1DataGridViewTextBoxColumn,
            this.prefix2DataGridViewTextBoxColumn,
            this.suffix0DataGridViewTextBoxColumn,
            this.suffix1DataGridViewTextBoxColumn,
            this.suffix2DataGridViewTextBoxColumn});
			this.dataGridView1.DataSource = this.itemBindingSource;
			this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.dataGridView1.Location = new System.Drawing.Point(3, 182);
			this.dataGridView1.Name = "dataGridView1";
			this.dataGridView1.Size = new System.Drawing.Size(618, 91);
			this.dataGridView1.TabIndex = 5;
			// 
			// itemBindingSource
			// 
			this.itemBindingSource.DataSource = typeof(esCharView.Item);
			// 
			// isEquippedDataGridViewCheckBoxColumn
			// 
			this.isEquippedDataGridViewCheckBoxColumn.DataPropertyName = "IsEquipped";
			this.isEquippedDataGridViewCheckBoxColumn.HeaderText = "IsEquipped";
			this.isEquippedDataGridViewCheckBoxColumn.Name = "isEquippedDataGridViewCheckBoxColumn";
			// 
			// isInSocketDataGridViewCheckBoxColumn
			// 
			this.isInSocketDataGridViewCheckBoxColumn.DataPropertyName = "IsInSocket";
			this.isInSocketDataGridViewCheckBoxColumn.HeaderText = "IsInSocket";
			this.isInSocketDataGridViewCheckBoxColumn.Name = "isInSocketDataGridViewCheckBoxColumn";
			// 
			// isIdentifiedDataGridViewCheckBoxColumn
			// 
			this.isIdentifiedDataGridViewCheckBoxColumn.DataPropertyName = "IsIdentified";
			this.isIdentifiedDataGridViewCheckBoxColumn.HeaderText = "IsIdentified";
			this.isIdentifiedDataGridViewCheckBoxColumn.Name = "isIdentifiedDataGridViewCheckBoxColumn";
			// 
			// isSwitchInDataGridViewCheckBoxColumn
			// 
			this.isSwitchInDataGridViewCheckBoxColumn.DataPropertyName = "IsSwitchIn";
			this.isSwitchInDataGridViewCheckBoxColumn.HeaderText = "IsSwitchIn";
			this.isSwitchInDataGridViewCheckBoxColumn.Name = "isSwitchInDataGridViewCheckBoxColumn";
			// 
			// isSwitchOutDataGridViewCheckBoxColumn
			// 
			this.isSwitchOutDataGridViewCheckBoxColumn.DataPropertyName = "IsSwitchOut";
			this.isSwitchOutDataGridViewCheckBoxColumn.HeaderText = "IsSwitchOut";
			this.isSwitchOutDataGridViewCheckBoxColumn.Name = "isSwitchOutDataGridViewCheckBoxColumn";
			// 
			// isBrokenDataGridViewCheckBoxColumn
			// 
			this.isBrokenDataGridViewCheckBoxColumn.DataPropertyName = "IsBroken";
			this.isBrokenDataGridViewCheckBoxColumn.HeaderText = "IsBroken";
			this.isBrokenDataGridViewCheckBoxColumn.Name = "isBrokenDataGridViewCheckBoxColumn";
			// 
			// isSocketedDataGridViewCheckBoxColumn
			// 
			this.isSocketedDataGridViewCheckBoxColumn.DataPropertyName = "IsSocketed";
			this.isSocketedDataGridViewCheckBoxColumn.HeaderText = "IsSocketed";
			this.isSocketedDataGridViewCheckBoxColumn.Name = "isSocketedDataGridViewCheckBoxColumn";
			// 
			// isStoreItemDataGridViewCheckBoxColumn
			// 
			this.isStoreItemDataGridViewCheckBoxColumn.DataPropertyName = "IsStoreItem";
			this.isStoreItemDataGridViewCheckBoxColumn.HeaderText = "IsStoreItem";
			this.isStoreItemDataGridViewCheckBoxColumn.Name = "isStoreItemDataGridViewCheckBoxColumn";
			// 
			// isEarDataGridViewCheckBoxColumn
			// 
			this.isEarDataGridViewCheckBoxColumn.DataPropertyName = "IsEar";
			this.isEarDataGridViewCheckBoxColumn.HeaderText = "IsEar";
			this.isEarDataGridViewCheckBoxColumn.Name = "isEarDataGridViewCheckBoxColumn";
			// 
			// isStarterItemDataGridViewCheckBoxColumn
			// 
			this.isStarterItemDataGridViewCheckBoxColumn.DataPropertyName = "IsStarterItem";
			this.isStarterItemDataGridViewCheckBoxColumn.HeaderText = "IsStarterItem";
			this.isStarterItemDataGridViewCheckBoxColumn.Name = "isStarterItemDataGridViewCheckBoxColumn";
			// 
			// isSimpleItemDataGridViewCheckBoxColumn
			// 
			this.isSimpleItemDataGridViewCheckBoxColumn.DataPropertyName = "IsSimpleItem";
			this.isSimpleItemDataGridViewCheckBoxColumn.HeaderText = "IsSimpleItem";
			this.isSimpleItemDataGridViewCheckBoxColumn.Name = "isSimpleItemDataGridViewCheckBoxColumn";
			// 
			// isEtherealDataGridViewCheckBoxColumn
			// 
			this.isEtherealDataGridViewCheckBoxColumn.DataPropertyName = "IsEthereal";
			this.isEtherealDataGridViewCheckBoxColumn.HeaderText = "IsEthereal";
			this.isEtherealDataGridViewCheckBoxColumn.Name = "isEtherealDataGridViewCheckBoxColumn";
			// 
			// isPersonalizedDataGridViewCheckBoxColumn
			// 
			this.isPersonalizedDataGridViewCheckBoxColumn.DataPropertyName = "IsPersonalized";
			this.isPersonalizedDataGridViewCheckBoxColumn.HeaderText = "IsPersonalized";
			this.isPersonalizedDataGridViewCheckBoxColumn.Name = "isPersonalizedDataGridViewCheckBoxColumn";
			// 
			// isGambleDataGridViewCheckBoxColumn
			// 
			this.isGambleDataGridViewCheckBoxColumn.DataPropertyName = "IsGamble";
			this.isGambleDataGridViewCheckBoxColumn.HeaderText = "IsGamble";
			this.isGambleDataGridViewCheckBoxColumn.Name = "isGambleDataGridViewCheckBoxColumn";
			// 
			// isRunewordDataGridViewCheckBoxColumn
			// 
			this.isRunewordDataGridViewCheckBoxColumn.DataPropertyName = "IsRuneword";
			this.isRunewordDataGridViewCheckBoxColumn.HeaderText = "IsRuneword";
			this.isRunewordDataGridViewCheckBoxColumn.Name = "isRunewordDataGridViewCheckBoxColumn";
			// 
			// locationDataGridViewTextBoxColumn
			// 
			this.locationDataGridViewTextBoxColumn.DataPropertyName = "Location";
			this.locationDataGridViewTextBoxColumn.HeaderText = "Location";
			this.locationDataGridViewTextBoxColumn.Name = "locationDataGridViewTextBoxColumn";
			// 
			// positionOnBodyDataGridViewTextBoxColumn
			// 
			this.positionOnBodyDataGridViewTextBoxColumn.DataPropertyName = "PositionOnBody";
			this.positionOnBodyDataGridViewTextBoxColumn.HeaderText = "PositionOnBody";
			this.positionOnBodyDataGridViewTextBoxColumn.Name = "positionOnBodyDataGridViewTextBoxColumn";
			// 
			// positionXDataGridViewTextBoxColumn
			// 
			this.positionXDataGridViewTextBoxColumn.DataPropertyName = "PositionX";
			this.positionXDataGridViewTextBoxColumn.HeaderText = "PositionX";
			this.positionXDataGridViewTextBoxColumn.Name = "positionXDataGridViewTextBoxColumn";
			// 
			// positionYDataGridViewTextBoxColumn
			// 
			this.positionYDataGridViewTextBoxColumn.DataPropertyName = "PositionY";
			this.positionYDataGridViewTextBoxColumn.HeaderText = "PositionY";
			this.positionYDataGridViewTextBoxColumn.Name = "positionYDataGridViewTextBoxColumn";
			// 
			// storageIdDataGridViewTextBoxColumn
			// 
			this.storageIdDataGridViewTextBoxColumn.DataPropertyName = "StorageId";
			this.storageIdDataGridViewTextBoxColumn.HeaderText = "StorageId";
			this.storageIdDataGridViewTextBoxColumn.Name = "storageIdDataGridViewTextBoxColumn";
			// 
			// itemCodeDataGridViewTextBoxColumn
			// 
			this.itemCodeDataGridViewTextBoxColumn.DataPropertyName = "ItemCode";
			this.itemCodeDataGridViewTextBoxColumn.HeaderText = "ItemCode";
			this.itemCodeDataGridViewTextBoxColumn.Name = "itemCodeDataGridViewTextBoxColumn";
			// 
			// goldAmountDataGridViewTextBoxColumn
			// 
			this.goldAmountDataGridViewTextBoxColumn.DataPropertyName = "GoldAmount";
			this.goldAmountDataGridViewTextBoxColumn.HeaderText = "GoldAmount";
			this.goldAmountDataGridViewTextBoxColumn.Name = "goldAmountDataGridViewTextBoxColumn";
			// 
			// socketsFilledDataGridViewTextBoxColumn
			// 
			this.socketsFilledDataGridViewTextBoxColumn.DataPropertyName = "SocketsFilled";
			this.socketsFilledDataGridViewTextBoxColumn.HeaderText = "SocketsFilled";
			this.socketsFilledDataGridViewTextBoxColumn.Name = "socketsFilledDataGridViewTextBoxColumn";
			// 
			// idDataGridViewTextBoxColumn
			// 
			this.idDataGridViewTextBoxColumn.DataPropertyName = "Id";
			this.idDataGridViewTextBoxColumn.HeaderText = "Id";
			this.idDataGridViewTextBoxColumn.Name = "idDataGridViewTextBoxColumn";
			// 
			// levelDataGridViewTextBoxColumn1
			// 
			this.levelDataGridViewTextBoxColumn1.DataPropertyName = "Level";
			this.levelDataGridViewTextBoxColumn1.HeaderText = "Level";
			this.levelDataGridViewTextBoxColumn1.Name = "levelDataGridViewTextBoxColumn1";
			// 
			// qualityDataGridViewTextBoxColumn
			// 
			this.qualityDataGridViewTextBoxColumn.DataPropertyName = "Quality";
			this.qualityDataGridViewTextBoxColumn.HeaderText = "Quality";
			this.qualityDataGridViewTextBoxColumn.Name = "qualityDataGridViewTextBoxColumn";
			// 
			// hasGraphicDataGridViewCheckBoxColumn
			// 
			this.hasGraphicDataGridViewCheckBoxColumn.DataPropertyName = "HasGraphic";
			this.hasGraphicDataGridViewCheckBoxColumn.HeaderText = "HasGraphic";
			this.hasGraphicDataGridViewCheckBoxColumn.Name = "hasGraphicDataGridViewCheckBoxColumn";
			// 
			// graphicDataGridViewTextBoxColumn
			// 
			this.graphicDataGridViewTextBoxColumn.DataPropertyName = "Graphic";
			this.graphicDataGridViewTextBoxColumn.HeaderText = "Graphic";
			this.graphicDataGridViewTextBoxColumn.Name = "graphicDataGridViewTextBoxColumn";
			// 
			// uniqueSetIdDataGridViewTextBoxColumn
			// 
			this.uniqueSetIdDataGridViewTextBoxColumn.DataPropertyName = "UniqueSetId";
			this.uniqueSetIdDataGridViewTextBoxColumn.HeaderText = "UniqueSetId";
			this.uniqueSetIdDataGridViewTextBoxColumn.Name = "uniqueSetIdDataGridViewTextBoxColumn";
			// 
			// defenseDataGridViewTextBoxColumn
			// 
			this.defenseDataGridViewTextBoxColumn.DataPropertyName = "Defense";
			this.defenseDataGridViewTextBoxColumn.HeaderText = "Defense";
			this.defenseDataGridViewTextBoxColumn.Name = "defenseDataGridViewTextBoxColumn";
			// 
			// maxDurabilityDataGridViewTextBoxColumn
			// 
			this.maxDurabilityDataGridViewTextBoxColumn.DataPropertyName = "MaxDurability";
			this.maxDurabilityDataGridViewTextBoxColumn.HeaderText = "MaxDurability";
			this.maxDurabilityDataGridViewTextBoxColumn.Name = "maxDurabilityDataGridViewTextBoxColumn";
			// 
			// durabilityDataGridViewTextBoxColumn
			// 
			this.durabilityDataGridViewTextBoxColumn.DataPropertyName = "Durability";
			this.durabilityDataGridViewTextBoxColumn.HeaderText = "Durability";
			this.durabilityDataGridViewTextBoxColumn.Name = "durabilityDataGridViewTextBoxColumn";
			// 
			// isIndestructableDataGridViewCheckBoxColumn
			// 
			this.isIndestructableDataGridViewCheckBoxColumn.DataPropertyName = "IsIndestructable";
			this.isIndestructableDataGridViewCheckBoxColumn.HeaderText = "IsIndestructable";
			this.isIndestructableDataGridViewCheckBoxColumn.Name = "isIndestructableDataGridViewCheckBoxColumn";
			// 
			// socketCountDataGridViewTextBoxColumn
			// 
			this.socketCountDataGridViewTextBoxColumn.DataPropertyName = "SocketCount";
			this.socketCountDataGridViewTextBoxColumn.HeaderText = "SocketCount";
			this.socketCountDataGridViewTextBoxColumn.Name = "socketCountDataGridViewTextBoxColumn";
			// 
			// quantityDataGridViewTextBoxColumn
			// 
			this.quantityDataGridViewTextBoxColumn.DataPropertyName = "Quantity";
			this.quantityDataGridViewTextBoxColumn.HeaderText = "Quantity";
			this.quantityDataGridViewTextBoxColumn.Name = "quantityDataGridViewTextBoxColumn";
			// 
			// randomFlagDataGridViewCheckBoxColumn
			// 
			this.randomFlagDataGridViewCheckBoxColumn.DataPropertyName = "RandomFlag";
			this.randomFlagDataGridViewCheckBoxColumn.HeaderText = "RandomFlag";
			this.randomFlagDataGridViewCheckBoxColumn.Name = "randomFlagDataGridViewCheckBoxColumn";
			// 
			// unknownGoldFlagDataGridViewCheckBoxColumn
			// 
			this.unknownGoldFlagDataGridViewCheckBoxColumn.DataPropertyName = "UnknownGoldFlag";
			this.unknownGoldFlagDataGridViewCheckBoxColumn.HeaderText = "UnknownGoldFlag";
			this.unknownGoldFlagDataGridViewCheckBoxColumn.Name = "unknownGoldFlagDataGridViewCheckBoxColumn";
			// 
			// classFlagDataGridViewCheckBoxColumn
			// 
			this.classFlagDataGridViewCheckBoxColumn.DataPropertyName = "ClassFlag";
			this.classFlagDataGridViewCheckBoxColumn.HeaderText = "ClassFlag";
			this.classFlagDataGridViewCheckBoxColumn.Name = "classFlagDataGridViewCheckBoxColumn";
			// 
			// earClassDataGridViewTextBoxColumn
			// 
			this.earClassDataGridViewTextBoxColumn.DataPropertyName = "EarClass";
			this.earClassDataGridViewTextBoxColumn.HeaderText = "EarClass";
			this.earClassDataGridViewTextBoxColumn.Name = "earClassDataGridViewTextBoxColumn";
			// 
			// earLevelDataGridViewTextBoxColumn
			// 
			this.earLevelDataGridViewTextBoxColumn.DataPropertyName = "EarLevel";
			this.earLevelDataGridViewTextBoxColumn.HeaderText = "EarLevel";
			this.earLevelDataGridViewTextBoxColumn.Name = "earLevelDataGridViewTextBoxColumn";
			// 
			// prefixFlag0DataGridViewCheckBoxColumn
			// 
			this.prefixFlag0DataGridViewCheckBoxColumn.DataPropertyName = "PrefixFlag0";
			this.prefixFlag0DataGridViewCheckBoxColumn.HeaderText = "PrefixFlag0";
			this.prefixFlag0DataGridViewCheckBoxColumn.Name = "prefixFlag0DataGridViewCheckBoxColumn";
			// 
			// prefixFlag1DataGridViewCheckBoxColumn
			// 
			this.prefixFlag1DataGridViewCheckBoxColumn.DataPropertyName = "PrefixFlag1";
			this.prefixFlag1DataGridViewCheckBoxColumn.HeaderText = "PrefixFlag1";
			this.prefixFlag1DataGridViewCheckBoxColumn.Name = "prefixFlag1DataGridViewCheckBoxColumn";
			// 
			// prefixFlag2DataGridViewCheckBoxColumn
			// 
			this.prefixFlag2DataGridViewCheckBoxColumn.DataPropertyName = "PrefixFlag2";
			this.prefixFlag2DataGridViewCheckBoxColumn.HeaderText = "PrefixFlag2";
			this.prefixFlag2DataGridViewCheckBoxColumn.Name = "prefixFlag2DataGridViewCheckBoxColumn";
			// 
			// suffixFlag0DataGridViewCheckBoxColumn
			// 
			this.suffixFlag0DataGridViewCheckBoxColumn.DataPropertyName = "SuffixFlag0";
			this.suffixFlag0DataGridViewCheckBoxColumn.HeaderText = "SuffixFlag0";
			this.suffixFlag0DataGridViewCheckBoxColumn.Name = "suffixFlag0DataGridViewCheckBoxColumn";
			// 
			// suffixFlag1DataGridViewCheckBoxColumn
			// 
			this.suffixFlag1DataGridViewCheckBoxColumn.DataPropertyName = "SuffixFlag1";
			this.suffixFlag1DataGridViewCheckBoxColumn.HeaderText = "SuffixFlag1";
			this.suffixFlag1DataGridViewCheckBoxColumn.Name = "suffixFlag1DataGridViewCheckBoxColumn";
			// 
			// suffixFlag2DataGridViewCheckBoxColumn
			// 
			this.suffixFlag2DataGridViewCheckBoxColumn.DataPropertyName = "SuffixFlag2";
			this.suffixFlag2DataGridViewCheckBoxColumn.HeaderText = "SuffixFlag2";
			this.suffixFlag2DataGridViewCheckBoxColumn.Name = "suffixFlag2DataGridViewCheckBoxColumn";
			// 
			// prefix0DataGridViewTextBoxColumn
			// 
			this.prefix0DataGridViewTextBoxColumn.DataPropertyName = "Prefix0";
			this.prefix0DataGridViewTextBoxColumn.HeaderText = "Prefix0";
			this.prefix0DataGridViewTextBoxColumn.Name = "prefix0DataGridViewTextBoxColumn";
			// 
			// prefix1DataGridViewTextBoxColumn
			// 
			this.prefix1DataGridViewTextBoxColumn.DataPropertyName = "Prefix1";
			this.prefix1DataGridViewTextBoxColumn.HeaderText = "Prefix1";
			this.prefix1DataGridViewTextBoxColumn.Name = "prefix1DataGridViewTextBoxColumn";
			// 
			// prefix2DataGridViewTextBoxColumn
			// 
			this.prefix2DataGridViewTextBoxColumn.DataPropertyName = "Prefix2";
			this.prefix2DataGridViewTextBoxColumn.HeaderText = "Prefix2";
			this.prefix2DataGridViewTextBoxColumn.Name = "prefix2DataGridViewTextBoxColumn";
			// 
			// suffix0DataGridViewTextBoxColumn
			// 
			this.suffix0DataGridViewTextBoxColumn.DataPropertyName = "Suffix0";
			this.suffix0DataGridViewTextBoxColumn.HeaderText = "Suffix0";
			this.suffix0DataGridViewTextBoxColumn.Name = "suffix0DataGridViewTextBoxColumn";
			// 
			// suffix1DataGridViewTextBoxColumn
			// 
			this.suffix1DataGridViewTextBoxColumn.DataPropertyName = "Suffix1";
			this.suffix1DataGridViewTextBoxColumn.HeaderText = "Suffix1";
			this.suffix1DataGridViewTextBoxColumn.Name = "suffix1DataGridViewTextBoxColumn";
			// 
			// suffix2DataGridViewTextBoxColumn
			// 
			this.suffix2DataGridViewTextBoxColumn.DataPropertyName = "Suffix2";
			this.suffix2DataGridViewTextBoxColumn.HeaderText = "Suffix2";
			this.suffix2DataGridViewTextBoxColumn.Name = "suffix2DataGridViewTextBoxColumn";
			// 
			// buttonItemViewerRefresh
			// 
			this.buttonItemViewerRefresh.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonItemViewerRefresh.Location = new System.Drawing.Point(252, 12);
			this.buttonItemViewerRefresh.Name = "buttonItemViewerRefresh";
			this.buttonItemViewerRefresh.Size = new System.Drawing.Size(75, 23);
			this.buttonItemViewerRefresh.TabIndex = 6;
			this.buttonItemViewerRefresh.Text = "Refresh";
			this.buttonItemViewerRefresh.UseVisualStyleBackColor = true;
			this.buttonItemViewerRefresh.Click += new System.EventHandler(this.buttonItemViewerRefresh_Click);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(632, 343);
			this.Controls.Add(this.tabControl1);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.textBoxUnknownFlags);
			this.Controls.Add(this.textBoxName);
			this.Controls.Add(this.checkBoxDied);
			this.Controls.Add(this.checkBoxHardcore);
			this.Controls.Add(this.checkBoxExpansion);
			this.Controls.Add(this.buttonSave);
			this.Controls.Add(this.buttonLoadCharacter);
			this.MinimumSize = new System.Drawing.Size(600, 250);
			this.Name = "Form1";
			this.Text = "D2ES Save Fix";
			this.tabControlInventory.ResumeLayout(false);
			this.tabPageInventoryPlayer.ResumeLayout(false);
			this.tabPageInventoryCorpse.ResumeLayout(false);
			this.tabPageInventoryMerc.ResumeLayout(false);
			this.tabPageInventoryGolem.ResumeLayout(false);
			this.tabControl1.ResumeLayout(false);
			this.tabPageGeneral.ResumeLayout(false);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewCharacter)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.characterBindingSource)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewStats)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.statBindingSource)).EndInit();
			this.tabPageInventoryEditor.ResumeLayout(false);
			this.tabPageItemEditor.ResumeLayout(false);
			this.tabPageItemEditor.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.itemBindingSource)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button buttonLoadCharacter;
		private System.Windows.Forms.ListBox listBoxInventory;
		private System.Windows.Forms.OpenFileDialog openFileDialog2;
		private System.Windows.Forms.Button buttonRemoveItem;
		private System.Windows.Forms.Button buttonSave;
		private System.Windows.Forms.SaveFileDialog saveFileDialog1;
		private System.Windows.Forms.CheckBox checkBoxExpansion;
		private System.Windows.Forms.CheckBox checkBoxHardcore;
		private System.Windows.Forms.CheckBox checkBoxDied;
		private System.Windows.Forms.TextBox textBoxName;
		private System.Windows.Forms.TextBox textBoxUnknownFlags;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TabControl tabControlInventory;
		private System.Windows.Forms.TabPage tabPageInventoryPlayer;
		private System.Windows.Forms.TabPage tabPageInventoryCorpse;
		private System.Windows.Forms.ListBox listBoxCorpseInventory;
		private System.Windows.Forms.TabPage tabPageInventoryMerc;
		private System.Windows.Forms.ListBox listBoxMercInventory;
		private System.Windows.Forms.TabPage tabPageInventoryGolem;
		private System.Windows.Forms.ListBox listBoxGolemInventory;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPageGeneral;
		private System.Windows.Forms.TabPage tabPageInventoryEditor;
		private System.Windows.Forms.TabPage tabPageItemEditor;
		private System.Windows.Forms.TextBox textBoxItemEditor;
		private System.Windows.Forms.Button buttonRemoveSocket;
		private System.Windows.Forms.ListBox listBoxItemEditorSockets;
		private System.Windows.Forms.Label labelItemName;
		private System.Windows.Forms.DataGridView dataGridViewCharacter;
		private System.Windows.Forms.DataGridViewTextBoxColumn nameDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn hardcoreDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn diedDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn expansionDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn unknownFlagsDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn progressionDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn classDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn levelDisplayDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn hasMercenaryDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn mercenaryNameIdDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn mercenaryTypeDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn mercenaryExpDataGridViewTextBoxColumn;
		private System.Windows.Forms.BindingSource characterBindingSource;
		private System.Windows.Forms.DataGridView dataGridViewStats;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.DataGridViewTextBoxColumn strengthDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn energyDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn dexterityDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn vitalityDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn statPointsDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn skillPointsDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn hitpointsDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn baseHitpointsDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn manaDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn baseManaDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn staminaDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn baseStaminaDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn levelDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn experienceDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn goldDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn goldBankDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn killCountDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn deathCountDataGridViewTextBoxColumn;
		private System.Windows.Forms.BindingSource statBindingSource;
		private System.Windows.Forms.DataGridView dataGridView1;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isEquippedDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isInSocketDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isIdentifiedDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isSwitchInDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isSwitchOutDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isBrokenDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isSocketedDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isStoreItemDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isEarDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isStarterItemDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isSimpleItemDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isEtherealDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isPersonalizedDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isGambleDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isRunewordDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn locationDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn positionOnBodyDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn positionXDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn positionYDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn storageIdDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn itemCodeDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn goldAmountDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn socketsFilledDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn idDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn levelDataGridViewTextBoxColumn1;
		private System.Windows.Forms.DataGridViewTextBoxColumn qualityDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn hasGraphicDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn graphicDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn uniqueSetIdDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn defenseDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn maxDurabilityDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn durabilityDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn isIndestructableDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn socketCountDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn quantityDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn randomFlagDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn unknownGoldFlagDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn classFlagDataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn earClassDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn earLevelDataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn prefixFlag0DataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn prefixFlag1DataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn prefixFlag2DataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn suffixFlag0DataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn suffixFlag1DataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewCheckBoxColumn suffixFlag2DataGridViewCheckBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn prefix0DataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn prefix1DataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn prefix2DataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn suffix0DataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn suffix1DataGridViewTextBoxColumn;
		private System.Windows.Forms.DataGridViewTextBoxColumn suffix2DataGridViewTextBoxColumn;
		private System.Windows.Forms.BindingSource itemBindingSource;
		private System.Windows.Forms.Button buttonItemViewerRefresh;

	}
}

