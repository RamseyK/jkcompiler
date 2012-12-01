PROGRAM FunctionCallParam;

CLASS FunctionCallParam
BEGIN
	VAR num : integer;

FUNCTION DoubleValue(value : integer): integer;
BEGIN
	DoubleValue := 2 * value
END
;

FUNCTION FunctionCallParam;
BEGIN
	num := DoubleValue(69);
	PRINT num
END

END
.