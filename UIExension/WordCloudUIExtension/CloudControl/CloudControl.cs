﻿using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using Gma.CodeCloud.Controls.Geometry;
using Gma.CodeCloud.Controls.TextAnalyses.Processing;

namespace Gma.CodeCloud.Controls
{
    public class CloudControl : Panel
    {
        private IEnumerable<IWord> m_Words;
        private readonly Color[] m_DefaultPalette = new[] { Color.DarkRed, Color.DarkBlue, Color.DarkGreen, Color.Navy, Color.DarkCyan, Color.DarkOrange, Color.DarkGoldenrod, Color.DarkKhaki, Color.Blue, Color.Red, Color.Green };
        private Color[] m_Palette;
        private LayoutType m_LayoutType;
		private int m_MaxFontSize;
		private int m_MinFontSize;
		private ILayout m_Layout;
		private Color m_BackColor;
		private int m_MinWordWeight;
		private int m_MaxWordWeight;
		
        protected LayoutItem m_ItemUnderMouse;

        public CloudControl()
        {
            m_MinWordWeight = 0;
            m_MaxWordWeight = 0;

            MaxFontSize = 68;
            MinFontSize = 6;
           
            this.BorderStyle = BorderStyle.FixedSingle;
            this.ResizeRedraw = true;
            
            m_Palette = m_DefaultPalette;
            m_BackColor = Color.White;
            m_LayoutType = LayoutType.Spiral;
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (m_Words == null || !m_Words.Any()) { return; }
            if (m_Layout == null) { return; }

            IEnumerable<LayoutItem> wordsToRedraw = m_Layout.GetWordsInArea(e.ClipRectangle);

            using (IGraphicEngine graphicEngine =
                    NewGraphicEngine(e.Graphics, this.Font.FontFamily, FontStyle.Regular, m_Palette, MinFontSize, MaxFontSize, m_MinWordWeight, m_MaxWordWeight))
            {
                foreach (LayoutItem currentItem in wordsToRedraw)
                {
                    if (m_ItemUnderMouse == currentItem)
                    {
                        graphicEngine.DrawEmphasized(currentItem);
                    }
                    else
                    {
                        graphicEngine.Draw(currentItem);                        
                    }
                }
            }
        }

        protected virtual IGraphicEngine NewGraphicEngine(Graphics graphics, FontFamily fontFamily, FontStyle fontStyle, Color[] palette, float minFontSize, float maxFontSize, int minWordWeight, int maxWordWeight)
        {
            return new GdiGraphicEngine(graphics, this.Font.FontFamily, FontStyle.Regular, palette, minFontSize, maxFontSize, minWordWeight, maxWordWeight);
        }

        private void BuildLayout()
        {
            if (m_Words == null || !m_Words.Any())
				return;

            using (Graphics graphics = this.CreateGraphics())
            {
                IGraphicEngine graphicEngine =
                    new GdiGraphicEngine(graphics, this.Font.FontFamily, FontStyle.Regular, m_Palette, MinFontSize, MaxFontSize, m_MinWordWeight, m_MaxWordWeight);
                m_Layout = LayoutFactory.CrateLayout(m_LayoutType, this.Size);
                m_Layout.Arrange(m_Words, graphicEngine);
            }
        }

        public LayoutType LayoutType
        {
            get { return m_LayoutType; }
            set
            {
                if (value == m_LayoutType)
                {
                    return;
                }

                m_LayoutType = value;
                BuildLayout();
                Invalidate();
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            LayoutItem nextItemUnderMouse;
            Point mousePositionRelativeToControl = this.PointToClient(new Point(MousePosition.X, MousePosition.Y));
            this.TryGetItemAtLocation(mousePositionRelativeToControl, out nextItemUnderMouse);
            if (nextItemUnderMouse != m_ItemUnderMouse)
            {
                if (nextItemUnderMouse != null)
                {
                    Rectangle newRectangleToInvalidate = RectangleGrow(nextItemUnderMouse.Rectangle, 6);
                    this.Invalidate(newRectangleToInvalidate);
                }
                if (m_ItemUnderMouse != null)
                {
                    Rectangle prevRectangleToInvalidate = RectangleGrow(m_ItemUnderMouse.Rectangle, 6);
                    this.Invalidate(prevRectangleToInvalidate);
                }
                m_ItemUnderMouse = nextItemUnderMouse;
            }
            base.OnMouseMove(e);
        }

        protected override void OnResize(EventArgs eventargs)
        {
            BuildLayout();
            base.OnResize(eventargs);
        }

        private static Rectangle RectangleGrow(RectangleF original, int growByPixels)
        {
            return new Rectangle(
                (int)(original.X - growByPixels),
                (int)(original.Y - growByPixels),
                (int)(original.Width + growByPixels + 1),
                (int)(original.Height + growByPixels + 1));
        }


        public override Color BackColor
        {
            get
            {
                return m_BackColor;
            }
            set
            {
                if (m_BackColor == value)
                {
                    return;
                }
                m_BackColor = value;
                Invalidate();
            }
        }

        public int MaxFontSize
        {
            get { return m_MaxFontSize; }
            set
            {
                m_MaxFontSize = value;
                BuildLayout();
                Invalidate();
            }
        }

        public int MinFontSize
        {
            get { return m_MinFontSize; }
            set
            {
                m_MinFontSize = value;
                BuildLayout();
                Invalidate();
            }
        }

        public Color[] Palette
        {
            get { return m_Palette; }
            set
            {
				if (value.Length > 0)
				{
					m_Palette = value;
					Invalidate();
				}
            }
        }

        public IEnumerable<IWord> WeightedWords
        {
            get { return m_Words; }
            set
            {
				if (value == null)
					return;

                m_Words = value;

				if (m_Words.Any())
				{
					IWord first = m_Words.First();

					if (first!=null)
					{
						m_MaxWordWeight = first.Occurrences;
						m_MinWordWeight = m_Words.Last().Occurrences;
					}

					BuildLayout();
				}

                Invalidate();
            }
        }

        public IEnumerable<LayoutItem> GetItemsInArea(RectangleF area)
        {
            if (m_Layout == null)
            {
                return new LayoutItem[] {};
            }

            return m_Layout.GetWordsInArea(area);
        }

        public bool TryGetItemAtLocation(Point location, out LayoutItem foundItem)
        {
            foundItem = null;
            IEnumerable<LayoutItem> itemsInArea = GetItemsInArea(new RectangleF(location, new SizeF(0, 0)));
            foreach (LayoutItem item in itemsInArea)
            {
                foundItem = item;
                return true;
            }
            return false;
        }
    }
}
