#include "..\..\script_macros.hpp"

private _newJobs = missionNamespace getVariable["KOM_REST_newJobs", []];
private _jobs = missionNamespace getVariable["KOM_REST_currentJobs", []];
private _completedJobs = missionNamespace getVariable["KOM_REST_completedJobs", []];

{
	private _id = ("extension" callExtension[_x#1, [_x#2, _x#3, _x#4]])#1;
	_x set[0, _id];
	_jobs pushBack _x;
	_newJobs set[_foreachIndex, nil];
} foreach _newJobs;


private _results = _jobs apply {
	private _status = "extension" callExtension ["CheckJob", [_x#0]];
	private _status = if ((_status#1) == JOB_COMPLETE) then {
		true;
	} else {
		false;
	};
	_status
};

{
	if (_x) then {
		private _resultArray = [];
		private _curJob = (_jobs#_foreachIndex);
		private _result = "extension" callExtension ["GetResult", [(_curJob#0)]];
		_resultArray pushBack (_result#0);
		if ((_result#1) == RESULT_SLICED) then {
			while { (_result#1) != RESULT_COMPLETE } do {
				_result = "extension" callExtension ["GetResult", [(_curJob#0)]];
				_resultArray pushBack (_result#0);
			}
		};
		_curJob set[5, true];
		_curJob set[6, call compile(_resultArray joinString "")];
		_completedJobs pushBack _curJob;
		_curJob spawn (_curJob#4);
	}
} foreach _results;

{
	if (_x#5) then {
		_jobs set[_foreachIndex, nil];
	}
} foreach _jobs;

missionNamespace setVariable["KOM_REST_currentJobs", _jobs];
missionNamespace setVariable["KOM_REST_completedJobs", _completedJobs];