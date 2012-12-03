PROGRAM FunctionCallNoParam;

CLASS FunctionCallNoParam
BEGIN
	VAR num : integer;

FUNCTION DoubleNum : integer;
BEGIN
	num := 2 * num;
	DoubleNum := num
END
;

FUNCTION FunctionCallNoParam;
BEGIN
	num := 69;
	num := DoubleNum;
	PRINT num
END

END
.