#ifdef SERVER

//! Localized strings for the /players command.
class CommonMod_PlayersL10n
{
  //! Lead text before the player list.
  string playersListPrefix = "Сейчас играют";
  //! Reply when the sender is the only one online.
  string youAreAlone       = "Кроме вас на сервере никого нет";
  //! Short summary.
  string description       = "Список игроков онлайн";
}

//! Config block for the /players command.
class CommonMod_PlayersCommandConfig
{
  //! Master command on/off switch.
  bool enabled = true;
  //! Exact word that triggers the command.
  string trigger = "/players";
  //! Extra trigger words (same behavior as `trigger`).
  ref array<string> aliases = new array<string>();
  //! User-facing strings.
  ref CommonMod_PlayersL10n l10n = new CommonMod_PlayersL10n();
}

//! Localized strings for the /time command.
class CommonMod_TimeL10n
{
  //! Lead text before the time value.
  string timePrefix  = "Серверное время";
  //! Short summary.
  string description  = "Текущее игровое время на сервере";
}

//! Config block for the /time command.
class CommonMod_TimeCommandConfig
{
  //! Master command on/off switch.
  bool enabled = true;
  //! Exact word that triggers the command.
  string trigger = "/time";
  //! Extra trigger words (same behavior as `trigger`).
  ref array<string> aliases = new array<string>();
  //! User-facing strings.
  ref CommonMod_TimeL10n l10n = new CommonMod_TimeL10n();
}

//! Localized strings for the /ping command.
class CommonMod_PingL10n
{
  //! Lead text before the ping values.
  string pingPrefix    = "Ваш пинг";
  //! Label for the current (last-measured) value.
  string currentLabel  = "текущий";
  //! Label for the average value.
  string averageLabel  = "средний";
  //! Unit suffix appended to each millisecond value.
  string unit          = "мс";
  //! Short summary.
  string description    = "Ваш пинг до сервера";
}

//! Config block for the /ping command.
class CommonMod_PingCommandConfig
{
  //! Master command on/off switch.
  bool enabled = true;
  //! Exact word that triggers the command.
  string trigger = "/ping";
  //! Extra trigger words (same behavior as `trigger`).
  ref array<string> aliases = new array<string>();
  //! User-facing strings.
  ref CommonMod_PingL10n l10n = new CommonMod_PingL10n();
}

//! Localized strings for the /rules command.
class CommonMod_ServerRulesCommandL10n
{
  //! Short summary.
  string description = "Показать правила сервера";
}

//! Config block for the /rules command.
class CommonMod_ServerRulesCommandConfig
{
  //! Master command on/off switch.
  bool enabled = true;
  //! Exact word that triggers the command.
  string trigger = "/rules";
  //! Extra trigger words (same behavior as `trigger`).
  ref array<string> aliases = new array<string>();
  //! User-facing strings.
  ref CommonMod_ServerRulesCommandL10n l10n = new CommonMod_ServerRulesCommandL10n();
}

//! One admin-declared extra command for /help - a command from ANOTHER mod, listed so players see it too.
class CommonMod_HelpExtraCommand
{
  //! Trigger/name as players type it.
  string name = "";
  //! One-line summary.
  string description = "";
}

//! Localized strings for the /help command.
class CommonMod_HelpL10n
{
  //! Header line shown before the command list.
  string listPrefix = "Доступные команды";
  //! Short summary (this command's own line in the list).
  string description = "Список доступных команд";
}

//! Config block for the /help command.
class CommonMod_HelpCommandConfig
{
  //! Master command on/off switch.
  bool enabled = true;
  //! Exact word that triggers the command.
  string trigger = "/help";
  //! Extra trigger words (same behavior as `trigger`).
  ref array<string> aliases = {"/commands", "/h", "/usage"};
  //! Admin-declared extra entries (commands from other mods) appended to the built-in list.
  ref array<ref CommonMod_HelpExtraCommand> extraCommands = new array<ref CommonMod_HelpExtraCommand>();
  //! User-facing strings.
  ref CommonMod_HelpL10n l10n = new CommonMod_HelpL10n();
}

//! Per-command configuration blocks.
class CommonMod_CommandsConfig
{
  ref CommonMod_PlayersCommandConfig playersList = new CommonMod_PlayersCommandConfig();
  ref CommonMod_TimeCommandConfig time = new CommonMod_TimeCommandConfig();
  ref CommonMod_PingCommandConfig ping = new CommonMod_PingCommandConfig();
  ref CommonMod_ServerRulesCommandConfig serverRules = new CommonMod_ServerRulesCommandConfig();
  ref CommonMod_HelpCommandConfig help = new CommonMod_HelpCommandConfig();
}

