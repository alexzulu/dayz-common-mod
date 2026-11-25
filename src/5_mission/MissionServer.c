#ifdef SERVER

//! CommonMod entry point: loads config, registers enabled chat commands, dispatches "/..." lines.
modded class MissionServer
{
  //! Registered (enabled-only) chat commands, scanned on every chat line.
  protected ref array<ref CommonMod_Command> m_CommonModCommands;
  //! PvP-zone warning feature; null when disabled. Its Tick() is a repeating gameplay call.
  protected ref CommonMod_PvpZone m_CommonModPvpZone;
  //! Rules-on-join feature; null when disabled. Driven by the InvokeOnConnect hook.
  protected ref CommonMod_RulesOnJoin m_CommonModRulesOnJoin;

  //! Engine setup hook (after ctor, before mission start) - load config, register enabled commands.
  override void OnInit()
  {
    super.OnInit();
    CommonMod_Config.Load();

    CommonMod_Config cfg = CommonMod_Config.Get();
    m_CommonModCommands = new array<ref CommonMod_Command>();

    // shared rules renderer - one body/rendering for both the /rules command and the on-join show
    CommonMod_Rules rules = new CommonMod_Rules();
    rules.Configure(cfg.serverRules);

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

    if (cfg.commands.serverRules.enabled)
    {
      CommonMod_CommandServerRules serverRules = new CommonMod_CommandServerRules();
      serverRules.Configure(cfg.commands.serverRules, rules);
      m_CommonModCommands.Insert(serverRules);
    }

    if (cfg.pvpZone.enabled)
    {
      m_CommonModPvpZone = new CommonMod_PvpZone();
      m_CommonModPvpZone.Configure(cfg.pvpZone);
      GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(
        m_CommonModPvpZone.Tick, cfg.pvpZone.checkIntervalSeconds * 1000, true);
    }

    if (cfg.showServerRulesOnJoin.enabled)
    {
      m_CommonModRulesOnJoin = new CommonMod_RulesOnJoin();
      m_CommonModRulesOnJoin.Configure(cfg.showServerRulesOnJoin, rules);
    }

    // registered last so /help lists itself at the end; it reads the registry lazily in Run(), so order
    // affects only output sequence, not completeness.
    if (cfg.commands.help.enabled)
    {
      CommonMod_CommandHelp help = new CommonMod_CommandHelp();
      help.Configure(cfg.commands.help, m_CommonModCommands);
      m_CommonModCommands.Insert(help);
    }
  }

  //! Player connect hook - kick off the rules-on-join show cycle for the joining player.
  override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
  {
    super.InvokeOnConnect(player, identity);

    if (m_CommonModRulesOnJoin && player)
      m_CommonModRulesOnJoin.OnConnect(player);
  }

  //! Player disconnect hook - end the player's rules-on-join cycle so a future reconnect restarts it from the top.
  override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid)
  {
    super.PlayerDisconnected(player, identity, uid);

    if (m_CommonModRulesOnJoin)
      m_CommonModRulesOnJoin.OnDisconnect(uid);
  }

  //! Mission teardown - drop our queued gameplay calls so they can't fire on a dead object after teardown.
  override void OnMissionFinish()
  {
    if (m_CommonModPvpZone)
      GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(m_CommonModPvpZone.Tick);

    if (m_CommonModRulesOnJoin)
      GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(m_CommonModRulesOnJoin.ShowForUid);

    super.OnMissionFinish();
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
