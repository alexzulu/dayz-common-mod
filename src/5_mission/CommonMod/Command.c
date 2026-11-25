#ifdef SERVER

//! Base for a server-side chat command. One subclass per command; only enabled commands are
//! instantiated and registered in the modded MissionServer, then matched against chat lines.
//! A command owns its action - Run() reads the args and sends any reply via Reply().
//! NB: Enforce forbids super(args)/ctor overloading - keep ctors parameterless.
class CommonMod_Command
{
  //! Trigger word, full (e.g. "/players"); set from config at registration.
  string trigger;
  //! Alternative trigger words.
  ref array<string> aliases;
  //! One-line summary.
  string description;

  //! True if `word` (already lowercased) equals the trigger or any alias.
  bool Matches(string word)
  {
    string t = trigger;
    t.ToLower();
    if (t == word)
      return true;

    if (aliases)
    {
      foreach (string a : aliases)
      {
        string al = a;
        al.ToLower();
        if (al == word)
          return true;
      }
    }

    return false;
  }

  //! Executes the command - reads `args`, sends any reply itself via Reply(). Override per command.
  //! `senderName` is the chat sender's display name; `args` is the raw text after the trigger.
  void Run(string senderName, string args)
  {
  }

  //! Sends `line` privately to the sender, matched by display name. Duplicate nicks -> every matching
  //! player gets it (collision fallback - a chat event carries no UID).
  protected void Reply(string senderName, string line)
  {
    foreach (PlayerBase pb : MatchByName(senderName))
      pb.MessageStatus(line);
  }

  //! Connected players whose display name equals `senderName`. A chat event carries no UID, so routing is by
  //! name; duplicate nicks -> all matches (collision fallback).
  protected array<PlayerBase> MatchByName(string senderName)
  {
    array<PlayerBase> matches = new array<PlayerBase>();
    array<Man> players = new array<Man>();
    GetGame().GetPlayers(players);

    foreach (Man m : players)
    {
      PlayerBase pb = PlayerBase.Cast(m);
      if (pb && pb.GetIdentity() && pb.GetIdentity().GetName() == senderName)
        matches.Insert(pb);
    }

    return matches;
  }
}

#endif
