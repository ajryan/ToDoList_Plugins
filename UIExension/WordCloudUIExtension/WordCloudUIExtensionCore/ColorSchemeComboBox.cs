﻿using System;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Abstractspoon.Tdl.PluginHelpers;

namespace WordCloudUIExtension
{
	[System.ComponentModel.DesignerCategory("")]
	class ColorSchemeComboBox : ComboBox
	{
		protected class ColorScheme
		{
			public ColorScheme(Color[] colors)
			{
				Colors = new List<Color>();

				foreach (Color color in colors)
					Colors.Add(color);
			}

			public ColorScheme(String[] hexColors)
			{
				Colors = new List<Color>();

				FromHexColors(hexColors);
			}

			public override string ToString()
			{
				StringBuilder hex = new StringBuilder();

				foreach (Color color in Colors)
				{
					hex.Append(ColorTranslator.ToHtml(color));
					hex.Append(',');
				}
				
				return hex.ToString();
			}

			private bool FromHexColors(String[] hexColors)
			{
				Colors.Clear();

				foreach (String color in hexColors)
				{
					if ((color.Length != 0) && (color[0] == '#'))
					{
						Color c = ColorTranslator.FromHtml(color);

						if (c != null)
							Colors.Add(c);
					}
				}

				return (Colors.Count > 0);
			}

			public List<Color> Colors { private set; get; }
		}

		public ColorSchemeComboBox()
		{
			Initialise();

			this.DropDownStyle = ComboBoxStyle.DropDownList;
			this.DrawMode = DrawMode.OwnerDrawFixed;
			this.MaxDropDownItems = 20;
			this.DropDownHeight = 200;
			this.ForeColor = Color.White;
			this.DoubleBuffered = true;
		}

		public void Initialise()
		{
			Items.Add(new ColorScheme(new String[] { "#CC0000", "#99FF00", "#FFCC00", "#3333FF" }));
			Items.Add(new ColorScheme(new String[] { "#76988F", "#AC8C18", "#3F3C29", "#262416" }));
			Items.Add(new ColorScheme(new String[] { "#097054", "#FFDE00", "#6599FF", "#FF9900" }));
			Items.Add(new ColorScheme(new String[] { "#FF3333", "#999999", "#669999", "#003333" }));
			Items.Add(new ColorScheme(new String[] { "#663300", "#CC9900", "#333300", "#CC6600" }));
			Items.Add(new ColorScheme(new String[] { "#FFD800", "#587058", "#587498", "#E86850" }));
			Items.Add(new ColorScheme(new String[] { "#669966", "#336699", "#FFFF00", "#990033" }));
			Items.Add(new ColorScheme(new String[] { "#7A3E48", "#EECD86", "#E18942", "#B95835", "#3D3242" }));
			Items.Add(new ColorScheme(new String[] { "#020731", "#3862C6", "#6E7587", "#806641", "#AE956D" }));
			Items.Add(new ColorScheme(new String[] { "#EBC137", "#E38C2D", "#DB4C2C", "#771E10", "#48110C" }));
			Items.Add(new ColorScheme(new String[] { "#EF597B", "#FF6D31", "#73B66B", "#FFCB18", "#29A2C6" }));
			Items.Add(new ColorScheme(new String[] { "#7EB5D6", "#2A75A9", "#274257", "#DFC184", "#8F6048", "#644436" }));
			Items.Add(new ColorScheme(new String[] { "#21B6A8", "#177F75", "#B6212D", "#7F171F", "#B67721", "#7F5417" }));
			Items.Add(new ColorScheme(new String[] { "#9DAF72", "#566047", "#562F32", "#462D44", "#859731", "#640E27" }));
			Items.Add(new ColorScheme(new String[] { "#00904B", "#64BD4F", "#E13987", "#F191BA", "#FEEA65", "#835322" }));
			Items.Add(new ColorScheme(new Color[] { Color.DarkRed, Color.DarkBlue, Color.DarkGreen, Color.Navy, Color.DarkCyan, Color.DarkOrange, Color.DarkGoldenrod, Color.DarkKhaki, Color.Blue, Color.Red, Color.Green }));
// 			Items.Add(new ColorScheme(new String[] { "", "", "", "", "", "", "", "", "", "", "" }));
// 			Items.Add(new ColorScheme(new String[] { "", "", "", "", "", "", "", "", "", "", "" }));

			SelectedIndex = -1;
		}

		protected override void OnDrawItem(DrawItemEventArgs e)
		{
			base.OnDrawItem(e);
			
			e.DrawBackground();
			e.DrawFocusRectangle();

			if (e.Index > -1)
			{
				ColorScheme scheme = (ColorScheme)Items[e.Index];
				
				if (scheme != null)
				{
					Rectangle rect = new Rectangle(e.Bounds.Location, e.Bounds.Size);
					rect.Inflate(-2, -2);
					rect.Width = rect.Height;

					foreach (Color color in scheme.Colors)
					{
						using (Brush fill = new SolidBrush(color))
						{
							e.Graphics.FillRectangle(fill, rect);
						}

						using (Pen border = new Pen(ColorUtil.DarkerDrawing(color, 0.2f)))
						{
							e.Graphics.DrawRectangle(border, rect);
						}

						// Next colour
						rect.Offset(rect.Height + 2, 0);
					}
				}
			}
		}

		public String GetSelectedSchemeAsString()
		{
			ColorScheme selItem = (ColorScheme)SelectedItem;

			if (selItem == null)
				return "";

			// else
			return selItem.ToString();
		}

		public Color[] GetSelectedScheme()
		{
			ColorScheme selItem = (ColorScheme)SelectedItem;

			if (selItem == null)
				return new Color[] { Color.Black };

			return selItem.Colors.ToArray();
		}

		public bool SetSelectedScheme(Color[] colors)
		{
			var item = FindScheme(colors);

			if (item == null)
			{
				// Create a new scheme for this arrangement
				item = new ColorScheme(colors);
				Items.Add(item);
			}

			SelectedItem = item;
			return true;
		}

		public bool SetSelectedScheme(String hex)
		{
			string[] hexColors = hex.Split(',');

			ColorScheme scheme = new ColorScheme(hexColors);

			if (scheme.Colors.Count == 0)
				return false;

			return SetSelectedScheme(scheme.Colors.ToArray());
		}

		protected ColorScheme FindScheme(Color[] colors)
		{
			if (colors.Length > 0)
			{
				foreach (ColorScheme scheme in Items)
				{
					if (colors.Length == scheme.Colors.Count)
					{
						int numMatches = 0;

						foreach (Color color in colors)
						{
							if (!scheme.Colors.Contains(color))
								break;

							// else
							numMatches++;
						}

						if (numMatches == colors.Length)
							return scheme;
					}
				}
			}

			// else
			return null;
		}
	}
}
