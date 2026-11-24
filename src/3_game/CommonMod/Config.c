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

//! Per-command configuration blocks.
class CommonMod_CommandsConfig
{
  ref CommonMod_PlayersCommandConfig playersList = new CommonMod_PlayersCommandConfig();
  ref CommonMod_TimeCommandConfig time = new CommonMod_TimeCommandConfig();
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

    CommonMod_Config loaded;
    if (JsonFileLoader<CommonMod_Config>.LoadFile(FILE, loaded, err))
      s_Data = loaded;
    else
      ErrorEx("CommonMod: cannot read config, keeping defaults: " + err);
  }
}

#endif
