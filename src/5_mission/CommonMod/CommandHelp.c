#ifdef SERVER

//! /help - lists every enabled command (trigger - description), plus admin-declared extra entries for
//! commands from other mods. Reads the live command registry so the list always matches what's enabled.
class CommonMod_CommandHelp : CommonMod_Command
{
  //! l10n: header line shown before the list.
  protected string m_ListPrefix;
  //! Live registry of enabled commands (shared with MissionServer - not owned). Includes this command.
  protected ref array<ref CommonMod_Command> m_Registry;
  //! Admin-declared extra entries (commands from other mods).
  protected ref array<ref CommonMod_HelpExtraCommand> m_Extra;

  //! Pulls trigger/aliases + localized strings; binds the shared registry and the extra-entries list.
  void Configure(CommonMod_HelpCommandConfig c, array<ref CommonMod_Command> registry)
  {
    trigger      = c.trigger;
    aliases      = c.aliases;
    description  = c.l10n.description;
    m_ListPrefix = c.l10n.listPrefix;
    m_Registry   = registry;
    m_Extra      = c.extraCommands;
  }

  //! Replies (yellow) with the header, then one line per enabled command and per admin-declared extra entry.
  override void Run(string senderName, string args)
  {
    array<PlayerBase> matches = MatchByName(senderName);
    if (matches.Count() == 0)
      return;

    array<string> lines = new array<string>();
    lines.Insert(m_ListPrefix + ":");

    foreach (CommonMod_Command cmd : m_Registry)
      lines.Insert(cmd.trigger + " - " + cmd.description);

    if (m_Extra)
    {
      foreach (CommonMod_HelpExtraCommand extra : m_Extra)
        lines.Insert(extra.name + " - " + extra.description);
    }

    // yellow (colorAction) via MessageAction - the base Reply() sends blue MessageStatus
    foreach (PlayerBase pb : matches)
    {
      foreach (string line : lines)
        pb.MessageAction(line);
    }
  }
}

#endif
