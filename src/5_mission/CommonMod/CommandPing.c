#ifdef SERVER

//! /ping - tells the sender their connection ping (current + average).
//! Engine gives an estimate, not an exact live value.
class CommonMod_CommandPing : CommonMod_Command
{
  //! l10n: lead text before the values.
  protected string m_Prefix;
  //! l10n: label for the current (act) value.
  protected string m_CurrentLabel;
  //! l10n: label for the average (avg) value.
  protected string m_AverageLabel;
  //! l10n: millisecond unit suffix.
  protected string m_Unit;

  //! Pulls trigger/aliases + localized strings from config.
  void Configure(CommonMod_PingCommandConfig c)
  {
    trigger        = c.trigger;
    aliases        = c.aliases;
    description    = c.l10n.description;
    m_Prefix       = c.l10n.pingPrefix;
    m_CurrentLabel = c.l10n.currentLabel;
    m_AverageLabel = c.l10n.averageLabel;
    m_Unit         = c.l10n.unit;
  }

  //! Replies with the sender's current + average ping. Ping is read per matching player (each has
  //! its own identity/value), so it bypasses the name-only base Reply() and messages directly.
  override void Run(string senderName, string args)
  {
    array<Man> players = new array<Man>();
    GetGame().GetPlayers(players);

    foreach (Man m : players)
    {
      PlayerBase pb = PlayerBase.Cast(m);
      if (!pb || !pb.GetIdentity())
        continue;

      if (pb.GetIdentity().GetName() != senderName)
        continue;

      int act = pb.GetIdentity().GetPingAct();
      int avg = pb.GetIdentity().GetPingAvg();
      string line = m_Prefix + ": " + m_CurrentLabel + " " + act.ToString() + " " + m_Unit + ", " + m_AverageLabel + " " + avg.ToString() + " " + m_Unit;
      pb.MessageStatus(line);
    }
  }
}

#endif
