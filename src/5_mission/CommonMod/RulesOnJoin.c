#ifdef SERVER

//! Rules-on-join - on connect, shows the rules in chat, repeated repeatCount times with a delay between shows
//! (per-join). Goal: a fresh full cycle on EVERY connect, reconnects included.
//! InvokeOnConnect fires twice per join (vanilla ClientNew + ClientReady) - m_Active dedups that so one connection
//! yields one cycle. A reconnect still restarts because PlayerDisconnect drops the m_Active entry; any chain left
//! over from the previous connection self-cancels via the per-player epoch - a new cycle bumps m_Epoch, the stale
//! timer fires, sees the epoch mismatch and stops. Show count rides along as a CallLater param (no count map).
//! Both maps are bounded to in-flight cycles (dropped when a cycle ends), so they don't grow with total joins.
//! Parameterless ctor + Configure(cfg, rules) (Enforce forbids ctor args).
class CommonMod_RulesOnJoin
{
  //! uid -> current cycle epoch. Bumped per cycle; dropped when the cycle ends. A queued show whose epoch != this
  //! value is stale (a newer cycle started) and stops.
  protected ref map<string, int> m_Epoch;
  //! uid -> true while a cycle runs on the CURRENT connection; gates the twin InvokeOnConnect. Dropped on disconnect.
  protected ref map<string, bool> m_Active;
  //! Delay before the first show, ms.
  protected int m_InitialDelayMs;
  //! Total shows per join.
  protected int m_RepeatCount;
  //! Delay between consecutive shows, ms.
  protected int m_IntervalMs;
  //! Shared rules renderer (owns the body + white rendering).
  protected ref CommonMod_Rules m_Rules;

  void CommonMod_RulesOnJoin()
  {
    m_Epoch  = new map<string, int>();
    m_Active = new map<string, bool>();
  }

  //! Pulls timing from config; binds the shared rules renderer.
  void Configure(CommonMod_ShowServerRulesOnJoinConfig c, CommonMod_Rules rules)
  {
    m_InitialDelayMs = c.initialDelaySeconds * 1000;
    m_RepeatCount    = c.repeatCount;
    m_IntervalMs     = c.intervalBetweenRepeatsSeconds * 1000;
    m_Rules          = rules;
  }

  //! Connect hook - starts a fresh cycle unless one is already in flight for this connection (dedups the twin
  //! InvokeOnConnect). The new epoch orphans any chain left over from a prior connection.
  void OnConnect(PlayerBase player)
  {
    if (m_RepeatCount <= 0 || !m_Rules || !m_Rules.HasLines())
      return;

    if (!player.GetIdentity())
      return;

    string uid = player.GetIdentity().GetId();
    if (m_Active.Contains(uid))
      return;

    m_Active.Set(uid, true);
    int epoch = m_Epoch.Get(uid) + 1; // Get -> 0 when absent
    m_Epoch.Set(uid, epoch);
    GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ShowForUid, m_InitialDelayMs, false, uid, epoch, 0);
  }

  //! Disconnect hook - end this connection's cycle so the next connect restarts from the top. m_Epoch is kept (the
  //! still-queued chain self-cancels when a reconnect bumps it); it is dropped later when that chain terminates.
  void OnDisconnect(string uid)
  {
    m_Active.Remove(uid);
  }

  //! One show for a player, re-resolved live by UID. A stale epoch (a newer cycle started) or an absent player
  //! (disconnected) stops the chain; otherwise sends the lines and reschedules until repeatCount is reached.
  //! Terminal branches drop the per-uid state so both maps stay bounded to in-flight cycles.
  void ShowForUid(string uid, int epoch, int shown)
  {
    if (m_Epoch.Get(uid) != epoch)
      return;

    PlayerBase pb = FindByUid(uid);
    if (!pb)
    {
      m_Active.Remove(uid);
      m_Epoch.Remove(uid);
      return;
    }

    m_Rules.SendTo(pb);

    int next = shown + 1;
    if (next >= m_RepeatCount)
    {
      m_Active.Remove(uid);
      m_Epoch.Remove(uid);
      return;
    }

    GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ShowForUid, m_IntervalMs, false, uid, epoch, next);
  }

  //! Returns the connected player whose identity UID matches, or null if none (disconnected). The player is
  //! re-resolved each fire - a timer carries no live object across the delay.
  protected PlayerBase FindByUid(string uid)
  {
    array<Man> players = new array<Man>();
    GetGame().GetPlayers(players);

    foreach (Man m : players)
    {
      PlayerBase pb = PlayerBase.Cast(m);
      if (!pb || !pb.GetIdentity())
        continue;

      if (pb.GetIdentity().GetId() == uid)
        return pb;
    }

    return null;
  }
}

#endif
