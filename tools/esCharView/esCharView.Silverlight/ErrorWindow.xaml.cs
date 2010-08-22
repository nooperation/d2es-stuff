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

namespace esCharView.Silverlight
{
	public partial class ErrorWindow : ChildWindow
	{
		public ErrorWindow()
		{
			InitializeComponent();
		}

		public ErrorWindow(string message, bool showInstantly = false)
		{
			InitializeComponent();
			errorMessage.Text = message;
			this.Show();
		}

		private void OKButton_Click(object sender, RoutedEventArgs e)
		{
			this.DialogResult = true;
		}

		private void CancelButton_Click(object sender, RoutedEventArgs e)
		{
			this.DialogResult = false;
		}

		private void ChildWindow_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.Key == Key.Escape || e.Key == Key.Space || e.Key == Key.Enter)
			{
				// Bug in Silverlight4 when closing with Close, parent form will stay disabled
				// http://forums.silverlight.net/forums/p/176423/449910.aspx
				Application.Current.RootVisual.SetValue(Control.IsEnabledProperty, true);

				this.Close();
			}
		}
	}
}

