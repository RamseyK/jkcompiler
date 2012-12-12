PROGRAM FunctionCallParam;

CLASS FunctionCallParam
BEGIN

FUNCTION DoubleValue(value : integer): integer;
BEGIN
	DoubleValue := 2 * value
END
;

FUNCTION FunctionCallParam;
VAR num : integer;
BEGIN
	num := DoubleValue(69);
	PRINT num
END

END
.