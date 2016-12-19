﻿
using System;
using TDLPluginHelpers;

// PLS DON'T ADD 'USING' STATEMENTS WHILE I AM STILL LEARNING!

namespace WordCloudUIExtension
{
	public class WordCloudUIExtensionCore : System.Windows.Forms.Panel, ITDLUIExtension
	{
		private Boolean m_taskColorIsBkgnd = false;
		private IntPtr m_hwndParent;

		public WordCloudUIExtensionCore(IntPtr hwndParent)
		{
			m_hwndParent = hwndParent;
			InitializeComponent();
		}

		private void CreateWordCloud()
		{
// 			Calendar.DrawTool drawTool = new Calendar.DrawTool();
// 
// 			this.m_WordCloud = new Calendar.WordCloud();
// 			drawTool.WordCloud = this.m_WordCloud;
// 
// 			this.m_WordCloud.ActiveTool = drawTool;
// 			this.m_WordCloud.AllowInplaceEditing = true;
// 			this.m_WordCloud.AllowNew = true;
// 			this.m_WordCloud.AmPmDisplay = true;
//             this.m_WordCloud.Anchor = (System.Windows.Forms.AnchorStyles.Bottom |
//                                      System.Windows.Forms.AnchorStyles.Left |
//                                      System.Windows.Forms.AnchorStyles.Right);
//             this.m_WordCloud.AppHeightMode = Calendar.WordCloud.AppHeightDrawMode.TrueHeightAll;
// 			this.m_WordCloud.DaysToShow = 7;
// 			//this.m_dayView.Dock = System.Windows.Forms.DockStyle.Fill;
// 			this.m_WordCloud.DrawAllAppBorder = false;
// 			this.m_WordCloud.Font = new System.Drawing.Font("Tahoma", 8);
// 			this.m_WordCloud.Location = new System.Drawing.Point(0, 0);
// 			this.m_WordCloud.MinHalfHourApp = false;
// 			this.m_WordCloud.Name = "m_dayView";
// 			this.m_WordCloud.Renderer = m_renderer;
// 			this.m_WordCloud.SelectionEnd = new System.DateTime(((long)(0)));
// 			this.m_WordCloud.SelectionStart = new System.DateTime(((long)(0)));
// 			this.m_WordCloud.Size = new System.Drawing.Size(798, 328);
// 			this.m_WordCloud.StartDate = DateTime.Now;
// 			this.m_WordCloud.TabIndex = 0;
// 			this.m_WordCloud.Text = "m_dayView";
// 			this.m_WordCloud.WorkingHourEnd = 19;
// 			this.m_WordCloud.WorkingHourStart = 9;
// 			this.m_WordCloud.WorkingMinuteEnd = 0;
// 			this.m_WordCloud.WorkingMinuteStart = 0;
// 
//             // I want the hour height to always be 20 for now
//             int hourHeight = 20;
//             //this.m_WordCloud.SlotsPerHour = 4;
//             this.m_WordCloud.SlotHeight = (hourHeight / this.m_WordCloud.SlotsPerHour);
// 
// 			this.m_WordCloud.StartDate = DateTime.Now;
// 			this.m_WordCloud.NewAppointment += new Calendar.NewAppointmentEventHandler(this.OnWordCloudNewAppointment);
// 			this.m_WordCloud.SelectionChanged += new Calendar.AppointmentEventHandler(this.OnWordCloudSelectionChanged);
// 			this.m_WordCloud.ResolveAppointments += new Calendar.ResolveAppointmentsEventHandler(this.OnWordCloudResolveAppointments);
// 			this.m_WordCloud.AppointmentMove += new Calendar.AppointmentEventHandler(this.OnWordCloudAppointmentChanged);
// 
// 			this.Controls.Add(m_WordCloud);

		}

		// ITDLUIExtension ------------------------------------------------------------------

		public bool SelectTask(UInt32 dwTaskID)
		{
// 			CalendarItem item;
// 
// 			if (m_Items.TryGetValue(dwTaskID, out item))
// 			{
// 				DateTime startDate, endDate;
// 				m_WordCloud.GetDateRange(out startDate, out endDate);
// 
// 				if (IsItemWithinRange(item, startDate, endDate))
// 				{
// 					m_WordCloud.SelectedAppointment = item;
// 					return true;
// 				}
// 			}
// 
// 			// all else 
// 			m_WordCloud.SelectedAppointment = null;
			return true;
		}

