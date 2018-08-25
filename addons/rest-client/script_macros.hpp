//Extension status codes
#define CALL_E_GENERIC 0xff //Generic error

#define	JOB_COMPLETE 0x10 //Job has been completed
#define	JOB_INCOMPLETE 0x11 //Job is not complete
#define	RESULT_COMPLETE 0x20 //Result is complete, proceed to parsing
#define	RESULT_SLICED 0x22 //A chunk of a large result was given, will give remaining chunks on subsequent calls
#define	RESULT_NOTREADY 0x2f //Result not ready, e.g. Job not complete
#define	DEBUG_INFO_DUMP 0xff //Echo input into logfile