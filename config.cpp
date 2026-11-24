class CfgMods
{
  class CommonMod
  {
    type = "mod";
    dir = "common-mod";
    name = "Common mod - alexzulu's server customization mod";
    version = "0.0.1";
    author = "Paramtamtam";
    credits = "alexzulu";
    hideName = 1;
    hidePicture = 1;
    defines[] = {"ALEXZULU"};
    dependencies[] = {"game", "world", "mission"};

    class defs
    {
      class gameScriptModule
      {
        files[] = {"common-mod/src/3_game"};
      };

      class missionScriptModule
      {
        files[] = {"common-mod/src/5_mission"};
      };
    };
  };
};

class CfgPatches
{
  class AlexZulu
  {
    requiredAddons[] = {"DZ_Data", "DZ_Scripts"};
  };
};
