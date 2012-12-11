PROGRAM FunctionCallNoParam;

CLASS FunctionCallNoParam
BEGIN

FUNCTION DoubleNum : integer;
VAR num2 : integer;
BEGIN
	num2 := 70;
	DoubleNum := num2
END
;

FUNCTION FunctionCallNoParam;
VAR num : integer;
BEGIN
	num := 69;
	PRINT num;
	num := DoubleNum;
	PRINT num
END

END
.