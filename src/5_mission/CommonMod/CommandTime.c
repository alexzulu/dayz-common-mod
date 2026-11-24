#ifdef SERVER

//! /time - tells the sender the current in-game server time (HH:MM).
class CommonMod_CommandTime : CommonMod_Command
{
  //! l10n: lead text before the time value (": " is appended in Run).
  protected string m_Prefix;

  //! Pulls trigger/aliases + localized strings from config.
  void Configure(CommonMod_TimeCommandConfig c)
  {
    trigger     = c.trigger;
    aliases     = c.aliases;
    description = c.l10n.description;
    m_Prefix    = c.l10n.timePrefix;
  }

  //! Replies with the current in-game time as HH:MM (date fields from GetDate are ignored).
  override void Run(string senderName, string args)
  {
    int year, month, day, hour, minute;
    GetGame().GetWorld().GetDate(year, month, day, hour, minute);
    Reply(senderName, m_Prefix + ": " + Pad(hour) + ":" + Pad(minute));
  }

  //! Zero-pads a value to 2 digits (Enforce has no printf-style width spec nor a ternary operator).
  protected string Pad(int v)
  {
    string zero;
    if (v < 10)
      zero = "0";

    return zero + v.ToString();
  }
}

#endif
