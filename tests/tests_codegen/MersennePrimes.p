PROGRAM MersennePrimes;

CLASS MersennePrimes
BEGIN

VAR number, divisor, remainder : integer;
    prime : boolean;

FUNCTION MersennePrimes;
BEGIN
	number := 3;
	WHILE number < 1048576 DO
	BEGIN
		divisor := 2;
		prime := TRUE;
		WHILE divisor * divisor < number DO
		BEGIN
			remainder := number MOD divisor;
			IF remainder = 0 THEN
				prime := FALSE
			ELSE
				prime := prime;
			divisor := divisor + 1
		END;

		IF prime THEN
			PRINT number
		ELSE
			number := number;

		number := 2 * number + 1
	END
END

END
.
