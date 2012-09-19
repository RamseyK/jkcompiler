program testFunctionLocalVariableDeclaredBaseClass;



class theBaseClass
BEGIN
   VAR someVar : integer;
END	       




class middleClass extends theBaseClass
BEGIN
   VAR middleman : integer;
END		 




class testFunctionLocalVariableDeclaredBaseClass extends middleClass

BEGIN
   VAR otherVar	: integer;
		

FUNCTION testFunctionLocalVariableDeclaredBaseClass;
BEGIN
   someVar := 5
END

END
.
