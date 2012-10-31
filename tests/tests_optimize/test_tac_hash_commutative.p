program testExpressionComplex;

class testExpressionComplex
BEGIN
   
   VAR aa: integer;
	bb: integer;
	cc: integer;
	dd: integer;
       ee: integer;
       ff: integer;

FUNCTION testExpressionComplex;
BEGIN
   aa := 1;
   bb := 2;
   cc := aa + bb;
   dd := bb + aa;
   ee := aa / bb;
   ff := bb / aa
END

END
.

