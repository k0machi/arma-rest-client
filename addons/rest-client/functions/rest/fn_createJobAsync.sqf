params ["_method", "_url", "_query", "_arguments", "_callback"];

private _jobQueue = missionNamespace getVariable["KOM_REST_newJobs", []];
private _index = -1;
switch (toLower _method) do {
	case "get": {
		_index = _jobQueue pushBack [-1, "GETRequest", _url, _query, _arguments, _callback];
	};
	default {
		[format["Unsupported method: %1", _method]] call BIS_fnc_error;
	};
};

if (_index == -1) exitWith {false};
missionNamespace setVariable["KOM_REST_newJobs", _jobQueue];
true