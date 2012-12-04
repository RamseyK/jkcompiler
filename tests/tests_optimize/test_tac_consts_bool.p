program testConstsBool;

class testConstsBool

BEGIN
   VAR aa, bb, ee, xx, yy, zz: boolean;
	cc, dd: integer;

FUNCTION testConstsBool;
BEGIN
	aa := 7 > 5;
	bb := 5 = 5;
	cc := 5;
	dd := 6;
	xx := aa AND bb;
	yy := aa = bb;
	zz := cc > dd;
	ee := True
END

END
.

