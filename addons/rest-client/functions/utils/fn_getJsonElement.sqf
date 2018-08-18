params["_json", "_jsonPath"];
scopeName "top";

#define ARRAY_OOB 0x100
#define OBJECT_KEY_NOT_FOUND 0x200

private _errno = -1;
private _errmsg = "";


private _pathArray = _jsonPath splitString "."; //0.name => 0, name
private _pathDepth = count _pathArray;

private _currentPathDepth = 0;
private _currentNode = _json;

private _found = false;
private _finalNode = [];

while { !_found } do {

	switch (_currentNode#0) do {
		case "a": {
			private _index = parseNumber (_pathArray#_currentPathDepth);
			private _curNodeCount = count (_currentNode#1);
			if (_index > (_curNodeCount - 1)) then {
				_errno = ARRAY_OOB;
				_errmsg = format["Array index out of bounds %1", _index];
				breakTo "top" 
			};
			_currentNode = (_currentNode#1)#_index;
			_currentPathDepth = _currentPathDepth + 1;

		};
		case "o": {
			private _elemTotal = count (_currentNode#1);
			private _pathKeyName = _pathArray#_currentPathDepth;
			private _object = _currentNode#1;
			private _keyIndex = -1;
			_keyIndex = call {
				scopeName "objKeySearch";
				for[{_i = 0}, {_i < _elemTotal}, {_i = _i + 1}] do {
					private _node = _object#_i;
					if ((_node#0) == _pathKeyName) then { _i breakOut "objKeySearch" };
					if (_i == (_elemTotal - 1)) then { -1 breakOut "objKeySearch" };
				}; 
			};

			if (_keyIndex == -1) then { 
				_errno = OBJECT_KEY_NOT_FOUND;
				_errmsg = format["Key %1 not found in the object", _pathKeyName];
				breakTo "top"; 
			};
			_currentNode = ((_currentNode#1)#_keyIndex)#1;
			_currentPathDepth = _currentPathDepth + 1;

		};
	};

	if ((_pathDepth == _currentPathDepth) && _errno == -1) then {
		_found = true;
		_finalNode = _currentNode;

	} else {

	};

};

if (_errno != -1) exitWith {
	_errmsg;
};

hint _errmsg;
_finalNode;