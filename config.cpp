class CfgMods
{
  class StateZ
  {
    type = "mod";
    dir = "common-mod";
    name = "Common mod - alexzulu's server customization mod";
    version = "0.0.1";
    author = "tarampampam";
    credits = "alexzulu";
    hideName = 1;
    hidePicture = 1;
    defines[] = {"ALEXZULU"};
    dependencies[] = {"game", "world", "mission"};

    class defs
    {
//      class gameScriptModule
//      {
//        files[] = {"statez/scripts/3_game"};
//      };
//
//      class worldScriptModule
//      {
//        files[] = { "statez/scripts/4_world" };
//      };
//
//      class missionScriptModule
//      {
//        files[] = { "statez/scripts/5_mission" };
//      };
    };
  };
};

class CfgPatches
{
  class AlexZulu
  {
//    requiredAddons[] = {
//      "DZ_Data",
//      "DZ_Scripts",
//    };
  };
};
