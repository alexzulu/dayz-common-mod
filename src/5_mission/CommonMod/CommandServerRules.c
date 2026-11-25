#ifdef SERVER

//! /rules - sends the server rules to the sender. Chat events carry no UID, so the sender is matched by display
//! name (dup nicks -> all matches get it).
class CommonMod_CommandServerRules : CommonMod_Command
{
  //! Shared rules renderer (owns the body + white rendering).
  protected ref CommonMod_Rules m_Rules;

  //! Pulls trigger/aliases + description from config; binds the shared renderer.
  void Configure(CommonMod_ServerRulesCommandConfig c, CommonMod_Rules rules)
  {
    trigger     = c.trigger;
    aliases     = c.aliases;
    description = c.l10n.description;
    m_Rules     = rules;
  }

  //! Sends the rules to every connected player matching the sender's name (no UID in a chat event).
  override void Run(string senderName, string args)
  {
    foreach (PlayerBase pb : MatchByName(senderName))
      m_Rules.SendTo(pb);
  }
}

#endif