		public bool SelectTasks(UInt32[] pdwTaskIDs)
		{
			return false;
		}

		public void UpdateTasks(TDLTaskList tasks, 
								TDLUIExtension.UpdateType type, 
								System.Collections.Generic.HashSet<TDLUIExtension.TaskAttribute> attribs)
		{
			switch (type)
			{
				case TDLUIExtension.UpdateType.Delete:
				case TDLUIExtension.UpdateType.Move:
				case TDLUIExtension.UpdateType.All:
					// Rebuild
					m_Items.Clear();
					break;
					
				case TDLUIExtension.UpdateType.New:
				case TDLUIExtension.UpdateType.Edit:
					// In-place update
					break;
			}

			TDLTask task = tasks.GetFirstTask();

			while (task.IsValid() && ProcessTaskUpdate(task, type, attribs))
				task = task.GetNextTask();


            // Clear selection
//             m_WordCloud.SelectionStart = m_WordCloud.SelectionEnd;
// 			m_WordCloud.Invalidate();
		}

		private bool ProcessTaskUpdate(TDLTask task, 
									   TDLUIExtension.UpdateType type,
                                       System.Collections.Generic.HashSet<TDLUIExtension.TaskAttribute> attribs)
		{
			if (!task.IsValid())
				return false;

			CalendarItem item;

			if (m_Items.TryGetValue(task.GetID(), out item))
			{
                if (attribs.Contains(TDLUIExtension.TaskAttribute.Title))
                    item.Title = task.GetTitle();

                if (attribs.Contains(TDLUIExtension.TaskAttribute.DoneDate))
                    item.EndDate = item.OrgEndDate = task.GetDoneDate();

                if (attribs.Contains(TDLUIExtension.TaskAttribute.DueDate))
                    item.EndDate = item.OrgEndDate = task.GetDueDate();

                if (attribs.Contains(TDLUIExtension.TaskAttribute.StartDate))
                    item.StartDate = item.OrgStartDate = task.GetStartDate();

                if (attribs.Contains(TDLUIExtension.TaskAttribute.AllocTo))
                    item.AllocTo = String.Join(", ", task.GetAllocatedTo());

                item.TaskTextColor = task.GetTextDrawingColor();
			}
			else
			{
				item = new CalendarItem();

				item.Title = task.GetTitle();
				item.EndDate = item.OrgEndDate = task.GetDueDate();
				item.StartDate = item.OrgStartDate = task.GetStartDate();
				item.AllocTo = String.Join(", ", task.GetAllocatedTo());
				item.Id = task.GetID();
				item.IsParent = task.IsParent();
				item.TaskTextColor = task.GetTextDrawingColor();
				item.DrawBorder = true;
			}

			if (item.EndDate > item.StartDate)
				m_Items[task.GetID()] = item;

			// Process children
			TDLTask subtask = task.GetFirstSubtask();

			while (subtask.IsValid() && ProcessTaskUpdate(subtask, type, attribs))
				subtask = subtask.GetNextTask();

			return true;
		}

		public bool WantEditUpdate(TDLUIExtension.TaskAttribute attrib)
		{
			switch (attrib)
			{
				case TDLUIExtension.TaskAttribute.Title:
				case TDLUIExtension.TaskAttribute.DoneDate:
				case TDLUIExtension.TaskAttribute.DueDate:
				case TDLUIExtension.TaskAttribute.StartDate:
				case TDLUIExtension.TaskAttribute.AllocTo:
					return true;
			}

			// all else
			return false;
		}
	   
		public bool WantSortUpdate(TDLUIExtension.TaskAttribute attrib)
		{
			return false;
		}
	   
