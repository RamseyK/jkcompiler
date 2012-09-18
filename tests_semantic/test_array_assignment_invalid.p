program testArrayAssignmentInvalid;

class testArrayAssignmentInvalid

BEGIN
   
   VAR XRay : ARRAY[0 .. 4] OF integer;
       YRay : ARRAY[8 .. 13] OF integer;

FUNCTION testArrayAssignmentInvalid;
BEGIN
   XRay := YRay
END

END
.
