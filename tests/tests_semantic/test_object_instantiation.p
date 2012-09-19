program testObjectInstantiation;

class AA

BEGIN
   VAR works : integer;
END

class testObjectInstantiation

BEGIN
   
   VAR objectAA : AA;

FUNCTION testObjectInstantiation;
BEGIN
   objectAA := new AA
END

END
.