		public bool PrepareNewTask(ref TDLTask task)
		{
            // Set the start/due dates to match the current selection
//             if (m_WordCloud.SelectionStart < m_WordCloud.SelectionEnd)
// 			{
//                 task.SetStartDate(m_WordCloud.SelectionStart);
//                 task.SetDueDate(m_WordCloud.SelectionEnd);
//             }

			return true;
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
		{
			return false;
		}
	   
		public bool DoAppCommand(TDLUIExtension.AppCommand cmd, UInt32 dwExtra)
		{
			return false;
		}

		public bool CanDoAppCommand(TDLUIExtension.AppCommand cmd, UInt32 dwExtra)
		{
			return false;
		}

		public bool GetLabelEditRect(ref Int32 left, ref Int32 top, ref Int32 right, ref Int32 bottom)
		{
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle();

//             if (m_WordCloud.GetAppointmentRect(m_WordCloud.SelectedAppointment, ref rect))
//             {
//                 rect = m_WordCloud.RectangleToScreen(rect);
// 
//                 left = rect.Left;
//                 top = rect.Top;
//                 right = rect.Right;
//                 bottom = (rect.Top + m_renderer.GetFontHeight() + 4); // 4 = border
// 
//                 return true;
//             }

            // else
            return false;
		}

		public TDLUIExtension.HitResult HitTest(Int32 xPos, Int32 yPos)
		{
//             System.Drawing.Point pt = m_WordCloud.PointToClient(new System.Drawing.Point(xPos, yPos));
//             Calendar.Appointment appointment = m_WordCloud.GetAppointmentAt(pt.X, pt.Y);
// 
//             if (appointment != null)
//             {
//                 return TDLUIExtension.HitResult.Task;
//             }
//             else if (m_WordCloud.GetTrueRectangle().Contains(pt))
//             {
//                 return TDLUIExtension.HitResult.Tasklist;
//             }

            // else
			return TDLUIExtension.HitResult.Nowhere;
		}

		public void SetUITheme(TDLTheme theme)
		{
//             m_WordCloud.Invalidate(true);

            this.BackColor = theme.GetAppDrawingColor(TDLTheme.AppColor.AppBackDark);
		}

		public void SetReadOnly(bool bReadOnly)
		{
		}

		public void SavePreferences(TDLPreferences prefs, String key)
		{
		}

		public void LoadPreferences(TDLPreferences prefs, String key, bool appOnly)
		{
			if (appOnly)
			{
				bool taskColorIsBkgnd = (prefs.GetProfileInt("Preferences", "ColorTaskBackground", 0) != 0);

				if (taskColorIsBkgnd != m_taskColorIsBkgnd)
				{
					m_taskColorIsBkgnd = taskColorIsBkgnd;
					Invalidate();
				}
			}
		}
		
		// PRIVATE ------------------------------------------------------------------------------

		private void InitializeComponent()
		{
			this.BackColor = System.Drawing.Color.White;
			this.m_Items = new System.Collections.Generic.Dictionary<UInt32, CalendarItem>();

			CreateWordCloud();
		}

        protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            System.Drawing.Rectangle Border = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);
            Border.Y = 20;
            Border.Height -= 20;

            System.Windows.Forms.ControlPaint.DrawBorder(e.Graphics, Border, System.Drawing.Color.DarkGray, System.Windows.Forms.ButtonBorderStyle.Solid);
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

//             System.Drawing.Rectangle WordCloud = new System.Drawing.Rectangle(ClientRectangle.Location, ClientRectangle.Size);
// 
//             WordCloud.Y = 20;
//             WordCloud.Height -= 20;
//             WordCloud.Inflate(-1, -1);
// 
//             m_WordCloud.Location = WordCloud.Location;
//             m_WordCloud.Size = WordCloud.Size;
// 
//             Invalidate();
        }

		private void OnWordCloudNewAppointment(object sender, Calendar.NewAppointmentEventArgs args)
		{
//             Calendar.Appointment m_Appointment = new Calendar.Appointment();
// 
//             m_Appointment.StartDate = args.StartDate;
//             m_Appointment.EndDate = args.EndDate;
//             m_Appointment.Title = args.Title;
//             m_Appointment.Group = "2";
// 
//             m_Appointments.Add(m_Appointment);
		}

