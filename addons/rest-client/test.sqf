private _result = ["GET", "http://10.25.2.1/", [["test", "mixed"]], []] call OZK_fnc_createJobSync;

LAST_RESULT = _result;
LAST_RESULT_JSON = call compile _result;

JSON_50_BODY_VALUE = [LAST_RESULT_JSON, "0.frist.collection"] call OZK_fnc_getJsonElement;