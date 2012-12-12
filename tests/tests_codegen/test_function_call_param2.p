PROGRAM FunctionCallParam;

CLASS FunctionCallParam
BEGIN
VAR aa : integer;

FUNCTION DoubleValue(value, value2 : integer): integer;
BEGIN
	DoubleValue := 2 * value + value2 + aa
END
;

FUNCTION FunctionCallParam;
VAR num, num2 : integer;
BEGIN
    aa := 3;
    num2 := 6;
	num := DoubleValue(1, num2);
	PRINT num
END

END
.