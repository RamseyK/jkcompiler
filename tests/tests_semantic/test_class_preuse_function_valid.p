program testClassPreUseFunctionValid;

class testClassPreUseFunctionValid

BEGIN

   VAR works  : boolean;
      objectB : classBB;
      value   : integer;

FUNCTION testClassPreUseFunctionValid;

BEGIN
   value := objectB.getCounter(7)
END

END	      


class classBB

BEGIN
   VAR counter: integer;

FUNCTION getCounter(value: integer): integer;
BEGIN
   counter := value;
   getCounter := 3
END

END

.
