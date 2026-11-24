#ifdef SERVER

//! CommonMod entry point: loads config, registers enabled chat commands, dispatches "/..." lines.
modded class MissionServer
{
  //! Registered (enabled-only) chat commands, scanned on every chat line.
  protected ref array<ref CommonMod_Command> m_CommonModCommands;

  //! Engine setup hook (after ctor, before mission start) - load config, register enabled commands.
  override void OnInit()
  {
    super.OnInit();
    CommonMod_Config.Load();

    CommonMod_Config cfg = CommonMod_Config.Get();
    m_CommonModCommands = new array<ref CommonMod_Command>();

    if (cfg.commands.playersList.enabled)
    {
      CommonMod_CommandPlayers players = new CommonMod_CommandPlayers();
      players.Configure(cfg.commands.playersList);
      m_CommonModCommands.Insert(players);
    }

    if (cfg.commands.time.enabled)
    {
      CommonMod_CommandTime time = new CommonMod_CommandTime();
      time.Configure(cfg.commands.time);
      m_CommonModCommands.Insert(time);
    }

    if (cfg.commands.ping.enabled)
    {
      CommonMod_CommandPing ping = new CommonMod_CommandPing();
      ping.Configure(cfg.commands.ping);
      m_CommonModCommands.Insert(ping);
    }
  }

  //! Engine event hook (chat, VON, connect/disconnect, etc.) - acts only on chat lines - matches the
  //! first word against a registered command, which sends its own reply.
  override void OnEvent(EventType eventTypeId, Param params)
  {
    super.OnEvent(eventTypeId, params); // keep vanilla chat flow intact

    if (eventTypeId != ChatMessageEventTypeID)
      return;

    ChatMessageEventParams chat = ChatMessageEventParams.Cast(params);
    if (!chat)
      return;

    string text = chat.param3;
    text.TrimInPlace();
    if (text == "")
      return;

    // first whitespace-separated token = trigger word, remainder = args (no hardcoded '/')
    string token = text;
    string args  = "";
    int space = text.IndexOf(" ");
    if (space != -1)
    {
      token = text.Substring(0, space);
      args  = text.Substring(space + 1, text.Length() - space - 1);
    }
    token.ToLower();

    foreach (CommonMod_Command cmd : m_CommonModCommands)
    {
      if (!cmd.Matches(token))
        continue;

      cmd.Run(chat.param2, args);
      return;
    }
  }
}

#endif
