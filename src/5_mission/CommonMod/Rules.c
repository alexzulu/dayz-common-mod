#ifdef SERVER

//! Shared server-rules renderer - holds the rules body and sends it to a player as white chat lines.
//! Parameterless ctor + Configure (Enforce forbids ctor args).
class CommonMod_Rules
{
  //! Rules body, one chat line per element. Points at the config array - not owned/copied.
  protected ref array<string> m_Lines;

  //! Binds the rules body (the config's `serverRules`).
  void Configure(array<string> lines)
  {
    m_Lines = lines;
  }

  //! True when there is at least one line to show.
  bool HasLines()
  {
    return m_Lines && m_Lines.Count() > 0;
  }

  //! Sends every rules line to `pb` in white.
  void SendTo(PlayerBase pb)
  {
    if (!m_Lines)
      return;

    // white text: "white" is a deliberately-unrecognized color class - `ChatLine.ColorNameToColor` only matches
    // colorStatus/Action/Friendly/Important; anything else falls through to its white (0xFFFFFFFF) default, which
    // reads far brighter than the blue status channel (an empty class would instead hit the red channel fallback)
    foreach (string line : m_Lines)
      pb.Message(line, "white");
  }
}

#endif
