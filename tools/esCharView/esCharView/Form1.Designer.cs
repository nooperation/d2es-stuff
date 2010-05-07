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
			this.buttonLoadCharacter = new System.Windows.Forms.Button();
			this.listBoxInventory = new System.Windows.Forms.ListBox();
			this.openFileDialog2 = new System.Windows.Forms.OpenFileDialog();
			this.textBoxItemLocation = new System.Windows.Forms.TextBox();
			this.buttonRemoveItem = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
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
			this.tabControlInventory.SuspendLayout();
			this.tabPageInventoryPlayer.SuspendLayout();
			this.tabPageInventoryCorpse.SuspendLayout();
			this.tabPageInventoryMerc.SuspendLayout();
			this.tabPageInventoryGolem.SuspendLayout();
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
			this.listBoxInventory.Size = new System.Drawing.Size(582, 198);
			this.listBoxInventory.TabIndex = 2;
			this.listBoxInventory.SelectedIndexChanged += new System.EventHandler(this.listBoxInventory_SelectedIndexChanged);
			// 
			// openFileDialog2
			// 
			this.openFileDialog2.Filter = "d2s files|*.d2s|All files|*.*";
			this.openFileDialog2.InitialDirectory = ".";
			// 
			// textBoxItemLocation
			// 
			this.textBoxItemLocation.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxItemLocation.Location = new System.Drawing.Point(53, 279);
			this.textBoxItemLocation.Name = "textBoxItemLocation";
			this.textBoxItemLocation.ReadOnly = true;
			this.textBoxItemLocation.Size = new System.Drawing.Size(424, 20);
			this.textBoxItemLocation.TabIndex = 3;
			// 
			// buttonRemoveItem
			// 
			this.buttonRemoveItem.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonRemoveItem.Location = new System.Drawing.Point(536, 277);
			this.buttonRemoveItem.Name = "buttonRemoveItem";
			this.buttonRemoveItem.Size = new System.Drawing.Size(75, 23);
			this.buttonRemoveItem.TabIndex = 4;
			this.buttonRemoveItem.Text = "Remove";
			this.buttonRemoveItem.UseVisualStyleBackColor = true;
			this.buttonRemoveItem.Click += new System.EventHandler(this.buttonRemoveItem_Click);
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 282);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(35, 13);
			this.label1.TabIndex = 5;
			this.label1.Text = "Offset";
			// 
			// buttonSave
			// 
			this.buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonSave.Enabled = false;
			this.buttonSave.Location = new System.Drawing.Point(536, 12);
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
			this.checkBoxExpansion.Location = new System.Drawing.Point(256, 15);
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
			this.checkBoxHardcore.Location = new System.Drawing.Point(337, 15);
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
			this.checkBoxDied.Location = new System.Drawing.Point(408, 15);
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
			this.textBoxName.Size = new System.Drawing.Size(137, 20);
			this.textBoxName.TabIndex = 11;
			// 
			// textBoxUnknownFlags
			// 
			this.textBoxUnknownFlags.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxUnknownFlags.Location = new System.Drawing.Point(503, 13);
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
			this.label2.Location = new System.Drawing.Point(464, 17);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(33, 13);
			this.label2.TabIndex = 13;
			this.label2.Text = "Other";
			// 
			// tabControlInventory
			// 
			this.tabControlInventory.Controls.Add(this.tabPageInventoryPlayer);
			this.tabControlInventory.Controls.Add(this.tabPageInventoryCorpse);
			this.tabControlInventory.Controls.Add(this.tabPageInventoryMerc);
			this.tabControlInventory.Controls.Add(this.tabPageInventoryGolem);
			this.tabControlInventory.Location = new System.Drawing.Point(15, 41);
			this.tabControlInventory.Name = "tabControlInventory";
			this.tabControlInventory.SelectedIndex = 0;
			this.tabControlInventory.Size = new System.Drawing.Size(596, 230);
			this.tabControlInventory.TabIndex = 14;
			// 
			// tabPageInventoryPlayer
			// 
			this.tabPageInventoryPlayer.Controls.Add(this.listBoxInventory);
			this.tabPageInventoryPlayer.Location = new System.Drawing.Point(4, 22);
			this.tabPageInventoryPlayer.Name = "tabPageInventoryPlayer";
			this.tabPageInventoryPlayer.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageInventoryPlayer.Size = new System.Drawing.Size(588, 204);
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
			this.tabPageInventoryCorpse.Size = new System.Drawing.Size(588, 204);
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
			this.listBoxCorpseInventory.Size = new System.Drawing.Size(582, 198);
			this.listBoxCorpseInventory.TabIndex = 0;
			// 
			// tabPageInventoryMerc
			// 
			this.tabPageInventoryMerc.Controls.Add(this.listBoxMercInventory);
			this.tabPageInventoryMerc.Location = new System.Drawing.Point(4, 22);
			this.tabPageInventoryMerc.Name = "tabPageInventoryMerc";
			this.tabPageInventoryMerc.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageInventoryMerc.Size = new System.Drawing.Size(588, 204);
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
			this.listBoxMercInventory.Size = new System.Drawing.Size(582, 198);
			this.listBoxMercInventory.TabIndex = 0;
			// 
			// tabPageInventoryGolem
			// 
			this.tabPageInventoryGolem.Controls.Add(this.listBoxGolemInventory);
			this.tabPageInventoryGolem.Location = new System.Drawing.Point(4, 22);
			this.tabPageInventoryGolem.Name = "tabPageInventoryGolem";
			this.tabPageInventoryGolem.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageInventoryGolem.Size = new System.Drawing.Size(588, 204);
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
			this.listBoxGolemInventory.Size = new System.Drawing.Size(582, 198);
			this.listBoxGolemInventory.TabIndex = 0;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(623, 309);
			this.Controls.Add(this.tabControlInventory);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.textBoxUnknownFlags);
			this.Controls.Add(this.textBoxName);
			this.Controls.Add(this.checkBoxDied);
			this.Controls.Add(this.checkBoxHardcore);
			this.Controls.Add(this.checkBoxExpansion);
			this.Controls.Add(this.buttonSave);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.buttonRemoveItem);
			this.Controls.Add(this.textBoxItemLocation);
			this.Controls.Add(this.buttonLoadCharacter);
			this.Name = "Form1";
			this.Text = "D2ES Save Fix";
			this.tabControlInventory.ResumeLayout(false);
			this.tabPageInventoryPlayer.ResumeLayout(false);
			this.tabPageInventoryCorpse.ResumeLayout(false);
			this.tabPageInventoryMerc.ResumeLayout(false);
			this.tabPageInventoryGolem.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button buttonLoadCharacter;
		private System.Windows.Forms.ListBox listBoxInventory;
		private System.Windows.Forms.OpenFileDialog openFileDialog2;
		private System.Windows.Forms.TextBox textBoxItemLocation;
		private System.Windows.Forms.Button buttonRemoveItem;
		private System.Windows.Forms.Label label1;
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

	}
}

