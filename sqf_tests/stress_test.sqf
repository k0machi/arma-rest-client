COUNTER = 0;
for[{_i = 0}, {_i < 500}, {_i = _i + 1}] do {
	[
		["GET", "https://jsonplaceholder.typicode.com/posts", [], []], 
		{
			COUNTER = COUNTER + 1;
			systemChat (_this#1);
		},
		[]
	] call OZK_fnc_createJobAsync;
};	