//! Localized strings for the PvP-zone warning.
class CommonMod_PvpZoneL10n
{
  //! Red chat line on crossing INTO the zone (shown once on entry).
  string chatEnterText = "Вы вошли в PvP-зону - здесь разрешены убийства игроков";
  //! Red chat line repeated while staying inside the zone.
  string chatText      = "ВНИМАНИЕ! Вы в находитесь PvP-зоне - убийства других игроков здесь разрешены";
  //! Yellow chat line on crossing OUT of the zone.
  string chatExitText  = "Вы покинули PvP-зону";
  //! Repeated toast title.
  string toastTitle    = "PvP ZONE";
  //! Repeated toast body.
  string toastBody     = "Вы в находитесь PvP-зоне - убийства других игроков здесь разрешены";
}

//! Config block for the PvP-zone warning.
class CommonMod_PvpZoneConfig
{
  //! Master on/off switch for the whole feature.
  bool enabled = true;
  //! Send the chat lines (enter / ongoing / exit) to players at the zone.
  bool showChatWarning = true;
  //! Show the toast notification to players inside the zone.
  bool showToastWarning = true;
  //! How often each player's position is checked against the zone. Only the check cadence - does
  //! NOT affect how often alerts fire.
  int  checkIntervalSeconds = 1;
  //! Minimum delay between repeated chat/toast alerts while a player stays inside. 33 = chat line's
  //! full on-screen life (ChatLine FADE_TIMEOUT 30s + FADE_OUT 3s), so the next line fires right as
  //! the previous one finishes fading out.
  int  intervalBetweenRepeatsSeconds = 33;
  //! Toast display duration (seconds); has no effect on the chat line.
  int  showToastTimeSeconds = 10;
  //! User-facing strings.
  ref CommonMod_PvpZoneL10n l10n = new CommonMod_PvpZoneL10n();
}

//! Config block for showing the server rules on player join.
class CommonMod_ShowServerRulesOnJoinConfig
{
  //! Master on/off switch.
  bool enabled = true;
  //! Delay before the first show - player is still loading right after connect.
  int  initialDelaySeconds = 5;
  //! Total number of times the rules are shown per join.
  int  repeatCount = 3;
  //! Delay between consecutive shows.
  int  intervalBetweenRepeatsSeconds = 60;
}

//! Root mod configuration, read from JSON on mission init.
class CommonMod_Config
{
  //! Directory (under the server -profiles dir) holding the config file.
  static const string DIR  = "$profile:common-mod";
  //! Full path of the JSON config file.
  static const string FILE = "$profile:common-mod/config.json";
  //! Loaded singleton; null until Load() runs.
  static ref CommonMod_Config s_Data;

  //! All command configuration.
  ref CommonMod_CommandsConfig commands = new CommonMod_CommandsConfig();

  //! PvP-zone warning configuration.
  ref CommonMod_PvpZoneConfig pvpZone = new CommonMod_PvpZoneConfig();

  //! Server rules body - one chat line per element. Single source shared by the /rules command and the
  //! on-join show.
  ref array<string> serverRules = {
    "Правила сервера: http://dayz.alexzulu.ru",
    "Telegram: https://t.me/alexzdayzservers",
    "• Сервер является PvE.",
    //"• Единственная PVP-зона - Авиационная база \"Выбор\" (NWAF).", // for chernarus only
    //"• Вне PvP-зоны убийства игроков запрещены.", // for servers with PvP zones
    //"• Имущество игроков вне PvP-зоны неприкосновенно." // for servers with PvP zones
  };

  //! Show-rules-on-join configuration (timing/toggle only; text is `serverRules`).
  ref CommonMod_ShowServerRulesOnJoinConfig showServerRulesOnJoin = new CommonMod_ShowServerRulesOnJoinConfig();

  //! Returns the loaded config, loading it on first access.
  static CommonMod_Config Get()
  {
    if (!s_Data)
      Load();

    return s_Data;
  }

  //! (Re)loads config - writes defaults if the file is missing, keeps defaults on a parse error.
  static void Load()
  {
    s_Data = new CommonMod_Config();
    MakeDirectory(DIR);

    string err;
    if (!FileExist(FILE))
    {
      if (!JsonFileLoader<CommonMod_Config>.SaveFile(FILE, s_Data, err))
        ErrorEx("CommonMod: cannot write default config: " + err);

      return;
    }

    // pre-seed with a defaults instance - the JSON serializer overlays ONLY the keys present in the file and
    // leaves the rest at these class defaults.
    CommonMod_Config loaded = new CommonMod_Config();
    if (JsonFileLoader<CommonMod_Config>.LoadFile(FILE, loaded, err))
    {
      s_Data = loaded;

      // self-healing - re-save the merged structure so fields added in a newer mod version get written into an
      // older on-disk config (missing keys load as their class defaults; existing values are preserved)
      if (!JsonFileLoader<CommonMod_Config>.SaveFile(FILE, s_Data, err))
        ErrorEx("CommonMod: cannot rewrite config after load: " + err);
    }
    else
      ErrorEx("CommonMod: cannot read config, keeping defaults: " + err);
  }
}

#endif
