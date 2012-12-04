PROGRAM FunctionCallParam;

CLASS FunctionCallParam
BEGIN
	VAR num : integer;
	    abcdInstance: Abcd;
	    blah : boolean;

FUNCTION DoubleValue(value : integer; blahVal : Abcd): integer;
BEGIN
	DoubleValue := 2 * value;
	blah := blahVal.compilerWorks
END
;

FUNCTION FunctionCallParam;
BEGIN
	abcdInstance := new Abcd;
	abcdInstance.compilerWorks := True;
	num := DoubleValue(68+1, abcdInstance);
	PRINT num
END

END

CLASS Abcd
BEGIN
	VAR compilerWorks : boolean;
END
.