_myArray = [1,2,3];

[_myArray] spawn {
	(_this#0) set[1,666];
};	
sleep 5;
hint str _myArray;