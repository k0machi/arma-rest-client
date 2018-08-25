_myLocal = ["Anal"]; //glo

[_myLocal] spawn { //req

	sleep 3;
	_this spawn { //cb
		(_this#0) set[0, "COCKS"];
		hint str _this;
	};
};

systemChat "end";