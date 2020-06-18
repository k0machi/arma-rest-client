private _myArray = [];

[
	["GET", "http://10.25.2.1/", [["test", "mixed"]], []], 
	_myArray
] spawn OZK_fnc_createJobSync;

sleep 15;
hint str _myArray;