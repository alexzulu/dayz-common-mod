#ifdef SERVER

//! PvP-zone warning - a periodic tick checks each player's position against the PvP zone polygon and alerts those
//! inside. Map-gated. The check cadence (every checkIntervalSeconds) is decoupled from the alert repeat period - a
//! per-player last-alert timestamp throttles alerts to one per intervalBetweenRepeatsSeconds while a player stays
//! inside. Presence of a UID in that map also means "inside the zone" - enter/exit transitions derive from it.
//! Parameterless ctor + Configure(cfg) (Enforce forbids ctor args).
class CommonMod_PvpZone
{
  //! NWAF (chernarusplus) contour, flat [x0,y0, x1,y1, ...] in in-game meters ([x]=pos[0], [y]=pos[2]). Open contour
  //! (last->first implied by Math2D.IsPointInPolygon). Hand-traced; ~1-2m tolerance.
  static const ref array<float> NWAF_POLYGON = {
    4530.7, 9348.9, 4374.8, 9532.2, 4368.3, 9702.0, 4173.8, 10056.6, 4108.6, 10158.0, 4052.1, 10161.3,
    3997.0, 10173.3, 3934.1, 10252.0, 3883.6, 10327.5, 3820.8, 10406.5, 3812.9, 10416.7, 3799.2, 10452.0,
    3768.3, 10562.1, 3745.1, 10625.6, 3747.0, 10699.0, 3707.3, 10767.7, 3663.0, 10824.6, 3613.1, 10872.4,
    3592.0, 10883.4, 3561.9, 10910.5, 3559.3, 10919.4, 3566.7, 10930.0, 3623.6, 10989.1, 3628.8, 11000.2,
    3630.8, 11012.9, 3631.1, 11049.3, 3636.6, 11066.3, 3636.3, 11080.4, 3596.5, 11162.3, 3596.2, 11228.4,
    3578.4, 11319.1, 3573.8, 11331.4, 3554.4, 11362.7, 3552.1, 11377.4, 3563.5, 11453.7, 3543.0, 11490.6,
    3516.0, 11546.2, 3494.6, 11597.2, 3494.8, 11618.6, 3507.0, 11637.9, 3534.7, 11667.6, 3623.2, 11755.1,
    3654.0, 11780.0, 3665.1, 11793.6, 3672.4, 11831.0, 3678.7, 11875.8, 3685.7, 11896.1, 3698.5, 11909.2,
    3744.2, 11940.4, 3762.7, 11958.8, 3787.3, 11980.2, 3818.0, 12004.3, 3850.8, 12025.9, 3875.2, 12045.1,
    3891.8, 12060.4, 3906.7, 12069.4, 3946.7, 12080.9, 3993.5, 12092.8, 4018.4, 12093.3, 4062.9, 12084.8,
    4084.0, 12084.7, 4102.9, 12082.4, 4176.9, 12055.9, 4194.8, 12043.5, 4258.9, 11978.8, 4305.4, 11937.0,
    4409.6, 11820.5, 4411.3, 11791.6, 4420.1, 11719.7, 4438.7, 11679.4, 4444.2, 11557.0, 4460.9, 11454.4,
    4461.7, 11414.3, 4464.9, 11377.9, 4479.3, 11331.4, 4488.9, 11311.5, 4513.2, 11281.6, 4553.3, 11242.5,
    4559.9, 11227.8, 4572.4, 11212.1, 4588.3, 11201.9, 4615.9, 11187.9, 4646.6, 11175.9, 4671.7, 11163.6,
    4687.7, 11151.6, 4715.3, 11102.1, 4725.5, 11089.1, 4746.6, 11069.5, 4756.4, 11063.1, 4765.0, 11052.7,
    4784.1, 11026.9, 4804.4, 10995.7, 4827.8, 10951.8, 4860.6, 10897.1, 4888.0, 10848.5, 4892.9, 10828.8,
    4906.6, 10750.3, 4923.5, 10565.4, 4942.0, 10520.6, 4974.6, 10453.0, 4982.6, 10426.2, 4974.9, 10399.6,
    4951.2, 10388.8, 4970.2, 10350.2, 4984.7, 10326.9, 4998.1, 10309.2, 5019.1, 10297.0, 5043.5, 10287.7,
    5072.3, 10278.9, 5129.5, 10296.6, 5179.0, 10317.2, 5195.5, 10309.9, 5207.1, 10295.3, 5224.8, 10275.0,
    5277.3, 10205.3, 5296.3, 10187.4, 5322.2, 10173.7, 5376.2, 10125.8, 5390.3, 10106.3, 5408.6, 10090.5,
    5413.9, 10070.6, 5444.8, 9986.1, 5456.5, 9950.5, 5458.5, 9941.1, 5459.0, 9920.4, 5453.6, 9849.4,
    5453.3, 9831.7, 5448.7, 9815.0, 5444.2, 9806.4, 5438.7, 9775.3, 5431.9, 9751.3, 5429.6, 9736.9,
    5420.7, 9713.8, 5398.1, 9670.0, 5392.4, 9661.7, 5378.5, 9652.6, 5356.7, 9633.5, 5333.8, 9607.8,
    5318.1, 9584.6, 5309.3, 9566.7, 5307.0, 9552.2, 5299.7, 9518.3, 5291.7, 9494.5, 5283.6, 9464.8,
    5281.5, 9441.9, 5282.6, 9424.4, 5286.9, 9407.1, 5301.9, 9385.3, 5311.2, 9369.7, 5311.8, 9355.0,
    5308.4, 9350.2, 5298.9, 9344.5, 5276.7, 9335.4, 5256.9, 9330.1, 5253.5, 9322.9, 5253.9, 9314.1,
    5251.2, 9298.3, 5247.5, 9288.2, 5237.6, 9271.6, 5233.9, 9260.4, 5232.5, 9247.6, 5214.0, 9203.2,
    5184.8, 9157.5, 5160.0, 9126.3, 5149.0, 9110.0, 5141.2, 9095.5, 5135.9, 9082.9, 5133.6, 9072.1,
    5129.5, 9064.0, 5114.8, 9042.4, 5101.3, 9018.2, 5090.4, 9004.7, 5081.8, 9001.9, 4997.8, 9031.9,
    4953.2, 9049.6, 4881.2, 9078.9, 4859.5, 9083.0, 4840.9, 9087.8, 4829.2, 9089.1, 4810.7, 9109.6,
    4791.5, 9132.8, 4770.3, 9161.5, 4756.5, 9181.8, 4742.8, 9199.6, 4727.0, 9216.1, 4696.3, 9239.5,
    4673.9, 9255.4, 4643.4, 9273.9, 4622.9, 9288.9, 4601.5, 9315.6, 4588.2, 9334.3, 4573.1, 9349.7
  };
  //! Per-player (UID -> last-alert mission time, ms). Presence == inside the zone (added on enter, dropped on
  //! exit); the timestamp throttles repeats. Bounded by players currently inside.
  protected ref map<string, float> m_LastAlert;
  //! Send the chat lines (enter / ongoing / exit).
  protected bool m_ShowChatWarning;
  //! Show the toast notification while inside.
  protected bool m_ShowToastWarning;
  //! Toast display duration, seconds.
  protected float m_ShowToastTime;
  //! Alert repeat period, milliseconds.
  protected float m_RepeatMs;
  //! l10n: red chat line on entering the zone.
  protected string m_ChatEnterText;
  //! l10n: red chat line repeated while inside.
  protected string m_ChatText;
  //! l10n: yellow chat line on leaving the zone.
  protected string m_ChatExitText;
  //! l10n: toast title.
  protected string m_ToastTitle;
  //! l10n: toast body.
  protected string m_ToastBody;
  //! Cached map gate: -1 unknown, 0 not chernarusplus, 1 chernarusplus.
  protected int m_MapOk;

