PROGRAM FunctionCallFactorial;

CLASS FunctionCallFactorial
BEGIN

FUNCTION Factorial(val : integer): integer;
VAR aa : integer;
BEGIN
	PRINT val;
	aa := val - 1;
	IF val <= 1 THEN
		Factorial := 1
	ELSE
		Factorial := val * Factorial(aa)
END
;

FUNCTION FunctionCallFactorial;
VAR num : integer;
BEGIN
    num := Factorial(3);
	PRINT num
END

END
.