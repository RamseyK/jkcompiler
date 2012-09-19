program testArrayAssignmentValid;

class testArrayAssignmentValid

BEGIN
   
   VAR XRay : ARRAY[0 .. 4] OF integer;
       YRay : ARRAY[8 .. 12] OF integer;

FUNCTION testArrayAssignmentValid;
BEGIN
   XRay := YRay
END

END
.
