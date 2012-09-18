program testClassAssignmentBaseTypeInvalid;

class BB

BEGIN
   VAR row : integer;
END

class AA extends BB

BEGIN
   VAR works : boolean;
END


class testClassAssignmentBaseTypeInvalid

BEGIN
   
   VAR objectA	: AA;
       objectB	: BB;

FUNCTION testClassAssignmentBaseTypeInvalid;
BEGIN
   objectB := objectA
END

END
.
