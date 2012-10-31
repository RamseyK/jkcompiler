program testExpressionComplex;

class testExpressionComplex
BEGIN
   
   VAR dd: integer;
	cc: integer;
	aa, bb: integer;

FUNCTION testExpressionComplex;
BEGIN
   dd := 16 / 2 / 2;
   cc := 16 / 2;
   aa := 0;

   if aa = 0 THEN
      bb := 1
   ELSE
      bb := 0
   ;
   cc := 0;
   
   WHILE cc < 7 DO
      cc := cc + 1
END

END
.

