program testClassPreUseAssignmentValid;

class testClassPreUseAssignmentValid

BEGIN

   VAR works  : boolean;
      objectB : classBB;
      value   : integer;

FUNCTION testClassPreUseAssignmentValid;

BEGIN
   value := objectB.counter
END

END	      


class classBB

BEGIN
   VAR counter: integer;
END

.
