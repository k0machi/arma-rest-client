#include "..\..\script_macros.hpp"
if (!canSuspend) exitWith { ["Scheduled environment is required for synchronous REST call"] call BIS_fnc_error };

params ["_jobParams", "_callback", "_callbackParams"];
_jobParams params ["_method", "_url", "_query", "_arguments"];

private _result = "";
private _id = -1;

switch (toLower _method) do {
	case "get": {
		_id = ("extension" callExtension ["GETRequest", [_url, _query, _arguments]])#1;
	};
	default {
		[format["Unsupported method: %1", _method]] call BIS_fnc_error;
	};
};

if (_id == -1) exitWith {
	_result = "#REST_CLIENT_SYNC_GENERIC_ERROR";
	[_callbackParams, _result] spawn _callback;
};

waitUntil { ((("extension" callExtension ["CheckJob", [_id]])#1) == JOB_COMPLETE) };

_clientResult = "extension" callExtension ["GetResult", [_id]];
_result = _clientResult#0;
if ((_clientResult#1) == RESULT_SLICED) then {
	while { (_clientResult#1) != RESULT_COMPLETE } do {
		_clientResult = "extension" callExtension ["GetResult", [_id]];
		_result = _result + (_clientResult#0);
	};
};

[_callbackParams, _result] spawn _callback;