		private void OnWordCloudSelectionChanged(object sender, Calendar.AppointmentEventArgs args)
		{
//             TDLUIExtension.TDLNotify notify = new TDLUIExtension.TDLNotify(m_hwndParent);
// 
// 			switch (this.m_WordCloud.Selection)
// 			{
// 				case Calendar.SelectionType.DateRange:
// 					break;
// 
// 				case Calendar.SelectionType.Appointment:
// 					if (args.Appointment != null)
// 						notify.NotifySelChange(args.Appointment.Id);
//                     else
//                         notify.NotifySelChange(0);
// 					break;
// 			}
		}

		private void OnWordCloudAppointmentChanged(object sender, Calendar.AppointmentEventArgs args)
		{
			Calendar.MoveAppointmentEventArgs move = args as Calendar.MoveAppointmentEventArgs;

			// Ignore moves whilst they are occurring
			if ((move == null) || !move.Finished)
				return;

			CalendarItem item = args.Appointment as CalendarItem;

			if (item == null)
				return;

            TDLUIExtension.TDLNotify notify = new TDLUIExtension.TDLNotify(m_hwndParent);

			switch (move.Mode)
			{
				case Calendar.SelectionTool.Mode.Move:
					if ((item.StartDate - item.OrgStartDate).TotalSeconds != 0.0)
					{
						item.OrgStartDate = item.StartDate;
						notify.NotifyMod(TDLUIExtension.TaskAttribute.OffsetTask, args.Appointment.StartDate);
					}
					break;

				case Calendar.SelectionTool.Mode.ResizeTop:
					if ((item.StartDate - item.OrgStartDate).TotalSeconds != 0.0)
					{
						item.OrgStartDate = item.StartDate;
						notify.NotifyMod(TDLUIExtension.TaskAttribute.StartDate, args.Appointment.StartDate);
					}
					break;

				case Calendar.SelectionTool.Mode.ResizeBottom:
					if ((item.EndDate - item.OrgEndDate).TotalSeconds != 0.0)
					{
						item.OrgEndDate = item.EndDate;
						notify.NotifyMod(TDLUIExtension.TaskAttribute.DueDate, args.Appointment.EndDate);
					}
					break;
			}
		}

		private void OnWordCloudResolveAppointments(object sender, Calendar.ResolveAppointmentsEventArgs args)
		{
			System.Collections.Generic.List<Calendar.Appointment> appts =
				new System.Collections.Generic.List<Calendar.Appointment>();

			foreach (System.Collections.Generic.KeyValuePair<UInt32, CalendarItem> item in m_Items)
			{
				if (IsItemWithinRange(item.Value, args.StartDate, args.EndDate))
				{
					// Recalculate colours
					if (m_taskColorIsBkgnd)
					{
						item.Value.TextColor = ((item.Value.TaskTextColor.GetBrightness() > 0.5) ? System.Drawing.Color.Black : System.Drawing.Color.White);
						item.Value.BorderColor = TDLColor.DarkerDrawing(item.Value.TaskTextColor, 0.5f);
						item.Value.BarColor = item.Value.TaskTextColor;
						item.Value.FillColor = item.Value.TaskTextColor;
					}
					else
					{
						item.Value.TextColor = item.Value.TaskTextColor;
						item.Value.BorderColor = item.Value.TaskTextColor;
						item.Value.FillColor = TDLColor.LighterDrawing(item.Value.TaskTextColor, 0.9f); 
						item.Value.BarColor = item.Value.TaskTextColor;
					}

					appts.Add(item.Value);
				}
			}

			args.Appointments = appts;
		}

		private bool IsItemWithinRange(CalendarItem item, DateTime startDate, DateTime endDate)
		{
			return ((item.StartDate >= startDate) && (item.EndDate <= endDate));
		}

		// --------------------------------------------------------------------------------------
		private System.Collections.Generic.Dictionary<UInt32, CalendarItem> m_Items;
	}

	public class CalendarItem : Calendar.Appointment
	{
		public DateTime OrgStartDate { get; set; }
		public DateTime OrgEndDate { get; set; }

		public System.Drawing.Color TaskTextColor { get; set; }

		public String AllocTo { get; set; }
		public Boolean IsParent { get; set; }
	}

}