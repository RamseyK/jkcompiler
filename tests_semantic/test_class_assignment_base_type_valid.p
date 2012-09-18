program testClassAssignmentBaseTypeValid;

class BB

BEGIN
   VAR row : integer;
END


class AA extends BB

BEGIN
   VAR works : boolean;
END


class testClassAssignmentBaseTypeValid

BEGIN
   
   VAR objectA	: AA;
       objectB	: BB;

FUNCTION testClassAssignmentBaseTypeValid;
BEGIN
   objectA := objectB
END

END
.
