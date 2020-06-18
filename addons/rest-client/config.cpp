class CfgPatches
{
    class ozk_rest_client 
    {
        name = "Arma 3 RESTful Client";
        url = "http://gitlab.onozuka.info/komachi/arma-rest-client";
        author = "Alice, Komachi";
        requiredVersion = 1.84;
        requiredAddons[] = {};
        units[] = {};
        weapons[] = {};
    }
}

class CfgFunctions
{
    class Onozuka
    {
        tag = "OZK";
        class REST
        {
            file = "ozk\addons\rest-client\functions\rest";
            class createJobAsync {};
            class createJobSync {};
            class createJobSyncSimple {};
        };
        class utils
        {
            file = "ozk\addons\rest-client\functions\utils";
            class getJsonElement {};
        }
    }
}