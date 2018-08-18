private _result = ["GET", "http://10.25.2.1/", [["test", "object"]], []] call OZK_fnc_createJobSync;

LAST_RESULT = _result;
hint LAST_RESULT;