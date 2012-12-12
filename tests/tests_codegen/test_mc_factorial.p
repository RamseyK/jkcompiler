PROGRAM FunctionCallFactorial;

CLASS FunctionCallFactorial
BEGIN

FUNCTION Factorial(val : integer): integer;
BEGIN
	PRINT val;
	IF val <= 1 THEN
		Factorial := 1
	ELSE
		Factorial := val * Factorial(val-1)
END
;

FUNCTION FunctionCallFactorial;
VAR num : integer;
BEGIN
    num := Factorial(5);
	PRINT num
END

END
.