  void CommonMod_PvpZone()
  {
    m_MapOk = -1;
    m_LastAlert = new map<string, float>();
  }

  //! Pulls toggles, timing and localized strings from config.
  void Configure(CommonMod_PvpZoneConfig c)
  {
    m_ShowChatWarning  = c.showChatWarning;
    m_ShowToastWarning = c.showToastWarning;
    m_ShowToastTime    = c.showToastTimeSeconds;
    m_RepeatMs         = c.intervalBetweenRepeatsSeconds * 1000;
    m_ChatEnterText    = c.l10n.chatEnterText;
    m_ChatText         = c.l10n.chatText;
    m_ChatExitText     = c.l10n.chatExitText;
    m_ToastTitle       = c.l10n.toastTitle;
    m_ToastBody        = c.l10n.toastBody;
  }

  //! Sends the PvP-zone toast to a player.
  protected void SendToast(PlayerBase pb)
  {
    NotificationSystem.SendNotificationToPlayerExtended(pb, m_ShowToastTime, m_ToastTitle, m_ToastBody);
  }

  //! One check pass - gate on map, then for each player test the polygon (native Math2D) and fire the matching
  //! alert - enter line once, ongoing line throttled, exit line once.
  void Tick()
  {
    if (m_MapOk == -1)
    {
      string world = GetGame().GetWorldName();
      world.ToLower();
      if (world == "chernarusplus")
        m_MapOk = 1;
      else
        m_MapOk = 0;
    }

    if (m_MapOk != 1)
      return;

    float now = GetGame().GetTime();

    array<Man> players = new array<Man>();
    GetGame().GetPlayers(players);

    foreach (Man m : players)
    {
      PlayerBase pb = PlayerBase.Cast(m);
      if (!pb || !pb.GetIdentity())
        continue;

      string uid   = pb.GetIdentity().GetId();
      vector pos   = pb.GetPosition();
      bool inside  = Math2D.IsPointInPolygon(NWAF_POLYGON, pos[0], pos[2]);
      bool tracked = m_LastAlert.Contains(uid); // was inside on the previous tick

      if (!inside)
      {
        // Exit: single yellow chat line, then stop tracking.
        if (tracked)
        {
          m_LastAlert.Remove(uid);

          if (m_ShowChatWarning)
            pb.MessageAction(m_ChatExitText);
        }

        continue;
      }

      if (!tracked)
      {
        // Enter: red enter line + toast; start the repeat clock so the ongoing line comes later.
        m_LastAlert.Set(uid, now);

        if (m_ShowChatWarning)
          pb.MessageImportant(m_ChatEnterText);

        if (m_ShowToastWarning)
          SendToast(pb);

        continue;
      }

      // Still inside: ongoing warning once per repeat period.
      if (now - m_LastAlert.Get(uid) < m_RepeatMs)
        continue;

      m_LastAlert.Set(uid, now);

      if (m_ShowChatWarning)
        pb.MessageImportant(m_ChatText);

      if (m_ShowToastWarning)
        SendToast(pb);
    }
  }
}

#endif
