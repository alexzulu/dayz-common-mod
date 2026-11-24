#ifdef SERVER

//! /players - tells the sender who else is online, or that they are alone.
class CommonMod_CommandPlayers : CommonMod_Command
{
  //! l10n: lead text before the list (": " is appended in Run).
  protected string m_Prefix;
  //! l10n: reply when the sender is alone.
  protected string m_Alone;

  //! Pulls trigger/aliases + localized strings from config.
  void Configure(CommonMod_PlayersCommandConfig c)
  {
    trigger     = c.trigger;
    aliases     = c.aliases;
    description = c.l10n.description;
    m_Prefix    = c.l10n.playersListPrefix;
    m_Alone     = c.l10n.youAreAlone;
  }

  //! Replies with every OTHER online player by GetName() (the engine suffixes duplicate nicks with " (N)");
  //! if the sender is alone, replies with the "nobody else" message instead.
  override void Run(string senderName, string args)
  {
    array<Man> players = new array<Man>();
    GetGame().GetPlayers(players);

    string names;
    foreach (Man m : players)
    {
      PlayerBase pb = PlayerBase.Cast(m);
      if (!pb || !pb.GetIdentity())
        continue;

      string name = pb.GetIdentity().GetName();
      if (name == senderName) // skip the requester themselves
        continue;

      if (names != "")
        names += ", ";

      names += name;
    }

    if (names == "")
      Reply(senderName, m_Alone);
    else
      Reply(senderName, m_Prefix + ": " + names);
  }
}

#endif
