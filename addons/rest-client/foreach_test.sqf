private _result = ["GET", "http://10.25.2.1/", [["test", "mixed"]], []] call OZK_fnc_createJobSync;

LAST_RESULT = _result;
LAST_RESULT_JSON = call compile _result;

JSON_ARRAY = [LAST_RESULT_JSON, "0.frist.collection"] call OZK_fnc_getJsonElement;

{
	systemChat str ([_x, "value"] call OZK_fnc_getJsonElement);
} foreach (JSON_ARRAY#1);